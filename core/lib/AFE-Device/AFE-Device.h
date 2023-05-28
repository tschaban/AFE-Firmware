/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_Device_h
#define _AFE_Device_h

//#include <AFE-Defaults.h>
#include <AFE-Data-Access.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEDevice {

private:
  AFEDataAccess Data;
  uint8_t deviceMode;

public:
  DEVICE configuration;

  /* This param is set by Upgrade class, if the frmware has been upgraded. It's
   * used to show a message in the config panel */
  uint8_t upgraded = AFE_UPGRADE_NONE;

  AFEDevice();

  /* Method reads device configuration data. It used in config panel */
  void begin();

  /* Method reboots device to specyfic mode  define by MODE_.. */
  void reboot(uint8_t mode = AFE_MODE_NORMAL);

  /* Method returns current device mode */
  uint8_t getMode();

  /* Method saves current device mode to EEPROM */
  void saveMode(uint8_t mode);


};

#endif