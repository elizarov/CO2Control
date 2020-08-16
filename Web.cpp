#include <WString.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "Web.h"
#include "Output.h"

Web web;

ESP8266WebServer s(80);

void handleMain();
void handleConfig();

typedef void (*handler_t)();

enum Page { MAIN, CONFIG, N_PAGES };
char* names[] = { "Main", "Config" };
char* links[]= { "/", "/config" };
handler_t handlers[] = { &handleMain, &handleConfig };

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

void handleMain() {
  String m = navigation(MAIN);
  m += F("<pre>\n");
  m += F("------ Network\n");
  m += F("   IP: "); m += WiFi.localIP().toString(); m += '\n';
  m += F(" RSSI: "); m += WiFi.RSSI(); m += F(" dBm\n");
  m += F("\n");
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

const char* CFG_PPM = "ppm";

void handleConfig() {
  String m = navigation(CONFIG);
  if (s.method() == HTTP_POST) {
    output.setPPM(fixnum16_0::parse(s.arg(CFG_PPM).c_str()));
  }
  m += F("<form method='post'>");
  m += formInputText(CFG_PPM, "CO2 PPM", output.ppm.format());
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
