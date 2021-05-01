/* ***************************************************************************
 * This file contains the implementation of functions to send
 * data using the UART 0. compatible with TI's tivaware microcontroller
 * header file
 *
 * Author:		Ameya Phadke
 * Date created:	4th Dec 2020
 * Last modified:	8th Jan 2021
 *
 * ***************************************************************************/

#include "main.h"
#include "tm4c129encpdt.h"

#include "uart.h"
#include "string.h"

/* *******************************************************************************
 * This function initializes the UART module
 *
 * param: baudrate	data transfer rate in bits/sec for UART module
 *
 * return: void
 * 
 * brief: The following function initializes all the necessary gpio and 
 * UART module settings to use UART
 *
 * baud rate is calculated using following formula
 *
 * BRD = BRDI + BRDF = UARTsysclk / (clkdiv * baudrate)
 *
 * where BRDI is the 16 bit integer part and BRDF is the
 * 6 bit fractional part
 *
 * *******************************************************************************/
void UARTInit(uint32_t baudrate){

	/* Enable the power to UART Module and appropriate GPIO port*/

	SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R0;
	while(!(SYSCTL_PRUART_R & SYSCTL_PRUART_R0));

	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0;
	while(!(SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R0));

	/* enable DEN and AFSEL for PA0 and PA1 */
	GPIO_PORTA_AHB_DEN_R	|= ((1U<<1) | (1U<<0));
	GPIO_PORTA_AHB_AFSEL_R	|= ((1U<<1) | (1U<<0));


	/* Set 4bit field in GPIOPCTL reg for tx, rx pins */
	GPIO_PORTA_AHB_PCTL_R	|= ((0x01<<4) | (0x01<<0));
	

	/* set the UART control register and baudrate
	 * first we disable the UART then set the parameters
	 * then reenable the UART
	 *
	 * set bit 9 for Rx enable
	 * set bit 8 for Tx enable
	 * set bit 0 for UART enable*/

	/* disable UART temporarily*/
	UART0_CTL_R &= ~(1U<<0);

	UART0_CTL_R |= ((1U<<9) | (1U<<8));

	UART0_IBRD_R = (uint32_t) SYSCLOCK_FREQ_Hz / (16 * baudrate);
	//float temp = 0.425347;
	//UART0_FBRD_R = (uint32_t) 44;//((temp *64) + 0.5);
	//float temp = 0.104;
	UART0_FBRD_R = (uint32_t) 7;//((temp *64) + 0.5);

	UART0_LCRH_R |= (0x03<<5);

	UART0_CC_R |= (0x00<<0);

	/* enable UART */
	UART0_CTL_R |= (1U<<0);
	
	return;
}


/* ***************************************************************************
 * This function receives a byte of data through UART module
 *
 * param: void
 *
 * return: uint8_t return a byte of data received from uart module
 * 
 * brief: The function return a byte of data received by the UART module on
 * the Rx pin of the microcontroller. A value of 0x00 is returned if no new
 * data is available.
 *
 * **************************************************************************/
uint8_t UARTRecvChar(void){

	/* check if new data is available, 
	 * otherwise return 0 */
	if((UART0_FR_R & (1U<<6))){
		return ((uint8_t) UART0_DR_R);
	}
	
	return 0;
}



/* ******************************************************************************
 * This function sends a byte of data through UART module
 *
 * param: c	byte of data that is to be sent
 *
 * return: void
 * 
 * brief: The functions sends a byte of data through uart module when an ongoing
 * transmission if any is completed.
 *
 * *****************************************************************************/
void UARTSendChar(char c){

	/* check the busy flag */
	while(UART0_FR_R & (1U<<5));
	
	UART0_DR_R = c;
	return;
}


/* *********************************************************************************
 * This function sends a string of data through UART module
 *
 * param: s	pointer to the string/ char array of the data that is to be sent.
 *
 * return: void
 * 
 * brief: The function send the string, pointed by the pointer argument through the
 * UART module. The function keeps sending the next byte until null terminating char
 * is reached. It is expected that the string to be sent is terminated
 * with '\0' by the caller.
 *
 * ********************************************************************************/
void UARTSendString(char *s){

	while(*s != '\0'){
		UARTSendChar(*s);
		s++;
	}

	return;
}


/* **********************************************************************************
 * This function sends a BASE 10 character representation of an integer through UART.
 *
 * param: num	The number that is to be sent through UART.
 *
 * return: void
 * 
 * brief: This function sends the BASE 10 character representation of an integer.
 * This function relies on a function which is used to convert integer into string.
 *
 * *********************************************************************************/
#ifdef _STRING_H
char tempStr[12];

void UARTPrintNumToString(uint32_t num){
	
	convertIntToString(num, tempStr,4);

	UARTSendString(tempStr);
	
	return;
}
#endif


