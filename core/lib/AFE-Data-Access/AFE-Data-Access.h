/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_Data_Access_h
#define _AFE_Data_Access_h

#include <AFE-Configuration.h>
#include <AFE-Data-Structures.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <IPAddress.h>
#include <TimeLib.h>

#include <AFE-Data-JSON-templates.h>

#ifdef AFE_ESP32

#include <LITTLEFS.h>
#include <WiFi.h>

#else /* ESP8266 */

#include <ESP8266WiFi.h>
#include <FS.h>

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
#include <LITTLEFS.h>
#endif

#endif // ESP32/ESP8266

#ifdef DEBUG
#include <AFE-Debugger.h>
#endif

class AFEDataAccess {
private:
#ifdef DEBUG
  AFEDebugger *Debugger;
#endif
  IPAddress IPfromString(const char *address);

#ifdef DEBUG
  void printFileOpeningInformation(const __FlashStringHelper *fileName,
                                   uint8_t id = AFE_NONE);
  void printFileCreatingInformation(const __FlashStringHelper *fileName,
                                    uint8_t id = AFE_NONE);
  void printFileOpeningError(const __FlashStringHelper *fileName,
                             uint8_t id = AFE_NONE);
  void printFileWritingInformation();
  void printFileContentInformation();
#endif

public:
  AFEDataAccess();

#ifdef DEBUG
  void printBufforSizeInfo(uint16_t bufferSize, uint16_t jsonSize);
  void printJSONNotPharsed();
#endif

  /* It formats SPIFFS */
  boolean setDefaultConfiguration();
  boolean initializeFileSystem();
  boolean formatFileSystem();

  /* Generic files processing methods */
  boolean fileExist(const char *path);
  boolean createFile(const char *path);

  boolean openFile(File &openedFile, const char *mode,
                   const __FlashStringHelper *path, uint8_t id = AFE_NONE,
                   boolean createIfNotExists = true);

  boolean openFile(File &openedFile, const char *mode, const char *path,
                   boolean createIfNotExists = true);

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  boolean checkIfFolderExists(const char *, boolean createdIfNotExsts = true);
#endif

  void getLogFileName(char *);
  void addLog(const char *);
  void addLog(const __FlashStringHelper *);
  void addLog(const __FlashStringHelper *, const char *);
  void addLog(const __FlashStringHelper *, uint16_t number,
              uint8_t intSize = 8);
  boolean readLogs(String &logs);
  void cleanLogsFile();

#ifdef DEBUG
  void addReference(AFEDebugger *_Debugger);
#endif

  void getConfiguration(DEVICE *);
  void getDeviceID(char *id, boolean extended = false);

  void saveConfiguration(DEVICE *);
  void createDeviceConfigurationFile();

  void getConfiguration(FIRMWARE *);
  void saveConfiguration(FIRMWARE *);
  void createFirmwareConfigurationFile();
  void saveFirmwareVersion(const char *);
  void saveLatestFirmwareVersion(const char *);
  void saveFirmwareAPIVersion();

  void getConfiguration(NETWORK *);
  void saveConfiguration(NETWORK *);
  void createNetworkConfigurationFile();

  void getConfiguration(MQTT *);
  void saveConfiguration(MQTT *);
  void createMQTTConfigurationFile();

  void getWelcomeMessage(String &message);
  void saveWelecomeMessage(const char *);

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  void getConfiguration(DOMOTICZ *);
  void saveConfiguration(DOMOTICZ *);
  void createDomoticzConfigurationFile();
#elif AFE_FIRMWARE_API == AFE_FIRMWARE_API_HOME_ASSISTANT
  boolean getConfiguration(HOME_ASSISTANT_CONFIG *);
  void saveConfiguration(HOME_ASSISTANT_CONFIG *);
  void createHomeAssistantConfigurationFile();
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

  uint8_t getDeviceMode();
  void saveDeviceMode(uint8_t mode);

  uint16_t getRebootCounter(boolean increase = true);
  void saveRebootCounter(unsigned long counter);

  void getConfiguration(PRO_VERSION *);
  void saveConfiguration(PRO_VERSION *);
  void createProVersionConfigurationFile();

  void getConfiguration(PASSWORD *);
  void saveConfiguration(PASSWORD *);
  void createPasswordConfigurationFile();

#ifdef AFE_CONFIG_HARDWARE_RELAY
  boolean getConfiguration(uint8_t id, RELAY *);
  void saveConfiguration(uint8_t id, RELAY *);
  void createRelayConfigurationFile();
  boolean getRelayState(uint8_t id);
  void saveRelayState(uint8_t id, boolean state);
  void createRelayStateFile();
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  boolean getConfiguration(uint8_t id, SWITCH *);
  void saveConfiguration(uint8_t id, SWITCH *);
  void createSwitchConfigurationFile();
#endif // AFE_CONFIG_HARDWARE_SWITCH

#ifdef AFE_CONFIG_HARDWARE_LED
  void getConfiguration(uint8_t id, LED *);
  void saveConfiguration(uint8_t id, LED *);
  void createLEDConfigurationFile();
  uint8_t getSystemLedID();
  void saveSystemLedID(uint8_t id);
  void createSystemLedIDConfigurationFile();
#endif // AFE_CONFIG_HARDWARE_LED

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
#ifdef AFE_ESP32
  void getConfiguration(uint8_t id, ADCINPUT *);
  void saveConfiguration(uint8_t id, ADCINPUT *);
#else  // ESP8266
  void getConfiguration(ADCINPUT *);
  void saveConfiguration(ADCINPUT *);
#endif // ESP32/ESP8266

  void createADCInputConfigurationFile();
#endif // AFE_CONFIG_HARDWARE_ANALOG_INPUT

#ifdef AFE_CONFIG_HARDWARE_DS18B20
  void getConfiguration(uint8_t id, DS18B20 *);
  void saveConfiguration(uint8_t id, DS18B20 *);
  void createDS18B20SensorConfigurationFile(void);
#endif // AFE_CONFIG_HARDWARE_DS18B20

#ifdef AFE_CONFIG_HARDWARE_DHT
  void getConfiguration(uint8_t id, DHT_CONFIG *);
  void saveConfiguration(uint8_t id, DHT_CONFIG *);
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
#ifdef AFE_ESP32
  void getConfiguration(uint8_t id, I2CPORT *);
  void saveConfiguration(uint8_t id, I2CPORT *);
#else  // ESP8266
  void getConfiguration(I2CPORT *);
  void saveConfiguration(I2CPORT *);
#endif // ESP32/ESP8266
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
  boolean getConfiguration(uint8_t id, BH1750_CONFIG *);
  void saveConfiguration(uint8_t id, BH1750_CONFIG *);
  void createBH1750SensorConfigurationFile();
#endif // AFE_CONFIG_HARDWARE_BH1750

#ifdef AFE_CONFIG_HARDWARE_AS3935
  void getConfiguration(uint8_t id, AS3935 *);
  void saveConfiguration(uint8_t id, AS3935 *);
  void createAS3935SensorConfigurationFile();
#endif // AFE_CONFIG_HARDWARE_AS3935

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
  boolean getConfiguration(ANEMOMETER *);
  void saveConfiguration(ANEMOMETER *);
  void createAnemometerSensorConfigurationFile();
#endif // AFE_CONFIG_HARDWARE_ANEMOMETER

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
  boolean getConfiguration(RAINMETER *);
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

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
  void getConfiguration(uint8_t id, PN532_SENSOR *);
  void saveConfiguration(uint8_t id, PN532_SENSOR *);
  void createPN532ConfigurationFile();
  void getConfiguration(uint8_t id, MIFARE_CARD *);
  void saveConfiguration(uint8_t id, MIFARE_CARD *);
  void createMiFareCardConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_CLED
  boolean getConfiguration(uint8_t id, CLED *);
  void saveConfiguration(uint8_t id, CLED *);
  void createCLEDConfigurationFile();

  boolean getConfiguration(uint8_t id, CLED_EFFECT_BLINKING *);
  void saveConfiguration(uint8_t id, CLED_EFFECT_BLINKING *);
  void createCLEDEffectBlinkingConfigurationFile();

  boolean getConfiguration(uint8_t id, CLED_EFFECT_WAVE *);
  void saveConfiguration(uint8_t id, CLED_EFFECT_WAVE *);
  void createCLEDEffectWaveConfigurationFile();

  boolean getConfiguration(uint8_t id, CLED_EFFECT_FADE_INOUT *);
  void saveConfiguration(uint8_t id, CLED_EFFECT_FADE_INOUT *);
  void createCLEDEffectFadeInOutConfigurationFile();

/* @TODO T5
#ifdef AFE_CONFIG_HARDWARE_CLED_LIGHT_CONTROLLED_EFFECT
  boolean getConfiguration(uint8_t id, CLED_BACKLIGHT *);
  void saveConfiguration(uint8_t id, CLED_BACKLIGHT *);
  void createCLEDBacklightConfigurationFile();
#endif // AFE_CONFIG_HARDWARE_CLED_LIGHT_CONTROLLED_EFFECT
*/

#endif // AFE_CONFIG_HARDWARE_CLED

#ifdef AFE_CONFIG_HARDWARE_TSL2561
  boolean getConfiguration(uint8_t id, TSL2561 *);
  void saveConfiguration(uint8_t id, TSL2561 *);
  void createTSL2561SensorConfigurationFile();
#endif // AFE_CONFIG_HARDWARE_TSL2561

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
  boolean getConfiguration(uint8_t id, MCP23XXX *);
  void saveConfiguration(uint8_t id, MCP23XXX *);
  void createMCP23XXXConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_FS3000
  boolean getConfiguration(uint8_t id, FS3000_CONFIG *);
  void saveConfiguration(uint8_t id, FS3000_CONFIG *);
  void createFS3000SensorConfigurationFile();
#endif
};
#endif // _AFE_Data_Access_h
