/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#ifndef _AFE_Gate_h
#define _AFE_Gate_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "AFE-Contactron.h"
#include <AFE-Data-Access.h>
#include <AFE-Device.h>
#include <AFE-GATE-Structure.h>
//#include <Streaming.h>

class AFEGate {
  AFEDevice Device;
  AFEDataAccess Data;
  GATE gateConfiguration;

  uint8_t numberOfContractors = 0;

  boolean _event = false;

public:
  AFEContactron Contactron[sizeof(Device.configuration.isContactron)];

  /* Constructors */

  AFEGate();

  void begin();

  uint8_t get();

  boolean event();

  void listener();
};

#endif
