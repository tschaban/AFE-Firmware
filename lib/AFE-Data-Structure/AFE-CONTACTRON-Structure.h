/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_CONTACTRON_Structure_h
#define _AFE_CONTACTRON_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-DOMOTICZ-Structure.h>
#include <AFE-MQTT-Structure.h>

#define CONTACTRON_NO 0 // Normally open
#define CONTACTRON_NC 1 // Normally closed

#define CONTACTRON_OPEN 0   // Open
#define CONTACTRON_CLOSED 1 // Closed

#define AFE_DEFAULT_CONTACTRON_BOUNCING 200

struct CONTACTRON {
  uint8_t gpio;
  byte outputDefaultState = CONTACTRON_NO;
  uint8_t ledID;
  uint16_t bouncing = AFE_DEFAULT_CONTACTRON_BOUNCING;
  char name[17];
  DOMOTICZ_BASIC_CONFIG domoticz;
  MQTT_BASIC_CONFIG mqtt;
};

#endif
