/* ***********************************************************
 * This file implements the API used to configure and 
 * use the various clocks present on tm4c129encpdt
 * This file uses TI's provided tm4c129encpdt.h header file
 *
 * Author:			Ameya Phadke
 * Date created:	28th April 2021
 * Last modified:	28th April 2021
 *
 * ***********************************************************/


#include "main.h"
#include "clock.h"


void configure_pll_clock(int clock_src,int Q, int N,int MINT, int MFRAC,int PSYSDIV,int MEMTIM_PARAM)
{
    /* Initialize the main oscilator if it is selected */
    if(clock_src == MOSC_CLOCK_SRC){
        SYSCTL_MOSCCTL_R |= ( SYSCTL_MOSCCTL_OSCRNG );
        SYSCTL_MOSCCTL_R &= ~( SYSCTL_MOSCCTL_NOXTAL | SYSCTL_MOSCCTL_PWRDN );

        SYSCTL_RSCLKCFG_R |= (SYSCTL_RSCLKCFG_PLLSRC_MOSC);
    }

    /* disable pll and configure registers */
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