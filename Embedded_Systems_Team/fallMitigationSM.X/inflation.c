/* ************************************************************************** */
/** Fall Injury Mitigation Wearable Team
 * UCSC ECE Senior Capstone 2020-21
 
 * File name: inflation.c
 
 * File description: This is the c file for the inflation control functions.
 * These functions control the inflatable portion of the wearable. 
 
 * Author: Archisha Sinha
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
#include "inflation.h"
#include "ADC.h"
#include "BOARD.h"
/* ************************************************************************** */
/* Private Variables and Functions                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/* Section: Library Functions                                                 */
/* ************************************************************************** */

/* 
Function: inflation_Init
Param: none
Return: none
Brief: Initializes the inflation/deflation pump pins and the pressure sensor AD pins
 */
void inflation_Init(void) {
    //Set inflation and deflation control pins as output
    TRISDbits.TRISD6 = 0; //Inflation pin
    TRISDbits.TRISD7 = 0; //Deflation pin

    //Make sure inflation pins are turned off
    INFLATION_RIGHT_CONTROL_PIN = 0;
    INFLATION_LEFT_CONTROL_PIN = 0;

    //Set pins for pressure sensors as analog inputs
    AD1PCFGbits.PCFG4 = 0; //Configure pin for AD1 as analog input
    TRISBbits.TRISB4 = 1;
    AD1PCFGbits.PCFG8 = 0; //Configure pin for AD2 as analog input
    TRISBbits.TRISB8 = 1;
}

/* 
Function: inflation_inflate
Param: none
Return: none
Brief: Sends signal to turn on the inflation pumps
 */
void inflation_inflate(void) {
    INFLATION_RIGHT_CONTROL_PIN = 1;
    INFLATION_LEFT_CONTROL_PIN = 1;
}

/* 
Function: inflation_deflate
Param: none
Return: none
Brief: Sends signal to turn off the inflation pumps
 */
void inflation_deflate(void) {
    INFLATION_RIGHT_CONTROL_PIN = 0;
    INFLATION_LEFT_CONTROL_PIN = 0;
}

/* 
Function: inflation_pressureCheck
Param: none
Return: SUCCESS if pressure is ok, ERROR if pressure is too low
Brief: Reads the pressure of the front and back and checks if they are too low
 */
int inflation_pressureCheck(void) {
    //Read front and back pressure sensors
    frontPressure = ADC_CurrentReading(FRONT_PRESSURE_PIN);
    backPressure = ADC_CurrentReading(BACK_PRESSURE_PIN);

    //If the pressure on the front of the back is lower than the min requirement, an error has occurred
    if (frontPressure < MIN_PRESSURE_FRONT || backPressure < MIN_PRESSURE_BACK) {
        return ERROR;
    }
    return SUCCESS;
}

/* 
Function: inflation_deflate
Param: none
Return: none
Brief: Sends signal to turn off all pumps
 */
void inflation_resetPumps(void) {
    INFLATION_RIGHT_CONTROL_PIN = 0;
    INFLATION_LEFT_CONTROL_PIN = 0;
}

/* ************************************************************************** */
/* Section: Test main for pressure sensors                                    */
/* ************************************************************************** */
//#define TEST_PRESSURE_MAIN
#ifdef TEST_PRESSURE_MAIN
#include "FRT.h"

int main(void) {
    BOARD_Init();
    FRT_Init();
    ADC_Init();
    inflation_Init();

    printf("Testing pressure sensors in inflation library\r\n");
    int pressure = 0;
    unsigned int pT = FRT_GetMilliSeconds();
    while (1) {
        if (FRT_GetMilliSeconds() - pT >= 10) {
            pressure = inflation_pressureCheck();
            printf("Front pressure: %d  Back pressure: %d\r\n", frontPressure, backPressure);
            printf("Is pressure too low(1 = no, 0 = yes): %d\r\n", pressure);
        }
    }
    return (1);
}
#endif

/* ************************************************************************** */
/* Section: Test main                                                         */
/* ************************************************************************** */
//#define TEST_INFLATION_MAIN
#ifdef TEST_INFLATION_MAIN
#include "FRT.h"

int main(void) {
    BOARD_Init();
    FRT_Init();
    ADC_Init();
    inflation_Init();

    printf("Testing inflation library\r\n");
    int pressure = 0;

    //Inflate fully. Check if full inflation occurs in 80ms
    inflation_inflate();
    unsigned int pT = FRT_GetMilliSeconds();
    unsigned int inflationTime = FRT_GetMilliSeconds();
    while (FRT_GetMilliSeconds() - inflationTime <= 80) {
        if (FRT_GetMilliSeconds() - pT >= 10) {
            pT = FRT_GetMilliSeconds();
            printf("Initial inflation occurring\r\n");
        }
    }

    //Maintain full inflation. Check if 30s is needed here
    inflationTime = FRT_GetMilliSeconds();
    pT = FRT_GetMilliSeconds();
    while (FRT_GetMilliSeconds() - inflationTime <= 30000) {
        if (FRT_GetMilliSeconds() - pT >= 10) {
            pT = FRT_GetMilliSeconds();
            printf("Maintaining inflation\r\n");
        }
        pressure = inflation_pressureCheck();
        if (pressure == ERROR) {
            printf("Pressure Low\r\n");
        }
    }

    //Deflate wearable. Check if deflation pump needed and for how long.
    inflation_deflate();
    printf("Deflating\r\n");

    return (1);
}
#endif