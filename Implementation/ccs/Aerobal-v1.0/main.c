#include "a.lib/ABTest.h"

int main(void){
	////////////////////
	// Begin Tests

	ABTestLCD(ABTEST_BOOL_TRUE);
	ABTestBMP(ABTEST_BOOL_TRUE);
	ABTestRelay(ABTEST_BOOL_TRUE);
	ABTestBT(ABTEST_BOOL_TRUE);
	ABTestDHT(ABTEST_BOOL_TRUE);
	ABTestAnalog(ABTEST_BOOL_TRUE);

	//Re-Run.
	ABTestLCD(ABTEST_BOOL_FALSE);
	ABTestBMP(ABTEST_BOOL_FALSE);
	ABTestRelay(ABTEST_BOOL_FALSE);
	ABTestBT(ABTEST_BOOL_FALSE);
	ABTestDHT(ABTEST_BOOL_FALSE);
	ABTestAnalog(ABTEST_BOOL_FALSE);

	// End Tests
	/////////////////////

	lcdClear();
	ABTestLog("ABTL: Aerobal","-------------","Aerobal System","Testing Successful!");
	while(1);
}
