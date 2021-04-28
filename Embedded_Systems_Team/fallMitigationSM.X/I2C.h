/* ************************************************************************** */
/** Fall Injury Mitigation Wearable Team
 * UCSC ECE Senior Capstone 2020-21
 
 * File name: I2C.h
 
 * File description: This is the header file for the I2C peripheral. I2C is used
 * communicate with the MPU9250 IMU.
 
 * Author: Archisha Sinha and David Prager
 */
/* ************************************************************************** */

#ifndef I2C_H    
#define I2C_H

/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
#include <math.h>

/* ************************************************************************** */
/* Section: Library Variables                                                 */
/* ************************************************************************** */
#define IMU_ERROR HUGE_VAL //This is the error code when reading the IMU
//int d = 0;
/* ************************************************************************** */
/* Section: Library Functions                                                 */
/* ************************************************************************** */
/* 
 * Function: I2C_Init
 * Param: none
 * Return: none
 * Brief: Initializes the I2C peripheral
*/
void I2C_Init(void);

/* 
 * Function: I2C_read
 * Param: deviceAddress = 8 bit device register address
 * Return: value at register or ERROR
 * Brief: Initializes the battery and touch sensor pins
*/
unsigned char I2C_read(unsigned char deviceAddress);

/* 
 * Function: I2C_write
 * Param: deviceAddress = 8 bit device register address
 * Param: data = information to be written to the register
 * Return: SUCCESS or ERROR
 * Brief: Initializes the battery and touch sensor pins
*/
unsigned char I2C_write(unsigned char deviceAddress, char data);

/* 
 * Function: I2C_readInteger
 * Param: deviceAddress = 8 bit device register address
 * Return: data read or ERROR
 * Brief: Initializes the battery and touch sensor pins
*/
int I2C_readInteger(unsigned char deviceAddress);

#endif