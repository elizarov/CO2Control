#ifndef _NETWORK_H
#define _NETWORK_H

#include <Arduino.h>

const int MAX_PACKET_SIZE = 255;

class Network {
public:
  char packet[MAX_PACKET_SIZE + 1]; // last received packet
  void setup();
  bool receiveMcast();                    
  uint8_t level(); // 0-4
  int16_t addr(); // last byte or -1
};

extern Network network;

#endif
