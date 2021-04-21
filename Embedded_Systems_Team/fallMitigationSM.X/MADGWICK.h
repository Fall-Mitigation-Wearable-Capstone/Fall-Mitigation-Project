#ifndef MADGWICK_H
#define MADGWICK_H

#include <math.h>

typedef struct Madgwick {
	float beta; // algorithm gain
    float q0;
    float q1;
    float q2;
    float q3;	// quaternion of sensor frame relative to auxiliary frame
    float invSampleFreq;
    float roll;
    float pitch;
    float yaw;
    char anglesComputed;
} Madgwick;

static float invSqrt(float x);
void computeAngles(Madgwick *filter);

struct Madgwick* begin(float sampleFrequency);
void update(float gx, float gy, float gz, float ax, float ay, float az, Madgwick *filter);

float getRoll(Madgwick *filter); 
float getPitch(Madgwick *filter); 
float getYaw(Madgwick *filter);

#endif MADGWICK_H
