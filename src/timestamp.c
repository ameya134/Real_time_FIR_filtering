/* *****************************************************************
 *
 * This file implements the timestamp functions using timer7
 * 
 * Author: 		Ameya Phadke
 * Date Created:	6th May 2021
 * Date Modified:	6th May 2021
 *
 * ****************************************************************/

#include "timestamp.h"



/* ***************************************************************************
 *
 * This function initializes the timer7 in 32-bit, one shot, count up mode.
 *
 * param: void
 *
 * return: void 
 * 
 * brief: This function starts the timer7 in 32-bit, one shot, count up mode
 * to be used for timekeeping. This function needs to be called initially 
 * before calling other timestamp functions.
 * 
 * **************************************************************************/
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



/* ***************************************************************************
 *
 * This function returns the count value of the timer7
 *
 * param: void
 *
 * return: uint32_t		timer7 count value
 * 
 * brief: This function returns count value of timer7 which can be used to 
 * calculate time period between 2 calls to this function.
 * 
 * **************************************************************************/
uint32_t getTimeStampVal(void)
{
    return TIMER7_TAV_R;
}



/* ***************************************************************************
 *
 * This function starts the timer7 to count up.
 *
 * param: void
 *
 * return: void 
 * 
 * brief: This function starts the timer7. Timer7 starts counting up in
 * one shot mode.
 * 
 * **************************************************************************/
void TimeStampStart(void)
{
	/* enable timer 0 */
	TIMER7_CTL_R	|= TIMER_CTL_TAEN;
}



/* ***************************************************************************
 *
 * This function halts the timer7.
 *
 * param: void
 *
 * return: void 
 * 
 * brief: This function halts the counting of timer7. timer7 retains its 
 * current counting value in TIMER7_TAV_R register.
 * 
 * **************************************************************************/
void TimeStampHalt(void)
{
	/* disable timer 0 */
	TIMER7_CTL_R    &= ~(TIMER_CTL_TAEN);
}



/* ***************************************************************************
 *
 * This function restarts the timer7 to count up.
 *
 * param: void
 *
 * return: void 
 * 
 * brief: This function restarts the timer7. the current value of timer7 is
 * set to 0 and then Timer7 starts counting up in one shot mode.
 * 
 * **************************************************************************/
void TimeStampRestart(void)
{
	/* reset the timer value to 0 */
	TIMER7_CTL_R    &= ~(TIMER_CTL_TAEN);
	TIMER7_TAV_R = 0;
	TIMER7_CTL_R	|= TIMER_CTL_TAEN;
}
