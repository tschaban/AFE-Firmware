/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

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
    if (strcmp(FirmwareConfiguration.version, "1.0.0") == 0) {
      upgradeToVersion120();
    }
    Data.saveVersion(String(FIRMWARE_VERSION));
  }
}

void AFEUpgrader::upgradeTypeOfFirmware() {
  NETWORK NetworkConfiguration;
  NetworkConfiguration = Data.getNetworkConfiguration();
  uint8_t language = Data.getLanguage();
  String deviceID = Data.getDeviceID();
  Defaults.eraseConfiguration();
  Defaults.set();
  Data.saveConfiguration(NetworkConfiguration);
  Data.saveDeviceMode(Data.getDeviceMode());
  Data.saveLanguage(language);
  /* Restores previous device ID */
  if (deviceID.length() > 0) {
    Data.saveDeviceID(deviceID);
  }
}

void AFEUpgrader::upgradeToVersion120() {
  AFEEEPROM Eeprom;
  DEVICE deviceConfiguration;

  /* Relay. Setting LED ID and IDX */
  for (uint8_t i = 0; i < sizeof(deviceConfiguration.isRelay); i++) {
    Eeprom.writeUInt8(531 + i, 0);
  }

  /* Switch functionality codes */
  for (uint8_t i = 0; i < sizeof(deviceConfiguration.isSwitch); i++) {
    if (Eeprom.readUInt8(496 + i * 8) == 11) {
      Eeprom.writeUInt8(496, 1);
    }
  }

  /* Add Domoticz default config */
  Eeprom.write(800, false);
  Defaults.addDomoticzConfiguration();

  /* Add device ID */
  if (Data.getDeviceID().length() == 0) {
    Defaults.addDeviceID();
  }

  /* Adding domoticz config for devces */
  Eeprom.write(942, 6, (long)0); // Contactron 1
  Eeprom.write(948, 6, (long)0); // Contactron 2
  Eeprom.write(936, 6, (long)0); // Gate

  Data.saveGateState(0);
  Data.saveSystemLedID(1);
}
