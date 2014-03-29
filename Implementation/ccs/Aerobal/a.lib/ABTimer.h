/*
 * ABTimer.h
 *
 *  Created on: Mar 28, 2014
 *      Author: Administrator
 */

#ifndef ABTIMER_H_
#define ABTIMER_H_

#include <stdlib.h>
#include "timers.h"

#define ABTIMER_BASE_TIMER_0 TIMER_0
#define ABTIMER_BASE_TIMER_1 TIMER_1
#define ABTIMER_BASE_TIMER_2 TIMER_2
#define ABTIMER_BASE_TIMER_3 TIMER_3
#define ABTIMER_BASE_TIMER_4 TIMER_4
#define ABTIMER_BASE_TIMER_5 TIMER_5

//#define ABTIMER_RESOLUTION_MICROSECOND 0 // TODO: Does not work.
#define ABTIMER_RESOLUTION_MILLISECOND 1

////////////////////////////////////
// API Layer 0
typedef struct abtime {
	int hours;
	int minutes;
	int seconds;
	int milliseconds;
	int microseconds;
} ABTime;

uint32_t ABTimer_BaseTimer;
ABTime ABTimerTimeMain ;
int ABTimerResolution;
int ABTimerRunning;
int ABTimerInited;

void ABTimerInit(uint32_t timer, uint32_t frequency);
void ABTimerInterruptHandler_Counter();
ABTime ABTimerTimeMake();
ABTime ABTimerIncrease(ABTime time);
ABTime ABTimerGetReference();
ABTime ABTimerGetDelta(ABTime time1, ABTime time2);
int ABTimerCompare(ABTime time1, ABTime time2);
ABTime ABTimerTimeMainGet();
void ABTimerStart();
void ABTimerStop();
void ABTimerReset();
int ABTimerIsRunning();
int ABTimerIsInited();

#endif /* ABTIMER_H_ */
