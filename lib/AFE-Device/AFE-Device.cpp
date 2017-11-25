#include "AFE-Device.h"

AFEDevice::AFEDevice() {
  configuration = Data.getDeviceConfiguration();
  deviceMode = Data.getDeviceMode();
}

void AFEDevice::reboot(uint8_t mode) {
  saveMode(mode);
  delay(10);
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
