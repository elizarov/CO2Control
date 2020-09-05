#pragma once

#include <Arduino.h>
#include <FixNum.h>
#include <Timeout.h>
#include <map>

#include "Push.h"

struct Data {
  unsigned long lastUpdate;
  fixnum16_0 co2ppm;
  fixnum16_1 temp;
  fixnum16_0 hum;

private: 
  void init(String nodeId);
  void pushData() const;
  
  PushItem* co2ppmTag = nullptr;
  PushItem* tempTag = nullptr;
  PushItem* humTag = nullptr;

  friend class Sensors;
};


class Sensors {
public:
  std::map<String, Data> dataMap;
  bool update();
  fixnum16_0 maxCO2ppm(fixnum16_0 ppmBase);
};

extern Sensors sensors;
