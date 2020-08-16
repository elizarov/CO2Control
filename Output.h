#ifndef _OUTPUT_H
#define _OUTPUT_H

#include <Arduino.h>
#include <FixNum.h>

class Output {
public:
  fixnum16_0 ppm; // CO2 ppm
  void setPPM(fixnum16_0 value);
};

extern Output output;

#endif
