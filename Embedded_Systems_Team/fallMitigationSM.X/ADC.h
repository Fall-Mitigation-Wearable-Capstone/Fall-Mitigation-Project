/* ************************************************************************** */
/** Fall Injury Mitigation Wearable Team
 * UCSC ECE Senior Capstone 2020-21
 
 * File name: ADC.h
 
 * File description: This is the c file for ADC peripheral. These functions allow
 * data to be collected off of the AD pins on the uc32.
 
 * Author: David Prager and Archisha Sinha
 */
/* ************************************************************************** */

#ifndef ADC_H
#define ADC_H

/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */

/* ************************************************************************** */
/* Section: Library Variables                                                 */
/* ************************************************************************** */


/* ************************************************************************** */
/* Section: Library Functions                                                 */
/* ************************************************************************** */
/*
 * Function: ADC_Init(void);
 * param: none
 * return: SUCCESS or ERROR
 * brief: initializes ADC pins 
*/
int ADC_Init(void);

/*
 * Function: ADC_CurrentReading
 * param: Pin to be read from
 * return: Raw AD reading from the pin
 * brief: reads the most recently stored value in the selected pins buffer
*/
short ADC_CurrentReading(short pin);

#endif
