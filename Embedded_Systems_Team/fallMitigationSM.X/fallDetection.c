/* ************************************************************************** */
/** Fall Injury Mitigation Wearable Team
 * UCSC ECE Senior Capstone 2020-21
 
 * File name: fallDetection.c
 
 * File description: This is the c file for the fall detection algorithm.
 * These functions read the IMU and check if the incoming data signals whether a 
 * fall or activity of daily life (ADL) has occurred. 
 
 * Author: David Prager and Archisha Sinha
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
#include <stdio.h>
#include <stdlib.h>
#include "BOARD.h"
#include "MADGWICK.h"
#include "MPU9250.h"
#include "fallDetection.h"
#include "sys/attribs.h"
#include <xc.h>

/* ************************************************************************** */
/* Private Constants and Variables                                            */
/* ************************************************************************** */
#define TWO_HUNDRED_MS 31  //number of data points in 200ms
#define DEBOUNCE 16 //number of data points for debouncing
#define FORWARD 0b0001  //Forward fall flag value
#define BACKWARDS 0b0010 //Backwards fall flag value
#define LEFT 0b0100  //Left fall flag value
#define RIGHT 0b1000 //Right fall flag value
#define FORWARD_THRESHOLD_DIFFROLL -5 //Differential roll threshold for forward fall
#define FORWARD_THRESHOLD_GYROX -60 //GyroX buffer threshold for forward fall 
#define BACKWARDS_THRESHOLD_ROLL 60 //Roll buffer threshold for backwards fall
#define BACKWARDS_THRESHOLD_DIFFROLL 8 //Differential roll theshold for backwards fall
#define BACKWARDS_THRESHOLD_GYROX 23 //GyroX buffer threshold for backwards fall
#define LEFT_THRESHOLD_DIFFPITCH -6 //Differential pitch threshold for left fall
#define RIGHT_THRESHOLD_DIFFPITCH 6 //Differential pitch threshold for right fall

//Buffers hold the previous 31 data points to be used to check if a fall can be detected
static volatile float rollBuffer[TWO_HUNDRED_MS]; //Previous 31 roll angles
static volatile float pitchBuffer[TWO_HUNDRED_MS]; //Previous 31 pitch angles
static volatile float gyroXBuffer[TWO_HUNDRED_MS]; //Previous 31 gyroX values
static volatile float gyroYBuffer[TWO_HUNDRED_MS]; //Previous 31 gyroY values

//buffer indexing
static int bufferIndex; //On first update, will be set to 0

//flags for tracking debouncing
static int forwardFlag;
static int backFlag;
static int leftFlag;
static int rightFlag;

//values for change in each euler angle after 0.2s 
static float diffRoll;
static float diffPitch;

/* ************************************************************************** */
/* Section: Library Functions                                                 */
/* ************************************************************************** */

/* 
Function: fallDetection_Init
Param: none
Return: none
Brief: Initializes IMU interrupt 
 */
void fallDetection_Init(void) {
    //Initialize TMR3 interrupt for reading IMU at 200Hz
    //wiggle wiggle
}

/* 
Function: fallDetection_updateData
Param: The most recently updated euler angles and gyroscope rates
Return: none
Brief: Updates the data buffers and recalculates euler angle slopes
 */
void fallDetection_updateData(float pitch, float roll, float gyroX, float gyroY) {
    //update index and consider roll-over
    bufferIndex++;
    if (bufferIndex == TWO_HUNDRED_MS) {
        bufferIndex = 0;
    }

    //update differential euler values
    int tempIndex = bufferIndex;
    diffRoll = roll - rollBuffer[tempIndex];
    diffPitch = pitch - pitchBuffer[tempIndex];
    
    //update the individual data buffers
    rollBuffer[bufferIndex] = roll;
    pitchBuffer[bufferIndex] = pitch;
    gyroXBuffer[bufferIndex] = gyroX;
    gyroYBuffer[bufferIndex] = gyroY;
}

/*
 Function: fallDetection_updateFlags
 Param: none
 Return: none
 Brief: Updates fall flag count if new data falls within a fall type's threshold
 */
void fallDetection_updateFlags(void) {
    //Check current data with forward fall thresholds
    if (diffRoll <= FORWARD_THRESHOLD_DIFFROLL && gyroXBuffer[bufferIndex] <= FORWARD_THRESHOLD_GYROX) {
        //increment forward counter in response to detected forward fall
        if (forwardFlag < DEBOUNCE) {
            forwardFlag++;
        }
    } else {
        //decrement forward counter in response to no detected forward fall
        if (forwardFlag > 0) {
            forwardFlag--;
        }
    }

    //Check current data with backwards fall thresholds
    if (rollBuffer[bufferIndex] > BACKWARDS_THRESHOLD_ROLL && diffRoll > BACKWARDS_THRESHOLD_DIFFROLL && gyroXBuffer[bufferIndex] > BACKWARDS_THRESHOLD_GYROX) {
        //increment backwards counter in response to detected backwards fall
        if (backFlag < DEBOUNCE) {
            backFlag++;
        }
    } else {
        //decrement backwards counter in response to no detected backwards fall
        if (backFlag > 0) {
            backFlag--;
        }
    }

    //Check current data with left fall thresholds
    if (diffPitch < LEFT_THRESHOLD_DIFFPITCH) {
        //increment left counter in response to detected left fall
        if (leftFlag < DEBOUNCE) {
            leftFlag++;
        }
    } else {
        //decrement left counter in response to no detected left fall
        if (leftFlag > 0) {
            leftFlag--;
        }
    }

    //Check current data with right fall thresholds
    if (diffPitch > RIGHT_THRESHOLD_DIFFPITCH) {
        //increment right counter in response to detected right fall
        if (rightFlag < DEBOUNCE) {
            rightFlag++;
        }
    } else {
        //decrement right counter in response to no detected right fall
        if (rightFlag > 0) {
            rightFlag--;
        }
    }
}

/*
 Function: fallDetection_detectFalls
 Param: The most recently updated euler angles and gyroscope rates
 Return: A bit-masked integer that indicates which falls may have occurred 
 Brief: Updates data and flags to see if any falls have occurred
 */
int fallDetection_detectFalls(float pitch, float roll, float gyroX, float gyroY) {
    //Update data buffers with new readings
    fallDetection_updateData(pitch, roll, gyroX, gyroY);
    //Update flags based on new data readings
    fallDetection_updateFlags();

    //return value to be bit-masked if falls are detected
    int out = 0;
    if (forwardFlag >= DEBOUNCE){
        forwardFlag = 0;
        out |= FORWARD; //forward count exceeds debounce => forward fall occurred
    } 
    if (backFlag >= DEBOUNCE){
        backFlag = 0;
        out |= BACKWARDS; //backwards count exceeds debounce => backwards fall occurred
    } 
    if (leftFlag >= DEBOUNCE){
        leftFlag = 0;
        out |= LEFT; //left count exceeds debounce => left fall occurred
    } 
    if (rightFlag >= DEBOUNCE){
        rightFlag = 0;
        out |= RIGHT; //right count exceeds debounce => right fall occurred
    } 
    return out;
}

/* ************************************************************************** */
/* Section: Test main                                                         */
/* ************************************************************************** */
#define TEST_FALL_DETECTION_MAIN
#ifdef TEST_FALL_DETECTION_MAIN
#include "BOARD.h"
#include "FRT.h"
#include <xc.h>
#include <sys/attribs.h>
#include <stdio.h>
#include <stdlib.h>

enum fallSubstates {
    READ_IMU, //Beginning of super state 2
    IMU_ERROR,
    DETECT_FALLS, //End of super state 1
    DONE
} states;
enum fallSubstates states = READ_IMU;

static unsigned int prevDataReadTime; //Used to keep track of data reading times to detect errors

int main(void) {
    BOARD_Init();
    FRT_Init();
    begin(200.f);
    
    TRISE = 0;
    LATE = 0;
    
    printf("Testing fall detection as state machine\r\n");
    if (MPU9250_Init() == ERROR) {
        printf("Error with sensor\r\n");
        while (1);
    }

    int time = FRT_GetMilliSeconds(), t, fall;
    while (1) {
        if (FRT_GetMilliSeconds() - time >= 5) {
            time = FRT_GetMilliSeconds();
            switch (states) {
                case READ_IMU:
                    //printf("detecting Read\r\n");
                    //Checks if data was read properly
                    if (dataReadStatus == ERROR) {
                        /*If data has not been read for 20ms, reading frequency has dropped
                         * below 100Hz which is too slow for accurate fall detection. */
                        printf("Error occurred with read\r\n");
                        if (FRT_GetMilliSeconds() - prevDataReadTime >= 80) {
                            states = IMU_ERROR; //Failed read
                        }
                    } else {
                        //printf("read occurred successfully\r\n");
                        prevDataReadTime = FRT_GetMilliSeconds(); //Update time of good data read 
                        states = DETECT_FALLS; //Data ready
                    }
                    break;

                case IMU_ERROR:
                    printf("detecting IMU ERROR\r\n");
                    printf("STOPPING SYSTEM (Reset ESP to continue testing)\r\n");
                    break;

                case DETECT_FALLS:
                    //                printf("detecting Falls\r\n");
                    //                    update(gyroX, gyroY, gyroZ, accelX, accelY, accelZ); //Convert raw IMU data to Euler angles
                    //                    float p = getPitch();
                    //                    float roll = getRoll();
                    printf("%.2f %.2f %.2f %.2f\r\n", getPitch(), getRoll(), gyroX, gyroY);
                    //Use the fall detection algorithm to detect falls versus ADLs
                    fall = fallDetection_detectFalls(getPitch(), getRoll(), gyroX, gyroY);
                    if (fall != 0) {
                        //                    printf("Fall detected\r\n");
                        //                    printf("Try to inflate now\r\n");
                        if(fall & FORWARD) printf("F");
                        if(fall & BACKWARDS) printf("B");
                        if(fall & LEFT) printf("L");
                        if(fall & RIGHT) printf("R");
                        printf("\r\n");
                        
                        states = DONE;
                        t = FRT_GetMilliSeconds();
                        LATE = 0xFF;
                        break;
                    } else {
                        //                    printf("ADL detected\r\n");
                        states = READ_IMU; //ADL detected
                    }
                    break;
                case DONE:
                    //printf("Fall has been detected:   ");
                    printf("%.2f %.2f %.2f %.2f\r\n", getPitch(), getRoll(), gyroX, gyroY);
                    if (FRT_GetMilliSeconds() - t > 1000) {
                        LATE = 0;
                        states = READ_IMU;
                    }
                    break;
            }
        }
    }
}
#endif

//#define TEST_IMU_MAIN
#ifdef TEST_IMU_MAIN

#include "FRT.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    BOARD_Init();
    FRT_Init();
    TRISE = 0;
    LATE = 0xFF;
    printf("Testing MPU9250 Library\r\n");
    if (MPU9250_Init() == ERROR) {
        printf("Error with sensor\r\n");
        while (1);
    }
    //    int q;
    //    for (q = 0; q < 100; q++) asm("nop");
    int t = FRT_GetMilliSeconds();
    while (1) {
        if (FRT_GetMilliSeconds() - t >= 100) {
            t = FRT_GetMilliSeconds();
            //            printf("ERROR\r\n");
            if (dataReadStatus == ERROR) {
                printf("\r\nERROR\r\n");
                printf("%d %f, %f, %f, %f, %f, %f,\n", FRT_GetMilliSeconds(), gyroX, gyroY, gyroZ, accelX, accelY, accelZ);
            } else {

                printf("%d %f, %f, %f, %f, %f, %f,\n", FRT_GetMilliSeconds(), gyroX, gyroY, gyroZ, accelX, accelY, accelZ);
            }
        }
    }
    return 1;
}

#endif