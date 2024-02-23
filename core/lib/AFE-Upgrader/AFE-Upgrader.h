/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_UPGRADER_h
#define _AFE_UPGRADER_h

#include <AFE-Firmware.h>


class AFEUpgrader {

private:
  AFEFirmware *Firmware;

  /* Upgrades configuration files structure after firmware type change */
  void upgradeFirmwarType();

  /* Upgrades configuration files structure after firmware version change */
  void updateFirmwareVersion();

  /* Upgrades configuration files structure after firmware API Change */
  void updateFirmwareAPIVersion();

  /* Perform post upgrade changes */
  void upgrade();

#ifndef AFE_ESP32

#ifdef T5_CONFIG
  /* Upgrades firmware from version T5-2.2.x */
  void upgradeToT5V220();
#endif

#ifdef T6_CONFIG
  void upgradeToT6V230();
  void upgradeToT6V250();
#endif

#endif // !ESP32

public:
  /* Constructor */
  AFEUpgrader(AFEFirmware *);

  /* Checks if firmware has been upgraded */
  void upgraded();
};

#endif
