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

#if !(defined(T0_SHELLY_1_CONFIG) || defined(T6_CONFIG))

    if (
#if defined(T0_CONFIG) || defined(T4_CONFIG)
        strcmp(FirmwareConfiguration.version, "1.0.0") == 0 ||
        strcmp(FirmwareConfiguration.version, "1.0.1") == 0
#elif defined(T1_CONFIG) || defined(T3_CONFIG)
        strcmp(FirmwareConfiguration.version, "1.0.0") == 0 ||
        strcmp(FirmwareConfiguration.version, "1.0.1") == 0 ||
        strcmp(FirmwareConfiguration.version, "1.0.2") == 0
#elif defined(T2_CONFIG)
        strcmp(FirmwareConfiguration.version, "1.0.0") == 0 ||
        strcmp(FirmwareConfiguration.version, "1.2.0")
#elif defined(T5_CONFIG)
        strcmp(FirmwareConfiguration.version, "1.0.0") == 0
#endif
    ) {

#ifdef T1_CONFIG /* Upgrade T1 1.0.x to version T1.1.0 */
      upgradeToVersion110();
#elif defined(T0_CONFIG) || defined(T2_CONFIG) || defined(T4_CONFIG)
      upgradeToVersion120();
#elif defined(T3_CONFIG) || defined(T5_CONFIG)
      upgradeToVersion130();
#endif
    }

/* Upgrade from version T1-1.1.0 to 1.2.0 */
#ifdef T1_CONFIG
    if (strcmp(FirmwareConfiguration.version, "1.1.0") == 0) {
      upgradeToVersion120();
    }
#endif

/* Upgrade from version T2 - 1.2.x to 1.3.0 */
#ifdef T2_CONFIG
    if (strcmp(FirmwareConfiguration.version, "1.2.1") == 0 ||
        strcmp(FirmwareConfiguration.version, "1.2.2") == 0 ||
        strcmp(FirmwareConfiguration.version, "1.2.3") == 0 ||
        strcmp(FirmwareConfiguration.version, "1.2.4")) {
      upgradeToVersion130();
    }
#endif

#endif

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

/* Upgrade from version T1 1.1.x to 1.1.0 */
#ifdef T1_CONFIG
void AFEUpgrader::upgradeToVersion110() {
  AFEEEPROM Eeprom;

  /* LEDs */
  Eeprom.write(443, false);
  Defaults.addLEDConfiguration(1, 3);
  Data.saveSystemLedID(1);

  /* Set that both switces controls relay 1 */
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

  /* Set sending temperature only if it changes */
  Eeprom.write(446, true);
}
#endif

/* Upgrade to version 1.2.0 */
#if defined(T0_CONFIG) || defined(T1_CONFIG) || defined(T2_CONFIG) ||          \
    defined(T4_CONFIG)

void AFEUpgrader::upgradeToVersion120() {
  AFEEEPROM Eeprom;
#ifndef T1_CONFIG
  DEVICE deviceConfiguration;
#endif

#ifdef T0_CONFIG
  /* LEDs */
  Eeprom.write(418, false);
  Defaults.addLEDConfiguration(1, 3);
  Data.saveSystemLedID(1);

  /* Set that both switches controls relay 1 */
  Eeprom.writeUInt8(416, 1);
  Eeprom.writeUInt8(417, 1);

  /* Set that none of led informs about status of a relay */
  Eeprom.writeUInt8(421, 0);
#endif

#ifdef T2_CONFIG
  /* LEDs */
  Eeprom.write(464, false);
  Defaults.addLEDConfiguration(1, 3);
  Data.saveSystemLedID(1);

  /* Set that both switces controls relay 1 */
  Eeprom.writeUInt8(461, 1);
  Eeprom.writeUInt8(462, 1);

  /* Set that none of led informs about status of a relay */
  Eeprom.writeUInt8(430, 0);
#endif

#if defined(T0_CONFIG) || defined(T2_CONFIG) || defined(T4_CONFIG)
  /* Upgrade to new switch functionality codes */
  for (uint8_t i = 0; i < sizeof(deviceConfiguration.isSwitch); i++) {

#ifdef T0_CONFIG
    if (Eeprom.readUInt8(401 + i * 7) == 11) {
      Eeprom.writeUInt8(401 + i * 7, 1);
    }
#elif T2_CONFIG
    if (Eeprom.readUInt8(396 + i * 7) == 11) {
      Eeprom.writeUInt8(396 + i * 7, 1);
    }
#elif T4_CONFIG
    if (Eeprom.readUInt8(496 + i * 8) == 11) {
      Eeprom.writeUInt8(496 + i * 8, 1);
    }
#endif
  }
#endif

  /* T0, T1, T4 */

  /* Add Domoticz default config */
  Eeprom.write(800, false);
  Defaults.addDomoticzConfiguration();

  /* Device ID */
  if (Data.getDeviceID().length() == 0) {
    Defaults.addDeviceID();
  }

#if defined(T2_CONFIG)
  /* Set sending temperature only if it changes */
  Eeprom.write(467, true);
  /* Publish HeatIndex - no */
  Eeprom.write(974, false);
  /* IDXs */
  Eeprom.write(930, 6, (long)0);
  Eeprom.write(936, 6, (long)0);
  Eeprom.write(942, 6, (long)0);
  Eeprom.write(948, 6, (long)0);

#endif

#if defined(T4_CONFIG)

  /* Relay. Setting LED ID and IDX */
  for (uint8_t i = 0; i < sizeof(deviceConfiguration.isRelay); i++) {
    Eeprom.writeUInt8(531 + i, 0);
    Eeprom.write(930 + 6 * i, 6, (long)0);
  }

  Data.saveSystemLedID(1);
#endif
}

#endif

#if defined(T2_CONFIG) || defined(T3_CONFIG) || defined(T5_CONFIG)
/* Methods upgrades to v1.3.0 */
void AFEUpgrader::upgradeToVersion130() {
  AFEEEPROM Eeprom;

#if defined(T2_CONFIG) || defined(T5_CONFIG)
  /* Setting default value for publishing DewPoint */
  Eeprom.write(382, false);
#endif

#if defined(T3_CONFIG) || defined(T5_CONFIG)
  DEVICE deviceConfiguration;

#if defined(T3_CONFIG)
  /* Switch functionality codes */
  for (uint8_t i = 0; i < sizeof(deviceConfiguration.isSwitch); i++) {
    if (Eeprom.readUInt8(476 + i * 7) == 11) {
      Eeprom.writeUInt8(476, 1);
    }
    /* Relay set to 1 for first switch and None for remaining */
    i == 0 ? Eeprom.writeUInt8(622, 1) : Eeprom.writeUInt8(622 + i, 0);
  }

  /* None of LEDs indicates Relay status */
  for (uint8_t i = 0; i < sizeof(deviceConfiguration.isRelay); i++) {
    Eeprom.writeUInt8(618 + i, 0);
  }

  /* Setting PIR Type NO */
  for (uint8_t i = 0; i < sizeof(deviceConfiguration.isPIR); i++) {
    Eeprom.writeUInt8(613 + i, 0);
  }

#else
  /* Switch functionality codes */
  for (uint8_t i = 0; i < sizeof(deviceConfiguration.isSwitch); i++) {
    if (Eeprom.readUInt8(496 + i * 8) == 11) {
      Eeprom.writeUInt8(496, 1);
    }
  }
  Data.saveGateState(0);
#endif

  /* Add Domoticz default config */
  Eeprom.write(800, false);
  Defaults.addDomoticzConfiguration();

  /* Add device ID */
  if (Data.getDeviceID().length() == 0) {
    Defaults.addDeviceID();
  }

  /* Adding domoticz IDX for devces
  T5: Gate, Contractons, DHxx. 930, 972 - are not neccessary for T5, but made
  anyway
  T3: 4 Relay, 4 PIRs */
  for (uint8_t i = 0; i < 8; i++) {
    Eeprom.write(930 + i * 6, 6, (long)0);
  }

  Data.saveSystemLedID(1);
#endif
}
#endif
