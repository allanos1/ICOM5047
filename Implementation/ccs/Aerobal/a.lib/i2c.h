/*
 * i2c.h
 *
 * AeroBal I2C Library
 *
 * Used for activating an I2C module.
 * Interface to be provided in later versions.
 *
 *  Created on: Apr 4, 2014
 *      Author: Anthony
 */

#ifndef I2C_H_
#define I2C_H_

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
#include "gpio.h"

#define AB_I2C_MODULE_0 0
#define AB_I2C_MODULE_1 1
#define AB_I2C_MODULE_2 2
#define AB_I2C_MODULE_3 3
#define AB_I2C_MODULE_4 4
#define AB_I2C_MODULE_5 5

/////////////////////////////////////
// API Layer 0

uint32_t i2cGetBase(uint32_t i2cModule);
uint32_t i2cGetPort(uint32_t i2cModule);
uint32_t i2cGetSCL(uint32_t i2cModule);
uint32_t i2cGetSDA(uint32_t i2cModule);
void i2cEnable(uint32_t i2cModule);
uint32_t i2cGetSCLPin(uint32_t i2cModule);
uint32_t i2cGetSDAPin(uint32_t i2cModule);
uint32_t i2cGetInterruptID(uint32_t i2cModule);

//////////////////////////////////////
// API Layer 1
void i2cInit(uint32_t i2cModule);

#endif /* I2C_H_ */
