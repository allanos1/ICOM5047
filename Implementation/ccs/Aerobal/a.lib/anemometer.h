/*
 * anemometer.c
 * The Anemometer Time Library for
 * Young 05103
 *
 * v0.8
 *
 *  Created on: Mar 27, 2014
 *      Author: Anthony
 *
 *  TODO: Expand comments.
 */

#ifndef ANEMOMETER_H_
#define ANEMOMETER_H_

#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"
#include "timers.h"
#include "ABTimer.h"


#define ANEMOMETER_PORTA GPIO_PORTA
#define ANEMOMETER_PORTB GPIO_PORTB
#define ANEMOMETER_PORTC GPIO_PORTC
#define ANEMOMETER_PORTD GPIO_PORTD
#define ANEMOMETER_PORTE GPIO_PORTE
#define ANEMOMETER_PORTF GPIO_PORTF

#define ANEMOMETER_PIN0 0x01
#define ANEMOMETER_PIN1 0x02
#define ANEMOMETER_PIN2 0x04
#define ANEMOMETER_PIN3 0x08
#define ANEMOMETER_PIN4 0x10
#define ANEMOMETER_PIN5 0x20
#define ANEMOMETER_PIN6 0x40
#define ANEMOMETER_PIN7 0x80

#define ANEMOMETER_BUFFER_SIZE 40

////////////////////////////////
// API Layer 0
typedef struct {
	int bufferCount;
	float buffer[ANEMOMETER_BUFFER_SIZE];
	float sum;
} FloatBuffer;

uint32_t anemometerPort;
uint32_t anemometerPin;
uint32_t anemometerTimer;
unsigned long anemometerCount;
ABTime anemometerT0 ;
ABTime anemometerT1 ;

void anemometerInit(uint32_t gpioPort, uint32_t gpioPin);
void anemometerInterruptHandler();
void anemometerStart();
void anemometerEnd();
int anemometerGetCount();
ABTime anemometerGetTimeDelta();
ABTime anemometerGetCurrentDelta();
float anemometerSpeedGet();
void anemometerReset();

/////////////////////////////
// API Layer 1
void anemometerSpeedBufferAdd(float speed);
float anemometerSpeedBufferGetAverage();
ABTime anemometerSpeedBufferRefresh();

/////////////////////////////
// API Layer 2
float anemometerSpeedConvertMiH(float speed);
float anemometerSpeedConvertKmH(float speed);

#endif /* ANEMOMETER_H_ */
