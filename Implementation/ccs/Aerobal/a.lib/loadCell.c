/*
 * loadCell.c
 *
 *  Created on: Apr 25, 2014
 *      Author: Juan, Anthony
 */

#include "loadCell.h"

LoadCellBuffer loadCellBufferDragFront;
LoadCellBuffer loadCellBufferDragBack;
LoadCellBuffer loadCellBufferLiftUp;
LoadCellBuffer loadCellBufferLiftDown;
LoadCellBuffer loadCellBufferSideLeft;
LoadCellBuffer loadCellBufferSideRight;

//////////////////////////
// API Layer 0

/* Initializes the loadCell pins to perform measurements using the pins
 * PE0-PE5.
 *
 */
void loadCellInit(){
	adcInit(ADC_0);
	adcMuxPinSet(ADC_0,LOADCELL_FORCE_MUX_DRAG_FRONT,LOADCELL_FORCE_PIN_DRAG_FRONT);
	adcMuxPinSet(ADC_0,LOADCELL_FORCE_MUX_DRAG_BACK,LOADCELL_FORCE_PIN_DRAG_BACK);
	adcMuxPinSet(ADC_0,LOADCELL_FORCE_MUX_LIFT_UP,LOADCELL_FORCE_PIN_LIFT_UP);
	adcMuxPinSet(ADC_0,LOADCELL_FORCE_MUX_LIFT_DOWN,LOADCELL_FORCE_PIN_LIFT_DOWN);
	adcMuxPinSet(ADC_0,LOADCELL_FORCE_MUX_SIDE_LEFT,LOADCELL_FORCE_PIN_SIDE_LEFT);
	adcMuxPinSet(ADC_0,LOADCELL_FORCE_MUX_SIDE_RIGHT,LOADCELL_FORCE_PIN_SIDE_RIGHT);
	adcSetSequencerSize(ADC_0,7);
	loadCellRefreshSetSize(LOADCELL_BUFFER_SIZE); //Default and Max Refresh Size.
}

/*
 * Refreshes a buffer of LoadCellBuffer type. This Essentially performs
 * a rolling buffer refresh.
 */
void loadCellBufferRefresh(LoadCellBuffer* sensorBuffer, float newValue){
	sensorBuffer->sum -= sensorBuffer->buffer[sensorBuffer->count];
	sensorBuffer->sum += newValue;
	sensorBuffer->buffer[sensorBuffer->count] = newValue;
	sensorBuffer->count = (sensorBuffer->count + 1) % LOADCELL_BUFFER_SIZE;
	sensorBuffer->average = sensorBuffer->sum /(float)LOADCELL_BUFFER_SIZE ;
}

/*
 * Refreshes all of the pins of the LoadCells.
 */
void loadCellRefresh(){

	int i = 0;
	for(i = 0; i < loadCellRefreshSize; i++){
		adcRefresh();
		loadCellBufferRefresh(&loadCellBufferDragFront,
			adcDataGet(LOADCELL_FORCE_PIN_DRAG_FRONT));
		loadCellBufferRefresh(&loadCellBufferDragBack,
			adcDataGet(LOADCELL_FORCE_PIN_DRAG_BACK));
		loadCellBufferRefresh(&loadCellBufferLiftUp,
			adcDataGet(LOADCELL_FORCE_PIN_LIFT_UP));
		loadCellBufferRefresh(&loadCellBufferLiftDown,
			adcDataGet(LOADCELL_FORCE_PIN_LIFT_DOWN));
		loadCellBufferRefresh(&loadCellBufferSideLeft,
			adcDataGet(LOADCELL_FORCE_PIN_SIDE_LEFT));
		loadCellBufferRefresh(&loadCellBufferSideRight,
			adcDataGet(LOADCELL_FORCE_PIN_SIDE_RIGHT));
	}
}

/*
 * Sets the size of the refreshing performed on the
 * pins.
 */
void loadCellRefreshSetSize(int size){
	if(size < 1){
		loadCellRefreshSize = 1;
	}
	else if(size > LOADCELL_BUFFER_SIZE){
		loadCellRefreshSize = LOADCELL_BUFFER_SIZE;
	}
	else{
		loadCellRefreshSize = size;
	}
}

///////////////////////////////////
// API Layer 1
/*
 * Returns the value of the Drag-Front buffer.
 */
float loadCellGetDragFront(){
	return loadCellBufferDragFront.average*0.535-63.946;
}

/*
 * Returns the value of the Drag-Back buffer.
 */
float loadCellGetDragBack(){
	return loadCellBufferDragBack.average*0.1055-3.9814;
}

/*
 * Returns the value of the Lift-Up buffer.
 */
float loadCellGetLiftUp(){
	return loadCellBufferLiftUp.average*0.1684-24.589;
}

/*
 * Returns the value of the Lift-Down buffer.
 */
float loadCellGetLiftDown(){
	return loadCellBufferLiftDown.average*0.4405-8.758;
}

/*
 * Returns the value of the Side-Left buffer.
 */
float loadCellGetSideLeft(){
	return loadCellBufferSideLeft.average*0.0718-19.8506;
}

/*
 * Returns the value of the Side-Right buffer.
 */
float loadCellGetSideRight(){
	return loadCellBufferSideRight.average*0.2744-130.8;
}
