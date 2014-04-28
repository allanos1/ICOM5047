/*
 * ABTime.h
 *
 *  Created on: Mar 28, 2014
 *      Author: Administrator
 */

#ifndef ABTIME_H_
#define ABTIME_H_

#include <stdlib.h>
#include "timers.h"

#define ABTIME_BASE_TIMER_0 TIMER_0
#define ABTIME_BASE_TIMER_1 TIMER_1
#define ABTIME_BASE_TIMER_2 TIMER_2
#define ABTIME_BASE_TIMER_3 TIMER_3
#define ABTIME_BASE_TIMER_4 TIMER_4
#define ABTIME_BASE_TIMER_5 TIMER_5

//#define ABTIME_RESOLUTION_MICROSECOND 0 // TODO: Does not work.
#define ABTIME_RESOLUTION_MILLISECOND 1

////////////////////////////////////
// API Layer 0
typedef struct {
	int hours;
	int minutes;
	int seconds;
	int milliseconds;
	int microseconds;
} ABTime;

uint32_t ABTime_BaseTimer;
ABTime ABTimeTimeMain ;
int ABTimeResolution;
int ABTimeRunning;
int ABTimeInited;

void ABTimeInit(uint32_t timer, uint32_t frequency);
void ABTimeInterruptHandler_Counter();
ABTime ABTimeTimeMake();
ABTime ABTimeIncrease(ABTime time);
ABTime ABTimeGetReference();
ABTime ABTimeGetDelta(ABTime time1, ABTime time2);
int ABTimeCompare(ABTime time1, ABTime time2);
ABTime ABTimeTimeMainGet();
void ABTimeStart();
void ABTimeStop();
void ABTimeReset();
int ABTimeIsRunning();
int ABTimeIsInited();

#endif /* ABTIME_H_ */
