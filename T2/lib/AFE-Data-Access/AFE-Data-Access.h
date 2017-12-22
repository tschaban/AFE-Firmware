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

  /* Methods read configuration from EEPROM */
  DEVICE getDeviceConfiguration();
  FIRMWARE getFirmwareConfiguration();
  NETWORK getNetworkConfiguration();
  MQTT getMQTTConfiguration();
  LED getLEDConfiguration();
  LED getLEDConfiguration(uint8_t); // Used only to be compatible with AFE-LED.h
  RELAY getRelayConfiguration();
  SWITCH getSwitchConfiguration(uint8_t id);
  DH getDHTConfiguration();

  /* Methods save configuration to EEPROM */
  void saveConfiguration(DEVICE configuration);
  void saveConfiguration(FIRMWARE configuration);
  void saveConfiguration(NETWORK configuration);
  void saveConfiguration(MQTT configuration);
  void saveConfiguration(LED configuration);
  void saveConfiguration(RELAY configuration);
  void saveConfiguration(RELAYSTAT configuration, boolean thermostat);
  void saveConfiguration(uint8_t id, SWITCH configuration);
  void saveConfiguration(DH configuration);

  /* Methods read and save firmware version from/to EEPROM */
  const char getVersion();
  void saveVersion(String version);

  /* Methods read and save relay state from/to EEPROM */
  boolean getRelayState();
  void saveRelayState(boolean state);

  /* Methods read and save device mode from/to EEPROM */
  uint8_t getDeviceMode();
  void saveDeviceMode(uint8_t mode);

  /* Methods read and save firmware Language from/to EEPROM */
  uint8_t getLanguage();
  void saveLanguage(uint8_t language);

  /* Methods read and save thermostate state */
  boolean isThermostatEnabled();
  void saveThermostatState(boolean state);
  void saveThermostatState(
      uint8_t id,
      boolean state); // This is only for compatibility with AFE-Thermostat.h

  /* Methods read and save humidistat state */
  boolean isHumidistatEnabled();
  void saveHumidistatState(boolean state);
  void saveHumidistatState(
      uint8_t id,
      boolean state); // This is only for compatibility with AFE-Humidistat.h
};
#endif
