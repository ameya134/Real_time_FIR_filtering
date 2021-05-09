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
#include "convolution.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "bsp.h"
#include "adc.h"
#include "timer0.h"
#include "pwm.h"
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


int count=0;
volatile int ADC_buf_var = 0;

uint16_t INPUT_BUFFER_A[DATA_BUF_LEN];
uint16_t INPUT_BUFFER_B[DATA_BUF_LEN];

uint16_t FIR_h_n[FILTER_LEN] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

uint16_t OUTPUT_BUFFER_A[CONVOLUTION_LEN];
uint16_t OUTPUT_BUFFER_B[CONVOLUTION_LEN];

SemaphoreHandle_t ADC_data_ready;

extern struct DMA_control_word ADC_channel_control_word;
extern struct DMA_control_word TIMER0_channel_control_word;

void FIR_filter_task(void *param)
{
	UARTSendString("FIR filtering started...\n\r");

	ADC_data_ready = xSemaphoreCreateBinary();

	DMA_configure_channel( ADC_DMA_CHANNEL_NO, /* channel no 14 for adc0 ss0 */
				ADC_DMA_CHANNEL_ENCODE, /* channel coding 0 for adc0 ss0 */
				1,						/* Use burst only */
				(uint32_t *)&ADC0_SSFIFO0_R, /* source end pointer */
				(uint32_t *)&INPUT_BUFFER_A[DATA_BUF_LEN -1], /* destination end pointer */
				&ADC_channel_control_word /* channel control word */
			);

	DMA_configure_channel( TIMER0_DMA_CHANNEL_NO, /* channel no 14 for adc0 ss0 */
				TIMER0_DMA_CHANNEL_ENCODE, /* channel coding 0 for adc0 ss0 */
				1,						/* Use burst only */
				(uint32_t *)&OUTPUT_BUFFER_A[DATA_BUF_LEN -1], /* source end pointer */
				(uint32_t *)&PWM0_0_CMPA_R, /* destination end pointer */
				&TIMER0_channel_control_word /* channel control word */
			);

	init_timer0((uint32_t) SystemCoreClock/TIMER0_FREQUENCY_HZ);
	/* start the dma transfer */
	DMA_start_transfer(ADC_DMA_CHANNEL_NO);
	DMA_start_transfer(TIMER0_DMA_CHANNEL_NO);

	init_timer0((uint32_t) SystemCoreClock/TIMER0_FREQUENCY_HZ);

	for(;;){
		
		/* wait for semaphore to be released when 
		 * DMA completes ADC data transfer */
		if(pdTRUE == xSemaphoreTake(ADC_data_ready,0xffff)){
			/* process the data from ADC */

			/* use buffer that has been updated by uDMA */
			if(ADC_buf_var == 1){
				memset(OUTPUT_BUFFER_A,0,(CONVOLUTION_LEN)*sizeof(*OUTPUT_BUFFER_A));
				memcpy(OUTPUT_BUFFER_A,
				(OUTPUT_BUFFER_B+DATA_BUF_LEN),
				(FILTER_LEN-1)*sizeof(*OUTPUT_BUFFER_A));

				convolve_16x16(INPUT_BUFFER_A,FIR_h_n,OUTPUT_BUFFER_A);
				int i;
				for(i=0;i<31;i++){
					OUTPUT_BUFFER_A[i] = OUTPUT_BUFFER_A[i]>>8;
				}
			}
			else{
				memset(OUTPUT_BUFFER_B,0,(CONVOLUTION_LEN)*sizeof(*OUTPUT_BUFFER_A));
				memcpy(OUTPUT_BUFFER_B,
				(OUTPUT_BUFFER_A+DATA_BUF_LEN),
				(FILTER_LEN-1)*sizeof(*OUTPUT_BUFFER_B));

				convolve_16x16(INPUT_BUFFER_B,FIR_h_n,OUTPUT_BUFFER_B);
				int i;
				for(i=0;i<31;i++){
					OUTPUT_BUFFER_B[i] = OUTPUT_BUFFER_B[i]>>8;
				}
			}

			count++;
			if(count == TIMER0_FREQUENCY_HZ/DATA_BUF_LEN){
				count = 0;
				LED_TOGGLE_STATE(LED3_PORT,LED3_PIN);
				//UARTSendString("\n\rFIR_TASK");
			}
			/* Test input
			convolve_16x16(test_filter_ip,FIR_h_n,filter_output);*/
			
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


