#ifndef _OTA_H
#define _OTA_H

class OTA {
public:
  void setup();
  void update();
  bool active;
private:
  bool connected = false;
};

extern OTA ota;

#endif
