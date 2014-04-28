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
#include "i2c.h"


#define BMP085_I2C_ADDRESS  0x77

#define BMP_I2C_MODULE_0 AB_I2C_MODULE_0
#define BMP_I2C_MODULE_1 AB_I2C_MODULE_1
#define BMP_I2C_MODULE_2 AB_I2C_MODULE_2
#define BMP_I2C_MODULE_3 AB_I2C_MODULE_3

int bmpStructCounter;
float bmpTemperature;
float bmpPressure;
int bmpI2CInited;
int bmpCurrentInstance;
volatile uint_fast8_t bmp085_dataFlag;

void bmp085AppCallback(void* bmp085CallbackData, uint_fast8_t bmp085Status);
void bmp085I2CIntHandler(void);
void bmp085Init(uint32_t i2cModule);
void bmp085DataRead(int index);
float bmp085GetTemperature();
float bmp085GetPressure();


#endif /* BMP085_H_ */
