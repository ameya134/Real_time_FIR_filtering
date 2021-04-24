/* *****************************************************************
 * 
 * Header file for app.c
 *
 * Author:		Ameya Phadke
 * Date Created:	4th April 2021
 * Date Modified:	4th April 2021
 *
 * *****************************************************************/

#ifndef _APP_H
#define _APP_H


#include "tm4c129encpdt.h"
#include "FreeRTOSConfig.h"


#define LED_BLINK_ENABLE 1
#define LED_BLINK_DELAY_MS	1000
#define LED_BLINK_PRIO		tskIDLE_PRIORITY+1


#define TIMER0_FREQUENCY_HZ	44100


#define configADC_INT_PRIO	(configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1)
#define FIR_FILTER_PRIO		LED_BLINK_PRIO+1




void app_tasks_setup(void);


#endif
