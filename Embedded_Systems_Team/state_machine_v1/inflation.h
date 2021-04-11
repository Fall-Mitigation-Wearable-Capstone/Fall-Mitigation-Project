#ifndef Inflation_h
#define Inflation_h

#include "Arduino.h"
#include "checking.h"

#define INFLATION_PUMP1 25
#define INFLATION_PUMP2 27
#define INFLATION_PUMP3 12
#define INFLATION_PUMP4 9

#define RELEASE_PUMP1 26
#define RELEASE_PUMP2 14
#define RELEASE_PUMP3 13
#define RELEASE_PUMP4 10

#define PRESSURE1 36
#define PRESSURE2 39

#define MIN_PRESSURE_BACK 10000
#define MIN_PRESSURE_FRONT 10000


class Inflation
{
public:
  Inflation();
  void fullInflate();
  void partialDeflate();
  void fullDeflate();
  void getPressure();
  int pressureCheck();
private:
  const int pump_in1;
  const int pump_in2;
  const int pump_in3;
  const int pump_in4;
  const int pump_in5;
  const int pump_in6;
  const int pump_release1;
  const int pump_release2;
  const int pump_release3;
  const int pump_release4;
  const int pump_release5;
  const int pump_release6;
  int pressure1;
  int pressure2;
};

#endif
