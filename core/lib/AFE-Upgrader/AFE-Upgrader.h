/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_UPGRADER_h
#define _AFE_UPGRADER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>
#include <AFE-Device.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEUpgrader {

private:
  FIRMWARE FirmwareConfiguration;
  AFEDataAccess *Data;
  AFEDevice *Device;

  /* Upgrades configuration files structure after firmware type change */
  void upgradeFirmwarType();

  /* Upgrades configuration files structure after firmware version change */
  void updateFirmwareVersion();

  /* Upgrades configuration files structure after firmware API Change */
  void updateFirmwareAPIVersion();

#ifdef T0_CONFIG
  /* Upgrades firmware from version T0-2.0.x to T0-2.1.x */
  void upgradeToT0V210();
#endif

#ifdef T5_CONFIG
  /* Upgrades firmware from version T5-2.2.x */
  void upgradeToT5V220();
#endif

#ifdef T6_CONFIG
  void upgradeToT6V230();
#endif

public:
  /* Constructor */
  AFEUpgrader(AFEDataAccess *, AFEDevice *);

  /* Checks if firmware has been upgraded */
  boolean upgraded();

  /* Perform post upgrade changes */
  void upgrade();
};

#endif
