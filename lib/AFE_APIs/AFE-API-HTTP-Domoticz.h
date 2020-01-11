/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_API_HTTP_DOMOTICZ_h
#define _AFE_API_HTTP_DOMOTICZ_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Configuration.h>

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED

#include <AFE-API.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <rBase64.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEAPIHTTPDomoticz : public AFEAPI {

private:
  HTTPClient http;
  WiFiClient client;
  DOMOTICZ configuration;

  char serverURL[AFE_CONFIG_API_DOMOTICZ_URL_LENGTH];

#ifdef AFE_CONFIG_API_PROCESS_REQUESTS
  DOMOTICZ_IDX_CACHE idxCache[AFE_CONFIG_API_DOMOTICZ_IDX_CACHE_LENGTH];
#endif

  const String getApiCall(const char *param, unsigned int idx);
  boolean callURL(const String url);

  boolean sendSwitchCommand(unsigned int idx, const char *value);
  boolean sendCustomSensorCommand(unsigned int idx, const char *value);

public:
  /* Constructor: it sets all necessary parameters */
  AFEAPIHTTPDomoticz();

#ifdef AFE_CONFIG_HARDWARE_LED
  void begin(AFEDataAccess *, AFEDevice *, AFELED *);
#else
  void begin(AFEDataAccess *, AFEDevice *);
#endif

  void init();

#ifdef AFE_CONFIG_HARDWARE_RELAY
  virtual void addClass(AFERelay *);
  boolean publishRelayState(uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  virtual void addClass(AFESwitch *);
  boolean publishSwitchState(uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  virtual void addClass(AFEAnalogInput *);
  void publishADCValues();
#endif
};

#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
#endif // _AFE_API_HTTP_DOMOTICZ_h