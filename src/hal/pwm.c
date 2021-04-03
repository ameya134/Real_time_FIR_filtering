/* ***********************************************************
 * This file implements the functions used to configure and 
 * use the pwm module present on tm4c129encpdt
 * This file uses TI's provided tm4c129encpdt.h header file
 *
 * Author:			Ameya Phadke
 * Date created:	13th Dec 2020
 * Last modified:	8th Jan 2021
 *
 * ***********************************************************/

#include "tm4c129encpdt.h"
#include "main.h"

#include "pwm.h"
#include "uart.h"



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
void PWMLedInit(uint32_t period, uint8_t duty){


	/* Enable power to the pwm module */
	SYSCTL_RCGCPWM_R |= SYSCTL_RCGCPWM_R0;
	while(!(SYSCTL_PRPWM_R & SYSCTL_PRPWM_R0));

	/* Enable power to the gpio module */
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;
	while(!(SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5));

	/* init gpio */

	GPIO_PORTF_AHB_DEN_R	|= (1U<<0);
	GPIO_PORTF_AHB_AFSEL_R	|= (1U<<0);
	GPIO_PORTF_AHB_PCTL_R	|= (0x06<<0);

	/* Init PWM */

	/* disable the pwm before setup*/
	PWM0_0_CTL_R |= (0x0<<0);
	PWM0_0_GENA_R |= ((0x3<<6) | (0x2<<2));

	uint32_t loadVal = (SYSCLOCK_Hz / 1000000) * period;
	uint32_t dutyVal = (duty * loadVal)/100;

	PWM0_0_LOAD_R = loadVal;
	PWM0_0_CMPA_R = dutyVal;

	PWM0_0_CTL_R	|= (1U<<0);
	PWM0_ENABLE_R	|= (1U<<0);
	
	return;
}


/* ********************************************************************
 * This function updates the duty cycle of pwm
 *
 * param: duty		duty cycle in percentage for the pwm.
 *
 * return: void
 * 
 * brief: Updates the compare register to update the duty cycle of the
 * pwm signal.
 * 
 * ********************************************************************/
void PWMLedDutyUpdate(uint8_t duty){

	uint32_t temp;
	
	/* If duty is greater than 100 set it as 100.
	 * load_val -1 is done so as pwm would stop if
	 * compare and load values are same */
	if(duty >= 100){
		temp = PWM0_0_LOAD_R - 1;
	}
	else{
		temp = (duty * PWM0_0_LOAD_R)/100;
	}
	PWM0_0_CMPA_R = temp;

	return;
}


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
uint8_t PWMGetDuty(void){

	uint8_t duty = (100*PWM0_0_CMPA_R)/PWM0_0_LOAD_R;

	return (duty);
}


