/* ********************************************************
 * Header file for ADC module
 *
 * Author:		Ameya Phadke
 * 
 * Date Created:	22nd March 2021
 * Date Modified:	22nd March 2021
 * 
 * ********************************************************/


#ifndef _ADC_H
#define _ADC_H

#include "tm4c129encpdt.h"
#include "main.h"


#define ADC_USE_DMA	1


#if( ADC_USE_DMA == 1)

#include "dma.h"

#define ADC_DMA_CHANNEL_NO	14
#define ADC_DMA_CHANNEL_ENCODE	0
#define ADC_DMA_BUF_LEN 32


void ADC0_sequencer0_handler(void);

#endif





void ADC_init(void);
uint16_t ADC_get_val(void);

#endif
