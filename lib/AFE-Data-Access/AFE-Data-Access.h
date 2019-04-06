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

#include <AFE-Configuration.h>
#include <AFE-Data-Structures.h>
//#include <AFE-EEPROM.h>
#include <IPAddress.h>

#include <ArduinoJson.h>
#include <FS.h>
#include <IPAddress.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEDataAccess {
private:
  // AFEEEPROM Eeprom;
  IPAddress IPfromString(const char *address);

public:
  AFEDataAccess();

  /* Methods saves and reads device ID */
  const String getDeviceUID();
  void saveDeviceUID(const char *);

  PASSWORD getPasswordConfiguration();
  void saveConfiguration(PASSWORD configuration);

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

  PRO_VERSION getProVersionConfiguration();
  void saveConfiguration(PRO_VERSION configuration);

#if !defined(T5_CONFIG)
  /* Methods reads and saves relay state from/to EEPROM */
  boolean getRelayState(uint8_t id);
  void saveRelayState(uint8_t id, boolean state);
#endif

  SWITCH getSwitchConfiguration(uint8_t id);
  void saveConfiguration(uint8_t id, SWITCH configuration);

  /* Methods saves firmware version from/to EEPROM */
  void saveVersion(const char *);

  /* Methods read and save device mode from/to EEPROM */
  uint8_t getDeviceMode();
  void saveDeviceMode(uint8_t mode);

/* Methods turns on / off APIs */
#ifdef CONFIG_FUNCTIONALITY_API_CONTROL
  void saveAPI(uint8_t apiID, boolean state);
#endif

#ifdef CONFIG_HARDWARE_LED
  LED getLEDConfiguration(uint8_t id);
  void saveConfiguration(uint8_t id, LED configuration);
  /* Methods read and save ID of system led */
  uint8_t getSystemLedID();
  void saveSystemLedID(uint8_t id);
#endif

#ifdef CONFIG_HARDWARE_DS18B20
  DS18B20 getSensorConfiguration();
  void saveConfiguration(DS18B20 configuration);
#endif

#ifdef CONFIG_HARDWARE_DHXX
  DH getSensorConfiguration();
  void saveConfiguration(DH configuration);
#endif

#ifdef CONFIG_FUNCTIONALITY_REGULATOR
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

#ifdef CONFIG_HARDWARE_UART
  SERIALPORT getSerialPortConfiguration();
  void saveConfiguration(SERIALPORT configuration);
#endif

#ifdef CONFIG_HARDWARE_HPMA115S0
  HPMA115S0 getHPMA115S0SensorConfiguration();
  void saveConfiguration(HPMA115S0 configuration);
#endif

#ifdef CONFIG_HARDWARE_BMX80
  BMx80 getBMx80SensorConfiguration();
  void saveConfiguration(BMx80 configuration);
#endif

#ifdef CONFIG_HARDWARE_BH1750
  BH1750 getBH1750SensorConfiguration();
  void saveConfiguration(BH1750 configuration);
#endif

#ifdef CONFIG_HARDWARE_ADC_VCC
  ADCINPUT getADCInputConfiguration();
  void saveConfiguration(ADCINPUT configuration);
#endif
};
#endif
