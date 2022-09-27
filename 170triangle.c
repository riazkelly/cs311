#include <stdio.h>
#include <math.h>

void interpolate(int i, int j, const double unif[], const texTexture *tex[], const shaShading *sha, const double a[], const double b[], const double c[], double chi[]) {
    /* This functions interpolates a, b, and c to an attrDim–dimensional vector chi.
        attrDim is passed to us through shaShading. */

    int attrDim = sha->attrDim;


    /* Find p and q */
    double A[2][2] = {{b[0] - a[0], c[0] - a[0]}, {b[1] - a[1], c[1] - a[1]}};
    double AInv[2][2], bMinusA[attrDim], cMinusA[attrDim], pq[2], xMinusa[2], X[2];
    double det;
    double attr[attrDim];
    double rgb[3];
    det = mat22Invert(A, AInv);
    if (det != 0.0) {
        X[0] = i;
        X[1] = j;
        vecSubtract(2, X, a, xMinusa);
        mat221Multiply(AInv, xMinusa, pq);

        /* Figure out the color at each pixel and paint it */
        vecSubtract(attrDim, b, a, bMinusA);
        vecSubtract(attrDim, c, a, cMinusA);
        vecScale(attrDim, pq[0], bMinusA, bMinusA);
        vecScale(attrDim, pq[1], cMinusA, cMinusA);
        vecAdd(attrDim, a, bMinusA, attr);
        vecAdd(attrDim, attr, cMinusA, attr);

        shadeFragment(sha->unifDim, unif, sha->texNum, tex, sha->attrDim, attr, rgb);

        pixSetRGB(i, j, rgb[0], rgb[1], rgb[2]);
    }
}

void triRenderWithALeft(const shaShading *sha, const double unif[], const texTexture *tex[],
const double a[], const double b[], const double c[]) {
    /* This function rasterizes the triangle a, b, c, assuming that a is the leftmost point
    and a -> b -> c is counter clock–wise */
    double top, bottom;
    int i, j;
    double chi[sha->attrDim];
    if (c[0] <= b[0]) { // If c[0] is closer to a[0] then b[0]
        for (i = ceil(a[0]); i <= floor(c[0]); i++) {
            bottom = ceil(a[1] + ((b[1] - a[1])/(b[0] - a[0])) * (i - a[0])); // Line from a to b
            if ((c[0] - a[0]) == 0)
                break;
            top = floor(a[1] + ((c[1] - a[1])/(c[0] - a[0])) * (i - a[0])); // Line from a to c
            for (j = bottom; j <= top; j++) {
                interpolate(i, j, unif, tex, sha, a, b, c, chi);
            }
        }
        for (i = floor(c[0]) + 1; i <= floor(b[0]); i++) {
            bottom = ceil(a[1] + ((b[1] - a[1])/(b[0] - a[0])) * (i - a[0])); // Line from a to b
            if ((c[0] - b[0]) == 0)
                break;
            top = floor(c[1] + ((b[1] - c[1])/(b[0] - c[0])) * (i - c[0])); // Line from c to b
            for (j = bottom; j <= top; j++) {
                interpolate(i, j, unif, tex, sha, a, b, c, chi);
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
                interpolate(i, j, unif, tex, sha, a, b, c, chi);
            }
        }
        for (i = floor(b[0]) + 1; i <= floor(c[0]); i++) {
            if ((c[0] - b[0]) == 0)
                break;
            bottom = ceil(b[1] + ((c[1] - b[1])/(c[0] - b[0])) * (i - b[0])); // Line from b to c
            top = floor(a[1] + ((c[1] - a[1])/(c[0] - a[0])) * (i - a[0])); // Line from a to c
            for (j = bottom; j <= top; j++) {
                interpolate(i, j, unif, tex, sha, a, b, c, chi);
            }
        }
    }
}


/* Assumes that the 0th and 1th elements of a, b, c are the 'x' and 'y'
coordinates of the vertices, respectively (used in rasterization, and to
interpolate the other elements of a, b, c). */
void triRender(
        const shaShading *sha, const double unif[], const texTexture *tex[],
        const double a[], const double b[], const double c[]) {
        /* This function makes sure that a is the leftmost point */
        if ((a[0] > b[0]) || (a[0] > c[0])) { // If a is not the left most, we rotate the points by recursion
            triRender(sha, unif, tex, c, a, b);
        }
        else { // When we know a is the leftmost point we call our function
            triRenderWithALeft(sha, unif, tex, a, b, c);
        }
}
