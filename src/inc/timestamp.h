/* ********************************************************
 *
 * header file for timestamp API
 *
 * Author:		Ameya Phadke
 * Date Created:	6th May 2021
 * Date Modified:	6th May 2021
 *
 * *******************************************************/

#ifndef _TIMESTAMP_H
#define _TIMESTAMP_H

#include "main.h"
#include "tm4c129encpdt.h"

void init_timestamp_timer(void);

uint32_t getTimeStampVal(void);

void TimeStampStart(void);

void TimeStampHalt(void);

void TimeStampRestart(void);

#endif