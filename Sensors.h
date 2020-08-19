#ifndef _SENSORS_H
#define _SENSORS_H

#include <Arduino.h>
#include <FixNum.h>
#include <map>

struct Data {
  unsigned long lastUpdate;
  fixnum16_0 co2ppm;
  fixnum16_1 temp;
  fixnum16_0 hum;
};


class Sensors {
public:
  std::map<String, Data> dataMap;
  bool update();
  fixnum16_0 maxCO2ppm(fixnum16_0 ppmBase);
};

extern Sensors sensors;

#endif
