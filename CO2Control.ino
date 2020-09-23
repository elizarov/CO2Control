
#include <FixNum.h>

#include "Network.h"
#include "Web.h"
#include "OTA.h"
#include "Output.h"
#include "Sensors.h"
#include "Push.h"
#include "Display.h"

// ------------- blink -------------

const unsigned long BLINK_TIMEOUT = 1000;
Timeout blinkTimeout(BLINK_TIMEOUT);

bool blinkUpdate() {
  if (!blinkTimeout.check()) return false;
  blinkTimeout.reset(BLINK_TIMEOUT);
  display.blinkState = !display.blinkState;
  return true;
}

// ------------- Main -------------

void setup() {
  digitalWrite(LED_BUILTIN, 1);
  Serial.begin(115200); // debug
  display.setup();
  display.show(SETUP_NETWORK);
  network.setup();
  display.show(SETUP_OTA);
  ota.setup();
  display.show(SETUP_WEB);
  web.setup();
  display.show(SETUP_DONE);
  output.setup();
  digitalWrite(LED_BUILTIN, 0);
}

void loop() {
  // Blink
  bool blinkUpdated = blinkUpdate();

  // Network
  boolean networkUpdated = network.update();
  web.update();
  ota.update();

  // Sensors
  bool sensorsUpdated = sensors.update();
  if (sensorsUpdated) {
    output.setPPM(sensors.maxCO2ppm(output.ppmBase));
  }

  // Upload
  bool pushUpdated = push.update();

  // Display
  if (blinkUpdated || networkUpdated || sensorsUpdated || pushUpdated) {
    display.update();
  }
}
