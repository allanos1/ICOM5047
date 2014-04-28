/*
 * binCounter.h
 *
 *  Created on: Apr 5, 2014
 *      Author: Administrator
 */

#ifndef BINCOUNTER_H_
#define BINCOUNTER_H_

#include "gpio.h"

#define BINCOUNTER_GPIO_PORTA GPIO_PORTA
#define BINCOUNTER_GPIO_PORTB GPIO_PORTB
#define BINCOUNTER_GPIO_PORTC GPIO_PORTC
#define BINCOUNTER_GPIO_PORTD GPIO_PORTD
#define BINCOUNTER_GPIO_PORTE GPIO_PORTE

#define BINCOUNTER_GPIO_PIN_0 0x01
#define BINCOUNTER_GPIO_PIN_1 0x02
#define BINCOUNTER_GPIO_PIN_2 0x04
#define BINCOUNTER_GPIO_PIN_3 0x08
#define BINCOUNTER_GPIO_PIN_4 0x10
#define BINCOUNTER_GPIO_PIN_5 0x20
#define BINCOUNTER_GPIO_PIN_6 0x40
#define BINCOUNTER_GPIO_PIN_7 0x80

#define BINCOUNTER_FUNCTION_CLEAR 0
#define BINCOUNTER_FUNCTION_INCREASE 1
#define BINCOUNTER_FUNCTION_DECREASE 2


uint32_t _binCounterClockPort;
uint32_t _binCounterClockPin;
uint32_t _binCounterFunction0Port;
uint32_t _binCounterFunction0Pin;
uint32_t _binCounterFunction1Port;
uint32_t _binCounterFunction1Pin;
uint32_t _binCounterDirection;

///////////////////////////////////////
// API Layer 0
void binCounterInit(uint32_t binCounterGpioPortClk, uint32_t binCounterGpioPinClk,
		uint32_t binCounterGpioPortDir1, uint32_t binCounterGpioPinDir1,
		uint32_t binCounterGpioPortDir2, uint32_t binCounterGpioPinDir2);
void binCounterFunctionSetIncrease();
void binCounterFunctionSetDecrease();
void binCounterFunctionSetClear();
void binCounterClockRun();
int binCounterIsIncreasing();
int binCounterIsDecreasing();
int binCounterIsClearing();
void binCounterClockRun();


///////////////////////////////////////
// API Layer 1
void binCounterIncrease();
void binCounterDecrease();
void binCounterClear();

#endif /* BINCOUNTER_H_ */
