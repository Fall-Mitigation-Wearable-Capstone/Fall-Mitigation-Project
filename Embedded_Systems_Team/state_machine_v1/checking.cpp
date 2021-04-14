#include "Arduino.h"
#include "checking.h"


// Initialize the pins for the battery, leds, and touch touch sensors
// To be called at the beginning of the state machine file
Checking::Checking()
{
  pinMode(BATTERY_PIN, INPUT);
  pinMode(TOUCH_LEFT_PIN, INPUT);
  pinMode(TOUCH_RIGHT_PIN, INPUT);
  pinMode(LOW_BATTERY_LED, OUTPUT);
  pinMode(MID_BATTERY_LED, OUTPUT);
  pinMode(HIGH_BATTERY_LED, OUTPUT);
}

// Get the battery level by reading the AD pin
// *****NOTE: Filtering might be needed later******
// Returns ERROR if the battery is too low. SUCCESS if the battery is still usable
// The batteryLevel gets stored in a private variable that can be used in other functions
int Checking::getBatteryLevel()
{
  batteryLevel = analogRead(BATTERY_PIN);
  if(batteryLevel <= BATTERY_LOW) return ERROR;
  return SUCCESS;
}

// Set the led indicator lights depending on the battery level
// ****NOTE: Exact values for the levels need to be determined****
// 1 led corresponds to a "battery full state" (100% -> 95%)
// 1 led corresponsds to a "battery medium state" (95% -> 85%)
// 1 led corresponds to a "battery low state" (85% -> 0%)
void Checking::setBatteryLights()
{
  if(batteryLevel >= BATTERY_HIGH)
  {
    digitalWrite(LOW_BATTERY_LED, LOW);
    digitalWrite(MID_BATTERY_LED, LOW);
    digitalWrite(HIGH_BATTERY_LED, HIGH);
  }
  else if(batteryLevel < BATTERY_HIGH && batteryLevel > BATTERY_LOW)
  {
    digitalWrite(LOW_BATTERY_LED, LOW);
    digitalWrite(MID_BATTERY_LED, HIGH);
    digitalWrite(HIGH_BATTERY_LED, LOW);
  }
  else if(batteryLevel <= BATTERY_LOW)
  {
    digitalWrite(LOW_BATTERY_LED, HIGH);
    digitalWrite(MID_BATTERY_LED, LOW);
    digitalWrite(HIGH_BATTERY_LED, LOW);
  }
}

// Function gets called while the wearable is charging
// Battery indicator lights get set depending on the interrupt reading
// Returns SUCCESS if battery is full, else returns ERROR
int Checking::checkBatteryCharging()
{
  //getBatteryLevel will be called in an interrupt so no need to call it here
  getBatteryLevel(); //temporary call here

  setBatteryLights();
  if(batteryLevel == BATTERY_FULL) return SUCCESS;
  return ERROR;
}

void Checking::getTouchSensorReading()
{
  rightTouchState = digitalRead(TOUCH_RIGHT_PIN);
  leftTouchState = digitalRead(TOUCH_LEFT_PIN);
}

int Checking::checkForUser()
{
  getTouchSensorReading();
  if(rightTouchState && leftTouchState) return SUCCESS;
  return ERROR;
}
