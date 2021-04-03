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

void ADC_init(void);
uint16_t ADC_get_val(void);

#endif
