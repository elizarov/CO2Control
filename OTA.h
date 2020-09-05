#pragma once

class OTA {
public:
  void setup();
  void update();
  bool active;
private:
  bool connected = false;
};

extern OTA ota;
