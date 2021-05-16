/* ***********************************************************************
 * This is the main file which initializes the FreeRTOS
 *
 * main calls the initialization functions and then starts the scheduler
 *
 * Author:			Ameya Phadke
 * Date Created:	3rd April 2021
 * Last modified:	15th May 2021
 * 
 * **********************************************************************/

#include "main.h"
#include "system_tm4c.h"
#include "../test/test.h"

#include "FreeRTOS.h"
#include "task.h"

#include "app.h"



/* **********************************************************************
 * The main function calls the system initialize function, rtos task
 * setup function and then starts the FreeRTOS scheduler.
 * 
 * if PERFORM_TEST flag has been set to 1 then it directly calls the 
 * test function.
 * 
 * **********************************************************************/
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

/* Idle tasks calls this hook */
void vApplicationIdleHook(void)
{
	/*do any background work here*/
	return;
}

/* A stackoverflow results in a call to this function 
 * and the task hangs in the while loop */
void vApplicationStackOverflowHook( TaskHandle_t xTask, char * pcTaskName )
{
	while(1);
	return;
}

/* A failed call to malloc results in a call to this function
 * and the task hangs in the while loop */
void vApplicationMallocFailedHook(void)
{
	while(1);
	return;
}

