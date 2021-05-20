/* *********************************************************************
 * 
 * This file implements functions to calculate Linear convolution
 * of two signals
 * 
 * Author:          Ameya Phadke
 * Date created:	6th May 2021
 * Last modified:	13th May 2021
 * 
 * *********************************************************************/

#include "convolution.h"


/* **********************************************************************
 * This function computes the linear convolution of two signals
 * of length 16
 *
 * param: x	pointer to signal 1 array (generally input)
 * param: h	pointer to signal 2 array (generally transfer function)
 * param: y	pointer to output array to store answer of convolution
 *
 * return: void
 * 
 * brief: This function computes the Linear Convolution of two descrete 
 * time signals of length 16. The length of output is (2*16 -1) = 31.
 *
 * ***********************************************************************/
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
