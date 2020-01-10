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

#include <AFE-Data-Access.h>
#include <AFE-Device.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <rBase64.h>

#ifdef AFE_CONFIG_HARDWARE_RELAY
#include <AFE-Relay.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
#include <AFE-Switch.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
#include <AFE-Analog-Input.h>
#endif

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEAPIHTTPDomoticz {

private:
  HTTPClient http;
  WiFiClient client;
  AFEDataAccess *_Data;
  AFEDevice *_Device;
  DOMOTICZ configuration;

  char serverURL[AFE_CONFIG_API_DOMOTICZ_URL_LENGTH];

  /* Is API enabled, set in begin() */
  boolean enabled = false;

#ifdef AFE_CONFIG_API_PROCESS_REQUESTS
  DOMOTICZ_IDX_CACHE idxCache[AFE_CONFIG_API_DOMOTICZ_IDX_CACHE_LENGTH];
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
  AFERelay *_Relay[AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS];
  DOMOTICZ_BASIC_CONFIG bypassProcessing;
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  AFESwitch *_Switch[AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES];
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  AFEAnalogInput *_Analog;
#endif

  const String getApiCall(const char *param, unsigned int idx);
  boolean callURL(const String url);

  boolean sendSwitchCommand(unsigned int idx, const char *value);
  boolean sendCustomSensorCommand(unsigned int idx, const char *value);

public:
  /* Constructor: it sets all necessary parameters */
  AFEAPIHTTPDomoticz();
  void begin(AFEDataAccess *, AFEDevice *);

  boolean idxForProcessing(uint32_t idx);

#ifdef AFE_CONFIG_HARDWARE_RELAY
  void addClass(AFERelay *);
  boolean publishRelayState(uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  void addClass(AFESwitch *);
  boolean publishSwitchState(uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  void addClass(AFEAnalogInput *);
  void publishADCValues();
#endif
};

#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

#endif // _AFE_API_HTTP_DOMOTICZ_h