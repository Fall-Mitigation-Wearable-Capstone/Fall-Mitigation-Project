#include <xc.h>
#include "BOARD.h"
#include "ADC.h"
#include <sys/attribs.h>

#define CHANNELS 3 //number of ADC ports used (0-2)
#define BUFFERLENGTH 32 //Max length of each channel's buffer

void __ISR(_ADC_VECTOR) ADCIntHandler(void); //ADC isr

static short data[CHANNELS][BUFFERLENGTH]; //buffers to hold pin readings
static int bufferIndex; //index all channel readings

/**
Function: ADC_Init(void);
param: none
return: SUCCESS or ERROR
brief: initializes ADC pins 
**/
int ADC_Init(void){
	AD1CON1bits.ADON = 0; //turn off all AD control bits
    IEC1bits.AD1IE = 0; //turn off all 

    AD1PCFGbits.PCFG2 = 0; //Connect A0 to ADC
    AD1PCFGbits.PCFG4 = 0; //Connect A1 to ADC
    AD1PCFGbits.PCFG8 = 0; //Connect A2 to ADC
    //AD1PCFGbits.PCFG10 = 0;

    AD1CSSLbits.CSSL2 = 1; //Add A0 to scanner
    AD1CSSLbits.CSSL4 = 1; //Add A1 to scanner
    AD1CSSLbits.CSSL8 = 1; //Add A2 to scanner
    //AD1CSSLbits.CSSL10 = 1;

    TRISBbits.TRISB2 = 1; //Set A0 pin as input
    TRISBbits.TRISB4 = 1; //Set A1 pin as input
    TRISBbits.TRISB8 = 1; //Set A2 pin as input
    //TRISBbits.TRISB10 = 1;

    AD1CON1bits.ASAM = 1; //auto-sample
    AD1CON1bits.SSRC = 7; //auto convert
    AD1CON1bits.FORM = 0; //unsigned 16 bit mode
    AD1CON2bits.VCFG = 0; //ADC uses internal voltage references
    AD1CON2bits.CSCNA = 1; //scan mode
    AD1CON2bits.SMPI = 3; //interrupt after each set of measurements
    AD1CON2bits.BUFM = 0; //ADC uses 16 bit buffer
    AD1CON3bits.ADRC = 0; //TAD uses PBclk

	// TAD (ADC internal clk)
	// TPB (Peripheral Bus clk)
    AD1CON3bits.ADCS = 173; //ADC TAD to 348 TPB
    AD1CON3bits.SAMC = 16; //sample time to 16 TAD

    IFS1bits.AD1IF = 0; //ADC isr flag set low
    IPC6bits.AD1IP = 1; //ADC isr priorty set to 1

    AD1CON1bits.ADON = 1; //turn on ADC
    IEC1bits.AD1IE = 1; //enable ADC interrupt

	return 0;
}

/**
Function: ADC_CurrentReading
param: Pin to be read from
return: Raw AD reading from the pin
brief: reads the most recently stored value in the selected pins buffer
**/
short ADC_CurrentReading(short pin){
	return data[pin][bufferIndex];
}

void __ISR(_ADC_VECTOR) ADCIntHandler(void){
	IFS1bits.AD1F = 0; //clear isr flag
	
	bufferIndex++; //increment buffer index
	bufferIndex = bufferIndex % BUFFERLENGTH; //address rollover
	 
	data[0][bufferIndex] = ADC1BUF0; //update buffer A0
	data[1][bufferIndex] = ADC1BUF1; //update buffer A1
	data[2][bufferIndex] = ADC1BUF2; //update buffer A2
}