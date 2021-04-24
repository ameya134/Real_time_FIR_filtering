#include <main.h>
#include <system_tm4c.h>

#include "app.h"

#include "bsp.h"
#include "uart.h"
#include "dma.h"
#include "adc.h"
#include "timer0.h"

/* system related functions and declarations */


uint32_t SystemCoreClock = 16000000; /* System Base Clock Frequency in Hz */


void SystemInit(void){

	/* Do any system initialization that is required */

	/* Init required peripherals */

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

	/* Initialize the timer */
	//init_timer0((uint32_t) SystemCoreClock/TIMER0_FREQUENCY_HZ);
        UARTSendString("timer_initialized\n\r");
	
	
	
	return;
}
