/* ************************************************************************** */
/** Fall Injury Mitigation Wearable Team
 * UCSC ECE Senior Capstone 2020-21
 
 * File name: I2C.c
 
 * File description: This is the c file for the I2C peripheral. I2C is used
 * communicate with the MPU9250 IMU.
 
 * Author: Archisha Sinha and David Prager
 */
/* ************************************************************************** */


/* ************************************************************************** */
/* Included Files                                                             */
/* ************************************************************************** */
#include <stdio.h>
#include <stdlib.h>
#include "sys/attribs.h"
#include <xc.h>
#include "BOARD.h"
#include "I2C.h"

/* ************************************************************************** */
/* Constants                                                                  */
/* ************************************************************************** */
#define MPU_ADDRESS 0x68

/* ************************************************************************** */
/* Variables                                                                  */
/* ************************************************************************** */

/* 
 * Function: I2C_Init
 * Param: none
 * Return: none
 * Brief: Initializes the I2C peripheral
 */
void I2C_Init(void) {
    I2C1CONbits.ON = 0;

    // Setting the frequency of the I2C to 100kHz (lowest SCL clock frequency)
    //[PB_Clock / (2 * 100kHz)] - 2
    I2C1BRG = 0x00C6;

    I2C1CONbits.ON = 1;
    I2C1CONbits.PEN = 1;
    while (I2C1CONbits.PEN == 1);
}

/* 
 * Function: I2C_read
 * Param: deviceAddress = 8 bit device register address
 * Return: value at register or ERROR
 * Brief: Reads incoming data from the given register address
 */
unsigned char I2C_read(unsigned char deviceAddress) {
    unsigned char dataRead = ERROR;             //Initialize return value to ERROR in case no data is read
    
    I2C1CONbits.SEN = 1;                        //Initiate START condition on SDA & SCL pins, automatically set to Idle 
    while(I2C1CONbits.SEN == 1);                //Wait for START to be sent
    
    I2C1TRN = MPU_ADDRESS << 1;                 //MPU address shifted to hold 1 bit of R/W = 0
    while(I2C1STATbits.TRSTAT != 0);            //Wait for transmission and another clock cycle for ACK/NACK to occur
    if(I2C1STATbits.ACKSTAT == 1){              //If ACK not received
        I2C1CONbits.PEN = 1;                    //Initiate STOP condition on SDA & SCL pins, automatically set to Idle
        while(I2C1CONbits.PEN == 1);            //Wait for STOP to be sent
        return ERROR;                           //Error out
    };
    
    I2C1TRN = deviceAddress;                    //Send 8 bit device address
    while(I2C1STATbits.TRSTAT != 0);            //Wait for transmission and another clock cycle for ACK/NACK to occur
    if(I2C1STATbits.ACKSTAT == 1){              //If ACK not received
        I2C1CONbits.PEN = 1;                    //Initiate STOP condition on SDA & SCL pins, automatically set to Idle
        while(I2C1CONbits.PEN == 1);            //Wait for STOP to be sent
        return ERROR;                           //Error out
    };
    
    I2C1CONbits.RSEN = 1;                       //Initiate REPEATED START condition on SDA & SCL pins, automatically set to Idle 
    while(I2C1CONbits.RSEN == 1);               //Wait for REPEATED START to be sent

    I2C1TRN = (MPU_ADDRESS << 1) + 1;           //EEPROM address shifted to hold 1 bit of R/W = 1
    while(I2C1STATbits.TRSTAT != 0);            //Wait for transmission and another clock cycle for ACK/NACK to occur
    if(I2C1STATbits.ACKSTAT == 1){              //If ACK not received
        I2C1CONbits.PEN = 1;                    //Initiate STOP condition on SDA & SCL pins, automatically set to Idle
        while(I2C1CONbits.PEN == 1);            //Wait for STOP to be sent
        return ERROR;                           //Error out
    };
    
    I2C1CONbits.RCEN = 1;                       //Set I2C to receive mode
    int d = 0;
    for(d = 0; d < 100; d++) asm("nop");
    while(I2C1STATbits.RBF != 1);
    dataRead = I2C1RCV;                         //Read data from buffer
    
    I2C1CONbits.PEN = 1;                        //Initiate STOP condition on SDA & SCL pins, automatically set to Idle
    while(I2C1CONbits.PEN == 1);                //Wait for STOP to be sent
    
    return dataRead;
}

/* 
 * Function: I2C_write
 * Param: deviceAddress = 8 bit device register address
 * Param: data = information to be written to the register
 * Return: SUCCESS or ERROR
 * Brief: Writes data to the given register address
 */
unsigned char I2C_write(unsigned char deviceAddress, char data){
    I2C1CONbits.SEN = 1;                    //Initiate START condition on SDA & SCL pins, automatically set to Idle 
    while(I2C1CONbits.SEN);                 //Wait for START to be sent

    I2C1TRN = MPU_ADDRESS << 1;             //EEPROM address shifted to hold 1 bit of R/W = 0
    while(I2C1STATbits.TRSTAT);             //Wait for transmission and another clock cycle for ACK/NACK to occur
    if(I2C1STATbits.ACKSTAT){               //If ACK not received
        I2C1CONbits.PEN = 1;                //Initiate STOP condition on SDA & SCL pins, automatically set to Idle
        while(I2C1CONbits.PEN);             //Wait for STOP to be sent
        return ERROR;                       //Error out
    };
    
    I2C1TRN = deviceAddress;                //Send 8 bit device address
    while(I2C1STATbits.TRSTAT);             //Wait for transmission and another clock cycle for ACK/NACK to occur
    if(I2C1STATbits.ACKSTAT){               //If ACK not received
        I2C1CONbits.PEN = 1;                //Initiate STOP condition on SDA & SCL pins, automatically set to Idle
        while(I2C1CONbits.PEN);             //Wait for STOP to be sent
        return ERROR;                       //Error out
    };
    
    I2C1TRN = data;                         //Send upper 8 bits of address
    while(I2C1STATbits.TRSTAT);             //Wait for transmission and another clock cycle for ACK/NACK to occur
    if(I2C1STATbits.ACKSTAT){               //If ACK not received
        I2C1CONbits.PEN = 1;                //Initiate STOP condition on SDA & SCL pins, automatically set to Idle
        while(I2C1CONbits.PEN);             //Wait for STOP to be sent
        return ERROR;                       //Error out
    };
    
    I2C1CONbits.PEN = 1;                    //Initiate STOP condition on SDA & SCL pins, automatically set to Idle
    while(I2C1CONbits.PEN);                 //Wait for STOP to be sent

    return SUCCESS;
}

/* 
 * Function: I2C_readInteger
 * Param: deviceAddress = 8 bit device register address
 * Return: data read or ERROR
 * Brief: Reads 2 sequential registers to create a 16 bit value of data
 */
int I2C_readInteger(unsigned char deviceAddress){
    short data = IMU_ERROR;                           //Initialized to ERROR in case no data is read
    I2C1CONbits.SEN = 1;                        //Initiate START condition on SDA & SCL pins, automatically set to Idle 
    while(I2C1CONbits.SEN == 1);                //Wait for START to be sent
    
    I2C1TRN = MPU_ADDRESS << 1;                 //EEPROM address shifted to hold 1 bit of R/W = 0
    while(I2C1STATbits.TRSTAT != 0);            //Wait for transmission and another clock cycle for ACK/NACK to occur
    if(I2C1STATbits.ACKSTAT == 1){              //If ACK not received
        I2C1CONbits.PEN = 1;                    //Initiate STOP condition on SDA & SCL pins, automatically set to Idle
        while(I2C1CONbits.PEN == 1);            //Wait for STOP to be sent
        return ERROR;                           //Error out
    };
    
    I2C1TRN = deviceAddress;                    //Send 8 bit device address
    while(I2C1STATbits.TRSTAT != 0);            //Wait for transmission and another clock cycle for ACK/NACK to occur
    if(I2C1STATbits.ACKSTAT == 1){              //If ACK not received
        I2C1CONbits.PEN = 1;                    //Initiate STOP condition on SDA & SCL pins, automatically set to Idle
        while(I2C1CONbits.PEN == 1);            //Wait for STOP to be sent
        return ERROR;                           //Error out
    }; 
    
    I2C1CONbits.RSEN = 1;                       //Initiate REPEATED START condition on SDA & SCL pins, automatically set to Idle 
    while(I2C1CONbits.RSEN == 1);               //Wait for REPEATED START to be sent

    I2C1TRN = (MPU_ADDRESS << 1) + 1;           //EEPROM address shifted to hold 1 bit of R/W = 1
    while(I2C1STATbits.TRSTAT != 0);            //Wait for transmission and another clock cycle for ACK/NACK to occur
    if(I2C1STATbits.ACKSTAT == 1){              //If ACK not received
        I2C1CONbits.PEN = 1;                    //Initiate STOP condition on SDA & SCL pins, automatically set to Idle
        while(I2C1CONbits.PEN == 1);            //Wait for STOP to be sent
        return ERROR;                           //Error out
    };
    
    I2C1CONbits.RCEN = 1;                       //Set I2C to receive mode
    int d = 0;
    while(I2C1STATbits.RBF != 1){
//        printf("h");
        d++;
        if(d > 500000000){
//            printf("t\r\n");
            return ERROR;
        }
    }
    d = 0;
    data = I2C1RCV << 8;                        //Read data from buffer. Data is big Endian. Shift to add bottom 8 bits in next read

    I2C1CONbits.ACKEN = 1;                      //Transmit ACKDT data
    while(I2C1CONbits.ACKEN == 1);              //Wait for NACK to be sent
    
    I2C1CONbits.RCEN = 1;                       //Set I2C to receive mode
    while(I2C1STATbits.RBF != 1);
    data |= I2C1RCV;                             //Read data from buffer. Data is big Endian and is added to bottom 8 bits of data value
    
    // Unsure if changes actually needed here
    I2C1CONbits.ACKDT = 1;                      //ACK is sent
    I2C1CONbits.ACKEN = 1;                      //Transmit ACKDT data
    while(I2C1CONbits.ACKEN == 1);              //Wait for NACK to be sent
    
    I2C1CONbits.ACKDT = 0;                      //NACK is sent
    I2C1CONbits.ACKEN = 1;                      //Transmit ACKDT data
    while(I2C1CONbits.ACKEN == 1);              //Wait for NACK to be sent
    
    I2C1CONbits.PEN = 1;                        //Initiate STOP condition on SDA & SCL pins, automatically set to Idle
    while(I2C1CONbits.PEN == 1);                //Wait for STOP to be sent
    
    return data;
}