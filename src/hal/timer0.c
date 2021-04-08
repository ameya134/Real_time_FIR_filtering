/* *****************************************************************
 *
 * HAL for timer0 configuration and interrupt handlers
 * 
 * Author: 		Ameya Phadke
 * Date Created:	1st March 2021
 * Date Modified:	1st March 2021
 *
 * ****************************************************************/


#include "main.h"
#include "timer0.h"
#include "bsp.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

void init_timer0(uint32_t loadVal)
{
	/* enable clocking to timer module */
	SYSCTL_RCGCTIMER_R |= (1U << 0);
	/* wait till the peripheral is ready */
	while(!(SYSCTL_PRTIMER_R & SYSCTL_PRTIMER_R0));

	/* Disable the timer and clear previous configuration
	 * before doing the configuration */
	TIMER0_CTL_R	&= ~(TIMER_CTL_TAEN);
	TIMER0_CFG_R	= 0x0;

	/* Configure timer mode
	 * set periodic mode and enable interrupt */
	TIMER0_TAMR_R	|= (0x2 << 0);

#if( TIMER0A_GENERATE_ADC_TRIG == 1)
	TIMER0_CTL_R	|= TIMER_CTL_TAOTE;
	TIMER0_ADCEV_R	|= TIMER_ADCEV_TATOADCEN;
#endif


#if( TIMER0A_GENERATE_INTERRUPT == 1 )	
	/* set interrupt mask to generate periodic interrupt */
	TIMER0_IMR_R	|= (1U <<0);
	NVIC_EN0_R	|= (1U <<TIMER0A_INT_NUM);
	/* Priority of interrupt needs to be lower than max_syscall_prio
	 * in order to use the FreeRTOS API. This is done by modifying the
	 * NVIC register corresponding to this timer interrupt number. 
	 * 
	 * The higher 3 bits of a byte specify the interrupt prio. the corresponding
	 * bits to be changed for this interrupt are on the most significant byte
	 * (ie. 4th byte) of the PRI4 register.
	 *
	 * Higher numerical value corresponds to lower logical priotiy level.*/
	NVIC_PRI4_R	=  ( ( TIMER0A_INT_NVIC_PRIO << (8 - configPRIO_BITS) ) << 3*8 );
#else
	TIMER0_TAMR_R	|= (0x1 << 12);
#endif


	/* load the timer value */
	TIMER0_TAILR_R	= loadVal;

	/* enable timer 0 */
	TIMER0_CTL_R	|= (1U << 0);

	return;
}


#if(TIMER0A_GENERATE_INTERRUPT == 1)
void timer0_handler(void)
{
	/* clear the interrrupt bit */
	TIMER0_ICR_R |= (1U<<0);
	
	return;
}
#endif
