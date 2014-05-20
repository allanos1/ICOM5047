/*
 * loadCell.h
 *
 *  Created on: Apr 25, 2014
 *      Author: Juan, Anthony
 */

#ifndef LOADCELL_H_
#define LOADCELL_H_

#include "adc.h"

////////////////////////////
//AeroBal Defined pins for forces.
// PE0-PE5
// Drag:
// 		Front	PE0
//		Back 	PE1
// Lift:
//		Up		PE2
//		Down	PE3
// Side:
//		Left	PE4
//		Right	PE5
//
#define LOADCELL_FORCE_PIN_DRAG_FRONT ADC_PIN_IN03_PE0
#define LOADCELL_FORCE_PIN_DRAG_BACK ADC_PIN_IN02_PE1
#define LOADCELL_FORCE_PIN_LIFT_UP ADC_PIN_IN01_PE2
#define LOADCELL_FORCE_PIN_LIFT_DOWN ADC_PIN_IN00_PE3
#define LOADCELL_FORCE_PIN_SIDE_LEFT ADC_PIN_IN09_PE4
#define LOADCELL_FORCE_PIN_SIDE_RIGHT ADC_PIN_IN08_PE5

#define LOADCELL_FORCE_MUX_DRAG_FRONT ADC_MUX_1
#define LOADCELL_FORCE_MUX_DRAG_BACK ADC_MUX_2
#define LOADCELL_FORCE_MUX_LIFT_UP ADC_MUX_3
#define LOADCELL_FORCE_MUX_LIFT_DOWN ADC_MUX_4
#define LOADCELL_FORCE_MUX_SIDE_LEFT ADC_MUX_5
#define LOADCELL_FORCE_MUX_SIDE_RIGHT ADC_MUX_6

#define LOADCELL_BUFFER_SIZE 10

typedef struct {
	float sum ;
	float buffer[LOADCELL_BUFFER_SIZE];
	int count;
	float average;
} LoadCellBuffer;

int loadCellRefreshSize;

//////////////////////////
// API Layer 0
void loadCellInit();
void loadCellRefresh();
void loadCellRefreshSetSize(int size);


///////////////////////////////////
// API Layer 1 - Load Cell Characterizations.
float loadCellGetDragFront();
float loadCellGetDragBack();
float loadCellGetLiftUp();
float loadCellGetLiftDown();
float loadCellGetSideLeft();
float loadCellGetSideRight();

#endif /* LOADCELL_H_ */
