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
ABSensorServerBuffer ABSSBufferMPSAPressure[16];
ABSensorServerBuffer ABSSBufferMPSATemperature[16];


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
	if(dht11CanRefresh()){
		//SysCtlDelay(3000000);
		dht11init();
		dht11getData();
		while(dhtIsActive());
	}

	//Buffer Storage:

	//DHT Humidity
	ABSSBufferRefresh(&ABSSBufferDHTHumidity, dht22GetHumidity());

	//DHT Temperature
	ABSSBufferRefresh(&ABSSBufferDHTTemperature, dht22GetTemperature());
}

void ABSSRefreshBMP(){

	//Refresh Sequence.
	ABSSRefreshMPSAIndex(0);

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

void ABSSRefreshMPSA(){
	int i = 0;
	bmp085ArrayReset();
	for(i = 0; i < 16; i++){
		SysCtlDelay(10000);
		//Refresh Sequence.
		bmp085ArrayDataRead();

		//Buffer Storage
		ABSSBufferRefresh(&ABSSBufferMPSAPressure[i],bmp085ArrayGetPressure());
		ABSSBufferRefresh(&ABSSBufferMPSATemperature[i],bmp085ArrayGetTemperature());

		//Next Sensor
		bmp085ArrayNextSensor();
	}

}

void ABSSRefreshMPSAIndex(int index){
	//Refresh Sequence.
	bmp085ArrayDataReadPosition(index);
	//Buffer Storage;
	ABSSBufferRefresh(&ABSSBufferMPSAPressure[index],bmp085ArrayGetPressure());
	ABSSBufferRefresh(&ABSSBufferMPSATemperature[index],bmp085ArrayGetTemperature());

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

float ABSSGetMPSAIndexPressure(int index){
	return ABSSBufferMPSAPressure[index].average;
}

float ABSSGetMPSAIndexTemperature(int index){
	return ABSSBufferMPSATemperature[index].average;
}

/////////////////////////////////////////////
// API Layer 2 - Automatization Routines

void ABSSRefreshSequential(){
	switch(ABSSSequentialRefreshCount){
	case 0: ABSSRefreshDHT(); break;
	case 1: ABSSRefreshBMP(); break;
	case 2: ABSSRefreshWindVane(); break;
	case 3: ABSSRefreshAnemometer(); break;
	case 4: ABSSRefreshLoadCells(); break;
	case 5: ABSSRefreshMPSA(); break;
	default: ABSSSequentialRefreshCount=-1; break;
	}
	ABSSSequentialRefreshCount++;
}

void ABSSRefreshAll(){
	ABSSRefreshDHT();
	ABSSRefreshBMP();
	ABSSRefreshWindVane();
	ABSSRefreshAnemometer();
	ABSSRefreshWindVane();
	ABSSRefreshLoadCells();
	ABSSRefreshMPSA();
}
