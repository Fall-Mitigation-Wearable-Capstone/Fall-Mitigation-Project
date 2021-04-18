/* ************************************************************************** */
/** Fall Injury Mitigation Wearable Team
 * UCSC ECE Senior Capstone 2020-21
 
 * File name: fallDetection.h
 
 * File description: This is the header file for the fall detection algorithm.
 * These functions read the IMU and check if the incoming data signals whether a 
 * fall or activity of daily life (ADL) has occurred. 
 
 * Author: Archisha Sinha and David Prager
 */
/* ************************************************************************** */

#ifndef FALL_DETECTION_H    
#define FALL_DETECTION_H


/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */

   
/* ************************************************************************** */
/* Section: Constants                                                         */
/* ************************************************************************** */
#define BATTERY_PIN
#define TOUCH_LEFT_PIN
#define TOUCH_RIGHT_PIN

#define LOW_BATTERY_LED
#define MID_BATTERY_LED
#define HIGH_BATTERY_LED

#define BATTERY_LOW
#define BATTERY_MEDIUM
#define BATTERY_HIGH
#define BATTERY_FULL

/* ************************************************************************** */
/* Section: Variables
/* ************************************************************************** */

 
/* 
Function: checkingInit
Param: none
Return: none
Brief: Initializes the battery and touch sensor pins
*/
void checkingInit(void);

/* 
Function: getBatteryLevel
Param: none
Return: SUCCESS if battery is usable, ERROR if battery is too low
Brief: Reads the battery level
*/
int getBatteryLevel(void);

/*
 Function: setBatteryLevel
 Param: none
 Return: none
 Brief: Set LED lights to indicate the battery level
*/
void setBatteryLevel(void);

/*
 Function: checkBatteryCharging
 Param: none
 Return: SUCCESS if battery is full, ERROR if not full
 Brief: Reads the level of the battery while it is charging
 */
int checkBatteryCharging(void);

/*
 Function: getTouchSensorReading
 Param: none
 Return: none
 Brief: Read status of touch sensors
 */
void getTouchSensorReading(void);

/*
 Function: checkForUser
 Param: none
 Return: SUCCESS if both touch sensors have a high signal, ERROR if either are low
 Brief: Checks if the touch sensors signal that a user is wearing a wearable
 */
int checkForUser(void);