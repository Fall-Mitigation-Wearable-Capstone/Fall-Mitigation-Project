#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "FallDetection.h"

/*
#define TWO_HUNDRED_MS 31
#define DEBOUNCE 16 
#define FORWARD 0b0001
#define BACKWARDS 0b0010
#define LEFT 0b0100
#define RIGHT 0b1000
*/

FallDetection::FallDetection(){
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
Update buffers and other relevant data for detecting falls
*/
void FallDetection::updateData(float pitch, float roll, float gyroX, float gyroY){
    //update index and consider roll-over
    bufferIndex++;
    if(bufferIndex == TWO_HUNDRED_MS) bufferIndex = 0;
	
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
Update fall flags based on updated data
*/
void FallDetection::updateFlags(void){
    //forward detection flag
    if(diffRoll <= -5 && gyroXBuffer[bufferIndex] <= -60){
        if(forwardFlag < 16){
            forwardFlag++;
        } 
    } else {
        if(forwardFlag > 0) {
            forwardFlag--;            
        }
	}
	
	if(rollBuffer[bufferIndex] > 60 && diffRoll > 8 && gyroXBuffer[bufferIndex] > 23){
        if(backFlag < 16){
            backFlag++;
        } 
    } else {
        if(backFlag > 0) {
            backFlag--;            
        }
	}
	
	if(diffPitch < -6 && gyroYBuffer[bufferIndex] < -55){
        if(leftFlag < 16){
            leftFlag++;
        } 
    } else {
        if(leftFlag > 0) {
            leftFlag--;            
        }
	}
	
	if(diffPitch > 6 && gyroYBuffer[bufferIndex] > 55){
        if(rightFlag < 16){
            rightFlag++;
        } 
    } else {
        if(rightFlag > 0) {
            rightFlag--;            
        }
	}
}

/*
Call helper functions & and check debouncing of flags
*/
int FallDetection::detectFalls(float pitch, float roll, float gyroX, float gyroY){
	updateData(pitch, roll, gyroX, gyroY);
	updateFlags();
	
	int out = 0;
	if(forwardFlag >= 16) out |= FORWARD;
	if(backFlag >= 16) out |= BACKWARDS;
	if(leftFlag >= 16) out |= LEFT;
	if(rightFlag >= 16) out |= RIGHT;
	return out;
}
