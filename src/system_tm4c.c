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


uint32_t SystemCoreClock = SYSCLOCK_FREQ_Hz; /* System Base Clock Frequency in Hz */


void SystemInit(void){

	/* Do any system initialization that is required */

	/* Init required peripherals */

	/* Init system clock and set the PLL */
	configure_pll_clock(MOSC_CLOCK_SRC,0U,4U,96U,0U,(4U-1U),5);

	/* Init UART */
	UARTInit((uint32_t)115200);

#if( LED_BLINK_ENABLE == 1)
	/* INIT LED 1 */
	INIT_LED_1();
	INIT_LED_3();
#endif
	
	/* Init DMA for ADC */
	DMA_init();
	
	/* Initialize the ADC */
	ADC_init();

	PWMLedInit(OUPUT_PWM_PERIOD_CYCLES,1);
	/* Initialize the timer */
	//init_timer0((uint32_t) SystemCoreClock/TIMER0_FREQUENCY_HZ);  
	
	UARTSendString("timer_initialized\n\r");
		
	return;
}
