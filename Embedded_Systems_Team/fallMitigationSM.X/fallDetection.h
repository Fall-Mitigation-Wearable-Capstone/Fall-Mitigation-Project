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


/* ************************************************************************** */
/* Section: Variables
/* ************************************************************************** */

 
/* 
Function: fallDetection_Init
Param: none
Return: none
Brief: Initializes the battery and touch sensor pins
*/
void fallDetection_Init(void);

/* 
Function: fallDetection_updateData
Param: none
Return: SUCCESS if battery is usable, ERROR if battery is too low
Brief: Reads the battery level
*/
void fallDetection_updateData(float pitch, float roll, float gyroX, float gyroY);

/*
 Function: fallDetection_updateFlags
 Param: none
 Return: none
 Brief: Set LED lights to indicate the battery level
*/
void fallDetection_updateFlags(void);

/*
 Function: fallDetection_detectFalls
 Param: none
 Return: SUCCESS if battery is full, ERROR if not full
 Brief: Reads the level of the battery while it is charging
 */
int fallDetection_detectFalls(float pitch, float roll, float gyroX, float gyroY);
