/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_UART_h
#define _AFE_UART_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>
#include <AFE-SoftwareSerial.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEUART {

private:
  AFEDataAccess Data;

public:
  AFESoftwareSerial SerialBus;

  /* Constructor */
  AFEUART();
  void begin();

  /* Method sends data to Serial Port */
  void send(const uint8_t *cmd);
};

#endif
