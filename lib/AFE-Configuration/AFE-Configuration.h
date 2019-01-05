/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_Configuration_h
#define _AFE_Configuration_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#if defined(T0_CONFIG)
#define FIRMWARE_VERSION "1.3.1"
#define FIRMWARE_TYPE 0
#elif defined(T0_SHELLY_1_CONFIG)
#define FIRMWARE_VERSION "1.3.0"
#define FIRMWARE_TYPE 0
#elif defined(T1_CONFIG)
#define FIRMWARE_VERSION "1.3.2"
#define FIRMWARE_TYPE 1
#elif defined(T2_CONFIG)
#define FIRMWARE_VERSION "1.3.4"
#define FIRMWARE_TYPE 2
#elif defined(T3_CONFIG)
#define FIRMWARE_TYPE 3
#define FIRMWARE_VERSION "1.3.0"
#elif defined(T4_CONFIG)
#define FIRMWARE_VERSION "1.3.1"
#define FIRMWARE_TYPE 4
#elif defined(T5_CONFIG)
#define FIRMWARE_VERSION "1.3.2"
#define FIRMWARE_TYPE 5
#elif defined(T6_CONFIG)
#define FIRMWARE_VERSION "1.0.0"
#define FIRMWARE_TYPE 6
#endif
#endif
