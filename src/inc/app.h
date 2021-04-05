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

#define LED_BLINK_ENABLE 1
#if(LED_BLINK_ENABLE == 1)

#define LED_BLINK_DELAY_MS	1000
#define LED_BLINK_PRIO		tskIDLE_PRIORITY+1

#endif



void app_tasks_setup(void);


#endif
