
/* This code is brought to you by Ben Turner and Riaz Kelly */

/* On macOS, compile with...
    clang 230mainHomogenous.c 040pixel.o -lglfw -framework OpenGL -framework Cocoa -framework IOKit
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GLFW/glfw3.h>

#include "040pixel.h"

#include "080vector.c"
#include "230matrix.c"
#include "150texture.c"
#include "220shading.c"
/* New! We no longer need to include these files after shadeFragment and
shadeVertex. So instead we include them up here. It's good C style to have all
#includes in one section near the top of the file. */
#include "220triangle.c"
#include "220mesh.c"
#include "190mesh2D.c"

#define ATTRX 0
#define ATTRY 1
#define ATTRS 2
#define ATTRT 3
#define VARYX 0
#define VARYY 1
#define VARYS 2
#define VARYT 3
#define UNIFR 0
#define UNIFG 1
#define UNIFB 2
#define UNIFMODELING 3
#define TEXR 0
#define TEXG 1
#define TEXB 2

void shadeVertex(
        int unifDim, const double unif[], int attrDim, const double attr[],
        int varyDim, double varyHomog[]) {
    double attrHomog[3] = {attr[ATTRX], attr[ATTRY], 1};
    mat331Multiply((double(*)[3])(&unif[UNIFMODELING]), attrHomog, varyHomog);
    varyHomog[VARYS] = attr[ATTRS];
    varyHomog[VARYT] = attr[ATTRT];
}

void shadeFragment(
        int unifDim, const double unif[], int texNum, const texTexture *tex[],
        int varyDim, const double vary[], double rgb[3]) {
    double sample[tex[0]->texelDim];
    texSample(tex[0], vary[VARYS], vary[VARYT], sample);
    vecModulate(3, sample, &unif[UNIFR], rgb);
}

shaShading sha;
texTexture texture;
const texTexture *textures[1] = {&texture};
const texTexture **tex = textures;
meshMesh mesh;
double unif[3 + 9] = {1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
double rotationAngle = 0, translationVector[2] = {0.0,0.0};

void render(void) {
    pixClearRGB(0.0, 0.0, 0.0);
    meshRender(&mesh, &sha, unif, tex);
}

void handleKeyUp(
        int key, int shiftIsDown, int controlIsDown, int altOptionIsDown,
        int superCommandIsDown) {
    if (key == GLFW_KEY_ENTER) {
        if (texture.filtering == texLINEAR)
            texSetFiltering(&texture, texNEAREST);
        else
            texSetFiltering(&texture, texLINEAR);
        render();
    }
}

void handleTimeStep(double oldTime, double newTime) {
    if (floor(newTime) - floor(oldTime) >= 1.0)
        printf("handleTimeStep: %f frames/sec\n", 1.0 / (newTime - oldTime));

    double isom[3][3];

    /* Translated in a circular pattern around the middle of the screen */
    translationVector[0] = 256.0 + cos(newTime) * 128.0;
    translationVector[1] = 256.0 + sin(newTime) * 128.0;

    /* Rotates at this speed about the origin */
    rotationAngle += (newTime - oldTime) * 3.0;
    mat33Isometry(rotationAngle, translationVector, isom);
    vecCopy(9, (double *)isom, &unif[UNIFMODELING]);
    render();
}

int main(void) {
    if (pixInitialize(512, 512, "Shader Program") != 0)
        return 1;
    if (texInitializeFile(&texture, "messi.jpeg") != 0) {
        pixFinalize();
        return 2;
    }
    if (mesh2DInitializeEllipse(&mesh, 0.0, 0.0, 256.0, 128.0, 40) != 0) {
        texFinalize(&texture);
        pixFinalize();
        return 3;
    }
    texSetFiltering(&texture, texNEAREST);
    texSetLeftRight(&texture, texREPEAT);
    texSetTopBottom(&texture, texREPEAT);
    /* New! The shader program now records which shader functions to use. */
    sha.unifDim = 3 + 9;
    sha.attrDim = 2 + 2;
    sha.varyDim = 2 + 2;
    sha.texNum = 1;
    sha.shadeVertex = shadeVertex;
    sha.shadeFragment = shadeFragment;
    render();
    pixSetKeyUpHandler(handleKeyUp);
    pixSetTimeStepHandler(handleTimeStep);
    pixRun();
    meshFinalize(&mesh);
    texFinalize(&texture);
    pixFinalize();
    return 0;
}
