/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#ifndef _AFE_DEVICE_Structure_h
#define _AFE_DEVICE_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <ESP8266WiFi.h>

#define MODE_NORMAL 0
#define MODE_CONFIGURATION 1
#define MODE_ACCESS_POINT 2

struct DEVICE {
  char name[32];
  boolean mqttAPI;
  boolean httpAPI;
  boolean domoticzAPI;
  boolean isLED[5];
  boolean isRelay[4];
  boolean isSwitch[5];
  boolean isPIR[4];
  boolean isDS18B20;
  boolean isDHT;
};

#endif
