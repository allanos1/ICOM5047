#include "a.lib/ABTest.h"
#include "a.lib/ABUI.h"

void ABRun(){
	ABUIInit();
	while(1){
		ABUIStateMachineBackgroundRun();
	}
}

int main(int argc, const char * argv[]){


<<<<<<< HEAD
	bmp085ArrayTest(16,0);
}
=======
	//ABTestAnemometer();
	//ABTestWindVane();
	//ABTestADC();
	//ABTestBluetooth();
	//ABTestDHT();
	//ABTestBinaryCounter();
>>>>>>> f19a722fba1d9340186e27bf2be22684737f47b9

	//ABBmp085ArrayTest(16,0);

	//Easy Config.
	//*************************
	//ABTimerTestInterruptHandler();
	//ABTimerTest();

	//AeroBal Software Main Routine.
	ABRun();

	while(1);

}


