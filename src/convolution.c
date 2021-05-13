#include "convolution.h"


void convolve_16x16(int32_t *x,int32_t *h, int32_t *y)
{
    uint8_t i,j;

    for(i=0;i<16;i++){
        for(j=0;j<16;j++){
            y[i+j] += x[i]*h[j];
        }
    }

    return;
}
