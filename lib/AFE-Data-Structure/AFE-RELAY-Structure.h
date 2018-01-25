/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#ifndef _AFE_RELAY_Structure_h
#define _AFE_RELAY_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-REGULATOR-Structure.h>

/* Relay states */
#define RELAY_ON 1
#define RELAY_OFF 0

struct RELAY {
  uint8_t gpio;
  float timeToOff;
  uint8_t statePowerOn;
  char name[16];
  uint8_t stateMQTTConnected;
  char mqttTopic[49];
  boolean showStatusUsingLED; // is this really used?
  REGULATOR thermostat;
  REGULATOR humidistat;
  float thermalProtection;
  uint8_t ledID;
};

#endif
