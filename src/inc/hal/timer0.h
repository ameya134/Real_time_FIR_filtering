/* ********************************************************
 *
 * timer0 setup and interrupt handler declarations
 *
 * Author:		Ameya Phadke
 * Date Created:	1st March 2021
 * Date Modified:	1st March 2021
 *
 * *******************************************************/


#include "tm4c129encpdt.h"


#define TIMER0A_INT_NUM		(19U)

/* the priority of interrupt needs to be lower than max_sycall_prio
 * of FreeRTOS in order to use FreeRTOS API inside the ISR */
#define TIMER0A_INT_NVIC_PRIO	(configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1)
void init_timer0(uint32_t loadVal);


void timer0_handler(void);
