#include <MPU9250_RegisterMap.h>

/************************************************************
  MPU9250_DMP_Quaternion
  Quaternion example for MPU-9250 DMP Arduino Library
  Jim Lindblom @ SparkFun Electronics
  original creation date: November 23, 2016
  https://github.com/sparkfun/SparkFun_MPU9250_DMP_Arduino_Library

  The MPU-9250's digital motion processor (DMP) can calculate
  four unit quaternions, which can be used to represent the
  rotation of an object.

  This exmaple demonstrates how to configure the DMP to
  calculate quaternions, and prints them out to the serial
  monitor. It also calculates pitch, roll, and yaw from those
  values.

  Development environment specifics:
  Arduino IDE 1.6.12
  SparkFun 9DoF Razor IMU M0

  Supported Platforms:
  - ATSAMD21 (Arduino Zero, SparkFun SAMD21 Breakouts)
*************************************************************/
#include <SparkFunMPU9250-DMP.h>

#define INTERRUPT_PIN_MPU 19
//#define SerialPort SerialUSB

MPU9250_DMP imu;
unsigned short fifoCnt;
inv_error_t result;

const signed char orientationDefault[9] = {0, 1, 0, 0, 0, 1, 1, 0, 0};

void setup()
{
  Serial.begin(115200);

  pinMode(INTERRUPT_PIN_MPU, INPUT_PULLUP);

  if (imu.begin() != INV_SUCCESS)
  {
    while (1)
    {
      Serial.println("Unable to communicate with MPU-9250");
      Serial.println("Check connections, and try again.");
      Serial.println();
      delay(5000);
    }
  }

  imu.enableInterrupt();
  imu.setIntLevel(INT_ACTIVE_LOW);
  imu.setIntLatched(INT_LATCHED);

  imu.dmpBegin(DMP_FEATURE_6X_LP_QUAT |  // Enable 6-axis quat
                   DMP_FEATURE_GYRO_CAL, // Use gyro calibration
              10);                      // Set DMP FIFO rate to 10 Hz
  imu.dmpSetOrientation(orientationDefault);
}

void loop()
{
  if(digitalRead(INTERRUPT_PIN_MPU) == LOW){
    fifoCnt = imu.fifoAvailable();
    
    if(fifoCnt > 0){
      result = imu.dmpUpdateFifo();
      Serial.println("penis" + String(result));
      
      if(result == INV_SUCCESS){
        imu.computeEulerAngles();
      }
    }
  }
}
