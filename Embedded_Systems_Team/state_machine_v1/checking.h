#ifndef Checking_h
#define Checking_h

#include "Arduino.h"

#define SUCCESS 1
#define ERROR 0

#define BATTERY_PIN 34
#define TOUCH_LEFT_PIN 36
#define TOUCH_RIGHT_PIN 35

#define LOW_BATTERY_LED 5
#define MID_BATTERY_LED 17
#define HIGH_BATTERY_LED 16

#define BATTERY_LOW 2750
#define BATTERY_MEDIUM 3000     // value needs to be calculated
#define BATTERY_HIGH 3300
#define BATTERY_FULL 3600

class Checking
{
  public:
    Checking();
    int getBatteryLevel();
    int checkForUser();
    void setBatteryLights();
    int checkBatteryCharging();
  private:
    int batteryLevel;
    int rightTouchState;
    int leftTouchState;
    int touchSensorState;

    void getTouchSensorReading();
    void IRAM_ATTR batteryInterrupt();
};

#endif
