#pragma once

#include <Arduino.h>
#include <Timeout.h>
#include <FixNum.h>

#include "MsgBuf.h"

struct PushItem {
  PushItem* next;
  String tag;
  int32_t val;    // value
  prec_t prec;    // precision
  byte updated;
  byte sending;
};

class PushDest {
protected:
  byte _mask;
  const char* _host;
  int _port;
  const char* _url;
  const char* _auth;
  const char* _method;

  Timeout _period;
  Timeout _timeout;

  bool _next;
  bool _sending;

  bool sendPacket(byte size);
  void parseChar(char ch);
  bool readResponse();

  virtual void doneSend(bool success);
  virtual void printExtraUrlParams() {}
  virtual void printExtraHeaders() {}
  virtual void parseResponseHeaders(char ch) {}
  virtual void parseResponseBody(char ch) {}
public:
  PushDest(byte mask, char* host, int port, char* url, char* auth);
  void check();
  bool isSending();
  long remaining();
};

const long MAX_COOKIE_LEN = 20;

class PushMsgDest : PushDest {
protected:
  long _indexIn;
  msg_index_t _indexOut;
  bool _newSession;
  char _cookie[MAX_COOKIE_LEN + 1];
  byte _parseCookieState; // Parse Set-Cookie response header
  byte _parseBodyState; // Parse response messages from body
  bool _wait;
  virtual void doneSend(bool success);
  virtual void printExtraUrlParams();
  virtual void printExtraHeaders();
  virtual void parseResponseHeaders(char ch);
  virtual void parseResponseBody(char ch);
public:
  PushMsgDest(byte mask, char* host, int port, char* url, char* auth);
  void check();
};

// declared in PushConfig.cpp
extern PushDest haworks_data;
// extern PushMsgDest haworks_message; // todo: no messages now

class Push {
private:
  bool _wasSending;
public:
  PushItem* newTag(const String& tag); // get or create new item with a given tag
  void put(PushItem* item, int32_t val, prec_t prec);
  
  bool update();
  bool isSending();
  uint16_t remaining(uint16_t max); 

  template<typename T, prec_t prec> void put(PushItem* item, FixNum<T, prec> val);
};

extern Push push;

template<typename T, prec_t prec> void Push::put(PushItem* item, FixNum<T, prec> val) {
  if (val.valid()) put(item, val.mantissa(), prec);
}
