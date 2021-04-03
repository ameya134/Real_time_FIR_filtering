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

/*
 * void ADC_init (void)
 *
 * Inittializes the ADC0 module for software triggered transfers on
 * AIN0 pin on GPIO PE3.
 * 
 *
 * */

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
	ADC0_EMUX_R |= (ADC_EMUX_EM0_M & ADC_EMUX_EM0_ALWAYS); /*default*/
	ADC0_SSMUX0_R |= (0x0 << ADC_SSMUX0_MUX0_S);
	ADC0_SSCTL0_R |= (ADC_SSCTL0_IE0 | ADC_SSCTL0_END0);

	/* enable SS0 */
	ADC0_ACTSS_R |= ADC_ACTSS_ASEN0;

}


uint16_t ADC_get_val(void)
{

	/* TRIGGERT SS0 USING SOFTWARE */
	ADC0_PSSI_R = ADC_PSSI_SS0;

	/* wait while Raw Interrupt status is 0 */
	while(!(ADC0_RIS_R & ADC_RIS_INR0));
	/* clear the RIS by Setting ISC register bit */
	ADC0_ISC_R |= (ADC_ISC_IN0);

	return (ADC_SSFIFO0_DATA_M & ADC0_SSFIFO0_R);
}


