/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#ifndef _AFE_Data_Access_h
#define _AFE_Data_Access_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Structures.h>
#include <AFE-EEPROM.h>
#include <Streaming.h>

class AFEDataAccess {
private:
  AFEEEPROM Eeprom;

public:
  AFEDataAccess();
  DEVICE getDeviceConfiguration();
  FIRMWARE getFirmwareConfiguration();
  NETWORK getNetworkConfiguration();
  MQTT getMQTTConfiguration();
  LED getLEDConfiguration(uint8_t id);
  RELAY getRelayConfiguration(uint8_t id);
  SWITCH getSwitchConfiguration(uint8_t id);
  // @TODO DOMOTICZ getDomoticzConfiguration();
  // @TODO DS18B20 getDS18B20Configuration();

  void saveConfiguration(DEVICE configuration);
  void saveConfiguration(FIRMWARE configuration);
  void saveConfiguration(NETWORK configuration);
  void saveConfiguration(MQTT configuration);
  void saveConfiguration(uint8_t id, LED configuration);
  void saveConfiguration(uint8_t id, RELAY configuration);
  void saveConfiguration(uint8_t id, SWITCH configuration);
  // @TODO DOMOTICZ void saveConfiguration(DOMOTICZ configuration);
  // @TODO DS18B20 void saveConfiguration(DS18B20 configuration);

  const char getVersion();
  void saveVersion(String version);

  boolean getRelayState(uint8_t id);
  void saveRelayState(uint8_t id, boolean state);

  uint8_t getDeviceMode();
  void saveDeviceMode(uint8_t mode);

  uint8_t getLanguage();
  void saveLanguage(uint8_t language);
};
#endif
