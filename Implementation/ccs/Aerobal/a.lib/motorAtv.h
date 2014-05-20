/*
 * motorAtv.h
 *
 *  Created on: May 1, 2014
 *      Author: Administrator
 */

#ifndef MOTORATV_H_
#define MOTORATV_H_

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "gpio.h"
#include "relay.h"
#include "ABTime.h"
#include "lcdSerial.h"

//Not used since already defined in the relay module.
//The relay module is essentially part of this module.
#define MOTORATV_PORT_R_STOP GPIO_PORTA
#define MOTORATV_PIN_R_STOP 0x04 //A2
#define MOTORATV_PORT_F_FORWARD GPIO_PORTA
#define MOTORATV_PIN_F_FORWARD 0x08 //A3

///////////////////
//Increase and Decrease Ports of the
//DS1809 Digital Potentiometer module.
//Defined to be PF1 and PF2.
#define MOTORATV_PORT_SPEED_POT_INC GPIO_PORTF //PF2
#define MOTORATV_PIN_SPEED_POT_INC 0x04
#define MOTORATV_PORT_SPEED_POT_DEC GPIO_PORTF //PF1
#define MOTORATV_PIN_SPEED_POT_DEC 0x02

/////////////////////
// Variables.
int motorAtvCurrentStep;
int motorAtvTargetSpeed;

/////////////////////////////////
// API Layer 0
void motorAtvInit();
void motorAtvTurnOn();
void motorAtvTurnOff();
void motorAtvSpeedInc();
void motorAtvSpeedDec();
void motorAtvSpeedReset();
void motorAtvSetTargetSpeed(int speed);
void motorAtvSetSpeedCharacterized();
int motorAtvGetTargetSpeed();

/////////////////////////////////
// API Layer 1
int motorAtvGetStep(int speed);




#endif /* MOTORATV_H_ */
