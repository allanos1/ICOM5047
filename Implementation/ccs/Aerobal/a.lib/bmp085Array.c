/*
 * bmp085Array.c
 *
 *  Created on: Apr 3, 2014
 *      Author: JJLB
 */

#include "bmp085Array.h"

float bmpTemperature;
float bmpPressure;
float errorFlag = 0;
int currentSensor = 1;

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
void bmp085ArrayInit(){

	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

	gpioSetMasterEnable(GPIO_PORTD);
	gpioSetDigitalEnable(GPIO_PORTD, GPIO_PIN_2, GPIO_PIN_2);
	gpioSetDirection(GPIO_PORTD, GPIO_PIN_2, GPIO_PIN_2);

	timerSetup(TIMER_0,TIMER_CONFIG_PERIODIC, 1.0);
	timerStart(TIMER_0);

}

void bmp085ArrayStepForward(){
	gpioSetData(GPIO_PORTD, GPIO_PIN_2 , ~gpioGetData(GPIO_PORTD,GPIO_PIN_2));
	currentSensor++;
}

void bmp085ArraySampleTimer(){
	timerInterruptClear(TIMER_0);

	if(currentSensor <= sensorQuantity){
	//bmp085Init();
	bmp085ArrayStepForward();
	}
	else{
		timerStop(TIMER_0);
		currentSensor = 1;
	}
}



void bmp085ArraySynchronize(){
	if(currentSensor != 1){
		currentSensor = 1;
	}
	errorFlag = 1;
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
void bmp085ArrayDataRead(int sensorIndex){
//place holder
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
float bmp085ArrayGetTemperature(int sensorIndex){
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
float bmp085ArrayGetPressure(int sensorIndex){
	return bmpPressure;
}



