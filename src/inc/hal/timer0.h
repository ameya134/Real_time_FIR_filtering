/* *****************************************************************
 *
 * This file provides the declarations for configuration and
 * use of Timer0 peripheral on tm4c129encpdt
 *
 * Author:          Ameya Phadke
 * Date Created:	1st March 2021
 * Date Modified:	13th May 2021
 *
 * *****************************************************************/

#ifndef _TIMER0_H
#define _TIMER0_H


#include "tm4c129encpdt.h"



/* macros to configure the interrupt, adc and dma triggers */
#define TIMER0A_GENERATE_INTERRUPT      0
#define TIMER0A_GENERATE_ADC_TRIG       1
#define TIMER0A_GENERATE_DMA_TRIG       1


/* channel no and encoding no for timer0 dma transfer request */
#define TIMER0_DMA_CHANNEL_NO           18
#define TIMER0_DMA_CHANNEL_ENCODE       0


/* macros for interrupt no and priority of the interrupt */
#if( TIMER0A_GENERATE_INTERRUPT == 1)

#define TIMER0A_INT_NUM		(19U)
/* the priority of interrupt needs to be lower than max_sycall_prio
 * of FreeRTOS in order to use FreeRTOS API inside the ISR */
#define TIMER0A_INT_NVIC_PRIO	(configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1)

void timer0_handler(void);

#endif


/* **************************************************************************
 * This function Initialized the Timer0 with a load value
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
void init_timer0(uint32_t loadVal);



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
void timer0_enable(void);



#endif
