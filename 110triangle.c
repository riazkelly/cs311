#include <stdio.h>
#include <math.h>

void interpolate(int i, int j, const double a[2], const double b[2], const double c[2],
        const double rgb[3], const double alpha[3], const double beta[3],
        const double gamma[3]) {
    /* This functions replaces the call to pixSetRGB inside of triRender.
       It interpolates the point x = (i, j) with points a, b, c and color rgb. */
    /* Find p and q */
    double A[2][2] = {{b[0] - a[0], c[0] - a[0]}, {b[1] - a[1], c[1] - a[1]}};
    double AInv[2][2], xColor[3], tempArray[3], tempArray2[3], pq[2], xMinusa[2], X[2];
    double det;
    det = mat22Invert(A, AInv);
    if (det != 0.0) {
        X[0] = i;
        X[1] = j;
        vecSubtract(2, X, a, xMinusa);
        mat221Multiply(AInv, xMinusa, pq);

        /* Figure out the color at each pixel and paint it */
        vecSubtract(3, beta, alpha, tempArray);
        vecSubtract(3, gamma, alpha, tempArray2);
        vecScale(3, pq[0], tempArray, tempArray);
        vecScale(3, pq[1], tempArray2, tempArray2);
        vecAdd(3, alpha, tempArray, xColor);
        vecAdd(3, xColor, tempArray2, xColor);
        vecModulate(3, xColor, rgb, xColor);
        pixSetRGB(i, j, xColor[0], xColor[1], xColor[2]);
    }
}

void triRenderWithALeft(const double a[2], const double b[2], const double c[2],
        const double rgb[3], const double alpha[3], const double beta[3],
        const double gamma[3]) {
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
                interpolate(i, j, a, b, c, rgb, alpha, beta, gamma);
            }
        }
        for (i = floor(c[0]) + 1; i <= floor(b[0]); i++) {
            bottom = ceil(a[1] + ((b[1] - a[1])/(b[0] - a[0])) * (i - a[0])); // Line from a to b
            if ((c[0] - b[0]) == 0)
                break;
            top = floor(c[1] + ((b[1] - c[1])/(b[0] - c[0])) * (i - c[0])); // Line from c to b
            for (j = bottom; j <= top; j++) {
                interpolate(i, j, a, b, c, rgb, alpha, beta, gamma);
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
                interpolate(i, j, a, b, c, rgb, alpha, beta, gamma);
            }
        }
        for (i = floor(b[0]) + 1; i <= floor(c[0]); i++) {
            if ((c[0] - b[0]) == 0)
                break;
            bottom = ceil(b[1] + ((c[1] - b[1])/(c[0] - b[0])) * (i - b[0])); // Line from b to c
            top = floor(a[1] + ((c[1] - a[1])/(c[0] - a[0])) * (i - a[0])); // Line from a to c
            for (j = bottom; j <= top; j++) {
                interpolate(i, j, a, b, c, rgb, alpha, beta, gamma);
            }
        }
    }
}

void triRender(const double a[2], const double b[2], const double c[2],
        const double rgb[3], const double alpha[3], const double beta[3],
        const double gamma[3])  {
    /* This function makes sure that a is the leftmost point */
    if ((a[0] > b[0]) || (a[0] > c[0])) { // If a is not the left most, we rotate the points by recursion
        triRender(c, a, b, rgb, gamma, alpha, beta);
    }
    else { // When we know a is the leftmost point we call our function
        triRenderWithALeft(a, b, c, rgb, alpha, beta, gamma);
    }
}
