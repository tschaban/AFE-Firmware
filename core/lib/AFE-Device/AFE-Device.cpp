/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-Device.h"

AFEDevice::AFEDevice() {}

void AFEDevice::begin(AFEDataAccess *_data, AFEDebugger *_debugger)
{
  Debugger = _debugger;
  begin(_data);
}

void AFEDevice::begin(AFEDataAccess *_data)
{
  Data = _data;
  refreshConfiguration();
}

void AFEDevice::refreshConfiguration()
{
  deviceMode = Data->getDeviceMode();
  Data->getConfiguration(&configuration);
}

void AFEDevice::reboot(uint8_t mode)
{
  saveMode(mode);
  yield();
#ifdef DEBUG
  Debugger->printHeader(2);
  Debugger->printValue(F("Rebooting device in 1sec"));
  Debugger->printHeader(1, 1);
#endif
  delay(1000);
  ESP.restart();
}

uint8_t AFEDevice::getMode() { return deviceMode; }

void AFEDevice::saveMode(uint8_t mode) { Data->saveDeviceMode(mode); }
