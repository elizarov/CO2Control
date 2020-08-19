#include "Output.h"

const uint8_t PIN = D8;
const uint16_t PPM_RANGE = 2112; // 3.3V vcc * 3.2 gain * 200 ppm/V
const uint16_t PPM_MAX = 2000; // corresponds to 10V
const uint16_t PPM_BASE = 400;

Output output;

Output::Output() : ppmBase(PPM_BASE), ppmOverride(-1) {}

void Output::setup() {
  analogWriteRange(PPM_RANGE);
  setPPM(ppmBase);
}

void Output::setPPM(fixnum16_0 value) {
  if (!value.valid()) return;
  ppm = value;
  update();
}

void Output::setOverridePPM(fixnum16_0 value) {
  if (!value.valid()) return;
  ppmOverride = value;
  update();
}

void Output::update() {
  int16_t value = ppmOverride >= 0 ? ppmOverride.mantissa() : ppm.mantissa();
  if (value < 0) value = 0;
  if (value > PPM_MAX) value = PPM_MAX;
  analogWrite(PIN, value);
}
