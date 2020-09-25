#include <WString.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "Web.h"
#include "Output.h"
#include "Sensors.h"

Web web;

ESP8266WebServer s(80);

void handleMain();
void handleOverride();

typedef void (*handler_t)();

enum Page { MAIN, OVERRIDE, N_PAGES };
char* names[] = { "Main", "Override" };
char* links[]= { "/", "/override" };
handler_t handlers[] = { &handleMain, &handleOverride };

String navigation(Page page) {
  String m = F("<!DOCTYPE html>");
  m += F("<h1>CO2 Control - "); m += names[page]; m += F("</h1>\n");
  for (int i = 0; i < N_PAGES; i++) {
    m += F("[ "); 
    if (i != page) { m += F("<a href='"); m += links[i]; m += F("'>"); }
    m += names[i];
    if (i != page) { m += F("</a>"); }
    m += F(" ] ");
  }
  m += "<br>\n<p>";
  return m;
}

const size_t NAME_LEN = 5;

void handleMain() {
  String m = navigation(MAIN);
  m += F("<pre>\n");
  m += F("------ Sensors\n");
  fixnum16_0 maxCO2ppm = sensors.maxCO2ppm(fixnum16_0(0));
  unsigned long now = millis();
  for (auto const& kv : sensors.dataMap) {
    char buf[NAME_LEN + 1];
    memset(buf, ' ', NAME_LEN);
    int n = min(NAME_LEN, kv.first.length());
    strncpy(&buf[NAME_LEN - n], kv.first.c_str(), n);
    buf[NAME_LEN] = 0;
    m += buf;
    m += F(": ");
    const Data& data = kv.second;
    if (data.co2ppm == maxCO2ppm) m += F(">>"); else m += F("  ");
    m += data.co2ppm.format(5, FMT_RIGHT);
    m += F(" ppm  ");
    m += data.temp.format(5, FMT_RIGHT | 1);
    m += F(" C  ");
    m += data.hum.format(2, FMT_RIGHT);
    m += F("%  ");
    m += data.rssi.format(3, FMT_RIGHT);
    m += F(" dBm  ");
    m += fixnum32_0((now - data.lastUpdate) / 1000).format(5, FMT_RIGHT);
    m += F("s ago  ");
    m += data.uptime.format(10, FMT_RIGHT);
    m += F(" uptime\n");
  }
  m += '\n';
  m += F("------ Output\n");
  m += F("  CO2:   "); m += output.ppm.format(5, FMT_RIGHT); m += F(" ppm\n");
  if (output.ppmOverride >= 0) {
    m += F("  OVR: >>"); m += output.ppmOverride.format(5, FMT_RIGHT); m += F(" ppm <<\n");
  }
  m += '\n';
  m += F("------ Network\n");
  m += F("   IP: "); m += WiFi.localIP().toString(); m += '\n';
  m += F(" RSSI: "); m += WiFi.RSSI(); m += F(" dBm\n");
  m += '\n';
  m += F("------ Chip\n");
  m += F("   ID: "); m += String(ESP.getChipId(), HEX); m += '\n';
  m += F(" Core: "); m += ESP.getCoreVersion(); m += '\n';
  m += F("  SDK: "); m += ESP.getSdkVersion(); m += '\n';
  m += F(" Freq: "); m += ESP.getCpuFreqMHz(); m += F(" MHz\n");
  m += F("Flash: "); m += (ESP.getFlashChipSize() >> 20); m += F(" MiB\n");
  m += F("</pre>");
  s.send(200, "text/html", m);
}

String formInputText(const char* name, const char* label, const char* value) {
  String m;
  m += F("<label for='"); m += name; m += F("'>"); m += label; m += F(":<label><br>");
  m += F("<input type='text' id='"); m += name; m += F("' name='"); m += name; m += F("' value='"); m += value; m += F("'><br>");
  return m;
}

const char* OVR_PPM = "ppm";

void handleOverride() {
  String m = navigation(OVERRIDE);
  if (s.method() == HTTP_POST) {
    output.setOverridePPM(fixnum16_0::parse(s.arg(OVR_PPM).c_str()));
  }
  m += F("<form method='post'>");
  m += formInputText(OVR_PPM, "CO2 PPM Override", output.ppmOverride.format());
  m += F("<p><input type='submit' value='Update'>");
  m += F("</form>");
  s.send(200, "text/html", m);
}

void handleNotFound() {
  s.send(404, "text/plain", "Not found");
}

void Web::setup() {
  for (int i = 0; i < N_PAGES; i++) s.on(links[i], handlers[i]);
  s.onNotFound(handleNotFound);
  s.begin();
}

void Web::update() {
  s.handleClient();
}
