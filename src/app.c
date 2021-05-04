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
#include "semphr.h"

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



SemaphoreHandle_t ADC_data_ready;
int count=0;
void FIR_filter_task(void *param)
{
	init_timer0((uint32_t) SystemCoreClock/TIMER0_FREQUENCY_HZ);
	ADC_data_ready = xSemaphoreCreateBinary();
	
	UARTSendString("FIR filtering started...\n\r");
	
	/* start the dma transfer */
	DMA_start_transfer(ADC_DMA_CHANNEL_NO);

	for(;;){
		
		/* wait for semaphore to be released when 
		 * DMA completes ADC data transfer */
		if(pdTRUE == xSemaphoreTake(ADC_data_ready,0xffff)){	
			/* process the data from ADC */
			count++;
			if(count == 689){
				count = 0;
				LED_TOGGLE_STATE(LED3_PORT,LED3_PIN);
				UARTSendString("\n\rFIR_TASK");
			}
			LED_TOGGLE_STATE(LED2_PORT,LED2_PIN);
		}else{
			UARTSendString("ERROR: Unable to aquire ADC data semaphore\n\r");
			while(1);
		}
		
		
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

	static TaskHandle_t xHandleFIRFilterTask = NULL;

	xRetVal = xTaskCreate(
			&FIR_filter_task, 	/* task handler pointer */
			"LED_BLINK",		/* task name */
			256,			/* stack size in words */
			(void *)0,		/* param for task */
			FIR_FILTER_PRIO,	/* priority of task */
			&xHandleFIRFilterTask	/* task handle variable */
			);
	if(xRetVal == 0)
	{
		while(1);
	}


	UARTSendString("Tasks Initialized. Starting Scheduler");
	return;
}


