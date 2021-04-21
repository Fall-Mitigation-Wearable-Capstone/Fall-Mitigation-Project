#ifndef ADC_H
#define ADC_H


/**
Function: ADC_Init(void);
param: none
return: SUCCESS or ERROR
brief: initializes ADC pins 
**/
int ADC_Init(void);

/**
Function: ADC_CurrentReading
param: Pin to be read from
return: Raw AD reading from the pin
brief: reads the most recently stored value in the selected pins buffer
**/
short ADC_CurrentReading(short pin);

#endif
