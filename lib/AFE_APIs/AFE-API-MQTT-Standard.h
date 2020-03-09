/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_API_MQTT_STANDARD_h
#define _AFE_API_MQTT_STANDARD_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Configuration.h>

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED

#include <AFE-API.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEAPIMQTTStandard : public AFEAPI {

private:
#ifdef AFE_CONFIG_API_PROCESS_REQUESTS
  /* Classifies incomming MQTT Topics and invokes code for processing them */
  void processRequest();
  /* Size of the cache that stories MQTT Topics AFE has subsribed to */
  uint8_t currentCacheSize = 0;
  /* Cache with MQTT Topics AFE has subsribed to */
  MQTT_TOPICS_CACHE mqttTopicsCache[AFE_CONFIG_API_MQTT_TOPICS_CACHE_LENGTH];
#endif // AFE_CONFIG_API_PROCESS_REQUESTS

public:
  /* Constructor: it sets all necessary parameters */
  AFEAPIMQTTStandard();
#ifdef AFE_CONFIG_HARDWARE_LED
  void begin(AFEDataAccess *, AFEDevice *, AFELED *);
#else
  void begin(AFEDataAccess *, AFEDevice *);
#endif // AFE_CONFIG_HARDWARE_LED

  /* Subscribes to MQTT Topcis */
  void subscribe();

  /* Synchronize device's items values after connection to MQTT Broker is
   * established */
  void synchronize();

  /* Listens for MQTT Messages */
  void listener();

#ifdef AFE_CONFIG_HARDWARE_RELAY
  /* Publishes relay state */
  boolean publishRelayState(uint8_t id);
  /* Processes MQTT Request related to Relay */
  void processRelay(uint8_t *id);
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  /* Publishes switch state */
  boolean publishSwitchState(uint8_t id);
  /* Processes MQTT Request related to Switch */
  void processSwitch(uint8_t *id);
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  /* Publishes ADC data */
  void publishADCValues();
  /* Processes ADC MQTT requests */
  void processADC();
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  void processBMEX80(uint8_t *id);
  boolean publishBMx80SensorData(uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  void processHPMA115S0(uint8_t *id);
  boolean publishHPMA115S0SensorData(uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
  void processBH1750(uint8_t *id);
  boolean publishBH1750SensorData(uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
  void processAS3935(uint8_t *id);
  boolean publishAS3935SensorData(uint8_t id);
#endif
};

#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
#endif // _AFE_API_MQTT_STANDARD_h