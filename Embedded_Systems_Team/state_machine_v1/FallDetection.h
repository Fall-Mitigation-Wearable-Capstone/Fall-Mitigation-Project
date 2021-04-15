#ifndef FallDetection_h
#define FallDetection_h

#include <SparkFunMPU9250-DMP.h>
#include <MadgwickAHRS.h>
#include "Arduino.h"
#include "checking.h"

#define INTERRUPT_PIN 22

#define TWO_HUNDRED_MS 31
#define DEBOUNCE 16
#define FORWARD 0b0001
#define BACKWARDS 0b0010
#define LEFT 0b0100
#define RIGHT 0b1000

class FallDetection {
	private:
		MPU9250_DMP imu;
		Madgwick filter;

		//buffers for tracking relevant data
		float rollBuffer[TWO_HUNDRED_MS];
		float pitchBuffer[TWO_HUNDRED_MS];
		float gyroXBuffer[TWO_HUNDRED_MS];
		float gyroYBuffer[TWO_HUNDRED_MS];

		//buffer indexing
		int bufferIndex;

		//flags for tracking debouncing
		int forwardFlag;
		int backFlag;
		int leftFlag;
		int rightFlag;

		//values for change in each euler angle after 0.2s
		float diffRoll;
		float diffPitch;
    
    // Functions to get the imu data into easier variables and check the data
//    void extractIMUData(void);
    void calculateEulers(void);
    void printIMUData(void);

	public:
 //values read off of the imu. public so they can be printed
    float accelX;
    float accelY;
    float accelZ;
    float gyroX;
    float gyroY;
    float gyroZ;
    float yaw;
    float pitch;
    float roll;
    
		FallDetection(void);		//who am I test occurs here
		void updateData(void);    //Edited this to have no parameters
		void updateFlags(void);
		int detectFalls(void);      //Edited this function to have no parameters
		int getIMUData(void);		// Uses SparkfunMPU9250-DMP library. If data is not ready, return ERROR
		int initIMU(void);
    void extractIMUData(void);
};

#endif
