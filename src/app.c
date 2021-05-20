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


/* This variable keeps track of buffers being used by hardware
 * Value of 0 represents hardware using buffer A
 * Value of 1 represents hardware using buffer B
 * The software uses the other buffer for calculations */
volatile int ADC_buf_var = 0;

/* Input buffers A and B */
int32_t INPUT_BUFFER_A[DATA_BUF_LEN];
int32_t INPUT_BUFFER_B[DATA_BUF_LEN];

/* Impulse response of FIR filter */
int32_t FIR_h_n[FILTER_LEN] = {-219,324,-24,1165,3469,6579,9625,11522,11522,9625,6579,3469,1165,-24,-324,-219};

/* Output buffers A and B */
int32_t OUTPUT_BUFFER_A[CONVOLUTION_LEN];
int32_t OUTPUT_BUFFER_B[CONVOLUTION_LEN];

/* Pointers to the input and output buffers to be used
 * by software for calculations */
int32_t *INPUT_BUFFER_ptr;
int32_t *OUTPUT_BUFFER_ptr;

/* This semaphore is released by ADC ISR when DATA_BUF_LEN 
 * number of ADC samples have been transfered to inpt buffers */
SemaphoreHandle_t ADC_data_ready;

/* DMA control words for ADC and TIMER0 channel of DMA */
extern struct DMA_control_word ADC_channel_control_word;
extern struct DMA_control_word TIMER0_channel_control_word;



/* *******************************************************************************
 *
 * This function does the setup required by the FIR_filter_task
 *
 * param: void
 *
 * return: void
 * 
 * brief: This function initializes the ADC_data_ready semaphore. It then
 * configures the ADC dma channels to transfer the ADC samples to input buffers.
 * DMA transfers are started and then timer0 is enabled which triggers ADC SS0
 * to sample AIN0 pin.
 * 
 * ******************************************************************************/
static void FIR_task_setup(void)
{
	UARTSendString("FIR filtering started...\n\r");

	ADC_data_ready = xSemaphoreCreateBinary();

	DMA_configure_channel( ADC_DMA_CHANNEL_NO,				/* channel no 14 for adc0 ss0 */
				ADC_DMA_CHANNEL_ENCODE,				/* channel coding 0 for adc0 ss0 */
				1,						/* Use burst only */
				(uint32_t *)&ADC0_SSFIFO0_R,			/* source end pointer */
				(uint32_t *)&INPUT_BUFFER_A[DATA_BUF_LEN -1],	/* destination end pointer */
				&ADC_channel_control_word			/* channel control word */
			);

	DMA_configure_channel( TIMER0_DMA_CHANNEL_NO,				/* channel no 14 for adc0 ss0 */
				TIMER0_DMA_CHANNEL_ENCODE,			/* channel coding 0 for adc0 ss0 */
				1,						/* Use burst only */
				(uint32_t *)&OUTPUT_BUFFER_A[DATA_BUF_LEN -1],	/* source end pointer */
				(uint32_t *)&PWM0_0_CMPA_R,			/* destination end pointer */
				&TIMER0_channel_control_word			/* channel control word */
			);

	/* start the dma transfer */
	DMA_start_transfer(ADC_DMA_CHANNEL_NO);
	DMA_start_transfer(TIMER0_DMA_CHANNEL_NO);

	timer0_enable();

	return;
}



/* *****************************************************************************
 *
 * This function pre-processes the output buffer before the main processing
 *
 * param: void
 *
 * return: void 
 * 
 * brief: This function prepares the output buffers before the convolution 
 * with impulse response is computed. This function implements the OAM method.
 * OVERLAP ADD METHOD(OAM) is used to make sure the effect of preious samples
 * is propagated to the full length in time of the impulse response.
 * 
 * ******************************************************************************/
static void FIR_buffer_prepare(void)
{
	/* use buffer A and B alternately
	* copy last FILTER_LEN-1 samples from
	* previous computations which are carried
	* forward in overlap ad method */
	if(ADC_buf_var == 1){
		memcpy(OUTPUT_BUFFER_A,
		(&OUTPUT_BUFFER_B[DATA_BUF_LEN]),
		(FILTER_LEN-1)*sizeof(*OUTPUT_BUFFER_A));
		
		INPUT_BUFFER_ptr = INPUT_BUFFER_A;
		OUTPUT_BUFFER_ptr = OUTPUT_BUFFER_A;
	}
	else{
		memcpy(OUTPUT_BUFFER_B,
		(&OUTPUT_BUFFER_A[DATA_BUF_LEN]),
		(FILTER_LEN-1)*sizeof(*OUTPUT_BUFFER_B));
		
		INPUT_BUFFER_ptr = INPUT_BUFFER_B;
		OUTPUT_BUFFER_ptr = OUTPUT_BUFFER_B;
	}
	/* clear the remaining array elements */
	memset(&OUTPUT_BUFFER_ptr[FILTER_LEN - 1],
		0,
		(CONVOLUTION_LEN - (FILTER_LEN -1))*sizeof(*OUTPUT_BUFFER_ptr));
	
	return;
}



/* *******************************************************************************
 *
 * This is a FreeRTOS task.
 *
 * param: *param	pointer to the parameters that are passed to the task
 *
 * return: void
 * 
 * brief: This task initializes the dma channels and starts the timer0 during
 * the setup. The tasks keeps pending on the ADC_data_ready semaphore which
 * is released by the ADC isr when sampling of DATA_BUF_LEN number of AIN0 is
 * done. the FIR_buffer_prepare pre-processes the output buffer to implement
 * overlap add method. After this convolution of input signal and the impulse
 * response is performed. After this the output data is scaled and then the
 * task again keeps pending on the semaphore
 * 
 * ******************************************************************************/
void FIR_filter_task(void *param)
{
	/* setup for fir filter task */
	FIR_task_setup();

	for(;;){
		
		/* wait for semaphore to be released when 
		 * DMA completes ADC data transfer */
		if(pdTRUE == xSemaphoreTake(ADC_data_ready,0xffff)){
			/* process the data from ADC */

			/* use buffer that has been updated by uDMA */
			FIR_buffer_prepare();

			convolve_16x16(INPUT_BUFFER_ptr,FIR_h_n,OUTPUT_BUFFER_ptr);

			/* scale the output */
			int i;
			for(i=0;i<16;i++){
				OUTPUT_BUFFER_ptr[i] >>= 8;
			}

		}else{
			/* error aquiring the semaphore */
			UARTSendString("ERROR: Unable to aquire ADC data semaphore\n\r");
			while(1);
		}
		
		
	}

	return;
}



/* *******************************************************************************
 *
 * This task blinks the led
 *
 * param: *param	pointer to the parameters that are passed to the task
 *
 * return: void
 * 
 * brief: This function keeps toggles the state of led at fixed intervals
 * 
 * ******************************************************************************/
#if( LED_BLINK_ENABLE == 1)
void LED_blink_task(void *param)
{

	const TickType_t xDelay = LED_BLINK_DELAY_MS/portTICK_PERIOD_MS;

	for(;;){
		/* Toggle the led and wait for xDelay period */
		LED_TOGGLE_STATE(LED1_PORT,LED1_PIN);
		vTaskDelay(xDelay);
	}

	return;
}
#endif



/* ********************************************************************************
 *
 * This is the setup function that creates the FreeRTOS tasks
 *
 * param: void
 *
 * return: void
 * 
 * brief: This function creates the FreeRTOS tasks and needs to be called 
 * before starting the FreeRTOS scheduler. It creates the FIR_filter_task.
 * It also creates the LED_blink_task if the LED_BLINK_ENABLE macros is 
 * defined as 1.
 * 
 * ********************************************************************************/
void app_tasks_setup(void)
{

	BaseType_t xRetVal;

#if( LED_BLINK_ENABLE == 1)
	static TaskHandle_t xHandleLedTask = NULL;

	xRetVal = xTaskCreate(
			&LED_blink_task,	/* task handler pointer */
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
			&FIR_filter_task,	/* task handler pointer */
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


	UARTSendString("Tasks Initialized...\n\r");
	UARTSendString("Starting Scheduler...\n\r");
	
	return;
}
