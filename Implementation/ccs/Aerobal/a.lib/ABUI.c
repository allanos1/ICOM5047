/*
 * ABUI.c
 *
 *  Created on: Apr 6, 2014
 *      Author: Administrator
 */

#include "ABUI.h"

/////////////////////////////////
// API Layer 0

/*
 * Maintains the state of the experiments performed on the HW UI.
 */
ABUIExpSetup expSetup ;

/*
 * Main initializer for the UI. Delays to
 * wait for stabilization of the circuit, primarily
 * the LCD Serial Backpack.
 */
void ABUIInit(){
	SysCtlDelay(3000000);
	ABUIInitModules();
}

/*
 * Blinks the power LED and waits the desired amount of delay.
 */
void ABUIPowerWait(uint32_t waitTime){
	gpioSetData(GPIO_PORTA,0x20,0x00);
	SysCtlDelay(waitTime/4);
	gpioSetData(GPIO_PORTA,0x20,~gpioGetData(GPIO_PORTA,0x20));
	SysCtlDelay(waitTime/4);
	gpioSetData(GPIO_PORTA,0x20,~gpioGetData(GPIO_PORTA,0x20));
	SysCtlDelay(waitTime/4);
	gpioSetData(GPIO_PORTA,0x20,~gpioGetData(GPIO_PORTA,0x20));
	SysCtlDelay(waitTime/4);
	gpioSetData(GPIO_PORTA,0x20,~gpioGetData(GPIO_PORTA,0x20));
}

/*
 * Writes a wait bar with no amount indication.
 * Step is a number between 0-15.
 */
void ABUIWriteWait(int step){
	if(step < 0) step = 0;
	if(step > 15) step = 15;
	lcdSerialCursorLine4();
	lcdSerialWriteString(" [");
	int i = 0;
	for(i = 0; i < step; i++) lcdSerialWriteString("=");
	for(i = step; i < 16; i++) lcdSerialWriteString(" ");
	lcdSerialWriteString("]");

}

/*
 * Writes a number that indicates the ratio total
 * of the wait time.
 */
void ABUIWriteWaitTimed(float current, float total, char* timeUnit){
	lcdSerialCursorLine4();
	lcdSerialWriteString(" [");
	int i = 0;
	for(i = 0; i < (int)8*(current/total); i++) lcdSerialWriteString("=");
	for(; i < 8; i++) lcdSerialWriteString(" ");
	lcdSerialWriteString("] ");
	lcdSerialWriteNumber(current);
	lcdSerialWriteString("/");
	lcdSerialWriteNumber(total);
	lcdSerialWriteString(" ");
	lcdSerialWriteString(timeUnit);

}

/*
 * Writes a module Load message.
 */
void ABUIWriteLoadMessage(char *message){
	lcdSerialCursorLine3();
	lcdSerialWriteString(message);
	int i = stringLength(message);
	for(; i<20;i++){
		lcdSerialWriteString(" ");
	}
}


/*
 * Main initializer for the UI. This function initializes all
 * of the modules of AeroBal to be interfaced with the UI.
 */
void ABUIInitModules(){
	//LEDs
	gpioSetMasterEnable(GPIO_PORTA);
	gpioSetDigitalEnable(GPIO_PORTA,0x20,0x20);
	gpioSetDirection(GPIO_PORTA,0x20,0x20);
	gpioSetData(GPIO_PORTA,0x20,0x20);
	SysCtlDelay(6000000);
	//LCD
	lcdSerialInit(LCDSERIAL_INIT_UART3);
	lcdSerialSetContrast(0x44);
	lcdSerialClear();
	ABUIMenu_Load_Write();
	ABUIWriteWait(0);
	ABUIPowerWait(4000000);
	//Anemometer
	ABUIWriteLoadMessage("Sensor: Anemometer...");
	anemometerInit(ANEMOMETER_PORTF,ANEMOMETER_PIN4);
	ABUIWriteWait(1);
	ABUIPowerWait(4000000);
	//Wind Vane
	ABUIWriteLoadMessage("Sensor: Wind Vane...");
	windVaneInit(WIND_VANE_ADC_0,WIND_VANE_ADC_MUX_0,WIND_VANE_ADC_PIN07_PD0);
	ABUIWriteWait(2);
	ABUIPowerWait(4000000);
	// Load Cells. TODO: Library.
	ABUIWriteLoadMessage("Sensor: Load Cells...");
	loadCellInit();
	loadCellRefreshSetSize(10);
	ABUIWriteWait(3);
	ABUIPowerWait(4000000);
	//Humidity
	ABUIWriteLoadMessage("Sensor: Humidity...");
	dhtSetup();
	ABUIWriteWait(4);
	ABUIPowerWait(4000000);
	//MPSA
	ABUIWriteLoadMessage("Sensor: MPSA...");
	//bmp085Init(BMP_I2C_MODULE_1);
	bmp085ArrayInit(GPIO_PORTD, GPIO_PIN_2 , GPIO_PORTD, GPIO_PIN_3, 14, true);
	bmp085ArraySetCurrentSensor(0);
	ABUIWriteWait(5);
	ABUIPowerWait(4000000);
	//Relay
	ABUIWriteLoadMessage("Control Motor: On");
	motorAtvInit();
	ABUIWriteWait(6);
	ABUIPowerWait(4000000);
	//Timer Library
	ABUIWriteLoadMessage("Time: ABTime...");
	ABTimeStart();
	ABUIWriteWait(12);
	ABUIPowerWait(4000000);
	//Bluetooth
	ABUIWriteLoadMessage("Com: Bluetooth...");
	bluetoothInit(BLUETOOTH_UART4,BLUETOOTH_UART_BAUD_9600);
	ABUIWriteWait(13);
	ABUIPowerWait(4000000);
	//State Machine
	ABUIWriteLoadMessage("UI: State Machine...");
	ABUIStateMachineNextState = ABUI_STATE_MAIN_1;
	ABUIBackgroundNextState = ABUI_STATE_NONE;
	ABUIWriteWait(14);
	ABUIPowerWait(4000000);
	//Buttons
	ABUIWriteLoadMessage("UI: Buttons...");
	ABUIButtonNextStateB0_UP = ABUI_STATE_MAIN_1;
	ABUIButtonNextStateB1_DOWN = ABUI_STATE_MAIN_1;
	ABUIButtonNextStateB2_ENTER = ABUI_STATE_MAIN_1;
	ABUIButtonNextStateB3_CANCEL = ABUI_STATE_MAIN_1;
	ABUIButtonNextStateB4_MENU = ABUI_STATE_MAIN_1;
	ABUIButtonNextStateB5_PANIC = ABUI_STATE_MAIN_1;

	//UI Config
	ABUIMenu_Main_OptionsSize = 5; //Size of Main Menu.
	ABUIMenu_Sensor_OptionsSize = 8; //Size of Sensor Menu.
	ABUIMenu_Control_OptionsSize = 3; //Size of control Menu.
	motorAtvSetTargetSpeed(5); //Default Initial Target Speed
	ABUIStateMachineSlot01 = ABUI_STATE_NONE; // Slot for state machine.

	ABUIWriteWait(16);
	ABUIPowerWait(4000000);
	buttonsInit();
	gpioSetData(GPIO_PORTA,0x20,0x20); //Leave the Power Light on.
	ABUIStateMachineRun();

}

/*
 * Writes the 4 given strings in the LCD.
 */
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
/*
 * Writes a menu with a title, and its options along with the selector.
 */
void ABUIWriteMenu(char* menuTitle, char* options[], int optionSize, int option){

	lcdSerialClear();
	//////////////////////////////////
	lcdSerialCursorLine1();
	lcdSerialWriteString(menuTitle);
	//////////////////////////////////
	lcdSerialCursorLine2();
	if(option < optionSize){
		if(option < optionSize-2){
			lcdSerialWriteString(">");
			lcdSerialWriteNumber(option+1);
			lcdSerialWriteString(".");
			lcdSerialWriteString(options[option]);
		}
		else{
			////////
			if(optionSize >= 3){
				if(option == optionSize-3)
				lcdSerialWriteString(">");
				else
				lcdSerialWriteString(" ");
				lcdSerialWriteNumber(optionSize-2);
				lcdSerialWriteString(".");
				lcdSerialWriteString(options[optionSize-3]);
			}
			else{
				if(option < optionSize && optionSize > 0){
					if(option == 0)
					lcdSerialWriteString(">");
					else
					lcdSerialWriteString(" ");
					lcdSerialWriteNumber(1);
					lcdSerialWriteString(".");
					lcdSerialWriteString(options[0]);
				}
			}
		}
	}

	////////////
	lcdSerialCursorLine3();
	if(option < optionSize){
		if(option < optionSize-2){
			lcdSerialWriteString(" ");
			lcdSerialWriteNumber(option+2);
			lcdSerialWriteString(".");
			lcdSerialWriteString(options[option+1]);
		}
		else{
			if(optionSize >= 3){
				if(option == optionSize-2)
				lcdSerialWriteString(">");
				else
				lcdSerialWriteString(" ");
				lcdSerialWriteNumber(optionSize-1);
				lcdSerialWriteString(".");
				lcdSerialWriteString(options[optionSize-2]);
			}
			else{
				if(option < optionSize && optionSize > 1){
					if(option == 1)
					lcdSerialWriteString(">");
					else
					lcdSerialWriteString(" ");
					lcdSerialWriteNumber(2);
					lcdSerialWriteString(".");
					lcdSerialWriteString(options[1]);
				}
			}
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
			if(optionSize >= 3){
				if(option == optionSize-1)
				lcdSerialWriteString(">");
				else
				lcdSerialWriteString(" ");
				lcdSerialWriteNumber(optionSize);
				lcdSerialWriteString(".");
				lcdSerialWriteString(options[optionSize-1]);
			}
			else{

			}
		}
	}

}

/*
 * Sets the next state of the state machine. The State
 * machine must be executed contrary to the background
 * state machine.
 */
void ABUIStateMachineSetNextState(uint32_t nextState){
	ABUIStateMachineNextState = nextState;
}

/*
 * Sets the next state of the background state machine.
 */
void ABUIStateMachineBackgroundSetNextState(uint32_t nextState){
	ABUIBackgroundNextState = nextState;
}

/*
 * Resets the Background State machine state to NONE.
 */
void ABUIStateMachineBackgroundReset(){
	ABUIBackgroundNextState = ABUI_STATE_NONE;
}

/*Interrupt Handler:
 * | extern void ABUIStateMachineSetNextState_InterruptHandler()
 ******************
 * Sets the next state of the state machine and executes it.
 *
 */
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

/*
 * Sets the next state that each button will have on next press.
 */
void ABUIButtonsSetNextState(uint32_t nextStateB0,uint32_t nextStateB1,uint32_t nextStateB2,
							 uint32_t nextStateB3,uint32_t nextStateB4,uint32_t nextStateB5){
	ABUIButtonNextStateB0_UP = nextStateB0;
	ABUIButtonNextStateB1_DOWN = nextStateB1;
	ABUIButtonNextStateB2_ENTER = nextStateB2;
	ABUIButtonNextStateB3_CANCEL = nextStateB3;
	ABUIButtonNextStateB4_MENU = nextStateB4;
	ABUIButtonNextStateB5_PANIC = nextStateB5;
}

/*
 * The AeroBal State Machine. The #define-d identifiers
 * denote and give an idea of what a state is about.
 */
void ABUIStateMachineRun(){
	if(ABUIStateMachineNextState != ABUI_STATE_NONE
			&& ABUIStateMachineNextState != ABUI_STATE_BUTTON_INCREASE
			&& ABUIStateMachineNextState != ABUI_STATE_BUTTON_DECREASE){
		lcdSerialClear();
		ABUIStateMachineBackgroundReset();
		if(ABUIResetMotor){
			//motorAtvTurnOff();
			ABUIResetMotor=0;
		}
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
					ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			buttonsEnable();
			break;
		case ABUI_STATE_MAIN_3:
			ABUIMenu_Main_Write(0xF & ABUI_STATE_MAIN_3);
			ABUIButtonsSetNextState(
					ABUI_STATE_MAIN_2,ABUI_STATE_MAIN_4,
					ABUI_STATE_CONTROL_1,ABUI_STATE_MAIN_3,
					ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			buttonsEnable();
			break;
		case ABUI_STATE_MAIN_4:
			ABUIMenu_Main_Write(0xF & ABUI_STATE_MAIN_4);
			ABUIButtonsSetNextState(
					ABUI_STATE_MAIN_3,ABUI_STATE_MAIN_5,
					ABUI_STATE_EXP_12_DISPLAY_RESULTS,ABUI_STATE_MAIN_4,
					ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			buttonsEnable();
			break;
		case ABUI_STATE_MAIN_5:
			ABUIMenu_Main_Write(0xF & ABUI_STATE_MAIN_5);
			ABUIButtonsSetNextState(
					ABUI_STATE_MAIN_4,ABUI_STATE_MAIN_5,
					ABUI_STATE_MAIN_5,ABUI_STATE_MAIN_5,
					ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			buttonsEnable();
			break;
		case ABUI_STATE_SENSOR_1:
			ABUIMenu_Sensor_Write(0xF & ABUI_STATE_SENSOR_1);
			ABUIButtonsSetNextState(
				ABUI_STATE_SENSOR_1,ABUI_STATE_SENSOR_2,
				ABUI_STATE_BG_SENSOR_FORCE,ABUI_STATE_MAIN_2,
				ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			buttonsEnable();
			break;
		case ABUI_STATE_SENSOR_2:
			ABUIMenu_Sensor_Write(0xF & ABUI_STATE_SENSOR_2);
			ABUIButtonsSetNextState(
				ABUI_STATE_SENSOR_1,ABUI_STATE_SENSOR_3,
				ABUI_STATE_BG_SENSOR_PRESSURE,ABUI_STATE_MAIN_2,
				ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			buttonsEnable();
			break;
		case ABUI_STATE_SENSOR_3:
			ABUIMenu_Sensor_Write(0xF & ABUI_STATE_SENSOR_3);
			ABUIButtonsSetNextState(
				ABUI_STATE_SENSOR_2,ABUI_STATE_SENSOR_4,
				ABUI_STATE_BG_SENSOR_TEMPERATURE,ABUI_STATE_MAIN_2,
				ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			buttonsEnable();
			break;
		case ABUI_STATE_SENSOR_4:
			ABUIMenu_Sensor_Write(0xF & ABUI_STATE_SENSOR_4);
			ABUIButtonsSetNextState(
				ABUI_STATE_SENSOR_3,ABUI_STATE_SENSOR_5,
				ABUI_STATE_BG_SENSOR_VELOCITY,ABUI_STATE_MAIN_2,
				ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			buttonsEnable();
			anemometerEnd();
			break;
		case ABUI_STATE_SENSOR_5:
			ABUIMenu_Sensor_Write(0xF & ABUI_STATE_SENSOR_5);
			ABUIButtonsSetNextState(
				ABUI_STATE_SENSOR_4,ABUI_STATE_SENSOR_6,
				ABUI_STATE_BG_SENSOR_HUMIDITY,ABUI_STATE_MAIN_2,
				ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			buttonsEnable();
			break;
		case ABUI_STATE_SENSOR_6:
			ABUIMenu_Sensor_Write(0xF & ABUI_STATE_SENSOR_6);
			ABUIButtonsSetNextState(
				ABUI_STATE_SENSOR_5,ABUI_STATE_SENSOR_7,
				ABUI_STATE_BG_SENSOR_DIRECTION,ABUI_STATE_MAIN_2,
				ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			buttonsEnable();
			break;
		case ABUI_STATE_SENSOR_7:
			ABUIMenu_Sensor_Write(0xF & ABUI_STATE_SENSOR_7);
			ABUIButtonsSetNextState(
				ABUI_STATE_SENSOR_6,ABUI_STATE_SENSOR_8,
				ABUI_STATE_BG_SENSOR_MPSA,ABUI_STATE_MAIN_2,
				ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			buttonsEnable();
			break;
		case ABUI_STATE_SENSOR_8:
			ABUIMenu_Sensor_Write(0xF & ABUI_STATE_SENSOR_8);
			ABUIButtonsSetNextState(
				ABUI_STATE_SENSOR_7,ABUI_STATE_SENSOR_8,
				ABUI_STATE_BG_SENSOR_PSPEED,ABUI_STATE_MAIN_2,
				ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			buttonsEnable();
			break;
		case ABUI_STATE_CONTROL_1:
			ABUIMenu_Control_Write(0xF & ABUI_STATE_CONTROL_1);
			ABUIButtonsSetNextState(
				ABUI_STATE_CONTROL_1,ABUI_STATE_CONTROL_2,
				ABUI_STATE_BG_CTL_FAN,ABUI_STATE_MAIN_3,
				ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			buttonsEnable();
			break;
		case ABUI_STATE_CONTROL_2:
			ABUIMenu_Control_Write(0xF & ABUI_STATE_CONTROL_2);
			ABUIButtonsSetNextState(
				ABUI_STATE_CONTROL_1,ABUI_STATE_CONTROL_3,
				ABUI_STATE_BG_CTL_WIND_SPEED,ABUI_STATE_MAIN_3,
				ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			buttonsEnable();
			break;
		case ABUI_STATE_CONTROL_3:
			ABUIMenu_Control_Write(0xF & ABUI_STATE_CONTROL_3);
			ABUIButtonsSetNextState(
				ABUI_STATE_CONTROL_2,ABUI_STATE_CONTROL_3,
				ABUI_STATE_BG_CTL_SET_TEST_WINDSPEED,ABUI_STATE_MAIN_3,
				ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			buttonsEnable();
			break;
		case ABUI_STATE_BG_SENSOR_FORCE:
			ABUIButtonsSetNextState(
				ABUI_STATE_BUTTON_INCREASE,ABUI_STATE_BUTTON_DECREASE,
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
				ABUI_STATE_NONE,ABUI_STATE_NONE,
				ABUI_STATE_NONE,ABUI_STATE_SENSOR_4,
				ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
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
		case ABUI_STATE_BG_SENSOR_MPSA:
			ABUIButtonsSetNextState(
				ABUI_STATE_BUTTON_INCREASE,ABUI_STATE_BUTTON_DECREASE,
				ABUI_STATE_NONE,ABUI_STATE_SENSOR_7,
				ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			buttonsEnable();
			lcdSerialClear();
			bmp085ArrayReset();
			ABUIEventCounter=0;
			ABUIStateMachineBackgroundSetNextState(ABUI_STATE_BG_SENSOR_MPSA);
			break;
		case ABUI_STATE_BG_SENSOR_PSPEED:
			ABUIButtonsSetNextState(
				ABUI_STATE_BUTTON_INCREASE,ABUI_STATE_BUTTON_DECREASE,
				ABUI_STATE_NONE,ABUI_STATE_SENSOR_7,
				ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			buttonsEnable();
			lcdSerialClear();
			ABUIEventCounter=0;
			ABUIStateMachineBackgroundSetNextState(ABUI_STATE_BG_SENSOR_PSPEED);
			break;
		case ABUI_STATE_BG_CTL_FAN:
			ABUIButtonsSetNextState(
				ABUI_STATE_BUTTON_INCREASE,ABUI_STATE_BUTTON_DECREASE,
				ABUI_STATE_NONE,ABUI_STATE_CONTROL_1,
				ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			buttonsEnable();
			lcdSerialClear();
			motorAtvSpeedReset();
			motorAtvTurnOn();
			ABUIEventCounter=0;
			ABUIResetMotor=1;
			anemometerStart();
			lcdSerialClear();
			ABUIStateMachineBackgroundSetNextState(ABUI_STATE_BG_CTL_FAN);
			break;
		case ABUI_STATE_BG_CTL_WIND_SPEED:
			ABUIButtonsSetNextState(
				ABUI_STATE_BUTTON_INCREASE,ABUI_STATE_BUTTON_DECREASE,
				ABUI_STATE_NONE,ABUI_STATE_CONTROL_2,
				ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			buttonsEnable();
			lcdSerialClear();
			motorAtvSpeedReset();
			motorAtvTurnOn();
			ABUIEventCounter=motorAtvGetTargetSpeed();
			ABUICounter01 = 0;
			ABUIIntVariable01 = 2;
			//ABUIResetMotor=1;
			anemometerStart();
			lcdSerialClear();
			ABUIT0 = ABTimeGetReference();
			ABUIStateMachineBackgroundSetNextState(ABUI_STATE_BG_CTL_WIND_SPEED);
			break;
		case ABUI_STATE_BG_CTL_SET_TEST_WINDSPEED:
			ABUIButtonsSetNextState(
				ABUI_STATE_BUTTON_INCREASE,ABUI_STATE_BUTTON_DECREASE,
				ABUI_STATE_CONTROL_3,ABUI_STATE_CONTROL_3,
				ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			ABUIEventCounter = motorAtvGetTargetSpeed();
			ABUIStateMachineBackgroundSetNextState(ABUI_STATE_BG_EXP_SETUP_WINDSPEED);
			break;
		case ABUI_STATE_EXP_1_SETUP_TIME:
			ABUIButtonsSetNextState(
				ABUI_STATE_BUTTON_INCREASE,ABUI_STATE_BUTTON_DECREASE,
				ABUI_STATE_EXP_2_SETUP_FREQUENCY,ABUI_STATE_MAIN_1,
				ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			buttonsEnable();
			lcdSerialClear();
			ABUIEventCounter = (ABUI_DEFAULTS_TIME_MAX-ABUI_DEFAULTS_TIME_MIN)/2;
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
			ABUIEventCounter = (ABUI_DEFAULTS_FREQUENCY_MAX-ABUI_DEFAULTS_FREQUENCY_MIN)/2;
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
			ABUIEventCounter = (ABUI_DEFAULTS_WINDSPEED_MAX-ABUI_DEFAULTS_WINDSPEED_MIN)/2;
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
			ABUIT0 = ABTimeGetReference();
			ABUIF0 = ABTimeGetReference();
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
				ABUI_STATE_NONE,ABUI_STATE_NONE,
				ABUI_STATE_NONE,ABUI_STATE_PANIC
			);
			///////
			buttonsMask();
			lcdSerialClear();
			motorAtvSpeedReset();
			motorAtvTurnOn();
			motorAtvSetTargetSpeed(expSetup.windSpeed);
			lcdSerialClear();
			motorAtvSetSpeedCharacterized();
			anemometerStart();
			ABUIT0 = ABTimeGetReference();
			ABUIStateMachineBackgroundSetNextState(ABUI_STATE_BG_EXP_WIND_SPEED);
			ABUIBackgroundStateSlot1 = ABUI_STATE_BG_EXP_CALIBRATE_NOOBJECT_WIND;
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
			ABUIT0 = ABTimeGetReference();
			ABUIF0 = ABTimeGetReference();
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

			buttonsMask();
			lcdSerialClear();
			motorAtvSpeedReset();
			motorAtvTurnOn();
			motorAtvSetTargetSpeed(expSetup.windSpeed);
			lcdSerialClear();
			motorAtvSetSpeedCharacterized();
			anemometerStart();
			ABUIT0 = ABTimeGetReference();
			lcdSerialClear();
			ABUIStateMachineBackgroundSetNextState(ABUI_STATE_BG_EXP_WIND_SPEED);
			ABUIBackgroundStateSlot1 = ABUI_STATE_BG_EXP_MEASURE_OBJECT_WIND;
			buttonsEnable();
			break;
		case ABUI_STATE_EXP_12_DISPLAY_RESULTS:
			ABUIButtonsSetNextState(
				ABUI_STATE_BUTTON_INCREASE,ABUI_STATE_BUTTON_DECREASE,
				ABUI_STATE_MAIN_1,ABUI_STATE_MAIN_1,
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
			relayOff();
			motorAtvSpeedReset();
			lcdSerialClear();
			lcdSerialWriteString("AeroBal PANIC State");
			lcdSerialCursorLine2();
			lcdSerialWriteString("====================");
			lcdSerialCursorLine3();
			lcdSerialWriteString("VFD has been reset.");
			lcdSerialCursorLine4();
			lcdSerialWriteString("Press any button.");
			ABUIButtonsSetNextState(
						ABUI_STATE_MAIN_1,ABUI_STATE_MAIN_1,
						ABUI_STATE_MAIN_1,ABUI_STATE_MAIN_1,
						ABUI_STATE_MAIN_1,ABUI_STATE_PANIC
			);
			buttonsEnable();
			break;
		case ABUI_STATE_ERROR:
			break;
		case ABUI_STATE_RESET:
			break;
		case ABUI_STATE_NONE:
			buttonsEnable();
			break;

	}
}

/*
 * The background state machine. This state machine is executed
 * when the micro is not doing another thing. A function is assigned
 * to the state that it is handling.
 */
void ABUIStateMachineBackgroundRun(){
	switch(ABUIBackgroundNextState){
		case ABUI_STATE_BG_SM_FG_RUN: ABUIStateMachineRun(); break;
		case ABUI_STATE_BG_SENSOR_FORCE: ABUIMenu_Sensor_Force(); break;
		case ABUI_STATE_BG_SENSOR_PRESSURE: ABUIMenu_Sensor_Pressure(); break;
		case ABUI_STATE_BG_SENSOR_TEMPERATURE: ABUIMenu_Sensor_Temperature(); break;
		case ABUI_STATE_BG_SENSOR_VELOCITY: ABUIMenu_Sensor_Velocity(); break;
		case ABUI_STATE_BG_SENSOR_HUMIDITY: ABUIMenu_Sensor_Humidity(); break;
		case ABUI_STATE_BG_SENSOR_DIRECTION: ABUIMenu_Sensor_Direction(); break;
		case ABUI_STATE_BG_SENSOR_MPSA: ABUIMenu_Sensor_MPSA(); break;
		case ABUI_STATE_BG_SENSOR_PSPEED: ABUIMenu_Sensor_PSpeed(); break;
		case ABUI_STATE_BG_EXP_SETUP_TIME: ABUIMenu_Experiment_SetupTime(); break;
		case ABUI_STATE_BG_EXP_SETUP_FREQUENCY: ABUIMenu_Experiment_SetupFrequency(); break;
		case ABUI_STATE_BG_EXP_SETUP_WINDSPEED: ABUIMenu_Experiment_SetupWindSpeed(); break;
		case ABUI_STATE_BG_EXP_CALIBRATE_NOOBJECT: ABUIMenu_Experiment_CalibrationNoObject(); break;
		case ABUI_STATE_BG_EXP_CALIBRATE_NOOBJECT_WIND: ABUIMenu_Experiment_CalibrationNoObjectWind(); break;
		case ABUI_STATE_BG_EXP_CALIBRATE_OBJECT: ABUIMenu_Experiment_CalibrationObject(); break;
		case ABUI_STATE_BG_EXP_MEASURE_OBJECT_WIND: ABUIMenu_Experiment_Measure(); break;
		case ABUI_STATE_BG_EXP_RESULTS: ABUIMenu_Experiment_ShowResults(); break;
		case ABUI_STATE_BG_CTL_FAN: ABUIMenu_Control_Motor(); break;
		case ABUI_STATE_BG_CTL_WIND_SPEED: ABSetMotorWindSpeed(); break;
		case ABUI_STATE_BG_CTL_SET_TEST_WINDSPEED: ABUIMenu_Experiment_SetupWindSpeed(); break;
		case ABUI_STATE_BG_EXP_WIND_SPEED: ABUIExperiment_Control_SetWindSpeed(); break;
		case ABUI_STATE_NONE:
			if(bluetoothGetSettingFanStatus()){
				//Disable the bluetooth
				bluetoothDisable();

				//Set the fan status of the bluetooth.
				bluetoothSetSettingFanStatus(0);
				lcdSerialClear();

				//Reset Wind Speed
				motorAtvSpeedReset();
				motorAtvTurnOn();
				ABUIBluetoothSettingWindSpeed = 1;
				anemometerStart();
				ABUIIntVariable02 = 0;
				lcdSerialClear();
				ABUIEventCounter = motorAtvGetTargetSpeed();
				expSetup.windSpeed = motorAtvGetTargetSpeed();
				//Set the bluetooth
				ABUIStateMachineBackgroundSetNextState(ABUI_STATE_BG_CTL_WIND_SPEED);
			}
			else{
				bluetoothEnable();
			}
			break;
		default:
			lcdSerialClear();
			lcdSerialWriteString("Unknown Background State.");
			break;
	}
}


/////////////////////////////////
// API Layer 1

/*
 *Writes the LoadBar of the AeroBal Initialize interface.
 */
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

/*
 * Writes the first output to the UI on micro
 * power on.
 */
void ABUIMenu_Load_Write(){
	ABUILCDWrite(" ---  AeroBal  ---  ",
				 "====================",
				 "Loading System...",
				 "");

}

/*
 * Returns the size of the Main Menu options.
 */
int ABUIMenu_Main_Size(){
	return ABUIMenu_Main_OptionsSize;
}

/*
 * Writes the Main Menu on screen. uses the last bit
 * of the state machine identifiers as a selector for options.
 */
void ABUIMenu_Main_Write(int option){

	char* ABUIMenu_Main_Options[] = {
		"Begin Experiment", "View Sensors", "View Controls", "Last Experiment", "System Test"
	};
	ABUIWriteMenu("Principal Menu:",ABUIMenu_Main_Options,ABUIMenu_Main_OptionsSize,option);
}

/*
 * Returns the size of the Sensor Options.
 */
int ABUIMenu_Sensor_Size(){
	return ABUIMenu_Sensor_OptionsSize;
}

/*
 * Writes the Sensor Menu on screen; uses the last bit
 * of the state machine identifiers as a selector for options.
 */
void ABUIMenu_Sensor_Write(int option){
	char *ABUIMenu_Sensor_Options[] = {
		"Force", "Pressure", "Temperature",
		"Velocity","Humidity","Direction",
		"Pressure Array", "Pressure Speed"
	};
	ABUIWriteMenu("View Sensors:",ABUIMenu_Sensor_Options,ABUIMenu_Sensor_OptionsSize,option);
}

/*
 * Returns the size of the Control options.
 */
int ABUIMenu_Control_Size(){
	return ABUIMenu_Control_OptionsSize;
}

/*
 * Wirtes the Control Menu on screen; uses the last bit
 * of the state machine identifiers as a selector for options.
 */
void ABUIMenu_Control_Write(int option){
	char *ABUIMenu_Control_Options[] = {
		"Motor", "Speed Algorithm", "Set Wind Speed"
	};
	ABUIWriteMenu("View Controls:",ABUIMenu_Control_Options,ABUIMenu_Control_OptionsSize,option);
}

/*
 * Stub function for writing a skeleton background process or routine.
 */
void ABUIMenu_Sensor_Stub(char * sensor){
	lcdSerialCursorLine1();
	lcdSerialWriteString(sensor);
	lcdSerialWriteString(" Routine:");
	lcdSerialCursorLine2();
	lcdSerialWriteNumber(ABUIEventCounter++);
}

/*
 * Force Sensor Routine.
 * Refreshes the values from the load cells and
 * then writes each value on screen.
 */
void ABUIMenu_Sensor_Force(){
	buttonsMask();
	ABSSRefreshLoadCells();
	if(ABUIEventCounter != 0){
		if(ABUIEventCounter > 0){
			ABUIRefDragFront = ABSSGetLoadCellDragFront();
			ABUIRefDragBack = ABSSGetLoadCellDragBack();
			ABUIRefLiftUp = ABSSGetLoadCellLiftUp();
			ABUIRefLiftDown = ABSSGetLoadCellLiftDown();
			ABUIRefSideLeft = ABSSGetLoadCellSideLeft();
			ABUIRefSideRight = ABSSGetLoadCellSideRight();
		}
		else{
			ABUIRefDragFront = 0;
			ABUIRefDragBack = 0;
			ABUIRefLiftUp = 0;
			ABUIRefLiftDown = 0;
			ABUIRefSideLeft = 0;
			ABUIRefSideRight = 0;
		}
		ABUIEventCounter=0;
	}
	lcdSerialCursorLine1();
	lcdSerialWriteString("[Up/Down] Set/Clr Ref.");
	lcdSerialCursorLine2();
	lcdSerialWriteString("DF:");
	lcdSerialWriteNumberWithBounds(ABSSGetLoadCellDragFront()-ABUIRefDragFront,0,1);
	lcdSerialWriteString(" ");
	lcdSerialWriteString("DB:");
	lcdSerialWriteNumberWithBounds(ABSSGetLoadCellDragBack()-ABUIRefDragBack,0,1);
	lcdSerialWriteString(" ");
	lcdSerialCursorLine3();
	lcdSerialWriteString("LU:");
	lcdSerialWriteNumberWithBounds(ABSSGetLoadCellLiftUp()-ABUIRefLiftUp,0,1);
	lcdSerialWriteString(" ");
	lcdSerialWriteString("LD:");
	lcdSerialWriteNumberWithBounds(ABSSGetLoadCellLiftDown()-ABUIRefLiftDown,0,1);
	lcdSerialWriteString(" ");
	lcdSerialCursorLine4();
	lcdSerialWriteString("SL:");
	lcdSerialWriteNumberWithBounds(ABSSGetLoadCellSideLeft()-ABUIRefSideLeft,0,1);
	lcdSerialWriteString(" ");
	lcdSerialWriteString("SR:");
	lcdSerialWriteNumberWithBounds(ABSSGetLoadCellSideRight()-ABUIRefSideRight,0,1);
	lcdSerialWriteString(" ");
	buttonsUnmask();
}

/*
 * Pressure Sensor Routine.
 * Refreshes the values from the pressure sensors and
 * then writes each value on screen.
 *
 * Uses the BMP Library sensor 0.
 */
void ABUIMenu_Sensor_Pressure(){
	buttonsMask();
	ABSSRefreshBMP();
	lcdSerialCursorLine1();
	lcdSerialWriteString("Pressure Sensor:");
	lcdSerialCursorLine2();
	lcdSerialWriteString("Pa: ");
	lcdSerialWriteNumberWithBounds(ABSSGetBMPPressure(),0,5);
	lcdSerialWriteString("  ");
	lcdSerialCursorLine4();
	lcdSerialWriteString("[Cancel]: Back");
	buttonsUnmask();

}

/*
 * Temperature Sensor Routine.
 * Refreshes the values from the temperature sensor and
 * then writes each value on screen.
 *
 * Uses the DHT22 temperature.
 */
void ABUIMenu_Sensor_Temperature(){
	buttonsMask();
	ABSSRefreshDHT();
	lcdSerialCursorLine1();
	lcdSerialWriteString("Temperature Sensor:");
	lcdSerialCursorLine2();
	lcdSerialWriteString("C: ");
	lcdSerialWriteNumberWithBounds(ABSSGetBMPTemperature(),0,5);
	lcdSerialWriteString("  ");
	lcdSerialCursorLine4();
	lcdSerialWriteString("[Cancel]: Back");
	buttonsUnmask();
}

/*
 * Anemometer Sensor Routine.
 * Refreshes the values from the anemometer sensor and
 * then writes each value on screen.
 *
 * Uses the Anemometer Library.
 */
void ABUIMenu_Sensor_Velocity(){
	buttonsMask();
	ABSSRefreshAnemometer();
	lcdSerialCursorLine1();
	lcdSerialWriteString("Anemometer Sensor:");
	lcdSerialCursorLine2();
	lcdSerialWriteString("KM/H: ");
	lcdSerialWriteNumberWithBounds(anemometerSpeedConvertKmH(ABSSGetAnemometerSpeed()),0,5);
	lcdSerialCursorLine3();
	lcdSerialWriteString("MI/H: ");
	lcdSerialWriteNumberWithBounds(anemometerSpeedConvertMiH(anemometerSpeedBufferGetAverage()),0,5);
	lcdSerialWriteString("  ");
	lcdSerialCursorLine4();
	lcdSerialWriteString("[Cancel]: Back");
	buttonsUnmask();
}

/*
 * Humidity Sensor Routine.
 * Refreshes the values from the humidity sensor and
 * then writes each value on screen.
 *
 * Uses the DHT22 library.
 */
void ABUIMenu_Sensor_Humidity(){
	buttonsMask();
	ABUIEventCounter = (ABUIEventCounter + 1) % 10;
	ABSSRefreshDHT();
	lcdSerialCursorLine1();
	lcdSerialWriteString("Humidity Sensor: [");
	lcdSerialWriteNumber(ABUIEventCounter);
	lcdSerialWriteString("]");
	lcdSerialCursorLine2();
	lcdSerialWriteString("Humd. %: ");
	lcdSerialWriteNumber(ABSSGetDHTHumidity());
	lcdSerialCursorLine3();
	lcdSerialWriteString("Temp. C: ");
	lcdSerialWriteNumber(ABSSGetDHTTemperature());
	lcdSerialWriteString("  ");
	lcdSerialCursorLine4();
	lcdSerialWriteString("[Cancel]: Back");
	buttonsUnmask();
}

/*
 * Direction Sensor Routine.
 * Refreshes the values from the direction sensor and
 * then writes each value on screen.
 *
 * Uses the Wind Vane library.
 */
void ABUIMenu_Sensor_Direction(){
	buttonsMask();
	ABSSRefreshWindVane();
	lcdSerialCursorLine1();
	lcdSerialWriteString("Wind Vane Sensor:");
	lcdSerialCursorLine2();
	lcdSerialWriteString("Angle: ");
	lcdSerialWriteNumber(ABSSGetWindVaneDirection());
	lcdSerialCursorLine4();
	lcdSerialWriteString("[Cancel]: Back");
	buttonsUnmask();

}

/*
 * MPSA Sensor Routine.
 * Refreshes the values from the pressure sensors and
 * then writes each value on screen on the selected index.
 *
 * Uses the MPSA (bmp085Array) library.
 *
 */
void ABUIMenu_Sensor_MPSA(){
	buttonsMask();
	if(ABUIEventCounter > 13 || ABUIEventCounter < 0) ABUIEventCounter = 0;
	ABSSRefreshMPSAIndex(ABUIEventCounter);
	lcdSerialCursorLine1();
	lcdSerialWriteString("MPSA Sensor: ");
	lcdSerialWriteNumber(ABUIEventCounter);
	lcdSerialWriteString("  ");
	lcdSerialCursorLine2();
	lcdSerialWriteString("T");
	lcdSerialWriteNumberWithBounds(ABUIEventCounter,2,0);
	lcdSerialWriteString(": ");
	lcdSerialWriteNumberWithBounds(ABSSGetMPSAIndexTemperature(ABUIEventCounter),0,3);
	lcdSerialWriteString("  ");
	lcdSerialCursorLine3();
	lcdSerialWriteString("P");
	lcdSerialWriteNumberWithBounds(ABUIEventCounter,2,0);
	lcdSerialWriteString(": ");
	lcdSerialWriteNumberWithBounds(ABSSGetMPSAIndexPressure(ABUIEventCounter),0,3);
	lcdSerialWriteString("  ");
	ABSSGetMPSAIndexPressure(ABUIEventCounter);
	ABSSGetMPSAIndexTemperature(ABUIEventCounter);
	buttonsUnmask();

}

/*
 * Temperature Sensor Routine.
 * Refreshes the values from the temperature sensor and
 * then writes each value on screen.
 *
 * Uses the DHT22 temperature.
 */
void ABUIMenu_Sensor_PSpeed(){
	buttonsMask();
	ABSSRefreshDHT();
	bmp085ArrayDataReadPosition(0);
	float pStatic = bmp085ArrayGetPressure();
	bmp085ArrayDataReadPosition(1);
	float pDynamic = bmp085ArrayGetPressure();
	float temperature = dht22GetTemperature();
	float humidity = dht22GetHumidity();
	lcdSerialCursorLine1();
	lcdSerialWriteString("PS:");
	lcdSerialWriteNumber(pStatic);
	lcdSerialCursorLine2();
	lcdSerialWriteString("PD: ");
	lcdSerialWriteNumber(pDynamic);
	lcdSerialWriteString("  ");
	lcdSerialCursorLine3();
	lcdSerialWriteString("T: ");
	lcdSerialWriteNumberWithBounds(temperature,0,2);
	lcdSerialWriteString(" H: ");
	lcdSerialWriteNumberWithBounds(humidity,0,2);
	lcdSerialCursorLine4();
	lcdSerialWriteString("Result: ");

	//////////////////

	float T = temperature;
	float RH = humidity;
	float P0 = pStatic;
	float adR = 8.314; // Universal Gas Constant - J/(K*mol)
	float adMv = 0.018016; //Molar Mass of water vapor - kg/mol.
	float adMd = 0.028964; //Molar Mass of Dry air - kg/mol
	//float adRv = 461.465; //Specific gas constant for water vapor - J/(kg*K)
	//float adRd = 287.058; //Specific gas constant for dry air - J/(kg*K)

	//Need to calculate pd and pv
	//pv = relative humidity * psat
	float adPsat = 6.1078 * pow(10.0,(7.5*T)/(T+237.3));
	float adPv = (RH/100.0 * adPsat)*100.0; //Pa - scaling

	//PD
	float adPd = P0-adPv;

	float rhoAir = ((adPd*adMd)+(adPv*adMv))/(adR*(T+273.0));

	float result = sqrt(2.0*(abs(pDynamic-pStatic))/rhoAir)/0.44704;

	///////////////////

	//lcdSerialWriteNumber(bmp085GetPressureVelocity(pStatic,pDynamic,temperature,humidity));
	lcdSerialWriteNumber(result);
	buttonsUnmask();
}

/////////////////////////////////////
// Control

/*
 * Motor Control Routine.
 * Allows control of the VFD by providing an increase
 * decrease interface for the motor.
 */
void ABUIMenu_Control_Motor(){
	buttonsMask();
	ABSSRefreshAnemometer();
	int step = 0;
	if(ABUIEventCounter > 0){
		ABUIEventCounter = 0;
		step = 1;
	}
	else if(ABUIEventCounter < 0){
		ABUIEventCounter = 0;
		step = -1;
	}
	lcdSerialCursorLine1();
	lcdSerialWriteString("Motor Control: ");
	lcdSerialCursorLine2();
	lcdSerialWriteString("MI/H: ");
	lcdSerialWriteNumberWithBounds(anemometerSpeedConvertMiH(ABSSGetAnemometerSpeed()),0,5);
	lcdSerialCursorLine3();
	lcdSerialWriteString("[Up/Down] Change.");
	lcdSerialCursorLine4();
	lcdSerialWriteString("[Cancel]: Back.");
	if(step < 0){
		motorAtvSpeedDec();
	}
	if(step > 0){
		motorAtvSpeedInc();
	}
	step = 0;
	buttonsUnmask();
}

/*
 *	Motor Wind Speed Algorithm Routine
 *	The algorithm simply uses characterization to set
 *	the wind speed due to resolution issues with the
 *	potentiometer.
 *
 */
void ABSetMotorWindSpeed(){
	buttonsMask();
	if(ABUICounter01 < 0 || ABUICounter01 > 15) ABUICounter01 = 0;
	ABUICounter01++;
	ABSSRefreshAnemometer();
	float currentWindSpeed = anemometerSpeedConvertMiH(anemometerSpeedBufferGetAverage());
	float targetWindSpeed = motorAtvGetTargetSpeed();

	//Speed Buttons Value
	if(ABUIEventCounter < 1 || ABUIEventCounter > 50)
		ABUIEventCounter = 1;

	//HWUI Sequence.
	if(ABUIEventCounter != motorAtvGetTargetSpeed() && !ABUIBluetoothSettingWindSpeed){
		motorAtvSetTargetSpeed(ABUIEventCounter);
		ABUIIntVariable01 = 1;
	}

	//Bluetooth's Sequence
	if(ABUIBluetoothSettingWindSpeed){
		ABUIIntVariable01 = -1;
		if(!ABUIIntVariable02){
			motorAtvSetTargetSpeed(expSetup.windSpeed);
			motorAtvSetSpeedCharacterized();
			ABUIIntVariable02 = 1;
		}
		if((currentWindSpeed > targetWindSpeed*(1-0.03)
				&& currentWindSpeed < targetWindSpeed*(1+0.03)) && currentWindSpeed > 0.5 ){
			ABUIBluetoothSettingWindSpeed = 0;
			ABUIStateMachineNextState = ABUI_STATE_MAIN_1;
			ABUIStateMachineBackgroundSetNextState(ABUI_STATE_NONE);
			SysCtlDelay(1000000);
			bluetoothSendAck();
			bluetoothEnable();
			ABUIStateMachineRun();
			buttonsUnmask();
			return ;
		}
	}



	lcdSerialCursorLine1();
	if(!ABUIBluetoothSettingWindSpeed){
		lcdSerialWriteString("Setting Wind Speed:");
	}
	else{
		lcdSerialWriteString("Bluetooth: WindSpeed:");
	}
	lcdSerialCursorLine2();
	lcdSerialWriteString("Current: ");
	lcdSerialWriteNumber(currentWindSpeed);
	lcdSerialWriteString("  ");
	lcdSerialCursorLine3();
	lcdSerialWriteString("Target:");
	lcdSerialWriteNumber(ABUIEventCounter);
	lcdSerialWriteString(" ");
	//For Debug.
	//lcdSerialWriteString("Stp: ");
	//lcdSerialWriteNumber(motorAtvGetStep(ABUIEventCounter));
	//lcdSerialWriteString(" ");
	lcdSerialCursorLine4();
	ABUIWriteWait(ABUICounter01);

	if(ABUIIntVariable01 != -1){
		ABUIIntVariable01 = -1;
		motorAtvSpeedReset();
		motorAtvSetSpeedCharacterized();
		lcdSerialClear();
	}

	if(!ABUIBluetoothSettingWindSpeed){
		buttonsUnmask();
	}

}

/*
 * Provides a quick way to set the desired speed
 * by characterization. This background process
 * waits for the speed to reach its place by delaying
 * for worst case set seconds.
 */
void ABUIExperiment_Control_SetWindSpeed(){
	buttonsMask();
	lcdSerialCursorLine1();
	lcdSerialWriteString("Setting Wind Speed...");
	lcdSerialCursorLine2();
	lcdSerialWriteString("Waiting ");
	lcdSerialWriteNumber(ABUI_DEFAULTS_CONTROL_WINDSPEED_WAIT);
	lcdSerialWriteString(" Seconds...");
	if(ABTimeGetDelta(ABTimeGetReference(),ABUIT0).seconds > ABUI_DEFAULTS_CONTROL_WINDSPEED_WAIT){
		ABUIT0 = ABTimeGetReference();
		ABUIF0 = ABTimeGetReference();
		ABUIStateMachineBackgroundSetNextState(ABUIBackgroundStateSlot1);
	}
	else{
		lcdSerialCursorLine3();
		lcdSerialWriteString("Time: ");
		lcdSerialWriteNumber(ABTimeGetDelta(ABTimeGetReference(),ABUIT0).seconds);
		lcdSerialWriteString("/");
		lcdSerialWriteNumber(ABUI_DEFAULTS_CONTROL_WINDSPEED_WAIT);
	}
}

/////////////////////////////////////
// Experiment Handlers:
////////////////////////
// Experiment Sequence is handled by functions below.

/*
 * Ask the user for the experiment time length.
 */
void ABUIMenu_Experiment_SetupTime(){
	buttonsMask();
	if(ABUIEventCounter<ABUI_DEFAULTS_TIME_MIN){
		ABUIEventCounter = ABUI_DEFAULTS_TIME_MIN;
	}
	if(ABUIEventCounter>ABUI_DEFAULTS_TIME_MAX){
		ABUIEventCounter = ABUI_DEFAULTS_TIME_MAX;
	}
	lcdSerialCursorLine1();
	lcdSerialWriteString("Set Time Length:");
	lcdSerialCursorLine2();
	lcdSerialWriteString("Seconds: ");
	lcdSerialWriteNumber(ABUIEventCounter);
	lcdSerialWriteString("  ");
	expSetup.timeSeconds = ABUIEventCounter;
	lcdSerialCursorLine3();
	lcdSerialWriteString("[Up/Down]: Change");
	lcdSerialCursorLine4();
	lcdSerialWriteString("[Enter]: Continue");
	buttonsUnmask();
}

/*
 * Ask for the experiment frequency of that lecture
 * for buffers.
 */
void ABUIMenu_Experiment_SetupFrequency(){
	buttonsMask();
	if(ABUIEventCounter<ABUI_DEFAULTS_FREQUENCY_MIN){
		ABUIEventCounter = ABUI_DEFAULTS_FREQUENCY_MIN;
	}
	if(ABUIEventCounter>ABUI_DEFAULTS_FREQUENCY_MAX){
		ABUIEventCounter = ABUI_DEFAULTS_FREQUENCY_MAX;
	}
	lcdSerialCursorLine1();
	lcdSerialWriteString("Set Data Frequency:");
	lcdSerialCursorLine2();
	lcdSerialWriteString("Hz: ");
	lcdSerialWriteNumber(ABUIEventCounter);
	lcdSerialWriteString("  ");
	expSetup.frequencyHz = ABUIEventCounter;
	lcdSerialCursorLine3();
	lcdSerialWriteString("[Up/Down]: Change");
	lcdSerialCursorLine4();
	lcdSerialWriteString("[Enter]: Continue");
	buttonsUnmask();
}

/*
 * Ask for the wind speed of the experiment.
 */
void ABUIMenu_Experiment_SetupWindSpeed(){
	buttonsMask();
	if(ABUIEventCounter<ABUI_DEFAULTS_WINDSPEED_MIN){
		ABUIEventCounter = ABUI_DEFAULTS_WINDSPEED_MIN;
	}
	if(ABUIEventCounter>ABUI_DEFAULTS_WINDSPEED_MAX){
		ABUIEventCounter = ABUI_DEFAULTS_WINDSPEED_MAX;
	}
	lcdSerialCursorLine1();
	lcdSerialWriteString("Set Wind Speed:");
	lcdSerialCursorLine2();
	lcdSerialWriteString("Mi/h: ");
	lcdSerialWriteNumber(ABUIEventCounter);
	lcdSerialWriteString("  ");
	expSetup.windSpeed=ABUIEventCounter;
	motorAtvSetTargetSpeed(expSetup.windSpeed);
	lcdSerialCursorLine3();
	lcdSerialWriteString("[Up/Down]: Change");
	lcdSerialCursorLine4();
	lcdSerialWriteString("[Enter]: Continue");
	buttonsUnmask();
}

/*
 * Instruct the user to remove any object from the tunnel and
 * balance.
 */
void ABUIMenu_Experiment_CalibrationNoObjectConfirm(){
	buttonsMask();
	ABECClearAllBuffers();
	ABUILCDWrite("Base Calibration:",
			"Please remove",
			"object.",
			"[Enter]: Continue");
	buttonsUnmask();
}

/*
 * Take initial measurements of the base weight.
 * This sets a reference frame for the base drag,
 * lift and tilt.
 */
void ABUIMenu_Experiment_CalibrationNoObject(){
	buttonsMask();
	ABUILCDWrite("Balance Calibration:",
			"Setting force",
			"reference frame.",
			"");
	ABSSRefreshLoadCells();
	ABTime t1 = ABTimeGetReference();
	ABTime f1 = ABTimeGetReference();
	ABUIWriteWaitTimed(ABTimeGetDelta(t1,ABUIT0).seconds,expSetup.timeSeconds,"s");

	if(ABTimeGetDelta(ABTimeGetReference(),ABUIF0).milliseconds+
			ABTimeGetDelta(ABTimeGetReference(),ABUIF0).seconds*1000
			> (1.0/((float)expSetup.frequencyHz))*1000.0){
		ABUIF0 = ABTimeGetReference();
		ABECAddForces();
	}

	if(ABTimeGetDelta(ABTimeGetReference(),ABUIT0).seconds > expSetup.timeSeconds){
		ABECStoreNONW();
		ABECClearForceBuffer();
		ABUIStateMachineNextState = ABUI_STATE_EXP_6_CALIBRATE_NOOBJECT_WIND_CONFIRM;
		ABUIBackgroundNextState = ABUI_STATE_NONE;
		buttonsUnmask();
		ABUIStateMachineRun();
	}

}

/*
 * Instruct the user to close the tunnel for wind speed setup.
 */
void ABUIMenu_Experiment_CalibrationNoObjectWindConfirm(){
	buttonsMask();
	ABUILCDWrite("Base Calibration",
			"Wind: Close tunnel",
			"without object.",
			"[Enter]: Continuar");
	buttonsUnmask();
}

/*
 * Take the measurements of the forces on wind.
 */
void ABUIMenu_Experiment_CalibrationNoObjectWind(){
	buttonsMask();
	ABUILCDWrite("Base Calibration:   ",
				"Measuring base      ",
				"aerodynamic forces. ",
				"");
	ABSSRefreshLoadCells();
	ABTime t1 = ABTimeGetReference();
	ABTime f1 = ABTimeGetReference();
	ABUIWriteWaitTimed(ABTimeGetDelta(t1,ABUIT0).seconds,expSetup.timeSeconds,"s");

	if(ABTimeGetDelta(ABTimeGetReference(),ABUIF0).milliseconds+
			ABTimeGetDelta(ABTimeGetReference(),ABUIF0).seconds*1000
			> (1.0/((float)expSetup.frequencyHz))*1000.0){
		ABUIF0 = ABTimeGetReference();
		ABECAddForces();
	}

	if(ABTimeGetDelta(ABTimeGetReference(),ABUIT0).seconds > expSetup.timeSeconds){
		ABECStoreNO_W();
		ABECClearForceBuffer();
		ABUIStateMachineNextState = ABUI_STATE_EXP_8_CALIBRATE_OBJECT_CONFIRM;
		ABUIBackgroundNextState = ABUI_STATE_NONE;
		buttonsUnmask();
		motorAtvTurnOff();
		ABUIStateMachineRun();
	}
}

/*
 * Instruct the user to set the object in the base.
 */
void ABUIMenu_Experiment_CalibrationObjectConfirm(){
	buttonsMask();
	ABUILCDWrite("Object Calibration:",
			"Set object in base.",
			"",
			"[Enter]: Continue");
	motorAtvTurnOff();
	buttonsUnmask();
}

/*
 * Take object+base reference frame measurements.
 */
void ABUIMenu_Experiment_CalibrationObject(){
	buttonsMask();
	ABUILCDWrite("Calibrating Object",
			"with no wind, please",
			"wait.               ",
			"");
	ABSSRefreshLoadCells();
	ABTime t1 = ABTimeGetReference();
	ABTime f1 = ABTimeGetReference();
	ABUIWriteWaitTimed(ABTimeGetDelta(t1,ABUIT0).seconds,expSetup.timeSeconds,"s");

	if(ABTimeGetDelta(ABTimeGetReference(),ABUIF0).milliseconds+
			ABTimeGetDelta(ABTimeGetReference(),ABUIF0).seconds*1000
			> (1.0/((float)expSetup.frequencyHz))*1000.0){
		ABUIF0 = ABTimeGetReference();
		ABECAddForces();
	}

	if(ABTimeGetDelta(ABTimeGetReference(),ABUIT0).seconds > expSetup.timeSeconds){
		ABECStoreO_NW();
		ABECClearForceBuffer();
		ABUIStateMachineNextState = ABUI_STATE_EXP_10_OBJECT_WIND_CONFIRM;
		ABUIBackgroundNextState = ABUI_STATE_NONE;
		buttonsUnmask();
		ABUIStateMachineRun();
	}

}

/*
 * Instruct the user to close the tunnel.
 */
void ABUIMenu_Experiment_MeasureConfirm(){
	buttonsMask();
	ABUILCDWrite("Confirm Start:",
			"Please close tunnel",
			"to begin.",
			"[Enter]: Continue");
	buttonsUnmask();
}

/*
 * Take the experiment measurements. In this phase
 * all measurements are taken.
 */
void ABUIMenu_Experiment_Measure(){
	buttonsMask();
	ABUILCDWrite("AeroBal:",
			"Reading Sensors...",
			"Please Wait...",
			"");

	ABSSRefreshLoadCells();
	ABSSRefreshDHT();
	ABSSRefreshBMP();
	ABSSRefreshWindVane();
	ABSSRefreshAnemometer();
	ABSSRefreshLoadCells();

	ABTime t1 = ABTimeGetReference();
	ABTime f1 = ABTimeGetReference();
	ABUIWriteWaitTimed(ABTimeGetDelta(t1,ABUIT0).seconds,expSetup.timeSeconds,"s");

	if(ABTimeGetDelta(ABTimeGetReference(),ABUIF0).milliseconds+
			ABTimeGetDelta(ABTimeGetReference(),ABUIF0).seconds*1000
			> (1.0/((float)expSetup.frequencyHz))*1000.0){
		ABUIF0 = ABTimeGetReference();
		ABECAddForces();
		ABECAddHumidity();
		ABECAddPressure();
		ABECAddTemperature();
		ABECAddVelocity();
		ABECAddWindDirection();
	}

	if(ABTimeGetDelta(ABTimeGetReference(),ABUIT0).seconds > expSetup.timeSeconds){
		ABUIStateMachineNextState = ABUI_STATE_EXP_12_DISPLAY_RESULTS;
		ABUIBackgroundNextState = ABUI_STATE_NONE;
		motorAtvTurnOff();
		buttonsUnmask();
		ABUIStateMachineRun();
	}

}

/*
 * Show the results stored in the last experiment.
 */
void ABUIMenu_Experiment_ShowResults(){
	buttonsMask();
	if(ABUIEventCounter < 0 || ABUIEventCounter > 4){
		ABUIEventCounter = 0;
	}
	if(ABUIIntVariable01 != ABUIEventCounter) lcdSerialClear();
	ABUIIntVariable01 = ABUIEventCounter;
	switch(ABUIEventCounter){
	case 0:
		lcdSerialCursorLine1();
		lcdSerialWriteString("Average Results: 1/5");
		lcdSerialCursorLine2();
		lcdSerialWriteString("Drag:");
		lcdSerialWriteNumberWithBounds(ABECComputeObjectDrag(),0,3);
		lcdSerialWriteString(" LB");
		lcdSerialCursorLine3();
		lcdSerialWriteString("Lift: ");
		lcdSerialWriteNumberWithBounds(ABECComputeObjectLift(),0,3);
		lcdSerialWriteString(" LB");
		lcdSerialCursorLine4();
		lcdSerialWriteString("[Up/Down] Next/Back");
		break;
	case 1:
		lcdSerialCursorLine1();
		lcdSerialWriteString("Average Results: 2/5");
		lcdSerialCursorLine2();
		lcdSerialWriteString("Side: ");
		lcdSerialWriteNumberWithBounds(ABECComputeObjectSide(),0,3);
		lcdSerialWriteString(" LB");
		lcdSerialCursorLine3();
		lcdSerialWriteString("Pres: ");
		lcdSerialWriteNumberWithBounds(ABECGetAveragePressure()/1000.0,0,3);
		lcdSerialWriteString(" kPa");
		lcdSerialCursorLine4();
		lcdSerialWriteString("[Up/Down] Next/Back");
		break;
	case 2:
		lcdSerialCursorLine1();
		lcdSerialWriteString("Average Results: 3/5");
		lcdSerialCursorLine2();
		lcdSerialWriteString("Temp: ");
		lcdSerialWriteNumberWithBounds(ABECGetAverageTemperature(),0,3);
		lcdSerialWriteString(" ");
		lcdSerialWrite(223); //Angle Sign
		lcdSerialWriteString("C");
		lcdSerialCursorLine3();
		lcdSerialWriteString("Humd: ");
		lcdSerialWriteNumberWithBounds(ABECGetAverageHumidity(),0,3);
		lcdSerialWriteString(" %");
		lcdSerialCursorLine4();
		lcdSerialWriteString("[Up/Down] Next/Back");
		break;
	case 3:
		lcdSerialCursorLine1();
		lcdSerialWriteString("Average Results: 4/5");
		lcdSerialCursorLine2();
		lcdSerialWriteString("Vlty: ");
		lcdSerialWriteNumberWithBounds(ABECGetAverageVelocity(),0,3);
		lcdSerialWriteString(" mi/h");
		lcdSerialCursorLine3();
		lcdSerialWriteString("Dire: ");
		lcdSerialWriteNumberWithBounds(ABECGetAverageWindDirection(),0,3);
		lcdSerialWriteString(" ");
		lcdSerialWrite(223); //Angle Sign.
		lcdSerialCursorLine4();
		lcdSerialWriteString("[Up/Down] Next/Back");
		break;
	case 4:
		lcdSerialCursorLine1();
		lcdSerialWriteString("Average Results: 5/5");
		lcdSerialCursorLine2();
		lcdSerialWriteString("====================");
		lcdSerialCursorLine3();
		lcdSerialWriteString("[Enter/Menu] Menu.");
		lcdSerialCursorLine4();
		lcdSerialWriteString("[Up/Down] Next/Back");
		break;
	default:
		break;

	}
	buttonsUnmask();
}
