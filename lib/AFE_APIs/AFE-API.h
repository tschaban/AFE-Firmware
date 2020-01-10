/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_API_h
#define _AFE_API_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Configuration.h>
#include <AFE-Device.h>
#include <AFE-MQTT-Structure.h>
#include <AFE-MQTT.h>

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

class AFEAPI {

public:
  AFEMQTT Mqtt;
  AFEDevice *_Device;

  AFEAPI();
  void begin(AFEDataAccess *, AFEDevice *);

#ifdef AFE_CONFIG_HARDWARE_RELAY
  void addClass(AFERelay *);
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  void addClass(AFESwitch *);
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  void addClass(AFEAnalogInput *);
#endif

protected:
  /* Is API enabled, set in begin() */
  boolean enabled = false;

#ifdef AFE_CONFIG_HARDWARE_RELAY
  AFERelay *_Relay[AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS];
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  AFESwitch *_Switch[AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES];
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  AFEAnalogInput *_AnalogInput;
#endif

private:
  AFEDataAccess *_Data;
};

#endif // _AFE_API_h