/*
 * ABSensorServer.h
 *
 *  Created on: Apr 24, 2014
 *      Author: Administrator
 */

#ifndef ABSENSORSERVER_H_
#define ABSENSORSERVER_H_

#include "dht11.h"
#include "bmp085.h"
#include "anemometer.h"
#include "windVane.h"
#include "loadCell.h"
#include "bmp085Array.h"
#include "buttons.h"
#include "bluetooth.h"

#define ABSENSORSERVER_BUFFER_SIZE 10 //TODO: CHECK


typedef struct {
	float sum ;
	float buffer[ABSENSORSERVER_BUFFER_SIZE];
	int count;
	float average;
} ABSensorServerBuffer;

int ABSSSequentialRefreshCount;

//////////////////////////////////////
// API Layer 0 - Refresh SS Library
void ABSSRefreshDHT();
void ABSSRefreshBMP();
void ABSSRefreshWindVane();
void ABSSRefreshAnemometer();
void ABSSRefreshLoadCells();
void ABSSRefreshMPSA();
void ABSSRefreshMPSAIndex(int index);

//////////////////////////////////////
// API Layer 1 - Getter SS Library
float ABSSGetDHTHumidity();
float ABSSGetDHTTemperature();
float ABSSGetBMPPressure();
float ABSSGetBMPTemperature();
float ABSSGetAnemometerSpeed();
float ABSSGetWindVaneDirection();
float ABSSGetLoadCellDragFront();
float ABSSGetLoadCellDragBack();
float ABSSGetLoadCellLiftUp();
float ABSSGetLoadCellLiftDown();
float ABSSGetLoadCellSideLeft();
float ABSSGetLoadCellSideRight();
float ABSSGetMPSAIndexPressure(int index);
float ABSSGetMPSAIndexTemperature(int index);

//////////////////////////////////////
// API Layer 2 - Automatization Routines
void ABSSRefreshSequential();
void ABSSRefreshAll();



#endif /* ABSENSORSERVER_H_ */
