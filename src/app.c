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
#include "string.h"

#include "FreeRTOS.h"
#include "task.h"

#include "bsp.h"
#include "adc.h"
#include "timer0.h"
#include "uart.h"



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



void ADC_val_update_task(void *param)
{

	init_timer0((uint32_t) SystemCoreClock/TIMER0_FREQUENCY_HZ);	
	UARTSendString("timer_initialized\n\r");
	ADC_init();
	
	UARTSendString("Welcome\n\rADC val:\n\r");

	for(;;){
		UARTSendString("\b\b\b\b");
		UARTPrintNumToString((uint32_t) ADC_get_val());
		vTaskDelay(500);

	}

	return;
}



void app_tasks_setup(void)
{

	BaseType_t xRetVal;

#if( LED_BLINK_ENABLE == 1)
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

	static TaskHandle_t xHandleADCTask = NULL;

	xRetVal = xTaskCreate(
			&ADC_val_update_task, 	/* task handler pointer */
			"LED_BLINK",		/* task name */
			256,			/* stack size in words */
			(void *)0,		/* param for task */
			LED_BLINK_PRIO-1,	/* priority of task */
			&xHandleADCTask		/* task handle variable */
			);
	if(xRetVal == 0)
	{
		while(1);
	}


	UARTSendString("Tasks Initialized. Starting Scheduler");
	return;
}


