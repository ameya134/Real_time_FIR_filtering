/* ********************************************************
 * This file provides the funciton prototypes for 
 * configuration and use of pwm module which is present on
 * tm4c129encpdt 
 *
 * Author:		Ameya Phadke
 * Date created:	13th Dec 2020
 * Last modified:	8th Jan 2021
 *
 * ********************************************************/


#ifndef _PWM_H
#define _PWM_H


/* ********************************************************************
 * This function initializes the pwm unit and led
 *
 * param: period	time period in ms of the pwm cycle.
 *
 * param: duty		duty cycle in percentage for the pwm.
 *
 * return: void
 * 
 * brief: This function configures and enables the pwm module and the
 * gpio module required to send the pwm output to led on port F pin 4. 
 * port F pin 4 is configured as a tertiary function pin which is driven
 * by the pwm module.
 * 
 * ********************************************************************/
void PWMLedInit(uint32_t period, uint8_t duty);


/* ********************************************************************
 * This function updates the duty cycle of pwm
 *
 * param: duty		duty cycle in percentage for the pwm.
 *
 * return: void
 * 
 * brief: Updates the compare register t update the duty cycle of the
 * pwm signal.
 * 
 * ********************************************************************/
void PWMLedDutyUpdate(uint8_t duty);


/* ********************************************************************
 * This function initializes the pwm unit and led
 *
 * param: void
 *
 * return: duty  returns the duty cycle of the pwm signal
 * 
 * brief: Calculates and returns the current duty cycle value from 
 * compare and load registers.
 * 
 * ********************************************************************/
uint8_t PWMGetDuty(void);


#endif


