
/*
    clang 080mainRasterizing.c 040pixel.o -lglfw -framework OpenGL -framework Cocoa -framework IOKit
*/

#include <stdio.h>
#include <math.h>
#include <GLFW/glfw3.h>
#include "040pixel.h"
#include "080triangle.c"
#include "080vector.c"

int main(void) {
    /* Make a 512 x 512 window with the title 'Pixel Graphics'. This function
    returns 0 if no error occurred. */
    if (pixInitialize(512, 512, "Pixel Graphics") != 0)
        return 1;
    /* Clear the window to black. */
    pixClearRGB(0.0, 0.0, 0.0);
    /* Draws the given triangle(s) */
    double a[2] = {150, 100};
    double b[2] = {300, 300};
    double c[2] = {300, 500};
    double rgb[3] = {0.15, 0.5, 0.35};
    triRender(a, b, c, rgb);
    /* Run the events */
    pixRun();
    /* Ends the pixel loop */
    pixFinalize();
    return 0;
}
