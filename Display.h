#pragma once

enum SetupPhase {
  SETUP_DISPLAY,
  SETUP_NETWORK,
  SETUP_OTA,
  SETUP_WEB,
  SETUP_DONE
};

class Display {
public:
  void setup();
  void show(SetupPhase phase);
  void update();
  bool blinkState = false;
};

extern Display display;
