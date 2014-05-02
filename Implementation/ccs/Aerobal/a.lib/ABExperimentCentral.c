/*
 * ABExperimentCentral.c
 *
 *  Created on: May 2, 2014
 *      Author: Administrator
 */

#include "ABExperimentCentral.h"

ABExperimentCentralBuffer ABECMesForceDragFront;
ABExperimentCentralBuffer ABECMesForceDragBack;
ABExperimentCentralBuffer ABECMesForceLiftUp;
ABExperimentCentralBuffer ABECMesForceLiftDown;
ABExperimentCentralBuffer ABECMesForceSideLeft;
ABExperimentCentralBuffer ABECMesForceSideRight;
ABExperimentCentralBuffer ABECMesPressure;
ABExperimentCentralBuffer ABECMesTemperature;
ABExperimentCentralBuffer ABECMesVelocity;
ABExperimentCentralBuffer ABECMesHumidity;
ABExperimentCentralBuffer ABECMesWindVane;

void ABECBufferReset(ABExperimentCentralBuffer* sensorBuffer){
	sensorBuffer->sum = 0;
	sensorBuffer->count = 0;
	sensorBuffer->average = 0;
}

void ABECBufferRefresh(ABExperimentCentralBuffer* sensorBuffer, float newValue){
	sensorBuffer->sum -= sensorBuffer->buffer[sensorBuffer->count];
	sensorBuffer->sum += newValue;
	sensorBuffer->buffer[sensorBuffer->count] = newValue;
	sensorBuffer->count = (sensorBuffer->count + 1) % ABEC_BUFFER_SIZE;
	sensorBuffer->average = sensorBuffer->sum /(float)ABEC_BUFFER_SIZE;
}


void ABECAddForces(){
	ABECBufferRefresh(&ABECMesForceDragFront,ABSSGetLoadCellDragFront());
	ABECBufferRefresh(&ABECMesForceDragBack,ABSSGetLoadCellDragBack());
	ABECBufferRefresh(&ABECMesForceLiftUp,ABSSGetLoadCellLiftUp());
	ABECBufferRefresh(&ABECMesForceLiftDown,ABSSGetLoadCellLiftDown());
	ABECBufferRefresh(&ABECMesForceSideLeft,ABSSGetLoadCellSideLeft());
	ABECBufferRefresh(&ABECMesForceSideRight,ABSSGetLoadCellSideRight());
}

void ABECClearForceBuffer(){
	ABECBufferReset(&ABECMesForceDragFront);
	ABECBufferReset(&ABECMesForceDragBack);
	ABECBufferReset(&ABECMesForceLiftUp);
	ABECBufferReset(&ABECMesForceLiftDown);
	ABECBufferReset(&ABECMesForceSideLeft);
	ABECBufferReset(&ABECMesForceSideRight);
}

void ABECStoreNONW(){
	ABEC_NONW_DragFront=ABECMesForceDragFront.average;
	ABEC_NONW_DragBack=ABECMesForceDragBack.average;
	ABEC_NONW_LiftUp=ABECMesForceLiftUp.average;
	ABEC_NONW_LiftDown=ABECMesForceLiftDown.average;
	ABEC_NONW_SideLeft=ABECMesForceSideLeft.average;
	ABEC_NONW_SideRight=ABECMesForceSideRight.average;
}

void ABECStoreNO_W(){
	ABEC_NO_W_DragFront=ABECMesForceDragFront.average;
	ABEC_NO_W_DragBack=ABECMesForceDragBack.average;
	ABEC_NO_W_LiftUp=ABECMesForceLiftUp.average;
	ABEC_NO_W_LiftDown=ABECMesForceLiftDown.average;
	ABEC_NO_W_SideLeft=ABECMesForceSideLeft.average;
	ABEC_NO_W_SideRight=ABECMesForceSideRight.average;
}
void ABECStoreO_NW(){
	ABEC_O_NW_DragFront=ABECMesForceDragFront.average;
	ABEC_O_NW_DragBack=ABECMesForceDragBack.average;
	ABEC_O_NW_LiftUp=ABECMesForceLiftUp.average;
	ABEC_O_NW_LiftDown=ABECMesForceLiftDown.average;
	ABEC_O_NW_SideLeft=ABECMesForceSideLeft.average;
	ABEC_O_NW_SideRight=ABECMesForceSideRight.average;
}

void ABECAddPressure(){
	ABECBufferRefresh(&ABECMesPressure,ABSSGetMPSAIndexPressure(0));
}

void ABECAddTemperature(){
	ABECBufferRefresh(&ABECMesTemperature,/*ABSSGetMPSAIndexTemperature(0)*/ABSSGetDHTTemperature());

}

void ABECAddHumidity(){
	ABECBufferRefresh(&ABECMesHumidity,ABSSGetDHTHumidity());
}

void ABECAddWindDirection(){
	ABECBufferRefresh(&ABECMesHumidity,ABSSGetWindVaneDirection());
}

void ABECAddVelocity(){
	ABECBufferRefresh(&ABECMesHumidity,ABSSGetAnemometerSpeed());
}

/////////////////////////////////////
// API Layer 1

float ABECComputeBaseDrag(){
	if(ABEC_NO_W_DragFront > ABEC_NO_W_DragBack){
		return ABEC_NO_W_DragFront-ABEC_NONW_DragFront;
	}
	else{
		return ABEC_NO_W_DragBack-ABEC_NONW_DragBack;
	}
}
float ABECComputeBaseLift(){
	if(ABEC_NO_W_LiftUp > ABEC_NO_W_LiftDown){
		return ABEC_NO_W_LiftUp-ABEC_NONW_LiftUp;
	}
	else{
		return ABEC_NO_W_LiftDown-ABEC_NONW_LiftDown;
	}
}
float ABECComputeBaseSide(){
	if(ABEC_NO_W_SideLeft > ABEC_NO_W_SideRight){
		return ABEC_NO_W_SideLeft-ABEC_NONW_SideLeft;
	}
	else{
		return ABEC_NO_W_SideRight-ABEC_NONW_SideRight;
	}
}
/////
float ABECComputeObjectBaseDrag(){
	if(ABECMesForceDragFront.average  > ABECMesForceDragBack.average){
		return ABECMesForceDragFront.average-ABEC_O_NW_DragFront;
	}
	else{
		return ABECMesForceDragBack.average-ABEC_O_NW_DragBack;
	}
}
float ABECComputeObjectBaseLift(){
	if(ABECMesForceLiftUp.average  > ABECMesForceLiftDown.average){
		return ABECMesForceLiftUp.average-ABEC_O_NW_LiftUp;
	}
	else{
		return ABECMesForceLiftDown.average-ABEC_O_NW_LiftDown;
	}
}
float ABECComputeObjectBaseSide(){
	if(ABECMesForceSideLeft.average  > ABECMesForceSideRight.average){
		return ABECMesForceSideLeft.average-ABEC_O_NW_SideLeft;
	}
	else{
		return ABECMesForceSideRight.average-ABEC_O_NW_SideRight;
	}
}
float ABECComputeObjectDrag(){
	return ABECComputeObjectBaseDrag()-ABECComputeBaseDrag();
}
float ABECComputeObjectLift(){
	return ABECComputeObjectBaseLift()-ABECComputeBaseLift();
}
float ABECComputeObjectSide(){
	return ABECComputeObjectBaseSide()-ABECComputeBaseSide();
}

///////////////

float ABECGetAveragePressure(){
	return ABECMesPressure.average;
}
float ABECGetAverageTemperature(){
	return ABECMesTemperature.average;
}
float ABECGetAverageHumidity(){
	return ABECMesVelocity.average;
}
float ABECGetAverageWindDirection(){
	return ABECMesHumidity.average;
}
float ABECGetAverageVelocity(){
	return ABECMesWindVane.average;
}