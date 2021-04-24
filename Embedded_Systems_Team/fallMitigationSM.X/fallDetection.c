/* ************************************************************************** */
/** Fall Injury Mitigation Wearable Team
 * UCSC ECE Senior Capstone 2020-21
 
 * File name: fallDetection.c
 
 * File description: This is the c file for the fall detection algorithm.
 * These functions read the IMU and check if the incoming data signals whether a 
 * fall or activity of daily life (ADL) has occurred. 
 
 * Author: Archisha Sinha and David Prager
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BOARD.h"
#include "MADGWICK.h"
#include "MPU9250.h"
#include "fallDetection.h"

/* ************************************************************************** */
/* Private Constants and Variables                                            */
/* ************************************************************************** */
#define TWO_HUNDRED_MS 31
#define DEBOUNCE 16 
#define FORWARD 0b0001
#define BACKWARDS 0b0010
#define LEFT 0b0100
#define RIGHT 0b1000

static volatile float rollBuffer[TWO_HUNDRED_MS];
static volatile float pitchBuffer[TWO_HUNDRED_MS];
static volatile float gyroXBuffer[TWO_HUNDRED_MS];
static volatile float gyroYBuffer[TWO_HUNDRED_MS];

//buffer indexing
static int bufferIndex;

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
Brief: Initializes the battery and touch sensor pins
 */
void fallDetection_Init(void) {
    static float clearData[TWO_HUNDRED_MS];
    memcpy(rollBuffer, clearData, TWO_HUNDRED_MS);
    memcpy(pitchBuffer, clearData, TWO_HUNDRED_MS);
    memcpy(gyroXBuffer, clearData, TWO_HUNDRED_MS);
    memcpy(gyroYBuffer, clearData, TWO_HUNDRED_MS);

    bufferIndex = 0;

    forwardFlag = 0;
    backFlag = 0;
    leftFlag = 0;
    rightFlag = 0;

    diffRoll = 0;
    diffPitch = 0;
}

/* 
Function: fallDetection_updateData
Param: none
Return: SUCCESS if battery is usable, ERROR if battery is too low
Brief: Reads the battery level
 */
void fallDetection_updateData(float pitch, float roll, float gyroX, float gyroY) {
    //update index and consider roll-over
    bufferIndex++;
    if (bufferIndex == TWO_HUNDRED_MS) bufferIndex = 0;

    //update the individual buffers
    rollBuffer[bufferIndex] = roll;
    pitchBuffer[bufferIndex] = pitch;
    gyroXBuffer[bufferIndex] = gyroX;
    gyroYBuffer[bufferIndex] = gyroY;

    //update differential euler values
    int tempIndex = (bufferIndex % TWO_HUNDRED_MS) + 1;
    diffRoll = roll - rollBuffer[tempIndex];
    diffPitch = pitch - pitchBuffer[tempIndex];
}

/*
 Function: fallDetection_updateFlags
 Param: none
 Return: none
 Brief: Set LED lights to indicate the battery level
 */
void fallDetection_updateFlags(void) {
    //forward detection flag
    if (diffRoll <= -5 && gyroXBuffer[bufferIndex] <= -60) {
        if (forwardFlag < 16) {
            forwardFlag++;
        }
    } else {
        if (forwardFlag > 0) {
            forwardFlag--;
        }
    }

    if (rollBuffer[bufferIndex] > 60 && diffRoll > 8 && gyroXBuffer[bufferIndex] > 23) {
        if (backFlag < 16) {
            backFlag++;
        }
    } else {
        if (backFlag > 0) {
            backFlag--;
        }
    }

    if (diffPitch < -6) {
        if (leftFlag < 16) {
            leftFlag++;
        }
    } else {
        if (leftFlag > 0) {
            leftFlag--;
        }
    }

    if (diffPitch > 6) {
        if (rightFlag < 16) {
            rightFlag++;
        }
    } else {
        if (rightFlag > 0) {
            rightFlag--;
        }
    }
}

/*
 Function: fallDetection_detectFalls
 Param: none
 Return: SUCCESS if battery is full, ERROR if not full
 Brief: Reads the level of the battery while it is charging
 */
int fallDetection_detectFalls(float pitch, float roll, float gyroX, float gyroY) {
    fallDetection_updateData(pitch, roll, gyroX, gyroY);
    fallDetection_updateFlags();

    int out = 0;
    if (forwardFlag >= 16) out |= FORWARD;
    if (backFlag >= 16) out |= BACKWARDS;
    if (leftFlag >= 16) out |= LEFT;
    if (rightFlag >= 16) out |= RIGHT;
    return out;
}
