/*
 * ABUI.h
 *
 *  Created on: Apr 6, 2014
 *      Author: Administrator
 */

#ifndef ABUI_H_
#define ABUI_H_

#include "lcdSerial.h"
#include "buttons.h"
#include "string.h"
#include "windVane.h"
#include "anemometer.h"
#include "dht11.h"
#include "bmp085.h"
#include "bluetooth.h"
#include "bmp085Array.h"
#include "loadCell.h"
#include "ABSensorServer.h"
#include "bmp085Array.h"
#include "ABTime.h"
#include "relay.h"
#include "motorAtv.h"
#include "ABExperimentCentral.h"

#define ABUI_LCD_UARTPORT0 LCDSERIAL_INIT_UART0
#define ABUI_LCD_UARTPORT1 LCDSERIAL_INIT_UART1
#define ABUI_LCD_UARTPORT2 LCDSERIAL_INIT_UART2
#define ABUI_LCD_UARTPORT3 LCDSERIAL_INIT_UART3
#define ABUI_LCD_UARTPORT4 LCDSERIAL_INIT_UART4
#define ABUI_LCD_UARTPORT5 LCDSERIAL_INIT_UART5
#define ABUI_LCD_UARTPORT6 LCDSERIAL_INIT_UART6
#define ABUI_LCD_UARTPORT7 LCDSERIAL_INIT_UART7

#define ABUI_MENU_WELCOME 0x00
#define ABUI_MENU_MAIN 0x01
#define ABUI_MENU_START 0x02
#define ABUI_MENU_SETUP 0x03
#define ABUI_MENU_CALIBRATE 0x04

#define ABUI_STATE_MAIN_1 0x0000
#define ABUI_STATE_MAIN_2 0x0001
#define ABUI_STATE_MAIN_3 0x0002
#define ABUI_STATE_MAIN_4 0x0003
#define ABUI_STATE_SENSOR_1 0x0010
#define ABUI_STATE_SENSOR_2 0x0011
#define ABUI_STATE_SENSOR_3 0x0012
#define ABUI_STATE_SENSOR_4 0x0013
#define ABUI_STATE_SENSOR_5 0x0014
#define ABUI_STATE_SENSOR_6 0x0015
#define ABUI_STATE_SENSOR_7 0x0016
#define ABUI_STATE_BG_SENSOR_FORCE 0x0020
#define ABUI_STATE_BG_SENSOR_PRESSURE 0x0021
#define ABUI_STATE_BG_SENSOR_TEMPERATURE 0x0022
#define ABUI_STATE_BG_SENSOR_VELOCITY 0x0023
#define ABUI_STATE_BG_SENSOR_HUMIDITY 0x0024
#define ABUI_STATE_BG_SENSOR_DIRECTION 0x0025
#define ABUI_STATE_BG_SENSOR_MPSA 0x0026
#define ABUI_STATE_EXP_1_SETUP_TIME 0x0100
#define ABUI_STATE_EXP_2_SETUP_FREQUENCY 0x0110
#define ABUI_STATE_EXP_3_SETUP_WIND_SPEED 0x0120
#define ABUI_STATE_EXP_4_CALIBRATE_NOOBJECT_CONFIRM 0x0130
#define ABUI_STATE_EXP_5_CALIBRATE_NOOBJECT 0x0131
#define ABUI_STATE_EXP_6_CALIBRATE_NOOBJECT_WIND_CONFIRM 0x0140
#define ABUI_STATE_EXP_7_CALIBRATE_NOOBJECT_WIND_MEASURE 0x0141
#define ABUI_STATE_EXP_8_CALIBRATE_OBJECT_CONFIRM 0x0160
#define ABUI_STATE_EXP_9_CALIBRATE_OBJECT_MEASURE 0x0170
#define ABUI_STATE_EXP_10_OBJECT_WIND_CONFIRM 0x0180
#define ABUI_STATE_EXP_11_OBJECT_WIND_MEASURE 0x0190
#define ABUI_STATE_EXP_12_DISPLAY_RESULTS 0x01A0

#define ABUI_STATE_BG_SM_FG_RUN 0x200
#define ABUI_STATE_BG_EXP_SETUP_TIME 0x0210
#define ABUI_STATE_BG_EXP_SETUP_FREQUENCY 0x0220
#define ABUI_STATE_BG_EXP_SETUP_WINDSPEED 0x0230
#define ABUI_STATE_BG_EXP_CALIBRATE_NOOBJECT 0x0240
#define ABUI_STATE_BG_EXP_CALIBRATE_NOOBJECT_WIND 0x0250
#define ABUI_STATE_BG_EXP_CALIBRATE_OBJECT 0x0260
#define ABUI_STATE_BG_EXP_MEASURE_OBJECT_WIND 0x0270
#define ABUI_STATE_BG_EXP_RESULTS 0x0280

#define ABUI_STATE_CONTROL_1 0x0400
#define ABUI_STATE_CONTROL_2 0x0401
#define ABUI_STATE_BG_CTL_FAN 0x0480
#define ABUI_STATE_BG_CTL_WIND_SPEED 0x0481


#define ABUI_STATE_BUTTON_INCREASE 0xFD00
#define ABUI_STATE_BUTTON_DECREASE 0xFD01
#define ABUI_STATE_BTCONTROL 0xFE00
#define ABUI_STATE_PANIC 0xFFFC
#define ABUI_STATE_ERROR 0xFFFD
#define ABUI_STATE_RESET 0xFFFE
#define ABUI_STATE_NONE 0xFFFF

typedef struct {
	int timeSeconds;
	int frequencyHz;
	int windSpeed;
} ABUIExpSetup;

uint32_t ABUIButtonNextStateB0_UP;
uint32_t ABUIButtonNextStateB1_DOWN;
uint32_t ABUIButtonNextStateB2_ENTER;
uint32_t ABUIButtonNextStateB3_CANCEL;
uint32_t ABUIButtonNextStateB4_MENU;
uint32_t ABUIButtonNextStateB5_PANIC;

uint32_t ABUIStateMachineNextState;
uint32_t ABUIStateMachineSlot01;
uint32_t ABUIBackgroundNextState;
uint32_t ABUIBackgroundStateSlot1;
uint32_t ABUIBackgroundStateSlot2;
uint32_t ABUIBackgroundStateSlot3;
uint32_t ABUIBackgroundStateSlot4;

ABTime ABUIT0;
ABTime ABUIT1;

int ABUIMenu_Main_OptionsSize;
int ABUIMenu_Sensor_OptionsSize;
int ABUIMenu_Control_OptionsSize;

int ABUIEventCounter;
int ABUIResetMotor;
int ABUICounter01;
int ABUIIntVariable01;

/////////////////////////////////
// API Layer 0
void ABUIPowerWait(uint32_t waitTime);
void ABUIWriteWait(int step);
void ABUIInit();
void ABUIInitModules();
void ABUIPowerWait(uint32_t waitTime);
void ABUILCDWrite(char * ln1, char* ln2, char* ln3, char* ln4);
void ABUIWriteMenu(char* menuTitle, char* options[], int optionSize, int option);
void ABUIStateMachineRun();
void ABUIStateMachineSetNextState_InterruptHandler();
void ABUIButtonsSetNextState(uint32_t nextStateB0,uint32_t nextStateB1,uint32_t nextStateB2,
							 uint32_t nextStateB3,uint32_t nextStateB4,uint32_t nextStateB5);
void ABUIStateMachineBackgroundSetNextState(uint32_t nextState);
void ABUIStateMachineBackgroundReset();
void ABUIStateMachineBackgroundRun();

/////////////////////////////////
// API Layer 1
void ABUIMenu_LoadBar_Write(int delay, int lines);
void ABUIMenu_Load_Write();
int ABUIMenu_Main_Size();
void ABUIMenu_Main_Write(int option);
int ABUIMenu_Sensor_Size();
void ABUIMenu_Sensor_Write(int option);
int ABUIMenu_Control_Size();
void ABUIMenu_Control_Write(int option);

//Sensors
void ABUIMenu_Sensor_Force();
void ABUIMenu_Sensor_Pressure();
void ABUIMenu_Sensor_Temperature();
void ABUIMenu_Sensor_Velocity();
void ABUIMenu_Sensor_Humidity();
void ABUIMenu_Sensor_Direction();
void ABUIMenu_Sensor_MPSA();

//Control
void ABUIMenu_Control_Motor();
void ABSetMotorWindSpeed();

//Experiment
void ABUIMenu_Experiment_SetupTime();
void ABUIMenu_Experiment_SetupFrequency();
void ABUIMenu_Experiment_SetupWindSpeed();
void ABUIMenu_Experiment_CalibrationNoObjectConfirm();
void ABUIMenu_Experiment_CalibrationNoObject();
void ABUIMenu_Experiment_CalibrationNoObjectWindConfirm();
void ABUIMenu_Experiment_CalibrationNoObjectWind();
//
void ABUIMenu_Experiment_CalibrationObjectConfirm();
void ABUIMenu_Experiment_CalibrationObject();
void ABUIMenu_Experiment_MeasureConfirm();
void ABUIMenu_Experiment_Measure();
void ABUIMenu_Experiment_ShowResults();

#endif /* ABUI_H_ */
