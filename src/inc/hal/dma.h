/* ************************************************************************
 * This file provides the declarations to configure and use the DMA module
 * on tm4c129encpdt
 *
 * Author:			Ameya Phadke
 * Date Created:	15th April 2021
 * DAte Modified:	13th May 2021
 *
 * ************************************************************************/


#ifndef _DMA_H
#define _DMA_H

#include "tm4c129encpdt.h"


/* Structures for DMA channel control table */

/* Control word is used to configure the basic properties
 * of DMA transfer for a given channel. */
struct DMA_control_word{

	/* First member of the structure goes as LSB
	 * Last member of the structure goes as MSB */

	uint16_t XFERMODE		: 3;	/* bit 02:00 uDMA transfer mode */
	uint16_t NXTUSEBURST	: 1;	/* bit 03 next use burst */
	uint16_t XFERSIZE		: 10;	/* bit 13:04 transfer size */
	uint16_t ARBSIZE		: 4;	/* bit 17:14 arbitration size */
	uint16_t SRCPROT0		: 1;	/* bit 18 source privilage access */
	uint16_t reserved1		: 2;	/* bit 20:19 reserved */
	uint16_t DESTPROT0		: 1;	/* bit 21 destination privilage access */
	uint16_t reserved0		: 2;	/* bit 23:22 reserved */
	uint16_t SRCSIZE		: 2;	/* bit 25:24 source data size */
	uint16_t SRCINC			: 2;	/* bit 27:26 source address increment */
	uint16_t DESTSIZE		: 2;	/* bit 29:28 destination data size */
	uint16_t DESTINC		: 2;	/* bit 31:30 destination address increment */

} __attribute__((packed));



/* Control structure for a channel consists of source and destination
 * pointers and the control word. */
struct DMA_control_structure{
	
	uint32_t *src_end_ptr;					/* pointer to the end of source buffer or source addr */
	uint32_t *dst_end_ptr;					/* pointer to the end of destination buffer or destination addr */
	struct DMA_control_word control_word;	/* conrol word for the channel */
	uint32_t reserved;
};



/* Control table consists of primary and secondary control structures
 * for all the channels. control table needs to be aligned at 1024 
 * byte boundary */
struct DMA_control_table{

	struct DMA_control_structure channel_ctl_struct[32];			/* primary control structs */
	struct DMA_control_structure secondary_channel_ctl_struct[32];	/* secondary control structs */

} __attribute((aligned(1024))) DMA_control_table;




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
void DMA_init(void);



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
		uint32_t *src_end_ptr, uint32_t *dst_end_ptr,
		struct DMA_control_word *control_word);



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
                struct DMA_control_word *control_word);



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
void DMA_start_transfer(uint8_t channel_no);



#endif
