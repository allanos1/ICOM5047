#include "a.lib/ABTest.h"


int main(void){
	////////////////////
	// Begin Tests

	ABTestLCD();
	ABTestBMP();
	ABTestRelay();
	ABTestBT();
	//ABTestDHT();

	// End Tests
	/////////////////////

	lcdClear();
	ABTestLog("ABTL: Aerobal","-------------","Aerobal System","Testing Successful!");
	while(1);
}
