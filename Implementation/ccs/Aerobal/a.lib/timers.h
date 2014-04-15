/*
 * timers.h
 *
 *  Created on: Mar 9, 2014
 *      Author: Administrator
 */

#ifndef TIMERS_H_
#define TIMERS_H_


#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#define TIMER_0 0x00
#define TIMER_1 0x01
#define TIMER_2 0x02
#define TIMER_3 0x03
#define TIMER_4 0x04
#define TIMER_5 0x05

#define TIMER_CONFIG_PERIODIC 0x00
#define TIMER_CONFIG_ONESHOT 0x01


/////////////////////////////////
// API Layer 1
void timerPeripheralEnable(uint32_t timer);
void timerSetPeriodicity(uint32_t timer, int type);
void timerSetFrequency(uint32_t timer, float frequency);
void timerInterruptEnable(uint32_t timer);
void timerEnable(uint32_t timer);
void timerInterruptClear(uint32_t timer);
void timerInterruptDisable(uint32_t timer);
void timerDisable(uint32_t timer);

/////////////////////////////////
// API Layer 2
void timerSetInterruptHandler(uint32_t timer, void (*pfnHandler)(void));
void timerSetup(uint32_t timer, uint32_t periodicityType, float frequency);
void timerStart(uint32_t timer);
void timerStop(uint32_t timer);

//////////////////////////////////
// API Layer 3

#endif /* TIMERS_H_ */
