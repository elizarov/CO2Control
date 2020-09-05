
#include <FixNum.h>

#include "Blink.h"
#include "Network.h"
#include "Web.h"
#include "OTA.h"
#include "Output.h"
#include "Sensors.h"
#include "Push.h"

void setup() {
  Serial.begin(115200); // debug
  network.setup();
  web.setup();
  ota.setup();
  output.setup();
}

void loop() {
  blink.update();
  network.update();
  web.update();
  ota.update();
  if (sensors.update()) {
    output.setPPM(sensors.maxCO2ppm(output.ppmBase));
  }
  haworks_data.check();
  //haworks_message.check(); // todo: no messages now
}
