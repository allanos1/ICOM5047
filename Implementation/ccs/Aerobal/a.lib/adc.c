/*
 * adc.h
 *
 * The Analog to Digital Converter Library
 *
 *  Created on: Mar 30, 2014
 *      Author: Juan, Anthony
 */

#include "adc.h"

/*
 * Returns the base of the ADC module given.
 *
 */
uint32_t adcGetBase(uint32_t adc){
	switch(adc){
		case ADC_0: return ADC0_BASE;
		case ADC_1: return ADC1_BASE;
		default: return 0;
	}
}

/*
 * Returns the GPIO port assigned to the ADC
 * pin provided.
 */
uint32_t adcPinGetGPIOPort(uint32_t adc_pin_in){
	switch(adc_pin_in){
		case ADC_PIN_IN00_PE3: return GPIO_PORTE;
		case ADC_PIN_IN01_PE2: return GPIO_PORTE;
		case ADC_PIN_IN02_PE1: return GPIO_PORTE;
		case ADC_PIN_IN03_PE0: return GPIO_PORTE;
		case ADC_PIN_IN04_PD3: return GPIO_PORTD;
		case ADC_PIN_IN05_PD2: return GPIO_PORTD;
		case ADC_PIN_IN06_PD1: return GPIO_PORTD;
		case ADC_PIN_IN07_PD0: return GPIO_PORTD;
		case ADC_PIN_IN08_PE5: return GPIO_PORTE;
		case ADC_PIN_IN09_PE4: return GPIO_PORTE;
		case ADC_PIN_IN10_PB4: return GPIO_PORTB;
		case ADC_PIN_IN11_PB5: return GPIO_PORTB;
		default: return 0;
	}
}

/*
 * Returns the GPIO Port Pin assigned to the ADC
 * Pin provided.
 */
uint32_t adcPinGetGPIOPin(uint32_t adc_pin_in){
	switch(adc_pin_in){
		case ADC_PIN_IN00_PE3: return 0x08;
		case ADC_PIN_IN01_PE2: return 0x04;
		case ADC_PIN_IN02_PE1: return 0x02;
		case ADC_PIN_IN03_PE0: return 0x01;
		case ADC_PIN_IN04_PD3: return 0x08;
		case ADC_PIN_IN05_PD2: return 0x04;
		case ADC_PIN_IN06_PD1: return 0x02;
		case ADC_PIN_IN07_PD0: return 0x01;
		case ADC_PIN_IN08_PE5: return 0x20;
		case ADC_PIN_IN09_PE4: return 0x10;
		case ADC_PIN_IN10_PB4: return 0x10;
		case ADC_PIN_IN11_PB5: return 0x20;
		default: return 0;
	}
}

/*
 * Returns the mux shift of the given mux.
 */
uint32_t adcGetMuxShift(uint32_t mux){
	switch(mux){
	case ADC_MUX_0: return ADC_MUX_0_SHIFT;
	case ADC_MUX_1: return ADC_MUX_1_SHIFT;
	case ADC_MUX_2: return ADC_MUX_2_SHIFT;
	case ADC_MUX_3: return ADC_MUX_3_SHIFT;
	case ADC_MUX_4: return ADC_MUX_4_SHIFT;
	case ADC_MUX_5: return ADC_MUX_5_SHIFT;
	case ADC_MUX_6: return ADC_MUX_6_SHIFT;
	case ADC_MUX_7: return ADC_MUX_7_SHIFT;
	default: return 0;
	}
}
/*
 * Enables an ADC peripheral.
 *
 * adc - the ADC peripheral to enable.
 */
void adcEnable(uint32_t adc){
	switch(adc){
		case ADC_0: SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0); break;
		case ADC_1: SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1); break;
	}
}

/*
 * Configures the sequence to be taken by setting the sample sequence to use to
 * either 8, 4, or 1 samples per measurement, setting the trigger of the
 * measurements to
 *
 */
void adcSequenceConfigure(uint32_t adc){
	ADCSequenceConfigure(adcGetBase(adc),0, ADC_TRIGGER_PROCESSOR ,0);
}

/*
 * Enables the given ADC Pin used.
 */
void adcEnableADCPin(uint32_t adc_pin_in){
	gpioSetMasterEnable(adcPinGetGPIOPort(adc_pin_in));
	GPIOPinTypeADC(adcPinGetGPIOPort(adc_pin_in),adc_pin_in);
}

/*
 * Enables a sequence for it to work when triggered.
 */
void adcSequenceEnable(uint32_t adc){
	ADCSequenceEnable(adcGetBase(adc),0);
}

/*
 * Clears the interrupt flag of the given ADC and Sequencer.
 */

void adcInterruptClear(uint32_t adc){
	ADCIntClear(adcGetBase(adc),0);
}

/*
 * Triggers the measurement of the given ADC and its
 * sequencer 0.
 */
void adcTrigger(uint32_t adc){
	ADCProcessorTrigger(adcGetBase(adc),0);
}

/*
 * Waits for the ADC processor interrupt trigger by
 * halting CPU execution.
 */
void adcTriggerWait(uint32_t adc){
	while(!ADCIntStatus(adcGetBase(adc),0,0));
}


/*
 * Enable interrupt execution on the end sample
 * sequence capture.
 */
void adcInterruptEnable(uint32_t adc){
	HWREG(adcGetBase(adc) | 0x044) = 0b00000000000001100000000000000000;
}

/*
 * Enables data transfer to FIFO0.
 */
void adcFIFOEnable(uint32_t adc){
	HWREG(adcGetBase(adc) | 0x050) = 0x00000000; //Save Samples in FIFOn
}

/*
 * Enables the ADC's 10 divider clock from PLL.
 */
void adcClockADCEnable(){
	SysCtlClockSet(SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
}

/*
 * Enables the normal clock of the system.
 */
void adcClockNormalEnable(){
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
}
///////////////////////////////////////////
// API Layer 1

/*
 * Initializes the given ADC to automatically
 * use sequencer 0, manual triggers, interrupt based
 * captures, enable the sequencer and FIFO, and clears
 * initial interrupt.
 */
void adcInit(uint32_t adc){
	adcConfigured = adc;
	adcEnable(adc);
	adcSequenceConfigure(adc);
	adcInterruptEnable(adc);
	adcFIFOEnable(adc);
	adcSequenceEnable(adc);
	adcInterruptClear(adc);
}

/*
 * Assigns a data storage index for the data from the
 * ADC.
 */
void adcMuxPinSet(uint32_t adc, uint32_t adc_mux, uint32_t adc_pin_in){
	HWREG(adcGetBase(adc) | 0x040) = adc_pin_in << adcGetMuxShift(adc_mux) ;
	adcMuxConfig[adc_pin_in] = adc_mux;

}

/*
 * Refreshes the data struct of the module library for
 * the configured ADC.
 */
void adcRefresh(){
	adcClockADCEnable();
	adcTrigger(adcConfigured);
	adcTriggerWait(adcConfigured);
	adcInterruptClear(adcConfigured);
	ADCSequenceDataGet(adcGetBase(adcConfigured), 0, adcDataStruct);
	adcClockNormalEnable();
}

/*
 * Returns the data from the data struct in the module library.
 */
uint32_t adcDataGet(uint32_t adc_pin_in){
	return adcDataStruct[adcMuxConfig[adc_pin_in]];
}




