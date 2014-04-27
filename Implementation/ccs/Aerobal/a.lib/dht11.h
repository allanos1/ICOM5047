/*
 * dht11.h
 *
 * Created on: Nov 6, 2013
 * Author: Juan Lebron
 */

#ifndef DHT11_H_
#define DHT11_H_

/****TI Libraries****/
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
//***** TO DO *****//

/****Self Made Libraries****/
#include "gpio.h"
#include "lcdSerial.h"

/****************************************************/
/*	Constant Definitions							*/
/****************************************************/
#define REQUEST 0
#define REQUEST18MS 0
#define REQUEST40US 1

#define RESPONSE 1
#define RESPONSE54US 0
#define RESPONSE80US 1

#define CHECKIFRESPONSEISOK 2
#define CHECKIFRESPONSEISOKSTEP 0

#define DATAREADING 3
#define DATAREADINGINITSTEP	0
#define DATAREADINGSTEP 1

#define DATASIZE 40;

/****************************************************/
/*	Variable Definitions							*/
/****************************************************/
//NONE

/****************************************************/
/*	Function Definitions							*/
/****************************************************/

int responseStatusOk0;
int responseStatusOk1;

int dataBitReadStatus;
int count1uS;

int bitEntryTime;
int bitExitTime;
int bitTimingStatus;

int process;
int step;

int status0;
int status1;

int temp;
int decimalTemp;
int humidity;
int decimalHumidity;
int sumDataCheck;

int sum;
int _dhtActive;

/****Main Functions****/
void dhtSetup();
void dht11init();
void dht11getData();
void dht11request();
void dht11response();
void dht11responseStatus();
void dht11dataReading();
void dht11count1uS();
void dhtSetActive();
void dhtSetInactive();
int dhtIsActive();
int dht11getHumidity();
int dht11getTemperature();
void initTimer0Module();
void initTimer1Module();
void initTimeControlModule();
void endTimeControlModule();
void readDataBit();
void bitTimeElapsed();
void decodeData();
void convertTimetoBits();

/////////////////////////////////
// API Layer 1
float dht22GetTemperature();
float dht22GetHumidity();
int dhtCountDigits(int number);
float dhtGetDecimalHumidity();
float dhtGetDecimalTemperature();
float dhtPow(int base, int exponent);

//****Helper Functions****/

void dht11ClockSetup();

#endif /* DHT11_H_ */
