/* ***********************************************************************
 * This is the main file for tm4c terminal demo application
 *
 * main calls the initialization functions and then waits in the 
 * superloop executing backround tasks or in a low power mode waiting
 * for interrupts.
 *
 * Author:		Ameya Phadke
 * Date Created:	1st Dec 2020
 * Last modified:	8th Jan 2021
 * 
 * **********************************************************************/

#include "main.h"
#include "system_tm4c.h"
#include "../test/test.h"

#include "FreeRTOS.h"
#include "task.h"

#include "app.h"

int main(void){

#if( PERFORM_TEST == 1 )

	test_main();

#endif
	/* system init */
	SystemInit();

	/* Call the init task for application */
	app_tasks_setup();

	/* Start FreeRTOS */
	vTaskStartScheduler();

	/* control should never reach to this point */
	while(1){
		// wait for interrupt in low power sleep mode
		__asm("wfi");
	}
	
	return 0;
}


void vApplicationIdleHook(void)
{
	/*do any background work here*/
	return;
}

void vApplicationStackOverflowHook( TaskHandle_t xTask, char * pcTaskName )
{
	while(1);
	return;
}

void vApplicationMallocFailedHook(void)
{
	while(1);
	return;
}

