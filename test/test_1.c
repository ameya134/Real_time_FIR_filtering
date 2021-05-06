/* File for test 1 */

#include "main.h"
#include "clock.h"
#include "uart.h"
#include "convolution.h"
#include "timestamp.h"

uint16_t arr_x[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
uint16_t arr_h[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
uint16_t arr_y[16] = {0};

void test1(void)
{
    convolve_16x16(arr_x,arr_h,arr_y);

    return;
}


/* system inits required for running test */
void System_Init(void)
{
    /* Init system clock and set the PLL */
	configure_pll_clock(MOSC_CLOCK_SRC,0U,4U,96U,0U,(4U-1U),5);

    /* Init UART */
	UARTInit((uint32_t)115200);

    init_timestamp_timer();
}



int test_main(void)
{
    uint32_t time1,time2;
    System_Init();

    TimeStampStart();
    time1 = getTimeStampVal();
    test1();
    time2 = getTimeStampVal();
    UARTPrintNumToString(time1);
    UARTSendString("\n\r");
    UARTPrintNumToString(time2);
    UARTSendString("\n\r");

    /* Hang at the loop at the end of test
     * for system state examination */
    while(1);

    return 0;
}