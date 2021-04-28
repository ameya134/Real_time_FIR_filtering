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


void configure_pll_clock(int clock_src,int Q, int N,int MINT, int MFRAC,int PSYSDIV)
{
    if(clock_src == MOSC_CLOCK_SRC){
        SYSCTL_MOSCCTL_R |= ( SYSCTL_MOSCCTL_OSCRNG );
        SYSCTL_MOSCCTL_R &= ~( SYSCTL_MOSCCTL_NOXTAL | SYSCTL_MOSCCTL_PWRDN );
    }


    SYSCTL_PLLFREQ0_R &= ~(SYSCTL_PLLFREQ0_PLLPWR);

	SYSCTL_PLLFREQ0_R |= (MINT << SYSCTL_PLLFREQ0_MINT_S);
	SYSCTL_PLLFREQ0_R |= (MFRAC << SYSCTL_PLLFREQ0_MFRAC_S);
    SYSCTL_PLLFREQ1_R |= (N << SYSCTL_PLLFREQ1_N_S);
    SYSCTL_PLLFREQ1_R |= (Q << SYSCTL_PLLFREQ1_Q_S);

    SYSCTL_PLLFREQ0_R |= (SYSCTL_PLLFREQ0_PLLPWR);
	

    SYSCTL_RSCLKCFG_R |= (SYSCTL_RSCLKCFG_NEWFREQ);
    while(!(SYSCTL_PLLSTAT_R & SYSCTL_PLLSTAT_LOCK));

    return;
}