#ifndef FallDetection_h
#define FallDetection_h

#include "Arduino.h"

#define TWO_HUNDRED_MS 31
#define DEBOUNCE 16
#define FORWARD 0b0001
#define BACKWARDS 0b0010
#define LEFT 0b0100
#define RIGHT 0b1000

class FallDetection {
	private:
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

	public:
		FallDetection(void);		//who am I test occurs here
		void updateData(float pitch, float roll, float gyroX, float gyroY);
		void updateFlags(void);
		int detectFalls(float pitch, float roll, float gyroX, float gyroY);
		void getIMUData();		// Uses SparkfunMPU9250-DMP library. If data is not ready, return ERROR
};

#endif
