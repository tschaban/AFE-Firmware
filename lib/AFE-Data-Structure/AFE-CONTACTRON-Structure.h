/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_CONTACTRON_Structure_h
#define _AFE_CONTACTRON_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-DOMOTICZ-Structure.h>
#include <AFE-MQTT-Structure.h>

struct CONTACTRON {
  uint8_t gpio;
  byte type = AFE_CONTACTRON_NO;
  uint8_t ledID;
  uint16_t bouncing = AFE_CONFIG_HARDWARE_CONTACTRON_DEFAULT_BOUNCING;
  char name[17];
  DOMOTICZ_BASIC_CONFIG domoticz;
  MQTT_BASIC_CONFIG mqtt;
};

#endif
