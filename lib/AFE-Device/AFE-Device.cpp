#include "AFE-Device.h"

AFEDevice::AFEDevice() { deviceMode = Data.getDeviceMode(); }

void AFEDevice::reboot(uint8_t mode) {
  Serial << endl << "WARN: Rebooting starting";
  Serial << endl << "INFO: saveing device mode: " << mode;
  saveMode(mode);
  delay(500);
  Serial << endl << "WARN: Sending kill signal ";
  ESP.restart();
}

uint8_t AFEDevice::getMode() { return deviceMode; }
void AFEDevice::saveMode(uint8_t mode) { Data.saveDeviceMode(mode); }
