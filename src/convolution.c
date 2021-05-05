#include "convolution.h"


void convolve_16x16(uint16_t *x,uint16_t *h, uint16_t *y)
{
    uint8_t i,j;

    for(i=0;i<16;i++){
        for(j=0;j<16;j++){
            y[i+j] += x[i]*h[j];
        }
    }

    return;
}
