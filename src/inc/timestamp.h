/* ********************************************************
 *
 * header file for timestamp API
 *
 * Author:		Ameya Phadke
 * Date Created:	6th May 2021
 * Date Modified:	6th May 2021
 *
 * *******************************************************/
#ifndef _TIMESTAMP_H
#define _TIMESTAMP_H



#include "main.h"
#include "tm4c129encpdt.h"



/* ***************************************************************************
 *
 * This function initializes the timer7 in 32-bit, one shot, count up mode.
 *
 * param: void
 *
 * return: void 
 * 
 * brief: This function starts the timer7 in 32-bit, one shot, count up mode
 * to be used for timekeeping. This function needs to be called initially 
 * before calling other timestamp functions.
 * 
 * **************************************************************************/
void init_timestamp_timer(void);



/* ***************************************************************************
 *
 * This function returns the count value of the timer7
 *
 * param: void
 *
 * return: uint32_t		timer7 count value
 * 
 * brief: This function returns count value of timer7 which can be used to 
 * calculate time period between 2 calls to this function.
 * 
 * **************************************************************************/
uint32_t getTimeStampVal(void);



/* ***************************************************************************
 *
 * This function starts the timer7 to count up.
 *
 * param: void
 *
 * return: void 
 * 
 * brief: This function starts the timer7. Timer7 starts counting up in
 * one shot mode.
 * 
 * **************************************************************************/
void TimeStampStart(void);



/* ***************************************************************************
 *
 * This function halts the timer7.
 *
 * param: void
 *
 * return: void 
 * 
 * brief: This function halts the counting of timer7. timer7 retains its 
 * current counting value in TIMER7_TAV_R register.
 * 
 * **************************************************************************/
void TimeStampHalt(void);



/* ***************************************************************************
 *
 * This function restarts the timer7 to count up.
 *
 * param: void
 *
 * return: void 
 * 
 * brief: This function restarts the timer7. the current value of timer7 is
 * set to 0 and then Timer7 starts counting up in one shot mode.
 * 
 * **************************************************************************/
void TimeStampRestart(void);

#endif
