#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "OTA.h"
#include "Blink.h"

OTA ota;

void OTA::setup() {
  ArduinoOTA.onStart([&]() {
    active = true;
    blink.update();
  });
  ArduinoOTA.onEnd([&]() {
    blink.update();
  });
  ArduinoOTA.onProgress([&](unsigned int done, unsigned int total) {
    blink.update();
  });
  ArduinoOTA.onError([&](ota_error_t error) {
    blink.update();
    ESP.restart();
  });
}

void OTA::update() {
  if (connected) {
    ArduinoOTA.handle();      
  } else {
    if (WiFi.status() == WL_CONNECTED) {
      connected = true;
      ArduinoOTA.begin();
    }
  }
}
