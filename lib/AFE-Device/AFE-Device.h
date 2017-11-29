/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#ifndef _AFE_Device_h
#define _AFE_Device_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>
#include <AFE-Data-Structures.h>
#include <AFE-Defaults.h>
//#include <Streaming.h>

#define MODE_NORMAL 0        // Device is in operating mode
#define MODE_CONFIGURATION 1 // Device is in configuration mode (within LAN)
#define MODE_ACCESS_POINT                                                      \
  2 // Device is in configuration mode (access point mode)

class AFEDevice {

private:
  AFEDataAccess Data;
  uint8_t deviceMode;

public:
  DEVICE configuration;

  AFEDevice();

  /* Method reads device configuration data. It used in config panel */
  void begin();

  /* Method reboots device to specyfic mode  define by MODE_.. */
  void reboot(uint8_t mode = MODE_NORMAL);

  /* Method returns current device mode */
  uint8_t getMode();

  /* Method saves current device mode to EEPROM */
  void saveMode(uint8_t mode);

  /* Method returns true if device is launched for a first time */
  boolean isFirstTimeLaunch();

  /* Method sets the device to it's default state */
  void setDevice();

  /* Is WiFi configured */
  boolean isConfigured();
};

#endif
