#include "a.lib/ABTest.h"
#include "a.lib/ABUI.h"
#include "a.lib/ABSensorServer.h"

void ABRun(){
	ABUIInit();
	while(1){
		ABSSRefreshSequential();
		ABUIStateMachineBackgroundRun();
	}
}

int main(int argc, const char * argv[]){

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

	//AeroBal Software Main Routine.
	ABRun();

	while(1);

}
