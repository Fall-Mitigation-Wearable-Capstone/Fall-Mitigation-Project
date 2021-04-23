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
#ifndef FRT_H
#define FRT_H

/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */

/* ************************************************************************** */
/* Section: Library Variables                                                 */
/* ************************************************************************** */


/* ************************************************************************** */
/* Section: Library Functions                                                 */
/* ************************************************************************** */

/**
Function: FRT_Init
param: none
return: none
brief: intializes timer 5 and its interrupt
**/
void FRT_Init(void);

/**
Function: FRT_GetMicroSeconds
param: none
return: unsigned int
brief: returns the current count for microseconds passed since initialization
**/
unsigned int FRT_GetMicroSeconds(void);

/**
Function: FRT_GetMilliSeconds
param: none
return: unsigned int
brief: returns the current count for milliseconds passed since initialization
**/
unsigned int FRT_GetMilliSeconds(void);


#endif 