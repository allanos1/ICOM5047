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
//#include "inc/hw_types.h"
#include "inc/hw_ints.h"
//#include "driverlib/debug.h"
#include "gpio.h"
#include "bmp085.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
//#include "driverlib/systick.h"
//#include "driverlib/uart.h"
//#include "utils/uartstdio.h"
//#include "sensorlib/hw_bmp180.h"
//#include "sensorlib/i2cm_drv.h"
//#include "sensorlib/bmp180.h"
#include "timers.h"

#define BMP085_I2C_ADDRESS  0x77
#define sensorQuantity 16

float bmpTemperature;
float bmpPressure;
float errorFlag = 0;

void bmp085ArrayInit();
void bmp085ArraySampleTimer();
void bmp085ArrayDataRead(int sensorIndex);
float bmp085ArrayGetTemperature(int sensorIndex);
float bmp085ArrayGetPressure(int sensorIndex);

#endif /* BMP085ARRAY_H_ */
