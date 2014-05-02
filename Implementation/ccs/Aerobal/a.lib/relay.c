/*
 * relay.c
 *
 *  Created on: Feb 22, 2014
 *      Author: Anthony
 *
 * Module created for the Relay of the Aerobal System.
 * The module is programmed for use with the A5 pin.
 * Uses custom GPIO library.
 *
 */


#include "relay.h"

/*
 * Initializes the port and pin of the relay: port A5.
 * Port is configured with digital function and output
 * direction.
 */
void relayInit(){
	gpioSetMasterEnable(AB_MODULE_RELAY_PORT);
	gpioSetDigitalEnable(AB_MODULE_RELAY_PORT,AB_MODULE_RELAY_PINS,0xFF);
	gpioSetDirection(AB_MODULE_RELAY_PORT,AB_MODULE_RELAY_PINS,0xFF);
	ABTimeInit(ABTIME_BASE_TIMER_5,ABTIME_RESOLUTION_MILLISECOND);
	ABTimeStart();
}

//Toggles the state of the relay;
void relayToggle(){
	uint8_t state = gpioGetData(AB_MODULE_RELAY_PORT,AB_MODULE_RELAY_PINS);
	if((state & 0x20)){
		gpioSetData(AB_MODULE_RELAY_PORT,AB_MODULE_RELAY_PINS,0x00);
	}
	else{
		gpioSetData(AB_MODULE_RELAY_PORT,AB_MODULE_RELAY_PINS,0xFF);
	}
}

void relayOn(){
	gpioSetData(AB_MODULE_RELAY_PORT,AB_MODULE_RELAY_PIN_STOP,0x00);
	//ABTime t0 = ABTimeGetReference();
	SysCtlDelay(6000000);
	//while(ABTimeGetDelta(ABTimeGetReference(),t0).seconds < 1);
	gpioSetData(AB_MODULE_RELAY_PORT,AB_MODULE_RELAY_PIN_START,0x00);
}

void relayOff(){
	gpioSetData(AB_MODULE_RELAY_PORT,AB_MODULE_RELAY_PINS,0xFF);
}
