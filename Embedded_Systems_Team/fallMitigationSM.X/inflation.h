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
#define INFLATION_RIGHT_TOP_CONTROL_PIN LATDbits.LATD5 //IO pin for inflation control = 34
#define INFLATION_LEFT_TOP_CONTROL_PIN LATDbits.LATD11 //IO pin for inflation control = 35
#define INFLATION_RIGHT_BOTTOM_CONTROL_PIN LATDbits.LATD6 //IO pin for inflation control = 36
#define INFLATION_LEFT_BOTTOM_CONTROL_PIN LATDbits.LATD7 //IO pin for deflation control = 37
#define FRONT_PRESSURE_PIN 1 //Front pressure sensor pin = AD1
#define BACK_PRESSURE_PIN 2  //Back pressure sensor pin = AD2

#define MIN_PRESSURE -10000//762 //Minimum required pressure at full inflation. This value equals 12kPa
#define DESIRED_PRESSURE 800  //This value equals 15kPa. Used to test what pressure the inflatable actually reaches

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

#endif