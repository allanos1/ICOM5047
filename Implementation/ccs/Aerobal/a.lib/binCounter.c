/*
 * binCounter.c
 *
 * The Binary Counter Library for Aerobal Project
 *
 * Model: SN54AS867
 *
 * Microprocessor: TI's Tiva TM4C123G[H6PM] Launchpad.
 *
 *  Created on: Apr 5, 2014
 *      Author: Anthony Llanos Velazquez
 */

#include "binCounter.h"

/////////////////////////////////
// API Layer 0


/*Initializes the binCounter0808's GPIO Port and Pins for its clock, and its
 * clear and direction function.
 *
 *The Clock pin drives the binCounter to do its current function using its
 *CLK port, the function 0 pin is mapped to the S0 Port, and
 *the function 1 pin is mapped to the S1 port.
 */
void binCounterInit(uint32_t binCounterClockPort, uint32_t binCounterClockPin,
		uint32_t binCounterFunction0Port, uint32_t binCounterFunction0Pin,
		uint32_t binCounterFunction1Port, uint32_t binCounterFunction1Pin){

	_binCounterClockPort = binCounterClockPort;
	_binCounterClockPin = binCounterClockPin;
	_binCounterFunction0Port = binCounterFunction0Port;
	_binCounterFunction0Pin = binCounterFunction0Pin;
	_binCounterFunction1Port = binCounterFunction1Port;
	_binCounterFunction1Pin = binCounterFunction1Pin;
	_binCounterDirection = 0;

	//
	gpioSetMasterEnable(binCounterClockPort);
	gpioSetDigitalEnable(binCounterClockPort,binCounterClockPin,binCounterClockPin);
	gpioSetDirection(binCounterClockPort,binCounterClockPin,binCounterClockPin);
	//
	gpioSetMasterEnable(binCounterFunction0Port);
	gpioSetDigitalEnable(binCounterFunction0Port,binCounterFunction0Pin,binCounterFunction0Pin);
	gpioSetDirection(binCounterFunction0Port,binCounterFunction0Pin,binCounterFunction0Pin);
	//
	gpioSetMasterEnable(binCounterFunction1Port);
	gpioSetDigitalEnable(binCounterFunction1Port,binCounterFunction1Pin,binCounterFunction1Pin);
	gpioSetDirection(binCounterFunction1Port,binCounterFunction1Pin,binCounterFunction1Pin);
}

/* Activates the Binary Counter's S0 and S1 pins such that the
 *
 */
void binCounterFunctionSetIncrease(){
	gpioSetData(_binCounterFunction0Port,_binCounterFunction0Pin,0xFF);
	gpioSetData(_binCounterFunction1Port,_binCounterFunction1Pin,0xFF);
	_binCounterDirection = BINCOUNTER_FUNCTION_INCREASE;
}

/*
 *
 */
void binCounterFunctionSetDecrease(){
	gpioSetData(_binCounterFunction0Port,_binCounterFunction0Pin,0xFF);
	gpioSetData(_binCounterFunction1Port,_binCounterFunction1Pin,0x00);
	_binCounterDirection = BINCOUNTER_FUNCTION_DECREASE;
}

/*
 *
 */
void binCounterFunctionSetClear(){
	gpioSetData(_binCounterFunction0Port,_binCounterFunction0Pin,0x00);
	gpioSetData(_binCounterFunction1Port,_binCounterFunction1Pin,0x00);
	_binCounterDirection = BINCOUNTER_FUNCTION_CLEAR;
}

/*
 *
 */
int binCounterIsIncreasing(){
	return _binCounterDirection == BINCOUNTER_FUNCTION_INCREASE;
}

/*
 *
 */
int binCounterIsDecreasing(){
	return _binCounterDirection == BINCOUNTER_FUNCTION_DECREASE;
}

/*
 *
 */
int binCounterIsClearing(){
	return _binCounterDirection == BINCOUNTER_FUNCTION_CLEAR;
}

/*
 *
 */
void binCounterClockRun(){
	gpioSetData(_binCounterClockPort,_binCounterClockPin,0x00);
	SysCtlDelay(2);
	gpioSetData(_binCounterClockPort,_binCounterClockPin,0x02);
	SysCtlDelay(2);
	gpioSetData(_binCounterClockPort,_binCounterClockPin,0x00);
}

/////////////////////////////////////
// API Layer 1

/*
 *
 */
void binCounterIncrease(){
	if(!binCounterIsIncreasing()){
		binCounterFunctionSetIncrease();
	}
	binCounterClockRun();
}

/*
 *
 */
void binCounterDecrease(){
	if(!binCounterIsDecreasing()){
		binCounterFunctionSetDecrease();
	}
	binCounterClockRun();
}

/*
 *
 */
void binCounterClear(){
	if(!binCounterIsClearing()){
		binCounterFunctionSetClear();
	}
	binCounterClockRun();
}

