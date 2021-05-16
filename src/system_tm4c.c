/* ********************************************************
 *
 * This file implements the system initilization function
 *
 * Author:		Ameya Phadke
 * Date Created:	6th May 2021
 * Date Modified:	6th May 2021
 *
 * *******************************************************/

#include <main.h>
#include <system_tm4c.h>

#include "app.h"

#include "clock.h"
#include "bsp.h"
#include "uart.h"
#include "dma.h"
#include "adc.h"
#include "pwm.h"
#include "timer0.h"



/* system related functions and declarations */

uint32_t SystemCoreClock = SYSCLOCK_FREQ_Hz; /* System Clock Frequency in Hz */



/* ***************************************************************************
 *
 * This function calls the initialization functions for different required
 * peripherals.
 *
 * param: void
 *
 * return: void 
 * 
 * brief: This function calls the initialization functions for different
 * peripherals. PLL is configured to change the system clock frequency.
 * 
 * **************************************************************************/
void SystemInit(void){

	/* Do system initializations */


	/* Init system clock and set the PLL */
	configure_pll_clock(MOSC_CLOCK_SRC,0U,4U,96U,0U,(4U-1U),5);

	/* Init UART */
	UARTInit((uint32_t)115200);

#if( LED_BLINK_ENABLE == 1)
	/* INIT LED 1 */
	INIT_LED_1();
#endif
	
	/* Init DMA for ADC */
	DMA_init();
	
	/* Initialize the ADC */
	ADC_init();

	/* Initialize PWM */
	PWMLedInit(OUPUT_PWM_PERIOD_CYCLES,1);

	/* Initialize timer0 */
	init_timer0((uint32_t) SystemCoreClock/TIMER0_FREQUENCY_HZ);

	UARTSendString("system initialization done...\n\r");

	return;
}
