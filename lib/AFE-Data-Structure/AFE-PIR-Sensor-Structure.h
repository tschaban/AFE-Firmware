/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_PIR_Structure_h
#define _AFE_PIR_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

/* Relay states */
#define PIR_OPEN 1
#define PIR_CLOSE 0

#define PIR_NO 0
#define PIR_NC 1

struct PIR {
  uint8_t gpio;
  char name[17];
  boolean state;
  char mqttTopic[50];
  uint8_t ledId;
  uint8_t relayId;
  uint16_t howLongKeepRelayOn;
  boolean invertRelayState;
  byte type;
  uint16_t bouncing;
  unsigned long idx;
};

#endif
