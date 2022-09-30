
/* This code is brought to you by Ben Turner and Riaz Kelly */

#include <stdio.h>

typedef struct shaShading shaShading;

struct shaShading {
    int unifDim;
    int attrDim;
    int texNum;
    int varyDim;

    /* pointer to shadeVertex function */
    void (*shadeVertex)(
        int unifDim, const double unif[], int attrDim, const double attr[],
        int varyDim, double vary[]);

    /* pointer to shadeFragment function */
    void (*shadeFragment)(
        int unifDim, const double unif[], int texNum, const texTexture *tex[],
        int varyDim, const double vary[], double rgb[3]);
};
