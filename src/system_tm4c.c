#include <main.h>
#include <system_tm4c.h>

#include "app.h"

#include "bsp.h"
#include "uart.h"

/* system related functions and declarations */


uint32_t SystemCoreClock = 16000000; /* System Base Clock Frequency in Hz */


void SystemInit(void){

	/* Do any system initialization that is required */

	/* Init required peripherals */

#if( LED_BLINK_ENABLE == 1)
	/* INIT LED 1 */
	INIT_LED_1();
#endif

	UARTInit((uint32_t)115200);

	return;
}
