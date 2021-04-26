/* ************************************************************************** */
/* Fall Injury Mitigation Wearable Team
 * UCSC ECE Senior Capstone 2020-21
 
 * File name: projectSM_main.c
 
 * File description: This is the main loop for the wearable. This file houses 
 * the main state machine which controls the wearable. All of the subsystems are
 * combined through software in this state machine.
 
 * Author: Archisha Sinha
 */
/* ************************************************************************** */
//#define MAIN
#ifdef MAIN
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
#include "BOARD.h"
#include "FRT.h"
#include "MADGWICK.h"
#include "MPU9250.h"
#include "checking.h"
#include "fallDetection.h"
#include "inflation.h"
#include "ADC.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/attribs.h>
#include <xc.h>
/* ************************************************************************** */
/* Function Prototypes                                                        */
/* ************************************************************************** */
void checkUsability(void); //Superstate for initial battery and user checks
void detectMovement(void); //Superstate for fall detection
void inflateWearable(void); //Superstate for inflation

/* ************************************************************************** */
/* Local Variables                                                            */
/* ************************************************************************** */
//Time in milliseconds for all situations
#define DATA_READ_ERROR_TIME 20
#define INFLATE_FULLY_TIME 80
#define MAINTAIN_INFLATION_TIME 30000
#define DEFLATION_TIME 1000

//Enum holds all super states of the state machine

enum Superstate {
    CHECK_USABILITY, //Super state 1
    DETECT_MOVEMENT, //Super state 2
    INFLATE_WEARABLE //Super state 3
};
enum Superstate super = CHECK_USABILITY;

//Enum holds all sub states for the super states in the state machine

enum Substate {
    START, //Beginning of super state 1
    CHECK_FOR_USER,
    CHECK_BATTERY_LEVEL,
    LOW_BATTERY, //End of super state 1
    READ_IMU, //Beginning of super state 2
    IMU_ERROR,
    DETECT_FALLS, //End of super state 1
    INFLATE_TO_100, //Beginning of super state 3
    MAINTAIN_FULL_INFLATION,
    INFLATION_ERROR,
    DEFLATE_FULLY //End of super state 1
};
enum Substate sub = START;

static unsigned int prevDataReadTime; //Used to keep track of data reading times to detect errors
static unsigned int inflationStartTime; //Time at which inflation begins

/* ************************************************************************** */
/* Section: Main Loop                                                         */
/* ************************************************************************** */
int main(void) {
    //This state machine switches between the superstates
    switch (super) {
        case CHECK_USABILITY:
            printf("CHECK USABILITY\r\n");
            checkUsability();
            break;

        case DETECT_MOVEMENT:
            printf("DETECT MOVEMENT\r\n");
            detectMovement();
            break;

        case INFLATE_WEARABLE:
            printf("INFLATE WEARABLE\r\n");
            inflateWearable();
            break;
    }
    return 1;
}


/* ************************************************************************** */
/* Function Definitions                                                       */
/* ************************************************************************** */
//Below are the super state state machines 

//Superstate for initial battery and user checks

void checkUsability(void) {
    switch (sub) {
        case START:
            printf("checking Start\r\n");
            //Initialize all systems here
            BOARD_Init();
            ADC_Init();
            FRT_Init();
            checking_Init();
            inflation_Init();
            if (MPU9250_Init() == ERROR) {
                printf("Error with sensor\r\n"); //Failed IMU Initialization
                sub = IMU_ERROR;
            } else {
                sub = CHECK_FOR_USER;
            }
            break;

        case CHECK_FOR_USER:
            printf("checking User\r\n");
            //Check if user has put the jacket on
            if (checking_checkForUser() == SUCCESS) {
                sub = CHECK_BATTERY_LEVEL; //User ready
            } else {
                sub = CHECK_FOR_USER; //No user
            }
            break;

        case CHECK_BATTERY_LEVEL:
            printf("checking Battery\r\n");
            //Check if battery level is too low
            if (batteryLevel < BATTERY_LOW) {
                sub = LOW_BATTERY; //Battery not ready
            } else {
                super = DETECT_MOVEMENT; //Battery ready
                sub = READ_IMU;
            }
            break;

        case LOW_BATTERY:
            printf("low battery (User takes off jacket)\r\n");
            //Charge the battery
            if (batteryLevel < BATTERY_LOW) {
                sub = START; //Battery charged
            } else {
                sub = LOW_BATTERY; //Battery charging
            }
            break;
    }
}

void detectMovement(void) {
    switch (sub) {
        case READ_IMU:
            printf("detecting Read\r\n");
            //Checks if data was read properly

            if (checking_checkForUser() == ERROR) { //Is user still wearing jacket?
                super = CHECK_USABILITY; //No user
                sub = START;
            } else if (batteryLevel < BATTERY_LOW) { //Is battery usable?
                super = CHECK_USABILITY; //Battery not ready
                sub = LOW_BATTERY;
            } else if (dataReadStatus == ERROR) {
                /*If data has not been read for 20ms, reading frequency has dropped
                 * below 100Hz which is too slow for accurate fall detection. */
                if (FRT_GetMilliSeconds() - prevDataReadTime >= DATA_READ_ERROR_TIME) {
                    sub = IMU_ERROR; //Failed read
                }
            } else {
                prevDataReadTime = FRT_GetMilliSeconds(); //Update time of good data read 
                sub = DETECT_FALLS; //Data ready
            }
            break;

        case IMU_ERROR:
            printf("detecting IMU ERROR\r\n");
            printf("STOPPING SYSTEM (Reset ESP to continue testing)\r\n");
            break;

        case DETECT_FALLS:
            printf("detecting Falls\r\n");

            update(gyroX, gyroY, gyroZ, accelX, accelY, accelZ); //Convert raw IMU data to Euler angles

            //Use the fall detection algorithm to detect falls versus ADLs
            if (checking_checkForUser() == ERROR) { //Is user still wearing jacket?
                super = CHECK_USABILITY; //No user
                sub = START;
            } else if (batteryLevel < BATTERY_LOW) { //Is battery usable?
                super = CHECK_USABILITY; //Low battery
                sub = LOW_BATTERY;
            } else if (fallDetection_detectFalls(filter.pitch, filter.roll, gyroX, gyroY) != 0) {
                inflationStartTime = FRT_GetMilliSeconds();
                super = INFLATE_WEARABLE; //Fall detected
                sub = INFLATE_TO_100;
            } else {
                sub = DETECT_FALLS; //ADL detected
            }
            break;
    }
}

void inflateWearable(void) {
    switch (sub) {
        case INFLATE_TO_100:
            printf("inflating 100\r\n");
            //Inflate the wearable fully. Should take 80ms
            inflation_inflate();
            while (FRT_GetMilliSeconds() - inflationStartTime <= INFLATE_FULLY_TIME) {
                sub = INFLATE_TO_100; //Time to inflate not done
            }
            inflationStartTime = FRT_GetMilliSeconds(); //Reset start time to be used to track time for maintaining inflation
            sub = MAINTAIN_FULL_INFLATION; //Time to inflate done
            break;

        case MAINTAIN_FULL_INFLATION:
            printf("inflating Full\r\n");
            //Hold the inflation at full until hold time completes and pressure is in a good range
            if (inflation_pressureCheck() == ERROR) {
                sub = INFLATION_ERROR; //Low pressure
            } else if (FRT_GetMilliSeconds() - inflationStartTime >= MAINTAIN_INFLATION_TIME) { //Inflation must be maintained for 30 seconds
                inflationStartTime = FRT_GetMilliSeconds();
                sub = DEFLATE_FULLY; //Hold time done
            } else {
                sub = MAINTAIN_FULL_INFLATION; //Hold time not done and correct pressure range
            }
            break;

        case INFLATION_ERROR:
            printf("inflating INFLATION ERROR\r\n");
            printf("STOPPING SYSTEM (Reset ESP to continue testing)\r\n");
            break;

        case DEFLATE_FULLY:
            printf("deflating Fully\r\n");
            //Deflate the wearable
            inflation_deflate();
            while (FRT_GetMilliSeconds() - inflationStartTime <= DEFLATION_TIME) {
                sub = DEFLATE_FULLY; //Deflation not done
            }
            inflation_resetPumps();
            super = DETECT_MOVEMENT; //Deflation done
            sub = READ_IMU;
            break;
    }
}
#endif