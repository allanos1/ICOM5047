/*
 * bmp085.c
 *
 *  Created on: Feb 21, 2014
 *      Author: Administrator
 */
#include "bmp085.h"

/*
 * The struct for the I2C Module Instance.
 */
tI2CMInstance TII2C_ModuleInstance;

/*
 * The struct for the BMP180 Application Instance.
 */
tBMP180 TIBMP085_AppInstance;

/*
 * Data flag to indicate that data is ready. Used by the
 * BMP085 App Callback.
 */
volatile uint_fast8_t bmp085_dataFlag;


/* The BMP085 callback for the applicaton. Indicates that data
 * is ready.
 */
void bmp085AppCallback(void* bmp085CallbackData, uint_fast8_t bmp085Status){
    if(bmp085Status == I2CM_STATUS_SUCCESS) bmp085_dataFlag = 1;
}


/********************
 * Interrupt Handler : Assign to I2C1 Module.
 ********************
 *
 * Interrupt handler function for the I2C module.
 * Calls the I2CModule Application designed by TI.
 *
 * Input:
 *  none
 * Returns:
 *  none
 */
void bmp085I2CIntHandler(void){
    I2CMIntHandler(&TII2C_ModuleInstance);
}

/* Initializes the BMP085 configuration to
 * the I2C1 module. Automatically enables interrupts,
 * I2C1 module, and the GPIO A Peripheral, and the
 * callback application for the BMP085 created by TI.
 *
 * I2C1 is enabled.
 * GPIOA is enabled.
 * PA6 - I2C1 SCL
 * PA7 - I2C1 SDA
 *
 * Input:
 * none
 * Returns:
 * none
 *
 */
void bmp085Init(){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinConfigure(GPIO_PA6_I2C1SCL);
	GPIOPinConfigure(GPIO_PA7_I2C1SDA);
	GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);
	GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);
	IntMasterEnable();
	I2CMInit(&TII2C_ModuleInstance, I2C1_BASE, INT_I2C1, 0xff, 0xff,
	         ROM_SysCtlClockGet()); //ROM is needed???? Why the fuck?
	BMP180Init(&TIBMP085_AppInstance, &TII2C_ModuleInstance, BMP085_I2C_ADDRESS,
			bmp085AppCallback, &TIBMP085_AppInstance);
	while(bmp085_dataFlag == 0);
	bmp085_dataFlag = 0;
}

/* Reads the data using the BMP085 App Instance. This data is read an stored within
 * the BMP085 Application Module and then is retrieved sending the addresses of
 * the variables in which to store.
 *
 * Input:
 *  none
 *
 * Returns:
 *  none
 */
void bmp085DataRead(){
	BMP180DataRead(&TIBMP085_AppInstance, bmp085AppCallback, &TIBMP085_AppInstance);
    while(bmp085_dataFlag == 0);
    bmp085_dataFlag = 0;
    BMP180DataTemperatureGetFloat(&TIBMP085_AppInstance, &bmpTemperature);
    BMP180DataPressureGetFloat(&TIBMP085_AppInstance, &bmpPressure);
}

/* Returns the temperature stored in the library. To obtain
 * new values, 'bmp085DataRead()' must be called.
 *
 * Input:
 *  none
 *
 * Returns:
 *  float - the last temperature read and stored in the
 *  library.
 */
float bmp085GetTemperature(){
	return bmpTemperature;
}

/* Returns the pressure stored in the library. To obtain
 * new values, 'bmp085DataRead()' must be called.
 *
 * Input:
 *  none
 *
 * Returns:
 *  float - the last temperature read and stored in the
 *  library.
 */
float bmp085GetPressure(){
	return bmpPressure;
}
