/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

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
  if (FirmwareConfiguration.type != FIRMWARE_TYPE) {
    upgradeTypeOfFirmware();
  } else {
    if (strcmp(FirmwareConfiguration.version, "1.0.0") == 0 ||
        strcmp(FirmwareConfiguration.version, "1.0.1")) {
      upgradeToVersion110();
    }
    Data.saveVersion(String(FIRMWARE_VERSION));
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

void AFEUpgrader::upgradeToVersion110() {
  AFEEEPROM Eeprom;

  /* Add second LED default config */
  LED LEDConfiguration;
  LEDConfiguration.gpio = 3;
  LEDConfiguration.changeToOppositeValue = false;
  Data.saveConfiguration(1, LEDConfiguration);
  Eeprom.write(443, false);

  /* Set first led as system one */
  Data.saveSystemLedID(1);

  /* Set that both switches controls relay 1 */
  Eeprom.writeUInt8(440, 1);
  Eeprom.writeUInt8(441, 1);

  /* Set that none of led informs about status of a relay */
  Eeprom.writeUInt8(442, 0);

  /* Upgrade to new switch functionality codes */
  if (Eeprom.readUInt8(388) == 11) {
    Eeprom.writeUInt8(388, 1);
  }
  if (Eeprom.readUInt8(395) == 11) {
    Eeprom.writeUInt8(395, 1);
  }
}
