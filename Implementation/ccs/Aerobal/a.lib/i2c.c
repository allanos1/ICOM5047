/*
 * i2c.c
 *
 *  Created on: Apr 4, 2014
 *      Author: Administrator
 */

#include "i2c.h"


//////////////////////////////////////
// API Layer 0

uint32_t i2cGetBase(uint32_t i2cModule){
	switch(i2cModule){
		case AB_I2C_MODULE_0: return I2C0_BASE;
		case AB_I2C_MODULE_1: return I2C1_BASE;
		case AB_I2C_MODULE_2: return I2C2_BASE;
		case AB_I2C_MODULE_3: return I2C3_BASE;
		default: return 0;
	}
}

uint32_t i2cGetPort(uint32_t i2cModule){
	switch(i2cModule){
		case AB_I2C_MODULE_0: return GPIO_PORTB;
		case AB_I2C_MODULE_1: return GPIO_PORTA;
		case AB_I2C_MODULE_2: return GPIO_PORTE;
		case AB_I2C_MODULE_3: return GPIO_PORTD;
		default: return 0;
	}
}

uint32_t i2cGetSCL(uint32_t i2cModule){
	switch(i2cModule){
		case AB_I2C_MODULE_0: return GPIO_PB2_I2C0SCL;
		case AB_I2C_MODULE_1: return GPIO_PA6_I2C1SCL;
		case AB_I2C_MODULE_2: return GPIO_PE4_I2C2SCL;
		case AB_I2C_MODULE_3: return GPIO_PD0_I2C3SCL;
		default: return 0;
	}
}

uint32_t i2cGetSDA(uint32_t i2cModule){
	switch(i2cModule){
		case AB_I2C_MODULE_0: return GPIO_PB3_I2C0SDA;
		case AB_I2C_MODULE_1: return GPIO_PA7_I2C1SDA;
		case AB_I2C_MODULE_2: return GPIO_PE5_I2C2SDA;
		case AB_I2C_MODULE_3: return GPIO_PD1_I2C3SDA;
		default: return 0;
	}
}

void i2cEnable(uint32_t i2cModule){
	switch(i2cModule){
		case AB_I2C_MODULE_0: SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0); break;
		case AB_I2C_MODULE_1: SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1); break;
		case AB_I2C_MODULE_2: SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C2); break;
		case AB_I2C_MODULE_3: SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C3); break;
		default: return;
	}
}

uint32_t i2cGetSCLPin(uint32_t i2cModule){
	switch(i2cModule){
		case AB_I2C_MODULE_0: return GPIO_PIN_2;
		case AB_I2C_MODULE_1: return GPIO_PIN_6;
		case AB_I2C_MODULE_2: return GPIO_PIN_4;
		case AB_I2C_MODULE_3: return GPIO_PIN_0;
		default: return 0;
	}
}

uint32_t i2cGetSDAPin(uint32_t i2cModule){
	switch(i2cModule){
		case AB_I2C_MODULE_0: return GPIO_PIN_3;
		case AB_I2C_MODULE_1: return GPIO_PIN_7;
		case AB_I2C_MODULE_2: return GPIO_PIN_5;
		case AB_I2C_MODULE_3: return GPIO_PIN_1;
		default: return 0;
	}
}

uint32_t i2cGetInterruptID(uint32_t i2cModule){
	switch(i2cModule){
		case AB_I2C_MODULE_0: return INT_I2C0;
		case AB_I2C_MODULE_1: return INT_I2C1;
		case AB_I2C_MODULE_2: return INT_I2C2;
		case AB_I2C_MODULE_3: return INT_I2C3;
		default: return 0;
	}
}
//////////////////////////////////////
// API Layer 1

void i2cInit(uint32_t i2cModule){
	i2cEnable(i2cModule);
	gpioSetMasterEnable(i2cGetPort(i2cModule));
	GPIOPinConfigure(i2cGetSCL(i2cModule));
	GPIOPinConfigure(i2cGetSDA(i2cModule));
	GPIOPinTypeI2CSCL(i2cGetPort(i2cModule), i2cGetSCLPin(i2cModule));
	GPIOPinTypeI2C(i2cGetPort(i2cModule), i2cGetSDAPin(i2cModule));
	IntMasterEnable();
}



