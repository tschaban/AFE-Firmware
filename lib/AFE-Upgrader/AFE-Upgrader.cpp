/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Upgrader.h"

AFEUpgrader::AFEUpgrader(AFEDataAccess *_Data, AFEDevice *_Device) {
  Data = _Data;
  Device = _Device;
  FirmwareConfiguration = Data->getFirmwareConfiguration();
}

boolean AFEUpgrader::upgraded() {
  if (strcmp(FirmwareConfiguration.version, AFE_FIRMWARE_VERSION) == 0 &&
      FirmwareConfiguration.type == AFE_FIRMWARE_TYPE) {
    return false;
  } else {
    return true;
  }
}

void AFEUpgrader::upgrade() {
  if (FirmwareConfiguration.type != AFE_FIRMWARE_TYPE) {
#ifdef DEBUG
    Serial << endl << "Upgrading Firmware type";
#endif
    upgradeFirmwarType();
    Device->upgraded = AFE_UPGRADE_VERSION_TYPE;
  } else if (strcmp(FirmwareConfiguration.version, AFE_FIRMWARE_VERSION) != 0) {
#ifdef DEBUG
    Serial << endl << "Upgrading Firmware version";
#endif
    updateFirmwareVersion();
    Device->upgraded = AFE_UPGRADE_VERSION;
  }
#ifdef DEBUG
  Serial << endl << "Upgrade completed";
#endif
}

void AFEUpgrader::upgradeFirmwarType() {
  Data->createFirmwareConfigurationFile();
  Data->createDeviceConfigurationFile();

  Data->createSwitchConfigurationFile();

#ifdef AFE_CONFIG_HARDWARE_RELAY
  Data->createRelayConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  Data->createContractonConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
  Data->createGateConfigurationFile();
#endif
}

void AFEUpgrader::updateFirmwareVersion() {
  Data->saveVersion(AFE_FIRMWARE_VERSION);
}
