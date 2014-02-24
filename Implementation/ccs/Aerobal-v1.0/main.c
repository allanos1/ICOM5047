#include "a.lib/ABTest.h"


int main(void){
	////////////////////
	// Begin Tests

	ABTestLCD();
	ABTestBMP();
	ABTestRelay();
	ABTestBT();

	// End Tests
	/////////////////////

	lcdClear();
	ABTestLog("ABTL","Testing Successful!","","");
	while(1);
}
