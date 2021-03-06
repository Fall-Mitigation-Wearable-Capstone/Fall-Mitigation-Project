/* ************************************************************************** */
/** Fall Injury Mitigation Wearable Team
 * UCSC ECE Senior Capstone 2020-21
 
 * File name: fallDetection.h
 
 * File description: This is the header file for the fall detection algorithm.
 * These functions read the IMU and check if the incoming data signals whether a 
 * fall or activity of daily life (ADL) has occurred. 
 
 * Author: David Prager and Archisha Sinha
 */
/* ************************************************************************** */

#ifndef FALL_DETECTION_H    
#define FALL_DETECTION_H

#define FORWARD 0b0001  //Forward fall flag value
#define BACKWARDS 0b0010 //Backwards fall flag value
#define LEFT 0b0100  //Left fall flag value
#define RIGHT 0b1000 //Right fall flag value
float diffRoll;
float diffPitch;

/* 
Function: fallDetection_updateData
Param: The most recently updated euler angles and gyroscope rates
Return: none
Brief: Updates the data buffers and recalculates euler angle slopes
*/
void fallDetection_updateData(float pitch, float roll, float gyroX, float gyroY);

/*
 Function: fallDetection_updateFlags
 Param: none
 Return: none
 Brief: Updates fall flag count if new data falls within a fall type's threshold
*/
void fallDetection_updateFlags(void);

/*
 Function: fallDetection_detectFalls
 Param: The most recently updated euler angles and gyroscope rates
 Return: A bit-masked integer that indicates which falls may have occurred 
 Brief: Updates data and flags to see if any falls have occurred
 */
int fallDetection_detectFalls(void);

void fallDetection_resetFlags(void);

#endif