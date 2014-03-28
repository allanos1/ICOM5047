/*
 * ABTest.h
 *
 *  Created on: Feb 22, 2014
 *      Author: Anthony
 */

#ifndef ABTEST_H_
#define ABTEST_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"
//
#include "gpio.h"
#include "lcd.h"
#include "bmp085.h"
#include "relay.h"
#include "bluetooth.h"
#include "dht11.h"
#include "loadCell.h"
#include "servo.h"
#include "buttons.h"

#define ABTEST_BOOL_TRUE 1
#define ABTEST_BOOL_FALSE 0

void ABTestDelay();
void ABTestLog(char* line1, char* line2, char* line3, char* line4);
void ABTestLogLineClear(char lineNumber);
int ABTestBMP(int init);
int ABTestLCD(int init);
int ABTestRelay(int init);
int ABTestBT(int init);
int ABTestDHT(int init);
int ABTestAnalog(int init);
int ABTestServo(int init);
int ABTestButtons(int init);
int ABTestWindVane(int init);
int ABTestAnemometer(int init);



#endif /* ABTEST_H_ */
