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
        strcmp(FirmwareConfiguration.version, "1.2.0") == 0) {
      upgradeToVersion121();
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

void AFEUpgrader::upgradeToVersion121() {
  AFEEEPROM Eeprom;

  /* LEDs */
  Eeprom.write(464, false);
  Defaults.addLEDConfiguration(1, 3);
  Data.saveSystemLedID(1);

  /* Set that both switces controls relay 1 */
  Eeprom.writeUInt8(461, 1);
  Eeprom.writeUInt8(462, 1);

  /* Set that none of led informs about status of a relay */
  Eeprom.writeUInt8(430, 0);

  /* Upgrade to new switch functionality codes */
  if (Eeprom.readUInt8(396) == 11) {
    Eeprom.writeUInt8(396, 1);
  }
  if (Eeprom.readUInt8(403) == 11) {
    Eeprom.writeUInt8(403, 1);
  }

  /* Set sending temperature only if it changes */
  Eeprom.write(467, true);

  /* Add Domoticz default config */
  Eeprom.write(800, false);
  Defaults.addDomoticzConfiguration();

  /* Device ID */
  if (Data.getDeviceID().length() == 0) {
    Defaults.addDeviceID();
  }
}
