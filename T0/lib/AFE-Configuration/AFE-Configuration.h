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

#define FIRMWARE_TYPE 0

#ifdef SHELLY_1_DEVICE
#define FIRMWARE_VERSION "S1.0.0"
#else
#define FIRMWARE_VERSION "1.2.3"
#endif

#endif
