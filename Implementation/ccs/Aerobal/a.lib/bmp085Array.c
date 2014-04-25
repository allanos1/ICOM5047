/*
 * bmp085Array.c
 *
 *  Created on: Apr 3, 2014
 *      Author: JJLB
 */

#include "bmp085Array.h"

int onPause = 1;
char errorFlag = 0;

/* Initializes the BMP085Array. Automatically enables interrupts,
 * and the GPIO Peripherals that are indicated by the input parameters.
 *
 * Input:
 * gpioPortCLK
 * gpioPinCLK
 * gpioPortZF
 * gpioPinZF
 *
 * Returns:
 * none
 *
 */
void bmp085ArrayInit(uint32_t gpioPortCLK, uint32_t gpioPinCLK, uint32_t gpioPortZF, uint32_t gpioPinZF, int sensorQuantity,int testNumber){

	bmp085ArrayGPIO_PortCLK = gpioPortCLK;
	bmp085ArrayGPIO_Pin_CLK = gpioPinCLK;
	bmp085ArrayGPIO_PortZF = gpioPortZF;
	bmp085ArrayGPIO_Pin_ZF = gpioPinZF;
	bmp085ArraySetSensorQuantity(sensorQuantity);

	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

	gpioSetMasterEnable(gpioPortCLK);
	gpioSetMasterEnable(gpioPortZF);

	gpioSetDigitalEnable(gpioPortCLK, gpioPinCLK, GPIO_ENABLE);
	gpioSetDigitalEnable(gpioPortZF, gpioPinZF , GPIO_ENABLE);

	gpioSetDirection(gpioPortCLK, gpioPinCLK , GPIO_OUTPUT);
	gpioSetDirection(gpioPortZF, gpioPinZF , GPIO_INPUT);

	//gpioSetInterruptMaskDisable(gpioPortZF,gpioPinZF , GPIO_ENABLE_INTERRUPTS); //Interrupt mask 410 //Detect Interrupt in Port D3
	//gpioSetInterruptBothEdges(gpioPortZF,gpioPinZF , GPIO_DELEGATE_EVENT_REGISTER);
	//gpioSetInterruptEvent(gpioPortZF,gpioPinZF , GPIO_INTERRUPT_RISING_EDGE);	//Event Register 40C //Detect Rising Edge
	//gpioSetInterruptEnable(gpioPortZF);
	//gpioHelperInterruptMasterEnable();
	if(testNumber){
	bmp085ArraySensorSetup(bmp085ArraySensorQuantity);
	}
}

/* Reads calibration parameters for every sensor connected and store parameters
 * within BMP085 library for later reference when reading data.
 *
 * Input:
 * sensorQuantity
 *
 * Returns:
 * none
 *
 */
void bmp085ArraySensorSetup(int sensorQuantity){

	bmp085ArrayReset();

	int i;

	for(i=0;i<=sensorQuantity;i++){
		bmp085Init(AB_I2C_MODULE_1);
		bmp085ArrayNextSensor();
		SysCtlDelay(100000);
	}

	bmp085ArrayReset();
}

/* Toggles the clock signal output for the BMP085Array circuit.
 *
 * Input:
 * none
 *
 * Returns:
 * none
 *
 */
void bmp085ArrayClockToggle(){

	gpioSetData(bmp085ArrayGPIO_PortCLK, bmp085ArrayGPIO_Pin_CLK , ~gpioGetData(bmp085ArrayGPIO_PortCLK,bmp085ArrayGPIO_Pin_CLK));
	gpioSetData(bmp085ArrayGPIO_PortCLK, bmp085ArrayGPIO_Pin_CLK , ~gpioGetData(bmp085ArrayGPIO_PortCLK,bmp085ArrayGPIO_Pin_CLK));

}

/* Enables next sensor on the BMP085Array and increase currentSensor variable.
 *
 * Input:
 * none
 *
 * Returns:
 * none
 *
 */
void bmp085ArrayNextSensor(){

	if(bmp085ArrayCurrentSensor >= bmp085ArraySensorQuantity){
		bmp085ArraySetCurrentSensor(bmp085ArrayCurrentSensor%bmp085ArraySensorQuantity);
	}
	else{
		bmp085ArrayClockToggle();
		bmp085ArrayCurrentSensor++;
	}

}

/* Set the currentSensor to the first on the array.
 *
 * Input:
 * none
 *
 * Returns:
 * none
 *
 */
void bmp085ArrayReset(){

	while(!(gpioGetData(bmp085ArrayGPIO_PortZF,bmp085ArrayGPIO_Pin_ZF) && 0xFF)){

		bmp085ArrayClockToggle();

	}
	bmp085ArrayCurrentSensor = 0;
}

/* Interrupt called function. Set errorFlag to high if current sensor value is not zero when called.
 *
 * Input:
 * none
 *
 * Returns:
 * none
 *
 */
void bmp085ArraySynchronize(){

	gpioSetInterruptClear(bmp085ArrayGPIO_PortZF,bmp085ArrayGPIO_Pin_ZF,bmp085ArrayGPIO_Pin_ZF);	//Clear PD3 InterruptFlag

	if(bmp085ArrayCurrentSensor != 0 && onPause){
		bmp085ArrayCurrentSensor = 0;
		errorFlag = 1;
		onPause = 1;
	}

}

/* Setter function for sensorQuantity variable.
 *
 * Input:
 * none
 *
 * Returns:
 * none
 *
 */
void bmp085ArraySetSensorQuantity(int sensorQuantity){
	bmp085ArraySensorQuantity = sensorQuantity - 1;
}

/* Getter function for sensorQuantity variable.
 *
 * Input:
 * none
 *
 * Returns:
 * bmp085ArraySensorQuantity
 *
 */
int bmp085ArrayGetSensorQuantity(){
	return bmp085ArraySensorQuantity + 1;
}

/* Setter function for currentSensor variable.
 *
 * Input:
 * none
 *
 * Returns:
 * currentSensor
 *
 */
void bmp085ArraySetCurrentSensor(int sensorIndex){

	int i;

		bmp085ArrayReset();

		for(i=0;i<sensorIndex;i++){
			bmp085ArrayClockToggle();
		}

	bmp085ArrayCurrentSensor = sensorIndex;

}

/* Getter function for currentSensor.
 *
 * Input:
 * none
 *
 * Returns:
 * currentSensor
 *
 */
int bmp085ArrayGetCurrentSensor(){
	return bmp085ArrayCurrentSensor;
}

/* Reads the data using the BMP085 Library. This data is read an stored within
 * the BMP085 Library and then is retrieved sending the index
 * of the sensor in the array that we would like the data from.
 *
 * Input:
 *  none
 *
 * Returns:
 *  none
 */
void bmp085ArrayDataRead(){

	bmp085DataRead(bmp085ArrayCurrentSensor);

}

/* Reads the data using the BMP085 Library. This data is read an stored within
 * the BMP085 Library and then is retrieved sending the index
 * of the sensor in the array that we would like the data from.
 *
 * Input:
 *  none
 *
 * Returns:
 *  none
 */
void bmp085ArrayDataReadPosition(int sensorIndex){

	bmp085ArraySetCurrentSensor(sensorIndex);
	bmp085ArrayDataRead();

}

/* Returns the temperature stored in the library. To obtain
 * new values, 'bmp085ArrayDataRead()' must be called.
 *
 * Input:
 *  none
 *
 * Returns:
 *  float - the last temperature read and stored in the
 *  library.
 */
float bmp085ArrayGetTemperature(){
	return bmp085GetTemperature();
}

/* Returns the pressure stored in the library. To obtain
 * new values, 'bmp085ArrayDataRead()' must be called.
 *
 * Input:
 *  none
 *
 * Returns:
 *  float - the last temperature read and stored in the
 *  library.
 */
float bmp085ArrayGetPressure(){
	return 	bmp085GetPressure();
}

/* Timer Based Data Reading Example */
/* Example of timed reading application. TIMER_0 is used at the desired frequency.
 *
 * Input:
 *  frequency
 *
 * Returns:
 *  none
 */
void bmp085StartDataAdquisition(int frequency){
	bmp085ArrayReset();
	onPause = 0;
	timerSetup(TIMER_0,TIMER_CONFIG_PERIODIC, frequency);
	timerStart(TIMER_0);
}

void bmp085ArraySampleTimer(){
	timerInterruptClear(TIMER_0);

	if(bmp085ArrayCurrentSensor <= bmp085ArraySensorQuantity){
	bmp085ArrayNextSensor();
	}
	else{
		timerStop(TIMER_0);
	}
}

