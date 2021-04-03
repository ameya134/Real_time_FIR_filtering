#ifndef BSP_H
#define BSP_H

#include <stdint.h>
#include "tm4c129encpdt.h"

#define GPIO_PORT_N 12
#define GPIO_PORT_F 5


/*connected to port N*/
#define LED1_PIN	1
#define LED1_PORT GPIO_PORT_N

#define LED2_PIN	0
#define LED2_PORT GPIO_PORT_N

/*connected to port F*/
#define LED3_PIN	4
#define LED3_PORT GPIO_PORT_F

#define LED4_PIN	0
#define LED4_PORT GPIO_PORT_F


/* Initialises the led connected to
 * port N pin 1 as gpio output */
void INIT_LED_1(void);
/* Initialises the led connected to
 * port N pin 0 as gpio output */
void INIT_LED_2(void);
/* Initialises the led connected to
 * port F pin 4 as gpio output */
void INIT_LED_3(void);
/* Initialises the led connected to
 * port F pin 0 as gpio output */
void INIT_LED_4(void);


/* ************************************************************
 * This function turns the led on
 *
 * param: portBase	the port letter of the led
 * param: pinNo		the pin no of led pin
 *
 * return: void
 * 
 * brief: Function turns on led connected to port portBase and
 * pin pinNo 
 *
 * ************************************************************/
void LED_TURN_ON(uint8_t portBase, uint8_t pinNO);


/* ************************************************************
 * This function turns the led off
 *
 * param: portBase	the port letter of the led
 * param: pinNo		the pin no of led pin
 *
 * return: void
 * 
 * brief: Function turns off led connected to port portBase and
 * pin pinNo 
 *
 * ************************************************************/
void LED_TURN_OFF(uint8_t portBase, uint8_t pinNO);


/* ************************************************************
 * This function toggles the led
 *
 * param: portBase	the port letter of the led
 * param: pinNo		the pin no of led pin
 *
 * return: void
 * 
 * brief: Function toggles the led connected to port portBase and
 * pin pinNo 
 *
 * ************************************************************/
void LED_TOGGLE_STATE(uint8_t portBase, uint8_t pinNO);

#endif

