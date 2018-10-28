/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_Data_Access_h
#define _AFE_Data_Access_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Structures.h>
#include <AFE-EEPROM.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

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
  DOMOTICZ getDomoticzConfiguration();
#ifndef T0_SHELLY_1_CONFIG
  LED getLEDConfiguration(uint8_t id);
#endif
  RELAY getRelayConfiguration(uint8_t id);
  SWITCH getSwitchConfiguration(uint8_t id);

#ifdef T1_CONFIG
  DS18B20 getSensorConfiguration();
#endif

#ifdef T2_CONFIG
  DH getSensorConfiguration();
#endif

  /* Methods save configuration to EEPROM */
  void saveConfiguration(DEVICE configuration);
  void saveConfiguration(FIRMWARE configuration);
  void saveConfiguration(NETWORK configuration);
  void saveConfiguration(MQTT configuration);
  void saveConfiguration(DOMOTICZ configuration);
#ifndef T0_SHELLY_1_CONFIG
  void saveConfiguration(uint8_t id, LED configuration);
#endif
  void saveConfiguration(uint8_t id, RELAY configuration);
  void saveConfiguration(uint8_t id, SWITCH configuration);

#ifdef T1_CONFIG
  void saveConfiguration(DS18B20 configuration);
#endif

#ifdef T2_CONFIG
  void saveConfiguration(DH configuration);
#endif

#if defined(T1_CONFIG)
  void saveConfiguration(REGULATOR configuration);
#endif

#if defined(T2_CONFIG)
  void saveConfiguration(uint8_t id, REGULATOR configuration,
                         boolean thermostat);
#endif
  /* Methods saves firmware version from/to EEPROM */
  void saveVersion(String version);

  /* Methods reads and saves relay state from/to EEPROM */
  boolean getRelayState(uint8_t id);
  void saveRelayState(uint8_t id, boolean state);

  /* Methods read and save device mode from/to EEPROM */
  uint8_t getDeviceMode();
  void saveDeviceMode(uint8_t mode);

  /* Methods read and save firmware Language from/to EEPROM */
  uint8_t getLanguage();
  void saveLanguage(uint8_t language);

#if defined(T1_CONFIG) || defined(T2_CONFIG)
  /* Methods read and save thermostate state */
  boolean isThermostatEnabled(uint8_t id);
  void saveThermostatState(uint8_t id, boolean state);
#endif

#if defined(T2_CONFIG)
  boolean isHumidistatEnabled(uint8_t id);
  void saveHumidistatState(uint8_t id, boolean state);
#endif

#ifndef T0_SHELLY_1_CONFIG
  /* Methods read and save ID of system led */
  uint8_t getSystemLedID();
  void saveSystemLedID(uint8_t id);
#endif

  /* Methods saves and reads device ID */
  const String getDeviceID();
  void saveDeviceID(String id);

  /* Methods turns on / off APIs */
  void saveAPI(uint8_t apiID, boolean state);
};
#endif
