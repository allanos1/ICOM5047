/*
 * ABSensorServer.c
 *
 *  Created on: Apr 24, 2014
 *      Author: Anthony
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
 * Takes a new measurement  and stores it in the buffer.
 */
void ABSSRefreshDHT(){

	//Refresh Sequence.
	if(dht11CanRefresh()){
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

/* SensorServer Refresh Function for BMP.
 * Takes a new measurement and stores it in the buffer.
 */
void ABSSRefreshBMP(){

	anemometerEnd();
	//Refresh Sequence.
	bmp085DataRead(0);
	//ABSSRefreshMPSAIndex(0);
	anemometerStart();
	//Buffer Storage:

	//BMP Temperature
	ABSSBufferRefresh(&ABSSBufferBMPTemperature, bmp085GetTemperature());

	//BMP Pressure
	ABSSBufferRefresh(&ABSSBufferBMPPressure, bmp085GetPressure());
}

/* SensorServer Refresh Function for Anemometer.
 * Takes a new measurement  and stores it in the buffer.
 */
void ABSSRefreshAnemometer(){
	//Refresh Sequence.
	anemometerSpeedBufferRefresh();
	//Buffer Storage
	//TODO: Settings.
	ABSSBufferRefresh(&ABSSBufferAnemometer,anemometerSpeedBufferGetAverage());
}

/* SensorServer Refresh Function for WindVane.
 * Takes a new measurement  and stores it in the buffer.
 */
void ABSSRefreshWindVane(){
	//Refresh Sequence.
	windVaneRefresh(WIND_VANE_BUFFER_SIZE);
	//Buffer Storage;
	ABSSBufferRefresh(&ABSSBufferWindVane,windVaneGetAngle());
}

/* SensorServer Refresh Function for LoadCells.
 * Takes a new measurement  and stores it in the buffer.
 */
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

/* SensorServer Refresh Function for MPSA.
 * Takes a new measurement  and stores it in the buffers.
 */
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

/* SensorServer Refresh Function for MPSA by Index.
 * Takes a new measurement and stores it in the buffer.
 */
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

/* SensorServer average function for DHT's Temperature value.
 */
float ABSSGetDHTTemperature(){
	return ABSSBufferDHTTemperature.average;
}

/* SensorServer average function for DHT's Humidity value.
 */
float ABSSGetDHTHumidity(){
	return ABSSBufferDHTHumidity.average;
}

/* SensorServer average function for BMP's Pressure value.
 */
float ABSSGetBMPPressure(){
	return ABSSBufferBMPPressure.average;
}

/* SensorServer average function for BMP's Temperature value.
 */
float ABSSGetBMPTemperature(){
	return ABSSBufferBMPTemperature.average;
}

/* SensorServer average function for Anemometer's speed value.
 */
float ABSSGetAnemometerSpeed(){
	return ABSSBufferAnemometer.average;
}

/* SensorServer average function for WindVane's direction value.
 */
float ABSSGetWindVaneDirection(){
	return ABSSBufferWindVane.average;
}


/* SensorServer average function for DragFront LoadCell Value.
 */
float ABSSGetLoadCellDragFront(){
	return ABSSBufferLCDragFront.average;
}

/* SensorServer average function for DragBack LoadCell Value.
 */
float ABSSGetLoadCellDragBack(){
	return ABSSBufferLCDragBack.average;
}

/* SensorServer average function for LiftUp LoadCell Value.
 */
float ABSSGetLoadCellLiftUp(){
	return ABSSBufferLCLiftUp.average;
}

/* SensorServer average function for LiftDown LoadCell Value.
 */
float ABSSGetLoadCellLiftDown(){
	return ABSSBufferLCLiftDown.average;
}

/* SensorServer average function for SideLeft LoadCell Value.
 */
float ABSSGetLoadCellSideLeft(){
	return ABSSBufferLCSideLeft.average;
}

/* SensorServer average function for SideRight LoadCell Value.
 */
float ABSSGetLoadCellSideRight(){
	return ABSSBufferLCSideRight.average;
}

/* SensorServer average function for MPSA Buffer[index] pressure value.
 */
float ABSSGetMPSAIndexPressure(int index){
	return ABSSBufferMPSAPressure[index].average;
}

/* SensorServer average function for MPSA Buffer[index] temperature value.
 */
float ABSSGetMPSAIndexTemperature(int index){
	return ABSSBufferMPSATemperature[index].average;
}

/////////////////////////////////////////////
// API Layer 2 - Automatization Routines

/*
 * Refresh Sequence for all sensors of the AeroBal System.
 * It is performed sequentially by increasing a rolling counter.
 * This can be put to run on a background process.
 *
 * TODO: Bluetooth is disabled during execution.
 */
void ABSSRefreshSequential(){
	buttonsMask();
	bluetoothDisable();
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
	buttonsUnmask();
	bluetoothEnable();
}

/*
 * Initializes a forced complete refresh of all sensors for
 * one time per execution.
 */
void ABSSRefreshAll(){
	ABSSRefreshDHT();
	//ABSSRefreshBMP();
	ABSSRefreshWindVane();
	ABSSRefreshAnemometer();
	ABSSRefreshWindVane();
	ABSSRefreshLoadCells();
	//ABSSRefreshMPSA();
}
