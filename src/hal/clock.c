/* ***********************************************************
 * This file implements the API used to configure and 
 * use the various clocks present on tm4c129encpdt
 * This file uses TI's provided tm4c129encpdt.h header file
 *
 * Author:			Ameya Phadke
 * Date created:	28th April 2021
 * Last modified:	13th May 2021
 *
 * ***********************************************************/

#include "main.h"
#include "clock.h"



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
void configure_pll_clock(int clock_src,int Q, int N,int MINT, int MFRAC,int PSYSDIV,int MEMTIM_PARAM)
{
	/* Initialize the main oscilator if it is selected */
	if(clock_src == MOSC_CLOCK_SRC){
	SYSCTL_MOSCCTL_R |= ( SYSCTL_MOSCCTL_OSCRNG );
	SYSCTL_MOSCCTL_R &= ~( SYSCTL_MOSCCTL_NOXTAL | SYSCTL_MOSCCTL_PWRDN );

	SYSCTL_RSCLKCFG_R |= (SYSCTL_RSCLKCFG_PLLSRC_MOSC);
	}

	/* disable pll and configure the parameters in pll registers */
	SYSCTL_PLLFREQ0_R &= ~(SYSCTL_PLLFREQ0_PLLPWR);

	SYSCTL_PLLFREQ0_R |= (MINT << SYSCTL_PLLFREQ0_MINT_S);
	SYSCTL_PLLFREQ0_R |= (MFRAC << SYSCTL_PLLFREQ0_MFRAC_S);
	SYSCTL_PLLFREQ1_R |= (N << SYSCTL_PLLFREQ1_N_S);
	SYSCTL_PLLFREQ1_R |= (Q << SYSCTL_PLLFREQ1_Q_S);

	/* enable pll */
	SYSCTL_PLLFREQ0_R |= (SYSCTL_PLLFREQ0_PLLPWR);

	/* notify system of the new frequency and wait for pll to stabilize */
	SYSCTL_RSCLKCFG_R |= (SYSCTL_RSCLKCFG_NEWFREQ);
	while(!(SYSCTL_PLLSTAT_R & SYSCTL_PLLSTAT_LOCK));

	/* Configure memory timing parameters */
	SYSCTL_MEMTIM0_R |= ((MEMTIM_PARAM << SYSCTL_MEMTIM0_FWS_S) | (MEMTIM_PARAM << SYSCTL_MEMTIM0_EWS_S) );

	/* Memory timing parameters are calculated and updated according to the target frequency.
	* These timing parameters can be found in the datasheet. */
	if(MEMTIM_PARAM == 0){
	SYSCTL_MEMTIM0_R |= ((SYSCTL_MEMTIM0_FBCHT_M & (MEMTIM_PARAM<< 6)) 
			| (SYSCTL_MEMTIM0_EBCHT_M & (MEMTIM_PARAM<< 22)) );    
	}
	SYSCTL_MEMTIM0_R |= ((SYSCTL_MEMTIM0_FBCHT_M & ((MEMTIM_PARAM+1)<< 6)) 
			| (SYSCTL_MEMTIM0_EBCHT_M & ((MEMTIM_PARAM+1) << 22)) );

	SYSCTL_MEMTIM0_R &= ~(SYSCTL_MEMTIM0_FBCE | SYSCTL_MEMTIM0_EBCE);


	SYSCTL_RSCLKCFG_R |= (SYSCTL_RSCLKCFG_USEPLL | SYSCTL_RSCLKCFG_MEMTIMU | (PSYSDIV << SYSCTL_RSCLKCFG_PSYSDIV_S));

	return;
}
