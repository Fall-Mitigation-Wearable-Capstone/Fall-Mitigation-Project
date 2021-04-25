/* ************************************************************************** */
/** Fall Injury Mitigation Wearable Team
 * UCSC ECE Senior Capstone 2020-21
 
 * File name: checking.h
 
 * File description: This is the header file for the initial status checking
 * functions. These functions check the status of the battery and touch sensors. 
 
 * Author: Archisha Sinha
 */
/* ************************************************************************** */

#ifndef CHECKING_H    /* Guard against multiple inclusion */
#define CHECKING_H


/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
#include "BOARD.h"
#include <xc.h>
#include "FRT.h"
#include <sys/attribs.h>   
/* ************************************************************************** */
/* Section: Constants                                                         */
/* ************************************************************************** */
#define BATTERY_PIN 3       //Set this to 0 if testing with potentiometer
#define TOUCH_RIGHT_PIN PORTDbits.RD1
#define TOUCH_LEFT_PIN PORTDbits.RD2

#define LOW_BATTERY_LED LATGbits.LATG8
#define MID_BATTERY_LED LATGbits.LATG7
#define HIGH_BATTERY_LED LATGbits.LATG6

#define BATTERY_LOW 100
#define BATTERY_MEDIUM 800
#define BATTERY_HIGH 1000
#define BATTERY_FULL 1023

/* ************************************************************************** */
/* Section: Variables                                                         */
/* ************************************************************************** */
volatile short batteryLevel; //Stores the AD value of the battery level to be called in the SM directly
volatile int touchRightStatus; //Stores the value of the right touch sensor (0 or 1)
volatile int touchLeftStatus; //Stores the value of the left touch sensor (0 or 1)

/* ************************************************************************** */
/* Section: Library Functions                                                 */
/* ************************************************************************** */
/* 
Function: checkingInit
Param: none
Return: none
Brief: Initializes the battery and touch sensor pins
 */
void checking_Init(void);

/* 
Function: getBatteryLevel
Param: none
Return: SUCCESS if battery is usable, ERROR if battery is too low
Brief: Reads the battery level
 */
void checking_getBatteryLevel(void);

/*
 Function: setBatteryLevel
 Param: none
 Return: none
 Brief: Set LED lights to indicate the battery level
 */
void checking_setBatteryLevelLights(void);

/*
 Function: checkBatteryCharging
 Param: none
 Return: SUCCESS if battery is full, ERROR if not full
 Brief: Reads the level of the battery while it is charging
 */
int checking_checkBatteryCharging(void);

/*
 Function: getTouchSensorReading
 Param: none
 Return: none
 Brief: Read status of touch sensors
 */
void checking_getTouchSensorReading(void);

/*
 Function: checkForUser
 Param: none
 Return: SUCCESS if both touch sensors have a high signal, ERROR if either are low
 Brief: Checks if the touch sensors signal that a user is wearing a wearable
 */
int checking_checkForUser(void);

#endif