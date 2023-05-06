/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-Device.h"

AFEDevice::AFEDevice() {}

void AFEDevice::begin() {
  deviceMode = Data.getDeviceMode();
  Data.getConfiguration(&configuration);

}

void AFEDevice::reboot(uint8_t mode) {
  saveMode(mode);
  yield();
#ifdef DEBUG
  Serial << endl
         << endl
         << F("##################################################") << endl
         << F("####      INFO: Rebooting device in 1sec      ####") << endl
         << F("##################################################") << endl
         << endl;
#endif
  delay(1000);
  ESP.restart();
}

uint8_t AFEDevice::getMode() { return deviceMode; }

void AFEDevice::saveMode(uint8_t mode) { Data.saveDeviceMode(mode); }

boolean AFEDevice::setDevice(void) {
  AFEDefaults Defaults;
  return Defaults.set();
}
