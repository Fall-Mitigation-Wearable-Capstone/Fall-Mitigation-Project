#ifndef Inflation_h
#define Inflation_h

#include "Arduino.h"

class Inflation
{
  public:
    Inflation(int pin);
    int get_input(int val);
  private:
    int pin1;
};

#endif
