#include <stdio.h>
#include <math.h>

void triRenderWithALeft(double a0, double a1, double b0, double b1, double c0, double c1,
double r, double g, double b) {
    /* This function rasterizes the triangle a, b, c, assuming that a is the leftmost point
    and a -> b -> c is counter clock–wise */
    double top, bottom;
    int i, j;
    if (c0 <= b0) { // If c0 is closer to a0 then b0
        for (i = ceil(a0); i <= floor(c0); i++) {
            bottom = ceil(a1 + ((b1 - a1)/(b0 - a0)) * (i - a0)); // Line from a to b
            top = floor(a1 + ((c1 - a1)/(c0 - a0)) * (i - a0)); // Line from a to c
            for (j = bottom; j <= top; j++) {
                pixSetRGB(i, j, r, g, b);
            }
        }
        for (i = floor(c0) + 1; i <= floor(b0); i++) {
            bottom = ceil(a1 + ((b1 - a1)/(b0 - a0)) * (i - a0)); // Line from a to b
            top = floor(c1 + ((b1 - c1)/(b0 - c0)) * (i - c0)); // Line from c to b
            for (j = bottom; j <= top; j++) {
                pixSetRGB(i, j, r, g, b);
            }
        }
    }
    else { //If b0 is closer to a0 then c0
        for (i = ceil(a0); i <= floor(b0); i++) {
            bottom = ceil(a1 + ((b1 - a1)/(b0 - a0)) * (i - a0)); // Line from a to b
            top = floor(a1 + ((c1 - a1)/(c0 - a0)) * (i - a0)); // Line from a to c
            for (j = bottom; j <= top; j++) {
                pixSetRGB(i, j, r, g, b);
            }
        }
        for (i = floor(b0) + 1; i <= floor(c0); i++) {
            bottom = ceil(b1 + ((c1 - b1)/(c0 - b0)) * (i - b0)); // Line from b to c
            top = floor(a1 + ((c1 - a1)/(c0 - a0)) * (i - a0)); // Line from a to c
            for (j = bottom; j <= top; j++) {
                pixSetRGB(i, j, r, g, b);
            }
        }
    }
}

void triRender(double a0, double a1, double b0, double b1, double c0, double c1,
double r, double g, double b) {
    /* This function makes sure that a is the leftmost point */
    double temp0, temp1;
    if ((a0 > b0) || (a0 > c0)) { // If a is not the left most, we rotate the points by recursion
        triRender(c0, c1, a0, a1, b0, b1, r, g, b);
    }
    else { // When we know a is the leftmost point we call our function
        triRenderWithALeft(a0, a1, b0, b1, c0, c1, r, g, b);
    }
}
