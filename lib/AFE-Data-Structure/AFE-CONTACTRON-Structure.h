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

#define CONTACTRON_NO 0 // Normally open
#define CONTACTRON_NC 1 // Normally closed

#define CONTACTRON_OPEN 0   // Open
#define CONTACTRON_CLOSED 1 // Closed

struct CONTACTRON {
  uint8_t gpio;
  byte outputDefaultState;
  uint8_t ledID;
  uint16_t bouncing;
  char name[16];
  char mqttTopic[49];
};

#endif
