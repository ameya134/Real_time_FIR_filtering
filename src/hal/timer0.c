/* ***********************************************************************
 *
 * This file provides implementation of functions for configuration and
 * use of Timer0 peripheral on tm4c129encpdt
 * 
 * Author:			Ameya Phadke
 * Date Created:	1st March 2021
 * Date Modified:	13th May 2021
 *
 * ***********************************************************************/

#include "main.h"
#include "timer0.h"
#include "bsp.h"
#include "dma.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "app.h"



/* This control word is used by the application which needs the
 * Timer0 to generate DMA transfer requests. This control word is configured
 * according to the needs of the application. */
#if( TIMER0A_GENERATE_DMA_TRIG == 1)
struct DMA_control_word TIMER0_channel_control_word = {

	.XFERMODE			= 0x1,					/* Basic mode */
	.NXTUSEBURST		= 0x0,					/* no next use burst for last transfers */
	.XFERSIZE			= (DATA_BUF_LEN -1),	/* Transfer size */
	.ARBSIZE			= 0x0,					/* arbitrate after 1 transfer */
	.SRCPROT0			= 0x0,					/* non privilaged access */
	/* .reserved1 */
	.DESTPROT0			= 0x0,					/* non privilaged access */
	/* .reserved0 */
	.SRCSIZE			= 0x1,					/* 16 bit data size */
	.SRCINC				= 0x2,					/* 32 bit increment */
	.DESTSIZE			= 0x1,					/* 16 bit data size */
	.DESTINC			= 0x3,					/* No increment */
};
#endif



/* **************************************************************************
 * This function Initializes the Timer0 with a load value
 *
 * param: loadval	Initial load value of the timer
 *
 * return: void
 * 
 * brief: Function initializes the timer0 module in periodic count down 
 * mode with the given load value.
 * 
 * If the TIMER0A_GENERATE_INTERRUPT flag is defined as 1 then the timer
 * generated.
 * 
 * TIMER0A_GENERATE_ADC_TRIG flag, if turned on, generates an ADC trigger
 * every time the timer0 reaches a value of 0 while counting down.
 * 
 * TIMER0A_GENERATE_DMA_TRIG flag, if turned on, generates a DMA trigger
 * every time the timer0 reaches a value of 0 while counting down.
 *
 * **************************************************************************/
void init_timer0(uint32_t loadVal)
{
	/* enable clocking to timer module */
	SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R0;
	/* wait till the peripheral is ready */
	while(!(SYSCTL_PRTIMER_R & SYSCTL_PRTIMER_R0));

	/* Disable the timer and clear previous configuration
	 * before doing the configuration */
	TIMER0_CTL_R	&= ~(TIMER_CTL_TAEN);
	TIMER0_CFG_R	= 0x0;

	/* Configure timer mode
	 * set periodic mode and enable interrupt if flag is set */
	TIMER0_TAMR_R	|= (0x2 << 0);

	/* configure to generate adc trigger */
#if( TIMER0A_GENERATE_ADC_TRIG == 1)
	TIMER0_CTL_R	|= TIMER_CTL_TAOTE;
	TIMER0_ADCEV_R	|= TIMER_ADCEV_TATOADCEN;
#endif

	/* configure to generate dma transfer trigger */
#if( TIMER0A_GENERATE_DMA_TRIG == 1)
	TIMER0_DMAEV_R |= (TIMER_DMAEV_TATODMAEN);
#endif

	/* configure to generate timeout interrupt */
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

	return;
}


/* ***************************************************************************
 *
 * This function enables the timer0
 *
 * param: void
 *
 * return: void 
 * 
 * brief: This function enables the timer0 which starts the timer0 counting
 * 
 * **************************************************************************/
void timer0_enable(void)
{
	/* enable timer 0 */
	TIMER0_CTL_R	|= (1U << 0);

	return;
}


/* Interrupt handler for timer0 if interrupts are generated. */
#if(TIMER0A_GENERATE_INTERRUPT == 1)
void timer0_handler(void)
{
	/* clear the interrrupt bit */
	TIMER0_ICR_R |= (1U<<0);
	
	return;
}
#endif
