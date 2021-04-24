/* ************************************************************************** */
/** Fall Injury Mitigation Wearable Team
 * UCSC ECE Senior Capstone 2020-21
 
 * File name: MPU9250.h
 
 * File description: This is the header file for MPU9250 IMU. The IMU uses I2C 
 * for communication and is used to detect movement. 
 
 * Author: Archisha Sinha and David Prager
 */
/* ************************************************************************** */

#ifndef MPU9250_H   
#define MPU9250_H

/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */

/* ************************************************************************** */
/* Section: Library Variables                                                 */
/* ************************************************************************** */
//Variables store the newest read data from the IMU
volatile float gyroX;  //x-axis gyroscope data
volatile float gyroY;  //y-axis gyroscope data
volatile float gyroZ;  //z-axis gyroscope data
volatile float accelX; //x-axis accelerometer data
volatile float accelY; //y-axis accelerometer data
volatile float accelZ; //z-axis accelerometer data

volatile int dataReadStatus; //variable stores whether or not data was read successfully
/* ************************************************************************** */
/* Section: Library Functions                                                 */
/* ************************************************************************** */
/* 
 * Function: MPU9250_Init
 * Param: none
 * Return: SUCCESS or ERROR
 * Brief: Initializes the IMU  and Timer3 interrupt
*/
int MPU9250_Init(void);

/* 
 * Function: MPU9250_readGyroX
 * Param: none
 * Return: none
 * Brief: Reads the gyroscope x-axis value and converts it to a float
 */
void MPU9250_readGyroX(void);

/* 
 * Function: MPU9250_readGyroY
 * Param: none
 * Return: none
 * Brief: Reads the gyroscope y-axis value and converts it to a float
 */
void MPU9250_readGyroY(void);

/* 
 * Function: MPU9250_readGyroZ
 * Param: none
 * Return: none
 * Brief: Reads the gyroscope z-axis value and converts it to a float
 */
void MPU9250_readGyroZ(void);

/* 
 * Function: MPU9250_readAccelX
 * Param: none
 * Return: none
 * Brief: Reads the accelerometer x-axis value and converts it to a float
 */
void MPU9250_readAccelX(void);

/* 
 * Function: MPU9250_readAccelY
 * Param: none
 * Return: none
 * Brief: Reads the accelerometer y-axis value and converts it to a float
 */
void MPU9250_readAccelY(void);

/* 
 * Function: MPU9250_readAccelZ
 * Param: none
 * Return: none
 * Brief: Reads the accelerometer z-axis value and converts it to a float
 */
void MPU9250_readAccelZ(void);

/* 
 * Function: MPU9250_readIMU
 * Param: none
* Return: SUCCESS or ERROR depending on if data has been read
 * Brief: Get all orientation data from the IMU
*/
int MPU9250_readIMU(void);
#endif 