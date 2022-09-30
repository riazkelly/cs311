
/* This code is brought to you by Ben Turner and Riaz Kelly */

/* On macOS, compile with...
    clang 240mainShadings.c 040pixel.o -lglfw -framework OpenGL -framework Cocoa -framework IOKit
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

/* Code for shading the messi rectangle mesh */

void shadeVertexMessi(
        int unifDim, const double unif[], int attrDim, const double attrHomog[],
        int varyDim, double varyHomog[]) {
    mat331Multiply((double(*)[3])(&unif[UNIFMODELING]), attrHomog, varyHomog);
    varyHomog[VARYS] = attrHomog[ATTRS];
    varyHomog[VARYT] = attrHomog[ATTRT];
}

void shadeFragmentMessi(
        int unifDim, const double unif[], int texNum, const texTexture *tex[],
        int varyDim, const double vary[], double rgb[3]) {
    double sample[tex[0]->texelDim];
    texSample(tex[0], vary[VARYS], vary[VARYT], sample);
    vecModulate(3, sample, &unif[UNIFR], rgb);
}

/* Code for shading the goal rectangle mesh */

void shadeVertexGoal(
        int unifDim, const double unif[], int attrDim, const double attrHomog[],
        int varyDim, double varyHomog[]) {
    mat331Multiply((double(*)[3])(&unif[UNIFMODELING]), attrHomog, varyHomog);
    varyHomog[VARYS] = attrHomog[ATTRS];
    varyHomog[VARYT] = attrHomog[ATTRT];
}

void shadeFragmentGoal(
        int unifDim, const double unif[], int texNum, const texTexture *tex[],
        int varyDim, const double vary[], double rgb[3]) {
    double sample[tex[1]->texelDim];
    texSample(tex[1], vary[VARYS], vary[VARYT], sample);
    vecModulate(3, sample, &unif[UNIFR], rgb);
}

/* Code for shading the ball ellipse mesh */

void shadeVertexBall(
        int unifDim, const double unif[], int attrDim, const double attrHomog[],
        int varyDim, double varyHomog[]) {

    double attrHomogenous[3] = {attrHomog[ATTRX], attrHomog[ATTRY], 1};
    mat331Multiply((double(*)[3])(&unif[UNIFMODELING]), attrHomogenous, varyHomog);
    varyHomog[VARYS] = attrHomog[ATTRS];
    varyHomog[VARYT] = attrHomog[ATTRT];
}

void shadeFragmentBall(
        int unifDim, const double unif[], int texNum, const texTexture *tex[],
        int varyDim, const double vary[], double rgb[3]) {
    double sample[tex[2]->texelDim];
    texSample(tex[2], vary[VARYS], vary[VARYT], sample);
    vecModulate(3, sample, &unif[UNIFR], rgb);
}

shaShading shaMessi;
shaShading shaGoal;
shaShading shaBall;

texTexture texMessi;
texTexture texGoal;
texTexture texBall;
const texTexture *textures[3] = {&texMessi, &texGoal, &texBall};
const texTexture **tex = textures;

meshMesh meshMessi;
meshMesh meshGoal;
meshMesh meshBall;
double unifBall[3 + 9] = {1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
double unifGoal[3 + 9] = {1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
double unifMessi[3 + 9] = {1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
double rotationAngle = 0, translationVector[2] = {0.0,0.0}, translateZero[2] = {0.0, 0.0}, rotateZero = 0.0;

void render(void) {
    pixClearRGB(0.0, 0.0, 0.0);
    meshRender(&meshMessi, &shaMessi, unifMessi, tex);
    meshRender(&meshGoal, &shaGoal, unifGoal, tex);
    meshRender(&meshBall, &shaBall, unifBall, tex);
}

void handleKeyUp(
        int key, int shiftIsDown, int controlIsDown, int altOptionIsDown,
        int superCommandIsDown) {
    if (key == GLFW_KEY_ENTER) {

        if (texMessi.filtering == texLINEAR) {

            texSetFiltering(&texMessi, texNEAREST);
            texSetFiltering(&texGoal, texNEAREST);
            texSetFiltering(&texBall, texNEAREST);
        }
        else {

            texSetFiltering(&texMessi, texLINEAR);
            texSetFiltering(&texGoal, texLINEAR);
            texSetFiltering(&texBall, texLINEAR);
        }
        render();
    }
}

void handleTimeStep(double oldTime, double newTime) {
    if (floor(newTime) - floor(oldTime) >= 1.0)
        printf("handleTimeStep: %f frames/sec\n", 1.0 / (newTime - oldTime));

    /* Code that keeps messi and the goal at the same place while moving the ball
    back and forth between messi and the cross bar of the goal */

    double isomMessi[3][3], isomGoal[3][3], isomBall[3][3];
    translationVector[0] = 300.0;
    translationVector[1] = 260 + sin(newTime) * 220;
    rotationAngle = 0;
    mat33Isometry(rotationAngle, translationVector, isomBall);
    vecCopy(9, (double *)isomBall, &unifBall[UNIFMODELING]);

    mat33Isometry(rotateZero, translateZero, isomMessi);
    vecCopy(9, (double *)isomMessi, &unifMessi[UNIFMODELING]);

    mat33Isometry(rotateZero, translateZero, isomGoal);
    vecCopy(9, (double *)isomGoal, &unifGoal[UNIFMODELING]);
    render();
}

int main(void) {
    if (pixInitialize(512, 512, "Shader Program") != 0)
        return 1;
    if (texInitializeFile(&texMessi, "messiKicking.jpeg") != 0) {
        pixFinalize();
        return 6;
    }
    if (texInitializeFile(&texGoal, "soccerGoal.jpeg") != 0) {
        pixFinalize();
        return 7;
    }
    if (texInitializeFile(&texBall, "soccerBall.jpeg") != 0) {
        pixFinalize();
        return 2;
    }
    if (mesh2DInitializeEllipse(&meshBall, 0.0, 0.0, 10.0, 10.0, 40) != 0) {
        texFinalize(&texBall);
        pixFinalize();
        return 3;
    }
    if (mesh2DInitializeRectangle(&meshMessi, 100.0, 412.0, 0.0, 128.0) != 0) {
        texFinalize(&texMessi);
        pixFinalize();
        return 4;
    }
    if (mesh2DInitializeRectangle(&meshGoal, 150.0, 362.0, 412.0, 512.0) != 0) {
        texFinalize(&texGoal);
        pixFinalize();
        return 5;
    }
    texSetFiltering(&texMessi, texNEAREST);
    texSetLeftRight(&texMessi, texREPEAT);
    texSetTopBottom(&texMessi, texREPEAT);

    texSetFiltering(&texGoal, texNEAREST);
    texSetLeftRight(&texGoal, texREPEAT);
    texSetTopBottom(&texGoal, texREPEAT);

    texSetFiltering(&texBall, texNEAREST);
    texSetLeftRight(&texBall, texREPEAT);
    texSetTopBottom(&texBall, texREPEAT);
    /* New! The shader program now records which shader functions to use. */
    shaGoal.unifDim = 3 + 9;
    shaGoal.attrDim = 2 + 2;
    shaGoal.varyDim = 2 + 2;
    shaGoal.texNum = 1;
    shaGoal.shadeVertex = shadeVertexGoal;
    shaGoal.shadeFragment = shadeFragmentGoal;

    shaMessi.unifDim = 3 + 9;
    shaMessi.attrDim = 2 + 2;
    shaMessi.varyDim = 2 + 2;
    shaMessi.texNum = 1;
    shaMessi.shadeVertex = shadeVertexMessi;
    shaMessi.shadeFragment = shadeFragmentMessi;

    shaBall.unifDim = 3 + 9;
    shaBall.attrDim = 2 + 2;
    shaBall.varyDim = 2 + 2;
    shaBall.texNum = 1;
    shaBall.shadeVertex = shadeVertexBall;
    shaBall.shadeFragment = shadeFragmentBall;

    render();
    pixSetKeyUpHandler(handleKeyUp);
    pixSetTimeStepHandler(handleTimeStep);
    pixRun();
    meshFinalize(&meshMessi);
    meshFinalize(&meshGoal);
    meshFinalize(&meshBall);
    texFinalize(&texMessi);
    texFinalize(&texGoal);
    texFinalize(&texBall);
    pixFinalize();
    return 0;
}
