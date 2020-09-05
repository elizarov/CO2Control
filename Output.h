#pragma once

#include <Arduino.h>
#include <FixNum.h>

class Output {
public:
  fixnum16_0 ppm; // CO2 ppm
  fixnum16_0 ppmOverride; // CO2 ppm override
  fixnum16_0 ppmBase; // 400 ppm
  Output();
  void setup();
  void setPPM(fixnum16_0 value);
  void setOverridePPM(fixnum16_0 value);
private:
  void update();  
};

extern Output output;
