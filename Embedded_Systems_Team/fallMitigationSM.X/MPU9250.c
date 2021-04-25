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
/* Private Constants and Local Functions                                      */
/* ************************************************************************** */
#define GYRO_FSR 250    //Full scale range of gyroscope
#define ACCEL_FSR 2     //Full scale range of accelerometer
#define GYRO_SSF 131.f  //Sensitivity scale factor of gyroscope in LSB/(deg/sec)
#define ACCEL_SSF 16384.f //Sensitivity scale factor of accelerometer in LSB/g
#define WHOAMI_DEFAULT_VALUE 0x71 //Expected value of the who am I when reading register

//Values used to calculate PR2
#define PB_CLOCK_FREQUENCY 40000000
#define TIMER3_FREQ 200
#define PRESCALAR3 4

#define GYRO_READ_ERROR (float)IMU_ERROR/GYRO_SSF //Value of all gyro variables if a data read error occured
#define ACCEL_READ_ERROR (float)IMU_ERROR/ACCEL_SSF //Value of all accel variables if a data read error occured

//Enum holds addresses for all needed MPU9250 registers

static enum {
    SMPLRT_DIV = 26, //Sets sampling rate
    GYRO_CONFIG = 27, //Configures gyroscope full scale select
    ACCEL_CONFIG = 28, //Configures accelerometer full scale select
    ACCEL_CONFIG_2 = 29, //Configures accelerometer data rate
    ACCEL_XOUT_H = 59, //High byte of accelerometer x-axis data
    ACCEL_YOUT_H = 61, //High byte of accelerometer y-axis data
    ACCEL_ZOUT_H = 63, //High byte of accelerometer z-axis data   
    GYRO_XOUT_H = 67, //High byte of gyroscope x-axis data
    GYRO_YOUT_H = 69, //High byte of gyroscope y-axis data
    GYRO_ZOUT_H = 71, //High byte of gyroscope z-axis data
    PWR_MGMT_1 = 107, //Turns on/off IMU from sleep mode
    WHO_AM_I = 117, //Register address used to verify identity of device
};

//TMR3 isr to read IMU data at 200Hz
void __ISR(_TIMER_3_VECTOR) Timer3IntHandler(void);
/* ************************************************************************** */
/* Section: Library Functions                                                 */
/* ************************************************************************** */

/* 
 * Function: MPU9250_Init
 * Param: none
 * Return: SUCCESS or ERROR
 * Brief: Initializes the IMU and Timer3 interrupt
 */
int MPU9250_Init(void) {
    //Initialize the I2C peripheral to communicate with MPU9250
    I2C_Init();

    //Check to see that communication is occurring with the correct device
    unsigned char byteCheck = I2C_read(WHO_AM_I); //Read the device
    //If "who am I" test fails, return ERROR
    if (byteCheck != WHOAMI_DEFAULT_VALUE) { //Verify for the correct device
        return ERROR;
    }

    //Turn on IMU from sleep mode
    //Set it to auto select best available clock source and choose PLL if possible
    I2C_write(PWR_MGMT_1, 0x01);

    //Configure gyroscope to 250 deg/sec full scale select
    I2C_write(GYRO_CONFIG, 0x00);

    //Set sample rate of IMU to 200Hz
    //Internal sample rate = 1kHz
    I2C_write(SMPLRT_DIV, 4); //SMPLRT_DIV = (Internal sample rate / desired rate) - 1

    //Set internal sample rate of IMU to 1kHz and bandwidth of accelerometer to 184Hz (closest possible to 200Hz requirement)
    I2C_write(ACCEL_CONFIG_2, 0x9);

    //Set accelerometer full scale select to 2g
    I2C_write(ACCEL_CONFIG, 0x00);

    //Set up TMR3 interrupt to read the IMU at 200Hz
    T3CON = 0x0; //Stop the timer and clear the register
    T3CONbits.TCKPS = 0b010; //Select desired timer input clock prescale to 1:4
    TMR3 = 0x0; //Clear the timer register
    PR3 = PB_CLOCK_FREQUENCY / (TIMER3_FREQ * PRESCALAR3); //Load period register to rollover at 200Hz
    IFS0bits.T3IF = 0; //TMR3 flag low
    IPC3bits.T3IP = 7; //TMR3 priority set to highest in the system
    IPC3bits.T3IS = 1; //TMR3 sub priority high in the priority level
    IEC0bits.T3IE = 1; //TMR3 enabled
    T3CONbits.ON = 1; //Turn on TMR3
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
    //read integer value of the x-axis gyroscope register
    int temp_gyroX = I2C_readInteger(GYRO_XOUT_H);
    //cast value as float and divide by gyroscope ssf to keep value in range 
    gyroX = (float) temp_gyroX / GYRO_SSF;
}

/* 
 * Function: MPU9250_readGyroY
 * Param: none
 * Return: none
 * Brief: Reads the gyroscope y-axis value and converts it to a float
 */
void MPU9250_readGyroY(void) {
    //read integer value of the y-axis gyroscope register
    int temp_gyroY = I2C_readInteger(GYRO_YOUT_H);
    //cast value as float and divide by gyroscope ssf to keep value in range
    gyroY = (float) temp_gyroY / GYRO_SSF;
}

/* 
 * Function: MPU9250_readGyroZ
 * Param: none
 * Return: none
 * Brief: Reads the gyroscope z-axis value and converts it to a float
 */
void MPU9250_readGyroZ(void) {
    //read integer value of the z-axis gyroscope register
    int temp_gyroZ = I2C_readInteger(GYRO_ZOUT_H);
    //cast value as float and divide by gyroscope ssf to keep value in range
    gyroZ = (float) temp_gyroZ / GYRO_SSF;
}

/* 
 * Function: MPU9250_readAccelX
 * Param: none
 * Return: none
 * Brief: Reads the accelerometer x-axis value and converts it to a float
 */
void MPU9250_readAccelX(void) {
    //read integer value of the x-axis accelerometer register
    int temp_accelX = I2C_readInteger(ACCEL_XOUT_H);
    //cast value as float and divide by accelerometer ssf to keep value in range
    accelX = (float) temp_accelX / ACCEL_SSF;
}

/* 
 * Function: MPU9250_readAccelY
 * Param: none
 * Return: none
 * Brief: Reads the accelerometer y-axis value and converts it to a float
 */
void MPU9250_readAccelY(void) {
    //read integer value of the x-axis accelerometer register
    int temp_accelY = I2C_readInteger(ACCEL_YOUT_H);
    //cast value as float and divide by accelerometer ssf to keep value in range
    accelY = (float) temp_accelY / ACCEL_SSF;
}

/* 
 * Function: MPU9250_readAccelZ
 * Param: none
 * Return: none
 * Brief: Reads the accelerometer z-axis value and converts it to a float
 */
void MPU9250_readAccelZ(void) {
    //read integer value of the x-axis accelerometer register
    int temp_accelZ = I2C_readInteger(ACCEL_ZOUT_H);
    //cast value as float and divide by accelerometer ssf to keep value in range
    accelZ = (float) temp_accelZ / ACCEL_SSF;
}

/* 
 * Function: MPU9250_readIMU
 * Param: none
 * Return: SUCCESS or ERROR depending on if data has been read
 * Brief: Get all orientation data from the IMU
 */
int MPU9250_readIMU(void) {
    //Read all 6 relevant orientation data points and store in global variables
    MPU9250_readGyroX();
    MPU9250_readGyroY();
    MPU9250_readGyroZ();
    MPU9250_readAccelX();
    MPU9250_readAccelY();
    MPU9250_readAccelZ();

    //Check if data has been read. Return ERROR if not, SUCCESS if yes
    //Gyro variables will equal GYRO_READ_ERROR if read error occurred
    //Accel variables will equal ACCEL_READ_ERROR if read error occurred
    if (gyroX == GYRO_READ_ERROR) {
        return ERROR;
    } else if (gyroY == GYRO_READ_ERROR) {
        return ERROR;
    } else if (gyroZ == GYRO_READ_ERROR) {
        return ERROR;
    } else if (accelX == ACCEL_READ_ERROR) {
        return ERROR;
    } else if (accelY == ACCEL_READ_ERROR) {
        return ERROR;
    } else if (accelZ == ACCEL_READ_ERROR) {
        return ERROR;
    } else {
        return SUCCESS;
    }
}

//TMR3 isr will read the MPU9250 at 200Hz

void __ISR(_TIMER_3_VECTOR) Timer3IntHandler(void) {
    if (IFS0bits.T3IF == 1) {
        IFS0bits.T3IF = 0; //Clear interrupt flag

        //Read and store IMU data. Store data read success status in global variable
        //Variable will be used in SM to determine if data read error occurred for too long
        dataReadStatus = MPU9250_readIMU();
    }
}
/* ************************************************************************** */
/* Section: Test main                                                         */
/* ************************************************************************** */
//#define TEST_IMU_MAIN
#ifdef TEST_IMU_MAIN

#include "FRT.h"
#include <math.h>

int main(void) {
    BOARD_Init();
    FRT_Init();

    printf("Testing MPU9250 Library\r\n");
    if (MPU9250_Init() == ERROR) {
        printf("Error with sensor\r\n");
        while (1);
    }
    //    int q;
    //    for (q = 0; q < 100; q++) asm("nop");

    while (1) {
        MPU9250_readIMU();
        printf("%d %f, %f, %f, %f, %f, %f,\n", FRT_GetMilliSeconds(), gyroX, gyroY, gyroZ, accelX, accelY, accelZ);
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

int main(void) {
    BOARD_Init();
    TRISE = 0;
    LATE = 0xFF;
    while (1) {
        printf("Hello MR. PRAGER\r\n");
    }
    return (1);
}

#endif