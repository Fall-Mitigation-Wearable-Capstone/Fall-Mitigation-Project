/* ************************************************************************** */
/** Fall Injury Mitigation Wearable Team
 * UCSC ECE Senior Capstone 2020-21
 
 * File name: inflation.h
 
 * File description: This is the header file for the inflation control functions.
 * These functions control the inflatable portion of the wearable.
 
 * Author: Archisha Sinha
 */
/* ************************************************************************** */

#ifndef INFLATION_H    /* Guard against multiple inclusion */
#define INFLATION_H


/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
#include <xc.h>
#include <sys/attribs.h>   
/* ************************************************************************** */
/* Constants                                                                  */
/* ************************************************************************** */
#define INFLATION_CONTROL_PIN LATDbits.LATD6 //IO pin for inflation control = 36
#define DEFLATION_CONTROL_PIN LATDbits.LATD7 //IO pin for deflation control = 37
#define FRONT_PRESSURE_PIN 1 //Front pressure sensor pin = AD1
#define BACK_PRESSURE_PIN 2  //Back pressure sensor pin = AD2

#define MIN_PRESSURE_FRONT 500 //Minimum required pressure in the front panel. Values are based on pressure sensor readings
#define MIN_PRESSURE_BACK 800  //Minimum required pressure in the back panel. Values are based on pressure sensor readings

/* ************************************************************************** */
/* Variables                                                                  */
/* ************************************************************************** */
int frontPressure;
int backPressure;

/* ************************************************************************** */
/* Library Functions                                                          */
/* ************************************************************************** */

/* 
Function: inflation_Init
Param: none
Return: none
Brief: Initializes the inflation/deflation pump pins and the pressure sensor AD pins
 */
void inflation_Init(void);

/* 
Function: inflation_inflate
Param: none
Return: none
Brief: Sends signal to turn on the inflation pumps
 */
void inflation_inflate(void);

/* 
Function: inflation_deflate
Param: none
Return: none
Brief: Sends signal to turn on the deflation pumps
 */
void inflation_deflate(void);

/* 
Function: inflation_pressureCheck
Param: none
Return: SUCCESS if pressure is ok, ERROR if pressure is too low
Brief: Reads the pressure of the front and back and checks if they are too low
 */
int inflation_pressureCheck(void);

/* 
Function: inflation_deflate
Param: none
Return: none
Brief: Sends signal to turn off all pumps
 */
void inflation_resetPumps(void);
#endif