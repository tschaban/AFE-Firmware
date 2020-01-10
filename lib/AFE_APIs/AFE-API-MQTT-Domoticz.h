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

#include <AFE-DOMOTICZ-Structure.h>
#include <AFE-Device.h>
#include <AFE-MQTT-Structure.h>
#include <AFE-MQTT.h>
#include <ArduinoJson.h>

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

class AFEAPIMQTTDomoticz {

private:
  AFEDataAccess *_Data;
  AFEDevice *_Device;

  /* Is API enabled, set in begin() */
  boolean enabled = false;

  void generateSwitchMessage(char *json, uint32_t idx, boolean relayState);
  void generateDeviceValue(char *json, uint32_t idx, char *value);

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

#ifdef AFE_CONFIG_API_PROCESS_REQUESTS
  DOMOTICZ_MQTT_COMMAND getCommand();
  void processRequest();
  boolean idxForProcessing(uint32_t idx);
#endif

public:
  AFEMQTT Mqtt;

  /* Constructor: it sets all necessary parameters */
  AFEAPIMQTTDomoticz();
  void begin(AFEDataAccess *, AFEDevice *);

  void listener();

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

#endif //_AFE_API_MQTT_DOMOTICZ_h

