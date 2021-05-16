/* ********************************************************
 * This file provides the API declaration for 
 * configuration and use of various clock sources which are 
 * present on tm4c129encpdt 
 *
 * Author:          Ameya Phadke
 * Date created:	28th April 2021
 * Last modified:	13th May 2021
 *
 * ********************************************************/

#ifndef _CLOCK_H
#define _CLOCK_H

#include "tm4c129encpdt.h"


/* DEFINES for clock sources */
#define PIOSC_CLOCK_SRC 0
#define MOSC_CLOCK_SRC  1

/* Frequency in Hertz for various clock sources */
#define PIOSC_FREQ_Hz   16000000 /* Precision internal oscilator at 16MHz */
#define MOSC_FREQ_Hz    25000000 /* Main external crystal oscilator at 25MHz */


/* *******************************************************************************
 * This function configures the PLL to produce the desired
 * clock frequency for the system clock.
 *
 * param: clock_src     The clock source input to the PLL(MOSC,PIOSC etc)
 * param: Q             Q is a divisor value for PLL
 * param: N             N is a divisor value for PLL
 * param: MINT          Integer part of PLL multiplier value
 * param: MFRAC         Fractional part of PLL multiplier value
 * param: PSYDIV        clock divisor for PLL VCO frequency.
 * param: MEMTIM_PARAM  memory timing parameter
 *
 * return: void
 * 
 * brief: This function takes arguments for PLL multiplier and divisor and sets the
 * Pll output frequency accordingly.
 * 
 * PLL can be set to use either PIOSC_CLOCK_SRC (16MHz) or 
 * MOSC_CLOCK_SRC (externally connected crystal).
 * 
 * The output frequency of the PLL can be calculated with the help of following
 * formulae.
 * 
 * PLL output freq Fvco = Fin * MDIV
 * 
 * where,
 * 
 * Fin = Fxtal/((Q+1)*(N+1))
 * 
 * and
 * 
 * MDIV = MINT + MFRAC/1024
 * 
 * The system clock frequency SysClock is obtained by following equation.
 * 
 * SysClk = Fvco / (PSYDIV + 1)
 * 
 * As the system clock frequency is changed we need to change the memory timing parameters.
 * The appropriate Memory timing parameters for a bracket of frequency range are given 
 * in the datasheet and should be appropriately passed by the caller.
 * 
 * *******************************************************************************/
void configure_pll_clock(int clock_src,int Q, int N,int MINT, int MFRAC,int PSYSDIV,int MEMTIM_PARAM);


#endif
