/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#ifndef _AFE_GATE_Structure_h
#define _AFE_GATE_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define GATE_OPEN 0           // Open
#define GATE_CLOSED 1         // Closed
#define GATE_PARTIALLY_OPEN 2 // Being opened

struct GATE {
  uint8_t state[4];
};

#endif
