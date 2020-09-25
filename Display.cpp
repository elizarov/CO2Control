#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <ESP8266WiFi.h>

#include <Timeout.h>
#include <DrawRSSI.h>

#include "Sensors.h"
#include "OTA.h"
#include "Display.h"
#include "Push.h"

// SCL GPIO5
// SDA GPIO4
// OLED Display 128x64
Adafruit_SSD1306 d(128, 64, &Wire);

Display display;

const unsigned long MIN_SHOW_TIME = 100;
unsigned long prevShowTime = 0;

void Display::setup() {
  d.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  show(SETUP_DISPLAY);
}

void Display::show(SetupPhase phase) {
  d.clearDisplay();
  d.setTextColor(WHITE); 
  d.setCursor(0, 16);
  d.print(F("SETUP:"));
  d.setCursor(0, 24);
  switch (phase) {
    case SETUP_DISPLAY: d.print(F("DISPLAY")); break;
    case SETUP_NETWORK: d.print(F("NETWORK")); break;
    case SETUP_OTA:     d.print(F("OTA")); break;
    case SETUP_WEB:     d.print(F("WEB")); break;
    case SETUP_DONE:    d.print(F("DONE")); break;
  }
  d.setCursor(0, 32);
  d.print(phase, DEC);
  d.print('/');
  d.print(SETUP_DONE, DEC);
  d.display();
  unsigned long now = millis();
  unsigned long since = now - prevShowTime;
  if (since < MIN_SHOW_TIME) delay(MIN_SHOW_TIME - since);
  prevShowTime = now;
}

void displaySensors() {
  // CO2 PPM num
  fixnum16_0 co2ppm = sensors.maxCO2ppm(fixnum16_0());
  if (co2ppm.valid()) {  
    d.setFont(&FreeMonoBold18pt7b);
    d.setCursor(2, 50);
    d.print(co2ppm.format());
    d.setFont();

    // CO2 PPM label
    d.setCursor(0, 10);
    d.print(F("CO2"));
    d.setCursor(40, 10);
    d.print(F("ppm"));
  }
  // Sensors
  fixnum16_0 maxCO2ppm = sensors.maxCO2ppm(fixnum16_0(0));
  uint16_t y = 10;
  for (auto const& kv : sensors.dataMap) {
    const Data& data = kv.second;
    if (data.co2ppm == maxCO2ppm) {
      d.fillTriangle(71, y + 3, 74, y, 74, y + 6, WHITE);
    }
    d.setCursor(78, y);
    d.print(kv.first);
    d.print(":");
    d.setCursor(95, y);
    d.print(data.co2ppm.format());
    if (data.rssi.valid()) drawRSSI(d, 117, y, data.rssi.mantissa(), WHITE);
    y += 8;
  }
  // Push
  if (push.isSending()) {
    d.fillTriangle(2, 61, 8, 61, 5, 58, WHITE);
  }
  d.drawLine(0, 63, push.remaining(128), 63, WHITE);
  // Blinking dot
  if (display.blinkState) { 
    d.fillCircle(2, 3, 2, WHITE);
  }
}

void displayOTA() {
  d.setCursor(0, 16);
  d.print(F("FLASH:"));
  d.setCursor(0, 24);
  d.print(ota.progress, DEC);
  d.print('%');
  d.setCursor(0, 32);
  switch (ota.status) {
    case OTA_DONE: d.print(F("DONE")); break;
    case OTA_ERROR: d.print(F("ERROR")); break;
  }
}

void Display::update() {
  d.clearDisplay();
  d.setTextColor(WHITE);

  // Dipslay OTA status or sensors
  if (ota.status != OTA_NONE) {
    displayOTA();
  } else {
    displaySensors();
  }

  if (WiFi.status() == WL_CONNECTED) {
    // IP address
    d.setCursor(18, 0);
    IPAddress addr = WiFi.localIP(); 
    for (uint8_t i = 0; i <= 3; i++) {
      if (i > 0) d.print('.');
      d.print(addr[i], DEC);
    }

    // RSSI Level
    drawRSSI(d, 117, 0, WiFi.RSSI(), WHITE);
  }

  d.display();
}
