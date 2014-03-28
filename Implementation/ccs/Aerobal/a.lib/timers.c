/*
 * timers.c
 *
 *  Created on: Mar 9, 2014
 *      Author: Administrator
 */

#include "timers.h"

////////////////////////////////////////
// API Layer 0

/*
 * Gets the base of the timer as in TI's library.
 *
 * Args:
 * timer - the timer TIMER_# where # = 0...5
 */
uint32_t timerGetTimerBase(uint32_t timer){
	switch(timer){
		case TIMER_0: return TIMER0_BASE;
		case TIMER_1: return TIMER1_BASE;
		case TIMER_2: return TIMER2_BASE;
		case TIMER_3: return TIMER3_BASE;
		case TIMER_4: return TIMER4_BASE;
		case TIMER_5: return TIMER5_BASE;
		default: break;
	}
	return 0;
}


/*
 * Gets timer periodicity: periodic or one shot. Returns equivalent
 * of ti's library.
 *
 * Args:
 * type - either TIMER_CONFIG_PERIODIC or TIMER_CONFIG_ONESHOT
 *
 */
uint32_t timerGetTimerPeriodicity(int type){
	switch(type){
		case TIMER_CONFIG_PERIODIC: return TIMER_CFG_PERIODIC;
		case TIMER_CONFIG_ONESHOT: return TIMER_CFG_ONE_SHOT;
		default: break;
	}
	return 0;
}

/* Gets the interrupt base for the timer as used in
 * the timer library of TI.
 * Args:
 * timer - the timer TIMER_# where # = 0...5
 */
uint32_t timerGetInterruptBase(uint32_t timer){
	switch(timer){
		case TIMER_0: return INT_TIMER0A;
		case TIMER_1: return INT_TIMER1A;
		case TIMER_2: return INT_TIMER2A;
		case TIMER_3: return INT_TIMER3A;
		case TIMER_4: return INT_TIMER4A;
		case TIMER_5: return INT_TIMER5A;
		default: break;
	}
	return 0;
}

/////////////////////////////////////
// API Layer 1


/* Enables a timer.
 * Use TIMER_# where # = 0...5.
 * Args:
 * timer - the timer TIMER_# where # = 0...5
 */
void timerPeripheralEnable(uint32_t timer){
	switch(timer){
		case TIMER_0: SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0); break;
		case TIMER_1: SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1); break;
		case TIMER_2: SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2); break;
		case TIMER_3: SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3); break;
		case TIMER_4: SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER4); break;
		case TIMER_5: SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER5); break;
		default: break;
	}

	SysCtlDelay(20);
	return;
}

/*
 * Configures the periodicity of a timer to either
 * periodic or one-shot.
 *
 * Args:
 * timer - timer TIMER_# where # = 0...5
 * type - either TIMER_CONFIG_PERIODIC or TIMER_CONFIG_ONESHOT
 */
void timerSetPeriodicity(uint32_t timer, int type){
	TimerConfigure(timerGetTimerBase(timer),timerGetTimerPeriodicity(type));
}

/*
 * Configures the frequency of the timer. Hardcoded to timer A.
 *
 * Args:
 * timer - timer TIMER_# where # = 0...5
 * frequency - the frequency of the timer.
 */
void timerSetFrequency(uint32_t timer, float frequency){
	TimerLoadSet(timerGetTimerBase(timer), TIMER_A, SysCtlClockGet()/frequency);
}

/*
 * Enables the interrupts of the desired timer.
 * Hardcoded to timer A.
 *
 * Args:
 * timer - timer TIMER_# where # = 0...5
 */
void timerInterruptEnable(uint32_t timer){
    IntEnable(timerGetInterruptBase(timer));
    TimerIntEnable(timerGetTimerBase(timer), TIMER_TIMA_TIMEOUT);
}


/*
 * Enables the timer and system wide interrupts. Timer begins execution.
 *
 * * System wide interrupts are necessary for timers to be
 * detected. May change in later versions as further understanding
 * of timers happen as the timer may continue execution without
 * calling its interrupt handler.
 *
 * Args:
 * timer - the timer to enable its base.
 */
void timerEnable(uint32_t timer){
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC |
						SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
	IntMasterEnable();
	SysCtlDelay(20);
	TimerEnable(timerGetTimerBase(timer), TIMER_A);
}

/*
 * Clears the timer interrupt from the designated timer.
 *
 */
void timerInterruptClear(uint32_t timer){
	TimerIntClear(timerGetTimerBase(timer), TIMER_TIMA_TIMEOUT);
}

/*
 * Disables the interrupts for the specified timer.
 *
 * Args:
 * timer - the timer to disable its interrupt.
 */
void timerInterruptDisable(uint32_t timer){
	IntDisable(timerGetInterruptBase(timer));
	TimerIntDisable(timerGetInterruptBase(timer), TIMER_TIMA_TIMEOUT);
}

/*
 * Disables the timer specified.
 *
 * Args:
 * timer - the timer to disable.
 */
void timerDisable(uint32_t timer){
	TimerDisable(timerGetTimerBase(timer), TIMER_A);
}


/////////////////////////////////
// API Layer 2


/*
 * Registers a timer by configuring its periodicity type, setting its frequency,
 * enabling its interrupts, and enabling the timer peripheral.
 *
 * Args:
 * timer - timer TIMER_# where # = 0...5
 * periodicityType - either TIMER_CONFIG_PERIODIC or TIMER_CONFIG_ONESHOT
 * frequency - the frequency of the timer. (Hz)
 *
 */
void timerSetup(uint32_t timer, uint32_t periodicityType, float frequency){
	timerPeripheralEnable(timer);
	timerSetPeriodicity(timer, periodicityType);
	timerSetFrequency(timer, frequency);
	timerInterruptEnable(timer);
}

/*
 * Start the timer.
 * Be sure to init the timer with timerSetup.
 *
 * Args:
 * timer - the timer to start.
 */
void timerStart(uint32_t timer){
	timerInterruptEnable(timer);
	timerEnable(timer);
}

/*
 * Stop the timer.
 * Disables interrupt. Only use, start and stop.
 *
 * Args:
 * timer - the timer to stop.
 *
 */
void timerStop(uint32_t timer){
	timerDisable(timer);
	timerInterruptDisable(timer);
}

///////////////////////////////////////////
// API Layer 3

