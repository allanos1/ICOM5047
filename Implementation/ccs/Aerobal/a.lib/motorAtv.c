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
	lcdSerialWriteString("Resetting Speed...");
	int i = 0;
	while(i < 64){
		lcdSerialCursorLine2();
		lcdSerialWriteNumber(i);
		lcdSerialWriteString("/64");
		motorAtvSpeedDec();
		i++;
	}
	lcdSerialCursorLine3();
	lcdSerialWriteString("Reset Complete.");
}

void motorAtvSpeedInc(){
	ABTime t0 = ABTimeGetReference();
	gpioSetData(MOTORATV_PORT_SPEED_POT_INC,MOTORATV_PIN_SPEED_POT_INC,0x00);
	while(ABTimeGetDelta(ABTimeGetReference(),t0).milliseconds < 200);
	gpioSetData(MOTORATV_PORT_SPEED_POT_INC,MOTORATV_PIN_SPEED_POT_INC,0xFF);
}
void motorAtvSpeedDec(){
	gpioSetData(MOTORATV_PORT_SPEED_POT_DEC,MOTORATV_PIN_SPEED_POT_DEC,0x00);
	ABTime mt0 = ABTimeGetReference();
	ABTime mt1 = ABTimeGetReference();
	while(ABTimeGetDelta(mt1,mt0).milliseconds < 200){
		mt1 = ABTimeGetReference();
	}
	gpioSetData(MOTORATV_PORT_SPEED_POT_DEC,MOTORATV_PIN_SPEED_POT_DEC,0xFF);
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
