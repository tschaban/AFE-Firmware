/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Data_Access_h
#define _AFE_Data_Access_h

#include <AFE-Configuration.h>
#include <AFE-Data-Structures.h>
#include <ESP8266WiFi.h>
#include <IPAddress.h>

#include <ArduinoJson.h>



#include <FS.h>
#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
#include <LittleFS.h>
#endif

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

  void getWelcomeMessage(String &message);
  void saveWelecomeMessage(const __FlashStringHelper *message);

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  void getConfiguration(DOMOTICZ *);
  void saveConfiguration(DOMOTICZ *);
  void createDomoticzConfigurationFile();
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

  uint8_t getDeviceMode();
  void saveDeviceMode(uint8_t mode);

  void getConfiguration(PRO_VERSION *);
  void saveConfiguration(PRO_VERSION *);
  void createProVersionConfigurationFile();

  void getConfiguration(PASSWORD *);
  void saveConfiguration(PASSWORD *);
  void createPasswordConfigurationFile();

#ifdef AFE_CONFIG_HARDWARE_RELAY
  void getConfiguration(uint8_t id, RELAY *);
  void saveConfiguration(uint8_t id, RELAY *);
  void createRelayConfigurationFile();
//  void createRelayConfigurationFile(uint8_t id);
  boolean getRelayState(uint8_t id);
  void saveRelayState(uint8_t id, boolean state);
  void createRelayStateFile();
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  void getConfiguration(uint8_t id, SWITCH *);
  void saveConfiguration(uint8_t id, SWITCH *);
  void createSwitchConfigurationFile();
//  void createSwitchConfigurationFile(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_SWITCH

#ifdef AFE_CONFIG_HARDWARE_LED
  void getConfiguration(uint8_t id, LED *);
  void saveConfiguration(uint8_t id, LED *);
  void createLEDConfigurationFile();
//  void createLEDConfigurationFile(uint8_t id);
  uint8_t getSystemLedID();
  void saveSystemLedID(uint8_t id);
  void createSystemLedIDConfigurationFile();
#endif // AFE_CONFIG_HARDWARE_LED

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  void getConfiguration(ADCINPUT *);
  void saveConfiguration(ADCINPUT *);
  void createADCInputConfigurationFile();
#endif // AFE_CONFIG_HARDWARE_ADC_VCC


#ifdef AFE_CONFIG_HARDWARE_DS18B20
  void getConfiguration(uint8_t id, DS18B20 *);
  void saveConfiguration(uint8_t id, DS18B20 *);
  void createDS18B20SensorConfigurationFile(void);
#endif // AFE_CONFIG_HARDWARE_DS18B20

#ifdef AFE_CONFIG_HARDWARE_DHT
  void getConfiguration(uint8_t id, DHT *);
  void saveConfiguration(uint8_t id, DHT *);
  void createDHTSensorConfigurationFile(void);
#endif // AFE_CONFIG_HARDWARE_DHT

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  void getConfiguration(uint8_t id, REGULATOR *);
  void saveConfiguration(uint8_t id, REGULATOR *);
  void createRegulatorConfigurationFile(void);
#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
  void getConfiguration(uint8_t id, THERMAL_PROTECTOR *);
  void saveConfiguration(uint8_t id, THERMAL_PROTECTOR *);
  void createThermalProtectorConfigurationFile(void);
#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR


#if defined(T3_CONFIG)
  PIR getPIRConfiguration(uint8_t id);
  void saveConfiguration(uint8_t id, PIR configuration);
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  void getConfiguration(uint8_t id, CONTACTRON *);
  void saveConfiguration(uint8_t id, CONTACTRON *);
  void createContractonConfigurationFile();
#endif // AFE_CONFIG_HARDWARE_CONTACTRON

#ifdef AFE_CONFIG_HARDWARE_GATE
  void getConfiguration(uint8_t id, GATE *);
  void saveConfiguration(uint8_t id, GATE *);
  void createGateConfigurationFile();
  uint8_t getGateState(uint8_t id);
  void saveGateState(uint8_t id, uint8_t state);
#endif // AFE_CONFIG_HARDWARE_GATE

#ifdef AFE_CONFIG_HARDWARE_I2C
  void getConfiguration(I2CPORT *);
  void saveConfiguration(I2CPORT *);
  void createI2CConfigurationFile();
#endif // AFE_CONFIG_HARDWARE_I2C

#ifdef AFE_CONFIG_HARDWARE_UART
  void getConfiguration(SERIALPORT *);
  void saveConfiguration(SERIALPORT *);
  void createSerialConfigurationFile();
#endif // AFE_CONFIG_HARDWARE_UART

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  void getConfiguration(uint8_t id, HPMA115S0 *);
  void saveConfiguration(uint8_t id, HPMA115S0 *);
  void createHPMA115S0SensorConfigurationFile();
#endif // AFE_CONFIG_HARDWARE_HPMA115S0

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  void getConfiguration(uint8_t id, BMEX80 *);
  void saveConfiguration(uint8_t id, BMEX80 *);
  void createBMEX80SensorConfigurationFile();
#endif // AFE_CONFIG_HARDWARE_BMEX80

#ifdef AFE_CONFIG_HARDWARE_BH1750
  void getConfiguration(uint8_t id, BH1750 *);
  void saveConfiguration(uint8_t id, BH1750 *);
  void createBH1750SensorConfigurationFile();
#endif // AFE_CONFIG_HARDWARE_BH1750

#ifdef AFE_CONFIG_HARDWARE_AS3935
  void getConfiguration(uint8_t id, AS3935 *);
  void saveConfiguration(uint8_t id, AS3935 *);
  void createAS3935SensorConfigurationFile();
#endif // AFE_CONFIG_HARDWARE_AS3935

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
  void getConfiguration(ANEMOMETER *);
  void saveConfiguration(ANEMOMETER *);
  void createAnemometerSensorConfigurationFile();
#endif // AFE_CONFIG_HARDWARE_ANEMOMETER

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
  void getConfiguration(RAINMETER *);
  void saveConfiguration(RAINMETER *);
  void createRainmeterSensorConfigurationFile();
  void get(RAINMETER_DATA *);
  void save(RAINMETER_DATA *);
  void createRainmeterSensorDataConfigurationFile();
#endif // AFE_CONFIG_HARDWARE_RAINMETER

/* Methods turns on / off APIs */
#ifdef AFE_CONFIG_FUNCTIONALITY_API_CONTROL
  void saveAPI(uint8_t apiID, boolean state);
#endif // AFE_CONFIG_FUNCTIONALITY_API_CONTROL

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
  void getConfiguration(uint8_t id, BINARY_SENSOR *);
  void saveConfiguration(uint8_t id, BINARY_SENSOR *);
  void createBinarySensorConfigurationFile();
#endif


};
#endif
