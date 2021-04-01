#include "Arduino.h"
#include "inflation.h"

Inflation::Inflation(int pin)
{
  pin1 = pin;
}

int Inflation::get_input(int val)
{
  return val+pin1;
}
