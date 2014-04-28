/*
 * bmp085Array.h
 *
 *  Created on: Apr 3, 2014
 *      Author: JJLB
 */

#ifndef BMP085ARRAY_H_
#define BMP085ARRAY_H_

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "gpio.h"
#include "bmp085.h"
#include "string.h"
#include "uart.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "timers.h"

#define MPSA_PORTA GPIO_PORTA
#define MPSA_PORTB GPIO_PORTB
#define MPSA_PORTC GPIO_PORTC
#define MPSA_PORTD GPIO_PORTD
#define MPSA_PORTE GPIO_PORTE
#define MPSA_PORTF GPIO_PORTF

#define MPSA_PIN0 0x01
#define MPSA_PIN1 0x02
#define MPSA_PIN2 0x04
#define MPSA_PIN3 0x08
#define MPSA_PIN4 0x10
#define MPSA_PIN5 0x20
#define MPSA_PIN6 0x40
#define MPSA_PIN7 0x80

#define BMP085_I2C_ADDRESS  0x77

uint32_t bmp085ArrayGPIO_PortCLK;
uint32_t bmp085ArrayGPIO_Pin_CLK;
uint32_t bmp085ArrayGPIO_PortZF;
uint32_t bmp085ArrayGPIO_Pin_ZF;

float bmp085ArrayTemperature;
float bmp085ArrayPressure;
int bmp085ArrayCurrentSensor;
int bmp085ArraySensorQuantity;


int bmp085ArrayOnPause;
char bmp085ArrayErrorFlag;

void bmp085ArrayInit(uint32_t gpioPortCLK, uint32_t gpioPinCLK, uint32_t gpioPortZF, uint32_t gpioPinZF, int sensorQuantity, int testNumber);
void bmp085ArraySensorSetup(int sensorQuantity);
void bmp085ArrayClockToggle();
void bmp085ArrayNextSensor();
void bmp085ArrayReset();
void bmp085ArraySynchronize();
void bmp085ArraySetSensorQuantity(int sensorQuantity);
int bmp085ArrayGetSensorQuantity();
void bmp085ArraySetCurrentSensor(int sensorIndex);
int bmp085ArrayGetCurrentSensor();
void bmp085ArrayDataRead();
void bmp085ArrayDataReadPosition(int sensorIndex);
float bmp085ArrayGetTemperature();
float bmp085ArrayGetPressure();
void bmp085StartDataAdquisition(int frequency);
void bmp085ArraySampleTimer();


#endif /* BMP085ARRAY_H_ */
