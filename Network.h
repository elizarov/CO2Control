#pragma once

#include <Arduino.h>

const int MAX_PACKET_SIZE = 255;

class Network {
public:
  char packet[MAX_PACKET_SIZE + 1]; // last received packet
  void setup();
  void update();
  bool receiveMcast();                    
};

extern Network network;
