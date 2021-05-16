/* **************************************************************
 *
 * This file defines the functions needed to configure and use
 * the DMA peripheral on tm4c129encpdt microcontroller.
 *
 * Author:			Ameya Phadke
 * Date Created:	15th April 2021
 * Date Modified:	13th May 2021
 *
 * *************************************************************/

#include "main.h"
#include "dma.h"
#include "hw_types.h"




/* ********************************************************************
 * This function Initializes the DMA module.
 *
 * param: void
 *
 * return: void
 * 
 * brief: This function inititalizes the DMA module and sets the pointer
 * to the control table located in the SRAM.
 * 
 * ********************************************************************/
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



/* ********************************************************************************************
 * This function configures a channel to transfer data from source
 * to destination.
 *
 * param: channel_no		DMA channel to be configured for transfer
 * param: channel_encoding	encoding value for the peripheral connected to channel
 * param: burstModeOnly		Channel responds to only burst requests when this is 1.
 * param: src_end_ptr		pointer to the last memory location of the 
 * 							source buffer or memory location
 * param: dest_end_ptr		pointer to the last memory location of the 
 * 							destination buffer or memory location
 * param: control_word		control word for the configuration of channel
 *
 * return: void
 * 
 * brief: This function configures the DMA channel to transfer data from source location
 * to the destination. the source and/or destination can be single memory locations or
 * arrays of memory locations. The size of elements, transfer size, transfer type is programmed
 * in the control word.
 * 
 * ********************************************************************************************/
void DMA_configure_channel(uint8_t channel_no,uint8_t channel_encoding,uint8_t burstModeOnly,
			uint32_t *src_end_ptr,uint32_t *dst_end_ptr, struct DMA_control_word *control_word)
			//uint8_t data_size, uint8_t arbsize, uint16_t xfersize)
{

	/* channel priority is default */
	/* UDMA_PRIOSET_R |= (1U << channel_no) */	

	/* select primary channel control structure */
	UDMA_ALTCLR_R |= (1U << channel_no);

	/* set dma to respond to burst request */
	if(burstModeOnly){
		UDMA_USEBURSTSET_R |= (1U << channel_no);
	}
	else{
		UDMA_USEBURSTCLR_R |= (1U << channel_no);
	}
	
	/* clear the mask for the channel to recognize the requests */
	UDMA_REQMASKCLR_R |= (1U << channel_no);

	/* configure channel assignments */
	HWREG(((uint32_t)&UDMA_CHMAP0_R) + ((channel_no >> 3)*4))
		|= ( (0xF & channel_encoding) << (4*(0x07 & channel_no)));

	/* configure the channel control structure */
	DMA_control_table.channel_ctl_struct[channel_no].src_end_ptr = src_end_ptr;
	DMA_control_table.channel_ctl_struct[channel_no].dst_end_ptr = dst_end_ptr;
	DMA_control_table.channel_ctl_struct[channel_no].control_word = *control_word;

	return;
}



/* ********************************************************************************************
 * This function reconfigures the dma channel after the previous transfer has been completed.
 *
 * param: channel_no		DMA channel to be configured for transfer
 * param: src_end_ptr		pointer to the last memory location of the 
 * 							source buffer or memory location
 * param: dest_end_ptr		pointer to the last memory location of the 
 * 							destination buffer or memory location
 * param: control_word		control word for the configuration of channel
 *
 * return: void
 * 
 * brief: This function reconfigures the dma channel after the previous transfer has been
 * completed.
 * 
 * ********************************************************************************************/
void DMA_reconfigure_channel(uint8_t channel_no,
		uint32_t *src_end_ptr,
		uint32_t *dst_end_ptr,
		struct DMA_control_word *control_word)
{
	DMA_control_table.channel_ctl_struct[channel_no].src_end_ptr = src_end_ptr;
	DMA_control_table.channel_ctl_struct[channel_no].dst_end_ptr = dst_end_ptr;
	DMA_control_table.channel_ctl_struct[channel_no].control_word = *control_word;

	return;
}



/* ********************************************************************
 * This function starts/enables the DMA transfers on a channel.
 *
 * param: channel_no	Dma channel no to be enabled/ started.
 *
 * return: void
 * 
 * brief: This function starts/ enables the dma channel to responds to
 * transfer requests from peripherals and/or software triggers.
 * ********************************************************************/
void DMA_start_transfer(uint8_t channel_no)
{
	UDMA_ENASET_R |= (1U << channel_no);
}
