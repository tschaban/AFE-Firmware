/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#include "AFE-Upgrader.h"

AFEUpgrader::AFEUpgrader() {
  FirmwareConfiguration = Data.getFirmwareConfiguration();
}

boolean AFEUpgrader::upgraded() {

  if (strcmp(FirmwareConfiguration.version, Defaults.getFirmwareVersion()) ==
          0 &&
      FirmwareConfiguration.type == Defaults.getFirmwareType()) {
    return false;
  } else {
    return true;
  }
}

void AFEUpgrader::upgrade() {
  if (FirmwareConfiguration.type != Defaults.getFirmwareType()) {
    upgradeTypeOfFirmware();
  } else {
    if (strcmp(FirmwareConfiguration.version, "1.0rc1") == 0 &&
        strcmp(FirmwareConfiguration.version, "1.0rc2") == 0) {
      Defaults.eraseConfiguration();
      Defaults.set();
    } else {
      Data.saveVersion(String(Defaults.getFirmwareVersion()));
    }
  }
}

void AFEUpgrader::upgradeTypeOfFirmware() {
  NETWORK NetworkConfiguration;
  NetworkConfiguration = Data.getNetworkConfiguration();
  uint8_t language = Data.getLanguage();
  Defaults.eraseConfiguration();
  Defaults.set();
  Data.saveConfiguration(NetworkConfiguration);
  Data.saveDeviceMode(Data.getDeviceMode());
  Data.saveLanguage(language);
}
