#include "Arduino.h"
#include "inflation.h"

Inflation::Inflation(int air_in1, int air_in2, int air_in3, int air_in4, int air_in5, int air_in6,
                    int air_out1, int air_out2, int air_out3, int air_out4, int air_out5, int air_out6,
                    int pres1, int pres2)
{
  pinMode(air_in1, OUTPUT);
  pump_in1 = air_in1;
  pinMode(air_in2, OUTPUT);
  pump_in2 = air_in2;
  pinMode(air_in3, OUTPUT);
  pump_in3 = air_in3;
  pinMode(air_in4, OUTPUT);
  pump_in4 = air_in4;
  pinMode(air_in5, OUTPUT);
  pump_in5 = air_in5;
  pinMode(air_in6, OUTPUT);
  pump_in6 = air_in6;

  pinMode(air_release1, OUTPUT);
  pump_release1 = air_out1;
  pinMode(air_release2, OUTPUT);
  pump_release2 = air_out2;
  pinMode(air_release3, OUTPUT);
  pump_release3 = air_out3;
  pinMode(air_release4, OUTPUT);
  pump_release4 = air_out4;
  pinMode(air_release5, OUTPUT);
  pump_release5 = air_out5;
  pinMode(air_release6, OUTPUT);
  pump_release6 = air_out6;

  pinMode(pres1, INPUT);
  pressure1 = pres1;
  pinMode(pres2, INPUT);
  pressure2 = pres2;
}

// Inflates the pockets to their maximum volume
// The air pumps turn on with a 1 signal and off with a 0 signal
// Full inflation takes 80ms per slot
void Inflation::full_inflate()
{
  digitalWrite(air_in1, HIGH);
  digitalWrite(air_in2, HIGH);
  digitalWrite(air_in3, HIGH);
  digitalWrite(air_in4, HIGH);
  digitalWrite(air_in5, HIGH);
  digitalWrite(air_in6, HIGH);
  delay(80);
  digitalWrite(air_in1, LOW);
  digitalWrite(air_in2, LOW);
  digitalWrite(air_in3, LOW);
  digitalWrite(air_in4, LOW);
  digitalWrite(air_in5, LOW);
  digitalWrite(air_in6, LOW);
}


void Inflation::partial_deflate()
{
  digitalWrite(air_release1, HIGH);
  digitalWrite(air_release2, HIGH);
  digitalWrite(air_release3, HIGH);
  digitalWrite(air_release4, HIGH);
  digitalWrite(air_release5, HIGH);
  digitalWrite(air_release6, HIGH);
}
