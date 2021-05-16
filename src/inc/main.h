/* PROJECT HEADER FILE FOR IMPORTAT CONSTANTS */


#ifndef _MAIN_H
#define _MAIN_H

#include "stdint.h"
#include "clock.h"


/* SYSTEM RELATED MACROS ARE DECLARED HERE */

/* clock source selection macro */
#define SYS_CLOCK_SRC MOSC_CLOCK_SRC /* Use main external oscilator of 25MHz */

/* system clock frequency */
#define SYSCLOCK_FREQ_Hz 120000000 /* Sysclock frequency is 120 MHz */

/* Set this macro to 1 to call the test function */
#define PERFORM_TEST 0


#endif
