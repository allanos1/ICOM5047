/*
 * ABExperimentCentral.h
 *
 *  Created on: May 2, 2014
 *      Author: Administrator
 */

#ifndef ABEXPERIMENTCENTRAL_H_
#define ABEXPERIMENTCENTRAL_H_

#include "ABSensorServer.h"

#define ABEC_BUFFER_SIZE 100


typedef struct {
	float sum ;
	float buffer[ABEC_BUFFER_SIZE];
	int count;
	float average;
} ABExperimentCentralBuffer;


float ABEC_NONW_DragFront;
float ABEC_NONW_DragBack;
float ABEC_NONW_LiftUp;
float ABEC_NONW_LiftDown;
float ABEC_NONW_SideLeft;
float ABEC_NONW_SideRight;

float ABEC_NO_W_DragFront;
float ABEC_NO_W_DragBack;
float ABEC_NO_W_LiftUp;
float ABEC_NO_W_LiftDown;
float ABEC_NO_W_SideLeft;
float ABEC_NO_W_SideRight;

float ABEC_O_NW_DragFront;
float ABEC_O_NW_DragBack;
float ABEC_O_NW_LiftUp;
float ABEC_O_NW_LiftDown;
float ABEC_O_NW_SideLeft;
float ABEC_O_NW_SideRight;

float ABEC_O_W_DragFront;
float ABEC_O_W_DragBack;
float ABEC_O_W_LiftUp;
float ABEC_O_W_LiftDown;
float ABEC_O_W_SideLeft;
float ABEC_O_W_SideRight;

//////////////////////////
// API Layer 0
void ABECBufferReset(ABExperimentCentralBuffer* sensorBuffer);
void ABECBufferRefresh(ABExperimentCentralBuffer* sensorBuffer, float newValue);
void ABECAddForces();
void ABECClearForceBuffer();
void ABECStoreNONW();
void ABECStoreNO_W();
void ABECStoreO_NW();
void ABECAddPressure();
void ABECAddTemperature();
void ABECAddHumidity();
void ABECAddWindDirection();
void ABECAddVelocity();

///////////////////////////
// API Layer 1
float ABECComputeBaseDrag();
float ABECComputeBaseLift();
float ABECComputeBaseSide();
float ABECComputeObjectBaseDrag();
float ABECComputeObjectBaseLift();
float ABECComputeObjectBaseSide();
float ABECComputeObjectDrag();
float ABECComputeObjectLift();
float ABECComputeObjectSide();
//
float ABECGetAveragePressure();
float ABECGetAverageTemperature();
float ABECGetAverageHumidity();
float ABECGetAverageWindDirection();
float ABECGetAverageVelocity();


#endif /* ABEXPERIMENTCENTRAL_H_ */
