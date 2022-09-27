
/* This code is sponsored by Ben (Churner) Turner and Riaz (Smelly) Kelly */

/* On macOS, compile with...
    clang 190mainMesh.c 040pixel.o -lglfw -framework OpenGL -framework Cocoa -framework IOKit
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
#include "170shading.c"

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
#define UNIFR 0
#define UNIFG 1
#define UNIFB 2
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
        int attrDim, const double attr[], double rgb[3]) {

        double sample[attrDim];
        // double attrRGB[3] = {attr[ATTRR], attr[ATTRG], attr[ATTRB]};
        double unifRGB[3] = {unif[UNIFR], unif[UNIFG], unif[UNIFB]};

        texSample(*tex, attr[ATTRS], attr[ATTRT], sample);
        // vecModulate(attrDim, sample, attrRGB, sample);
        vecModulate(attrDim, sample, unifRGB, rgb);

}

/* We have to include triangle.c after defining shadeFragment, because triRender
refers to shadeFragment. (Later in the course we handle this issue better.) */
#include "170triangle.c"

#include "190mesh.c"

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

void render(void) {
	pixClearRGB(0.0, 0.0, 0.0);
	// double a[4] = {400.0, 100.0, 1.0, 1.0};
	// double b[4] = {500.0, 500.0, 0.0, 1.0};
	// double c[4] = {30.0, 30.0, 0.0, 0.0};
    // double d[4] = {30.0, 500.0, 0.0, 0.0};
	double unif[3] = {1.0, 1.0, 1.0};
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
}

int main(void) {
	if (pixInitialize(512, 512, "Abstracted") != 0)
		return 1;
	if (texInitializeFile(&texture, "messi.jpeg") != 0) {
	    pixFinalize();
		return 2;
	}
    if (mesh2DInitializeRectangle(&mesh, 10.0, 500.0, 200.0, 500.0) != 0) {
        pixFinalize();
        return 3;
    }
    texSetFiltering(&texture, texNEAREST);
    texSetLeftRight(&texture, texREPEAT);
    texSetTopBottom(&texture, texREPEAT);
    sha.unifDim = 3;
    sha.attrDim = 2 + 2;
    sha.texNum = 1;
    render();
    pixSetKeyUpHandler(handleKeyUp);
    pixSetTimeStepHandler(handleTimeStep);
    pixRun();
    meshFinalize(&mesh);
    texFinalize(&texture);
    pixFinalize();
    return 0;
}
