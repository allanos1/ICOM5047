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
//#include "dht11.h"

void ABTestDelay();
void ABTestLog(char* line1, char* line2, char* line3, char* line4);
void ABTestLogLineClear(char lineNumber);
int ABTestBMP();
int ABTestLCD();
int ABTestRelay();
int ABTestBT();

#endif /* ABTEST_H_ */
