#include "a.lib/ABTest.h"
#include "a.lib/ABUI.h"

void ABRun(){
	ABUIInit();
	while(1){
		ABUIStateMachineBackgroundRun();
	}
}

int main(int argc, const char * argv[]){


	//ABTestAnemometer();
	//ABTestWindVane();
	//ABTestADC();
	//ABTestBluetooth();
	//ABTestDHT();
	//ABTestBinaryCounter();

	//ABBmp085ArrayTest(2,0); //Remember startup_ccs.c! and above comment ABRun().

	//Easy Config.
	//*************************
	//ABTimerTestInterruptHandler();
	//ABTimerTest();

	//AeroBal Software Main Routine.
	ABRun();

	while(1);

}


