/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#include "AFE-Upgrader.h"

AFEUpgrader::AFEUpgrader() {
  FirmwareConfiguration = Data.getFirmwareConfiguration();
}

boolean AFEUpgrader::upgraded() {

  if (String(FirmwareConfiguration.version) ==
          String(Defaults.getFirmwareVersion()) &&
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
    if (strcmp(Defaults.getFirmwareVersion(), "1.0rc3") ==
        0) { // While upgrading to this version EPPROM is cleared
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
