/*
 * bmp085.h
 *
 *  Created on: Feb 21, 2014
 *      Author: Administrator
 */

#ifndef BMP085_H_
#define BMP085_H_

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "inc/hw_memmap.h"
//#include "inc/hw_types.h"
#include "inc/hw_ints.h"
//#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
//#include "driverlib/systick.h"
//#include "driverlib/uart.h"
//#include "utils/uartstdio.h"
#include "sensorlib/hw_bmp180.h"
#include "sensorlib/i2cm_drv.h"
#include "sensorlib/bmp180.h"

#define BMP085_I2C_ADDRESS  0x77

float bmpTemperature;
float bmpPressure;

void bmp085AppCallback(void* bmp085CallbackData, uint_fast8_t bmp085Status);
void bmp085I2CIntHandler(void);
void bmp085Init();
void bmp085DataRead();
float bmp085GetTemperature();
float bmp085GetPressure();


#endif /* BMP085_H_ */
