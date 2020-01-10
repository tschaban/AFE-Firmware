/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_API_MQTT_STANDARD_h
#define _AFE_API_MQTT_STANDARD_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Configuration.h>
#include <AFE-API.h>


#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEAPIMQTTStandard : public AFEAPI {

private:

#ifdef AFE_CONFIG_API_PROCESS_REQUESTS
  void processRequest();
#endif

public:
  /* Constructor: it sets all necessary parameters */
  AFEAPIMQTTStandard();
  void begin(AFEDataAccess *, AFEDevice *);
  void subscribe();
  void synchronize();
  void listener();


#ifdef AFE_CONFIG_HARDWARE_RELAY
  boolean publishRelayState(uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  boolean publishSwitchState(uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  void publishADCValues();
#endif
};

#endif