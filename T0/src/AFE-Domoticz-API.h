/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#ifndef _AFE_Domoticz_API_h
#define _AFE_Domoticz_API_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>
#include <AFE-Defaults.h>
#include <ESP8266HTTPClient.h>
#include <Streaming.h>

class AFEDomoticz {

private:
  AFEDataAccess Data;
  HTTPClient http;
  char serverURL[65];

public:
  DOMOTICZ configuration;

  AFEDomoticz();
  void begin();
  void callURL(const char *url);
  const String getRelayUpdateUrl(uint16_t idx, const char *value);
};

#endif
