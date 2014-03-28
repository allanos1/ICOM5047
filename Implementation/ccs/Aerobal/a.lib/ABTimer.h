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

#define ABTIMER_RESOLUTION_MICROSECOND 0
#define ABTIMER_RESOLUTION_MILLISECOND 1

////////////////////////////////////
// API Layer 0
struct abtime {
	int hours;
	int minutes;
	int seconds;
	int milliseconds;
	int microseconds;
};

typedef struct abtime ABTime;
ABTime ABTimerMain ;
int ABTimerResolution;

ABTime ABTimerTimeMake();
void ABTimerInit(uint32_t timer, uint32_t frequency);
void ABTimerInterruptHandler_Counter();
ABTime ABTimerIncrease(ABTime time);
ABTime ABTimerGetReference();
ABTime ABTimerGetDelta(ABTime time1, ABTime time2);
int ABTimerCompare(ABTime time1, ABTime time2);
ABTime ABTimerGetMain();


#endif /* ABTIMER_H_ */
