#include "Output.h"

const uint8_t PIN = D8;
const uint16_t MAX_PPM = 1320; // 3.3V vcc * 2 gain * 200 ppm/V

Output output;

void Output::setPPM(fixnum16_0 value) {
  ppm = value;
  uint16_t analog = 
    value.mantissa() <= 0 ? 0 :
    value.mantissa() >= MAX_PPM ? PWMRANGE :
      (value.mantissa() * (long)PWMRANGE) / MAX_PPM;
  analogWrite(PIN, analog);
  Serial.print("setPPM: analog = ");
  Serial.println(analog, DEC);
}
