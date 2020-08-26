
#include <Arduino.h>
#include <BlinkLed.h>

#include "Blink.h"
#include "Ota.h"

Blink blink;

const unsigned long BLINK_NORMAL = 1000;
const unsigned long BLINK_OTA = 100;

BlinkLed blinkLed(LED_BUILTIN);

void Blink::update() {
  blinkLed.blink(ota.active ? BLINK_OTA : BLINK_NORMAL);
}
