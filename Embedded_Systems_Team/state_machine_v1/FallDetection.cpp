#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "FallDetection.h"
#include <SparkFunMPU9250-DMP.h>
#include <MadgwickAHRS.h>

/*
#define TWO_HUNDRED_MS 31
#define DEBOUNCE 16
#define FORWARD 0b0001
#define BACKWARDS 0b0010
#define LEFT 0b0100
#define RIGHT 0b1000
*/

FallDetection::FallDetection()
{
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

  accelX = 0;
  accelY = 0;
  accelZ = 0;
  gyroX = 0;
  gyroY = 0;
  gyroZ = 0;
}

/*
Update buffers and other relevant data for detecting falls
*/
//Edited this to have no parameters
void FallDetection::updateData()
{
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
void FallDetection::updateFlags(void)
{
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
//Edited this to have no parameters
int FallDetection::detectFalls(void)
{
	updateData();
	updateFlags();

	int out = 0;
	if(forwardFlag >= 16) out |= FORWARD;
	if(backFlag >= 16) out |= BACKWARDS;
	if(leftFlag >= 16) out |= LEFT;
	if(rightFlag >= 16) out |= RIGHT;
	return out;
}

/*
 * Initializes the IMU with a sampling rate of 200Hz and a "data ready" interrupt
 * Returns SUCCESS if initialization works, ERROR if the initialization does not work
 */
int FallDetection::initIMU(void)
{
  pinMode(INTERRUPT_PIN, INPUT_PULLUP);
	if (imu.begin() != INV_SUCCESS){
		return ERROR;
	}
	imu.setSensors(INV_XYZ_GYRO | INV_XYZ_ACCEL);
	imu.setGyroFSR(250);
	imu.setAccelFSR(2);
	imu.setLPF(5);
	imu.setSampleRate(200);

  // For the interrupt
  imu.enableInterrupt();  //Interrupt outputs as a "data ready" indicator
  imu.setIntLevel(INT_ACTIVE_LOW);  //Using INT pin's internal pull-up resistor
  imu.setIntLatched(INT_LATCHED);   //Interrupt latches until data has been read2
  
	filter.begin(200);
	return SUCCESS;
}

/*
 * Reads the IMU data when the hardware interrupt indicates that data is ready
 * Returns SUCCESS if data is read and ERROR if data is not read
 */
int FallDetection::getIMUData(void)
{
  if(digitalRead(INTERRUPT_PIN) == LOW){
    imu.update(UPDATE_ACCEL | UPDATE_GYRO);
    extractIMUData();
    calculateEulers();
    return SUCCESS;
  } else{
    return ERROR;
  }
}

// Extract newest data and convert it to the correct units
void FallDetection::extractIMUData(void)
{
  // Use the calcAccel, calcGyro, and calcMag functions to
  // convert the raw sensor readings (signed 16-bit values)
  // to their respective units.
  accelX = imu.calcAccel(imu.ax);
  accelY = imu.calcAccel(imu.ay);
  accelZ = imu.calcAccel(imu.az);
  gyroX = imu.calcGyro(imu.gx);
  gyroY = imu.calcGyro(imu.gy);
  gyroZ = imu.calcGyro(imu.gz);
}

void FallDetection::calculateEulers(void)
{
  filter.updateIMU(gyroX, gyroY, gyroZ, accelX, accelY, accelZ);
  roll = filter.getRoll();
  pitch = filter.getPitch();
  yaw = filter.getYaw();
}
