#include <stdio.h>
#include <math.h>

void triRenderWithALeft(const double a[2], const double b[2], const double c[2], const double rgb[3]) {
    /* This function rasterizes the triangle a, b, c, assuming that a is the leftmost point
    and a -> b -> c is counter clock–wise */
    double top, bottom;
    int i, j;
    if (c[0] <= b[0]) { // If c[0] is closer to a[0] then b[0]
        for (i = ceil(a[0]); i <= floor(c[0]); i++) {
            bottom = ceil(a[1] + ((b[1] - a[1])/(b[0] - a[0])) * (i - a[0])); // Line from a to b
            if ((c[0] - a[0]) == 0)
                break;
            top = floor(a[1] + ((c[1] - a[1])/(c[0] - a[0])) * (i - a[0])); // Line from a to c
            for (j = bottom; j <= top; j++) {
                pixSetRGB(i, j, rgb[0], rgb[1], rgb[2]);
            }
        }
        for (i = floor(c[0]) + 1; i <= floor(b[0]); i++) {
            bottom = ceil(a[1] + ((b[1] - a[1])/(b[0] - a[0])) * (i - a[0])); // Line from a to b
            if ((c[0] - b[0]) == 0)
                break;
            top = floor(c[1] + ((b[1] - c[1])/(b[0] - c[0])) * (i - c[0])); // Line from c to b
            for (j = bottom; j <= top; j++) {
                pixSetRGB(i, j, rgb[0], rgb[1], rgb[2]);
            }
        }
    }
    else { //If b[0] is closer to a[0] then c[0]
        for (i = ceil(a[0]); i <= floor(b[0]); i++) {
            if ((b[0] - a[0]) == 0)
                break;
            bottom = ceil(a[1] + ((b[1] - a[1])/(b[0] - a[0])) * (i - a[0])); // Line from a to b
            top = floor(a[1] + ((c[1] - a[1])/(c[0] - a[0])) * (i - a[0])); // Line from a to c
            for (j = bottom; j <= top; j++) {
                pixSetRGB(i, j, rgb[0], rgb[1], rgb[2]);
            }
        }
        for (i = floor(b[0]) + 1; i <= floor(c[0]); i++) {
            if ((c[0] - b[0]) == 0)
                break;
            bottom = ceil(b[1] + ((c[1] - b[1])/(c[0] - b[0])) * (i - b[0])); // Line from b to c
            top = floor(a[1] + ((c[1] - a[1])/(c[0] - a[0])) * (i - a[0])); // Line from a to c
            for (j = bottom; j <= top; j++) {
                pixSetRGB(i, j, rgb[0], rgb[1], rgb[2]);
            }
        }
    }
}

void triRender(const double a[2], const double b[2], const double c[2], const double rgb[3])  {
    /* This function makes sure that a is the leftmost point */
    if ((a[0] > b[0]) || (a[0] > c[0])) { // If a is not the left most, we rotate the points by recursion
        triRender(c, a, b, rgb);
    }
    else { // When we know a is the leftmost point we call our function
        triRenderWithALeft(a, b, c, rgb);
    }
}
