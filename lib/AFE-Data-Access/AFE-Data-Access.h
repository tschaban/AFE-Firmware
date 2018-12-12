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

  DEVICE getDeviceConfiguration();
  void saveConfiguration(DEVICE configuration);

  FIRMWARE getFirmwareConfiguration();
  void saveConfiguration(FIRMWARE configuration);

  NETWORK getNetworkConfiguration();
  void saveConfiguration(NETWORK configuration);

  MQTT getMQTTConfiguration();
  void saveConfiguration(MQTT configuration);

  DOMOTICZ getDomoticzConfiguration();
  void saveConfiguration(DOMOTICZ configuration);

  RELAY getRelayConfiguration(uint8_t id);
  void saveConfiguration(uint8_t id, RELAY configuration);

#if !defined(T5_CONFIG)
  /* Methods reads and saves relay state from/to EEPROM */
  boolean getRelayState(uint8_t id);
  void saveRelayState(uint8_t id, boolean state);
#endif

  SWITCH getSwitchConfiguration(uint8_t id);
  void saveConfiguration(uint8_t id, SWITCH configuration);

  /* Methods saves firmware version from/to EEPROM */
  void saveVersion(String version);

  /* Methods read and save device mode from/to EEPROM */
  uint8_t getDeviceMode();
  void saveDeviceMode(uint8_t mode);

  /* Methods read and save firmware Language from/to EEPROM */
  uint8_t getLanguage();
  void saveLanguage(uint8_t language);

  /* Methods saves and reads device ID */
  const String getDeviceID();
  void saveDeviceID(String id);

  /* Methods turns on / off APIs */
  void saveAPI(uint8_t apiID, boolean state);

#if !defined(T0_SHELLY_1_CONFIG)
  LED getLEDConfiguration(uint8_t id);
  void saveConfiguration(uint8_t id, LED configuration);
  /* Methods read and save ID of system led */
  uint8_t getSystemLedID();
  void saveSystemLedID(uint8_t id);
#endif

#if defined(T1_CONFIG)
  DS18B20 getSensorConfiguration();
  void saveConfiguration(DS18B20 configuration);
#endif

#if defined(T2_CONFIG) || defined(T5_CONFIG)
  DH getSensorConfiguration();
  void saveConfiguration(DH configuration);
#endif

#if defined(T1_CONFIG) || defined(T2_CONFIG)
  /* Method saves regulator configuration */
  void saveConfiguration(REGULATOR configuration,
                         uint8_t type = THERMOSTAT_REGULATOR);
  /* Methods read and save regulator state.
  Regulator can be Thermostat or humidistat */
  boolean isRegulatorEnabled(uint8_t type = THERMOSTAT_REGULATOR);
  void saveRegulatorState(boolean state, uint8_t type = THERMOSTAT_REGULATOR);
#endif

#if defined(T3_CONFIG)
  PIR getPIRConfiguration(uint8_t id);
  void saveConfiguration(uint8_t id, PIR configuration);
#endif

#if defined(T5_CONFIG)
  CONTACTRON getContactronConfiguration(uint8_t id);
  void saveConfiguration(uint8_t id, CONTACTRON configuration);
  GATE getGateConfiguration();
  void saveConfiguration(GATE configuration);
  uint8_t getGateState();
  void saveGateState(uint8_t state);
#endif

#if defined(T6_CONFIG)
  SERIALPORT getSerialPortConfiguration();
  void saveConfiguration(SERIALPORT configuration);
  HPMA115S0 getHPMA115S0SensorConfiguration();
  void saveConfiguration(HPMA115S0 configuration);
  BME680 getBME680SensorConfiguration();
  void saveConfiguration(BME680 configuration);
  BH1750 getBH1750SensorConfiguration();
  void saveConfiguration(BH1750 configuration);

#endif
};
#endif
