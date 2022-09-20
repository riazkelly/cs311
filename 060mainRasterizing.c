
/*
    clang 060mainRasterizing.c 040pixel.o -lglfw -framework OpenGL -framework Cocoa -framework IOKit
*/

#include <stdio.h>
#include <math.h>
#include <GLFW/glfw3.h>
#include "040pixel.h"
#include "060triangle.c"

int main(void) {
    /* Make a 512 x 512 window with the title 'Pixel Graphics'. This function
    returns 0 if no error occurred. */
    if (pixInitialize(512, 512, "Pixel Graphics") != 0)
        return 1;
    /* Clear the window to black. */
    pixClearRGB(0.0, 0.0, 0.0);
    /* Draws the given triangle(s) */
    triRender(150, 100, 300, 300, 50, 200, 0.25, 0.5, 0.75);
    triRender(100, 100, 500, 10, 250, 500, 0.15, 0.5, 0.35);
    /* Run the events */
    pixRun();
    /* Ends the pixel loop */
    pixFinalize();
    return 0;
}
