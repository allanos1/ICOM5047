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
//#include "driverlib/debug.h"
//#include "inc/hw_types.h"
//#include "driverlib/systick.h"
//#include "driverlib/uart.h"
//#include "utils/uartstdio.h"
//#include "sensorlib/hw_bmp180.h"
//#include "sensorlib/i2cm_drv.h"
//#include "sensorlib/bmp180.h"
#include "timers.h"
#include "ABPortsInterruptHandler.h"


#define BMP085_I2C_ADDRESS  0x77

uint32_t bmp085ArrayGPIO_PortCLK;
uint32_t bmp085ArrayGPIO_Pin_CLK;
uint32_t bmp085ArrayGPIO_PortZF;
uint32_t bmp085ArrayGPIO_Pin_ZF;

float bmp085ArrayTemperature;
float bmp085ArrayPressure;
int bmp085ArraySensorQuantity;

void bmp085ArrayInit(uint32_t gpioPortCLK, uint32_t gpioPinCLK, uint32_t gpioPortZF, uint32_t gpioPinZF, int sensorQuantity);
void bmp085ArraySampleTimer();
void bmp085ArrayClockToggle();
void bmp085ArrayNextSensor();
void bmp085ArraySynchronize();
void bmp085ArrayReset();
void bmp085StartDataAdquisition(int frequency);
void bmp085ArraySetSensorQuantity(int sensorQuantity);
int bmp085ArrayGetSensorQuantity();
void bmp085ArraySetCurrentSensor(int sensorIndex);
int bmp085ArrayGetCurrentSensor();
void bmp085ArrayDataRead();
void bmp085ArrayDataReadPosition(int sensorIndex);
float bmp085ArrayGetTemperature();
float bmp085ArrayGetPressure();
void bmp085ArraySensorSetup(int sensorQuantity);
void bmp085ArrayTest(int sensorQuantity);

#endif /* BMP085ARRAY_H_ */
