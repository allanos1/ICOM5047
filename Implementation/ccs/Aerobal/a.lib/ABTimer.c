/*
 * ABTimer.c
 *
 *  Created on: Mar 28, 2014
 *      Author: Administrator
 */

#include "ABTimer.h"

int ABTimerInited = 0;

ABTime ABTimerTimeMake(){
	ABTime abt ;
	abt.hours = 0;
	abt.minutes = 0;
	abt.seconds = 0;
	abt.milliseconds = 0;
	abt.microseconds = 0;
	return abt;
}



void ABTimerInit(uint32_t timer, uint32_t resolution){

	ABTimerResolution = resolution;
	switch(ABTimerResolution){
		case ABTIMER_RESOLUTION_MICROSECOND:
			timerSetup(timer,TIMER_CONFIG_PERIODIC,1000000);
			break;
		case ABTIMER_RESOLUTION_MILLISECOND:
			timerSetup(timer, TIMER_CONFIG_PERIODIC,1000);
			break;
		default:
			return;
	}
	ABTimerMain = ABTimerTimeMake();
	ABTimerInited = 1;
}
void ABTimerInterruptHandler_Counter(){
	ABTimerMain = ABTimerIncrease(ABTimerMain);

}
ABTime ABTimerIncrease(ABTime time){
	switch(ABTimerResolution){
		case ABTIMER_RESOLUTION_MICROSECOND: time.microseconds+=1; break;
		case ABTIMER_RESOLUTION_MILLISECOND: time.milliseconds+=1; break;
		default: break;
	}
	if(time.microseconds >= 1000){
		time.microseconds = 0;
		time.milliseconds++;
	}
	if(time.milliseconds >= 1000){
		time.milliseconds = 0;
		time.seconds++;
	}
	if(time.seconds >= 60){
		time.seconds = 0;
		time.minutes++;
	}
	if(time.minutes >= 60){
		time.minutes = 0;
		time.hours++;
	}

	return time;
}

ABTime  ABTimerGetReference(){
	ABTime referenceTime;
	referenceTime.hours = ABTimerMain.hours;
	referenceTime.minutes = ABTimerMain.minutes;
	referenceTime.seconds = ABTimerMain.seconds;
	referenceTime.milliseconds = ABTimerMain.milliseconds;
	referenceTime.microseconds = ABTimerMain.microseconds;
	return referenceTime;
}

ABTime ABTimerGetDelta(ABTime time1, ABTime time2){
	ABTime delta;
	delta.hours = time1.hours - time2.hours;
	delta.minutes = time1.minutes - time2.minutes;
	delta.seconds = time1.seconds - time2.seconds;
	delta.milliseconds = time1.milliseconds - time2.milliseconds;
	delta.microseconds = time1.microseconds - time2.microseconds;

	if(delta.minutes < 0){
		delta.hours --;
		delta.minutes += 60;
	}
	if(delta.seconds < 0){
		delta.minutes--;
		delta.seconds += 60;
	}
	if(delta.milliseconds < 0){
		delta.seconds--;
		delta.milliseconds+=1000;
	}
	if(delta.microseconds < 0){
		delta.milliseconds--;
		delta.microseconds+=1000;
	}
	return delta;

}

/*
 *
 */
int ABTimerCompare(ABTime time1, ABTime time2){
	if(time1.hours > time2.hours) return 1;
	else if(time1.hours == time2.hours){
		if(time1.minutes > time2.minutes) return 1;
		else if(time1.minutes == time2.minutes){
			if(time1.seconds > time2.seconds) return 1;
			else if(time1.seconds == time2.seconds){
				if(time1.milliseconds > time2.milliseconds) return 1;
				else if(time1.milliseconds == time2.milliseconds){
					if(time1.microseconds > time2.microseconds) return 1;
					else if(time1.microseconds == time2.microseconds){
						return 0;
					}
					else return -1;
				}
				else return -1;
			}
			else return -1;
		}
		else return -1;
	}
	else return -1;
}


ABTime ABTimerMainGet(){
	return ABTimerMain;
}

ABTime ABTimerMainIncrease(){
	ABTimerMain = ABTimerIncrease(ABTimerMain);
}

void ABTimerMainSet(ABTime time){
	ABTimerMain = time;
}
