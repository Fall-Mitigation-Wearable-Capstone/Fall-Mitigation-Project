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
//#define MAIN/
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
void initTestLeds(void); //Initialize LEDs for testing the system
/* ************************************************************************** */
/* Local Variables                                                            */
/* ************************************************************************** */
//Time in milliseconds for all situations
#define CALIBRATION_TIME 5000
#define DATA_READ_ERROR_TIME 20
#define INFLATE_FULLY_TIME 80
#define MAINTAIN_INFLATION_TIME 10000
#define DEFLATION_TIME 1000
#define ERROR_TIME 10000

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
    CALIBRATE, //Beginning of super state 2
    DETECT_FALLS,
    IMU_ERROR, //End of super state 2
    INFLATE_TO_100, //Beginning of super state 3
    MAINTAIN_FULL_INFLATION,
    INFLATION_ERROR,
    DEFLATE_FULLY //End of super state 3
};
enum Substate sub = START;

static unsigned int prevDataReadTime; //Used to keep track of data reading times to detect errors
static unsigned int prevTime; //Variable to track time intervals

/* ************************************************************************** */
/* Section: Main Loop                                                         */

/* ************************************************************************** */
int main(void) {
    //This state machine switches between the superstates
    while (1) {
        switch (super) {
            case CHECK_USABILITY:
                checkUsability();
                break;

            case DETECT_MOVEMENT:
                detectMovement();
                break;

            case INFLATE_WEARABLE:
                inflateWearable();
                break;
        }
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
            //Initialize all systems here
            BOARD_Init();
            ADC_Init();
            FRT_Init();
            checking_Init();

            initTestLeds(); //For testing

            sub = CHECK_FOR_USER;
            break;

        case CHECK_FOR_USER:
            //Check if user has put the jacket on
            if (checking_checkForUser() == SUCCESS) {
                LATFbits.LATF1 = 1;
                //Initialize remaining hardware
                inflation_Init();

                if (MPU9250_Init() == ERROR) {
                    printf("Error with sensor\r\n"); //Failed IMU Initialization
                    sub = IMU_ERROR;
                } else {
                    sub = CHECK_BATTERY_LEVEL; //User ready    
                }
            } else {
                sub = CHECK_FOR_USER; //No user
            }
            break;

        case CHECK_BATTERY_LEVEL:
            checking_setBatteryLevelLights(); //Set lights for battery indicators

            //Check if battery level is too low
            if (batteryLevel < BATTERY_LOW) {
                sub = LOW_BATTERY; //Battery not ready
            } else {
                super = DETECT_MOVEMENT; //Battery ready
                sub = CALIBRATE;
                prevTime = FRT_GetMilliSeconds();
            }
            break;

        case LOW_BATTERY:
//            printf("low battery (User takes off jacket)\r\n");

            checking_flashBatteryLight(); //Indicate the battery is too low for usage
            //Charge the battery
            if (batteryLevel < BATTERY_LOW) {
                BOARD_End(); //Shut down system
                sub = START; //Battery charged
            } else {
                sub = LOW_BATTERY; //Battery charging
            }
            break;
    }
}

void detectMovement(void) {
    switch (sub) {
        case CALIBRATE:
//            printf("Calibrating\r\n");

            if (checking_checkForUser() == ERROR) { //Is user still wearing jacket?
                LATFbits.LATF1 = 0;
                BOARD_End(); //Shut down system
                super = CHECK_USABILITY; //No user
                sub = START;
            } else if (batteryLevel < BATTERY_LOW) { //Is battery usable?
                super = CHECK_USABILITY; //Battery not ready
                sub = LOW_BATTERY;
            } else {
                //Calibrate IMU for 5 seconds
                while (FRT_GetMilliSeconds() - prevTime < CALIBRATION_TIME) {
                    fallDetection_updateData(getPitch(), getRoll(), gyroX, gyroY);
                }

                checking_setBatteryLevelLights();
                printf("Done Calibrating\r\n");
                sub = DETECT_FALLS;
            }
            break;

        case DETECT_FALLS:
            printf("Detecting falls\r\n");

            //Checks if data was read properly
            if (checking_checkForUser() == ERROR) { //Is user still wearing jacket?
                LATFbits.LATF1 = 0;
                BOARD_End(); //Shut down system
                super = CHECK_USABILITY; //No user
                sub = START;
            } else if (batteryLevel < BATTERY_LOW) { //Is battery usable?
                super = CHECK_USABILITY; //Battery not ready
                sub = LOW_BATTERY;
            } else if (dataReadStatus == ERROR) {
                /*If data has not been read for 20ms, reading frequency has dropped
                 * below 100Hz which is too slow for accurate fall detection. */
                printf("Error occurred with read\r\n");
                if (FRT_GetMilliSeconds() - prevDataReadTime >= DATA_READ_ERROR_TIME) {
                    sub = IMU_ERROR; //Failed read
                }
            } else {
                prevDataReadTime = FRT_GetMilliSeconds(); //Update time of good data read 

                //Go through fall detection algorithm process
                fallDetection_updateData(getPitch(), getRoll(), gyroX, gyroY); //Update data buffers and euler angle slopes
                fallDetection_updateFlags(); //Compare new data with fall thresholds
                fall = fallDetection_detectFalls(); //Determine if a fall was detected

                printf("%.2f %.2f %.2f %.2f\r\n", diffRoll, gyroX, diffPitch, gyroY);

                //If a fall is detected
                if (fall != 0) {
                    LATFbits.LATF2 = 1;
                    if (fall & FORWARD) printf("F");
                    if (fall & BACKWARDS) printf("B");
                    if (fall & LEFT) printf("L");
                    if (fall & RIGHT) printf("R");
                    printf("\r\n");

                    sub = INFLATE_TO_100;
                    prevTime = FRT_GetMilliSeconds();
                    break;
                } else {
                    sub = DETECT_FALLS; //ADL detected
                }
            }
            checking_setBatteryLevelLights();
            break;

        case IMU_ERROR:
            checking_errorLeds();
            prevTime = FRT_GetMilliSeconds();
            while(FRT_GetMilliSeconds() - prevTime <= ERROR_TIME);
            BOARD_End(); //Shut down system
            printf("detecting IMU ERROR\r\n");
            printf("STOPPING SYSTEM (Reset ESP to continue testing)\r\n");
            break;
    }
}

void inflateWearable(void) {
    switch (sub) {
        case INFLATE_TO_100:
            printf("inflating 100\r\n");

            //Inflate the wearable fully. Should take 80ms
            inflation_inflate();
            while (FRT_GetMilliSeconds() - prevTime <= INFLATE_FULLY_TIME) {
                sub = INFLATE_TO_100; //Time to inflate not done
            }
            
            LATFbits.LATF2 = 0;
            LATFbits.LATF3 = 1;
            prevTime = FRT_GetMilliSeconds(); //Reset start time to be used to track time for maintaining inflation
            sub = MAINTAIN_FULL_INFLATION; //Time to inflate done
            break;

        case MAINTAIN_FULL_INFLATION:
            printf("inflating Full\r\n");

            //Hold the inflation at full until hold time completes and pressure is in a good range
            if (inflation_pressureCheck() == ERROR) {
                sub = INFLATION_ERROR; //Low pressure
            } else if (FRT_GetMilliSeconds() - prevTime >= MAINTAIN_INFLATION_TIME) { //Inflation must be maintained for 30 seconds
                LATFbits.LATF3 = 0;
                prevTime = FRT_GetMilliSeconds();
                sub = DEFLATE_FULLY; //Hold time done
            } else {
                sub = MAINTAIN_FULL_INFLATION; //Hold time not done and correct pressure range
            }
            break;

        case INFLATION_ERROR:
            checking_errorLeds();
            prevTime = FRT_GetMilliSeconds();
            while(FRT_GetMilliSeconds() - prevTime <= ERROR_TIME);
            BOARD_End(); //Shut down system
            printf("inflating INFLATION ERROR\r\n");
            printf("STOPPING SYSTEM (Reset ESP to continue testing)\r\n");
            break;

        case DEFLATE_FULLY:
            printf("deflating Fully\r\n");

            //Deflate the wearable
            inflation_deflate();

            super = DETECT_MOVEMENT; //Deflation done
            sub = CALIBRATE;
            break;
    }
}

void initTestLeds(void) {
    TRISFbits.TRISF1 = 0;
    TRISFbits.TRISF2 = 0;
    TRISFbits.TRISF3 = 0;
}

#endif

//#define TEST_LEDS
#ifdef TEST_LEDS
#include "BOARD.h"
#include <stdio.h>
#include <stdlib.h>
#include <proc/p32mx320f128h.h>

int main(void) {
    BOARD_Init();
    TRISFbits.TRISF1 = 0;
    TRISFbits.TRISF2 = 0;
    TRISFbits.TRISF3 = 0;

    while (1) {
        LATFbits.LATF1 = 1;
        LATFbits.LATF2 = 1;
        LATFbits.LATF3 = 1;
    }
    return (1);
}
#endif