
/* This file made possible by the work and effort of both Ben Turner AND Riaz Kelly*/

/* On macOS, compile with...
    clang 140mainTexturing.c 040pixel.o -lglfw -framework OpenGL -framework Cocoa -framework IOKit
On Ubuntu, compile with...
    cc 100mainTesting.c 040pixel.o -lglfw -lGL -lm -ldl
*/

#include <stdio.h>
#include <math.h>
#include "040pixel.h"

#include "080vector.c"
#include "100matrix.c"
#include "140texture.c"
#include "140triangle.c"

double a[2] = {144.0, -156.0};
double b[2] = {244.0, 244.0};
double c[2] = {-226.0, -226.0};
double rgb[3] = {1.0, 1.0, 0.0};
double angle = 0.0;
texTexture tex;
const char* path = "alex-ovechkin-goal-50.jpeg\0";

void handleTimeStep(double oldTime, double newTime) {
    if (floor(newTime) - floor(oldTime) >= 1.0)
        printf("handleTimeStep: %f frames/sec\n", 1.0 / (newTime - oldTime));
    double transl[2] = {256.0, 256.0};
    double aa[2], bb[2], cc[2], rrggbb[3], rot[2][2], alpha[2] = {0,0}, beta[2] = {0,1}, gamma[2] = {1,1};
    angle += (newTime - oldTime) / 10.0;
    mat22Rotation(angle, rot);
    mat221Multiply(rot, a, aa);
    mat221Multiply(rot, b, bb);
    mat221Multiply(rot, c, cc);
    vecAdd(2, transl, aa, aa);
    vecAdd(2, transl, bb, bb);
    vecAdd(2, transl, cc, cc);
    vecScale(3, (2.0 + sin(newTime)) / 3.0, rgb, rrggbb);
    pixClearRGB(0.0, 0.0, 0.0);
    triRender(aa, bb, cc, rrggbb, &tex, alpha, beta, gamma);
}

int main(void) {
    if (pixInitialize(512, 512, "Testing") != 0)
        return 1;
    if (texInitializeFile(&tex,path) != 0)
        return 1;
    texSetFiltering(&tex, 1);
    texSetLeftRight(&tex, 3);
    texSetTopBottom(&tex, 3);
    pixSetTimeStepHandler(handleTimeStep);
    pixRun();
    texFinalize(&tex);
    pixFinalize();
    return 0;
}
