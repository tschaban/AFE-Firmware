/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

/* Class
- establishes connection to MQTT Broker
- is a proxy between AFE-APIs and MQTT Broker
- it also stories referencies to global classes that can be used for AFE-APIs */

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

#ifdef AFE_CONFIG_HARDWARE_LED
#include <AFE-LED.h>
#endif

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
private:
public:
  AFEMQTT Mqtt;
  AFEDevice *_Device;
  AFEDataAccess *_Data;
#ifdef AFE_CONFIG_HARDWARE_LED
  AFELED *_Led;
#endif

  AFEAPI();

#ifdef AFE_CONFIG_HARDWARE_LED
  void begin(AFEDataAccess *, AFEDevice *, AFELED *);
#else
  void begin(AFEDataAccess *, AFEDevice *);
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
  virtual void addClass(AFERelay *);
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  virtual void addClass(AFESwitch *);
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  virtual void addClass(AFEAnalogInput *);
#endif

protected:
  /* Is API enabled, set in begin() */
  boolean enabled = false;

#ifdef AFE_CONFIG_HARDWARE_RELAY
  /* Stories reference to global Relay class */
  AFERelay *_Relay[AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS];
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  /* Stories reference to global Switch class */
  AFESwitch *_Switch[AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES];
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  /* Stories reference to global ADC class */
  AFEAnalogInput *_AnalogInput;
#endif
};

#endif // _AFE_API_h