
/* This file made possible by the work and effort of both Ben Turner AND Riaz Kelly*/

/* On macOS, compile with...
    clang 150mainTexturizing.c 040pixel.o -lglfw -framework OpenGL -framework Cocoa -framework IOKit
On Ubuntu, compile with...
    cc 100mainTesting.c 040pixel.o -lglfw -lGL -lm -ldl
*/



#include <stdio.h>
#include <math.h>

#include "040pixel.h"

#include "080vector.c"
#include "100matrix.c"
#include "150texture.c"
#include "140triangle.c"

#include <GLFW/glfw3.h>

double a[2] = {44.0, 56.0};
double b[2] = {244.0, 44.0};
double c[2] = {426.0, 450.0};
double rgb[3] = {1.0, 1.0, 0.0};
texTexture tex;
int isNearestNeighbor = 1;

/* This function is a user interface 'callback'. Once we register it with the
user interface infrastructure, it is called whenever the user releases a
keyboard key. For details, see the pixSetKeyUpHandler documentation. */
void handleKeyUp(int key, int shiftIsDown, int controlIsDown,
        int altOptionIsDown, int superCommandIsDown) {
    printf("handleKeyUp: %d, shift %d, control %d, altOpt %d, supComm %d\n",
        key, shiftIsDown, controlIsDown, altOptionIsDown, superCommandIsDown);
    if (key == GLFW_KEY_ENTER)
        isNearestNeighbor += 1;
        texSetFiltering(&tex, isNearestNeighbor%2);

}

void handleTimeStep(double oldTime, double newTime) {
    if (floor(newTime) - floor(oldTime) >= 1.0)
        printf("handleTimeStep: %f frames/sec\n", 1.0 / (newTime - oldTime));
    double transl[2] = {256.0, 256.0};
    double alpha[2] = {0,0}, beta[2] = {0,1}, gamma[2] = {1,1};
    triRender(a, b, c, rgb, &tex, alpha, beta, gamma);
}

int main(void) {

    if (pixInitialize(512, 512, "Testing") != 0)
        return 1;
    if (texInitializeFile(&tex,"messi.jpeg") != 0)
        return 1;

    texSetFiltering(&tex, 1);
    texSetLeftRight(&tex, 3);
    texSetTopBottom(&tex, 3);

    pixSetKeyUpHandler(handleKeyUp);
    pixSetTimeStepHandler(handleTimeStep);
    pixRun();

    texFinalize(&tex);
    pixFinalize();
    return 0;
}
