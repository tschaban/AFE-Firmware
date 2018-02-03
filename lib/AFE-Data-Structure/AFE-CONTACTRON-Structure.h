/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

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
  uint8_t outputDefaultState;
  uint8_t function;
  uint8_t ledID;
  uint16_t bouncing;
};

#endif
