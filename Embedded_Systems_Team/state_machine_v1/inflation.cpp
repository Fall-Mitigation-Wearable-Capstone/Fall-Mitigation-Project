#include "Arduino.h"
#include "inflation.h"
#include "checking.h"

// All code written with the assumption of 2 inflation pumps and 2 deflation pumps
// Each pump has 2 switches, 1 to each battery

Inflation::Inflation()
{
  pinMode(INFLATION_PUMP1, OUTPUT);
  pinMode(INFLATION_PUMP2, OUTPUT);
  pinMode(INFLATION_PUMP3, OUTPUT);
  pinMode(INFLATION_PUMP4, OUTPUT);

  pinMode(RELEASE_PUMP1, OUTPUT);
  pinMode(RELEASE_PUMP2, OUTPUT);
  pinMode(RELEASE_PUMP3, OUTPUT);
  pinMode(RELEASE_PUMP4, OUTPUT);

  pinMode(PRESSURE1, INPUT);
  pinMode(PRESSURE2, INPUT);
}

// Inflates the pockets to their maximum volume
// The air pumps turn on with a 1 signal and off with a 0 signal
// Full inflation takes 80ms per slot
void Inflation::fullInflate()
{
  digitalWrite(INFLATION_PUMP1, HIGH);
  digitalWrite(INFLATION_PUMP2, HIGH);
  digitalWrite(INFLATION_PUMP3, HIGH);
  digitalWrite(INFLATION_PUMP4, HIGH);
  delay(80);
}


void Inflation::partialDeflate()
{
  digitalWrite(RELEASE_PUMP1, HIGH);
  digitalWrite(RELEASE_PUMP2, HIGH);
  digitalWrite(RELEASE_PUMP3, HIGH);
  digitalWrite(RELEASE_PUMP4, HIGH);
  delay(2000);
  digitalWrite(RELEASE_PUMP1, LOW);
  digitalWrite(RELEASE_PUMP2, LOW);
  digitalWrite(RELEASE_PUMP3, LOW);
  digitalWrite(RELEASE_PUMP4, LOW);
}

void Inflation::fullDeflate()
{
  digitalWrite(RELEASE_PUMP1, HIGH);
  digitalWrite(RELEASE_PUMP2, HIGH);
  digitalWrite(RELEASE_PUMP3, HIGH);
  digitalWrite(RELEASE_PUMP4, HIGH);
}

void Inflation::getPressure()
{
  pressure1 = digitalRead(PRESSURE1);
  pressure2 = digitalRead(PRESSURE2);
}

int Inflation::pressureCheck()
{
  getPressure();
  if(pressure1 < MIN_PRESSURE_FRONT || pressure2 < MIN_PRESSURE_BACK){
    return ERROR;
  } else{
    return SUCCESS;
  }
}
