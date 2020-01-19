/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_API_HTTP_DOMOTICZ_h
#define _AFE_API_HTTP_DOMOTICZ_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Configuration.h>

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED // Code excluded for not Standard API version of Firmware

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
  // Stories IDXs
  DOMOTICZ_IDX_CACHE idxCache[AFE_CONFIG_API_DOMOTICZ_IDX_CACHE_LENGTH];
#endif

  /* Invokes API setup */
  void init();

  /* It creates Domoticz url with all required params */
  const String getApiCall(const char *param, unsigned int idx);

  /* Calls url. Publishes to Domoticz */
  boolean callURL(const String url);

  /* Sends to Domoticz switch update call */
  boolean sendSwitchCommand(unsigned int idx, const char *value);

  /* sends to Domoticz custome sensor call */
  boolean sendCustomSensorCommand(unsigned int idx, const char *value);

public:
  /* Constructor: it sets all necessary parameters */
  AFEAPIHTTPDomoticz();

#ifdef AFE_CONFIG_HARDWARE_LED
  void begin(AFEDataAccess *, AFEDevice *, AFELED *);
#else
  void begin(AFEDataAccess *, AFEDevice *);
#endif // AFE_CONFIG_HARDWARE_LED

#ifdef AFE_CONFIG_HARDWARE_RELAY
  /* Add reference to global class and adds IDX to cache */
  virtual void addClass(AFERelay *);
  /* Publishes current relay state to Domoticz */
  boolean publishRelayState(uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  /* Add reference to global class */
  virtual void addClass(AFESwitch *);
  /* Publishes current switch state to Domoticz */
  boolean publishSwitchState(uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  /* Add reference to global class*/
  virtual void addClass(AFEAnalogInput *);
  /* Publishes current ADC data to Domoticz */
  void publishADCValues();
#endif
};

#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
#endif // _AFE_API_HTTP_DOMOTICZ_h