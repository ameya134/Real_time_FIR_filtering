/* *****************************************************************
 *
 * API for timestamp usng timer7
 * 
 * Author: 		Ameya Phadke
 * Date Created:	6th May 2021
 * Date Modified:	6th May 2021
 *
 * ****************************************************************/

#include "timestamp.h"


void init_timestamp_timer(void)
{
    /* enable clocking to timer module */
	SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R7;
	/* wait till the peripheral is ready */
	while(!(SYSCTL_PRTIMER_R & SYSCTL_PRTIMER_R7));

	/* Disable the timer and clear previous configuration
	 * before doing the configuration.
     * Timer is in 32bit mode by default */
	TIMER7_CTL_R	&= ~(TIMER_CTL_TAEN);
	TIMER7_CFG_R	= 0x0;

	/* Configure timer mode
	 * set oneshot mode and disable interrupt
     * set count direction to count up */
	TIMER7_TAMR_R	|= (TIMER_TAMR_TACINTD | TIMER_TAMR_TACDIR | TIMER_TAMR_TAMR_1_SHOT);

	/* load the timer value */
	TIMER7_TAILR_R	= 0xFFFF;

	return;
}

uint32_t getTimeStampVal(void)
{
    return TIMER7_TAV_R;
}

void TimeStampStart(void)
{
    /* enable timer 0 */
	TIMER7_CTL_R	|= TIMER_CTL_TAEN;
}

void TimeStampHalt(void)
{
    /* disable timer 0 */
	TIMER7_CTL_R    &= ~(TIMER_CTL_TAEN);
}

void TimeStampRestart(void)
{
    /* reset the timer value to 0 */
    TIMER7_CTL_R    &= ~(TIMER_CTL_TAEN);
    TIMER7_TAV_R = 0;
	TIMER7_CTL_R	|= TIMER_CTL_TAEN;
}