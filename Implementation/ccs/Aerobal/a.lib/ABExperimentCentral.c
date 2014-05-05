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

/*
 * Resets a buffer for an experiment. Updates the data count
 * for each value to zero.
 */
void ABECBufferReset(ABExperimentCentralBuffer* sensorBuffer){
	sensorBuffer->sum = 0;
	sensorBuffer->count = 0;
	int i = 0;
	for(i = 0; i < ABEC_BUFFER_SIZE; i++){
		sensorBuffer->buffer[i] = 0;
	}
	sensorBuffer->average = 0;
	sensorBuffer->dataCount = 0;
}

/*
 * Refresh a particular value to the given buffer. Updates the average.
 */
void ABECBufferRefresh(ABExperimentCentralBuffer* sensorBuffer, float newValue){
	sensorBuffer->sum -= sensorBuffer->buffer[sensorBuffer->count];
	sensorBuffer->sum += newValue;
	sensorBuffer->buffer[sensorBuffer->count] = newValue;
	sensorBuffer->count = (sensorBuffer->count + 1) % ABEC_BUFFER_SIZE;
	if(sensorBuffer->dataCount < ABEC_BUFFER_SIZE) sensorBuffer->dataCount++;
	sensorBuffer->average = sensorBuffer->sum /(float)sensorBuffer->dataCount;
}

/*
 * Updates the force buffers for an experiment.
 */
void ABECAddForces(){
	ABECBufferRefresh(&ABECMesForceDragFront,ABSSGetLoadCellDragFront());
	ABECBufferRefresh(&ABECMesForceDragBack,ABSSGetLoadCellDragBack());
	ABECBufferRefresh(&ABECMesForceLiftUp,ABSSGetLoadCellLiftUp());
	ABECBufferRefresh(&ABECMesForceLiftDown,ABSSGetLoadCellLiftDown());
	ABECBufferRefresh(&ABECMesForceSideLeft,ABSSGetLoadCellSideLeft());
	ABECBufferRefresh(&ABECMesForceSideRight,ABSSGetLoadCellSideRight());
}

/*
 * Clears the force buffer for the experiments.
 */
void ABECClearForceBuffer(){
	ABECBufferReset(&ABECMesForceDragFront);
	ABECBufferReset(&ABECMesForceDragBack);
	ABECBufferReset(&ABECMesForceLiftUp);
	ABECBufferReset(&ABECMesForceLiftDown);
	ABECBufferReset(&ABECMesForceSideLeft);
	ABECBufferReset(&ABECMesForceSideRight);
}

void ABECClearAllBuffers(){
	ABECClearForceBuffer();
	ABECBufferReset(&ABECMesPressure);
	ABECBufferReset(&ABECMesTemperature);
	ABECBufferReset(&ABECMesVelocity);
	ABECBufferReset(&ABECMesHumidity);
	ABECBufferReset(&ABECMesWindVane);
}

/*
 * Stores the base reference frame averages.
 */
void ABECStoreNONW(){
	ABEC_NONW_DragFront=ABECMesForceDragFront.average;
	ABEC_NONW_DragBack=ABECMesForceDragBack.average;
	ABEC_NONW_LiftUp=ABECMesForceLiftUp.average;
	ABEC_NONW_LiftDown=ABECMesForceLiftDown.average;
	ABEC_NONW_SideLeft=ABECMesForceSideLeft.average;
	ABEC_NONW_SideRight=ABECMesForceSideRight.average;
}

/*
 * Stores the base with wind aerodynamic force averages.
 */
void ABECStoreNO_W(){
	ABEC_NO_W_DragFront=ABECMesForceDragFront.average;
	ABEC_NO_W_DragBack=ABECMesForceDragBack.average;
	ABEC_NO_W_LiftUp=ABECMesForceLiftUp.average;
	ABEC_NO_W_LiftDown=ABECMesForceLiftDown.average;
	ABEC_NO_W_SideLeft=ABECMesForceSideLeft.average;
	ABEC_NO_W_SideRight=ABECMesForceSideRight.average;
}

/*
 * Stores the object+base reference frame averages.
 */
void ABECStoreO_NW(){
	ABEC_O_NW_DragFront=ABECMesForceDragFront.average;
	ABEC_O_NW_DragBack=ABECMesForceDragBack.average;
	ABEC_O_NW_LiftUp=ABECMesForceLiftUp.average;
	ABEC_O_NW_LiftDown=ABECMesForceLiftDown.average;
	ABEC_O_NW_SideLeft=ABECMesForceSideLeft.average;
	ABEC_O_NW_SideRight=ABECMesForceSideRight.average;
}

/*
 * Adds a pressure value to its buffer.
 */
void ABECAddPressure(){
	ABECBufferRefresh(&ABECMesPressure,ABSSGetBMPPressure());
}

/*
 * Adds a temperature value to its buffer.
 */
void ABECAddTemperature(){
	ABECBufferRefresh(&ABECMesTemperature,ABSSGetDHTTemperature());

}

/*
 * Adds a humidity value to its buffer.
 */
void ABECAddHumidity(){
	ABECBufferRefresh(&ABECMesHumidity,ABSSGetDHTHumidity());
}

/*
 * Adds a wind direction value to its buffer.
 */
void ABECAddWindDirection(){
	ABECBufferRefresh(&ABECMesWindVane,ABSSGetWindVaneDirection());
}

/*
 * Adds a velocity value to its buffer.
 */
void ABECAddVelocity(){
	ABECBufferRefresh(&ABECMesVelocity,ABSSGetAnemometerSpeed());
}


/*
 * Adds a drag front value to its buffer.
 */
float ABECTestGetAverageDragFront(){
	return ABECMesForceDragFront.average;
}

/*
 * Adds a drag back value to its buffer.
 */
float ABECTestGetAverageDragBack(){
	return ABECMesForceDragBack.average;
}

/*
 * Adds a lift up value to its buffer.
 */
float ABECTestGetAverageLiftUp(){
	return ABECMesForceLiftUp.average;
}

/*
 * Adds a lift down value to its buffer.
 */
float ABECTestGetAverageLiftDown(){
	return ABECMesForceLiftDown.average;
}

/*
 * Adds a side left value to its buffer.
 */
float ABECTestGetAverageSideLeft(){
	return ABECMesForceSideLeft.average;
}

/*
 * Adds a side right value to its buffer.
 */
float ABECTestGetAverageSideRight(){
	return ABECMesForceSideRight.average;
}


/////////////////////////////////////
// API Layer 1

/*
 * Computes the drag of the base by substracting
 * the reference frame values from the aerodynamic values.
 */
float ABECComputeBaseDrag(){
	if(ABEC_NO_W_DragFront > ABEC_NO_W_DragBack){
		return ABEC_NO_W_DragFront-ABEC_NONW_DragFront;
	}
	else{
		return (-1.0)*(ABEC_NO_W_DragBack-ABEC_NONW_DragBack);
	}
}

/*
 * Computes the lift of the base by substracting
 * the reference frame values from the aerodynamic values.
 */
float ABECComputeBaseLift(){
	if(ABEC_NO_W_LiftUp > ABEC_NO_W_LiftDown){
		return ABEC_NO_W_LiftUp-ABEC_NONW_LiftUp;
	}
	else{
		return (-1.0)*(ABEC_NO_W_LiftDown-ABEC_NONW_LiftDown);
	}
}

/*
 * Computes the side of the base by substracting
 * the reference frame values from the aerodynamic values.
 */
float ABECComputeBaseSide(){
	if(ABEC_NO_W_SideLeft > ABEC_NO_W_SideRight){
		return (-1.0)*(ABEC_NO_W_SideLeft-ABEC_NONW_SideLeft);
	}
	else{
		return ABEC_NO_W_SideRight-ABEC_NONW_SideRight;
	}
}
/////

/*
 * Computes the drag of the object+base by substracting
 * the reference frame values from the aerodynamic values.
 */
float ABECComputeObjectBaseDrag(){
	if(ABECMesForceDragFront.average  > ABECMesForceDragBack.average){
		return (ABECMesForceDragFront.average-ABEC_O_NW_DragFront);
	}
	else{
		return (-1.0)*(ABECMesForceDragBack.average-ABEC_O_NW_DragBack);
	}
}
/*
 * Computes the lift of the object+base by substracting
 * the reference frame values from the aerodynamic values.
 */
float ABECComputeObjectBaseLift(){
	if(ABECMesForceLiftUp.average  > ABECMesForceLiftDown.average){
		return ABECMesForceLiftUp.average-ABEC_O_NW_LiftUp;
	}
	else{
		return (-1.0)*(ABECMesForceLiftDown.average-ABEC_O_NW_LiftDown);
	}
}
/*
 * Computes the side of the object+base by substracting
 * the reference frame values from the aerodynamic values.
 */
float ABECComputeObjectBaseSide(){
	if(ABECMesForceSideLeft.average  > ABECMesForceSideRight.average){
		return (-1.0)*(ABECMesForceSideLeft.average-ABEC_O_NW_SideLeft);
	}
	else{
		return ABECMesForceSideRight.average-ABEC_O_NW_SideRight;
	}
}

/*
 * Computes the drag of the object by substracting
 * the object+base drag - base drag values.
 */
float ABECComputeObjectDrag(){
	return ABECComputeObjectBaseDrag()-ABECComputeBaseDrag();
}
/*
 * Computes the lift of the object by substracting
 * the object+base lift - base lift values.
 */
float ABECComputeObjectLift(){
	return ABECComputeObjectBaseLift()-ABECComputeBaseLift();
}
/*
 * Computes the side of the object by substracting
 * the object+base side - base side values.
 */
float ABECComputeObjectSide(){
	return ABECComputeObjectBaseSide()-ABECComputeBaseSide();
}

///////////////

/*
 * Returns the average pressure stored in the buffer.
 */
float ABECGetAveragePressure(){
	return ABECMesPressure.average;
}
/*
 * Returns the average temperature stored in the buffer.
 */
float ABECGetAverageTemperature(){
	return ABECMesTemperature.average;
}
/*
 * Returns the average humidity stored in the buffer.
 */
float ABECGetAverageHumidity(){
	return ABECMesHumidity.average;
}
/*
 * Returns the average wind direction stored in the buffer.
 */
float ABECGetAverageWindDirection(){
	return ABECMesWindVane.average;
}
/*
 * Returns the average velocity stored in the buffer.
 */
float ABECGetAverageVelocity(){
	return ABECMesVelocity.average;
}
