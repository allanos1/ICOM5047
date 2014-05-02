/*
 * anemometer.c
 * The Anemometer Time Library for
 * Young 05103
 *
 * v0.8
 *
 *  Created on: Mar 27, 2014
 *      Author: Anthony
 *
 *  TODO: Expand comments.
 */

#include "anemometer.h"

FloatBuffer anemometerBuffer ;

///////////////////////////////////
// API Layer 0

/*
 * Initalizes a new instance of the anemometer module library,
 * setting the port/pin to read to the given port and pin in function.
 * This port and pin will be configured to receive interrupts. This port
 * will also be used in the interrupt handler set in the library.
 *
 *
 * gpioPort -  The port to use.
 * gpioPin - The pin to use.
 */
void anemometerInit(uint32_t gpioPort, uint32_t gpioPin){

	anemometerPort = gpioPort;
	anemometerPin = gpioPin;
	ABTimeInit(ABTIME_BASE_TIMER_5,ABTIME_RESOLUTION_MILLISECOND);
	gpioSetMasterEnable(anemometerPort);
	gpioSetDigitalEnable(anemometerPort,anemometerPin,0xFF);
	gpioSetDirection(anemometerPort,anemometerPin,0x00);
	gpioSetInterruptBothEdges(anemometerPort,anemometerPin,0x00);
	gpioSetInterruptEvent(anemometerPort,anemometerPin,0x00);
	gpioHelperInterruptMasterEnable(anemometerPort);
	gpioSetInterruptEnable(anemometerPort);

}

/***************************
 *  Interrupt Handler
 *  | extern void anemometerInterruptHandler_Counter();
 ***************************
 * Increases the number of pulses received from the anemometer.
 * 3 pulses per revolution.
 */
void anemometerInterruptHandler_Counter(){
	anemometerCount++;
	gpioSetInterruptClear(anemometerPort,anemometerPin,0xFF);
}

/*
 * Starts the anemometer interrupt updates by removing the
 * mask from the interrupt port pin. Resets the number of
 * pulses to 0 and sets a new timer reference.
 */
void anemometerStart(){
	anemometerCount = 0;
	anemometerT0 = ABTimeGetReference();
	if(!ABTimeIsRunning()) ABTimeStart();
	gpioSetInterruptMaskDisable(anemometerPort,anemometerPin,0xFF);

}

/*
 * Ends anemometer execution by setting the mask to the interrupt
 * port pin.
 */
void anemometerEnd(){
	gpioSetInterruptMaskDisable(anemometerPort,anemometerPin,0x00);
	anemometerT1 = ABTimeGetReference();

}
/*
 * Returns the count of anemometer interrupts or pulses.
 */
int anemometerGetCount(){
	return anemometerCount;
}

/*
 * Gets the timer delta between last reset execution and the
 * end of execution.
 *
 * Returns:
 * ABTime struct - containes the delta between the last reset and
 * end.
 */
ABTime anemometerGetTimeDelta(){
	return ABTimeGetDelta(anemometerT1,anemometerT0);
}

/*
 *Gets the timer delta between the last reset and the moment
 *this function is executed.
 */
ABTime anemometerGetCurrentDelta(){
	return ABTimeGetDelta(ABTimeGetReference(),anemometerT0);
}

/*
 * Returns the speed calculated with the current anemometer count
 * and the current time since last reset.
 * TODO: Expand...
 */
float anemometerSpeedGet(){
	ABTime now = anemometerGetCurrentDelta();
	float seconds = (float) now.minutes*60
			+ (float)now.seconds
			+ ((float)now.milliseconds)/1000.0;
	float anemometerRevolutions = ((float)anemometerCount) / ANEMOMETER_CALIBRATION_INCI;
	float rev_s = anemometerRevolutions/seconds; //rev/s
	float baseSpeed = 0.098 ; //m/s
	float ms = rev_s *baseSpeed;
	return ms;

}

/*
 * Resets the anemometer count and the first time execution
 * of the module as if it were started on the moment this
 * function is executed.
 */
void anemometerReset(){
	anemometerT0 = ABTimeGetReference();
	anemometerCount = 0;
}

//////////////////////////////////
// API Layer 1 - Buffer

/*
 *Adds the current speed of the tunnel to the buffer, calculates
 *the sum of the buffer and increments the buffer pointer.
 */
void anemometerSpeedBufferAdd(float speed){
	anemometerBuffer.sum -= anemometerBuffer.buffer[anemometerBuffer.bufferCount];
	anemometerBuffer.sum += speed;
	anemometerBuffer.buffer[anemometerBuffer.bufferCount] = speed;
	anemometerBuffer.bufferCount = (anemometerBuffer.bufferCount + 1) % ANEMOMETER_BUFFER_SIZE;
}

/*
 * Calculates the average speed of the tunnel from the current
 * buffer sum.
 */
float anemometerSpeedBufferGetAverage(){
	return anemometerBuffer.sum / (float) ANEMOMETER_BUFFER_SIZE;
}

/*
 * Refreshes the buffer by issuing a addition to the buffer if
 * the amount of time since last addition is greater than 100.
 * TODO: Parametize this function?
 */
ABTime anemometerSpeedBufferRefresh(){
	ABTime now = anemometerGetCurrentDelta();
	if(now.milliseconds > 100){
		anemometerSpeedBufferAdd(anemometerSpeedGet());
		anemometerReset();
	}
	return now;
}

///////////////////////////////////
// API Layer 2 - Conversion
/*
 * Convert the given speed in meters per second
 * to miles per hour.
 */
float anemometerSpeedConvertMiH(float speed){
	return speed * 2.23693629205;
}

/*
 * Convert the given speed in meters per second
 * to kilometers per hour.
 */
float anemometerSpeedConvertKmH(float speed){
	return speed * 3.6;
}



