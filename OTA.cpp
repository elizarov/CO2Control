#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "OTA.h"
#include "Blink.h"

OTA ota;

void OTA::setup() {
  ArduinoOTA.onStart([&]() {
    progress = 0;
    blink.update();
  });
  ArduinoOTA.onEnd([&]() {
    progress = -1;
    blink.update();
  });
  ArduinoOTA.onProgress([&](unsigned int done, unsigned int total) {
    progress = ((unsigned long)done * 100) / total;
    blink.update();
  });
  ArduinoOTA.onError([&](ota_error_t error) {
    progress = -1;
    blink.update();
    ESP.restart();
  });
}

void OTA::update() {
  if (connected) {
    ArduinoOTA.handle();      
  } else {
    if (WiFi.waitForConnectResult() == WL_CONNECTED) {
      connected = true;
      ArduinoOTA.begin();
    }
  }
}

bool OTA::isActive() {
  return progress >= 0;  
}
