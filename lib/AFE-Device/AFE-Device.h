/*
  Device
  AFE Firmware for smart home devices build on ESP8266
  More info: https://github.com/tschaban/AFE-Firmware
  LICENCE: http://opensource.org/licenses/MIT
*/

#ifndef _AFE_Device_h
#define _AFE_Device_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>
#include <Streaming.h>

#define MODE_NORMAL 0
#define MODE_CONFIGURATION 1
#define MODE_ACCESS_POINT 2

class AFEDevice {

private:
  AFEDataAccess Data;
  uint8_t deviceMode;

public:
  AFEDevice();
  void reboot(uint8_t mode = MODE_NORMAL);
  uint8_t getMode();
  void saveMode(uint8_t mode);
};

#endif
