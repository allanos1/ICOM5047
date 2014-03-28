/*
 * servo.c
 *
 *  Created on: Nov 20, 2013
 *      Author: JJLB
 */
/****************************************************/
/*					Instructions					*/
/****************************************************/
/*set a timer for timer() function
 *
 *
 *
 */

#include "servo.h"

/****************************************************/
/*	Variable Definitions							*/
/****************************************************/

#define SERVO_ULPERIOD 9500000

int down_time = 40;
int currentPosition = 0;
int _servoState = 1;

/****************************************************/
/*				Function Definitions				*/
/****************************************************/

/****Main Functions****/



void servoSetupPWM(){
	//SysCtlClockSet(SYSCTL_SYSDIV_10|SYSCTL_USE_OSC|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinConfigure(GPIO_PF2_T1CCP0);
	GPIOPinTypeTimer(GPIO_PORTF_BASE, GPIO_PIN_2);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
	TimerConfigure(TIMER1_BASE, TIMER_CFG_SPLIT_PAIR|TIMER_CFG_A_PWM);
	TimerLoadSet(TIMER1_BASE, TIMER_A, SERVO_ULPERIOD-1);
}
void servosetPositionPWM(int _servoDegrees, int _servoSpeed){
	long dutyCycle;
	SysCtlClockSet(SYSCTL_SYSDIV_10|SYSCTL_USE_OSC|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	dutyCycle = 9299600+_servoDegrees*(2250/180);

	TimerEnable(TIMER1_BASE, TIMER_A);
	TimerMatchSet(TIMER1_BASE, TIMER_A, dutyCycle);

	SysCtlDelay(1000000-(7500*(_servoSpeed%100)));
	TimerDisable(TIMER1_BASE, TIMER_A);
	SysCtlClockSet(SYSCTL_SYSDIV_1|SYSCTL_USE_OSC|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
}

void servosetPositionPWMEnable(int _servoDegrees, int _servoSpeed){
	long dutyCycle;
	SysCtlClockSet(SYSCTL_SYSDIV_10|SYSCTL_USE_OSC|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	dutyCycle = 9299600+_servoDegrees*(2250/180);

	TimerEnable(TIMER1_BASE, TIMER_A);
	TimerMatchSet(TIMER1_BASE, TIMER_A, dutyCycle);

}

void servosetPositionPWMDisable(){
	TimerDisable(TIMER1_BASE, TIMER_A);
	SysCtlClockSet(SYSCTL_SYSDIV_1|SYSCTL_USE_OSC|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

}

