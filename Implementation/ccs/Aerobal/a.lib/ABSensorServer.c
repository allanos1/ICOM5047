/*
 * ABSensorServer.c
 *
 *  Created on: Apr 24, 2014
 *      Author: Administrator
 */

#include "ABSensorServer.h"

ABSensorServerBuffer ABSSBufferDHTHumidity;
ABSensorServerBuffer ABSSBufferDHTTemperature;
ABSensorServerBuffer ABSSBufferBMPPressure;
ABSensorServerBuffer ABSSBufferBMPTemperature;
ABSensorServerBuffer ABSSBufferAnemometer;
ABSensorServerBuffer ABSSBufferWindVane;
ABSensorServerBuffer ABSSBufferLCDragFront;
ABSensorServerBuffer ABSSBufferLCDragBack;
ABSensorServerBuffer ABSSBufferLCLiftUp;
ABSensorServerBuffer ABSSBufferLCLiftDown;
ABSensorServerBuffer ABSSBufferLCSideLeft;
ABSensorServerBuffer ABSSBufferLCSideRight;


/////////////////////////////////////
// API Layer 0 - Refresh SS Library
//
// Basic Refresh Function:
//void ABSSRefresh<x>(){
//	//Refresh Sequence.
//	<x>
//	//Buffer Storage;
//	<x>
//}

/* Buffer Refresh Function. v1.0
 *
 * Removes the current buffer value from the sum, adds the new value,
 * stores the new value in the current buffer index, increases the pointer
 * modularly in the buffer, and computes the average to store it in
 * the struct for later access.
 *
 */
void ABSSBufferRefresh(ABSensorServerBuffer* sensorBuffer, float newValue){
	sensorBuffer->sum -= sensorBuffer->buffer[sensorBuffer->count];
	sensorBuffer->sum += newValue;
	sensorBuffer->buffer[sensorBuffer->count] = newValue;
	sensorBuffer->count = (sensorBuffer->count + 1) % ABSENSORSERVER_BUFFER_SIZE;
	sensorBuffer->average = sensorBuffer->sum /(float)ABSENSORSERVER_BUFFER_SIZE ;
}
/* SensorServer Refresh Function for DHT.
 *
 */
void ABSSRefreshDHT(){

	//Refresh Sequence.
	SysCtlDelay(3000000);
	dht11init();
	dht11getData();
	while(dhtIsActive());

	//Buffer Storage:

	//DHT Humidity
	ABSSBufferRefresh(&ABSSBufferDHTHumidity, dht22GetHumidity());

	//DHT Temperature
	ABSSBufferRefresh(&ABSSBufferDHTTemperature, dht22GetTemperature());
}

void ABSSRefreshBMP(){

	//Refresh Sequence.
	bmp085DataRead(0);

	//Buffer Storage:

	//BMP Temperature
	ABSSBufferRefresh(&ABSSBufferBMPTemperature, bmp085GetTemperature());

	//BMP Pressure
	ABSSBufferRefresh(&ABSSBufferBMPPressure, bmp085GetPressure());
}

void ABSSRefreshAnemometer(){
	//Refresh Sequence.
	anemometerSpeedBufferRefresh();
	//Buffer Storage
	//TODO: Settings.
	ABSSBufferRefresh(&ABSSBufferAnemometer,anemometerSpeedConvertKmH(anemometerSpeedBufferGetAverage()));
}

void ABSSRefreshWindVane(){
	//Refresh Sequence.
	windVaneRefresh(WIND_VANE_BUFFER_SIZE);
	//Buffer Storage;
	ABSSBufferRefresh(&ABSSBufferWindVane,windVaneGetAngle());
}

void ABSSRefreshLoadCells(){
	//TODO: Load Cell Library
	//Refresh Sequence.
	loadCellRefresh();

	//Buffer Storage;
	ABSSBufferRefresh(&ABSSBufferLCDragFront,loadCellGetDragFront());
	ABSSBufferRefresh(&ABSSBufferLCDragBack,loadCellGetDragBack());
	ABSSBufferRefresh(&ABSSBufferLCLiftUp,loadCellGetLiftUp());
	ABSSBufferRefresh(&ABSSBufferLCLiftDown,loadCellGetLiftDown());
	ABSSBufferRefresh(&ABSSBufferLCSideLeft,loadCellGetSideLeft());
	ABSSBufferRefresh(&ABSSBufferLCSideRight,loadCellGetSideRight());

}

/*
void ABSSRefresh<x>(){
	//Refresh Sequence.
	<x>
	//Buffer Storage;
	<x>
}
*/

//////////////////////////////////////
// API Layer 1 - Getter SS Library

float ABSSGetDHTTemperature(){
	return ABSSBufferDHTTemperature.average;
}

float ABSSGetDHTHumidity(){
	return ABSSBufferDHTHumidity.average;
}

float ABSSGetBMPPressure(){
	return ABSSBufferBMPPressure.average;
}

float ABSSGetBMPTemperature(){
	return ABSSBufferBMPTemperature.average;
}

float ABSSGetAnemometerSpeed(){
	return ABSSBufferAnemometer.average;
}

float ABSSGetWindVaneDirection(){
	return ABSSBufferWindVane.average;
}

float ABSSGetLoadCellDragFront(){
	return ABSSBufferLCDragFront.average;
}

float ABSSGetLoadCellDragBack(){
	return ABSSBufferLCDragBack.average;
}

float ABSSGetLoadCellLiftUp(){
	return ABSSBufferLCLiftUp.average;
}

float ABSSGetLoadCellLiftDown(){
	return ABSSBufferLCLiftDown.average;
}

float ABSSGetLoadCellSideLeft(){
	return ABSSBufferLCSideLeft.average;
}

float ABSSGetLoadCellSideRight(){
	return ABSSBufferLCSideRight.average;
}
