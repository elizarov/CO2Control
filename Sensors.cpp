#include "Sensors.h"
#include "Network.h"

const unsigned long REMOVE_TIMEOUT = 3 * 60000; // 3 min

Sensors sensors;

bool Sensors::update() {
  bool removed = false;
  unsigned long now = millis();
  for (auto it = dataMap.cbegin(); it != dataMap.cend(); ) {
    if ((now - it->second.lastUpdate) > REMOVE_TIMEOUT ) {
      dataMap.erase(it++);
      removed = true;
    } else {
      it++;
    }
  }
  if (!network.receiveMcast()) return removed;
  if (network.packet[0] != '[') return removed;
  char* start = &network.packet[1];
  char* cur = start;
  while (*cur != 0 && *cur != ']' && *cur != ':') cur++;
  if (*cur != ':') return removed;
  *cur = 0;
  String nodeId(start);
  Data& data = dataMap[nodeId];
  data.init(nodeId);
  data.lastUpdate = now;
  data.co2ppm.clear();
  data.temp.clear();
  data.hum.clear();
  cur++;
  FixNumParser<int32_t> parser;
  while (*cur != 0 && *cur != ']') {
    start = cur;
    if (*cur != 0 && *cur != ']' && *cur != '+' && *cur != '-' && *cur != '.' && (*cur < '0' || *cur > '9')) cur++;
    char* end = cur;
    parser.reset();
    while (parser.parse(*cur) == FixNumParser<int16_t>::NUM) cur++;        
    if (end - start != 1) continue;
    switch (*start) {
      case 'c': data.co2ppm = parser; break;
      case 't': data.temp = parser; break;
      case 'h': data.hum = parser; break;
      case 'r': data.rssi = parser; break;
      case 'u': data.uptime = parser; break;
    }
  }
  data.pushData();
  return true;
}

fixnum16_0 Sensors::maxCO2ppm(fixnum16_0 ppmBase) {
  for (const auto& kv : dataMap) {
    const Data& data = kv.second;
    if (!ppmBase.valid() || data.co2ppm > ppmBase) ppmBase = data.co2ppm;
  }
  return ppmBase;
}

// --- Data --- 

String PREFIX = "C";

void Data::init(String nodeId) {
  String tag = PREFIX + nodeId;
  if (!co2ppmTag) co2ppmTag = push.newTag(tag + 'c');
  if (!tempTag) tempTag = push.newTag(tag + 't');
  if (!humTag) humTag = push.newTag(tag + 'h');
}

void Data::pushData() const {
  push.put(co2ppmTag, co2ppm);
  push.put(tempTag, temp);
  push.put(humTag, hum);
}
