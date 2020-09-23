#pragma once

enum OTAStatus {
  OTA_NONE,
  OTA_ACTIVE,
  OTA_ERROR,
  OTA_DONE
};

class OTA {
public:
  void setup();
  void update();
  bool active;
  OTAStatus status = OTA_NONE;
  int progress = -1;
private:
  bool connected = false;
};

extern OTA ota;
