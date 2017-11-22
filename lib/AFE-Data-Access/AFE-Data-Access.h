/*

   AFE Firmware for smart home devices build on ESP8266
   More info: https://github.com/tschaban/AFE-Firmware
   LICENCE: http://opensource.org/licenses/MIT
 */

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
  NETWORK getNetworkConfiguration();
  MQTT getMQTTConfiguration();
  RELAY getRelayConfiguration(uint8_t id);
  SWITCH getSwitchConfiguration(uint8_t id);
  FIRMWARE getFirmwareConfiguration();
  LED getLEDConfiguration();

  // @TODO DOMOTICZ getDomoticzConfiguration();
  // @TODO DS18B20 getDS18B20Configuration();

  void saveConfiguration(NETWORK configuration);
  void saveConfiguration(MQTT configuration);
  void saveConfiguration(uint8_t id, RELAY configuration);
  void saveConfiguration(uint8_t id, SWITCH configuration);
  void saveConfiguration(FIRMWARE configuration);
  void saveConfiguration(LED configuration);
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

  /* Those get and save information if HTTP API is turned On or Off */
  boolean getHTTPAPI();
  void saveHTTPAPI(boolean state);
};
#endif
