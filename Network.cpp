
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include "Network.h"
#include "NetworkConfig.h"

Network network;
WiFiUDP udp;
bool initialized;

void Network::setup() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(networkSsid, networkPass);  
}

bool Network::receiveMcast() {
  if (WiFi.status() != WL_CONNECTED) return false;
  if (!initialized) {
    initialized = true;
    udp.beginMulticast(WiFi.localIP(), mcastAddr, mcastPort);
  }
  int packetSize = udp.parsePacket();
  if (packetSize == 0) return false;
  if (packetSize > MAX_PACKET_SIZE) packetSize = MAX_PACKET_SIZE;
  udp.read(packet, packetSize);
  packet[packetSize] = 0;
  while (udp.available()) udp.read(); // discard the rest
  return true;
}

uint8_t Network::level() {
  if (WiFi.status() != WL_CONNECTED) return 0;
  int rssi = WiFi.RSSI();
  if (rssi >= -40) return 4;
  if (rssi >= -50) return 3;
  if (rssi >= -60) return 2;
  if (rssi >= -70) return 1;
  return 0; 
}

int16_t Network::addr() {
  if (WiFi.status() != WL_CONNECTED) return -1;
  return WiFi.localIP()[3];  
}
