/* *****************************************************************
 * 
 * Header file for convolution.c 
 *
 * Author:		Ameya Phadke
 * Date Created:	5th May 2021
 * Date Modified:	5th May 2021
 *
 * *****************************************************************/

#ifndef _CONVOLUTION_H
#define _CONVOLUTION_H


#include "main.h"



/* **********************************************************************
 * This function computes the linear convolution of two signals
 * of length 16
 *
 * param: x     pointer to signal 1 array (generally input)
 * param: h	    pointer to signal 2 array (generally transfer function)
 * param: y     pointer to output array to store answer of convolution
 *
 * return: void
 * 
 * brief: This function computes the Linear Convolution of two descrete 
 * time signals of length 16. The length of output is (2*16 -1) = 31.
 *
 * ***********************************************************************/
void convolve_16x16(int32_t *x,int32_t *h, int32_t *y);


#endif
