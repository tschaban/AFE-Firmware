/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Data_Access_h
#define _AFE_Data_Access_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Configuration.h>
#include <AFE-Data-Structures.h>
#include <ESP8266WiFi.h>
#include <IPAddress.h>

#include <ArduinoJson.h>
#include <FS.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEDataAccess {
private:
  // AFEEEPROM Eeprom;
  IPAddress IPfromString(const char *address);

public:
  AFEDataAccess();

  boolean formatFileSystem();

  const String getDeviceUID();
  void saveDeviceUID(const char *);
  void createDeviceUIDFile();

  DEVICE getDeviceConfiguration();
  void saveConfiguration(DEVICE *);
  void createDeviceConfigurationFile();

  FIRMWARE getFirmwareConfiguration();
  void saveConfiguration(FIRMWARE *);
  void createFirmwareConfigurationFile();

  NETWORK getNetworkConfiguration();
  void saveConfiguration(NETWORK configuration);
  void createNetworkConfigurationFile();

  MQTT getMQTTConfiguration();
  void saveConfiguration(MQTT configuration);
  void createMQTTConfigurationFile();

  DOMOTICZ getDomoticzConfiguration();
  void saveConfiguration(DOMOTICZ configuration);
  void createDomoticzConfigurationFile();

  RELAY getRelayConfiguration(uint8_t id);
  void saveConfiguration(uint8_t id, RELAY configuration);
  void createRelayConfigurationFile();

#ifdef AFE_CONFIG_HARDWARE_RELAY
  boolean getRelayState(uint8_t id);
  void saveRelayState(uint8_t id, boolean state);
  void createRelayStateFile();
#endif

  SWITCH getSwitchConfiguration(uint8_t id);
  void saveConfiguration(uint8_t id, SWITCH configuration);
  void createSwitchConfigurationFile();

  /* Methods saves firmware version from/to EEPROM */
  void saveVersion(const char *);

  /* Methods read and save device mode from/to EEPROM */
  uint8_t getDeviceMode();
  void saveDeviceMode(uint8_t mode);

  PRO_VERSION getProVersionConfiguration();
  void saveConfiguration(PRO_VERSION configuration);
  void createProVersionConfigurationFile();

  PASSWORD getPasswordConfiguration();
  void saveConfiguration(PASSWORD configuration);
  void createPasswordConfigurationFile();

  /* ADC Inout create/read/write methods */
#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  ADCINPUT getADCInputConfiguration();
  void saveConfiguration(ADCINPUT configuration);
  void createADCInputConfigurationFile();
#endif

#if AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
  LED getLEDConfiguration(uint8_t id);
  void saveConfiguration(uint8_t id, LED configuration);
  void createLEDConfigurationFile();
  /* Methods read and save ID of system led */
  uint8_t getSystemLedID();
  void saveSystemLedID(uint8_t id);
  void createSystemLedIDConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
  DS18B20 getSensorConfiguration();
  void saveConfiguration(DS18B20 configuration);
#endif

#ifdef AFE_CONFIG_HARDWARE_DHXX
  DH getSensorConfiguration();
  void saveConfiguration(DH configuration);
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
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

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  CONTACTRON getContactronConfiguration(uint8_t id);
  void saveConfiguration(uint8_t id, CONTACTRON configuration);
  void createContractonConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
  GATE getGateConfiguration(uint8_t id);
  void saveConfiguration(uint8_t id, GATE configuration);
  void createGateConfigurationFile();
  uint8_t getGateState(uint8_t id);
  void saveGateState(uint8_t id, uint8_t state);
#endif

#ifdef AFE_CONFIG_HARDWARE_UART
  SERIALPORT getSerialPortConfiguration();
  void saveConfiguration(SERIALPORT configuration);
  void createSerialConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  HPMA115S0 getHPMA115S0SensorConfiguration(uint8_t id);
  void saveConfiguration(uint8_t id, HPMA115S0 configuration);
  void createHPMA115S0SensorConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  BMEX80 getBMEX80SensorConfiguration(uint8_t id);
  void saveConfiguration(uint8_t id, BMEX80 configuration);
  void createBMEX80SensorConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
  BH1750 getBH1750SensorConfiguration(uint8_t id);
  void saveConfiguration(uint8_t id, BH1750 configuration);
  void createBH1750SensorConfigurationFile();
#endif

/* Methods turns on / off APIs */
#ifdef AFE_CONFIG_FUNCTIONALITY_API_CONTROL
  void saveAPI(uint8_t apiID, boolean state);
#endif
};
#endif
