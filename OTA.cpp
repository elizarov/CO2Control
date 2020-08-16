#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "OTA.h"

OTA ota;

int progress = -1;

void OTA::setup() {
  ArduinoOTA.onStart([]() {
    progress = 0;
  });
  ArduinoOTA.onEnd([]() {
    progress = -1;
  });
  ArduinoOTA.onProgress([](unsigned int done, unsigned int total) {
    progress = ((unsigned long)done * 100) / total;
  });
  ArduinoOTA.onError([](ota_error_t error) {
    progress = -1;
  });
  ArduinoOTA.begin();
}

void OTA::update() {
  ArduinoOTA.handle();  
}

bool OTA::isActive() {
  return progress >= 0;  
}
