/* **************************************************************
 *
 * This file defines the functions needed to configure and use
 * the DMA peripheral on tm4c129encpdt microcontroller.
 *
 * Author:		Ameya Phadke
 * Date Created:	15th April 2021
 * Date Modified:	15th April 2021
 *
 * *************************************************************/


#include "main.h"
#include "dma.h"


void DMA_init(void)
{
	/* Enable the clock to the dma perpheral */
	SYSCTL_RCGCDMA_R |= SYSCTL_RCGCDMA_R0;
	while(!(SYSCTL_PRDMA_R & SYSCTL_PRDMA_R0));

	/* Set dma master enable bit and set the
	 * channel control base pointer*/
	UDMA_CFG_R |= UDMA_CFG_MASTEN;
	UDMA_CTLBASE_R = (uint32_t) &DMA_control_table;

	return;
}

void DMA_configure_channel(uint8_t channel_no,uint8_t channel_encoding,
			uint32_t *src_end_ptr,uint32_t *dst_end_ptr, struct DMA_control_word *control_word)
			//uint8_t data_size, uint8_t arbsize, uint16_t xfersize)
{

	/* channel priority is default */
	/* UDMA_PRIOSET_R |= (1U << channel_no) */	

	/* select primary channel control structure */
	UDMA_ALTCLR_R |= (1U << channel_no);
	/* set dma to respond to only burst request */
	UDMA_USEBURSTSET_R |= (1U << channel_no);
	/* clear the mask for the channel to recognize the requests */
	UDMA_REQMASKCLR_R |= (1U << channel_no);

	/* configure channel assignments */
	//(*( (uint32_t*) (((uint32_t)&UDMA_CHMAP0_R) + ((channel_no >> 3)*4)) )) 
	//	|= ( (0xF << (4*(0x07 & channel_no)) ) & channel_encoding );

	(*( (uint32_t*) (((uint32_t)&UDMA_CHMAP0_R) + ((channel_no >> 3)*4)) )) 
		|= ( (0xF & channel_encoding) << (4*(0x07 & channel_no)));
	/* configure the channel control structure */
	DMA_control_table.channel_ctl_struct[channel_no].src_end_ptr = src_end_ptr;
	DMA_control_table.channel_ctl_struct[channel_no].dst_end_ptr = dst_end_ptr;
	DMA_control_table.channel_ctl_struct[channel_no].control_word = *control_word;

	return;
}


void DMA_start_transfer(uint8_t channel_no)
{
	UDMA_ENASET_R |= (1U << channel_no);
}
