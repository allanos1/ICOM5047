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

/* Calculates the humidity of air (water vapor)
 * using the static pressure, the temperature and
 * the relative humidity.
 *
 * Procedure taken using the Humidity Section of
 * http://en.wikipedia.org/wiki/Density_of_air.
 *
 */
float bmp085GetAirDensity(float P0, float T, float RH){

	//Air Density

	//P0 is static in Pa

	float adR = 8.314; // Universal Gas Constant - J/(K*mol)
	float adMv = 0.018016; //Molar Mass of water vapor - kg/mol.
	float adMd = 0.028964; //Molar Mass of Dry air - kg/mol
	//float adRv = 461.465; //Specific gas constant for water vapor - J/(kg*K)
	//float adRd = 287.058; //Specific gas constant for dry air - J/(kg*K)

	//Need to calculate pd and pv
	//pv = relative humidity * psat
	float adPsat = 6.1078 * pow(10.0,(7.5*T)/(T+237.3));
	float adPv = (RH/100.0 * adPsat)*100; //Pa - scaling

	//PD
	float adPd = P0-adPv;

	return ((adPd*adMd)+(adPv*adMv))/(adR*(T+273));
}

/*
 *Calculates the velocity of air using the static and dynamic pressures along
 *with the temperature and humidity.
 *
 *Taken from Bernoulli's Equation.
 */
float bmp085GetPressureVelocity(float pStatic, float pDynamic,float temperature, float humidity){
	return sqrt(2*(pDynamic-pStatic)/bmp085GetAirDensity(pStatic,temperature,humidity));
}
