#include "AFE-Device.h"

AFEDevice::AFEDevice() { deviceMode = Data.getDeviceMode(); }

void AFEDevice::reboot(uint8_t mode) {
  Serial << endl << "WARN: Rebooting starting";
  Serial << endl << "INFO: saveing device mode: " << mode;
  saveMode(mode);
  delay(10);
  Serial << endl << "WARN: Sending kill signal ";
  ESP.restart();
}

uint8_t AFEDevice::getMode() { return deviceMode; }

void AFEDevice::saveMode(uint8_t mode) { Data.saveDeviceMode(mode); }

boolean AFEDevice::isFirstTimeLaunch() {
  uint8_t mode = Data.getDeviceMode();
  Serial << endl << "INFO: Checking if device is configured. [" << mode << "]";
  return mode == 0 ? false : mode == 1 ? false : mode == 2 ? false : true;
}

void AFEDevice::setDevice() {
  AFEDefaults Defaults;
  Defaults.eraseConfiguration();
  Defaults.set();
}

boolean AFEDevice::isConfigured() {
  NETWORK configuration = Data.getNetworkConfiguration();

  Serial << endl
         << "strlen(configuration.ssid) = " << strlen(configuration.ssid);

  if (strlen(configuration.ssid) == 0 || strlen(configuration.password) == 0) {
    return false;
  } else {
    return true;
  }
}
