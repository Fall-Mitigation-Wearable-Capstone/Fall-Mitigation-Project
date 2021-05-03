/* ************************************************************************** */
/** Fall Injury Mitigation Wearable Team
 * UCSC ECE Senior Capstone 2020-21
 
 * File name: FRT.c
 
 * File description: This is the c file for free running timer peripheral. These
 * functions allow the program to keep track of time which aids in data logging
 * and ensuring correctly timed intervals.
 
 * Author: David Prager
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
#include <xc.h>
#include "BOARD.h"
#include "FRT.h"
#include <sys/attribs.h>

/* ************************************************************************** */
/* Private Variables and Functions                                            */
/* ************************************************************************** */
#define PB_FREQUENCY 40000000 //40MHz clk
#define TIMER_FREQUENCY 1000 //1ms roll-over
#define PRESCALER 0 //1:1 prescaler

void __ISR(_TIMER_5_VECTOR, ipl3auto) Timer5IntHandler(void); //tmr5 isr

static unsigned int milli; //millisecond counter
static unsigned int micro; //microsecond counter

/* ************************************************************************** */
/* Section: Library Functions                                                 */
/* ************************************************************************** */

/**
Function: FRT_Init
param: none
return: none
brief: intializes timer 5 and its interrupt
 **/
void FRT_Init(void) {
    T5CON = 0; //turn off all control bits related to tmr5
    T5CONbits.TCKPS = PRESCALER; // prescaler set to 1:1
    TMR5 = 0; //clear tmr5 register
    PR5 = PB_FREQUENCY / (TIMER_FREQUENCY); //initializing period register

    IFS0bits.T5IF = 0; //tmr5 isr flag set low
    IPC5bits.T5IP = 2; //tmr5 priority set to 2
    IEC0bits.T5IE = 1; //tmr5 enabled

    T5CONbits.ON = 1; //tmr5 enabled
}

/**
Function: FRT_GetMicroSeconds
param: none
return: unsigned int
brief: returns the current count for microseconds passed since initialization
 **/
unsigned int FRT_GetMicroSeconds(void) {
    return micro;
}

/**
Function: FRT_GetMilliSeconds
param: none
return: unsigned int
brief: returns the current count for milliseconds passed since initialization
 **/
unsigned int FRT_GetMilliSeconds(void) {
    return milli;
}

void __ISR(_TIMER_5_VECTOR, ipl3auto) Timer5IntHandler(void) {
    if (IFS0bits.T5IF) {
        IFS0bits.T5IF = 0; //interrupt flag set low

        milli++; //increment millisecond count
        micro += 1000; //increment microsecond count
        TMR5 = 0; //clear tmr5 register
    }
}