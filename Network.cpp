
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <Timeout.h>

#include "Network.h"
#include "NetworkConfig.h"

Network network;
WiFiUDP udp;
bool initialized;

// reset if not connected after timeout
const unsigned long NETWORK_TIMEOUT = 10000; 
Timeout networkTimeout;
bool wasConnected = false;

void Network::setup() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(networkSsid, networkPass);  
  networkTimeout.reset(NETWORK_TIMEOUT);
}

void Network::update() {
  bool isConnected = WiFi.status() == WL_CONNECTED;
  if (!isConnected) {
    if (wasConnected) {
      networkTimeout.reset(NETWORK_TIMEOUT);
    }
    if (networkTimeout.check()) {
      ESP.reset();
    }
  }
  wasConnected = isConnected;
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
