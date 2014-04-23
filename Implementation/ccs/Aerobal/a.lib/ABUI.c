/*
 * ABUI.c
 *
 *  Created on: Apr 6, 2014
 *      Author: Administrator
 */

#include "ABUI.h"

/////////////////////////////////
// API Layer 0

//uint32_t ABUIButtonNextStateB0_UP = ABUI_STATE_MAIN_1;
//uint32_t ABUIButtonNextStateB1_DOWN = ABUI_STATE_MAIN_1;
//uint32_t ABUIButtonNextStateB2_ENTER = ABUI_STATE_MAIN_1;
//uint32_t ABUIButtonNextStateB3_CANCEL = ABUI_STATE_MAIN_1;
//uint32_t ABUIButtonNextStateB4_MENU = ABUI_STATE_MAIN_1;
//uint32_t ABUIButtonNextStateB5_PANIC = ABUI_STATE_MAIN_1;

//uint32_t ABUIStateMachineNextState = ABUI_STATE_NONE;
//uint32_t ABUIBackgroundNextState = ABUI_STATE_NONE;

ABUIExpSetup expSetup ;
int ABUIEventCounter = 0;

void ABUIInit(){
	SysCtlDelay(5000000);
	ABUIInitModules();
}

void ABUIInitModules(){
	//LCD
	lcdSerialInit(LCDSERIAL_INIT_UART3);
	lcdSerialSetContrast(0x44);
	lcdSerialClear();
	ABUIMenu_Load_Write();
	SysCtlDelay(6000000);
	//Anemometer
	anemometerInit(ANEMOMETER_PORTF,ANEMOMETER_PIN4);
	lcdSerialCursorLine3();
	lcdSerialWriteString("Sensor: Anemometer...");
	SysCtlDelay(6000000);
	//Wind Vane
	lcdSerialCursorLine3();
	lcdSerialWriteString("Sensor: Wind Vane... ");
	windVaneInit(WIND_VANE_ADC_0,WIND_VANE_ADC_MUX_0,WIND_VANE_ADC_PIN07_PD0);
	SysCtlDelay(6000000);
	// Load Cells. TODO: Library.
	adcInit(ADC_0);
	adcMuxPinSet(ADC_0,ADC_MUX_1,ADC_PIN_IN03_PE0);
	adcMuxPinSet(ADC_0,ADC_MUX_2,ADC_PIN_IN02_PE1);
	adcMuxPinSet(ADC_0,ADC_MUX_3,ADC_PIN_IN01_PE2);
	adcMuxPinSet(ADC_0,ADC_MUX_4,ADC_PIN_IN00_PE3);
	adcMuxPinSet(ADC_0,ADC_MUX_5,ADC_PIN_IN09_PE4);
	adcMuxPinSet(ADC_0,ADC_MUX_6,ADC_PIN_IN08_PE5);
	lcdSerialCursorLine3();
	lcdSerialWriteString("Sensor: Load Cells... ");
	SysCtlDelay(6000000);
	//Humidity
	lcdSerialCursorLine3();
	lcdSerialWriteString("Sensor: Humidity...");
	dhtSetup();
	SysCtlDelay(6000000);
	//Pressure
	lcdSerialCursorLine3();
	lcdSerialWriteString("Sensor: Pressure...");
	bmp085Init(BMP_I2C_MODULE_1);
	SysCtlDelay(6000000);
	//Bluetooth
	lcdSerialCursorLine3();
	lcdSerialWriteString("Com: Bluetooth...");
	bluetoothInit(BLUETOOTH_UART4,BLUETOOTH_UART_BAUD_9600);
	SysCtlDelay(6000000);
	//LEDs
	lcdSerialCursorLine3();
	lcdSerialWriteString("UI: LEDs...      ");
	gpioSetMasterEnable(GPIO_PORTF);
	gpioSetDigitalEnable(GPIO_PORTF,0x0E,0x0E);
	gpioSetDirection(GPIO_PORTF,0x0E,0x0E);
	gpioSetData(GPIO_PORTF,0x0E,0x02);
	SysCtlDelay(6000000);
	//State Machine
	lcdSerialCursorLine3();
	lcdSerialWriteString("UI: State Machine...");
	ABUIStateMachineNextState = ABUI_STATE_MAIN_1;
	ABUIBackgroundNextState = ABUI_STATE_NONE;
	SysCtlDelay(6000000);
	//Buttons
	lcdSerialCursorLine3();
	lcdSerialWriteString("UI: Buttons...      ");
	ABUIButtonNextStateB0_UP = ABUI_STATE_MAIN_1;
	ABUIButtonNextStateB1_DOWN = ABUI_STATE_MAIN_1;
	ABUIButtonNextStateB2_ENTER = ABUI_STATE_MAIN_1;
	ABUIButtonNextStateB3_CANCEL = ABUI_STATE_MAIN_1;
	ABUIButtonNextStateB4_MENU = ABUI_STATE_MAIN_1;
	ABUIButtonNextStateB5_PANIC = ABUI_STATE_MAIN_1;
	ABUIMenu_Main_OptionsSize = 3;
	ABUIMenu_Sensor_OptionsSize = 6;
	SysCtlDelay(6000000);
	buttonsInit();
	ABUIStateMachineRun();

}

void ABUILCDWrite(char * ln1, char* ln2, char* ln3, char* ln4){
	if(stringLength(ln1)){
		lcdSerialCursorLine1();
		lcdSerialWriteString(ln1);
	}
	if(stringLength(ln2)){
		lcdSerialCursorLine2();
		lcdSerialWriteString(ln2);
	}
	if(stringLength(ln3)){
		lcdSerialCursorLine3();
		lcdSerialWriteString(ln3);
	}
	if(stringLength(ln4)){
		lcdSerialCursorLine4();
		lcdSerialWriteString(ln4);
	}

}

void ABUIWriteMenu(char* menuTitle, char* options[], int optionSize, int option){

	lcdSerialClear();
	////////////
	lcdSerialCursorLine1();
	lcdSerialWriteString(menuTitle);

	////////////
	lcdSerialCursorLine2();
	if(option < optionSize){
		if(option < optionSize-2){
			lcdSerialWriteString(" >");
			lcdSerialWriteNumber(option+1);
			lcdSerialWriteString(".");
			lcdSerialWriteString(options[option]);
		}
		else{
			if(option == optionSize-3)
			lcdSerialWriteString(" >");
			else
			lcdSerialWriteString("  ");
			lcdSerialWriteNumber(optionSize-2);
			lcdSerialWriteString(".");
			lcdSerialWriteString(options[optionSize-3]);
		}
	}

	////////////
	lcdSerialCursorLine3();
	if(option < optionSize){
		if(option < optionSize-2){
			lcdSerialWriteString("  ");
			lcdSerialWriteNumber(option+2);
			lcdSerialWriteString(".");
			lcdSerialWriteString(options[option+1]);
		}
		else{
			if(option == optionSize-2)
			lcdSerialWriteString(" >");
			else
			lcdSerialWriteString("  ");
			lcdSerialWriteNumber(optionSize-1);
			lcdSerialWriteString(".");
			lcdSerialWriteString(options[optionSize-2]);
		}
	}

	/////////////
	lcdSerialCursorLine4();
	if(option < optionSize){
		if(option < optionSize-2){
			lcdSerialWriteString(" ");
			lcdSerialWriteNumber(option+3);
			lcdSerialWriteString(".");
			lcdSerialWriteString(options[option+2]);
		}
		else{
			if(option == optionSize-1)
			lcdSerialWriteString(">");
			else
			lcdSerialWriteString(" ");
			lcdSerialWriteNumber(optionSize);
			lcdSerialWriteString(".");
			lcdSerialWriteString(options[optionSize-1]);
		}
	}

}

void ABUIStateMachineBackgroundSetNextState(uint32_t nextState){
	ABUIBackgroundNextState = nextState;
}

void ABUIStateMachineBackgroundReset(){
	ABUIBackgroundNextState = ABUI_STATE_NONE;
}

void ABUIStateMachineSetNextState_InterruptHandler(){
	buttonsInterruptHandler();
	if(buttonsWasPressed()){
		if(buttonsWasPressedB0()){
			ABUIStateMachineNextState = ABUIButtonNextStateB0_UP;
		}
		else if(buttonsWasPressedB1()){
			ABUIStateMachineNextState = ABUIButtonNextStateB1_DOWN;
		}
		else if(buttonsWasPressedB2()){
			ABUIStateMachineNextState = ABUIButtonNextStateB2_ENTER;
		}
		else if(buttonsWasPressedB3()){
			ABUIStateMachineNextState = ABUIButtonNextStateB3_CANCEL;
		}
		else if(buttonsWasPressedB4()){
			ABUIStateMachineNextState = ABUIButtonNextStateB4_MENU;
		}
		else if(buttonsWasPressedB5()){
			ABUIStateMachineNextState = ABUIButtonNextStateB5_PANIC;
		}

	}
	ABUIStateMachineRun();
}

void ABUIButtonsSetNextState(uint32_t nextStateB0,uint32_t nextStateB1,uint32_t nextStateB2,
							 uint32_t nextStateB3,uint32_t nextStateB4,uint32_t nextStateB5){
	ABUIButtonNextStateB0_UP = nextStateB0;
	ABUIButtonNextStateB1_DOWN = nextStateB1;
	ABUIButtonNextStateB2_ENTER = nextStateB2;
	ABUIButtonNextStateB3_CANCEL = nextStateB3;
	ABUIButtonNextStateB4_MENU = nextStateB4;
	ABUIButtonNextStateB5_PANIC = nextStateB5;
}

void ABUIStateMachineRun(){
	if(ABUIStateMachineNextState != ABUI_STATE_NONE
			&& ABUIStateMachineNextState != ABUI_STATE_BUTTON_INCREASE
			&& ABUIStateMachineNextState != ABUI_STATE_BUTTON_DECREASE){
		lcdSerialClear();
		ABUIStateMachineBackgroundReset();
	}
	switch(ABUIStateMachineNextState){
		case ABUI_STATE_MAIN_1:
			ABUIMenu_Main_Write(0xF & ABUI_STATE_MAIN_1);
			ABUIButtonsSetNextState(
					ABUI_STATE_MAIN_1,ABUI_STATE_MAIN_2,
					ABUI_STATE_EXP_1_SETUP_TIME,ABUI_STATE_MAIN_1,
					ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			buttonsEnable();
			break;
		case ABUI_STATE_MAIN_2:
			ABUIMenu_Main_Write(0xF & ABUI_STATE_MAIN_2);
			ABUIButtonsSetNextState(
					ABUI_STATE_MAIN_1,ABUI_STATE_MAIN_3,
					ABUI_STATE_SENSOR_1,ABUI_STATE_MAIN_2,
					ABUI_STATE_MAIN_2,ABUI_STATE_PANIC
			);
			buttonsEnable();
			break;
			case ABUI_STATE_MAIN_3:
			ABUIMenu_Main_Write(0xF & ABUI_STATE_MAIN_3);
			ABUIButtonsSetNextState(
					ABUI_STATE_MAIN_2,ABUI_STATE_MAIN_3,
					ABUI_STATE_SENSOR_1,ABUI_STATE_MAIN_3,
					ABUI_STATE_MAIN_3,ABUI_STATE_PANIC
			);
			buttonsEnable();
			break;
		case ABUI_STATE_SENSOR_1:
			ABUIMenu_Sensor_Write(0xF & ABUI_STATE_SENSOR_1);
			ABUIButtonsSetNextState(
				ABUI_STATE_SENSOR_1,ABUI_STATE_SENSOR_2,
				ABUI_STATE_BG_SENSOR_FORCE,ABUI_STATE_MAIN_1,
				ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			buttonsEnable();
			break;
		case ABUI_STATE_SENSOR_2:
			ABUIMenu_Sensor_Write(0xF & ABUI_STATE_SENSOR_2);
			ABUIButtonsSetNextState(
				ABUI_STATE_SENSOR_1,ABUI_STATE_SENSOR_3,
				ABUI_STATE_BG_SENSOR_PRESSURE,ABUI_STATE_MAIN_1,
				ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			buttonsEnable();
			break;
		case ABUI_STATE_SENSOR_3:
			ABUIMenu_Sensor_Write(0xF & ABUI_STATE_SENSOR_3);
			ABUIButtonsSetNextState(
				ABUI_STATE_SENSOR_2,ABUI_STATE_SENSOR_4,
				ABUI_STATE_BG_SENSOR_TEMPERATURE,ABUI_STATE_MAIN_1,
				ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			buttonsEnable();
			break;
		case ABUI_STATE_SENSOR_4:
			ABUIMenu_Sensor_Write(0xF & ABUI_STATE_SENSOR_4);
			ABUIButtonsSetNextState(
				ABUI_STATE_SENSOR_3,ABUI_STATE_SENSOR_5,
				ABUI_STATE_BG_SENSOR_VELOCITY,ABUI_STATE_MAIN_1,
				ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			buttonsEnable();
			anemometerEnd();
			break;
		case ABUI_STATE_SENSOR_5:
			ABUIMenu_Sensor_Write(0xF & ABUI_STATE_SENSOR_5);
			ABUIButtonsSetNextState(
				ABUI_STATE_SENSOR_4,ABUI_STATE_SENSOR_6,
				ABUI_STATE_BG_SENSOR_HUMIDITY,ABUI_STATE_MAIN_1,
				ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			buttonsEnable();
			break;
		case ABUI_STATE_SENSOR_6:
			ABUIMenu_Sensor_Write(0xF & ABUI_STATE_SENSOR_6);
			ABUIButtonsSetNextState(
				ABUI_STATE_SENSOR_5,ABUI_STATE_SENSOR_6,
				ABUI_STATE_BG_SENSOR_DIRECTION,ABUI_STATE_MAIN_1,
				ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			buttonsEnable();
			break;
		case ABUI_STATE_BG_SENSOR_FORCE:
			ABUIButtonsSetNextState(
				ABUI_STATE_NONE,ABUI_STATE_NONE,
				ABUI_STATE_NONE,ABUI_STATE_SENSOR_1,
				ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			buttonsEnable();
			lcdSerialClear();
			ABUIStateMachineBackgroundSetNextState(ABUI_STATE_BG_SENSOR_FORCE);
			break;
		case ABUI_STATE_BG_SENSOR_PRESSURE:
			ABUIButtonsSetNextState(
				ABUI_STATE_NONE,ABUI_STATE_NONE,
				ABUI_STATE_NONE,ABUI_STATE_SENSOR_2,
				ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			buttonsEnable();
			lcdSerialClear();
			ABUIStateMachineBackgroundSetNextState(ABUI_STATE_BG_SENSOR_PRESSURE);
			break;
		case ABUI_STATE_BG_SENSOR_TEMPERATURE:
			ABUIButtonsSetNextState(
				ABUI_STATE_NONE,ABUI_STATE_NONE,
				ABUI_STATE_NONE,ABUI_STATE_SENSOR_3,
				ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			buttonsEnable();
			lcdSerialClear();
			ABUIStateMachineBackgroundSetNextState(ABUI_STATE_BG_SENSOR_TEMPERATURE);
			break;
		case ABUI_STATE_BG_SENSOR_VELOCITY:
			ABUIButtonsSetNextState(
				ABUI_STATE_SENSOR_4,ABUI_STATE_SENSOR_4,
				ABUI_STATE_SENSOR_4,ABUI_STATE_SENSOR_4,
				ABUI_STATE_SENSOR_4,ABUI_STATE_PANIC
			);
			buttonsEnable();
			lcdSerialClear();
			ABUIStateMachineBackgroundSetNextState(ABUI_STATE_BG_SENSOR_VELOCITY);
			anemometerStart();
			break;
		case ABUI_STATE_BG_SENSOR_HUMIDITY:
			ABUIButtonsSetNextState(
				ABUI_STATE_NONE,ABUI_STATE_NONE,
				ABUI_STATE_NONE,ABUI_STATE_SENSOR_5,
				ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			buttonsEnable();
			lcdSerialClear();
			ABUIStateMachineBackgroundSetNextState(ABUI_STATE_BG_SENSOR_HUMIDITY);
			break;
		case ABUI_STATE_BG_SENSOR_DIRECTION:
			ABUIButtonsSetNextState(
				ABUI_STATE_NONE,ABUI_STATE_NONE,
				ABUI_STATE_NONE,ABUI_STATE_SENSOR_6,
				ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			buttonsEnable();
			lcdSerialClear();
			ABUIStateMachineBackgroundSetNextState(ABUI_STATE_BG_SENSOR_DIRECTION);
			break;
		case ABUI_STATE_EXP_1_SETUP_TIME:
			ABUIButtonsSetNextState(
				ABUI_STATE_BUTTON_INCREASE,ABUI_STATE_BUTTON_DECREASE,
				ABUI_STATE_EXP_2_SETUP_FREQUENCY,ABUI_STATE_MAIN_1,
				ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			buttonsEnable();
			lcdSerialClear();
			ABUIEventCounter = 0;
			ABUIStateMachineBackgroundSetNextState(ABUI_STATE_BG_EXP_SETUP_TIME);
			break;
		case ABUI_STATE_EXP_2_SETUP_FREQUENCY:
			expSetup.timeSeconds = ABUIEventCounter ;
			ABUIButtonsSetNextState(
				ABUI_STATE_BUTTON_INCREASE,ABUI_STATE_BUTTON_DECREASE,
				ABUI_STATE_EXP_3_SETUP_WIND_SPEED,ABUI_STATE_MAIN_1,
				ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			buttonsEnable();
			lcdSerialClear();
			ABUIEventCounter = 0;
			ABUIStateMachineBackgroundSetNextState(ABUI_STATE_BG_EXP_SETUP_FREQUENCY);
			break;
		case ABUI_STATE_EXP_3_SETUP_WIND_SPEED:
			expSetup.frequencyHz = ABUIEventCounter ;
			ABUIButtonsSetNextState(
				ABUI_STATE_BUTTON_INCREASE,ABUI_STATE_BUTTON_DECREASE,
				ABUI_STATE_EXP_4_CALIBRATE_NOOBJECT_CONFIRM,ABUI_STATE_MAIN_1,
				ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			buttonsEnable();
			lcdSerialClear();
			ABUIEventCounter = 0;
			ABUIStateMachineBackgroundSetNextState(ABUI_STATE_BG_EXP_SETUP_WINDSPEED);
			break;
		case ABUI_STATE_EXP_4_CALIBRATE_NOOBJECT_CONFIRM:
			expSetup.windSpeed = ABUIEventCounter;
			ABUIEventCounter = 0;
			ABUIButtonsSetNextState(
				ABUI_STATE_NONE,ABUI_STATE_NONE,
				ABUI_STATE_EXP_5_CALIBRATE_NOOBJECT,ABUI_STATE_MAIN_1,
				ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			ABUIMenu_Experiment_CalibrationNoObjectConfirm();
			buttonsEnable();
			break;
		case ABUI_STATE_EXP_5_CALIBRATE_NOOBJECT:
			ABUIButtonsSetNextState(
				ABUI_STATE_NONE,ABUI_STATE_NONE,
				ABUI_STATE_EXP_6_CALIBRATE_NOOBJECT_WIND_CONFIRM,ABUI_STATE_NONE,
				ABUI_STATE_NONE,ABUI_STATE_PANIC
			);
			ABUIStateMachineBackgroundSetNextState(ABUI_STATE_BG_EXP_CALIBRATE_NOOBJECT);
			buttonsEnable();
			break;
		case ABUI_STATE_EXP_6_CALIBRATE_NOOBJECT_WIND_CONFIRM:
			ABUIButtonsSetNextState(
				ABUI_STATE_NONE,ABUI_STATE_NONE,
				ABUI_STATE_EXP_7_CALIBRATE_NOOBJECT_WIND_MEASURE,ABUI_STATE_NONE,
				ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			ABUIMenu_Experiment_CalibrationNoObjectWindConfirm();
			buttonsEnable();
			break;
		case ABUI_STATE_EXP_7_CALIBRATE_NOOBJECT_WIND_MEASURE:
			ABUIButtonsSetNextState(
				ABUI_STATE_NONE,ABUI_STATE_NONE,
				ABUI_STATE_EXP_8_CALIBRATE_OBJECT_CONFIRM,ABUI_STATE_NONE,
				ABUI_STATE_NONE,ABUI_STATE_PANIC
			);
			ABUIStateMachineBackgroundSetNextState(ABUI_STATE_BG_EXP_CALIBRATE_NOOBJECT_WIND);
			buttonsEnable();
			break;
		case ABUI_STATE_EXP_8_CALIBRATE_OBJECT_CONFIRM:
			ABUIButtonsSetNextState(
				ABUI_STATE_NONE,ABUI_STATE_NONE,
				ABUI_STATE_EXP_9_CALIBRATE_OBJECT_MEASURE,ABUI_STATE_NONE,
				ABUI_STATE_NONE,ABUI_STATE_PANIC
			);
			ABUIMenu_Experiment_CalibrationObjectConfirm();
			buttonsEnable();
			break;
		case ABUI_STATE_EXP_9_CALIBRATE_OBJECT_MEASURE:
			ABUIButtonsSetNextState(
				ABUI_STATE_NONE,ABUI_STATE_NONE,
				ABUI_STATE_EXP_10_OBJECT_WIND_CONFIRM,ABUI_STATE_NONE,
				ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			ABUIStateMachineBackgroundSetNextState(ABUI_STATE_BG_EXP_CALIBRATE_OBJECT);
			break;
		case ABUI_STATE_EXP_10_OBJECT_WIND_CONFIRM:
			ABUIButtonsSetNextState(
				ABUI_STATE_NONE,ABUI_STATE_NONE,
				ABUI_STATE_EXP_11_OBJECT_WIND_MEASURE,ABUI_STATE_NONE,
				ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			ABUIMenu_Experiment_MeasureConfirm();
			buttonsEnable();
			break;
		case ABUI_STATE_EXP_11_OBJECT_WIND_MEASURE:
			ABUIButtonsSetNextState(
				ABUI_STATE_NONE,ABUI_STATE_NONE,
				ABUI_STATE_EXP_12_DISPLAY_RESULTS,ABUI_STATE_NONE,
				ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			ABUIStateMachineBackgroundSetNextState(ABUI_STATE_BG_EXP_MEASURE_OBJECT_WIND);
			buttonsEnable();
			break;
		case ABUI_STATE_EXP_12_DISPLAY_RESULTS:
			ABUIButtonsSetNextState(
				ABUI_STATE_NONE,ABUI_STATE_NONE,
				ABUI_STATE_MAIN_1,ABUI_STATE_NONE,
				ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			ABUIStateMachineBackgroundSetNextState(ABUI_STATE_BG_EXP_RESULTS);
			buttonsEnable();
			break;
		case ABUI_STATE_BUTTON_INCREASE:
			ABUIEventCounter++;
			buttonsEnable();
			break;
		case ABUI_STATE_BUTTON_DECREASE:
			ABUIEventCounter--;
			buttonsEnable();
			break;
		case ABUI_STATE_BTCONTROL:
			break;
		case ABUI_STATE_PANIC:
			lcdSerialClear();
			lcdSerialWriteString("PANIC State");
			buttonsEnable();
			break;
		case ABUI_STATE_ERROR:
			break;
		case ABUI_STATE_RESET:
			break;
		case ABUI_STATE_NONE:
			//lcdSerialClear();
			//lcdSerialWriteString("Reset State");
			buttonsEnable();
			break;

	}
}
void ABUIStateMachineBackgroundRun(){
	switch(ABUIBackgroundNextState){
		case ABUI_STATE_BG_SM_FG_RUN: ABUIStateMachineRun(); break;
		case ABUI_STATE_BG_SENSOR_FORCE: ABUIMenu_Sensor_Force(); break;
		case ABUI_STATE_BG_SENSOR_PRESSURE: ABUIMenu_Sensor_Pressure(); break;
		case ABUI_STATE_BG_SENSOR_TEMPERATURE: ABUIMenu_Sensor_Temperature(); break;
		case ABUI_STATE_BG_SENSOR_VELOCITY: ABUIMenu_Sensor_Velocity(); break;
		case ABUI_STATE_BG_SENSOR_HUMIDITY: ABUIMenu_Sensor_Humidity(); break;
		case ABUI_STATE_BG_SENSOR_DIRECTION: ABUIMenu_Sensor_Direction(); break;
		case ABUI_STATE_BG_EXP_SETUP_TIME: ABUIMenu_Experiment_SetupTime(); break;
		case ABUI_STATE_BG_EXP_SETUP_FREQUENCY: ABUIMenu_Experiment_SetupFrequency(); break;
		case ABUI_STATE_BG_EXP_SETUP_WINDSPEED: ABUIMenu_Experiment_SetupWindSpeed(); break;
		case ABUI_STATE_BG_EXP_CALIBRATE_NOOBJECT: ABUIMenu_Experiment_CalibrationNoObject(); break;
		case ABUI_STATE_BG_EXP_CALIBRATE_NOOBJECT_WIND: ABUIMenu_Experiment_CalibrationNoObjectWind(); break;
		case ABUI_STATE_BG_EXP_CALIBRATE_OBJECT: ABUIMenu_Experiment_CalibrationObject(); break;
		case ABUI_STATE_BG_EXP_MEASURE_OBJECT_WIND: ABUIMenu_Experiment_Measure(); break;
		case ABUI_STATE_BG_EXP_RESULTS: ABUIMenu_Experiment_ShowResults(); break;
		case ABUI_STATE_NONE: break;
	}
}


/////////////////////////////////
// API Layer 1

void ABUIMenu_LoadBar_Write(int delay, int lines){
	lcdSerialCursorLine4();
	lcdSerialWriteString("[");
	if(delay) SysCtlDelay(1000000);
	int i = 0;
	for(i = 0; i < lines  & i < 16; i++){
		lcdSerialWriteString("-");
		if(delay) SysCtlDelay(1000000);
	}
	if(i == 16) lcdSerialWriteString("]");
	if(delay) SysCtlDelay(6000000);
}

void ABUIMenu_Load_Write(){
	ABUILCDWrite(" ---  AeroBal  ---  ",
				 "====================",
				 "Cargando Sistema...",
				 "");

}


//int ABUIMenu_Main_OptionsSize = 3;
int ABUIMenu_Main_Size(){
	return ABUIMenu_Main_OptionsSize;
}

void ABUIMenu_Main_Write(int option){

	char* ABUIMenu_Main_Options[] = {
		"Comenzar Exp.", "Ver Sensores", "Iniciar Prueba"
	};
	ABUIWriteMenu("Menu Principal:",ABUIMenu_Main_Options,ABUIMenu_Main_OptionsSize,option);
}

//////////////////////////////////////////
//int ABUIMenu_Sensor_OptionsSize = 6;
int ABUIMenu_Sensor_Size(){
	return ABUIMenu_Sensor_OptionsSize;
}
void ABUIMenu_Sensor_Write(int option){
	char *ABUIMenu_Sensor_Options[] = {
		"Fuerza", "Presion", "Temperatura", "Velocidad","Humedad","Direccion"
	};
	ABUIWriteMenu("Ver Sensores:",ABUIMenu_Sensor_Options,ABUIMenu_Sensor_OptionsSize,option);
}
/////////////////////////////////////////
void ABUIMenu_Sensor_Stub(char * sensor){
	lcdSerialCursorLine1();
	lcdSerialWriteString(sensor);
	lcdSerialWriteString(" Routine:");
	lcdSerialCursorLine2();
	lcdSerialWriteNumber(ABUIEventCounter++);

}
void ABUIMenu_Sensor_Force(){
	ABUIMenu_Sensor_Stub("Force");
}

void ABUIMenu_Sensor_Pressure(){
	buttonsMask();
	bmp085DataRead(0);
	lcdSerialCursorLine1();
	lcdSerialWriteString("Pressure Sensor:");
	lcdSerialCursorLine2();
	lcdSerialWriteString("Pa: ");
	lcdSerialWriteNumberWithBounds(bmp085GetPressure(),0,5);
	lcdSerialWriteString("  ");
	lcdSerialCursorLine4();
	lcdSerialWriteString("[Cancel]: Back");
	buttonsUnmask();

}
void ABUIMenu_Sensor_Temperature(){
	buttonsMask();
	bmp085DataRead(0);
	lcdSerialCursorLine1();
	lcdSerialWriteString("Temperature Sensor:");
	lcdSerialCursorLine2();
	lcdSerialWriteString("C: ");
	lcdSerialWriteNumberWithBounds(bmp085GetTemperature(),0,5);
	lcdSerialWriteString("  ");
	lcdSerialCursorLine4();
	lcdSerialWriteString("[Cancel]: Back");
	buttonsUnmask();
}

void ABUIMenu_Sensor_Velocity(){
	buttonsMask();
	anemometerSpeedBufferRefresh();
	lcdSerialCursorLine1();
	lcdSerialWriteString("Anemometer Sensor:");
	lcdSerialCursorLine2();
	lcdSerialWriteString("KM/H: ");
	lcdSerialWriteNumberWithBounds(anemometerSpeedConvertKmH(anemometerSpeedBufferGetAverage()),0,5);
	lcdSerialCursorLine3();
	lcdSerialWriteString("MI/H: ");
	lcdSerialWriteNumberWithBounds(anemometerSpeedConvertMiH(anemometerSpeedBufferGetAverage()),0,5);
	lcdSerialWriteString("  ");
	lcdSerialCursorLine4();
	lcdSerialWriteString("[Cancel]: Back");
	buttonsUnmask();
}

void ABUIMenu_Sensor_Humidity(){
	buttonsMask();
	ABUIEventCounter = (ABUIEventCounter + 1) % 10;
	SysCtlDelay(3000000);
	dht11init();
	dht11getData();
	while(dhtIsActive());
	lcdSerialCursorLine1();
	lcdSerialWriteString("Humidity Sensor: [");
	lcdSerialWriteNumber(ABUIEventCounter);
	lcdSerialWriteString("]");
	lcdSerialCursorLine2();
	lcdSerialWriteString("Humd. %: ");
	lcdSerialWriteNumber(dht22GetHumidity());
	lcdSerialCursorLine3();
	lcdSerialWriteString("Temp. C: ");
	lcdSerialWriteNumber(dht22GetTemperature());
	lcdSerialWriteString("  ");
	lcdSerialCursorLine4();
	lcdSerialWriteString("[Cancel]: Back");
	buttonsUnmask();
}


void ABUIMenu_Sensor_Direction(){
	buttonsMask();
	windVaneRefresh(WIND_VANE_BUFFER_SIZE);
	lcdSerialCursorLine1();
	lcdSerialWriteString("Wind Vane Sensor:");
	lcdSerialCursorLine2();
	lcdSerialWriteString("Angle: ");
	lcdSerialWriteNumber(windVaneGetAngle());
	lcdSerialCursorLine4();
	lcdSerialWriteString("[Cancel]: Back");
	buttonsUnmask();

}

/////////////////////////////////////


void ABUIMenu_Experiment_SetupTime(){
	buttonsMask();
	if(ABUIEventCounter<15){
		ABUIEventCounter = 15;
	}
	if(ABUIEventCounter>60){
		ABUIEventCounter = 60;
	}
	lcdSerialCursorLine1();
	lcdSerialWriteString("Tiempo de Prueba:");
	lcdSerialCursorLine2();
	lcdSerialWriteString("Seconds: ");
	lcdSerialWriteNumber(ABUIEventCounter);
	lcdSerialCursorLine4();
	lcdSerialWriteString("[Enter]: Continuar");
	buttonsUnmask();
}
void ABUIMenu_Experiment_SetupFrequency(){
	buttonsMask();
	if(ABUIEventCounter<15){
		ABUIEventCounter = 15;
	}
	if(ABUIEventCounter>60){
		ABUIEventCounter = 60;
	}
	lcdSerialCursorLine1();
	lcdSerialWriteString("Frecuencia de Datos:");
	lcdSerialCursorLine2();
	lcdSerialWriteString("Hz: ");
	lcdSerialWriteNumber(ABUIEventCounter);
	lcdSerialCursorLine4();
	lcdSerialWriteString("[Enter]: Continue");
	buttonsUnmask();
}
void ABUIMenu_Experiment_SetupWindSpeed(){
	buttonsMask();
	if(ABUIEventCounter<5){
		ABUIEventCounter = 5;
	}
	if(ABUIEventCounter>50){
		ABUIEventCounter = 50;
	}
	lcdSerialCursorLine1();
	lcdSerialWriteString("Rapidez Viento:");
	lcdSerialCursorLine2();
	lcdSerialWriteString("Mi/h: ");
	lcdSerialWriteNumber(ABUIEventCounter);
	lcdSerialCursorLine4();
	lcdSerialWriteString("[Enter]: Continue");
	buttonsUnmask();
}

void ABUIMenu_Experiment_CalibrationNoObjectConfirm(){
	buttonsMask();
	ABUILCDWrite("Calibracion Balanza:",
			"Favor de remover",
			"objeto.",
			"[Enter]: Continuar");
	buttonsUnmask();
}

void ABUIMenu_Experiment_CalibrationNoObject(){
	buttonsMask();
	ABUILCDWrite("Calibrando Balanza:",
			"Tomando Medidas sin",
			"objeto.",
			"");
	buttonsUnmask();
}

void ABUIMenu_Experiment_CalibrationNoObjectWindConfirm(){
	buttonsMask();
	ABUILCDWrite("Calibracion Balanza:",
			"Calibracion Viento:",
			"Cerrar tunel sin obj.",
			"[Enter]: Continuar");
	buttonsUnmask();
}
void ABUIMenu_Experiment_CalibrationNoObjectWind(){
	buttonsMask();
	ABUILCDWrite("Calibracion Balanza:",
				"Tomando Medidas sin",
				"objeto con viento.",
				"");
	buttonsUnmask();
}


void ABUIMenu_Experiment_CalibrationObjectConfirm(){
	buttonsMask();
	ABUILCDWrite("Calibracion Objeto:",
			"Colocar objeto en balanza",
			"y cerrar tunel.",
			"[Enter]: Continuar");
	buttonsUnmask();
}
void ABUIMenu_Experiment_CalibrationObject(){
	buttonsMask();
	ABUILCDWrite("Calibracion Objeto:",
			"Tomando Medidas de",
			"objeto en balanza.",
			"");
	buttonsUnmask();
}
void ABUIMenu_Experiment_MeasureConfirm(){
	buttonsMask();
	ABUILCDWrite("Comenzar Experimento:",
			"Presione enter para",
			"comenzar.",
			"[Enter]: Continuar");
	buttonsUnmask();
}
void ABUIMenu_Experiment_Measure(){
	buttonsMask();
	ABUILCDWrite("AeroBal:",
			"Tomando Medidas...",
			"",
			"");
	buttonsUnmask();
}

void ABUIMenu_Experiment_ShowResults(){
	buttonsMask();
	ABUILCDWrite("Resultados:",
			"Resultados Aqui.",
			"",
			"");
	buttonsUnmask();
}
