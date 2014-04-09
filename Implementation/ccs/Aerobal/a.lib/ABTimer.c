/*
 * ABTimer.c
 *
 * Aerobal Time Library
 *
 *  Created on: Mar 28, 2014
 *      Author: Anthony
 */

#include "ABTimer.h"

/////////////////////////////////
// API Layer 0


/*
 * Initalization routine. Initializes the main Aerobal
 * Timer, using the given base timer and the given
 * resolution. One resolution is compatible which is
 * the milliseconds, as of v0.5.
 */
void ABTimerInit(uint32_t baseTimer, uint32_t resolution){

	ABTimerResolution = resolution;
	ABTimer_BaseTimer = baseTimer;
	switch(ABTimerResolution){
		//case ABTIMER_RESOLUTION_MICROSECOND:
		//	timerSetup(baseTimer,TIMER_CONFIG_PERIODIC,1000000);
		//	break;
		case ABTIMER_RESOLUTION_MILLISECOND:
			timerSetup(baseTimer, TIMER_CONFIG_PERIODIC,1000);
			break;
		default:
			return;
	}
	ABTimerTimeMain = ABTimerTimeMake();
	ABTimerInited = 1;
}

/******************************
 * Interrupt Handler:
 * | extern void ABTimerInterruptHandler_Counter();
 * ****************************
 *
 * Interrupt handler that increases the resolution size
 * count in ABTimer's Main Timer.
 *
 */
void ABTimerInterruptHandler_Counter(){
	ABTimerTimeMain = ABTimerIncrease(ABTimerTimeMain);
	timerInterruptClear(ABTimer_BaseTimer);
}

/*
 * Initializes and returns a new copy of a ABTime Struct.
 * This struct contains variables for keeping track of
 * hours, minutes, seconds, milliseconds, and microseconds.
 *
 * Returns:
 * 	ABTime
 */
ABTime ABTimerTimeMake(){
	ABTime abt ;
	abt.hours = 0;
	abt.minutes = 0;
	abt.seconds = 0;
	abt.milliseconds = 0;
	abt.microseconds = 0;
	return abt;
}


/*
 * Increases by one resolution unit a given
 * ABTime struct and returns the result.
 */
ABTime ABTimerIncrease(ABTime time){
	switch(ABTimerResolution){
		//case ABTIMER_RESOLUTION_MICROSECOND:
		//	time.microseconds+=1; break;
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

/*
 * Returns the current time as reference.
 */
ABTime  ABTimerGetReference(){
	ABTime referenceTime;
	referenceTime.hours = ABTimerTimeMain.hours;
	referenceTime.minutes = ABTimerTimeMain.minutes;
	referenceTime.seconds = ABTimerTimeMain.seconds;
	referenceTime.milliseconds = ABTimerTimeMain.milliseconds;
	referenceTime.microseconds = ABTimerTimeMain.microseconds;
	return referenceTime;
}

/*
 * Calculates the delta from two time structs
 * and returns the result as an ABTime Struct.
 */
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
 * Compares two time structs. Returns -1 if time1
 * is less than time two, 0 if equal, and 1 if
 * greater.
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

/*
 * Returns the main ABTime struct used
 * by the ABTimer module library.
 */
ABTime ABTimerTimeMainGet(){
	return ABTimerTimeMain;
}

/*
 * Starts the timer that counts at the given
 * initialization resolution rate.
 */
void ABTimerStart(){
	if(ABTimerInited){
		timerStart(ABTimer_BaseTimer);
		ABTimerRunning = 1;
	}
}

/*
 * Stops the timer.
 */
void ABTimerStop(){
	if(ABTimerInited){
		timerStop(ABTimer_BaseTimer);
		ABTimerRunning = 0;
	}
}

/*
 * Resets the timer value.
 */
void ABTimerReset(){
	ABTimerTimeMain = ABTimerTimeMake();
}

/*
 * Returns whether the ABTimer module is
 * running.
 */
int ABTimerIsRunning(){
	return ABTimerRunning;
}

/*
 * Returns whether the ABTimer module has
 * been inited.
 */
int ABTimerIsInited(){
	return ABTimerInited;
}

