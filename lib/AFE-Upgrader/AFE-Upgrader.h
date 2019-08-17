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
  void upgradeFirmwarType();
  void updateFirmwareVersion();

public:
  /* Constructor */
  AFEUpgrader(AFEDataAccess *, AFEDevice *);

  /* Checks if firmware has been upgraded */
  boolean upgraded();

  /* Perform post upgrade changes */
  void upgrade();
};

#endif
