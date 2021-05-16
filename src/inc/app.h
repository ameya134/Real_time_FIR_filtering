/* *****************************************************************
 * 
 * Header file for app.c
 *
 * Author:          Ameya Phadke
 * Date Created:	4th April 2021
 * Date Modified:	16th May 2021
 *
 * *****************************************************************/
#ifndef _APP_H
#define _APP_H


#include "tm4c129encpdt.h"
#include "FreeRTOSConfig.h"


/* Macros needed by the application */

/* macros related to led blink task */
#define LED_BLINK_ENABLE 1
#define LED_BLINK_DELAY_MS	1000
#define LED_BLINK_PRIO		tskIDLE_PRIORITY+1


/* ADC sampling frequency used by the application in Hertz */
#define TIMER0_FREQUENCY_HZ	48000 /* 48KHz sampling rate */


#define configADC_INT_PRIO	(configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1)
#define FIR_FILTER_PRIO		LED_BLINK_PRIO+1

/* PWM period for output is 10 times smaller than sampling rate */
#define OUPUT_PWM_PERIOD_CYCLES ((SYSCLOCK_FREQ_Hz/TIMER0_FREQUENCY_HZ)/10)


/* Macros for required data processing */
#define DATA_BUF_LEN    16  /* 16 samples processed at a time */
#define FILTER_LEN      16  /* length of impulse response */
#define CONVOLUTION_LEN (DATA_BUF_LEN + FILTER_LEN -1) /* length of convolution output of input and filter */



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
void app_tasks_setup(void);


#endif
