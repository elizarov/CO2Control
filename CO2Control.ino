#include <BlinkLed.h>
#include <FixNum.h>

#include "Network.h"
#include "Web.h"
#include "OTA.h"
#include "Output.h"

const unsigned long BLINK_NORMAL = 1000;
const unsigned long BLINK_OTA = 100;

BlinkLed blinkLed(LED_BUILTIN);

void setup() {
  Serial.begin(115200); // debug
  network.setup();
  web.setup();
  ota.setup();
  output.setPPM(fixnum16_0(0));
}

void loop() {
  web.update();
  blinkLed.blink(ota.isActive() ? BLINK_OTA : BLINK_NORMAL);
  ota.update();
}
