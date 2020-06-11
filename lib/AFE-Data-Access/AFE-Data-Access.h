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

  void getConfiguration(DEVICE *);
#ifdef T0_CONFIG // Version 2.0.0 - 2 Device configuration. Method used only to
                 // upgrade to new version
  DEVICE_T0_200 getDeviceT0v200Configuration();
#endif
  void saveConfiguration(DEVICE *);
  void createDeviceConfigurationFile();

  void getConfiguration(FIRMWARE *);
  void saveConfiguration(FIRMWARE *);
  void createFirmwareConfigurationFile();
  void saveFirmwareVersion(const char *);
  void saveFirmwareAPIVersion();

  void getConfiguration(NETWORK *);
  void saveConfiguration(NETWORK *);
  void createNetworkConfigurationFile();

  void getConfiguration(MQTT *);
  void saveConfiguration(MQTT *);
  void createMQTTConfigurationFile();

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  void getConfiguration(DOMOTICZ *);
  void saveConfiguration(DOMOTICZ *);
  void createDomoticzConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
  void getConfiguration(uint8_t id, RELAY *);
  void saveConfiguration(uint8_t id, RELAY *);
  void createRelayConfigurationFile();

  boolean getRelayState(uint8_t id);
  void saveRelayState(uint8_t id, boolean state);
  void createRelayStateFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  void getConfiguration(uint8_t id, SWITCH *);
  void saveConfiguration(uint8_t id, SWITCH *);
  void createSwitchConfigurationFile();
#endif

  uint8_t getDeviceMode();
  void saveDeviceMode(uint8_t mode);

  void getConfiguration(PRO_VERSION *);
  void saveConfiguration(PRO_VERSION *);
  void createProVersionConfigurationFile();

  void getConfiguration(PASSWORD *);
  void saveConfiguration(PASSWORD *);
  void createPasswordConfigurationFile();

/* ADC Inout create/read/write methods */
#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  void getConfiguration(ADCINPUT *);
  void saveConfiguration(ADCINPUT *);
  void createADCInputConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
  void getConfiguration(uint8_t id, LED *);
  void saveConfiguration(uint8_t id, LED *);
  void createLEDConfigurationFile();
  /* Methods read and save ID of system led */
  uint8_t getSystemLedID();
  void saveSystemLedID(uint8_t id);
  void createSystemLedIDConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
  void getConfiguration(uint8_t id, DS18B20 *);
  void saveConfiguration(uint8_t id, DS18B20 *);
  void createDS18B20SensorConfigurationFile(void);
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
  void getConfiguration(uint8_t id, CONTACTRON *);
  void saveConfiguration(uint8_t id, CONTACTRON *);
  void createContractonConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
  void getConfiguration(uint8_t id, GATE *);
  void saveConfiguration(uint8_t id, GATE *);
  void createGateConfigurationFile();
  uint8_t getGateState(uint8_t id);
  void saveGateState(uint8_t id, uint8_t state);
#endif

#ifdef AFE_CONFIG_HARDWARE_I2C
  void getConfiguration(I2CPORT *);
  void saveConfiguration(I2CPORT *);
  void createI2CConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_UART
  void getConfiguration(SERIALPORT *);
  void saveConfiguration(SERIALPORT *);
  void createSerialConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  void getConfiguration(uint8_t id, HPMA115S0 *);
  void saveConfiguration(uint8_t id, HPMA115S0 *);
  void createHPMA115S0SensorConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  void getConfiguration(uint8_t id, BMEX80 *);
  void saveConfiguration(uint8_t id, BMEX80 *);
  void createBMEX80SensorConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
  void getConfiguration(uint8_t id, BH1750 *);
  void saveConfiguration(uint8_t id, BH1750 *);
  void createBH1750SensorConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
  void getConfiguration(uint8_t id, AS3935 *);
  void saveConfiguration(uint8_t id, AS3935 *);
  void createAS3935SensorConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER_SENSOR
  void getConfiguration(ANEMOMETER *);
  void saveConfiguration(ANEMOMETER *);
  void createAnemometerSensorConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER_SENSOR
  void getConfiguration(RAINMETER *);
  void saveConfiguration(RAINMETER *);
  void createRainmeterSensorConfigurationFile();

  void get(RAINMETER_DATA *);
  void save(RAINMETER_DATA *);
  void createRainmeterSensorDataConfigurationFile();

#endif

/* Methods turns on / off APIs */
#ifdef AFE_CONFIG_FUNCTIONALITY_API_CONTROL
  void saveAPI(uint8_t apiID, boolean state);
#endif
};
#endif
