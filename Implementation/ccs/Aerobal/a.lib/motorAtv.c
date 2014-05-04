/*
 * motorAtv.c
 *
 *  Created on: May 1, 2014
 *      Author: Administrator
 */

#include "motorAtv.h"

int motorAtvCharacterization[50]={
	2, 5, 8, 10, 10, 10, 10, 11, 12, 13,
	14, 15, 16, 18, 19, 20, 21, 22, 23, 25,
	26, 27, 28, 30, 31, 32, 34, 35, 36, 37,
	37, 38, 41, 42, 44, 45, 46, 48, 49, 50,
	51, 52, 53, 55, 56, 57, 58, 59, 60, 64
};


/////////////////////////////////
// API Layer 0
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
void motorAtvTurnOn(){
	relayOn();
}
void motorAtvTurnOff(){
	relayOff();
}

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
void motorAtvSpeedInc(){
	gpioSetData(MOTORATV_PORT_SPEED_POT_INC,MOTORATV_PIN_SPEED_POT_INC,0x00);
	SysCtlDelay(100000);
	gpioSetData(MOTORATV_PORT_SPEED_POT_INC,MOTORATV_PIN_SPEED_POT_INC,0xFF);
}
void motorAtvSpeedDec(){
	gpioSetData(MOTORATV_PORT_SPEED_POT_DEC,MOTORATV_PIN_SPEED_POT_DEC,0x00);
	SysCtlDelay(100000);
	gpioSetData(MOTORATV_PORT_SPEED_POT_DEC,MOTORATV_PIN_SPEED_POT_DEC,0xFF);
}
void motorAtvSetTargetSpeed(int speed){
	motorAtvTargetSpeed = speed;
}

int motorAtvGetTargetSpeed(){
	return motorAtvTargetSpeed;
}
///////////////////////////////////
// API Layer 1

int motorAtvGetStep(int step){
	return motorAtvCharacterization[step-1];
}
