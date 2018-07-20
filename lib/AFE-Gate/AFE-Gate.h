/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

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
  /* Via this class there is access to contactrons */
  AFEContactron Contactron[sizeof(Device.configuration.isContactron)];

  /* Constructors */
  AFEGate();

  /* Iniializing gate */
  void begin();

  /* Returns gate state based on contactron state */
  uint8_t get();

  /* Returns true if gate state has changed */
  boolean event();

  /* It should be added to main loop to listen for gate state changes and
   * request to be processed by the class */
  void listener();
};

#endif
