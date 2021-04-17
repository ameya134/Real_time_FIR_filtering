/* ************************************************************************
 *
 * Header file for DMA module
 *
 * Author:		Ameya Phadke
 * Date Created:	15th April 2021
 * DAte Modified:	15th April 2021
 *
 * ************************************************************************/


#ifndef _DMA_H
#define _DMA_H

#include "tm4c129encpdt.h"


/* Structures for DMA channel control table */
struct DMA_control_word{

	uint8_t DESTINC		: 2; /* bit 31:30 destination address increment */
	uint8_t DESTSIZE	: 2; /* bit 29:28 destination data size */
	uint8_t SRCINC		: 2; /* bit 27:26 source address increment */
	uint8_t SRCSIZE		: 2; /* bit 25:24 source data size */
	uint8_t reserved0	: 2; /* bit 23:22 reserved */
	uint8_t DESTPROT0	: 1; /* bit 21 destination privilage access */
	uint8_t reserved1	: 2; /* bit 20:19 reserved */
	uint8_t SRCPROT0	: 1; /* bit 18 source privilage access */
	uint8_t ARBSIZE		: 4; /* bit 17:14 arbitration size */
	uint16_t XFERSIZE	: 10;/* bit 13:04 transfer size */
	uint8_t NXTUSEBURST	: 1; /* bit 03 next use burst */
	uint8_t XFERMODE	: 3; /* bit 02:00 uDMA transfer mode */

};


struct DMA_control_structure{
	
	uint32_t *src_end_ptr; /* pointer to the end of source buffer or source addr */
	uint32_t *dst_end_ptr; /* pointer to the end of destination buffer or destination addr */
	struct DMA_control_word control_word; /* conrol word for the channel */

};


struct DMA_control_table{

	struct DMA_control_structure channel_ctl_struct[32];

} __attribute((aligned(1024))) DMA_control_table;



void DMA_init(void);

void DMA_configure_channel(uint8_t channel_no,uint8_t channel_encoding,
		uint32_t *src_end_ptr,uint32_t *dst_end_ptr, struct DMA_control_word *control_word);
		//uint8_t data_size, uint8_t arbsize, uint16_t xfersize);

void DMA_start_transfer(uint8_t channel_no);

#endif
