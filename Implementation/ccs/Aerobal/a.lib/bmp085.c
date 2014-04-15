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
tI2CMInstance bmpI2cModuleInstance;

/*
 * The struct for the BMP180 Application Instance.
 */
tBMP180 bmpAppInstance[20];

/*
 * Data flag to indicate that data is ready. Used by the
 * BMP085 App Callback.
 */
volatile uint_fast8_t bmp085_dataFlag;

int bmpI2CInited = 0;
int bmpStructCounter = 0;
int bmpCurrentInstance = 0;
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
    I2CMIntHandler(&bmpI2cModuleInstance);
}

/* Initializes the BMP085 configuration to
 * the I2C1 module. Automatically enables interrupts,
 * I2C1 module, and the GPIO A Peripheral, and the
 * callback application for the BMP085 created by TI.
 *
 * Input:
 * abI2cModule
 * Returns:
 * none
 *
 */
void bmp085Init(uint32_t i2cModule){
	if(!bmpI2CInited){
		i2cInit(AB_I2C_MODULE_1);
		I2CMInit(&bmpI2cModuleInstance, i2cGetBase(i2cModule), i2cGetInterruptID(i2cModule), 0xff, 0xff,
	         ROM_SysCtlClockGet());
		bmpI2CInited = 1;
	}
	BMP180Init(&bmpAppInstance[bmpStructCounter], &bmpI2cModuleInstance, BMP085_I2C_ADDRESS,
			bmp085AppCallback, &bmpAppInstance[bmpStructCounter]);
	bmpStructCounter++;
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

void bmp085DataRead(int index){
	BMP180DataRead(&bmpAppInstance[index], bmp085AppCallback, &bmpAppInstance[index]);
    while(bmp085_dataFlag == 0); bmp085_dataFlag = 0;
    BMP180DataTemperatureGetFloat(&bmpAppInstance[index], &bmpTemperature);
    BMP180DataPressureGetFloat(&bmpAppInstance[index], &bmpPressure);
}

/* Returns the temperature stored in the library. To obtain
 * new values, 'bmp085DataRead()' must be called. The values stored
 * in the library are the ones from the last module read.
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
