/* ****************************************************************
 *
 * This file implements the configuration and use of ADC API.
 * 
 * Author:		Ameya Phadke
 *
 * Date Created:	22nd March 2021
 * Date Modified:	22nd March 2021
 *
 * ****************************************************************/


#include "adc.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/*
 * void ADC_init (void)
 *
 * Inittializes the ADC0 module for software triggered transfers on
 * AIN0 pin on GPIO PE3.
 * 
 *
 * */

#if( ADC_USE_DMA == 1)

uint16_t ADC_udma_buffer_A[ADC_DMA_BUF_LEN]={0};
uint16_t ADC_udma_buffer_B[ADC_DMA_BUF_LEN]={0};

int ADC_buf_var = 1;


struct DMA_control_word ADC_channel_control_word = {

	.XFERMODE	= 0x1, /* Basic mode */
	.NXTUSEBURST	= 0x0, /* no next use burst for last transfers */
	.XFERSIZE	= (ADC_DMA_BUF_LEN -1), /* Transfer size */
	.ARBSIZE	= 0x2, /* arbitrate after 4 transfers (fifo half full for ss0) */
	.SRCPROT0	= 0x0, /* non privilaged access */
	/* .reserved1 */
	.DESTPROT0	= 0x0, /* non privilaged access */
	/* .reserved0 */
	.SRCSIZE	= 0x1, /* 16 bit data size */
	.SRCINC		= 0x3, /* No increment */
	.DESTSIZE	= 0x1, /* 16 bit data size */
	.DESTINC	= 0x1, /* 16 bit increment */
};


void ADC_udma_channel_config(void)
{
	DMA_configure_channel( ADC_DMA_CHANNEL_NO, /* channel no 14 for adc0 ss0 */
		       	ADC_DMA_CHANNEL_ENCODE, /* channel coding 0 for adc0 ss0 */
			(uint32_t *)&ADC0_SSFIFO0_R, /* source end pointer */
			(uint32_t *)&ADC_udma_buffer_A[ADC_DMA_BUF_LEN -1], /* destination end pointer */
			&ADC_channel_control_word /* channel control word */
			);
	return;
}


extern SemaphoreHandle_t ADC_data_ready;
BaseType_t xHigherPriorityTaskWoken;

void ADC0_sequencer0_handler(void)
{
	/* clear the interrupt by writing 1 to ADC0_ISC_R register*/
	ADC0_ISC_R |= (ADC_ISC_DMAIN0);

	/* Use buffers A and B alternately */

	if(ADC_buf_var == 0){
		ADC_buf_var = 1;
		DMA_reconfigure_channel( ADC_DMA_CHANNEL_NO, /* channel no 14 for adc0 ss0 */
				(uint32_t *) &ADC0_SSFIFO0_R, /* source end pointer */
				(uint32_t *) &ADC_udma_buffer_A[ADC_DMA_BUF_LEN -1], /* desetination end pointer */
				&ADC_channel_control_word /* channel control_word */
				);
	}
	else{
		ADC_buf_var = 0;
		DMA_reconfigure_channel( ADC_DMA_CHANNEL_NO, /* channel no 14 for adc0 ss0 */
				(uint32_t *) &ADC0_SSFIFO0_R, /* source end pointer */
				(uint32_t *) &ADC_udma_buffer_B[ADC_DMA_BUF_LEN -1], /* desetination end pointer */
				&ADC_channel_control_word /* channel control_word */
				);

	}


	DMA_start_transfer(ADC_DMA_CHANNEL_NO);

	/* Release the ADC_data_ready semaphore to start
	 * ADC data processing by FIR_filter_task */
	xHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR(ADC_data_ready,&xHigherPriorityTaskWoken);

	if(xHigherPriorityTaskWoken == pdTRUE){
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}

	return;
}

#endif

void ADC_init(void)
{

	/* enable clock to adc 0 module */
	SYSCTL_RCGCADC_R |= SYSCTL_RCGCADC_R0;
	while(!(SYSCTL_PRADC_R & SYSCTL_PRADC_R0));


	/* configure PLL and ADC clock */

	SYSCTL_PLLFREQ0_R &= ~(SYSCTL_PLLFREQ0_PLLPWR);
	SYSCTL_PLLFREQ0_R |= (SYSCTL_PLLFREQ0_MINT_M & (0x1 << SYSCTL_PLLFREQ0_MINT_S));
	SYSCTL_PLLFREQ0_R |= (SYSCTL_PLLFREQ0_PLLPWR);
	SYSCTL_RSCLKCFG_R |= (SYSCTL_RSCLKCFG_NEWFREQ);
	while(!(SYSCTL_PLLSTAT_R & SYSCTL_PLLSTAT_LOCK));

	ADC0_CC_R |= (ADC_CC_CS_M & ADC_CC_CS_SYSPLL);
	
	/* enable clock to port E*/
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4;
	while(!(SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R4));
	
	/* configure gpio PE3 for AIN0
	 * *enable alternate port function  
	 * *disable digital function on the pin 
	 * *select analog mode by disabling analog isolation */
	GPIO_PORTE_AHB_AFSEL_R |= (1U<<3);
	GPIO_PORTE_AHB_DEN_R &= ~(1U<<3);
	GPIO_PORTE_AHB_AMSEL_R |= (1U<<3);

	/* Configure the sample sequencer 0
	 * *Set ALT Clock as clock source
	 * *Disable SS0
	 * *trigger event in ADCEMUX is by default 0 (software trigger)
	 * *Set end bit after last sample and enable raw interrupt status
	 *  without enabling interrupt mask so that software can poll
	 *  raw interrupt status
	 * */

	ADC0_ACTSS_R &= ~(ADC_ACTSS_ASEN0);
	ADC0_EMUX_R |= (ADC_EMUX_EM0_M & ADC_EMUX_EM0_TIMER); /*timer trigger*/
	ADC0_SSMUX0_R |= (0x0 << ADC_SSMUX0_MUX0_S);
	ADC0_SSCTL0_R |= (ADC_SSCTL0_IE0 | ADC_SSCTL0_END0);

#if( ADC_USE_DMA == 1)
	/* Enable the dma for ss0 
	 * Enable the bit mask and nvic register for ss0 
	 * Set the interrupt prio lower (higher number)
	 * than FreeRTOS maxprio for syscall in order to call the FreeRTOS
	 * API from interrupt handler 
	 *
	 * 3 prio bits have been implemented on tm4c129encpdt. bits are located
	 * at MSB portion of corresponding 8 bit field in NVIC prio registers
	 * */

	ADC0_ACTSS_R |= (ADC_ACTSS_ADEN0);
	ADC0_IM_R |= (ADC_IM_DMAMASK0);
	
	NVIC_EN0_R	|= (1U << ADC0_SS0_INT_NUM);
	NVIC_PRI3_R	|= ( ( ADC0_SS0_INT_NVIC_PRIO << (8 - configPRIO_BITS) ) << 2*8);
	
	ADC_udma_channel_config();
#endif
	/* enable SS0 */
	ADC0_ACTSS_R |= ADC_ACTSS_ASEN0;

}


uint16_t ADC_get_val(void)
{

	/* TRIGGERT SS0 USING SOFTWARE */
	//ADC0_PSSI_R = ADC_PSSI_SS0;

	return (ADC_SSFIFO0_DATA_M & ADC0_SSFIFO0_R);
	
}


