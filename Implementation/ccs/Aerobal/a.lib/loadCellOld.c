/*
 * LoadCell.c
 *
 *  Created on: Nov 18, 2013
 *      Author: JJLB
 */
/****************************************************/
/*					Instructions					*/
/****************************************************/
/*
 *
 *
 *
 */
#include "loadCellOld.h"
#include "driverlib/adc.h"
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"

#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"
#include "driverlib/timer.h"

#define WIND_VANE_ARR_SIZE 51

void loadCellSetup();
void loadCellgetData();
float loadCellgetValues(int sensorIndex,int units);
/****************************************************/
/*	Variable Definitions							*/
/****************************************************/

int MAX_VAL = 1000;
int values[1000];
int counter = 0;
uint32_t pui32ADC0Value[7];



float _windVane_ADC[] = {0, 3.68, 300.0, 322.0, 336.0,
		374.00, 536.00, 684.00, 886.00, 1057.0,
		1220.0, 1380.0, 1585.0, 1800.0, 1920.0,
		2090.0, 2270.0, 2442.0, 2592.0, 2773.0,
		2911.0, 3022.0, 3103.0, 3194.0, 3262.0,
		3323.0, 3366.0, 3385.0, 3416.0, 3454.0,
		3491.0, 3524.0, 3558.0, 3602.0, 3627.0,
		3665.0, 3700.0, 3728.0, 3762.0, 3788.0,
		3822.0, 3839.0, 3877.0, 3880.0, 3900.0,
		3917.0, 3953.0, 3960.0, 3982.0, 3990.0,
		4086.5};

float _windVane_ANG[] =
	  { 00.000, 22.500, 28.125, 33.750, 39.375,
		45.000, 50.625, 56.250, 61.875, 67.500,
		73.125, 78.750, 84.375, 90.000, 95.625,
		101.250, 106.875, 112.500, 118.125, 123.750,
		129.375, 135.000, 140.625, 146.250, 151.875,
		157.500, 162.625, 168.250, 173.875, 180.000,
		185.625, 191.250, 196.875, 202.500, 208.125,
		213.750, 219.375, 225.000, 230.625, 236.250,
		241.875, 247.500, 253.125, 258.750, 264.375,
		270.000, 275.625, 281.250, 286.875,  292.500,
		297.000};

/****************************************************/
/*				Function Definitions				*/
/****************************************************/

void initializeADC(void){

	//SysCtlClockSet(SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

	GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4);

	ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
							//[PE0,PE4,PE1, PE2 PE3]
	HWREG(ADC0_BASE | 0x040) = 0x00000125; //Port Selection Port 9 / Port 2 / Port 1 / Port 0
	HWREG(ADC0_BASE | 0x044) = 0b00000000000001100000000000000000; //Sample until port 3
	HWREG(ADC0_BASE | 0x050) = 0x00000000; //Save Samples in FIFOn

	//HWREG(ADC0_BASE | 0x030)=0x6;	//Set Sample Averaging Control to 64x Hardware Oversampling
	//HWREG(ADC0_BASE | 0x024)=0x8;	//Set Phase Shift by 180 degree

	ADCSequenceEnable(ADC0_BASE, 0);

	ADCIntClear(ADC0_BASE, 0);
}

float pow2(int base, int exp){
	float result = 1 ;
	int i = 0;
	if(exp >= 0){
		for(i = 0; i < exp; i++){
			result *= base ;
		}
	}
	else{
		for(i = 0; i > exp; i--){
			result /= base;
		}
	}
	return result ;
}
float LCWindVaneGetAngle(float x){
	/* WindVane Formula:
	 *
	 * ADCVal >=1159 : angle = (-1) 0.0338*ADCVal + 170.58
	 * ADCVal < 1159 : angle = 0.0001*(ADCVal^2) - 0.3219*ADCVal + 359.44
	 */
	if(x > 4096 || x < 0){
		return -1 ;
	}

	int i = 0;
	float x1;
	float x2;
	float y1;
	float y2;
	for(i = 0; i < WIND_VANE_ARR_SIZE; i++){
		if(_windVane_ADC[i] > x){
			x1 = _windVane_ADC[i];
			x2 = _windVane_ADC[i+ 1];
			y1 = _windVane_ANG[i];
			y2 = _windVane_ANG[i+1];
			break;
		}
	}

	return (y1 + (y2-y1)*(x-x1)/(x2-x1))+23;

}
float averageValues(int sensorIndex){
	float average = 0;
	while(counter < MAX_VAL){
		values[counter++] = pui32ADC0Value[sensorIndex];
		loadCellgetData();
	}
		counter = 0;
		int i;
		for( i = 0; i < MAX_VAL; i++){
			average += values[i];
		}
		average /= MAX_VAL;

		return average;
}/****Main Functions****/
void loadCellSetup(){
	initializeADC();
}

void loadCellgetData(){

	SysCtlClockSet(SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
	//
	// Trigger the ADC conversion.
	//
	ADCProcessorTrigger(ADC0_BASE, 0);

	//
	// Wait for conversion to be completed.
	//
	while(!ADCIntStatus(ADC0_BASE, 0, false))
	{
	}
	//
	// Clear the ADC interrupt flag.
	//
	ADCIntClear(ADC0_BASE, 0);

	//
	// Read ADC Value.
	//
	ADCSequenceDataGet(ADC0_BASE, 0, pui32ADC0Value);
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
	SysCtlDelay(10);
}

float loadCellgetWeight(float average,int units,int sensorIndex){
	switch(sensorIndex){
			case  LOADCELL_LIFT_UP:
				if(units == OUNCES){
					return ((average - 148.81)/4.7143); //SG0
				}
				else{
					return ((average - 148.81)/4.7143)/16; //SG0
				}
			//break;
			case  LOADCELL_LIFT_DOWN:
				if(units == OUNCES){
					return ((average-125.22)/12.302); //SG1
				}
				else{
					return ((average-125.22)/12.302)/16; //SG1
				}
			//break;
			case  LOADCELL_DRAG_FRONT:
				if(units == OUNCES){
					return ((average-146)/5.9375); //SG2
				}
				else{
					return ((average-146)/5.9375)/16; //SG2
				}
			//break;
			case  LOADCELL_DRAG_BACK:
				if(units == OUNCES){
					return ((average-159.3)/7.7626); //SG3
				}
				else{
					return ((average-159.3)/7.7626)/16; //SG3
				}
			case WIND_VANE:
				return average ;
			//break;
	}
	return -1;
}

float loadCellgetValues(int sensorIndex, int units){
	switch(sensorIndex){
		case  LOADCELL_LIFT_UP:
			loadCellgetData();
			return loadCellgetWeight(averageValues(sensorIndex),units,sensorIndex);
		//break;
		case  LOADCELL_LIFT_DOWN:
			loadCellgetData();
			return loadCellgetWeight(averageValues(sensorIndex),units,sensorIndex);
		//break;
		case  LOADCELL_DRAG_FRONT:
			loadCellgetData();
			return loadCellgetWeight(averageValues(sensorIndex),units,sensorIndex);
		//break;
		case  LOADCELL_DRAG_BACK:
			loadCellgetData();
			return loadCellgetWeight(averageValues(sensorIndex),units,sensorIndex);
		case WIND_VANE:
			loadCellgetData();
			return LCWindVaneGetAngle(pui32ADC0Value[4]);
			//return averageValues(sensorIndex);
			//break;
	}
	return -1;
}

/**** Helper Functions *****/









