/* ********************************************************
 *
 * Header file for system related functions
 *
 * Author:		Ameya Phadke
 * Date Created:	6th May 2021
 * Date Modified:	6th May 2021
 *
 * *******************************************************/

#ifndef _SYSTEM_TM4C
#define _SYSTEM_TM4C


/* variable for system clock frequency */
extern uint32_t SystemCoreClock;



/* ***************************************************************************
 *
 * This function calls the initialization functions for different required
 * peripherals.
 *
 * param: void
 *
 * return: void 
 * 
 * brief: This function calls the initialization functions for different
 * peripherals. PLL is configured to change the system clock frequency.
 * 
 * **************************************************************************/
void SystemInit(void);

#endif
