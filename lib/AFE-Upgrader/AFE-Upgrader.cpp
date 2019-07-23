/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

  

#include "AFE-Upgrader.h"

AFEUpgrader::AFEUpgrader() {
  FirmwareConfiguration = Data.getFirmwareConfiguration();
}

boolean AFEUpgrader::upgraded() {
  if (strcmp(FirmwareConfiguration.version, FIRMWARE_VERSION) == 0 &&
      FirmwareConfiguration.type == FIRMWARE_TYPE) {
    return false;
  } else {
    return true;
  }
}

void AFEUpgrader::upgrade() {
  sprintf(FirmwareConfiguration.version, FIRMWARE_VERSION);
  FirmwareConfiguration.type = FIRMWARE_TYPE;
  Data.saveConfiguration(&FirmwareConfiguration);
}

void AFEUpgrader::upgradeTypeOfFirmware() {}
