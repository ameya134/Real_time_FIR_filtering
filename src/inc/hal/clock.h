/* ********************************************************
 * This file provides the API declaration for 
 * configuration and use of various clock sources which are 
 * present on tm4c129encpdt 
 *
 * Author:		Ameya Phadke
 * Date created:	13th Dec 2020
 * Last modified:	8th Jan 2021
 *
 * ********************************************************/

#ifndef _CLOCK_H
#define _CLOCK_H

#include "tm4c129encpdt.h"

#define PIOSC_CLOCK_SRC 0
#define MOSC_CLOCK_SRC  1

#define PIOSC_FREQ_Hz   16000000 /* Precision internal oscilator at 16MHz */
#define MOSC_FREQ_Hz    25000000 /* Main external crystal oscilator at 25MHz */

void configure_pll_clock(int clock_src,int Q, int N,int MINT, int MFRAC,int PSYSDIV,int MEMTIM_PARAM);

#endif
