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
//#include "loadCell.h"
#include "servo.h"
#include "buttons.h"
#include "bmp085Array.h"
#include "lcdSerial.h"
#include "anemometer.h"
#include "adc.h"
#include "windVane.h"
#include "bmp085Array.h"
#include "bluetooth.h"
#include "gpio.h"
#include "timers.h"
#include "binCounter.h"
#include "buttons.h"
#include "ABUI.h"
#include "string.h"
#include "ABPortsInterruptHandler.h"

#define ABTEST_BOOL_TRUE 1
#define ABTEST_BOOL_FALSE 0

void ABTestLCDInit();
void ABTestDAC();
void ABTestBluetooth();
void ABTestBMPSpeed();
void ABTestAnemometer();
void ABTestADC();
void ABTestWindVane();
void ABTimerTestInterruptHandler();
void ABTimerTest();
void ABTestBinaryCounter();
void ABTestButtonsInit();
void ABTestButtonsPool();
void ABTestButtonsT1();
void ABTestButtonsT2();
void ABTestButtonsCheckButton(char* msg, int result);
void ABTestButtons();
void ABTestUI();
void ABTestDHT();
void ABTestLCDHex();
void ABBmp085ArrayTest(int sensorIndex,int testNumber);

#endif /* ABTEST_H_ */
