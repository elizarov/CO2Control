#ifndef _OTA_H
#define _OTA_H

class OTA {
public:
  void setup();
  void update();
  bool isActive();
};

extern OTA ota;

#endif
