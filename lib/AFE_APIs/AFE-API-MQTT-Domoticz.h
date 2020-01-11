/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_API_MQTT_DOMOTICZ_h
#define _AFE_API_MQTT_DOMOTICZ_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Configuration.h>

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED

#include <AFE-API.h>
#include <ArduinoJson.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEAPIMQTTDomoticz : public AFEAPI {

private:

  void generateSwitchMessage(char *json, uint32_t idx, boolean relayState);
  void generateDeviceValue(char *json, uint32_t idx, char *value);

#ifdef AFE_CONFIG_API_PROCESS_REQUESTS
  DOMOTICZ_IDX_CACHE idxCache[AFE_CONFIG_API_DOMOTICZ_IDX_CACHE_LENGTH];
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
  DOMOTICZ_BASIC_CONFIG bypassProcessing;
#endif

#ifdef AFE_CONFIG_API_PROCESS_REQUESTS
  DOMOTICZ_MQTT_COMMAND getCommand();
  void processRequest();
  boolean idxForProcessing(uint32_t idx);
#endif

public:
  /* Constructor: it sets all necessary parameters */
  AFEAPIMQTTDomoticz();
  void begin(AFEDataAccess *, AFEDevice *);
#ifdef AFE_CONFIG_HARDWARE_LED
  void begin(AFEDataAccess *, AFEDevice *, AFELED *Led);
#endif
  void subscribe();
  void synchronize();
  void listener();

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

#endif //_AFE_API_MQTT_DOMOTICZ_h
