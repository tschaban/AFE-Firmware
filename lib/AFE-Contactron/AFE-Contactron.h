/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#ifndef _AFE_Contactron_h
#define _AFE_Contactron_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-CONTACTRON-Structure.h>
#include <AFE-Data-Access.h>
#include <AFE-LED.h>
//#include <Streaming.h>

class AFEContactron {

private:
  CONTACTRON ContractonConfiguration;
  boolean _initialized = false;
  boolean state;
  boolean previousState; // Actually this stores current cotactron state

  unsigned long startTime = 0;

  boolean connected =
      false; // It's set to true once button connected physically
  boolean _connected = false;

  AFELED ContactronLed;

public:
  /* Constructors */
  AFEContactron();
  AFEContactron(uint8_t id);

  void begin(uint8_t id);

  boolean getState();

  boolean changed();

  /* Method has to be added to the loop in order to listen for switch changes */
  void listener();
};

#endif
