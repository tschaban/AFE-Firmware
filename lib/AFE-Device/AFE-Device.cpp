/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#include "AFE-Device.h"

AFEDevice::AFEDevice() {
  deviceMode = Data.getDeviceMode();
  begin();
}

void AFEDevice::begin() { configuration = Data.getDeviceConfiguration(); }

void AFEDevice::reboot(uint8_t mode) {
  saveMode(mode);
  delay(1000);
  ESP.restart();
}

uint8_t AFEDevice::getMode() { return deviceMode; }

void AFEDevice::saveMode(uint8_t mode) { Data.saveDeviceMode(mode); }

boolean AFEDevice::isFirstTimeLaunch() {
  uint8_t mode = Data.getDeviceMode();
  return mode == 0 ? false : mode == 1 ? false : mode == 2 ? false : true;
}

void AFEDevice::setDevice() {
  AFEDefaults Defaults;
  Defaults.eraseConfiguration();
  Defaults.formatSPIFFS();
  Defaults.set();
}

boolean AFEDevice::isConfigured() {
  NETWORK configuration = Data.getNetworkConfiguration();
  if (strlen(configuration.ssid) == 0 || strlen(configuration.password) == 0) {
    return false;
  } else {
    return true;
  }
}

#ifdef CONFIG_HARDWARE_SPIFFS
boolean AFEDevice::checkConfiguration() {
  AFEDefaults Defaults;
#ifdef CONFIG_HARDWARE_ADC_VCC

  File configFile = SPIFFS.open("/cfg-device.json", "r");
  if (!configFile) {
#ifdef DEBUG
    Serial << endl << "Adding default configuration for: device";
#endif
    Defaults.addDeviceDefaultConfiguration();
  } else {
    configFile.close();
  }

  configFile = SPIFFS.open("/cfg-analog-input.json", "r");
  if (!configFile) {
#ifdef DEBUG
    Serial << endl << "Adding default configuration for: analog input";
#endif
    Defaults.addAnalogInputDefaultConfiguration();
  } else {
    configFile.close();
  }

#endif
  return true;
}
#endif
