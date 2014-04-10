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
#include "bmp085Array.h"

#define ABTEST_BOOL_TRUE 1
#define ABTEST_BOOL_FALSE 0
/*
tI2CMInstance i2cInstance;
tBMP180 bmpInstance;

void ABTestLCDInit();
void ABTestBluetooth();
void ABTestBMPSpeed();
void ABTestAnemometer();
void ABTestADC();
void ABTestWindVane();
void ABTimerTestInterruptHandler();
void ABTimerTest();
void ABTestBinaryCounter();

*/
/* Test Case
 *
 * Reads data from the desire quantity of sensors and send it through UART0
 * to the USB connected computer to be access by a serial monitor application.
 *
 */

void bmp085ArrayTest(int sensorIndex,int testNumber);

#endif /* ABTEST_H_ */
