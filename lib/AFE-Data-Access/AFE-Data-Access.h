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

// Disabled with T0 as it was added to the parsher, is it required for T6?
//#ifdef AFE_CONFIG_HARDWARE_BMEX80
//#include <Adafruit_BMP085.h>
//#endif

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEDataAccess {
private:
  IPAddress IPfromString(const char *address);

public:
  AFEDataAccess();
  
  /* It formats SPIFFS */
  boolean formatFileSystem();

  /* Returns true if a file exists */
  boolean fileExist(const char *);

  const String getDeviceUID();
  void saveDeviceUID(const char *);
  void createDeviceUIDFile();

  DEVICE getDeviceConfiguration();
#ifdef T0_CONFIG // Version 2.0.0 - 2 Device configuration. Method used only to
                 // upgrade to new version
  DEVICE_T0_200 getDeviceT0v200Configuration();
#endif
  void saveConfiguration(DEVICE *);
  void createDeviceConfigurationFile();

  FIRMWARE getFirmwareConfiguration();
  void saveConfiguration(FIRMWARE *);
  void createFirmwareConfigurationFile();
  void saveFirmwareVersion(const char *);
  void saveFirmwareAPIVersion();

  NETWORK getNetworkConfiguration();
  void saveConfiguration(NETWORK configuration);
  void createNetworkConfigurationFile();

  MQTT getMQTTConfiguration();
  void saveConfiguration(MQTT configuration);
  void createMQTTConfigurationFile();

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  DOMOTICZ getDomoticzConfiguration();
  void saveConfiguration(DOMOTICZ configuration);
  void createDomoticzConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
  RELAY getRelayConfiguration(uint8_t id);
  void saveConfiguration(uint8_t id, RELAY configuration);
  void createRelayConfigurationFile();


  boolean getRelayState(uint8_t id);
  void saveRelayState(uint8_t id, boolean state);
  void createRelayStateFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  SWITCH getSwitchConfiguration(uint8_t id);
  void saveConfiguration(uint8_t id, SWITCH configuration);
  void createSwitchConfigurationFile();
#endif

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

#ifdef AFE_CONFIG_HARDWARE_LED
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

#ifdef AFE_CONFIG_HARDWARE_I2C
  I2CPORT getI2CPortConfiguration();
  void saveConfiguration(I2CPORT configuration);
  void createI2CConfigurationFile();
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

#ifdef AFE_CONFIG_HARDWARE_AS3935
  AS3935 getAS3935SensorConfiguration(uint8_t id);
  void saveConfiguration(uint8_t id, AS3935 configuration);
  void createAS3935SensorConfigurationFile();
#endif

/* Methods turns on / off APIs */
#ifdef AFE_CONFIG_FUNCTIONALITY_API_CONTROL
  void saveAPI(uint8_t apiID, boolean state);
#endif
};
#endif
