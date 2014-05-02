/*
 * motorAtv.c
 *
 *  Created on: May 1, 2014
 *      Author: Administrator
 */

#include "motorAtv.h"

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

void motorAtvSpeedInc(){
	gpioSetData(MOTORATV_PORT_SPEED_POT_INC,MOTORATV_PIN_SPEED_POT_INC,0x00);
	SysCtlDelay(500000);
	gpioSetData(MOTORATV_PORT_SPEED_POT_INC,MOTORATV_PIN_SPEED_POT_INC,0xFF);
}
void motorAtvSpeedDec(){
	gpioSetData(MOTORATV_PORT_SPEED_POT_DEC,MOTORATV_PIN_SPEED_POT_DEC,0x00);
	SysCtlDelay(500000);
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

void motorAtvSetStep(int step){
	int diff = motorAtvCurrentStep - step;
	if(diff<0){
		//INC
		while(motorAtvCurrentStep != step){
			motorAtvSpeedInc();
		}
	}
	else{
		//DEC
		while(motorAtvCurrentStep != step){
			motorAtvSpeedDec();
		}
	}

}
