/*
 * motorAtv.c
 *
 *  Created on: May 1, 2014
 *      Author: Administrator
 */

#include "motorAtv.h"

/*
 * Characterization performed on the VFD
 * to reach the desired speeds faster. The
 * window can be used to achieve precise
 * control of the wind in the tunnel.
 */
int motorAtvCharacterization[50]={
	2, 5, 8, 10, 10, 10, 10, 11, 12, 13,
	14, 15, 16, 18, 19, 20, 21, 22, 23, 25,
	26, 27, 28, 30, 31, 32, 34, 35, 36, 37,
	37, 38, 41, 42, 44, 45, 46, 48, 49, 50,
	51, 52, 53, 55, 56, 57, 58, 59, 60, 64
};


/////////////////////////////////
// API Layer 0
/*
 *Initializes the MotorATV module. Uses Ports PA2 and PA3 by
 *activating the relay module to control the STOP and START
 *pins of the VFD. Uses Pins PF1 and PF2 to increase and decrease
 *the digital potentiometer used.
 */
void motorAtvInit(){
	relayInit();
	relayOff();

	gpioSetMasterEnable(MOTORATV_PORT_SPEED_POT_INC);
	gpioSetDigitalEnable(MOTORATV_PORT_SPEED_POT_INC,MOTORATV_PIN_SPEED_POT_INC,0xFF);
	gpioSetDirection(MOTORATV_PORT_SPEED_POT_INC,MOTORATV_PIN_SPEED_POT_INC,0xFF);
	gpioSetData(MOTORATV_PORT_SPEED_POT_INC,MOTORATV_PIN_SPEED_POT_INC,0xFF);

	gpioSetMasterEnable(MOTORATV_PORT_SPEED_POT_DEC);
	gpioSetDigitalEnable(MOTORATV_PORT_SPEED_POT_DEC,MOTORATV_PIN_SPEED_POT_DEC,0xFF);
	gpioSetDirection(MOTORATV_PORT_SPEED_POT_DEC,MOTORATV_PIN_SPEED_POT_DEC,0xFF);
	gpioSetData(MOTORATV_PORT_SPEED_POT_DEC,MOTORATV_PIN_SPEED_POT_DEC,0xFF);

}

/*
 * Turns on the VFD by activating the stop pin on the
 * relay and after a delay the forward or START Pin.
 */
void motorAtvTurnOn(){
	relayOn();
}

/*
 * Turns off the VFD by disconnecting the STOP (also the
 * START) pin of the VFD.
 */
void motorAtvTurnOff(){
	relayOff();
}

/*
 * Resets the speed of the MotorATV by decreasing
 * a minimum number of the maximum number of steps
 * in the digital potentiometer and hence
 * making sure it reaches zero. Must be careful to
 * not meet the minimum delay for decrease of the
 * potentiometer.
 *
 * TODO: This function can be changed to leave the pin
 * set for longer length as specified in the datasheet
 * such that the DS1809 initiates a decrease sequence.
 */
void motorAtvSpeedReset(){
	lcdSerialClear();
	lcdSerialCursorLine1();
	lcdSerialWriteString("AeroBal Motor Ctl");
	lcdSerialCursorLine2();
	lcdSerialWriteString("====================");
	lcdSerialCursorLine3();
	lcdSerialWriteString("Resetting Speed...");
	int i = 0;
	while(i < 64){
		lcdSerialCursorLine4();
		lcdSerialWriteNumber(i);
		lcdSerialWriteString("/64");
		motorAtvSpeedDec();
		i++;
	}
	lcdSerialCursorLine3();
	lcdSerialWriteString("Reset Complete.");
}

/*
 * Uses the characterization performed on the VFD
 * to set the desired speed set in this module.
 */
void motorAtvSetSpeedCharacterized(){
	if(motorAtvTargetSpeed <= 0) motorAtvTargetSpeed = 1;
	if(motorAtvTargetSpeed > 50) motorAtvTargetSpeed = 50;
	int i = 0;
	while(i < motorAtvCharacterization[motorAtvTargetSpeed-1]){
		motorAtvSpeedInc();
		SysCtlDelay(100000);
		i++;
	}
}

/*
 * Increases the voltage level of the potentiometer by
 * performing a step on the increase pin of the DS1809.
 */
void motorAtvSpeedInc(){
	gpioSetData(MOTORATV_PORT_SPEED_POT_INC,MOTORATV_PIN_SPEED_POT_INC,0x00);
	SysCtlDelay(100000);
	gpioSetData(MOTORATV_PORT_SPEED_POT_INC,MOTORATV_PIN_SPEED_POT_INC,0xFF);
}

/*
 * Decreases the voltage level of the potentiometer by
 * performing a step on the decrease pin of the DS1809.
 */
void motorAtvSpeedDec(){
	gpioSetData(MOTORATV_PORT_SPEED_POT_DEC,MOTORATV_PIN_SPEED_POT_DEC,0x00);
	SysCtlDelay(100000);
	gpioSetData(MOTORATV_PORT_SPEED_POT_DEC,MOTORATV_PIN_SPEED_POT_DEC,0xFF);
}

/*
 * Sets the module's stored target speed.
 */
void motorAtvSetTargetSpeed(int speed){
	motorAtvTargetSpeed = speed;
}

/*
 * Returns the target speed set in the module.
 */
int motorAtvGetTargetSpeed(){
	return motorAtvTargetSpeed;
}
///////////////////////////////////
// API Layer 1

/*
 * Returns the step required to reach the speed
 * given to this function.
 */
int motorAtvGetStep(int speed){
	return motorAtvCharacterization[speed-1];
}
