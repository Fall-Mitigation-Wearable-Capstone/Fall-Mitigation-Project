/* ************************************************************************** */
/** Fall Injury Mitigation Wearable Team
 * UCSC ECE Senior Capstone 2020-21
 
 * File name: checking.c
 
 * File description: This is the c file for the initial status checking
 * functions. These functions check the status of the battery and touch sensors. 
 
 * Author: Archisha Sinha
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
#include "checking.h"
#include "ADC.h"
#include "BOARD.h"
/* ************************************************************************** */
/* Private Variables and Functions                                            */
/* ************************************************************************** */
//Values used to calculate PR2
#define TIMER2_FREQ 100
#define PRESCALAR2 8
#define PB_CLOCK_FREQUENCY 40000000

//Timer 2 isr to read battery level and touch sensors status
void __ISR(_TIMER_2_VECTOR) Timer2IntHandler(void);

int test;
/* ************************************************************************** */
/* Section: Library Functions                                                 */
/* ************************************************************************** */

/* 
Function: checkingInit
Param: none
Return: none
Brief: Initializes the battery and touch sensor pins
 */
void checking_Init(void) {
    //    TRISE = 0;
    //    LATE = 0;
    test = 0;
    //Set up AD pin for battery level
    AD1PCFGbits.PCFG10 = 0; //Configure pin 17(AD3) as analog signal
    TRISBbits.TRISB10 = 1; //Pin for battery (AD3) set as input

    //Set up IO pins for touch sensors
    TRISDbits.TRISD1 = 1; //Pin for touch sensor right (5) set as input
    TRISDbits.TRISD2 = 1; //Pin for touch sensor left (6) set as input

    //Set up IO pins for battery level LEDS
    TRISGbits.TRISG8 = 0; //Pin for "battery level los" led set as output 
    TRISGbits.TRISG7 = 0; //Pin for "battery level mid" led set as output
    TRISGbits.TRISG6 = 0; //Pin for "battery level high" led set as output

    //Set up Timer2 for reading the battery level and touch sensors at 100Hz
    T2CON = 0x0; //Stop the timer and clear the register
    T2CONbits.TCKPS = 0b011; //Select desired timer input clock prescale to 1:8
    TMR2 = 0x0; //Clear the timer register
    PR2 = PB_CLOCK_FREQUENCY/(TIMER2_FREQ * PRESCALAR2); //Load period register to rollover at 100Hz
    IFS0bits.T2IF = 0; //TMR2 flag low
    IPC2bits.T2IP = 6; //TMR2 priority set to 2nd highest in the system
    IPC2bits.T2IS = 1; //TMR2 sub priority high in the priority level
    IEC0bits.T2IE = 1; //TMR2 enabled
    T2CONbits.ON = 1;  //Turn on TMR2

}

/* 
Function: getBatteryLevel
Param: none
Return: none
Brief: Reads the battery level
 */
void checking_getBatteryLevel(void) {
    //Read battery level off of the AD pin
    batteryLevel = ADC_CurrentReading(BATTERY_PIN);
}

/*
 Function: setBatteryLevel
 Param: none
 Return: none
 Brief: Set LED lights to indicate the battery level
 */
void checking_setBatteryLevelLights(void) {
    //    LATEbits.LATE1 = 1;
    //Read current battery level
    checking_getBatteryLevel();

    //Set battery level indicators depending on battery level
    if (batteryLevel >= BATTERY_HIGH) { //If battery level is in the high range
        LOW_BATTERY_LED = 0;
        MID_BATTERY_LED = 0;
        HIGH_BATTERY_LED = 1;
    } else if (batteryLevel < BATTERY_HIGH && batteryLevel > BATTERY_LOW) { //If battery level is mid range
        LOW_BATTERY_LED = 0;
        MID_BATTERY_LED = 1;
        HIGH_BATTERY_LED = 0;
    } else if (batteryLevel <= BATTERY_LOW) { //If battery level is in the low range
        LOW_BATTERY_LED = 1;
        MID_BATTERY_LED = 0;
        HIGH_BATTERY_LED = 0;
    }
}

/*
 Function: checkBatteryCharging
 Param: none
 Return: SUCCESS if battery is full, ERROR if not full
 Brief: Reads the level of the battery while it is charging
 */
int checking_checkBatteryCharging(void) {
    //Depends on TMR2 interrupt to get the battery level while device is charging
    if (batteryLevel == BATTERY_FULL) {
        return SUCCESS;
    }
    return ERROR;
}

/*
 Function: getTouchSensorReading
 Param: none
 Return: none
 Brief: Read status of touch sensors
 */
void checking_getTouchSensorReading(void) {
    //Read IO pins for the touch sensors and store their status in global variables
    touchRightStatus = TOUCH_RIGHT_PIN;
    touchLeftStatus = TOUCH_LEFT_PIN;
}

/*
 Function: checkForUser
 Param: none
 Return: SUCCESS if both touch sensors have a high signal, ERROR if either are low
 Brief: Checks if the touch sensors signal that a user is wearing a wearable
 */
int checking_checkForUser(void) {
    //If both touch sensors signal a 1, return SUCCESS
    if (touchRightStatus && touchLeftStatus) {
        return SUCCESS;
    }
    return ERROR;
}

//Reads the battery level, sets the battery indicator lights, and reads the touch sensor levels

void __ISR(_TIMER_2_VECTOR) Timer2IntHandler(void) {
    if (IFS0bits.T2IF == 1) {
        IFS0bits.T2IF = 0; //Clear interrupt flag
        //        LATEbits.LATE2 = 1;
        checking_setBatteryLevelLights(); //Update battery level and indicator leds
        checking_getTouchSensorReading(); //Update status of both touch sensors
    }
}

/* ************************************************************************** */
/* Section: Test main                                                         */
/* ************************************************************************** */
//#define TEST_CHECKING_MAIN
#ifdef TEST_CHECKING_MAIN

#include "FRT.h"

int main(void) {
    BOARD_Init();
    checking_Init();
    ADC_Init();
    FRT_Init();

    unsigned int prev = FRT_GetMilliSeconds();

    printf("Testing checking library");
    while (1) {
        if (FRT_GetMilliSeconds() - prev >= 100) {
            prev = FRT_GetMilliSeconds();
            printf("%d\r\n", batteryLevel);
            if (batteryLevel <= BATTERY_LOW) {
                printf("BATTERY LOW\r\n");
            }
            if (batteryLevel < BATTERY_HIGH && batteryLevel > BATTERY_LOW) {
                printf("BATTERY MID\r\n");
            }
            if (batteryLevel >= BATTERY_HIGH) {
                printf("BATTERY HIGH\r\n");
            }
            if (checking_checkForUser() == ERROR) {
                printf("NO USER\r\n\r\n");
            }
            if (checking_checkForUser() == SUCCESS) {
                printf("YES USER\r\n\r\n");
            }
        }
    }
    return (1);
}

#endif