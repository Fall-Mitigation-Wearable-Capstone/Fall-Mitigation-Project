/* ************************************************************************** */
/** Fall Injury Mitigation Wearable Team
 * UCSC ECE Senior Capstone 2020-21
 
 * File name: MPU9250.c
 
 * File description: This is the c file for MPU9250 IMU. The IMU uses I2C 
 * for communication and is used to detect movement. 
 
 * Author: Archisha Sinha
 * 
 * Datasheet References: 
 * https://cdn.sparkfun.com/assets/learn_tutorials/5/5/0/MPU-9250-Register-Map.pdf
 * https://invensense.tdk.com/wp-content/uploads/2015/02/PS-MPU-9250A-01-v1.1.pdf
 */
/* ************************************************************************** */


/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
#include <stdio.h>
#include <stdlib.h>
#include "sys/attribs.h"
#include <xc.h>
#include "BOARD.h"
#include "I2C.h"
#include "MPU9250.h"

/* ************************************************************************** */
/* Private Constants                                                          */
/* ************************************************************************** */
#define GYRO_FSR 250    //Full scale range of gyroscope
#define ACCEL_FSR 2     //Full scale range of accelerometer
#define GYRO_SSF 131.f  //Sensitivity scale factor of gyroscope in LSB/(deg/sec)
#define ACCEL_SSF 16384.f //Sensitivity scale factor of accelerometer in LSB/g
#define WHOAMI_DEFAULT_VALUE 0x71 //Expected value of the who am I when reading register

static enum {
    SMPLRT_DIV = 26, //Sets sampling rate
    GYRO_CONFIG = 27, //Configures gyroscope full scale select
    ACCEL_CONFIG = 28, //Configures accelerometer full scale select
    ACCEL_CONFIG_2 = 29, //Configures accelerometer data rate
    ACCEL_XOUT_H = 59, //High byte of accelerometer x-axis data
    ACCEL_XOUT_L = 60, //Low byte of accelerometer x-axis data
    ACCEL_YOUT_H = 61, //High byte of accelerometer y-axis data
    ACCEL_YOUT_L = 62, //Low byte of accelerometer y-axis data
    ACCEL_ZOUT_H = 63, //High byte of accelerometer z-axis data   
    ACCEL_ZOUT_L = 64, //Low byte of accelerometer z-axis data
    GYRO_XOUT_H = 67, //High byte of gyroscope x-axis data
    GYRO_XOUT_L = 68, //Low byte of gyroscope x-axis data
    GYRO_YOUT_H = 69, //High byte of gyroscope y-axis data
    GYRO_YOUT_L = 70, //Low byte of gyroscope y-axis data
    GYRO_ZOUT_H = 71, //High byte of gyroscope z-axis data
    GYRO_ZOUT_L = 72, //Low byte of gyroscope z-axis data
    PWR_MGMT_1 = 107, //Turns on/off IMU from sleep mode
    WHO_AM_I = 117, //Register address used to verify identity of device
} MPU_REGISTER_ADDRESSES;

/* ************************************************************************** */
/* Section: Library Variables                                                 */
/* ************************************************************************** */

/* 
 * Function: MPU9250_Init
 * Param: none
 * Return: SUCCESS or ERROR
 * Brief: Initializes the IMU
 */
int MPU9250_Init(void) {
    //Initialize the I2C peripheral to communicate with MPU9250
    I2C_Init();
    
    //Check to see that communication is occurring with the correct device
    unsigned char byteCheck = I2C_read(WHO_AM_I);   //Read the device
    //If who am I test fails, return ERROR
    if (byteCheck != WHOAMI_DEFAULT_VALUE){         //Verify for the correct device
        return ERROR;       
    }
    
    //Turn on IMU from sleep mode
    //Set it to auto select best available clock source and choose PLL if possible
    I2C_write(PWR_MGMT_1, 0x01);
    
    //Configure gyroscope to 250 deg/sec full scale select
    I2C_write(GYRO_CONFIG, 0x00);
    
    //Set sample rate of IMU to 200Hz
    //Internal sample rate = 1kHz
    I2C_write(SMPLRT_DIV, 4);  //SMPLRT_DIV = (Internal sample rate / desired rate) - 1
    
    //Set internal sample rate of IMU to 1kHz and bandwidth of accelerometer to 184Hz (closest possible to 200Hz requirement)
    I2C_write(ACCEL_CONFIG_2, 0x9);
    
    //Set accelerometer full scale select to 2g
    I2C_write(ACCEL_CONFIG, 0x00);
    
    //IMU initialized properly
    return SUCCESS;
}

/* 
 * Function: MPU9250_readGyroX
 * Param: none
 * Return: none
 * Brief: Reads the gyroscope x-axis value and converts it to a float
 */
void MPU9250_readGyroX(void) {
    //readInt
    int temp_gyroX = I2C_readInteger(GYRO_XOUT_H);
    //cast value as float and divide by ssf at 250 deg/sec 
    gyroX = (float) temp_gyroX / GYRO_SSF;
}

/* 
 * Function: MPU9250_readGyroY
 * Param: none
 * Return: none
 * Brief: Reads the gyroscope y-axis value and converts it to a float
 */
void MPU9250_readGyroY(void) {
    int temp_gyroY = I2C_readInteger(GYRO_YOUT_H);
    gyroY = (float) temp_gyroY / GYRO_SSF;
}

/* 
 * Function: MPU9250_readGyroZ
 * Param: none
 * Return: none
 * Brief: Reads the gyroscope z-axis value and converts it to a float
 */
void MPU9250_readGyroZ(void) {
    int temp_gyroZ = I2C_readInteger(GYRO_ZOUT_H);
    gyroZ = (float) temp_gyroZ / GYRO_SSF;
}

/* 
 * Function: MPU9250_readAccelX
 * Param: none
 * Return: none
 * Brief: Reads the accelerometer x-axis value and converts it to a float
 */
void MPU9250_readAccelX(void) {
    //readInt
    int temp_accelX = I2C_readInteger(ACCEL_XOUT_H);
    //cast value as float and divide by ssf at 2g
    accelX = (float) temp_accelX / ACCEL_SSF;
}

/* 
 * Function: MPU9250_readAccelY
 * Param: none
 * Return: none
 * Brief: Reads the accelerometer y-axis value and converts it to a float
 */
void MPU9250_readAccelY(void) {
    int temp_accelY = I2C_readInteger(ACCEL_YOUT_H);
    accelY = (float) temp_accelY / ACCEL_SSF;
}

/* 
 * Function: MPU9250_readAccelZ
 * Param: none
 * Return: none
 * Brief: Reads the accelerometer z-axis value and converts it to a float
 */
void MPU9250_readAccelZ(void) {
    int temp_accelZ = I2C_readInteger(ACCEL_ZOUT_H);
    accelZ = (float) temp_accelZ / ACCEL_SSF;
}

/* 
 * Function: MPU9250_readIMU
 * Param: none
 * Return: none
 * Brief: Initializes the I2C peripheral
 */
void MPU9250_readIMU(void) {
    MPU9250_readGyroX();
    MPU9250_readGyroY();
    MPU9250_readGyroZ();
    MPU9250_readAccelX();
    MPU9250_readAccelY();
    MPU9250_readAccelZ();
}


/* ************************************************************************** */
/* Section: Test main                                                         */
/* ************************************************************************** */
#define TEST_IMU_MAIN
#ifdef TEST_IMU_MAIN

#include <math.h>

int main(void)
{
    BOARD_Init();

    printf("Testing MPU9250 Library\r\n");
    if (MPU9250_Init() == ERROR) {
        printf("Error with sensor\r\n");
        while (1);
    }
//    int q;
//    for (q = 0; q < 100; q++) asm("nop");

    while (1) {
        MPU9250_readIMU();
        printf("%f, %f, %f, %f, %f, %f,\n", gyroX, gyroY, gyroZ, accelX, accelY, accelZ);
    }
    return 1;
}

#endif

/* ************************************************************************** */
/* Section: Hello world                                                       */
/* ************************************************************************** */
//#define HELLO
#ifdef HELLO

#include "serial.h"

int main(void){
    BOARD_Init();
    TRISE = 0;
    LATE = 0xFF;
    while(1){
        printf("Hello MR. PRAGER\r\n");
    }
    return(1);
}

#endif