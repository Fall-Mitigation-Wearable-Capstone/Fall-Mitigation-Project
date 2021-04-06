#ifndef Inflation_h
#define Inflation_h

#include "Arduino.h"

class Inflation
{
  public:
    Inflation(int air_in1, int air_in2, int air_in3, int air_in4, int air_in5, int air_in6,
              int air_out1, int air_out2, int air_out3, int air_out4, int air_out5, int air_out6,
              int pres1, int pres2);
    void full_inflate();
    void partial_deflate();
    void full_deflate();
    int pressure_check();
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
    const int pressure1;
    const int pressure2;
};

#endif
