/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_Device_h
#define _AFE_Device_h

// #include <AFE-Defaults.h>
#include <AFE-Data-Access.h>

#ifdef DEBUG
#include <AFE-Debugger.h>
#endif

class AFEDevice
{

private:
  AFEDataAccess *Data;
  #ifdef DEBUG
  AFEDebugger *Debugger;
  #endif
  uint8_t deviceMode;

/* Method reads device configuration data. It used in config panel */
#ifdef DEBUG
  void begin(AFEDataAccess *);
#endif

public:
  DEVICE configuration;

  /* This param is set by Upgrade class, if the frmware has been upgraded. It's
   * used to show a message in the config panel */
  uint8_t upgraded = AFE_UPGRADE_NONE;

  AFEDevice();

/* Method reads device configuration data. It used in config panel */
#ifdef DEBUG
  void begin(AFEDataAccess *, AFEDebugger *);
#else
  void begin(AFEDataAccess *);
#endif

  /* Method refreshes configuration */
  void refreshConfiguration();

  /* Method reboots device to specyfic mode  define by MODE_.. */
  void reboot(uint8_t mode = AFE_MODE_NORMAL);

  /* Method returns current device mode */
  uint8_t getMode();

  /* Method saves current device mode to EEPROM */
  void saveMode(uint8_t mode);
};

#endif