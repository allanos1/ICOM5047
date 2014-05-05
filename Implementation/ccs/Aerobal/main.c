/*
 * AeroBal v2.0
 *
 * Software for Interface of the AeroBal System.
 *
 * main.c
 *
 *  Created on: Feb 4, 2014
 *      Author: Anthony, Juan, Jesus, Manuel
 */

#include "a.lib/ABTest.h"
#include "a.lib/ABUI.h"
#include "a.lib/ABSensorServer.h"

/*
 * Run Routine.
 */
void ABRun(){
	ABUIInit();
	while(1){
		ABSSRefreshSequential();
		ABUIStateMachineBackgroundRun();
	}
}

/*
 * Testing Routine
 */
void ABRunTest(){
	//Uncomment to run the test you wish to run.
	//ABTestFanRelay();
	//ABBmp085ArrayTest(16,0);
	//ABTestAnemometer();
	//ABTestWindVane();
	//ABTestADC();
	//ABTestBluetooth();
	//ABTestDHT();
	//ABTestBinaryCounter();
	//ABBmp085ArrayTest(16,0);

	//Easy Config.
	//*************************
	//ABTimerTestInterruptHandler();
	//ABTimerTest();
}

/*
 * Main Execution.
 */
int main(int argc, const char * argv[]){

	//Aerobal Testing Routine
	//ABRunTest(); Uncomment to run a test.

	//AeroBal Software Main Routine.
	ABRun();

	while(1);

}
