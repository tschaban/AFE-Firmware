/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_DEVICE_Structure_h
#define _AFE_DEVICE_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define MODE_NORMAL 0
#define MODE_CONFIGURATION 1
#define MODE_ACCESS_POINT 2

#define API_HTTP 0
#define API_MQTT 1
#define API_DOMOTICZ 2

/* Custom DEVICE structure definition */
struct DEVICE {
  char name[32];
  boolean mqttAPI;
  boolean httpAPI;
  boolean domoticzAPI;
#if defined(T0_CONFIG)
  boolean isLED[2];
  boolean isSwitch[2];
  boolean isRelay[1];
#elif defined(T0_SHELLY_1_CONFIG)
  boolean isLED[0];
  boolean isSwitch[1];
  boolean isRelay[1];
#elif defined(T1_CONFIG)
  boolean isLED[2];
  boolean isRelay[1];
  boolean isSwitch[2];
  boolean isDS18B20;
#elif defined(T2_CONFIG)
  boolean isLED[2];
  boolean isRelay[1];
  boolean isSwitch[2];
  boolean isDHT;
#elif defined(T3_CONFIG)
  boolean isLED[5];
  boolean isRelay[4];
  boolean isSwitch[5];
  boolean isPIR[4];
#elif defined(T4_CONFIG)
  boolean isLED[5];
  boolean isSwitch[5];
  boolean isRelay[4];
#elif defined(T5_CONFIG)
  boolean isLED[3];
  boolean isSwitch[2];
  boolean isRelay[1];
  boolean isContactron[2];
  boolean isDHT;
#elif defined(T6_CONFIG)
  boolean isLED[2];
  boolean isSwitch[2];
  boolean isRelay[1];
  boolean isHPMA115S0;
  boolean isBME680;
#endif
};

#endif
