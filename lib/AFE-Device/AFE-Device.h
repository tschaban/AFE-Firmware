/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_Device_h
#define _AFE_Device_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>
#include <AFE-Defaults.h>

#ifdef CONFIG_HARDWARE_SPIFFS
#include <AFE-Defaults.h>
#include <FS.h>
#endif

#ifdef DEBUG
#include <Streaming.h>
#endif

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

  /* Method sets the device to it's default state */
  void setDevice();
};

#endif
