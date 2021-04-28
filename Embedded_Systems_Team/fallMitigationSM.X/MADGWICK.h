/* ************************************************************************** */
/** Fall Injury Mitigation Wearable Team
 * UCSC ECE Senior Capstone 2020-21
 
 * File name: MADGWICK.h
 
 * File description: This is the header file for the Madgwick filter. This 
 * filter is used to convert raw IMU data to Euler angles.
 
 * Author: David Prager
 */
/* ************************************************************************** */

#ifndef MADGWICK_H
#define MADGWICK_H

/* ************************************************************************** */
/* Section: Library Variables                                                 */
/* ************************************************************************** */
typedef struct Madgwick {
	float beta; // algorithm gain
    float q0;
    float q1;
    float q2;
    float q3;	// quaternion of sensor frame relative to auxiliary frame
    float invSampleFreq;
    float roll;
    float pitch;
    float yaw;
    char anglesComputed;
} Madgwick;

/* ************************************************************************** */
/* Section: Library Functions                                                 */
/* ************************************************************************** */

/* 
 * Function: MPU9250_Init
 * Param: none
 * Return: SUCCESS or ERROR
 * Brief: Initializes the IMU
*/
float invSqrt(float x);
void computeAngles(void);

void begin(float sampleFrequency);
void update(float gx, float gy, float gz, float ax, float ay, float az);

float getRoll(void); 
float getPitch(void); 
float getYaw(void);

#endif
