#ifndef _OTA_H
#define _OTA_H

class OTA {
public:
  void setup();
  void update();
  bool isActive();
private:
  int progress = -1;
  bool connected = false;
};

extern OTA ota;

#endif
