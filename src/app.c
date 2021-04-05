/* **************************************************************
 *
 * Application logic is implemented here
 *
 * All the FreeRTOS tasks are initialised from app_task_setup()
 * Fucntion call. Each task is scheduled by the scheduler when
 * main starts the scheduler.
 *
 * Author:		Ameya Phadke
 * Date Created:	4th April 2021
 * Date Modified:	4th April 2021
 *
 * ***************************************************************/


#include "main.h"
#include "app.h"

#include "FreeRTOS.h"
#include "task.h"

#include "bsp.h"


#if( LED_BLINK_ENABLE == 1)
void LED_blink_task(void *param)
{

	const TickType_t xDelay = LED_BLINK_DELAY_MS/portTICK_PERIOD_MS;

	for(;;){
		LED_TOGGLE_STATE(LED1_PORT,LED1_PIN);
		vTaskDelay(xDelay);
	}

	return;
}
#endif


void app_tasks_setup(void)
{

#if( LED_BLINK_ENABLE == 1)
	BaseType_t xRetVal;
	static TaskHandle_t xHandleLedTask = NULL;

	xRetVal = xTaskCreate(
			&LED_blink_task, 	/* task handler pointer */
			"LED_BLINK",		/* task name */
			256,			/* stack size in words */
			(void *)0,		/* param for task */
			LED_BLINK_PRIO,		/* priority of task */
			&xHandleLedTask		/* task handle variable */
			);
	if(xRetVal == 0)
	{
		while(1);
	}
#endif



	return;
}

