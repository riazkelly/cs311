
/* This file made possible by the work and effort of both Ben Turner AND Riaz Kelly*/

#include "040pixel.h"
#include <stdlib.h>

double calculateLine(
    const double a[2], const double b[2], double x0
) {
    /* return the height of a point along a line given its x value */
    return a[1] + ((b[1] - a[1])/(b[0] - a[0]))*(x0 - a[0]);
}

void interpolate(double x, double y, double AInv[2][2], const double a[2], const double b[2], const double c[2],
        const double rgb[3], const texTexture *tex, const double alpha[2], const double beta[2],
        const double gamma[2]) {

            double pq[2], xSubtractA[2], chi[2], temp[2], finalColor[3], xy[2] = {x,y};

            /* first we find pq for this pixel */
            vecSubtract(2, xy, a, xSubtractA);
            mat221Multiply(AInv, xSubtractA, pq);

            /* calculate the linear interpolation at point x or vector pq */
            vecSubtract(2, beta, alpha, chi);
            vecSubtract(2, gamma, alpha, temp);
            vecScale(2, pq[0], chi, chi);
            vecScale(2, pq[1], temp, temp);
            vecAdd(2, alpha, chi, chi);
            vecAdd(2, chi, temp, chi);

            double sample[tex->texelDim];

            /* texture lookup */
            texSample(tex, chi[0], chi[1], sample);

            /* now we modulate the new color with universal color*/
            vecModulate(3, sample, rgb, finalColor);

            pixSetRGB(x, y, finalColor[0], finalColor[1], finalColor[2]);

}

void triRenderWithALeft(
        const double a[2], const double b[2], const double c[2],
        const double rgb[3], const texTexture *tex, const double alpha[3], const double beta[3],
        const double gamma[3]) {


        int start, middle, finish, bottom, top;

        double AInv[2][2], x, y;
        const double A[2][2] = {{b[0] - a[0], c[0] - a[0]},{b[1] - a[1], c[1] - a[1]}};

         /* check if valid triangle */
        if (mat22Invert(A, AInv) == 0) {
            printf("Invalid triangle!!");
            exit(0);
        }

         /* if we have a point up triangle */
        if (c[0] <= b[0]) {
            /* define boundaries of triangle at the starting point*/
            start = ceil(a[0]);
            middle = floor(c[0]);
            finish = floor(b[0]);

            /* first loop to get left half of triangle*/
            for (x = start; x <= middle; x++) {
                bottom = ceil(calculateLine(a,b,x));
                if (a[0]==c[0]) {
                    break;
                }
                top = floor(calculateLine(a,c,x));
                for (y = bottom; y <= top; y++) {
                    interpolate(x, y, AInv, a, b, c, rgb, tex, alpha, beta, gamma);
                }
            }
            /* second loop to get right half of triangle*/
            for (x = middle + 1; x <= finish; x++) {
                bottom = ceil(calculateLine(a,b,x));
                if (c[0] == b[0]){
                    break;
                }
                top = floor(calculateLine(c,b,x));
                for (y = bottom; y <= top; y++) {
                    interpolate(x, y, AInv, a, b, c, rgb, tex, alpha, beta, gamma);
                }
            }
        } else {
            /* if we have a point down triangle */

            /* define boundaries of triangle at the starting point*/
            start = ceil(a[0]);
            middle = floor(b[0]);
            finish = floor(c[0]);

            /* first loop to get left half of triangle*/
            for (x = start; x <= middle; x++) {
                if (a[0]==b[0]) {
                    break;
                }
                bottom = ceil(calculateLine(a,b,x));
                top = floor(calculateLine(a,c,x));
                for (y = bottom; y <= top; y++) {
                    interpolate(x, y, AInv, a, b, c, rgb, tex, alpha, beta, gamma);
                }
            }

            /* second loop to get right half of triangle*/
            for (x = middle + 1; x <= finish; x++) {
                if (c[0] == b[0]){
                    break;
                }
                bottom = ceil(calculateLine(b,c,x));
                top = floor(calculateLine(a,c,x));
                for (y = bottom; y <= top; y++) {
                    interpolate(x, y, AInv, a, b, c, rgb, tex, alpha, beta, gamma);
                }
            }
        }
    }


void triRender(
        const double a[2], const double b[2], const double c[2],
        const double rgb[3], const texTexture *tex, const double alpha[2],
        const double beta[2], const double gamma[2]) {
    /* if vertex a is not on the left, then we rotate until it is*/
	if ((a[0] > b[0]) || (a[0] > c[0])) { // If a is not the left most, we rotate the points by recursion
        triRender(c, a, b, rgb, tex, gamma, alpha, beta);
    }
    else { // When we know a is the leftmost point we call our function
        triRenderWithALeft(a, b, c, rgb, tex, alpha, beta, gamma);
    }
}
