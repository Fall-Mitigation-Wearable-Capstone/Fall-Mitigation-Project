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
volatile float gyroX;
volatile float gyroY;
volatile float gyroZ;
volatile float accelX;
volatile float accelY;
volatile float accelZ;

/* 
 * Function: MPU9250_Init
 * Param: none
 * Return: SUCCESS or ERROR
 * Brief: Initializes the IMU
*/
int MPU9250_Init(void);

/* 
 * Function: MPU9250_readGyroX
 * Param: none
 * Return: none
 * Brief: Initializes the I2C peripheral
*/
void MPU9250_readGyroX(void);

/* 
 * Function: MPU9250_readGyroY
 * Param: none
 * Return: none
 * Brief: Initializes the I2C peripheral
*/
void MPU9250_readGyroY(void);

/* 
 * Function: MPU9250_readGyroZ
 * Param: none
 * Return: none
 * Brief: Initializes the I2C peripheral
*/
void MPU9250_readGyroZ(void);

/* 
 * Function: MPU9250_readAccelX
 * Param: none
 * Return: none
 * Brief: Initializes the I2C peripheral
*/
void MPU9250_readAccelX(void);

/* 
 * Function: MPU9250_readAccelY
 * Param: none
 * Return: none
 * Brief: Initializes the I2C peripheral
*/
void MPU9250_readAccelY(void);

/* 
 * Function: MPU9250_readAccelZ
 * Param: none
 * Return: none
 * Brief: Initializes the I2C peripheral
*/
void MPU9250_readAccelZ(void);

/* 
 * Function: MPU9250_readIMU
 * Param: none
 * Return: none
 * Brief: Initializes the I2C peripheral
*/
void MPU9250_readIMU(void);
#endif 