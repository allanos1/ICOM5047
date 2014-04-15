/*
 * servo.h
 *
 *  Created on: Nov 20, 2013
 *      Author: JJLB
 */

#ifndef SERVO_H_
#define SERVO_H_

/****TI Libraries****/
#include <stdint.h>
#include <stdbool.h>
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "inc/hw_memmap.h"
#include "driverlib/rom.h"
#include "driverlib/systick.h"



//***** TO DO *****//

/****Self Made Libraries****/
#include "gpio.h"

/****************************************************/
/*	Constant Definitions							*/
/****************************************************/

#define AB_SERVO_ANGLE_MAX 180
#define AB_SERVO_ANGLE_MIN 130

/****************************************************/
/*	Variable Definitions							*/
/****************************************************/
//NONE

/****************************************************/
/*	Function Definitions							*/
/****************************************************/

/****Main Functions****/
void servoSetupPWM();
void servosetPositionPWM(int _servoDegrees, int _servoSpeed);
void servosetPositionPWMEnable(int _servoDegrees, int _servoSpeed);
void servosetPositionPWMDisable();

//****Helper Functions****/
#endif /* SERVO_H_ */
