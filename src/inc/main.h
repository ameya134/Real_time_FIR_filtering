/* PROJECT HEADER FILE FOR IMPORTAT CONSTANTS */


#ifndef _MAIN_H
#define _MAIN_H

#include "stdint.h"
#include "clock.h"

/* PROJECT RELATED MACROS ARE DECLARED HERE */

#define SYS_CLOCK_SRC MOSC_CLOCK_SRC /* Using main oscilator of 25MHz */


/* system clock frequency */
#define SYSCLOCK_FREQ_Hz 120000000 /* Sysclock frequency is 120 MHz */

/* system tick period in miliseconds */
#define SYSTICK_PERIOD_ms 10


#endif
