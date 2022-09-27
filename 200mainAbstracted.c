
/* This code is sponsored by Ben (Churner) Turner and Riaz (Smelly) Kelly */


/* On macOS, compile with...
    clang 200mainAbstracted.c 040pixel.o -lglfw -framework OpenGL -framework Cocoa -framework IOKit
On Ubuntu, compile with...
    cc 160mainAbstracted.c 040pixel.o -lglfw -lGL -lm -ldl
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GLFW/glfw3.h>

#include "040pixel.h"

#include "080vector.c"
#include "100matrix.c"
#include "150texture.c"
#include "200shading.c"

/* It is important that shadeFragment correctly parse the data that we give it.
To help avoid errors in communication, we set up some constants. Notice that the
documentation for the new triRender requires ATTRX to be 0 and ATTRY to be 1. */
#define ATTRX 0
#define ATTRY 1
#define ATTRS 2
#define ATTRT 3
#define ATTRR 4
#define ATTRG 5
#define ATTRB 6

#define VARYX 0
#define VARYY 1
#define VARYS 2
#define VARYT 3
#define VARYR 4
#define VARYG 5
#define VARYB 6

#define UNIFR 0
#define UNIFG 1
#define UNIFB 2
#define UNIFROT 3
#define UNIFTRANSLX 4
#define UNIFTRANSLY 5

#define TEXR 0
#define TEXG 1
#define TEXB 2

/* attr has already been interpolated from the vertex attributes. tex is an
array of texNum elements, each of which is of type texTexture *. rgb is the
output parameter. The other parameters should be self-explanatory. This function
is not allowed to read from any variables other than its arguments and local
variables. It is not allowed to write to any variables except for its rgb
parameter and local variables. In particular, it cannot use any global
variables. */
void shadeFragment(
        int unifDim, const double unif[], int texNum, const texTexture *tex[],
        int varyDim, const double vary[], double rgb[3]) {

        double sample[varyDim];
        double unifRGB[3] = {unif[UNIFR], unif[UNIFG], unif[UNIFB]};

        texSample(*tex, vary[VARYS], vary[VARYT], sample);
        vecModulate(varyDim, sample, unifRGB, rgb);

}

void shadeVertex(
int unifDim, const double unif[], int attrDim, const double attr[],
int varyDim, double vary[]) {

        /* Code for rotating and translating */
        double rot[2][2];
        mat22Rotation(unif[UNIFROT], rot);
        mat221Multiply(rot, attr, vary);
        vary[VARYX] += unif[UNIFTRANSLX];
        vary[VARYY] += unif[UNIFTRANSLY];
        vary[VARYS] = attr[ATTRS];
        vary[VARYT] = attr[ATTRT];
}

/* We have to include triangle.c after defining shadeFragment, because triRender
refers to shadeFragment. (Later in the course we handle this issue better.) */
#include "200triangle.c"

#include "200mesh.c"

#include "190mesh2D.c"

meshMesh mesh;

/* This struct is initialized in main() below. */
shaShading sha;

/* Here we make an array of one texTexture pointer, in such a way that the
const qualifier can be enforced throughout the surrounding code. C is confusing
for stuff like this. Don't worry about mastering C at this level. It doesn't
come up much in our course. */
texTexture texture;
const texTexture *textures[1] = {&texture};
const texTexture **tex = textures;

double unif[6] = {1.0, 1.0, 1.0, 0.0, -120.0, -120.0};

void render(void) {
	pixClearRGB(0.0, 0.0, 0.0);
	meshRender(&mesh, &sha, unif, tex);
}

void handleKeyUp(int key, int shiftIsDown, int controlIsDown,
		int altOptionIsDown, int superCommandIsDown) {
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

    /* Animation that rotates about the origin while slowly moving it towards the top right */
    double timePassed, xTranslate, yTranslate, rotation;
    timePassed = (newTime - oldTime);
    xTranslate = timePassed * 10;
    yTranslate = timePassed * 10;
    rotation = timePassed;
    unif[UNIFTRANSLX] = 256.0 + cos(newTime) * 128.0;
    unif[UNIFTRANSLY] = 256.0 + sin(newTime) * 128.0;
    unif[UNIFROT] += (newTime - oldTime) * 3.0;
    // unif[UNIFROT] += rotation;
    // unif[UNIFTRANSLX] += xTranslate;
    // unif[UNIFTRANSLY] += yTranslate;
    render();
}

int main(void) {
	if (pixInitialize(512, 512, "Abstracted") != 0)
		return 1;
	if (texInitializeFile(&texture, "messi.jpeg") != 0) {
	    pixFinalize();
		return 2;
	}
    if (mesh2DInitializeRectangle(&mesh, 100.0, 200.0, 100.0, 200.0) != 0) {
        pixFinalize();
        return 3;
    }
    texSetFiltering(&texture, texNEAREST);
    texSetLeftRight(&texture, texREPEAT);
    texSetTopBottom(&texture, texREPEAT);
    sha.unifDim = 3 + 3;
    sha.attrDim = 2 + 2;
    sha.texNum = 1;
    sha.varyDim = 2 + 2;
    render();
    pixSetKeyUpHandler(handleKeyUp);
    pixSetTimeStepHandler(handleTimeStep);
    pixRun();
    meshFinalize(&mesh);
    texFinalize(&texture);
    pixFinalize();
    return 0;
}
