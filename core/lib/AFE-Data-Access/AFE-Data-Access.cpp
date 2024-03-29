#include "AFE-Data-Access.h"

AFEDataAccess::AFEDataAccess() {}

/**
 * @brief Formats the file system to LittleFS or SPIFFS, depending on
 * AFE_FILE_SYSTEM
 *
 * @return boolean
 */
boolean AFEDataAccess::formatFileSystem() {
#ifdef DEBUG
  Serial << endl << endl << F("INFO: Formatig File System");
#endif
#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  return LittleFS.format();
#else
  return SPIFFS.format();
#endif
}

/**
 * @brief Checks if a file exists
 *
 * @param  path             full path to the file: folder and file name
 * @return boolean
 */
boolean AFEDataAccess::fileExist(const char *path) {
#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  return LittleFS.exists(path);
#else
  return SPIFFS.exists(path);
#endif
}

/**
 * @brief Reads a string from AFE_FILE_WELCOME_MESSAGE file. After it's read the
 * string is removed from the file
 *
 * @param  message          read string
 */
void AFEDataAccess::getWelcomeMessage(String &message) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << F(AFE_FILE_WELCOME_MESSAGE)
         << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(AFE_FILE_WELCOME_MESSAGE, "r");
#else
  File configFile = SPIFFS.open(AFE_FILE_WELCOME_MESSAGE, "r");
#endif

  if (configFile) {
    message = configFile.readString();
    configFile.close();

    saveWelecomeMessage("");
  } else {
    message = "";
#ifdef DEBUG
    Serial << endl
           << F("ERROR: File: ") << F(AFE_FILE_WELCOME_MESSAGE)
           << F(" not opened");
#endif
  }
}

/**
 * @brief Saves a string in AFE_FILE_WELCOME_MESSAGE file
 *
 * @param  message          string to be saved
 */
void AFEDataAccess::saveWelecomeMessage(const char *message) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << F(AFE_FILE_WELCOME_MESSAGE)
         << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(AFE_FILE_WELCOME_MESSAGE, "w");
#else
  File configFile = SPIFFS.open(AFE_FILE_WELCOME_MESSAGE, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing to file: ") << message;
#endif

    configFile.print(message);
    configFile.close();

  }
#ifdef DEBUG
  else {
    Serial << endl
           << F("ERROR: File ") << F(AFE_FILE_WELCOME_MESSAGE)
           << F(" not opened for writing");
  }
#endif
}

void AFEDataAccess::getConfiguration(PRO_VERSION *configuration) {

#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << F(AFE_FILE_PRO_VERSION_CONFIGURATION)
         << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(AFE_FILE_PRO_VERSION_CONFIGURATION, "r");
#else
  File configFile = SPIFFS.open(AFE_FILE_PRO_VERSION_CONFIGURATION, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_PRO_VERSION> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif
      configuration->valid = root["valid"];
      sprintf(configuration->serial, root["serial"]);
#ifdef DEBUG
      printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_PRO_VERSION,
                          jsonBuffer.size());
#endif
    }
#ifdef DEBUG
    else {
      Serial << F("ERROR: JSON not pharsed");
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else {
    Serial << endl
           << F("ERROR: Configuration file: ")
           << F(AFE_FILE_PRO_VERSION_CONFIGURATION) << F(" not opened");
  }
#endif
}
void AFEDataAccess::saveConfiguration(PRO_VERSION *configuration) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << F(AFE_FILE_PRO_VERSION_CONFIGURATION)
         << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(AFE_FILE_PRO_VERSION_CONFIGURATION, "w");
#else
  File configFile = SPIFFS.open(AFE_FILE_PRO_VERSION_CONFIGURATION, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_PRO_VERSION> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["valid"] = configuration->valid;
    root["serial"] = configuration->serial;

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_PRO_VERSION, jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}
void AFEDataAccess::createProVersionConfigurationFile() {
#ifdef DEBUG
  Serial << endl
         << F("INFO: Creating file: ") << AFE_FILE_PRO_VERSION_CONFIGURATION;
#endif
  PRO_VERSION ProConfiguration;
  ProConfiguration.serial[0] = AFE_EMPTY_STRING;
  ProConfiguration.valid = false;
  saveConfiguration(&ProConfiguration);
}

void AFEDataAccess::getConfiguration(PASSWORD *configuration) {

#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << F(AFE_FILE_PASSWORD_CONFIGURATION)
         << F(" ... ");
#endif
#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(AFE_FILE_PASSWORD_CONFIGURATION, "r");
#else
  File configFile = SPIFFS.open(AFE_FILE_PASSWORD_CONFIGURATION, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_PASSWORD> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif
      configuration->protect = root["protect"];
      sprintf(configuration->password, root["password"]);

#ifdef DEBUG
      printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_PASSWORD, jsonBuffer.size());
#endif
    }
#ifdef DEBUG
    else {
      Serial << F("ERROR: JSON not pharsed");
    }
#endif

    configFile.close();
  }
#ifdef DEBUG
  else {
    Serial << endl
           << F("ERROR: Configuration file: ")
           << F(AFE_FILE_PASSWORD_CONFIGURATION) << F(" not opened");
  }
#endif
}
void AFEDataAccess::saveConfiguration(PASSWORD *configuration) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << F(AFE_FILE_PASSWORD_CONFIGURATION)
         << F(" ... ");
#endif
#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(AFE_FILE_PASSWORD_CONFIGURATION, "w");
#else
  File configFile = SPIFFS.open(AFE_FILE_PASSWORD_CONFIGURATION, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_PASSWORD> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["protect"] = configuration->protect;
    root["password"] = configuration->password;
    root.printTo(configFile);

#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_PASSWORD, jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}
void AFEDataAccess::createPasswordConfigurationFile() {
#ifdef DEBUG
  Serial << endl
         << F("INFO: Creating file: ") << F(AFE_FILE_PASSWORD_CONFIGURATION);
#endif
  PASSWORD PasswordConfiguration;
  PasswordConfiguration.protect = false;
  PasswordConfiguration.password[0] = AFE_EMPTY_STRING;
  saveConfiguration(&PasswordConfiguration);
}

void AFEDataAccess::getDeviceID(char *id) {
  byte m[6];
  WiFi.macAddress(m);
  sprintf(id, "%X%x%X%x-%X%x%X%x", m[0], m[5], m[1], m[4], m[2], m[3], m[3],
          m[2]);
}

void AFEDataAccess::getConfiguration(DEVICE *configuration) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << F(AFE_FILE_DEVICE_CONFIGURATION)
         << F(" ... ");
#endif

  JsonVariant exists;

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(AFE_FILE_DEVICE_CONFIGURATION, "r");
#else
  File configFile = SPIFFS.open(AFE_FILE_DEVICE_CONFIGURATION, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_DEVICE> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      sprintf(configuration->name, root["name"]);
      configuration->timeToAutoLogOff =
          root["timeToAutoLogOff"] | AFE_AUTOLOGOFF_DEFAULT_TIME;
      configuration->api.http = root["api"]["http"];
      configuration->api.mqtt = root["api"]["mqtt"];
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ

      exists = root["api"]["domoticz"];
      configuration->api.domoticz =
          exists.success() ? root["api"]["domoticz"] : false;

      /* HTTP API must be ON when Domoticz is ON */
      if (configuration->api.domoticz && !configuration->api.http) {
        configuration->api.http = true;
      }
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
      configuration->noOfLEDs =
          root["noOfLEDs"] | AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_LEDS;
#endif
#ifdef AFE_CONFIG_HARDWARE_SWITCH
      configuration->noOfSwitches =
          root["noOfSwitches"] | AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_SWITCHES;
#endif
#ifdef AFE_CONFIG_HARDWARE_RELAY
      configuration->noOfRelays =
          root["noOfRelays"] | AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_RELAYS;
#endif
#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT

#endif

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
#ifndef AFE_ESP32
      configuration->isAnalogInput = root["isAnalogInput"] | false;
#else
      configuration->noOfAnalogInputs =
          root["noOfAnalogInputs"] | AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_ADCS;
#endif // AFE_ESP32
#endif // AFE_CONFIG_HARDWARE_ANALOG_INPUT

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
      configuration->noOfContactrons =
          root["noOfContactrons"] |
          AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_CONTACTRONS;
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
      configuration->noOfGates =
          root["noOfGates"] | AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_GATES;
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
      configuration->noOfBMEX80s =
          root["noOfBMEX80s"] | AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_BMEX80;
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
      configuration->noOfHPMA115S0s =
          root["noOfHPMA115S0s"] |
          AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_HPMA115S0;
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
      configuration->noOfBH1750s =
          root["noOfBH1750s"] | AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_BH1750;
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
      configuration->noOfAS3935s =
          root["noOfAS3935s"] | AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_AS3935;
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
      configuration->noOfDS18B20s =
          root["noOfDS18B20s"] | AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_DS18B20;
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
      configuration->noOfAnemometerSensors =
          root["noOfAnemometerSensors"] |
          AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_ANEMOMETER_SENSORS;
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
      configuration->noOfRainmeterSensors =
          root["noOfRainmeterSensors"] |
          AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_RAINMETER_SENSORS;
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
      configuration->noOfRegulators =
          root["noOfRegulators"] |
          AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_REGULATORS;
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
      configuration->noOfThermalProtectors =
          root["noOfThermalProtectors"] |
          AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_THERMAL_PROTECTIORS;
#endif

#ifdef AFE_CONFIG_HARDWARE_DHT
      configuration->noOfDHTs =
          root["noOfDHTs"] | AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_DHT;
#endif

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
      configuration->noOfBinarySensors =
          root["noOfBinarySensors"] |
          AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_BINARY_SENSORS;
#endif

#ifdef AFE_CONFIG_HARDWARE_TSL2561
      configuration->noOfTSL2561s =
          root["noOfTSL2561s"] | AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_TSL2561;
#endif

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
      configuration->noOfPN532Sensors =
          root["noOfPN532Sensors"] |
          AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_PN532_SENSORS;
      configuration->noOfMiFareCards =
          root["noOfMiFareCards"] |
          AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_MIFARE_CARDS;
#endif

#ifdef AFE_CONFIG_HARDWARE_CLED
      configuration->noOfCLEDs =
          root["noOfCLEDs"] | AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_CLED_STRIPS;
#endif

#if defined(AFE_CONFIG_HARDWARE_I2C) && defined(AFE_ESP32)
      configuration->noOfI2Cs =
          root["noOfI2Cs"] | AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_I2C;
#endif

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
      configuration->noOfMCP23xxx =
          root["noOfMCP23xxx"] | AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_MCP23XXX;
#endif

#ifdef AFE_CONFIG_HARDWARE_FS3000
      exists = root["noOfFS3000s"];
      configuration->noOfFS3000s =
          exists.success() ? root["noOfFS3000s"]
                           : AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_FS3000;
#endif

#ifdef DEBUG
      printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_DEVICE, jsonBuffer.size());
#endif
    }
#ifdef DEBUG
    else {
      Serial << F("ERROR: JSON not pharsed");
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else {
    Serial << endl
           << F("ERROR: Configuration file: ")
           << F(AFE_FILE_DEVICE_CONFIGURATION) << F(" not opened");
  }
#endif
}
void AFEDataAccess::saveConfiguration(DEVICE *configuration) {

#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << F(AFE_FILE_DEVICE_CONFIGURATION)
         << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(AFE_FILE_DEVICE_CONFIGURATION, "w");
#else
  File configFile = SPIFFS.open(AFE_FILE_DEVICE_CONFIGURATION, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_DEVICE> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["name"] = configuration->name;
    root["timeToAutoLogOff"] = configuration->timeToAutoLogOff;
    JsonObject &jsonAPI = root.createNestedObject("api");
    jsonAPI["http"] = configuration->api.http;
    jsonAPI["mqtt"] = configuration->api.mqtt;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    jsonAPI["domoticz"] = configuration->api.domoticz;
#endif
#ifdef AFE_CONFIG_HARDWARE_LED
    root["noOfLEDs"] = configuration->noOfLEDs;
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
    root["noOfSwitches"] = configuration->noOfSwitches;
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
    root["noOfRelays"] = configuration->noOfRelays;
#endif

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
#ifndef AFE_ESP32
    root["isAnalogInput"] = configuration->isAnalogInput;
#else
    root["noOfAnalogInputs"] = configuration->noOfAnalogInputs;
#endif // AFE_ESP32
#endif // AFE_CONFIG_HARDWARE_ANALOG_INPUT

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
    root["noOfContactrons"] = configuration->noOfContactrons;
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
    root["noOfGates"] = configuration->noOfGates;
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
    root["noOfHPMA115S0s"] = configuration->noOfHPMA115S0s;
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
    root["noOfBMEX80s"] = configuration->noOfBMEX80s;
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
    root["noOfBH1750s"] = configuration->noOfBH1750s;
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
    root["noOfAS3935s"] = configuration->noOfAS3935s;
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
    root["noOfDS18B20s"] = configuration->noOfDS18B20s;
#endif

#ifdef AFE_CONFIG_HARDWARE_DHT
    root["noOfDHTs"] = configuration->noOfDHTs;
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
    root["noOfAnemometerSensors"] = configuration->noOfAnemometerSensors;
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
    root["noOfRainmeterSensors"] = configuration->noOfRainmeterSensors;
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
    root["noOfRegulators"] = configuration->noOfRegulators;
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
    root["noOfThermalProtectors"] = configuration->noOfThermalProtectors;
#endif

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
    root["noOfBinarySensors"] = configuration->noOfBinarySensors;
#endif

#ifdef AFE_CONFIG_HARDWARE_TSL2561
    root["noOfTSL2561s"] = configuration->noOfTSL2561s;
#endif

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
    root["noOfPN532Sensors"] = configuration->noOfPN532Sensors;
    root["noOfMiFareCards"] = configuration->noOfMiFareCards;
#endif

#if defined(AFE_CONFIG_HARDWARE_I2C) && defined(AFE_ESP32)
    root["noOfI2Cs"] = configuration->noOfI2Cs;
#endif

#ifdef AFE_CONFIG_HARDWARE_CLED
    root["noOfCLEDs"] = configuration->noOfCLEDs;
#endif

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
    root["noOfMCP23xxx"] = configuration->noOfMCP23xxx;
#endif

#ifdef AFE_CONFIG_HARDWARE_FS3000
    root["noOfFS3000s"] = configuration->noOfFS3000s;
#endif

    root.printTo(configFile);

#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_DEVICE, jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif

/**
 * @brief Removing connection between relay and a gate, if exists reseting
 * changing the number of gates
 *
 */
#ifdef AFE_CONFIG_HARDWARE_GATE
  if (configuration->noOfGates < AFE_CONFIG_HARDWARE_NUMBER_OF_GATES) {
    GATE _Gate;
    for (uint8_t i = configuration->noOfGates;
         i < AFE_CONFIG_HARDWARE_NUMBER_OF_GATES; i++) {
#ifdef DEBUG
      Serial << endl << F("INFO: Update of Gate configuration");

#endif
      getConfiguration(i, &_Gate);
      if (_Gate.relayId != AFE_HARDWARE_ITEM_NOT_EXIST) {
        _Gate.relayId = AFE_HARDWARE_ITEM_NOT_EXIST;
        saveConfiguration(i, &_Gate);
      }
#ifdef DEBUG
      else {
        Serial << F(": NOT required");
      }
#endif
    }
  }

#endif
}
void AFEDataAccess::createDeviceConfigurationFile() {
#ifdef DEBUG
  Serial << endl
         << F("INFO: Creating file: ") << F(AFE_FILE_DEVICE_CONFIGURATION);
#endif
  DEVICE configuration;
  sprintf(configuration.name, "AFE-Device");
  configuration.timeToAutoLogOff = AFE_AUTOLOGOFF_DEFAULT_TIME;
  /* APIs */
  configuration.api.mqtt = false;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  configuration.api.domoticz = false;
#endif
  configuration.api.http = true;

/* Relay presence */
#ifdef AFE_CONFIG_HARDWARE_RELAY
  configuration.noOfRelays = AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_RELAYS;
#endif

  /* Switch presence */
  configuration.noOfSwitches = AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_SWITCHES;

/* LEDs presence */
#ifdef AFE_CONFIG_HARDWARE_LED
  configuration.noOfLEDs = AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_LEDS;
#endif

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
#ifndef AFE_ESP32
  configuration.isAnalogInput = false;
#else
  configuration.noOfAnalogInputs = AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_ADCS;
#endif // AFE_ESP32
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  configuration.noOfContactrons =
      AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_CONTACTRONS;
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
  configuration.noOfGates = AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_GATES;
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  configuration.noOfHPMA115S0s =
      AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_HPMA115S0;
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  configuration.noOfBMEX80s = AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_BMEX80;
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
  configuration.noOfBH1750s = AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_BH1750;
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
  configuration.noOfAS3935s = AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_AS3935;
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
  configuration.noOfDS18B20s = AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_DS18B20;
#endif

#ifdef AFE_CONFIG_HARDWARE_DHT
  configuration.noOfDHTs = AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_DHT;
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
  configuration.noOfAnemometerSensors =
      AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_ANEMOMETER_SENSORS;
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
  configuration.noOfRainmeterSensors =
      AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_RAINMETER_SENSORS;
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  configuration.noOfRegulators =
      AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_REGULATORS;
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
  configuration.noOfThermalProtectors =
      AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_THERMAL_PROTECTIORS;
#endif

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
  configuration.noOfBinarySensors =
      AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_BINARY_SENSORS;
#endif

#ifdef AFE_CONFIG_HARDWARE_TSL2561
  configuration.noOfTSL2561s = AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_TSL2561;
#endif

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
  configuration.noOfPN532Sensors =
      AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_PN532_SENSORS;
  configuration.noOfMiFareCards =
      AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_MIFARE_CARDS;

#endif // AFE_CONFIG_HARDWARE_PN532_SENSOR

#ifdef AFE_CONFIG_HARDWARE_CLED
  configuration.noOfCLEDs = AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_CLED_STRIPS;
#endif

#if defined(AFE_CONFIG_HARDWARE_I2C) && defined(AFE_ESP32)
  configuration.noOfI2Cs = AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_I2C;
#endif

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
  configuration.noOfMCP23xxx = AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_MCP23XXX;
#endif

#ifdef AFE_CONFIG_HARDWARE_FS3000
  configuration.noOfFS3000s = AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_FS3000;
#endif

  saveConfiguration(&configuration);
}

void AFEDataAccess::getConfiguration(FIRMWARE *configuration) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << F(AFE_FILE_FIRMWARE_CONFIGURATION)
         << F(" ... ");
#endif
#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(AFE_FILE_FIRMWARE_CONFIGURATION, "r");
#else
  File configFile = SPIFFS.open(AFE_FILE_FIRMWARE_CONFIGURATION, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_FIRMWARE> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif
      configuration->type = root["type"].as<int>();
      configuration->api = root["api"] | AFE_HARDWARE_ITEM_NOT_EXIST;
      sprintf(configuration->installed_version, root["version"]);

      JsonVariant exists = root["latest_version"];
      sprintf(configuration->latest_version,
              (exists.success() ? root["latest_version"] : root["version"]));

    }
#ifdef DEBUG
    else {
      Serial << F("ERROR: JSON not pharsed");
    }
#endif
    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_FIRMWARE, jsonBuffer.size());
#endif

  }
#ifdef DEBUG
  else {
    Serial << endl
           << F("ERROR: Configuration file: ")
           << F(AFE_FILE_FIRMWARE_CONFIGURATION) << F(" not opened");
  }
#endif
}
void AFEDataAccess::saveConfiguration(FIRMWARE *configuration) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << F(AFE_FILE_FIRMWARE_CONFIGURATION)
         << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(AFE_FILE_FIRMWARE_CONFIGURATION, "w");
#else
  File configFile = SPIFFS.open(AFE_FILE_FIRMWARE_CONFIGURATION, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_FIRMWARE> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["type"] = configuration->type;
    root["api"] = configuration->api;
    root["version"] = configuration->installed_version;
    root["latest_version"] = configuration->latest_version;
    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_FIRMWARE, jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}
void AFEDataAccess::createFirmwareConfigurationFile() {
#ifdef DEBUG
  Serial << endl
         << F("INFO: Creating file: ") << F(AFE_FILE_FIRMWARE_CONFIGURATION);
#endif
  FIRMWARE firmwareConfiguration;
  sprintf(firmwareConfiguration.installed_version, AFE_FIRMWARE_VERSION);
  sprintf(firmwareConfiguration.latest_version, AFE_FIRMWARE_VERSION);
  firmwareConfiguration.type = AFE_FIRMWARE_TYPE;
  firmwareConfiguration.api = AFE_FIRMWARE_API;
  saveConfiguration(&firmwareConfiguration);
}

void AFEDataAccess::saveFirmwareVersion(const char *version) {
  FIRMWARE configuration;
  getConfiguration(&configuration);
  sprintf(configuration.installed_version, version);
  saveConfiguration(&configuration);
}

void AFEDataAccess::saveLatestFirmwareVersion(const char *version) {
  FIRMWARE configuration;
  getConfiguration(&configuration);
  sprintf(configuration.latest_version, version);
  saveConfiguration(&configuration);
}

void AFEDataAccess::saveFirmwareAPIVersion() {
  FIRMWARE configuration;
  getConfiguration(&configuration);
  configuration.api = AFE_FIRMWARE_API;
  saveConfiguration(&configuration);
}

uint8_t AFEDataAccess::getDeviceMode() {
  uint8_t mode = AFE_MODE_FIRST_TIME_LAUNCH;
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << F(AFE_FILE_DEVICE_MODE) << F(" ... ");
#endif
#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(AFE_FILE_DEVICE_MODE, "r");
#else
  File configFile = SPIFFS.open(AFE_FILE_DEVICE_MODE, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_DEVICE_MODE> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif
      mode = root["mode"];
#ifdef DEBUG
      printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_DEVICE_MODE,
                          jsonBuffer.size());
#endif
    }
#ifdef DEBUG
    else {
      Serial << F("ERROR: JSON not pharsed");
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else {
    Serial << endl
           << F("ERROR: Configuration file: ") << F(AFE_FILE_DEVICE_MODE)
           << F(" not opened");
  }
#endif
  return mode;
}
void AFEDataAccess::saveDeviceMode(uint8_t mode) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << F(AFE_FILE_DEVICE_MODE) << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(AFE_FILE_DEVICE_MODE, "w");
#else
  File configFile = SPIFFS.open(AFE_FILE_DEVICE_MODE, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_DEVICE_MODE> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["mode"] = mode;
    root.printTo(configFile);

#ifdef DEBUG
    root.printTo(Serial);
#endif

    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_DEVICE_MODE, jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}

void AFEDataAccess::getConfiguration(NETWORK *configuration) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << F(AFE_FILE_NETWORK_CONFIGURATION)
         << F(" ... ");
#endif
#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(AFE_FILE_NETWORK_CONFIGURATION, "r");
#else
  File configFile = SPIFFS.open(AFE_FILE_NETWORK_CONFIGURATION, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_NETWORK> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());

    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      sprintf(configuration->primary.ssid, root["ssid"] | "");
      sprintf(configuration->primary.password, root["password"] | "");
      sprintf(configuration->secondary.ssid, root["ssidBackup"] | "");
      sprintf(configuration->secondary.password, root["passwordBackup"] | "");

      configuration->primary.isDHCP = root["isDHCP"].as<int>();
      JsonVariant exists = root["isDHCPBackup"];
      configuration->secondary.isDHCP =
          exists.success() ? root["isDHCPBackup"] : 1;

      sprintf(configuration->primary.ip, root["ip"]);
      sprintf(configuration->primary.gateway, root["gateway"]);
      sprintf(configuration->primary.subnet, root["subnet"]);

      sprintf(configuration->secondary.ip, root["ipBackup"] | "");
      sprintf(configuration->secondary.gateway, root["gatewayBackup"] | "");
      sprintf(configuration->secondary.subnet, root["subnetBackup"] | "");

      exists = root["dns1"];
      sprintf(configuration->primary.dns1,
              root["dns1"] | AFE_CONFIG_NETWORK_DEFAULT_DNS1);

      exists = root["dns2"];
      sprintf(configuration->primary.dns2,
              root["dns2"] | AFE_CONFIG_NETWORK_DEFAULT_DNS2);

      exists = root["dns1Backup"];
      sprintf(configuration->secondary.dns1,
              root["dns1Backup"] | AFE_CONFIG_NETWORK_DEFAULT_DNS1);

      exists = root["dns2Backup"];
      sprintf(configuration->secondary.dns2,
              root["dns2Backup"] | AFE_CONFIG_NETWORK_DEFAULT_DNS2);

      configuration->noConnectionAttempts =
          root["noConnectionAttempts"].as<int>();
      configuration->waitTimeConnections =
          root["waitTimeConnections"].as<int>();
      configuration->waitTimeSeries = root["waitTimeSeries"].as<int>();
      configuration->noFailuresToSwitchNetwork =
          root["noFailuresToSwitchNetwork"] |
          AFE_CONFIG_NETWORK_DEFAULT_SWITCH_NETWORK_AFTER;

#if !defined(ESP32)
      exists = root["radioMode"];
      configuration->radioMode = exists.success()
                                     ? root["radioMode"].as<int>()
                                     : AFE_CONFIG_NETWORK_DEFAULT_RADIO_MODE;

      exists = root["outputPower"];
      configuration->outputPower =
          exists.success() ? root["outputPower"].as<float>()
                           : AFE_CONFIG_NETWORK_DEFAULT_OUTPUT_POWER;
#endif

    }
#ifdef DEBUG
    else {
      Serial << F("ERROR: JSON not pharsed");
    }
#endif

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_NETWORK, jsonBuffer.size());
#endif

    configFile.close();
  }

#ifdef DEBUG
  else {
    Serial << endl
           << F("ERROR: Configuration file: ")
           << F(AFE_FILE_NETWORK_CONFIGURATION) << F(" not opened");
  }
#endif
}
void AFEDataAccess::saveConfiguration(NETWORK *configuration) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << F(AFE_FILE_NETWORK_CONFIGURATION)
         << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(AFE_FILE_NETWORK_CONFIGURATION, "w");
#else
  File configFile = SPIFFS.open(AFE_FILE_NETWORK_CONFIGURATION, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_NETWORK> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();

    /**
     * @brief Primary network configuration
     *
     */
    root["ssid"] = configuration->primary.ssid;
    root["password"] = configuration->primary.password;
    root["isDHCP"] = configuration->primary.isDHCP;
    root["ip"] = configuration->primary.ip;
    root["gateway"] = configuration->primary.gateway;
    root["subnet"] = configuration->primary.subnet;
    root["dns1"] = configuration->primary.dns1;
    root["dns2"] = configuration->primary.dns2;
    /**
     * @brief Secondary network configuration
     *
     */

    root["ssidBackup"] = configuration->secondary.ssid;
    root["passwordBackup"] = configuration->secondary.password;
    root["isDHCPBackup"] = configuration->secondary.isDHCP;
    root["ipBackup"] = configuration->secondary.ip;
    root["gatewayBackup"] = configuration->secondary.gateway;
    root["subnetBackup"] = configuration->secondary.subnet;
    root["dns1Backup"] = configuration->secondary.dns1;
    root["dns2Backup"] = configuration->secondary.dns2;

    root["noConnectionAttempts"] = configuration->noConnectionAttempts;
    root["waitTimeConnections"] = configuration->waitTimeConnections;
    root["waitTimeSeries"] = configuration->waitTimeSeries;
    root["noFailuresToSwitchNetwork"] =
        configuration->noFailuresToSwitchNetwork;
#if !defined(ESP32)
    root["outputPower"] = configuration->outputPower;
    root["radioMode"] = configuration->radioMode;

#endif

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif

    /*
     char body[AFE_CONFIG_FILE_BUFFER_NETWORK];
     strcpy_P(body, AFE_CONFIG_BODY_NETWORK);

     sprintf(body, body, configuration->ssid, configuration->password,
             configuration->ssidBackup, configuration->passwordBackup,
             configuration->isDHCP, configuration->ip, configuration->gateway,
             configuration->subnet, configuration->isDHCPBackup,
             configuration->ipBackup, configuration->gatewayBackup,
             configuration->subnetBackup, configuration->noConnectionAttempts,
             configuration->waitTimeConnections, configuration->waitTimeSeries,
             configuration->noFailuresToSwitchNetwork,
     configuration->radioMode);

     //"{\"ssid\":\"%s\",\"password\":\"%s\",\"ssidBackup\":\"%s\",\"passwordBackup\":\"%s\",\"isDHCP\":%d,\"ip\":\"%s\",\"gateway\":\"%s\",\"subnet\":\"%s\",\"isDHCPBackup\":%d,\"ipBackup\":\"%s\",\"gatewayBackup\":\"%s\",\"subnetBackup\":\"%s\",\"noConnectionAttempts\":%d,\"waitTimeConnections\":%d,\"waitTimeSeries\":%d,\"noFailuresToSwitchNetwork\":%d,\"radioMode\":%d,\"outputPower\":%d}"

     configFile.print(body);

     */
    configFile.close();
    /*
    #ifdef DEBUG
        printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_NETWORK, jsonBuffer.size());
    #endif
     */
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}
void AFEDataAccess::createNetworkConfigurationFile() {
#ifdef DEBUG
  Serial << endl
         << F("INFO: Creating file: ") << F(AFE_FILE_NETWORK_CONFIGURATION);
#endif

  NETWORK configuration;
  /* Network default config */
  configuration.primary.ssid[0] = AFE_EMPTY_STRING;
  configuration.primary.password[0] = AFE_EMPTY_STRING;
  configuration.primary.isDHCP = true;
  configuration.primary.ip[0] = AFE_EMPTY_STRING;
  configuration.primary.gateway[0] = AFE_EMPTY_STRING;
  configuration.primary.subnet[0] = AFE_EMPTY_STRING;
  configuration.primary.dns1[0] = AFE_EMPTY_STRING;
  configuration.primary.dns2[0] = AFE_EMPTY_STRING;

  configuration.secondary.ssid[0] = AFE_EMPTY_STRING;
  configuration.secondary.password[0] = AFE_EMPTY_STRING;
  configuration.secondary.isDHCP = true;
  configuration.secondary.ip[0] = AFE_EMPTY_STRING;
  configuration.secondary.gateway[0] = AFE_EMPTY_STRING;
  configuration.secondary.subnet[0] = AFE_EMPTY_STRING;
  configuration.secondary.dns1[0] = AFE_EMPTY_STRING;
  configuration.secondary.dns2[0] = AFE_EMPTY_STRING;

  configuration.noConnectionAttempts =
      AFE_CONFIG_NETWORK_DEFAULT_CONNECTION_ATTEMPTS;
  configuration.waitTimeConnections = AFE_CONFIG_NETWORK_DEFAULT_WAIT_TIME;
  configuration.waitTimeSeries = AFE_CONFIG_NETWORK_DEFAULT_WAIT_SERIES;
  configuration.noFailuresToSwitchNetwork =
      AFE_CONFIG_NETWORK_DEFAULT_SWITCH_NETWORK_AFTER;
#if !defined(ESP32)
  configuration.radioMode = AFE_CONFIG_NETWORK_DEFAULT_RADIO_MODE;
  configuration.outputPower = AFE_CONFIG_NETWORK_DEFAULT_OUTPUT_POWER;
#endif
  saveConfiguration(&configuration);
}

void AFEDataAccess::getConfiguration(MQTT *configuration) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << F(AFE_FILE_MQTT_BROKER_CONFIGURATION)
         << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(AFE_FILE_MQTT_BROKER_CONFIGURATION, "r");
#else
  File configFile = SPIFFS.open(AFE_FILE_MQTT_BROKER_CONFIGURATION, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_MQTT_BROKER> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      sprintf(configuration->host, root["host"]);
      sprintf(configuration->ip, root["ip"]);
      configuration->port = root["port"];
      sprintf(configuration->user, root["user"]);
      sprintf(configuration->password, root["password"]);
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
      configuration->lwt.idx = root["lwt"] | AFE_DOMOTICZ_DEFAULT_IDX;
#else
      sprintf(configuration->lwt.topic, root["lwt"] | "");

      JsonVariant exists = root["s"];
      if (exists.success()) {
        sprintf(configuration->status.topic, root["s"]);
      } else {
        char _deviceId[AFE_CONFIG_DEVICE_ID_SIZE];
        getDeviceID(_deviceId);
        sprintf(configuration->status.topic,
                AFE_CONFIG_MQTT_DEFAULT_STATE_TOPIC, _deviceId);
      }

#endif

      configuration->retainLWT =
          root["retainLWT"] | AFE_CONFIG_MQTT_DEFAULT_RETAIN_LWT;
      configuration->retainAll =
          root["retainAll"] | AFE_CONFIG_MQTT_DEFAULT_RETAIN_ALL;
      configuration->qos = root["qos"] | AFE_CONFIG_MQTT_DEFAULT_QOS;

#ifdef DEBUG
      printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_MQTT_BROKER,
                          jsonBuffer.size());
#endif
    }

#ifdef DEBUG
    else {
      Serial << F("ERROR: JSON not pharsed");
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else {
    Serial << endl
           << F("ERROR: Configuration file: ")
           << F(AFE_FILE_MQTT_BROKER_CONFIGURATION) << F(" not opened");
  }
#endif
}
void AFEDataAccess::saveConfiguration(MQTT *configuration) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << F(AFE_FILE_MQTT_BROKER_CONFIGURATION)
         << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(AFE_FILE_MQTT_BROKER_CONFIGURATION, "w");
#else
  File configFile = SPIFFS.open(AFE_FILE_MQTT_BROKER_CONFIGURATION, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_MQTT_BROKER> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["host"] = configuration->host;
    root["ip"] = configuration->ip;
    root["port"] = configuration->port;
    root["user"] = configuration->user;
    root["password"] = configuration->password;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    root["lwt"] = configuration->lwt.idx;
#else
    root["lwt"] = configuration->lwt.topic;
    root["s"] = configuration->status.topic;
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

    root["retainAll"] = configuration->retainAll;
    root["retainLWT"] = configuration->retainLWT;
    root["qos"] = configuration->qos;

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_MQTT_BROKER, jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}
void AFEDataAccess::createMQTTConfigurationFile() {
#ifdef DEBUG
  Serial << endl
         << F("INFO: Creating file: ") << F(AFE_FILE_MQTT_BROKER_CONFIGURATION);
#endif
  MQTT configuration;
  /* MQTT Default config */
  configuration.host[0] = AFE_EMPTY_STRING;
  configuration.ip[0] = AFE_EMPTY_STRING;
  configuration.user[0] = AFE_EMPTY_STRING;
  configuration.password[0] = AFE_EMPTY_STRING;
  configuration.port = AFE_CONFIG_MQTT_DEFAULT_PORT;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  configuration.lwt.idx = AFE_DOMOTICZ_DEFAULT_IDX;
#else  
  char _deviceId[AFE_CONFIG_DEVICE_ID_SIZE];
  getDeviceID(_deviceId);
  sprintf(configuration.status.topic, AFE_CONFIG_MQTT_DEFAULT_STATE_TOPIC,
          _deviceId);
  sprintf(configuration.lwt.topic,"%s/lwt",configuration.status.topic);  
#endif

  configuration.retainAll = AFE_CONFIG_MQTT_DEFAULT_RETAIN_LWT;
  configuration.retainLWT = AFE_CONFIG_MQTT_DEFAULT_RETAIN_ALL;
  configuration.qos = AFE_CONFIG_MQTT_DEFAULT_QOS;
  saveConfiguration(&configuration);
}

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
void AFEDataAccess::getConfiguration(DOMOTICZ *configuration) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << F(AFE_FILE_DOMOTICZ_CONFIGURATION)
         << F(" ... ");
#endif
#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(AFE_FILE_DOMOTICZ_CONFIGURATION, "r");
#else
  File configFile = SPIFFS.open(AFE_FILE_DOMOTICZ_CONFIGURATION, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_DOMOTICZ> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      configuration->protocol = root["protocol"];
      sprintf(configuration->host, root["host"]);
      configuration->port = root["port"];
      sprintf(configuration->user, root["user"]);
      sprintf(configuration->password, root["password"]);

#ifdef DEBUG
      printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_DOMOTICZ, jsonBuffer.size());
#endif
    }
#ifdef DEBUG
    else {
      Serial << F("ERROR: JSON not pharsed");
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else {
    Serial << endl
           << F("ERROR: Configuration file: ")
           << F(AFE_FILE_DOMOTICZ_CONFIGURATION) << F(" not opened");
  }
#endif
}
void AFEDataAccess::saveConfiguration(DOMOTICZ *configuration) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << F(AFE_FILE_DOMOTICZ_CONFIGURATION)
         << F(" ... ");
#endif
#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(AFE_FILE_DOMOTICZ_CONFIGURATION, "w");
#else
  File configFile = SPIFFS.open(AFE_FILE_DOMOTICZ_CONFIGURATION, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_DOMOTICZ> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["protocol"] = configuration->protocol;
    root["host"] = configuration->host;
    root["port"] = configuration->port;
    root["user"] = configuration->user;
    root["password"] = configuration->password;
    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_DOMOTICZ, jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}
void AFEDataAccess::createDomoticzConfigurationFile() {
#ifdef DEBUG
  Serial << endl
         << F("INFO: Creating file: ") << AFE_CONFIG_FILE_BUFFER_MQTT_BROKER;
#endif
  DOMOTICZ DomoticzConfiguration;
  DomoticzConfiguration.protocol = 0;
  DomoticzConfiguration.host[0] = AFE_EMPTY_STRING;
  DomoticzConfiguration.user[0] = AFE_EMPTY_STRING;
  DomoticzConfiguration.password[0] = AFE_EMPTY_STRING;
  DomoticzConfiguration.port = 8080;
  saveConfiguration(&DomoticzConfiguration);
}
#elif AFE_FIRMWARE_API == AFE_FIRMWARE_API_HOME_ASSISTANT
boolean AFEDataAccess::getConfiguration(HOME_ASSISTANT_CONFIG *configuration) {
  boolean _ret = true;
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ")
         << F(AFE_FILE_HOME_ASSISTANT_CONFIGURATION) << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(AFE_FILE_HOME_ASSISTANT_CONFIGURATION, "r");
#else
  File configFile = SPIFFS.open(AFE_FILE_HOME_ASSISTANT_CONFIGURATION, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_HOME_ASSISTANT> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      configuration->removeingComponents =
          root["removing"] | AFE_CONFIG_HA_DEFAULT_DISCOVERY_ADDING_COMPONENTS;
      configuration->addingComponents =
          root["adding"] | AFE_CONFIG_HA_DEFAULT_DISCOVERY_REMOVING_COMPONENTS;
      configuration->retainConfiguration =
          root["retain"] | AFE_CONFIG_HA_DEFAULT_DISCOVERY_RETAIN_CONFIGURATION;
      sprintf(configuration->discovery.topic,
              root["topic"] | AFE_CONFIG_HA_DEFAULT_DISCOVERY_TOPIC);

#ifdef DEBUG
      printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_HOME_ASSISTANT,
                          jsonBuffer.size());
#endif
    } else {
      _ret = false;
#ifdef DEBUG
      Serial << F("ERROR: JSON not pharsed");

#endif
    }
    configFile.close();
  } else {
    _ret = false;
#ifdef DEBUG
    Serial << endl
           << F("ERROR: Configuration file: ")
           << F(AFE_FILE_HOME_ASSISTANT_CONFIGURATION) << F(" not opened");
#endif
  }
  return _ret;
}
void AFEDataAccess::saveConfiguration(HOME_ASSISTANT_CONFIG *configuration) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ")
         << F(AFE_FILE_HOME_ASSISTANT_CONFIGURATION) << F(" ... ");
#endif
#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(AFE_FILE_HOME_ASSISTANT_CONFIGURATION, "w");
#else
  File configFile = SPIFFS.open(AFE_FILE_HOME_ASSISTANT_CONFIGURATION, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_HOME_ASSISTANT> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["adding"] = configuration->addingComponents;
    root["removing"] = configuration->removeingComponents;
    root["retain"] = configuration->retainConfiguration;
    root["topic"] = configuration->discovery.topic;
    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_HOME_ASSISTANT,
                        jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}
void AFEDataAccess::createHomeAssistantConfigurationFile() {
#ifdef DEBUG
  Serial << endl
         << F("INFO: Creating file: ")
         << F(AFE_FILE_HOME_ASSISTANT_CONFIGURATION);
#endif
  HOME_ASSISTANT_CONFIG configuration;
  configuration.addingComponents =
      AFE_CONFIG_HA_DEFAULT_DISCOVERY_ADDING_COMPONENTS;
  configuration.removeingComponents =
      AFE_CONFIG_HA_DEFAULT_DISCOVERY_REMOVING_COMPONENTS;
  configuration.retainConfiguration =
      AFE_CONFIG_HA_DEFAULT_DISCOVERY_RETAIN_CONFIGURATION;
  sprintf(configuration.discovery.topic, AFE_CONFIG_HA_DEFAULT_DISCOVERY_TOPIC);
  saveConfiguration(&configuration);
}
#endif // AFE_FIRMWARE_API

#ifdef AFE_CONFIG_HARDWARE_LED
void AFEDataAccess::getConfiguration(uint8_t id, LED *configuration) {
  char fileName[18];
  sprintf(fileName, AFE_FILE_LED_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  // if (!LittleFS.exists(fileName)) {
  //  createLEDConfigurationFile(id);
  // }
  File configFile = LittleFS.open(fileName, "r");
#else
  //  if (!SPIFFS.exists(fileName)) {
  //    createLEDConfigurationFile(id);
  //  }
  File configFile = SPIFFS.open(fileName, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_LED> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      configuration->gpio = root["gpio"];
      configuration->changeToOppositeValue = root["changeToOppositeValue"];
#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
      configuration->mcp23017.id = root["mcp23017"]["id"];
      configuration->mcp23017.gpio = root["mcp23017"]["gpio"];
#endif

#ifdef DEBUG
      printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_LED, jsonBuffer.size());
#endif
    }
#ifdef DEBUG
    else {
      Serial << F("ERROR: JSON not pharsed");
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else {
    Serial << endl
           << F("ERROR: Configuration file: ") << fileName << F(" not opened");
  }
#endif
}

void AFEDataAccess::saveConfiguration(uint8_t id, LED *configuration) {
  char fileName[18];
  sprintf(fileName, AFE_FILE_LED_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "w");
#else
#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "w");
#else
  File configFile = SPIFFS.open(fileName, "w");
#endif
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_LED> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();

    root["gpio"] = configuration->gpio;
    root["changeToOppositeValue"] = configuration->changeToOppositeValue;

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
    JsonObject &mcp23017 = root.createNestedObject("mcp23017");
    mcp23017["id"] = configuration->mcp23017.id;
    mcp23017["gpio"] = configuration->mcp23017.gpio;
#endif // AFE_CONFIG_HARDWARE_MCP23XXX

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_LED, jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}
void AFEDataAccess::createLEDConfigurationFile() {
  LED LEDConfiguration;
  uint8_t index = 0;

#if defined(AFE_DEVICE_SONOFF_BASIC_V1)
  LEDConfiguration.changeToOppositeValue = false;
  LEDConfiguration.gpio = AFE_CONFIG_HARDWARE_LED_0_DEFAULT_GPIO;
  saveConfiguration(0, &LEDConfiguration);
  LEDConfiguration.gpio = AFE_CONFIG_HARDWARE_LED_1_DEFAULT_GPIO;
  saveConfiguration(1, &LEDConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS;
#elif defined(AFE_DEVICE_SONOFF_4CH)
  LEDConfiguration.changeToOppositeValue = false;
  LEDConfiguration.gpio = AFE_CONFIG_HARDWARE_LED_0_DEFAULT_GPIO;
  saveConfiguration(0, &LEDConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS;
#elif defined(AFE_DEVICE_SONOFF_TOUCH_1G)
  LEDConfiguration.changeToOppositeValue = false;
  LEDConfiguration.gpio = AFE_CONFIG_HARDWARE_LED_0_DEFAULT_GPIO;
  saveConfiguration(0, &LEDConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS;
#elif defined(AFE_DEVICE_SONOFF_TOUCH_2G)
  LEDConfiguration.changeToOppositeValue = false;
  LEDConfiguration.gpio = AFE_CONFIG_HARDWARE_LED_0_DEFAULT_GPIO;
  saveConfiguration(0, &LEDConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS;
#elif defined(AFE_DEVICE_SONOFF_TOUCH_3G)
  LEDConfiguration.changeToOppositeValue = false;
  LEDConfiguration.gpio = AFE_CONFIG_HARDWARE_LED_0_DEFAULT_GPIO;
  saveConfiguration(0, &LEDConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS;
#elif defined(AFE_DEVICE_iECS_GATE_DRIVERv2)
  LEDConfiguration.changeToOppositeValue = true;
  LEDConfiguration.gpio = AFE_CONFIG_HARDWARE_LED_0_DEFAULT_GPIO;
  saveConfiguration(0, &LEDConfiguration);
  index = AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_LEDS;
#elif defined(AFE_DEVICE_iECS_WHEATER_STATION_20)
  LEDConfiguration.changeToOppositeValue = true;
  LEDConfiguration.gpio = AFE_CONFIG_HARDWARE_LED_0_DEFAULT_GPIO;
  saveConfiguration(0, &LEDConfiguration);
  index = AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_LEDS;
#else
  LEDConfiguration.changeToOppositeValue = false;
  LEDConfiguration.gpio = AFE_CONFIG_HARDWARE_LED_0_DEFAULT_GPIO;
#endif

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
  LEDConfiguration.mcp23017.id = AFE_HARDWARE_ITEM_NOT_EXIST;
  LEDConfiguration.mcp23017.gpio = AFE_HARDWARE_ITEM_NOT_EXIST;
#endif // AFE_CONFIG_HARDWARE_MCP23XXX

  for (uint8_t i = index; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_LEDS; i++) {
#ifdef DEBUG
    Serial << endl << F("INFO: Creating file: cfg-led-") << i << F(".json");
#endif
    saveConfiguration(i, &LEDConfiguration);
  }
}
/*
void AFEDataAccess::createLEDConfigurationFile(uint8_t id) {
  LED LEDConfiguration;

  LEDConfiguration.changeToOppositeValue = false;
  LEDConfiguration.gpio = AFE_CONFIG_HARDWARE_LED_0_DEFAULT_GPIO;

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
  LEDConfiguration.mcp23017.address =
      AFE_CONFIG_HARDWARE_I2C_DEFAULT_NON_EXIST_ADDRESS;
  LEDConfiguration.mcp23017.gpio = AFE_HARDWARE_ITEM_NOT_EXIST;
#endif // AFE_CONFIG_HARDWARE_MCP23XXX

#ifdef DEBUG
  Serial << endl << F("INFO: Creating file: cfg-led-") << id << F(".json");
#endif
  saveConfiguration(id, &LEDConfiguration);
}
*/
uint8_t AFEDataAccess::getSystemLedID() {
  uint8_t id = 0;
#ifdef DEBUG
  Serial << endl
         << F("INFO: Opening file: ") << F(AFE_FILE_SYSTEM_LED_CONFIGURATION)
         << F(" ... ");
#endif
#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(AFE_FILE_SYSTEM_LED_CONFIGURATION, "r");
#else
  File configFile = SPIFFS.open(AFE_FILE_SYSTEM_LED_CONFIGURATION, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_SYSTEM_LED> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif
      id = root["id"];

#ifdef DEBUG
      printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_SYSTEM_LED, jsonBuffer.size());
#endif
    }
#ifdef DEBUG
    else {
      Serial << F("ERROR: JSON not pharsed");
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else {
    Serial << endl
           << F("ERROR: Configuration file: ")
           << AFE_FILE_SYSTEM_LED_CONFIGURATION << F(" not opened");
  }
#endif
  return id;
}
void AFEDataAccess::saveSystemLedID(uint8_t id) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << F(AFE_FILE_SYSTEM_LED_CONFIGURATION)
         << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(AFE_FILE_SYSTEM_LED_CONFIGURATION, "w");
#else
  File configFile = SPIFFS.open(AFE_FILE_SYSTEM_LED_CONFIGURATION, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_SYSTEM_LED> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["id"] = id;

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_SYSTEM_LED, jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}
void AFEDataAccess::createSystemLedIDConfigurationFile() {
#ifdef DEBUG
  Serial << endl
         << F("INFO: Creating file: ") << F(AFE_FILE_SYSTEM_LED_CONFIGURATION);
#endif
  saveSystemLedID(0);
}
#endif // AFE_CONFIG_HARDWARE_LED

#ifdef AFE_CONFIG_HARDWARE_RELAY

boolean AFEDataAccess::getConfiguration(uint8_t id, RELAY *configuration) {
  boolean _ret = false;
  char fileName[sizeof(AFE_FILE_RELAY_CONFIGURATION) + 1];
  sprintf(fileName, AFE_FILE_RELAY_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "r");
#else
  File configFile = SPIFFS.open(fileName, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_RELAY> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());

    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif
      configuration->gpio = root["gpio"];
      sprintf(configuration->name, root["name"]);
      configuration->timeToOff = root["timeToOff"];
      configuration->state.powerOn = root["statePowerOn"];
      configuration->state.MQTTConnected =
          root["stateMQTTConnected"] |
          AFE_CONFIG_HARDWARE_RELAY_DEFAULT_STATE_MQTT_CONNECTED;

      configuration->triggerSignal =
          root["triggerSignal"] |
          AFE_CONFIG_HARDWARE_RELAY_DEFAULT_SIGNAL_TRIGGER;

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
      configuration->domoticz.idx = root["idx"] | AFE_DOMOTICZ_DEFAULT_IDX;
#else
      sprintf(configuration->mqtt.topic, root["MQTTTopic"] | "");
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
      configuration->ledID = root["ledID"];
#endif

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
      configuration->mcp23017.id = root["mcp23017"]["id"];
      configuration->mcp23017.gpio = root["mcp23017"]["gpio"];
#endif

#ifdef DEBUG
      printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_RELAY, jsonBuffer.size());
#endif
      _ret = true;
    }
#ifdef DEBUG
    else {
      Serial << F("ERROR: JSON not pharsed");
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else {
    Serial << endl
           << F("ERROR: Configuration file: ") << fileName << F(" not opened");
  }
#endif
  return _ret;
}
void AFEDataAccess::saveConfiguration(uint8_t id, RELAY *configuration) {
  char fileName[20];
  sprintf(fileName, AFE_FILE_RELAY_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "w");
#else
  File configFile = SPIFFS.open(fileName, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_RELAY> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();

    root["gpio"] = configuration->gpio;
    root["name"] = configuration->name;
    root["timeToOff"] = configuration->timeToOff;
    root["statePowerOn"] = configuration->state.powerOn;
    root["stateMQTTConnected"] = configuration->state.MQTTConnected;
    root["triggerSignal"] = configuration->triggerSignal;
#ifdef AFE_CONFIG_HARDWARE_LED
    root["ledID"] = configuration->ledID;
#endif
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    root["idx"] = configuration->domoticz.idx;
#else
    root["MQTTTopic"] = configuration->mqtt.topic;
#endif

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
    JsonObject &mcp23017 = root.createNestedObject("mcp23017");
    mcp23017["id"] = configuration->mcp23017.id;
    mcp23017["gpio"] = configuration->mcp23017.gpio;
#endif // AFE_CONFIG_HARDWARE_MCP23XXX

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_RELAY, jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}
void AFEDataAccess::createRelayConfigurationFile() {

  RELAY RelayConfiguration;
  uint8_t index = 0;
/* Relay config */
#ifdef AFE_CONFIG_HARDWARE_LED
  RelayConfiguration.ledID = AFE_HARDWARE_ITEM_NOT_EXIST;
#endif

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  RelayConfiguration.domoticz.idx = AFE_DOMOTICZ_DEFAULT_IDX;
#else
  RelayConfiguration.mqtt.topic[0] = AFE_EMPTY_STRING;
#endif
  RelayConfiguration.state.MQTTConnected =
      AFE_CONFIG_HARDWARE_RELAY_DEFAULT_STATE_MQTT_CONNECTED;

#if !(defined(AFE_DEVICE_iECS_GATE_DRIVERv2) ||                                \
      defined(AFE_DEVICE_iECS_GATE_DRIVERv3))
  RelayConfiguration.timeToOff = AFE_CONFIG_HARDWARE_RELAY_DEFAULT_TIME_TO_OFF;
  RelayConfiguration.state.powerOn =
      AFE_CONFIG_HARDWARE_RELAY_DEFAULT_STATE_POWER_ON;
#endif

  RelayConfiguration.triggerSignal =
      AFE_CONFIG_HARDWARE_RELAY_DEFAULT_SIGNAL_TRIGGER;

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
  RelayConfiguration.mcp23017.id = AFE_HARDWARE_ITEM_NOT_EXIST;
  RelayConfiguration.mcp23017.gpio = AFE_HARDWARE_ITEM_NOT_EXIST;
#endif // AFE_CONFIG_HARDWARE_MCP23XXX

/* SONOFF Basic v1 */
#if defined(AFE_DEVICE_SONOFF_BASIC_V1)
  RelayConfiguration.gpio = AFE_CONFIG_HARDWARE_RELAY_0_DEFAULT_GPIO;
  sprintf(RelayConfiguration.name, AFE_CONFIG_HARDWARE_RELAY_0_DEFAULT_NAME);
#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY
  saveRelayState(0, false);
#endif
  saveConfiguration(0, &RelayConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS;
/* SONOFF 4CH */
#elif defined(AFE_DEVICE_SONOFF_4CH)
  RelayConfiguration.gpio = AFE_CONFIG_HARDWARE_RELAY_0_DEFAULT_GPIO;
  sprintf(RelayConfiguration.name, AFE_CONFIG_HARDWARE_RELAY_0_DEFAULT_NAME);
#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY
  saveRelayState(0, false);
#endif
  saveConfiguration(0, &RelayConfiguration);
  RelayConfiguration.gpio = AFE_CONFIG_HARDWARE_RELAY_1_DEFAULT_GPIO;
  sprintf(RelayConfiguration.name, AFE_CONFIG_HARDWARE_RELAY_1_DEFAULT_NAME);
#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY
  saveRelayState(1, false);
#endif
  saveConfiguration(1, &RelayConfiguration);
  RelayConfiguration.gpio = AFE_CONFIG_HARDWARE_RELAY_2_DEFAULT_GPIO;
  sprintf(RelayConfiguration.name, AFE_CONFIG_HARDWARE_RELAY_2_DEFAULT_NAME);
#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY
  saveRelayState(2, false);
#endif
  saveConfiguration(2, &RelayConfiguration);

  RelayConfiguration.gpio = AFE_CONFIG_HARDWARE_RELAY_3_DEFAULT_GPIO;
  sprintf(RelayConfiguration.name, AFE_CONFIG_HARDWARE_RELAY_3_DEFAULT_NAME);
#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY
  saveRelayState(3, false);
#endif
  saveConfiguration(3, &RelayConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS;
/* SONOFF Mini Extreme R4 */
#elif defined(AFE_DEVICE_SONOFF_MINI_R4)
  RelayConfiguration.gpio = AFE_CONFIG_HARDWARE_RELAY_0_DEFAULT_GPIO;
  sprintf(RelayConfiguration.name, AFE_CONFIG_HARDWARE_RELAY_0_DEFAULT_NAME);
#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY
  saveRelayState(0, false);
#endif
  saveConfiguration(0, &RelayConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS;
/* SONOFF Touch 1G */
#elif defined(AFE_DEVICE_SONOFF_TOUCH_1G)
  RelayConfiguration.gpio = AFE_CONFIG_HARDWARE_RELAY_0_DEFAULT_GPIO;
  sprintf(RelayConfiguration.name, AFE_CONFIG_HARDWARE_RELAY_0_DEFAULT_NAME);
#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY
  saveRelayState(0, false);
#endif
  saveConfiguration(0, &RelayConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS;
/* SONOFF Touch 2G */
#elif defined(AFE_DEVICE_SONOFF_TOUCH_2G)
  RelayConfiguration.gpio = AFE_CONFIG_HARDWARE_RELAY_0_DEFAULT_GPIO;
  sprintf(RelayConfiguration.name, AFE_CONFIG_HARDWARE_RELAY_0_DEFAULT_NAME);
#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY
  saveRelayState(0, false);
#endif
  saveConfiguration(0, &RelayConfiguration);
  RelayConfiguration.gpio = AFE_CONFIG_HARDWARE_RELAY_1_DEFAULT_GPIO;
  sprintf(RelayConfiguration.name, AFE_CONFIG_HARDWARE_RELAY_1_DEFAULT_NAME);
#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY
  saveRelayState(1, false);
#endif
  saveConfiguration(1, &RelayConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS;
/* SONOFF Touch 3G */
#elif defined(AFE_DEVICE_SONOFF_TOUCH_3G)
  RelayConfiguration.gpio = AFE_CONFIG_HARDWARE_RELAY_0_DEFAULT_GPIO;
  sprintf(RelayConfiguration.name, AFE_CONFIG_HARDWARE_RELAY_0_DEFAULT_NAME);
#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY
  saveRelayState(0, false);
#endif
  saveConfiguration(0, &RelayConfiguration);
  RelayConfiguration.gpio = AFE_CONFIG_HARDWARE_RELAY_1_DEFAULT_GPIO;
  sprintf(RelayConfiguration.name, AFE_CONFIG_HARDWARE_RELAY_1_DEFAULT_NAME);
#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY
  saveRelayState(1, false);
#endif
  saveConfiguration(1, &RelayConfiguration);
  RelayConfiguration.gpio = AFE_CONFIG_HARDWARE_RELAY_2_DEFAULT_GPIO;
  sprintf(RelayConfiguration.name, AFE_CONFIG_HARDWARE_RELAY_2_DEFAULT_NAME);
#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY
  saveRelayState(2, false);
#endif
  saveConfiguration(2, &RelayConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS;
/* Shelly-1 */
#elif defined(AFE_DEVICE_SHELLY_1)
  RelayConfiguration.gpio = AFE_CONFIG_HARDWARE_RELAY_0_DEFAULT_GPIO;
  sprintf(RelayConfiguration.name, AFE_CONFIG_HARDWARE_RELAY_0_DEFAULT_NAME);
#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY
  saveRelayState(0, false);
#endif
  saveConfiguration(0, &RelayConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS;
#elif defined(AFE_DEVICE_iECS_GATE_DRIVERv2) ||                                \
    defined(AFE_DEVICE_iECS_GATE_DRIVERv3)
  RelayConfiguration.gpio = AFE_CONFIG_HARDWARE_RELAY_0_DEFAULT_GPIO;
  sprintf(RelayConfiguration.name, AFE_CONFIG_HARDWARE_RELAY_0_DEFAULT_NAME);
  saveRelayState(1, false);
  saveConfiguration(1, &RelayConfiguration);
  RelayConfiguration.gpio = AFE_CONFIG_HARDWARE_RELAY_1_DEFAULT_GPIO;
  sprintf(RelayConfiguration.name, AFE_CONFIG_HARDWARE_RELAY_1_DEFAULT_NAME);
  RelayConfiguration.timeToOff = AFE_CONFIG_HARDWARE_RELAY_DEFAULT_TIME_TO_OFF;
  RelayConfiguration.state.powerOn =
      AFE_CONFIG_HARDWARE_RELAY_DEFAULT_STATE_POWER_ON;
  saveRelayState(0, false);
  saveConfiguration(0, &RelayConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS;
#elif defined(AFE_DEVICE_iECS_WHEATER_STATION_20)
  RelayConfiguration.gpio = AFE_CONFIG_HARDWARE_RELAY_0_DEFAULT_GPIO;
  sprintf(RelayConfiguration.name, AFE_CONFIG_HARDWARE_RELAY_0_DEFAULT_NAME);
#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY
  saveRelayState(0, false);
#endif
  saveConfiguration(0, &RelayConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS;
#endif

  /* Adding config files for remaining relays */
  RelayConfiguration.gpio = AFE_HARDWARE_ITEM_NOT_EXIST;
  for (uint8_t i = index; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_RELAYS; i++) {
#ifdef DEBUG
    Serial << endl << F("INFO: Creating file: cfg-relay-") << i << F(".json");
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY
    saveRelayState(i, false);
#endif
    sprintf(RelayConfiguration.name, "R%d", i + 1);
    saveConfiguration(i, &RelayConfiguration);
  }
}

/* Relay state methods*/
boolean AFEDataAccess::getRelayState(uint8_t id) {
  boolean state = false;
  char fileName[26];
  sprintf(fileName, AFE_FILE_RELAY_STATE_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "r");
#else
  File configFile = SPIFFS.open(fileName, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_RELAY_STATE> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif
      state = root["state"];

#ifdef DEBUG
      printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_RELAY_STATE,
                          jsonBuffer.size());
#endif
    }
#ifdef DEBUG
    else {
      Serial << F("ERROR: JSON not pharsed");
    }
#endif
    configFile.close();
  }

#ifdef DEBUG
  else {
    Serial << endl
           << F("ERROR: Configuration file: ") << fileName << F(" not opened");
  }
#endif
  return state;
}
void AFEDataAccess::saveRelayState(uint8_t id, boolean state) {
  char fileName[26];
  sprintf(fileName, AFE_FILE_RELAY_STATE_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "w");
#else
  File configFile = SPIFFS.open(fileName, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_RELAY_STATE> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();

    root["state"] = state;

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_RELAY_STATE, jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_SWITCH
boolean AFEDataAccess::getConfiguration(uint8_t id, SWITCH *configuration) {
  boolean _ret = false;
  char fileName[sizeof(AFE_FILE_SWITCH_CONFIGURATION) + 1];
  sprintf(fileName, AFE_FILE_SWITCH_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "r");
#else
  File configFile = SPIFFS.open(fileName, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_SWITCH> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif
      configuration->gpio = root["gpio"].as<int>();
      configuration->type = root["type"];
      configuration->sensitiveness = root["sensitiveness"];
      configuration->functionality = root["functionality"];
#ifdef AFE_CONFIG_HARDWARE_RELAY
      configuration->relayID = root["relayID"];
#endif
#ifdef AFE_CONFIG_HARDWARE_CLED
      JsonVariant exists = root["rgbLedID"];
      configuration->rgbLedID =
          exists.success() ? root["rgbLedID"] : AFE_HARDWARE_ITEM_NOT_EXIST;
#endif
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
      configuration->domoticz.idx = root["idx"] | AFE_DOMOTICZ_DEFAULT_IDX;

#else
      sprintf(configuration->mqtt.topic, root["MQTTTopic"] | "");
#endif

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
      configuration->mcp23017.id = root["mcp23017"]["id"];
      configuration->mcp23017.gpio = root["mcp23017"]["gpio"];
#endif

#ifdef DEBUG
      printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_SWITCH, jsonBuffer.size());
#endif
      _ret = true;
    }
#ifdef DEBUG
    else {
      Serial << F("ERROR: JSON not pharsed");
    }
#endif
    configFile.close();
  }

#ifdef DEBUG
  else {
    Serial << endl
           << F("ERROR: Configuration file: ") << fileName << F(" not opened");
  }
#endif
  return _ret;
}

void AFEDataAccess::saveConfiguration(uint8_t id, SWITCH *configuration) {
  char fileName[21];
  sprintf(fileName, AFE_FILE_SWITCH_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "w");
#else
  File configFile = SPIFFS.open(fileName, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_SWITCH> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["gpio"] = configuration->gpio;
    root["type"] = configuration->type;
    root["sensitiveness"] = configuration->sensitiveness;
    root["functionality"] = configuration->functionality;
#ifdef AFE_CONFIG_HARDWARE_RELAY
    root["relayID"] = configuration->relayID;
#endif
#ifdef AFE_CONFIG_HARDWARE_CLED
    root["rgbLedID"] = configuration->rgbLedID;
#endif
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    root["idx"] = configuration->domoticz.idx;
#else
    root["MQTTTopic"] = configuration->mqtt.topic;
#endif

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
    JsonObject &mcp23017 = root.createNestedObject("mcp23017");
    mcp23017["id"] = configuration->mcp23017.id;
    mcp23017["gpio"] = configuration->mcp23017.gpio;
#endif // AFE_CONFIG_HARDWARE_MCP23XXX

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_SWITCH, jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}
void AFEDataAccess::createSwitchConfigurationFile() {
  SWITCH SwitchConfiguration;
  uint8_t index = 0;

  SwitchConfiguration.sensitiveness = AFE_HARDWARE_SWITCH_DEFAULT_BOUNCING;
#ifdef AFE_CONFIG_HARDWARE_RELAY
#if defined(AFE_DEVICE_iECS_WHEATER_STATION_20)
  SwitchConfiguration.relayID = AFE_HARDWARE_ITEM_NOT_EXIST;
#else
  SwitchConfiguration.relayID = 0;
#endif
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_CLED
  SwitchConfiguration.rgbLedID = AFE_HARDWARE_ITEM_NOT_EXIST;
#endif // AFE_CONFIG_HARDWARE_CLED

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  SwitchConfiguration.domoticz.idx = AFE_DOMOTICZ_DEFAULT_IDX;
#else
  SwitchConfiguration.mqtt.topic[0] = AFE_EMPTY_STRING;
#endif
  /* Saving first switch. Common for all devices */
  SwitchConfiguration.gpio = AFE_HARDWARE_SWITCH_0_DEFAULT_GPIO;
  SwitchConfiguration.type = AFE_HARDWARE_SWITCH_0_DEFAULT_TYPE;
  SwitchConfiguration.functionality =
      AFE_HARDWARE_SWITCH_0_DEFAULT_FUNCTIONALITY;

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
  SwitchConfiguration.mcp23017.id = AFE_HARDWARE_ITEM_NOT_EXIST;
  SwitchConfiguration.mcp23017.gpio = AFE_HARDWARE_ITEM_NOT_EXIST;
#endif // AFE_CONFIG_HARDWARE_MCP23XXX

  saveConfiguration(0, &SwitchConfiguration);

#if defined(AFE_DEVICE_SONOFF_BASIC_V1)
  SwitchConfiguration.type = AFE_HARDWARE_SWITCH_X_DEFAULT_TYPE;
  SwitchConfiguration.functionality =
      AFE_HARDWARE_SWITCH_X_DEFAULT_FUNCTIONALITY;
  SwitchConfiguration.gpio = AFE_HARDWARE_SWITCH_1_DEFAULT_GPIO;
  saveConfiguration(1, &SwitchConfiguration);
  SwitchConfiguration.gpio = AFE_HARDWARE_SWITCH_2_DEFAULT_GPIO;
  saveConfiguration(2, &SwitchConfiguration);
  SwitchConfiguration.gpio = AFE_HARDWARE_SWITCH_3_DEFAULT_GPIO;
  saveConfiguration(3, &SwitchConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES;
#elif defined(AFE_DEVICE_SONOFF_4CH)
  SwitchConfiguration.type = AFE_HARDWARE_SWITCH_X_DEFAULT_TYPE;
  SwitchConfiguration.functionality =
      AFE_HARDWARE_SWITCH_X_DEFAULT_FUNCTIONALITY;
  SwitchConfiguration.gpio = AFE_HARDWARE_SWITCH_1_DEFAULT_GPIO;
  SwitchConfiguration.relayID = 1;
  saveConfiguration(1, &SwitchConfiguration);
  SwitchConfiguration.gpio = AFE_HARDWARE_SWITCH_2_DEFAULT_GPIO;
  SwitchConfiguration.relayID = 2;
  saveConfiguration(2, &SwitchConfiguration);
  SwitchConfiguration.gpio = AFE_HARDWARE_SWITCH_3_DEFAULT_GPIO;
  SwitchConfiguration.relayID = 3;
  saveConfiguration(3, &SwitchConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES;
#elif defined(AFE_DEVICE_SONOFF_TOUCH_1G)
  // Just one switch is possible for sonoff touch 1 gang, and one config file is
  // created
  index = AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_SWITCHES;
#elif defined(AFE_DEVICE_SONOFF_TOUCH_2G)
  SwitchConfiguration.type = AFE_HARDWARE_SWITCH_X_DEFAULT_TYPE;
  SwitchConfiguration.functionality =
      AFE_HARDWARE_SWITCH_X_DEFAULT_FUNCTIONALITY;
  SwitchConfiguration.gpio = AFE_HARDWARE_SWITCH_1_DEFAULT_GPIO;
  SwitchConfiguration.relayID = 1;
  saveConfiguration(1, &SwitchConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES;
#elif defined(AFE_DEVICE_SONOFF_TOUCH_3G)
  SwitchConfiguration.type = AFE_HARDWARE_SWITCH_X_DEFAULT_TYPE;
  SwitchConfiguration.functionality =
      AFE_HARDWARE_SWITCH_X_DEFAULT_FUNCTIONALITY;
  SwitchConfiguration.gpio = AFE_HARDWARE_SWITCH_1_DEFAULT_GPIO;
  SwitchConfiguration.relayID = 1;
  saveConfiguration(1, &SwitchConfiguration);
  SwitchConfiguration.gpio = AFE_HARDWARE_SWITCH_2_DEFAULT_GPIO;
  SwitchConfiguration.relayID = 2;
  saveConfiguration(2, &SwitchConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES;
#elif defined(AFE_DEVICE_SHELLY_1)
  // Just one switch is possible for Shelly-1, and one config file is created
  // for Switch
  index = AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_SWITCHES;
#elif defined(AFE_DEVICE_iECS_GATE_DRIVERv2)
  SwitchConfiguration.type = AFE_HARDWARE_SWITCH_X_DEFAULT_TYPE;
  SwitchConfiguration.functionality =
      AFE_HARDWARE_SWITCH_X_DEFAULT_FUNCTIONALITY;
  SwitchConfiguration.gpio = AFE_HARDWARE_SWITCH_1_DEFAULT_GPIO;
  SwitchConfiguration.functionality = AFE_SWITCH_FUNCTIONALITY_NONE;
  SwitchConfiguration.relayID = AFE_HARDWARE_ITEM_NOT_EXIST;
  saveConfiguration(1, &SwitchConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES;
#elif defined(AFE_DEVICE_iECS_WHEATER_STATION_20)
  SwitchConfiguration.type = AFE_HARDWARE_SWITCH_X_DEFAULT_TYPE;
  SwitchConfiguration.functionality =
      AFE_HARDWARE_SWITCH_X_DEFAULT_FUNCTIONALITY;
  SwitchConfiguration.gpio = AFE_HARDWARE_SWITCH_1_DEFAULT_GPIO;
  SwitchConfiguration.relayID = AFE_HARDWARE_ITEM_NOT_EXIST;
  saveConfiguration(1, &SwitchConfiguration);
  SwitchConfiguration.gpio = AFE_HARDWARE_SWITCH_2_DEFAULT_GPIO;
  SwitchConfiguration.relayID = AFE_HARDWARE_ITEM_NOT_EXIST;
  saveConfiguration(2, &SwitchConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES;
#else
  index = 1; // First switch created already
#endif
  if (index < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_SWITCHES) {
    SwitchConfiguration.gpio = AFE_HARDWARE_ITEM_NOT_EXIST;
    SwitchConfiguration.type = AFE_HARDWARE_SWITCH_X_DEFAULT_TYPE;
    SwitchConfiguration.functionality =
        AFE_HARDWARE_SWITCH_X_DEFAULT_FUNCTIONALITY;
    for (uint8_t i = index; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_SWITCHES;
         i++) {
#ifdef DEBUG
      Serial << endl
             << F("INFO: Creating file: cfg-switch-") << i << F(".json");
#endif
#ifdef AFE_CONFIG_HARDWARE_RELAY
      SwitchConfiguration.relayID = AFE_HARDWARE_ITEM_NOT_EXIST;
#endif
      saveConfiguration(i, &SwitchConfiguration);
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_SWITCH

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
#ifdef AFE_ESP32
void AFEDataAccess::getConfiguration(uint8_t id, ADCINPUT *configuration) {
  char fileName[26];
  sprintf(fileName, AFE_FILE_ADC_CONFIGURATION, id);
#else
void AFEDataAccess::getConfiguration(ADCINPUT *configuration) {
  char fileName[23];
  sprintf(fileName, AFE_FILE_ADC_CONFIGURATION);
#endif // AFE_ESP32

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "r");
#else
  File configFile = SPIFFS.open(fileName, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_ADC> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif

#ifdef AFE_ESP32
      sprintf(configuration->name, root["name"]);
#endif

      configuration->gpio = root["gpio"];
      configuration->interval = root["interval"];
      configuration->numberOfSamples = root["numberOfSamples"];
      configuration->maxVCC = root["maxVCC"];
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
      configuration->domoticz.raw =
          root["idx"]["raw"] | AFE_DOMOTICZ_DEFAULT_IDX;
      configuration->domoticz.percent =
          root["idx"]["percent"] | AFE_DOMOTICZ_DEFAULT_IDX;
      configuration->domoticz.voltage =
          root["idx"]["voltage"] | AFE_DOMOTICZ_DEFAULT_IDX;
      configuration->domoticz.voltageCalculated =
          root["idx"]["voltageCalculated"] | AFE_DOMOTICZ_DEFAULT_IDX;
#else
      sprintf(configuration->mqtt.topic, root["mqttTopic"] | "");
#endif
      configuration->divider.Ra = root["divider"]["Ra"];
      configuration->divider.Rb = root["divider"]["Rb"];

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
      configuration->battery.maxVoltage =
          root["batteryMeter"]["maxV"].as<float>();
      configuration->battery.minVoltage =
          root["batteryMeter"]["minV"].as<float>();
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
      configuration->battery.domoticz.idx =
          root["batteryMeter"]["idx"] | AFE_DOMOTICZ_DEFAULT_IDX;
#else
      sprintf(configuration->battery.mqtt.topic,
              root["batteryMeter"]["mqttTopic"] | "");
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
#endif // AFE_CONFIG_FUNCTIONALITY_BATTERYMETER

#ifdef DEBUG
      printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_ADC, jsonBuffer.size());
#endif
    }
#ifdef DEBUG
    else {
      Serial << F("ERROR: JSON not pharsed");
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else {
    Serial << endl
           << F("ERROR: Configuration file: ") << fileName << F(" not opened");
  }
#endif
}

#ifdef AFE_ESP32
void AFEDataAccess::saveConfiguration(uint8_t id, ADCINPUT *configuration) {
  char fileName[26];
  sprintf(fileName, AFE_FILE_ADC_CONFIGURATION, id);
#else
void AFEDataAccess::saveConfiguration(ADCINPUT *configuration) {
  char fileName[23];
  sprintf(fileName, AFE_FILE_ADC_CONFIGURATION);
#endif

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "w");
#else
  File configFile = SPIFFS.open(fileName, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_ADC> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    JsonObject &idx = root.createNestedObject("idx");
#endif
    JsonObject &divider = root.createNestedObject("divider");

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
    JsonObject &battery = root.createNestedObject("batteryMeter");
#endif

#ifdef AFE_ESP32
    root["name"] = configuration->name;
#endif
    root["gpio"] = configuration->gpio;
    root["interval"] = configuration->interval;
    root["numberOfSamples"] = configuration->numberOfSamples;
    root["maxVCC"] = configuration->maxVCC;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    idx["raw"] = configuration->domoticz.raw;
    idx["percent"] = configuration->domoticz.percent;
    idx["voltage"] = configuration->domoticz.voltage;
    idx["voltageCalculated"] = configuration->domoticz.voltageCalculated;
#else
    root["mqttTopic"] = configuration->mqtt.topic;
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
    divider["Ra"] = configuration->divider.Ra;
    divider["Rb"] = configuration->divider.Rb;

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
    battery["maxV"] = configuration->battery.maxVoltage;
    battery["minV"] = configuration->battery.minVoltage;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    battery["idx"] = configuration->battery.domoticz.idx;
#else
    battery["mqttTopic"] = configuration->battery.mqtt.topic;
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
#endif // AFE_CONFIG_FUNCTIONALITY_BATTERYMETER

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_ADC, jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
};

void AFEDataAccess::createADCInputConfigurationFile() {
#ifdef DEBUG
  Serial << endl << F("INFO: Creating file: ") << F(AFE_FILE_ADC_CONFIGURATION);
#endif
  ADCINPUT AnalogInputConfiguration;
  AnalogInputConfiguration.gpio = AFE_CONFIG_HARDWARE_ANALOG_INPUT_DEFAULT_GPIO;
  AnalogInputConfiguration.interval =
      AFE_CONFIG_HARDWARE_ANALOG_INPUT_DEFAULT_INTERVAL;
  AnalogInputConfiguration.numberOfSamples =
      AFE_CONFIG_HARDWARE_ANALOG_INPUT_DEFAULT_NUMBER_OF_SAMPLES;
  AnalogInputConfiguration.maxVCC =
      AFE_CONFIG_HARDWARE_ANALOG_INPUT_DEFAULT_MAX_VCC;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  AnalogInputConfiguration.domoticz.raw = 0;
  AnalogInputConfiguration.domoticz.voltage = 0;
  AnalogInputConfiguration.domoticz.percent = 0;
  AnalogInputConfiguration.domoticz.voltageCalculated = 0;
#else
  AnalogInputConfiguration.mqtt.topic[0] = AFE_EMPTY_STRING;
#endif
  AnalogInputConfiguration.divider.Ra = 0;
  AnalogInputConfiguration.divider.Rb = 0;

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
  AnalogInputConfiguration.battery.minVoltage =
      AFE_CONFIG_HARDWARE_ANALOG_INPUT_DEFAULT_BATTER_MIN_V;
  AnalogInputConfiguration.battery.maxVoltage =
      AFE_CONFIG_HARDWARE_ANALOG_INPUT_DEFAULT_BATTER_MAX_V;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  AnalogInputConfiguration.battery.domoticz.idx = AFE_DOMOTICZ_DEFAULT_IDX;
#else
  AnalogInputConfiguration.battery.mqtt.topic[0] = AFE_EMPTY_STRING;
#endif
#endif // AFE_CONFIG_FUNCTIONALITY_BATTERYMETER

#ifdef AFE_ESP32
  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_ADCS; i++) {
    sprintf(AnalogInputConfiguration.name, "ADC%d", i);
    saveConfiguration(i, &AnalogInputConfiguration);
  }
#else
  saveConfiguration(&AnalogInputConfiguration);
#endif
}
#endif //  AFE_CONFIG_HARDWARE_ANALOG_INPUT

#ifdef AFE_CONFIG_HARDWARE_DS18B20
void AFEDataAccess::getConfiguration(uint8_t id, DS18B20 *configuration) {

  char fileName[22];
  sprintf(fileName, AFE_FILE_DS18B20_SENSOR_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "r");
#else
  File configFile = SPIFFS.open(fileName, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_DS18B20> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      sprintf(configuration->name, root["name"]);
      configuration->gpio = root["gpio"];
      configuration->correction = root["correction"];
      configuration->interval = root["interval"];
      configuration->unit = root["unit"];
      configuration->sendOnlyChanges = root["sendOnlyChanges"];
      configuration->resolution =
          root["resolution"] | AFE_CONFIG_HARDWARE_DS18B20_DEFAULT_RESOLUTION;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
      configuration->domoticz.idx = root["idx"] | AFE_DOMOTICZ_DEFAULT_IDX;
#else
      sprintf(configuration->mqtt.topic, root["mqttTopic"] | "");
#endif

      for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_DS18B20_ADDRESS_LENGTH; i++) {
        configuration->address[i] = root["address"][i].as<int>();
      }

#ifdef DEBUG
      printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_DS18B20, jsonBuffer.size());
#endif
    }
#ifdef DEBUG
    else {
      Serial << F("ERROR: JSON not pharsed");
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else {
    Serial << endl
           << F("ERROR: Configuration file: ") << fileName << F(" not opened");
  }
#endif
}
void AFEDataAccess::saveConfiguration(uint8_t id, DS18B20 *configuration) {

  char fileName[22];
  sprintf(fileName, AFE_FILE_DS18B20_SENSOR_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "w");
#else
  File configFile = SPIFFS.open(fileName, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_DS18B20> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    JsonArray &address = jsonBuffer.createArray();
    root["name"] = configuration->name;
    address.copyFrom(configuration->address);
    root["address"] = address;
    root["gpio"] = configuration->gpio;
    root["correction"] = configuration->correction;
    root["interval"] = configuration->interval;
    root["unit"] = configuration->unit;
    root["sendOnlyChanges"] = configuration->sendOnlyChanges;
    root["resolution"] = configuration->resolution;

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    root["idx"] = configuration->domoticz.idx;
#else
    root["mqttTopic"] = configuration->mqtt.topic;
#endif
    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_DS18B20, jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}

void AFEDataAccess::createDS18B20SensorConfigurationFile(void) {
  DS18B20 configuration;

  configuration.gpio = AFE_CONFIG_HARDWARE_DS18B20_DEFAULT_GPIO;
  configuration.correction =
      AFE_CONFIG_HARDWARE_DS18B20_DEFAULT_TEMPERATURE_CORRECTION;
  configuration.interval = AFE_CONFIG_HARDWARE_DS18B20_DEFAULT_INTERVAL;
  DeviceAddress _address = {0, 0, 0, 0, 0, 0, 0, 0};
  memcpy(&configuration.address[0], _address, sizeof(_address[0]) * 8);
  configuration.unit = AFE_TEMPERATURE_UNIT_CELSIUS;
  configuration.sendOnlyChanges =
      AFE_CONFIG_HARDWARE_DS18B20_DEFAULT_SENDING_ONLY_CHANGES;
  configuration.resolution = AFE_CONFIG_HARDWARE_DS18B20_DEFAULT_RESOLUTION;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  configuration.domoticz.idx = AFE_DOMOTICZ_DEFAULT_IDX;
#else
  configuration.mqtt.topic[0] = AFE_EMPTY_STRING;
#endif

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_DS18B20; i++) {
#ifdef DEBUG
    Serial << endl
           << F("INFO: Creating file: /cfg-ds18b20-") << i << F(".json");
#endif
    sprintf(configuration.name, "DS18B20-%d", i + 1);
    saveConfiguration(i, &configuration);
  }
}

#endif // AFE_CONFIG_HARDWARE_DS18B20

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
void AFEDataAccess::getConfiguration(uint8_t id, CONTACTRON *configuration) {
  char fileName[24];
  sprintf(fileName, AFE_FILE_CONTACTRON_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "r");
#else
  File configFile = SPIFFS.open(fileName, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_CONTACTRON> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif
      configuration->gpio = root["gpio"];
      sprintf(configuration->name, root["name"]);
      configuration->type = root["type"];
      configuration->bouncing = root["bouncing"];
#ifdef AFE_CONFIG_HARDWARE_LED
      configuration->ledID = root["ledID"];
#endif
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
      configuration->domoticz.idx = root["idx"] | AFE_DOMOTICZ_DEFAULT_IDX;
#else
      sprintf(configuration->mqtt.topic, root["MQTTTopic"] | "");
#endif

#ifdef DEBUG
      printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_CONTACTRON, jsonBuffer.size());
#endif
    }
#ifdef DEBUG
    else {
      Serial << F("ERROR: JSON not pharsed");
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else {
    Serial << endl
           << F("ERROR: Configuration file: ") << fileName << F(" not opened");
  }
#endif
}

void AFEDataAccess::saveConfiguration(uint8_t id, CONTACTRON *configuration) {
  char fileName[24];
  sprintf(fileName, AFE_FILE_CONTACTRON_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "w");
#else
  File configFile = SPIFFS.open(fileName, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_CONTACTRON> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["gpio"] = configuration->gpio;
    root["name"] = configuration->name;
    root["type"] = configuration->type;
    root["bouncing"] = configuration->bouncing;
#ifdef AFE_CONFIG_HARDWARE_LED
    root["ledID"] = configuration->ledID;
#endif
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    root["idx"] = configuration->domoticz.idx;
#else
    root["MQTTTopic"] = configuration->mqtt.topic;
#endif

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_CONTACTRON, jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}

void AFEDataAccess::createContractonConfigurationFile() {
  CONTACTRON ContactronConfiguration;
  uint8_t index = 0;
  ContactronConfiguration.bouncing =
      AFE_CONFIG_HARDWARE_CONTACTRON_DEFAULT_BOUNCING;
  ContactronConfiguration.type =
      AFE_CONFIG_HARDWARE_CONTACTRON_DEFAULT_OUTPUT_TYPE;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  ContactronConfiguration.domoticz.idx = AFE_DOMOTICZ_DEFAULT_IDX;
#else
  ContactronConfiguration.mqtt.topic[0] = AFE_EMPTY_STRING;
#endif
#ifdef AFE_CONFIG_HARDWARE_LED
  ContactronConfiguration.ledID = AFE_HARDWARE_ITEM_NOT_EXIST;
#endif

#if defined(AFE_DEVICE_iECS_GATE_DRIVERv2) ||                                  \
    defined(AFE_DEVICE_iECS_GATE_DRIVERv3)
  ContactronConfiguration.gpio = AFE_CONFIG_HARDWARE_CONTACTRON_1_DEFAULT_GPIO;
  sprintf(ContactronConfiguration.name, "C1");
  saveConfiguration(0, &ContactronConfiguration);
  ContactronConfiguration.gpio = AFE_CONFIG_HARDWARE_CONTACTRON_2_DEFAULT_GPIO;
  sprintf(ContactronConfiguration.name, "C2");
  saveConfiguration(1, &ContactronConfiguration);
  ContactronConfiguration.gpio = AFE_CONFIG_HARDWARE_CONTACTRON_3_DEFAULT_GPIO;
  sprintf(ContactronConfiguration.name, "C3");
  saveConfiguration(2, &ContactronConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_CONTACTRONS;

#ifdef AFE_DEVICE_iECS_GATE_DRIVERv3
  ContactronConfiguration.gpio = AFE_CONFIG_HARDWARE_CONTACTRON_4_DEFAULT_GPIO;
  sprintf(ContactronConfiguration.name, "C4");
  saveConfiguration(2, &ContactronConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_CONTACTRONS;
#endif // V3
#endif // iECS

#if defined(AFE_DEVICE_SHELLY_1)
  ContactronConfiguration.gpio = AFE_CONFIG_HARDWARE_CONTACTRON_X_DEFAULT_GPIO;
  sprintf(ContactronConfiguration.name, "C1");
  saveConfiguration(0, &ContactronConfiguration);
  //  so not to create additional for Shelly-1
  index = AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_CONTACTRONS;
#endif // Shelly-1

  ContactronConfiguration.gpio = AFE_CONFIG_HARDWARE_CONTACTRON_X_DEFAULT_GPIO;
  for (uint8_t i = index; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_CONTACTRONS;
       i++) {
#ifdef DEBUG
    Serial << endl
           << F("INFO: Creating file: cfg-contactron-") << i << F(".json");
#endif
    sprintf(ContactronConfiguration.name, "C%d", i + 1);
    saveConfiguration(i, &ContactronConfiguration);
  }
}
#endif // AFE_CONFIG_HARDWARE_CONTACTRON

#ifdef AFE_CONFIG_HARDWARE_GATE

void AFEDataAccess::getConfiguration(uint8_t id, GATE *configuration) {
  char fileName[18];
  sprintf(fileName, AFE_FILE_GATE_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "r");
#else
  File configFile = SPIFFS.open(fileName, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_GATE> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      configuration->relayId = root["relayId"];
      sprintf(configuration->name, root["name"]);
      configuration->contactron.id[0] = root["contactrons"][0];
      configuration->contactron.id[1] = root["contactrons"][1];

      for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_CONTACTRONS;
           i++) {
        configuration->states.state[i] = root["states"][i];
      }
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
      configuration->domoticz.idx = root["idx"] | AFE_DOMOTICZ_DEFAULT_IDX;
      configuration->domoticzControl.idx =
          root["idxControl"] | AFE_DOMOTICZ_DEFAULT_IDX;
#else
      sprintf(configuration->mqtt.topic, root["MQTTTopic"] | "");
#endif

#ifdef DEBUG
      printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_GATE, jsonBuffer.size());
#endif
    }
#ifdef DEBUG
    else {
      Serial << F("ERROR: JSON not pharsed");
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else {
    Serial << endl
           << F("ERROR: Configuration file: ") << fileName << F(" not opened");
  }
#endif
}
void AFEDataAccess::saveConfiguration(uint8_t id, GATE *configuration) {

  char fileName[18];
  sprintf(fileName, AFE_FILE_GATE_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "w");
#else
  File configFile = SPIFFS.open(fileName, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_GATE> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    JsonArray &jsonContactron = root.createNestedArray("contactrons");
    JsonArray &jsonStates = root.createNestedArray("states");
    root["relayId"] = configuration->relayId;
    root["name"] = configuration->name;

    /* Contractors selection validaton */
    if ((configuration->contactron.id[0] == AFE_HARDWARE_ITEM_NOT_EXIST &&
         configuration->contactron.id[1] != AFE_HARDWARE_ITEM_NOT_EXIST) ||
        (configuration->contactron.id[0] != AFE_HARDWARE_ITEM_NOT_EXIST &&
         configuration->contactron.id[0] == configuration->contactron.id[1])) {
      configuration->contactron.id[0] = configuration->contactron.id[1];
      configuration->contactron.id[1] = AFE_HARDWARE_ITEM_NOT_EXIST;
    }

    jsonContactron.add(configuration->contactron.id[0]);
    jsonContactron.add(configuration->contactron.id[1]);

    for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_CONTACTRONS;
         i++) {
      jsonStates.add(configuration->states.state[i]);
    }

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    root["idx"] = configuration->domoticz.idx;
    root["idxControl"] = configuration->domoticzControl.idx;
#else
    root["MQTTTopic"] = configuration->mqtt.topic;
#endif

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_GATE, jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}
void AFEDataAccess::createGateConfigurationFile() {

  GATE GateConfiguration;

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  GateConfiguration.domoticz.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  GateConfiguration.domoticzControl.idx = AFE_DOMOTICZ_DEFAULT_IDX;
#else
  GateConfiguration.mqtt.topic[0] = AFE_EMPTY_STRING;
#endif

  GateConfiguration.contactron.id[1] = AFE_HARDWARE_ITEM_NOT_EXIST;

#if defined(AFE_DEVICE_iECS_GATE_DRIVERv2)
  GateConfiguration.contactron.id[0] = 0;
  GateConfiguration.relayId = 0;
  GateConfiguration.states.state[0] = AFE_GATE_OPEN;
  GateConfiguration.states.state[1] = AFE_GATE_UNKNOWN;
  GateConfiguration.states.state[2] = AFE_GATE_UNKNOWN;
  GateConfiguration.states.state[3] = AFE_GATE_CLOSED;
  sprintf(GateConfiguration.name, "G1");
  saveConfiguration(0, &GateConfiguration);
  saveGateState(0, AFE_GATE_UNKNOWN);
#endif

  GateConfiguration.contactron.id[0] = AFE_HARDWARE_ITEM_NOT_EXIST;
  GateConfiguration.relayId = AFE_HARDWARE_ITEM_NOT_EXIST;
  GateConfiguration.states.state[0] = AFE_GATE_UNKNOWN;
  GateConfiguration.states.state[1] = AFE_GATE_UNKNOWN;
  GateConfiguration.states.state[2] = AFE_GATE_UNKNOWN;
  GateConfiguration.states.state[3] = AFE_GATE_UNKNOWN;

  for (uint8_t i = AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_GATES;
       i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_GATES; i++) {
#ifdef DEBUG
    Serial << endl << F("INFO: Creating file: cfg-gate-") << i << F(".json");
#endif
    sprintf(GateConfiguration.name, "G%d", i + 1);
    saveConfiguration(i, &GateConfiguration);
    saveGateState(i, AFE_GATE_UNKNOWN);
  }
}

uint8_t AFEDataAccess::getGateState(uint8_t id) {
  uint8_t state = AFE_GATE_CLOSED;

  char fileName[24];
  sprintf(fileName, AFE_FILE_GATE_STATE_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "r");
#else
  File configFile = SPIFFS.open(fileName, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_GATE_STATE> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif
      state = root["state"];

#ifdef DEBUG
      printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_GATE_STATE, jsonBuffer.size());
#endif
    }
#ifdef DEBUG
    else {
      Serial << F("ERROR: JSON not pharsed");
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else {
    Serial << endl
           << F("ERROR: Configuration file: ") << fileName << F(" not opened");
  }
#endif

  return state;
}
void AFEDataAccess::saveGateState(uint8_t id, uint8_t state) {

  char fileName[24];
  sprintf(fileName, AFE_FILE_GATE_STATE_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "w");
#else
  File configFile = SPIFFS.open(fileName, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    String fileContent;

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_GATE_STATE> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();

    root["state"] = state;

    root.printTo(configFile);

#ifdef DEBUG
    root.printTo(Serial);
#endif

    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_GATE_STATE, jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}
#endif // AFE_CONFIG_HARDWARE_GATE

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
void AFEDataAccess::getConfiguration(uint8_t id, REGULATOR *configuration) {

  char fileName[24];
  sprintf(fileName, AFE_FILE_REGULATOR_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "r");
#else
  File configFile = SPIFFS.open(fileName, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_REGULATOR> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif
      sprintf(configuration->name, root["name"]);
      configuration->enabled =
          root["enabled"] | AFE_FUNCTIONALITY_REGULATOR_DEFAULT_ENABLED;
      configuration->sensorId = root["sensorId"] | AFE_HARDWARE_ITEM_NOT_EXIST;
      configuration->sensorHardware =
          root["sensorHardware"] | AFE_HARDWARE_ITEM_NOT_EXIST;
      configuration->relayId = root["relayId"] | AFE_HARDWARE_ITEM_NOT_EXIST;
      configuration->turnOn = root["turnOn"];
      configuration->turnOff = root["turnOff"];
      configuration->turnOnAbove =
          root["turnOnAbove"] |
          AFE_FUNCTIONALITY_REGULATOR_DEFAULT_ON_INDICATOR;
      configuration->turnOffAbove =
          root["turnOffAbove"] |
          AFE_FUNCTIONALITY_REGULATOR_DEFAULT_OFF_INDICATOR;

#ifdef AFE_CONFIG_HARDWARE_DHT
      configuration->controllingParameter =
          root["controllingParameter"] | AFE_HARDWARE_ITEM_NOT_EXIST;
#endif

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
      configuration->domoticz.idx = root["idx"] | AFE_DOMOTICZ_DEFAULT_IDX;
#else
      sprintf(configuration->mqtt.topic, root["MQTTTopic"] | "");
#endif

#ifdef DEBUG
      printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_REGULATOR, jsonBuffer.size());
#endif
    }
#ifdef DEBUG
    else {
      Serial << F("ERROR: JSON not pharsed");
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else {
    Serial << endl
           << F("ERROR: Configuration file: ") << fileName << F(" not opened");
  }
#endif
}

void AFEDataAccess::saveConfiguration(uint8_t id, REGULATOR *configuration) {
  char fileName[24];
  sprintf(fileName, AFE_FILE_REGULATOR_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "w");
#else
  File configFile = SPIFFS.open(fileName, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_REGULATOR> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["name"] = configuration->name;
    root["enabled"] = configuration->enabled;
    root["relayId"] = configuration->relayId;
    root["sensorId"] = configuration->sensorId;
    root["sensorHardware"] = configuration->sensorHardware;
    root["turnOn"] = configuration->turnOn;
    root["turnOff"] = configuration->turnOff;
    root["turnOnAbove"] = configuration->turnOnAbove;
    root["turnOffAbove"] = configuration->turnOffAbove;

#ifdef AFE_CONFIG_HARDWARE_DHT
    root["controllingParameter"] = configuration->controllingParameter;
#endif

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    root["idx"] = configuration->domoticz.idx;
#else
    root["MQTTTopic"] = configuration->mqtt.topic;
#endif

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_REGULATOR, jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}

void AFEDataAccess::createRegulatorConfigurationFile(void) {

  REGULATOR configuration;
  configuration.enabled = AFE_FUNCTIONALITY_REGULATOR_DEFAULT_ENABLED;
  configuration.relayId = AFE_HARDWARE_ITEM_NOT_EXIST;
  configuration.sensorId = AFE_HARDWARE_ITEM_NOT_EXIST;
  configuration.sensorHardware = AFE_HARDWARE_ITEM_NOT_EXIST;
  configuration.turnOff = AFE_FUNCTIONALITY_REGULATOR_DEFAULT_OFF;
  configuration.turnOn = AFE_FUNCTIONALITY_REGULATOR_DEFAULT_ON;
  configuration.turnOffAbove =
      AFE_FUNCTIONALITY_REGULATOR_DEFAULT_OFF_INDICATOR;
  configuration.turnOnAbove = AFE_FUNCTIONALITY_REGULATOR_DEFAULT_ON_INDICATOR;

#ifdef AFE_CONFIG_HARDWARE_DHT
  configuration.controllingParameter = AFE_HARDWARE_ITEM_NOT_EXIST;
#endif

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  configuration.domoticz.idx = AFE_DOMOTICZ_DEFAULT_IDX;
#else
  configuration.mqtt.topic[0] = AFE_EMPTY_STRING;
#endif

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_REGULATORS; i++) {
    sprintf(configuration.name, "regulator-%d", i + 1);
#ifdef DEBUG
    Serial << endl << F("INFO: Creating regulator configuration file: ") << i;
#endif
    saveConfiguration(i, &configuration);
  }
}
#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
void AFEDataAccess::getConfiguration(uint8_t id,
                                     THERMAL_PROTECTOR *configuration) {

  char fileName[33];
  sprintf(fileName, AFE_FILE_THERMAL_PROTECTOR_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "r");
#else
  File configFile = SPIFFS.open(fileName, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_THERMAL_PROTECTOR> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif
      sprintf(configuration->name, root["name"] | "");
      configuration->enabled =
          root["enabled"] | AFE_FUNCTIONALITY_THERMAL_PROTECTOR_DEFAULT_ENABLED;
      configuration->sensorId = root["sensorId"] | AFE_HARDWARE_ITEM_NOT_EXIST;
      configuration->sensorHardware =
          root["sensorHardware"] | AFE_HARDWARE_ITEM_NOT_EXIST;
      configuration->relayId = root["relayId"] | AFE_HARDWARE_ITEM_NOT_EXIST;
      configuration->temperature =
          root["temperature"] |
          AFE_FUNCTIONALITY_THERMAL_PROTECTOR_DEFAULT_TEMPERATURE;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
      configuration->domoticz.idx = root["idx"] | AFE_DOMOTICZ_DEFAULT_IDX;
#else
      sprintf(configuration->mqtt.topic, root["MQTTTopic"] | "");
#endif

#ifdef DEBUG
      printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_THERMAL_PROTECTOR,
                          jsonBuffer.size());
#endif
    }
#ifdef DEBUG
    else {
      Serial << F("ERROR: JSON not pharsed");
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else {
    Serial << endl
           << F("ERROR: Configuration file: ") << fileName << F(" not opened");
  }
#endif
}

void AFEDataAccess::saveConfiguration(uint8_t id,
                                      THERMAL_PROTECTOR *configuration) {
  char fileName[33];
  sprintf(fileName, AFE_FILE_THERMAL_PROTECTOR_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "w");
#else
  File configFile = SPIFFS.open(fileName, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_THERMAL_PROTECTOR> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["name"] = configuration->name;
    root["enabled"] = configuration->enabled;
    root["relayId"] = configuration->relayId;
    root["sensorId"] = configuration->sensorId;
    root["sensorHardware"] = configuration->sensorHardware;
    root["temperature"] = configuration->temperature;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    root["idx"] = configuration->domoticz.idx;
#else
    root["MQTTTopic"] = configuration->mqtt.topic;
#endif
    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_THERMAL_PROTECTOR,
                        jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}
void AFEDataAccess::createThermalProtectorConfigurationFile(void) {
  THERMAL_PROTECTOR configuration;
  configuration.enabled = AFE_FUNCTIONALITY_THERMAL_PROTECTOR_DEFAULT_ENABLED;
  configuration.relayId = AFE_HARDWARE_ITEM_NOT_EXIST;
  configuration.sensorId = AFE_HARDWARE_ITEM_NOT_EXIST;
  configuration.sensorHardware = AFE_HARDWARE_ITEM_NOT_EXIST;
  configuration.temperature =
      AFE_FUNCTIONALITY_THERMAL_PROTECTOR_DEFAULT_TEMPERATURE;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  configuration.domoticz.idx = AFE_DOMOTICZ_DEFAULT_IDX;
#else
  configuration.mqtt.topic[0] = AFE_EMPTY_STRING;
#endif

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_THERMAL_PROTECTOR;
       i++) {
    sprintf(configuration.name, "protector-%d", i + 1);
#ifdef DEBUG
    Serial << endl
           << F("INFO: Creating thermal protection configuration file: ") << i;
#endif
    saveConfiguration(i, &configuration);
  }
}
#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR

#ifdef AFE_CONFIG_FUNCTIONALITY_API_CONTROL
void AFEDataAccess::saveAPI(uint8_t apiID, boolean state) {
  DEVICE configuration;
  getConfiguration(&configuration);
  if (apiID == API_HTTP) {
    configuration.api.http = state;
  } else if (apiID == API_MQTT) {
    configuration.api.mqtt = state;
  } else if (apiID == API_HTTP_DOMOTICZ(NOT SET)) {
    configuration.api.domoticz = state;
    if (state) {
      configuration.api.http = true;
    }
  }
  saveConfiguration(configuration);
}
#endif // AFE_CONFIG_FUNCTIONALITY_API_CONTROL

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
void AFEDataAccess::getConfiguration(uint8_t id, HPMA115S0 *configuration) {
  char fileName[23];
  sprintf(fileName, AFE_FILE_HPMA114S0_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "r");
#else
  File configFile = SPIFFS.open(fileName, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_HPMA115S0> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif
      configuration->interval = root["interval"];
      configuration->timeToMeasure = root["timeToMeasure"];
      configuration->whoPM10Norm =
          root["whoPM10Norm"] |
          AFE_CONFIG_HARDWARE_HPMA115S_DEFAULT_WHO_NORM_PM10;
      configuration->whoPM25Norm =
          root["whoPM25Norm"] |
          AFE_CONFIG_HARDWARE_HPMA115S_DEFAULT_WHO_NORM_PM25;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
      configuration->domoticz.pm25.idx =
          root["idx"]["pm25"] | AFE_DOMOTICZ_DEFAULT_IDX;
      configuration->domoticz.pm10.idx =
          root["idx"]["pm10"] | AFE_DOMOTICZ_DEFAULT_IDX;
      configuration->domoticz.whoPM25Norm.idx =
          root["idx"]["whoPM25Norm"] | AFE_DOMOTICZ_DEFAULT_IDX;
      configuration->domoticz.whoPM10Norm.idx =
          root["idx"]["whoPM10Norm"] | AFE_DOMOTICZ_DEFAULT_IDX;
#else
      sprintf(configuration->mqtt.topic, root["mqttTopic"] | "");
#endif
      sprintf(configuration->name, root["name"]);

#ifdef DEBUG
      printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_HPMA115S0, jsonBuffer.size());
#endif
    }
#ifdef DEBUG
    else {
      Serial << F("ERROR: JSON not pharsed");
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else {
    Serial << endl
           << F("ERROR: Configuration file: ") << fileName << F(" not opened");
  }
#endif
}
void AFEDataAccess::saveConfiguration(uint8_t id, HPMA115S0 *configuration) {
  char fileName[23];
  sprintf(fileName, AFE_FILE_HPMA114S0_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "w");
#else
  File configFile = SPIFFS.open(fileName, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_HPMA115S0> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    JsonObject &idx = root.createNestedObject("idx");
#endif

    root["interval"] = configuration->interval;
    root["timeToMeasure"] = configuration->timeToMeasure;
    root["name"] = configuration->name;
    root["whoPM10Norm"] = configuration->whoPM10Norm;
    root["whoPM25Norm"] = configuration->whoPM25Norm;

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    idx["pm25"] = configuration->domoticz.pm25.idx;
    idx["pm10"] = configuration->domoticz.pm10.idx;
    idx["whoPM10Norm"] = configuration->domoticz.whoPM10Norm.idx;
    idx["whoPM25Norm"] = configuration->domoticz.whoPM25Norm.idx;
#else
    root["mqttTopic"] = configuration->mqtt.topic;
#endif

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_HPMA115S0, jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}
void AFEDataAccess::createHPMA115S0SensorConfigurationFile() {

  HPMA115S0 configuration;

  configuration.interval = AFE_CONFIG_HARDWARE_HPMA115S_DEFAULT_INTERVAL;
  configuration.timeToMeasure =
      AFE_CONFIG_HARDWARE_HPMA115S_DEFAULT_TIME_TO_MEASURE;
  configuration.whoPM10Norm =
      AFE_CONFIG_HARDWARE_HPMA115S_DEFAULT_WHO_NORM_PM10;
  configuration.whoPM25Norm =
      AFE_CONFIG_HARDWARE_HPMA115S_DEFAULT_WHO_NORM_PM25;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  configuration.domoticz.pm25.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  configuration.domoticz.pm10.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  configuration.domoticz.whoPM10Norm.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  configuration.domoticz.whoPM25Norm.idx = AFE_DOMOTICZ_DEFAULT_IDX;
#else
  configuration.mqtt.topic[0] = AFE_EMPTY_STRING;
#endif
  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_HPMA115S0; i++) {
#ifdef DEBUG
    Serial << endl
           << F("INFO: Creating file: cfg-hpma115s0-") << i << F(".json");
#endif
    sprintf(configuration.name, "HPMA115S0-%d", i + 1);
    saveConfiguration(i, &configuration);
  }
}
#endif // AFE_CONFIG_HARDWARE_HPMA115S0

#ifdef AFE_CONFIG_HARDWARE_UART
void AFEDataAccess::getConfiguration(SERIALPORT *configuration) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << F(AFE_FILE_UART_CONFIGURATION)
         << F(" ... ");
#endif
#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(AFE_FILE_UART_CONFIGURATION, "r");
#else
  File configFile = SPIFFS.open(AFE_FILE_UART_CONFIGURATION, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_UART> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      configuration->RXD = root[F("RXD")];
      configuration->TXD = root[F("TXD")];

#ifdef DEBUG
      printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_UART, jsonBuffer.size());
#endif
    }
#ifdef DEBUG
    else {
      Serial << F("ERROR: JSON not pharsed");
    }
#endif

    configFile.close();
  }
#ifdef DEBUG
  else {
    Serial << endl
           << F("ERROR: Configuration file: ") << F(AFE_FILE_UART_CONFIGURATION)
           << F(" not opened");
  }
#endif
}
void AFEDataAccess::saveConfiguration(SERIALPORT *configuration) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << F(AFE_FILE_UART_CONFIGURATION)
         << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(AFE_FILE_UART_CONFIGURATION, "w");
#else
  File configFile = SPIFFS.open(AFE_FILE_UART_CONFIGURATION, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_UART> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root[F("RXD")] = configuration->RXD;
    root[F("TXD")] = configuration->TXD;
    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();
#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_UART, jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}
void AFEDataAccess::createSerialConfigurationFile() {
#ifdef DEBUG
  Serial << endl
         << F("INFO: Creating file: ") << F(AFE_FILE_UART_CONFIGURATION);
#endif
  SERIALPORT configuration;
  configuration.RXD = AFE_CONFIG_HARDWARE_UART_DEFAULT_RXD;
  configuration.TXD = AFE_CONFIG_HARDWARE_UART_DEFAULT_TXD;
  saveConfiguration(&configuration);
}
#endif // AFE_CONFIG_HARDWARE_UART

#ifdef AFE_CONFIG_HARDWARE_I2C
#if AFE_ESP32
void AFEDataAccess::getConfiguration(uint8_t id, I2CPORT *configuration)
#else
void AFEDataAccess::getConfiguration(I2CPORT *configuration)
#endif
{
  char fileName[17];

#ifdef AFE_ESP32
  sprintf(fileName, AFE_FILE_I2C_CONFIGURATION, id);
#else
  sprintf(fileName, AFE_FILE_I2C_CONFIGURATION);
#endif

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif
#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "r");
#else
  File configFile = SPIFFS.open(fileName, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_I2C> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      configuration->SDA = root["SDA"];
      configuration->SCL = root["SCL"];

#ifdef AFE_ESP32
      configuration->frequency = root["frequency"];
#endif

#ifdef DEBUG
      printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_I2C, jsonBuffer.size());
#endif
    }
#ifdef DEBUG
    else {
      Serial << F("ERROR: JSON not pharsed");
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else {
    Serial << endl
           << F("ERROR: Configuration file: ") << F(AFE_FILE_I2C_CONFIGURATION)
           << F(" not opened");
  }
#endif
}
#ifdef AFE_ESP32
void AFEDataAccess::saveConfiguration(uint8_t id, I2CPORT *configuration)
#else
void AFEDataAccess::saveConfiguration(I2CPORT *configuration)
#endif
{
  char fileName[17];

#ifdef AFE_ESP32
  sprintf(fileName, AFE_FILE_I2C_CONFIGURATION, id);
#else
  sprintf(fileName, AFE_FILE_I2C_CONFIGURATION);
#endif

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "w");
#else
  File configFile = SPIFFS.open(fileName, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_I2C> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["SDA"] = configuration->SDA;
    root["SCL"] = configuration->SCL;
#ifdef AFE_ESP32
    root["frequency"] = configuration->frequency;
#endif

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_I2C, jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}
void AFEDataAccess::createI2CConfigurationFile() {
#ifdef DEBUG
  Serial << endl << F("INFO: Creating I2C confifuration file: ");
#endif
  I2CPORT configuration;
#ifdef AFE_ESP32
  configuration.SDA = AFE_CONFIG_HARDWARE_I2C_0_DEFAULT_SDA;
  configuration.SCL = AFE_CONFIG_HARDWARE_I2C_0_DEFAULT_SCL;
  configuration.frequency = AFE_CONFIG_HARDWARE_I2C_DEFAULT_FREQUENCY;
  saveConfiguration(0, &configuration);
  configuration.SDA = AFE_CONFIG_HARDWARE_I2C_1_DEFAULT_SDA;
  configuration.SCL = AFE_CONFIG_HARDWARE_I2C_1_DEFAULT_SCL;
  saveConfiguration(1, &configuration);
#else
  configuration.SDA = AFE_CONFIG_HARDWARE_I2C_0_DEFAULT_SDA;
  configuration.SCL = AFE_CONFIG_HARDWARE_I2C_0_DEFAULT_SCL;
  saveConfiguration(&configuration);
#endif
}
#endif // AFE_CONFIG_HARDWARE_I2C

#ifdef AFE_CONFIG_HARDWARE_BMEX80
void AFEDataAccess::getConfiguration(uint8_t id, BMEX80 *configuration) {
  char fileName[20];
  sprintf(fileName, AFE_FILE_BMX680_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "r");
#else
  File configFile = SPIFFS.open(fileName, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_BMEX80> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      configuration->type = root["type"];
#ifdef AFE_ESP32
      configuration->wirePortId = root["wirePortId"];
#endif
      configuration->i2cAddress = root["i2cAddress"];
      sprintf(configuration->name, root["name"]);
      configuration->interval = root["interval"];
      configuration->resolution = root["resolution"];
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
      configuration->domoticz.temperatureHumidityPressure.idx =
          root["idx"]["temperatureHumidityPressure"] | AFE_DOMOTICZ_DEFAULT_IDX;
      configuration->domoticz.temperatureHumidity.idx =
          root["idx"]["temperatureHumidity"] | AFE_DOMOTICZ_DEFAULT_IDX;
      configuration->domoticz.gasResistance.idx =
          root["idx"]["gasResistance"] | AFE_DOMOTICZ_DEFAULT_IDX;
      configuration->domoticz.temperature.idx =
          root["idx"]["temperature"] | AFE_DOMOTICZ_DEFAULT_IDX;
      configuration->domoticz.humidity.idx =
          root["idx"]["humidity"] | AFE_DOMOTICZ_DEFAULT_IDX;
      configuration->domoticz.pressure.idx =
          root["idx"]["pressure"] | AFE_DOMOTICZ_DEFAULT_IDX;
      configuration->domoticz.relativePressure.idx =
          root["idx"]["relativePressure"] | AFE_DOMOTICZ_DEFAULT_IDX;
      configuration->domoticz.dewPoint.idx =
          root["idx"]["dewPoint"] | AFE_DOMOTICZ_DEFAULT_IDX;
      configuration->domoticz.heatIndex.idx =
          root["idx"]["heatIndex"] | AFE_DOMOTICZ_DEFAULT_IDX;
      configuration->domoticz.iaq.idx =
          root["idx"]["iaq"] | AFE_DOMOTICZ_DEFAULT_IDX;
      configuration->domoticz.staticIaq.idx =
          root["idx"]["staticIaq"] | AFE_DOMOTICZ_DEFAULT_IDX;
      configuration->domoticz.co2Equivalent.idx =
          root["idx"]["co2Equivalent"] | AFE_DOMOTICZ_DEFAULT_IDX;
      configuration->domoticz.breathVocEquivalent.idx =
          root["idx"]["breathVocEquivalent"] | AFE_DOMOTICZ_DEFAULT_IDX;
      configuration->domoticz.absoluteHumidity.idx =
          root["idx"]["absoluteHumidity"] | AFE_DOMOTICZ_DEFAULT_IDX;
      configuration->domoticz.comfort.idx =
          root["idx"]["comfort"] | AFE_DOMOTICZ_DEFAULT_IDX;
      configuration->domoticz.perception.idx =
          root["idx"]["perception"] | AFE_DOMOTICZ_DEFAULT_IDX;
#else
      sprintf(configuration->mqtt.topic, root["mqttTopic"] | "");
#endif
      configuration->temperature.unit = root["temperature"]["unit"];
      configuration->temperature.correction = root["temperature"]["correction"];
      configuration->humidity.correction = root["humidity"]["correction"];
      configuration->pressure.unit = root["pressure"]["unit"];
      configuration->pressure.correction = root["pressure"]["correction"];
      configuration->seaLevelPressure = root["pressure"]["seaLevelPressure"];
      configuration->altitude = root["pressure"]["altitude"];

#ifdef DEBUG
      printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_BMEX80, jsonBuffer.size());
#endif
    }
#ifdef DEBUG
    else {
      Serial << F("ERROR: JSON not pharsed");
    }
#endif

    configFile.close();
  }
#ifdef DEBUG
  else {
    Serial << endl
           << F("ERROR: Configuration file: ") << fileName << F(" not opened");
  }
#endif
}
void AFEDataAccess::saveConfiguration(uint8_t id, BMEX80 *configuration) {
  char fileName[20];
  sprintf(fileName, AFE_FILE_BMX680_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "w");
#else
  File configFile = SPIFFS.open(fileName, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_BMEX80> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    JsonObject &idx = root.createNestedObject("idx");
#endif
    JsonObject &temperature = root.createNestedObject("temperature");
    JsonObject &humidity = root.createNestedObject("humidity");
    JsonObject &pressure = root.createNestedObject("pressure");

    root["type"] = configuration->type;
    root["interval"] = configuration->interval;
    root["name"] = configuration->name;
#ifdef AFE_ESP32
    root["wirePortId"] = configuration->wirePortId;
#endif
    root["i2cAddress"] = configuration->i2cAddress;
    root["resolution"] = configuration->resolution;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    idx["temperatureHumidity"] =
        configuration->domoticz.temperatureHumidity.idx;
    idx["temperatureHumidityPressure"] =
        configuration->domoticz.temperatureHumidityPressure.idx;
    idx["gasResistance"] = configuration->domoticz.gasResistance.idx;
    idx["temperature"] = configuration->domoticz.temperature.idx;
    idx["humidity"] = configuration->domoticz.humidity.idx;
    idx["pressure"] = configuration->domoticz.pressure.idx;
    idx["relativePressure"] = configuration->domoticz.relativePressure.idx;
    idx["dewPoint"] = configuration->domoticz.dewPoint.idx;
    idx["heatIndex"] = configuration->domoticz.heatIndex.idx;
    idx["iaq"] = configuration->domoticz.iaq.idx;
    idx["staticIaq"] = configuration->domoticz.staticIaq.idx;
    idx["co2Equivalent"] = configuration->domoticz.co2Equivalent.idx;
    idx["breathVocEquivalent"] =
        configuration->domoticz.breathVocEquivalent.idx;
    idx["perception"] = configuration->domoticz.perception.idx;
    idx["comfort"] = configuration->domoticz.comfort.idx;
    idx["absoluteHumidity"] = configuration->domoticz.absoluteHumidity.idx;
#else
    root["mqttTopic"] = configuration->mqtt.topic;
#endif
    temperature["unit"] = configuration->temperature.unit;
    temperature["correction"] = configuration->temperature.correction;
    humidity["correction"] = configuration->humidity.correction;
    pressure["unit"] = configuration->pressure.unit;
    pressure["correction"] = configuration->pressure.correction;
    pressure["seaLevelPressure"] = configuration->seaLevelPressure;
    pressure["altitude"] = configuration->altitude;

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_BMEX80, jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}
void AFEDataAccess::createBMEX80SensorConfigurationFile() {

  BMEX80 configuration;
  configuration.type = AFE_BMX_UNKNOWN_SENSOR;
  configuration.interval = AFE_CONFIG_HARDWARE_BMEX80_DEFAULT_INTERVAL;
#ifdef AFE_ESP32
  configuration.wirePortId = AFE_HARDWARE_ITEM_NOT_EXIST;
#endif
  configuration.i2cAddress = AFE_CONFIG_HARDWARE_I2C_DEFAULT_NON_EXIST_ADDRESS;
  configuration.resolution = 3; //  BMP085_ULTRAHIGHRES;
  configuration.temperature.unit = AFE_TEMPERATURE_UNIT_CELSIUS;
  configuration.temperature.correction = 0;
  configuration.humidity.correction = 0;
  configuration.pressure.unit = AFE_PRESSURE_UNIT_HPA;
  configuration.pressure.correction = 0;
  configuration.altitude = 0;
  configuration.seaLevelPressure = AFE_CONFIG_DEFAULT_SEA_LEVEL_PRESSURE;

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  configuration.domoticz.temperatureHumidityPressure.idx =
      AFE_DOMOTICZ_DEFAULT_IDX;
  configuration.domoticz.temperatureHumidity.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  configuration.domoticz.gasResistance.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  configuration.domoticz.temperature.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  configuration.domoticz.humidity.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  configuration.domoticz.pressure.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  configuration.domoticz.relativePressure.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  configuration.domoticz.dewPoint.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  configuration.domoticz.heatIndex.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  configuration.domoticz.iaq.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  configuration.domoticz.staticIaq.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  configuration.domoticz.co2Equivalent.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  configuration.domoticz.breathVocEquivalent.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  configuration.domoticz.perception.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  configuration.domoticz.comfort.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  configuration.domoticz.absoluteHumidity.idx = AFE_DOMOTICZ_DEFAULT_IDX;
#else
  configuration.mqtt.topic[0] = AFE_EMPTY_STRING;
#endif

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_BMEX80; i++) {
#ifdef DEBUG
    Serial << endl << F("INFO: Creating file: cfg-BMEX80-") << i << F(".json");
#endif

    sprintf(configuration.name, "BMEX80-%d", i + 1);
    saveConfiguration(i, &configuration);
  }
}
#endif // AFE_CONFIG_HARDWARE_BMEX80

#ifdef AFE_CONFIG_HARDWARE_BH1750
boolean AFEDataAccess::getConfiguration(uint8_t id,
                                        BH1750_CONFIG *configuration) {
  boolean _ret = true;
  char fileName[20];
  sprintf(fileName, AFE_FILE_BH1750_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "r");
#else
  File configFile = SPIFFS.open(fileName, "r");
#endif
  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_BH1750> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());

    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      sprintf(configuration->name, root["name"] | "");
      configuration->mode =
          root["mode"] | AFE_CONFIG_HARDWARE_BH1750_DEFAULT_MODE;
      configuration->interval =
          root["interval"] | AFE_CONFIG_HARDWARE_BH1750_DEFAULT_INTERVAL;

#ifdef AFE_ESP32
      configuration->wirePortId =
          root["wirePortId"] | AFE_HARDWARE_ITEM_NOT_EXIST;
#endif

      configuration->i2cAddress =
          root["i2cAddress"] |
          AFE_CONFIG_HARDWARE_I2C_DEFAULT_NON_EXIST_ADDRESS;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
      configuration->domoticz.idx = root["idx"] | AFE_DOMOTICZ_DEFAULT_IDX;
#else
      sprintf(configuration->mqtt.topic, root["mqttTopic"] | "");
#endif

#ifdef DEBUG
      printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_BH1750, jsonBuffer.size());
#endif
    } else {
      _ret = false;
#ifdef DEBUG
      Serial << F("ERROR: JSON not pharsed");
#endif
    }

    configFile.close();
  } else {
    _ret = false;
#ifdef DEBUG
    Serial << endl
           << F("ERROR: Configuration file: ") << fileName << F(" not opened");
#endif
  }

  return _ret;
}
void AFEDataAccess::saveConfiguration(uint8_t id,
                                      BH1750_CONFIG *configuration) {
  char fileName[20];
  sprintf(fileName, AFE_FILE_BH1750_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "w");
#else
  File configFile = SPIFFS.open(fileName, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_BH1750> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();

    root["name"] = configuration->name;
    root["mode"] = configuration->mode;
    root["interval"] = configuration->interval;

#ifdef AFE_ESP32
    root["wirePortId"] = configuration->wirePortId;
#endif

    root["i2cAddress"] = configuration->i2cAddress;

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    root["idx"] = configuration->domoticz.idx;
#else
    root["mqttTopic"] = configuration->mqtt.topic;
#endif

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_BH1750, jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}

void AFEDataAccess::createBH1750SensorConfigurationFile() {
  BH1750_CONFIG configuration;
  configuration.interval = AFE_CONFIG_HARDWARE_BH1750_DEFAULT_INTERVAL;
  configuration.i2cAddress = AFE_CONFIG_HARDWARE_I2C_DEFAULT_NON_EXIST_ADDRESS;

#ifdef AFE_ESP32
  configuration.wirePortId = AFE_HARDWARE_ITEM_NOT_EXIST;
#endif

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  configuration.domoticz.idx = AFE_DOMOTICZ_DEFAULT_IDX;
#else
  configuration.mqtt.topic[0] = AFE_EMPTY_STRING;
#endif
  configuration.mode = AFE_CONFIG_HARDWARE_BH1750_DEFAULT_MODE;
  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_BH1750; i++) {
#ifdef DEBUG
    Serial << endl << F("INFO: Creating file: cfg-bh1750-") << i << F(".json");
#endif

    sprintf(configuration.name, "BH1750-%d", i + 1);
    saveConfiguration(i, &configuration);
  }
}
#endif // AFE_CONFIG_HARDWARE_BH1750

#ifdef AFE_CONFIG_HARDWARE_AS3935
void AFEDataAccess::getConfiguration(uint8_t id, AS3935 *configuration) {
  char fileName[20];
  sprintf(fileName, AFE_FILE_AS3935_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "r");
#else
  File configFile = SPIFFS.open(fileName, "r");
#endif
  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_AS3935> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());

    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      sprintf(configuration->name, root["name"]);
      configuration->i2cAddress = root["i2cAddress"];
      configuration->irqGPIO = root["irqGPIO"];
      configuration->setNoiseFloorAutomatically =
          root["setNoiseFloorAutomatically"];
      configuration->noiseFloor = root["noiseFloor"];
      configuration->watchdogThreshold = root["watchdogThreshold"];
      configuration->minimumNumberOfLightningSpikes =
          root["minimumNumberOfLightningSpikes"];
      configuration->spikesRejectionLevel = root["spikesRejectionLevel"];
      configuration->indoor = root["indoor"];
      configuration->unit = root["unit"];
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
      configuration->domoticz.idx = root["idx"] | AFE_DOMOTICZ_DEFAULT_IDX;
#else
      sprintf(configuration->mqtt.topic, root["mqttTopic"] | "");
#endif

#ifdef DEBUG
      printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_AS3935, jsonBuffer.size());
#endif
    }
#ifdef DEBUG
    else {
      Serial << F("ERROR: JSON not pharsed");
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else {
    Serial << endl
           << F("ERROR: Configuration file: ") << fileName << F(" not opened");
  }
#endif
}

void AFEDataAccess::saveConfiguration(uint8_t id, AS3935 *configuration) {

  char fileName[20];
  sprintf(fileName, AFE_FILE_AS3935_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "w");
#else
  File configFile = SPIFFS.open(fileName, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_AS3935> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();

    root["name"] = configuration->name;
    root["i2cAddress"] = configuration->i2cAddress;
    root["irqGPIO"] = configuration->irqGPIO;
    root["setNoiseFloorAutomatically"] =
        configuration->setNoiseFloorAutomatically;
    root["noiseFloor"] = configuration->noiseFloor;
    root["watchdogThreshold"] = configuration->watchdogThreshold;
    root["minimumNumberOfLightningSpikes"] =
        configuration->minimumNumberOfLightningSpikes;
    root["spikesRejectionLevel"] = configuration->spikesRejectionLevel;
    root["indoor"] = configuration->indoor;
    root["unit"] = configuration->unit;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    root["idx"] = configuration->domoticz.idx;
#else
    root["mqttTopic"] = configuration->mqtt.topic;
#endif
    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_AS3935, jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}
void AFEDataAccess::createAS3935SensorConfigurationFile() {
  AS3935 configuration;

  configuration.irqGPIO = AFE_CONFIG_HARDWARE_AS3935_DEFAULT_GPIO;
  configuration.setNoiseFloorAutomatically = true;
  configuration.noiseFloor = AFE_CONFIG_HARDWARE_AS3935_DEFAULT_NOISE_FLOOR;
  configuration.minimumNumberOfLightningSpikes =
      AFE_CONFIG_HARDWARE_AS3935_DEFAULT_MINIMUM_NO_OF_SPIKES;
  configuration.watchdogThreshold =
      AFE_CONFIG_HARDWARE_AS3935_DEFAULT_WATCHDOG_THRESHOLD;
  configuration.spikesRejectionLevel =
      AFE_CONFIG_HARDWARE_AS3935_DEFAULT_SPIKES_REJECTION_LEVEL;
  configuration.indoor = true;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  configuration.domoticz.idx = AFE_DOMOTICZ_DEFAULT_IDX;
#else
  configuration.mqtt.topic[0] = AFE_EMPTY_STRING;
#endif
  configuration.unit = AFE_DISTANCE_KM;

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_AS3935; i++) {
#ifdef DEBUG
    Serial << endl << F("INFO: Creating file: cfg-as3935-") << i << F(".json");
#endif
    sprintf(configuration.name, "AS3935-%d", i + 1);
    saveConfiguration(i, &configuration);
  }
}
#endif // AFE_CONFIG_HARDWARE_AS3935

#ifdef AFE_CONFIG_HARDWARE_DHT
void AFEDataAccess::getConfiguration(uint8_t id, DHT *configuration) {

  char fileName[17];
  sprintf(fileName, AFE_FILE_DHT_SENSOR_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "r");
#else
  File configFile = SPIFFS.open(fileName, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_DHT> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      sprintf(configuration->name, root["name"]);
      configuration->gpio = root["gpio"];
      configuration->interval =
          root["interval"] | AFE_CONFIG_HARDWARE_DHT_DEFAULT_INTERVAL;
      configuration->sendOnlyChanges =
          root["sendOnlyChanges"] |
          AFE_CONFIG_HARDWARE_DHT_DEFAULT_SENDING_ONLY_CHANGES;

      configuration->type = root["type"];
      configuration->temperature.unit = root["temperature"]["unit"];
      configuration->temperature.correction =
          root["temperature"]["correction"] |
          AFE_CONFIG_HARDWARE_DHT_DEFAULT_TEMPERATURE_CORRECTION;
      configuration->humidity.unit = root["humidity"]["unit"];
      configuration->humidity.correction =
          root["humidity"]["correction"] |
          AFE_CONFIG_HARDWARE_DHT_DEFAULT_HUMIDITY_CORRECTION;

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
      configuration->domoticz.temperatureHumidity.idx =
          root["idx"]["temperatureHumidity"] | AFE_DOMOTICZ_DEFAULT_IDX;

      configuration->domoticz.temperature.idx =
          root["idx"]["temperature"] | AFE_DOMOTICZ_DEFAULT_IDX;

      configuration->domoticz.humidity.idx =
          root["idx"]["humidity"] | AFE_DOMOTICZ_DEFAULT_IDX;

      configuration->domoticz.dewPoint.idx =
          root["idx"]["dewPoint"] | AFE_DOMOTICZ_DEFAULT_IDX;

      configuration->domoticz.heatIndex.idx =
          root["idx"]["heatIndex"] | AFE_DOMOTICZ_DEFAULT_IDX;

      configuration->domoticz.absoluteHumidity.idx =
          root["idx"]["absoluteHumidity"] | AFE_DOMOTICZ_DEFAULT_IDX;

      configuration->domoticz.perception.idx =
          root["idx"]["perception"] | AFE_DOMOTICZ_DEFAULT_IDX;

      configuration->domoticz.comfort.idx =
          root["idx"]["comfort"] | AFE_DOMOTICZ_DEFAULT_IDX;

#else
      sprintf(configuration->mqtt.topic, root["mqttTopic"] | "");
#endif

#ifdef DEBUG
      printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_DHT, jsonBuffer.size());
#endif
    }
#ifdef DEBUG
    else {
      Serial << F("ERROR: JSON not pharsed");
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else {
    Serial << endl
           << F("ERROR: Configuration file: ") << fileName << F(" not opened");
  }
#endif
}
void AFEDataAccess::saveConfiguration(uint8_t id, DHT *configuration) {

  char fileName[17];
  sprintf(fileName, AFE_FILE_DHT_SENSOR_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "w");
#else
  File configFile = SPIFFS.open(fileName, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_DHT> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    JsonObject &temperature = root.createNestedObject("temperature");
    JsonObject &humidity = root.createNestedObject("humidity");
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    JsonObject &domoticz = root.createNestedObject("idx");
#endif
    root["name"] = configuration->name;
    root["gpio"] = configuration->gpio;
    root["type"] = configuration->type;
    root["interval"] = configuration->interval;
    root["sendOnlyChanges"] = configuration->sendOnlyChanges;
    temperature["unit"] = configuration->temperature.unit;
    temperature["correction"] = configuration->temperature.correction;
    humidity["unit"] = configuration->humidity.unit;
    humidity["correction"] = configuration->humidity.correction;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    domoticz["temperature"] = configuration->domoticz.temperature.idx;
    domoticz["humidity"] = configuration->domoticz.humidity.idx;
    domoticz["dewPoint"] = configuration->domoticz.dewPoint.idx;
    domoticz["heatIndex"] = configuration->domoticz.heatIndex.idx;
    domoticz["temperatureHumidity"] =
        configuration->domoticz.temperatureHumidity.idx;
    domoticz["absoluteHumidity"] = configuration->domoticz.absoluteHumidity.idx;
    domoticz["perception"] = configuration->domoticz.perception.idx;
    domoticz["comfort"] = configuration->domoticz.comfort.idx;
#else
    root["mqttTopic"] = configuration->mqtt.topic;
#endif
    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_DHT, jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}

void AFEDataAccess::createDHTSensorConfigurationFile(void) {
  DHT configuration;

  configuration.gpio = AFE_CONFIG_HARDWARE_DHT_DEFAULT_GPIO;
  configuration.interval = AFE_CONFIG_HARDWARE_DHT_DEFAULT_INTERVAL;
  configuration.sendOnlyChanges =
      AFE_CONFIG_HARDWARE_DHT_DEFAULT_SENDING_ONLY_CHANGES;
  configuration.type = AFE_HARDWARE_ITEM_NOT_EXIST;
  configuration.temperature.unit = AFE_TEMPERATURE_UNIT_CELSIUS;
  configuration.temperature.correction =
      AFE_CONFIG_HARDWARE_DHT_DEFAULT_TEMPERATURE_CORRECTION;
  configuration.humidity.unit = AFE_HUMIDITY_UNIT;
  configuration.humidity.correction =
      AFE_CONFIG_HARDWARE_DHT_DEFAULT_HUMIDITY_CORRECTION;

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  configuration.domoticz.temperatureHumidity.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  configuration.domoticz.temperature.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  configuration.domoticz.humidity.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  configuration.domoticz.dewPoint.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  configuration.domoticz.heatIndex.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  configuration.domoticz.comfort.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  configuration.domoticz.perception.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  configuration.domoticz.absoluteHumidity.idx = AFE_DOMOTICZ_DEFAULT_IDX;
#else
  configuration.mqtt.topic[0] = AFE_EMPTY_STRING;
#endif

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_DHT; i++) {
#ifdef DEBUG
    Serial << endl << F("INFO: Creating file: /cfg-dht-") << i << F(".json");
#endif
    sprintf(configuration.name, "DHT-%d", i + 1);
    saveConfiguration(i, &configuration);
  }
}

#endif // AFE_CONFIG_HARDWARE_DHT

IPAddress AFEDataAccess::IPfromString(const char *address) {
  IPAddress ip;
  if (!ip.fromString(address)) {
#ifdef DEBUG
    Serial << endl
           << F("ERROR: converting from IP String (") << address
           << F(") to IP address");
#endif
  };
  return ip;
}

/*************** OLD METHODS USED FOR UPGRADE ONLY ******************/

#if defined(T0_CONFIG) && !defined(ESP32)
DEVICE_T0_200 AFEDataAccess::getDeviceT0v200Configuration() {
  DEVICE_T0_200 configuration;
#ifdef DEBUG
  Serial << endl
         << endl
         << F("----------------- Reading File -------------------");
  Serial << endl << F("Opening file: cfg-device.json : ");
  Serial << endl << F("THIS IS OLD VERSION OF THE DEVICE FILE ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open("cfg-device.json", "r");
#else
  File configFile = SPIFFS.open("cfg-device.json", "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<526> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif
      sprintf(configuration.name, root["name"]);
      configuration.api.http = root["api"]["http"];
      configuration.api.mqtt = root["api"]["mqtt"];
      configuration.api.domoticz = root["api"]["domoticz"];
      /* HTTP API must be ON when Domoticz is ON */
      if (configuration.api.domoticz && !configuration.api.http) {
        configuration.api.http = true;
      }

#ifdef AFE_CONFIG_HARDWARE_LED
      for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_LEDS; i++) {
        configuration.isLED[i] = root["led"][i];
      }
#endif

      for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_SWITCHES; i++) {
        configuration.isSwitch[i] = root["switch"][i];
      }

      for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_RELAYS; i++) {
        configuration.isRelay[i] = root["relay"][i];
      }

#ifdef CONFIG_HARDWARE_ADC_VCC
      configuration.isAnalogInput = root["isAnalogInput"];
#endif

#ifdef DEBUG
      Serial << endl
             << F("success") << endl
             << F("JSON Buffer size: ") << jsonBuffer.size();
#endif
    }

#ifdef DEBUG
    else {
      Serial << F("failure");
    }
#endif
  }

#ifdef DEBUG
  else {
    Serial << F("failure");
  }
  Serial << endl << F("--------------------------------------------------");
#endif

  return configuration;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
boolean AFEDataAccess::getConfiguration(ANEMOMETER *configuration) {
  boolean _ret = true;
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ")
         << F(AFE_FILE_ANEMOMETER_SENSOR_CONFIGURATION) << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile =
      LittleFS.open(AFE_FILE_ANEMOMETER_SENSOR_CONFIGURATION, "r");
#else
  File configFile = SPIFFS.open(AFE_FILE_ANEMOMETER_SENSOR_CONFIGURATION, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_ANEMOMETER> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif
      sprintf(configuration->name, root["name"] | "");
      configuration->gpio = root["gpio"] | AFE_HARDWARE_ANEMOMETER_DEFAULT_GPIO;
      configuration->sensitiveness =
          root["sensitiveness"] | AFE_HARDWARE_ANEMOMETER_DEFAULT_BOUNCING;
      configuration->interval =
          root["interval"] | AFE_HARDWARE_ANEMOMETER_DEFAULT_INTERVAL;
      configuration->impulseDistance = root["impulseDistance"].as<float>();
      configuration->impulseDistanceUnit =
          root["impulseDistanceUnit"] |
          AFE_HARDWARE_ANEMOMETER_DEFAULT_IMPULSE_DISTANCE_UNIT;

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
      configuration->domoticz.idx = root["idx"] | AFE_DOMOTICZ_DEFAULT_IDX;
#else
      sprintf(configuration->mqtt.topic, root["MQTTTopic"] | "");
#endif

#ifdef DEBUG
      printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_ANEMOMETER, jsonBuffer.size());
#endif
    } else {
#ifdef DEBUG
      Serial << F("ERROR: JSON not pharsed");
#endif
      _ret = false;
    }
    configFile.close();
  }

  else {
    _ret = false;
#ifdef DEBUG
    Serial << endl
           << F("ERROR: Configuration file: ")
           << F(AFE_FILE_ANEMOMETER_SENSOR_CONFIGURATION) << F(" not opened");
#endif
  }
  return _ret;
}

void AFEDataAccess::saveConfiguration(ANEMOMETER *configuration) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ")
         << F(AFE_FILE_ANEMOMETER_SENSOR_CONFIGURATION) << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile =
      LittleFS.open(AFE_FILE_ANEMOMETER_SENSOR_CONFIGURATION, "w");
#else
  File configFile = SPIFFS.open(AFE_FILE_ANEMOMETER_SENSOR_CONFIGURATION, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_ANEMOMETER> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["name"] = configuration->name;
    root["gpio"] = configuration->gpio;
    root["sensitiveness"] = configuration->sensitiveness;
    root["interval"] = configuration->interval;
    root["impulseDistance"] = configuration->impulseDistance;
    root["impulseDistanceUnit"] = configuration->impulseDistanceUnit;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    root["idx"] = configuration->domoticz.idx;
#else
    root["MQTTTopic"] = configuration->mqtt.topic;
#endif
    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_ANEMOMETER, jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}

void AFEDataAccess::createAnemometerSensorConfigurationFile() {
#ifdef DEBUG
  Serial << endl
         << F("INFO: Creating file: ")
         << F(AFE_FILE_ANEMOMETER_SENSOR_CONFIGURATION);
#endif
  ANEMOMETER configuration;
  configuration.sensitiveness = AFE_HARDWARE_ANEMOMETER_DEFAULT_BOUNCING;
  configuration.interval = AFE_HARDWARE_ANEMOMETER_DEFAULT_INTERVAL;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  configuration.domoticz.idx = AFE_DOMOTICZ_DEFAULT_IDX;
#else
  configuration.mqtt.topic[0] = AFE_EMPTY_STRING;
#endif
  /* Saving first switch. Common for all devices */
  configuration.gpio = AFE_HARDWARE_ANEMOMETER_DEFAULT_GPIO;
  configuration.impulseDistance =
      AFE_HARDWARE_ANEMOMETER_DEFAULT_IMPULSE_DISTANCE;
  configuration.impulseDistanceUnit =
      AFE_HARDWARE_ANEMOMETER_DEFAULT_IMPULSE_DISTANCE_UNIT;

  sprintf(configuration.name, "anemometer");

  saveConfiguration(&configuration);
}
#endif // AFE_CONFIG_HARDWARE_ANEMOMETER

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
boolean AFEDataAccess::getConfiguration(RAINMETER *configuration) {
  boolean _ret = true;
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ")
         << F(AFE_FILE_RAINMETER_SENSOR_CONFIGURATION) << F(" ... ");
#endif
#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(AFE_FILE_RAINMETER_SENSOR_CONFIGURATION, "r");
#else
  File configFile = SPIFFS.open(AFE_FILE_RAINMETER_SENSOR_CONFIGURATION, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_RAINMETER> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif
      sprintf(configuration->name, root["name"] | "");
      configuration->gpio = root["gpio"] | AFE_HARDWARE_RAINMETER_DEFAULT_GPIO;
      configuration->sensitiveness =
          root["sensitiveness"] | AFE_HARDWARE_RAINMETER_DEFAULT_BOUNCING;
      configuration->interval =
          root["interval"] | AFE_HARDWARE_RAINMETER_DEFAULT_INTERVAL;
      configuration->resolution = root["resolution"].as<float>();

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
      configuration->domoticz.idx = root["idx"] | AFE_DOMOTICZ_DEFAULT_IDX;
#else
      sprintf(configuration->mqtt.topic, root["MQTTTopic"] | "");
#endif

#ifdef DEBUG
      printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_RAINMETER, jsonBuffer.size());
#endif
    } else {
#ifdef DEBUG
      Serial << F("ERROR: JSON not pharsed");
#endif
      _ret = false;
    }
    configFile.close();

  } else {
    _ret = false;
#ifdef DEBUG
    Serial << endl
           << F("ERROR: Configuration file: ")
           << F(AFE_FILE_RAINMETER_SENSOR_CONFIGURATION) << F(" not opened");
#endif
  }

  return _ret;
}

void AFEDataAccess::saveConfiguration(RAINMETER *configuration) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ")
         << F(AFE_FILE_RAINMETER_SENSOR_CONFIGURATION) << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(AFE_FILE_RAINMETER_SENSOR_CONFIGURATION, "w");
#else
  File configFile = SPIFFS.open(AFE_FILE_RAINMETER_SENSOR_CONFIGURATION, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_RAINMETER> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["name"] = configuration->name;
    root["gpio"] = configuration->gpio;
    root["sensitiveness"] = configuration->sensitiveness;
    root["interval"] = configuration->interval;
    root["resolution"] = configuration->resolution;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    root["idx"] = configuration->domoticz.idx;
#else
    root["MQTTTopic"] = configuration->mqtt.topic;
#endif
    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_RAINMETER, jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}

void AFEDataAccess::createRainmeterSensorConfigurationFile() {
#ifdef DEBUG
  Serial << endl
         << F("INFO: Creating file: ")
         << F(AFE_FILE_RAINMETER_SENSOR_CONFIGURATION);
#endif
  RAINMETER configuration;
  configuration.sensitiveness = AFE_HARDWARE_RAINMETER_DEFAULT_BOUNCING;
  configuration.interval = AFE_HARDWARE_RAINMETER_DEFAULT_INTERVAL;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  configuration.domoticz.idx = AFE_DOMOTICZ_DEFAULT_IDX;
#else
  configuration.mqtt.topic[0] = AFE_EMPTY_STRING;
#endif
  /* Saving first switch. Common for all devices */
  configuration.gpio = AFE_HARDWARE_RAINMETER_DEFAULT_GPIO;
  configuration.resolution = AFE_HARDWARE_RAINMETER_DEFAULT_RESOLUTION;

  sprintf(configuration.name, "rainmeter");

  saveConfiguration(&configuration);
}

void AFEDataAccess::get(RAINMETER_DATA *data) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << AFE_FILE_RAINMETER_SENSOR_DATA
         << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(AFE_FILE_RAINMETER_SENSOR_DATA, "r");
#else
  File configFile = SPIFFS.open(AFE_FILE_RAINMETER_SENSOR_DATA, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_RAINMETER_DATA> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      data->index1h = root["index1h"];
      for (uint8_t i = 0; i < 60; i++) {
        data->last1h[i] = root["last1h"][i].as<float>();
      }

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
      data->counter = root["counter"].as<float>();
#else
      data->index12h = root["index12h"];
      data->index24h = root["index24h"];
      for (uint8_t i = 0; i < 12; i++) {
        data->last12h[i] = root["last12h"][i].as<float>();
      }

      data->last24h[0] = root["last24h"][0].as<float>();
      data->last24h[1] = root["last24h"][1].as<float>();
#endif

#ifdef DEBUG
      printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_RAINMETER_DATA,
                          jsonBuffer.size());
#endif
    }
#ifdef DEBUG
    else {
      Serial << F("ERROR: JSON not pharsed");
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else {
    Serial << endl
           << F("ERROR: Configuration file: ")
           << F(AFE_FILE_RAINMETER_SENSOR_DATA) << F(" not opened");
  }
#endif
}
void AFEDataAccess::save(RAINMETER_DATA *data) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << F(AFE_FILE_RAINMETER_SENSOR_DATA)
         << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(AFE_FILE_RAINMETER_SENSOR_DATA, "w");
#else
  File configFile = SPIFFS.open(AFE_FILE_RAINMETER_SENSOR_DATA, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_RAINMETER_DATA> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    JsonArray &last1h = jsonBuffer.createArray();
#if AFE_FIRMWARE_API != AFE_FIRMWARE_API_DOMOTICZ
    JsonArray &last12h = jsonBuffer.createArray();
    JsonArray &last24h = jsonBuffer.createArray();
#endif

    last1h.copyFrom(data->last1h);
    root["index1h"] = data->index1h;
    root["last1h"] = last1h;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    root["counter"] = data->counter;
#else

    last12h.copyFrom(data->last12h);
    root["index12h"] = data->index12h;
    root["last12h"] = last12h;

    last24h.copyFrom(data->last24h);
    root["index24h"] = data->index24h;
    root["last24h"] = last24h;
#endif
    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_RAINMETER_DATA,
                        jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}

void AFEDataAccess::createRainmeterSensorDataConfigurationFile() {
#ifdef DEBUG
  Serial << endl
         << F("INFO: Creating file: ") << F(AFE_FILE_RAINMETER_SENSOR_DATA);
#endif
  RAINMETER_DATA data;
  data.index1h = 0;
  for (uint8_t i = 0; i < 60; i++) {
    data.last1h[i] = 0;
  }
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  data.counter = 0;
#else
  for (uint8_t i = 0; i < 12; i++) {
    data.last12h[i] = 0;
  }
  data.last24h[0] = 0;
  data.last24h[1] = 0;
  data.index12h = 0;
  data.index24h = 0;
#endif

  save(&data);
}

#endif // AFE_CONFIG_HARDWARE_RAINMETER

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
void AFEDataAccess::getConfiguration(uint8_t id, BINARY_SENSOR *configuration) {
  char fileName[28];
  sprintf(fileName, AFE_FILE_BINARY_SENSOR_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "r");
#else
  File configFile = SPIFFS.open(fileName, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_BINARY_SENSOR> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif
      sprintf(configuration->name, root["name"]);
      configuration->gpio = root["gpio"].as<int>();
      configuration->revertSignal =
          root["revertSignal"] |
          AFE_HARDWARE_BINARY_SENSOR_DEFAULT_REVERT_SIGNAL;
      configuration->internalPullUp =
          root["internalPullUp"] |
          AFE_HARDWARE_BINARY_SENSOR_DEFAULT_INTERNAL_PULLUP_RESISTOR;
      configuration->sendAsSwitch =
          root["sendAsSwitch"] |
          AFE_HARDWARE_BINARY_SENSOR_DEFAULT_SENT_AS_SWITCH;
      configuration->bouncing =
          root["bouncing"] | AFE_HARDWARE_BINARY_SENSOR_DEFAULT_BOUNCING;

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
      configuration->domoticz.idx = root["idx"] | AFE_DOMOTICZ_DEFAULT_IDX;
#else
      sprintf(configuration->mqtt.topic, root["MQTTTopic"] | "");
#endif

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
      configuration->mcp23017.id = root["mcp23017"]["id"];
      configuration->mcp23017.gpio = root["mcp23017"]["gpio"];
#endif

#ifdef DEBUG
      printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_BINARY_SENSOR,
                          jsonBuffer.size());
#endif
    }
#ifdef DEBUG
    else {
      Serial << F("ERROR: JSON not pharsed");
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else {
    Serial << endl
           << F("ERROR: Configuration file: ") << fileName << F(" not opened");
  }
#endif
}
void AFEDataAccess::saveConfiguration(uint8_t id,
                                      BINARY_SENSOR *configuration) {
  char fileName[28];
  sprintf(fileName, AFE_FILE_BINARY_SENSOR_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "w");
#else
  File configFile = SPIFFS.open(fileName, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_BINARY_SENSOR> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["name"] = configuration->name;
    root["gpio"] = configuration->gpio;
    root["revertSignal"] = configuration->revertSignal;
    root["internalPullUp"] = configuration->internalPullUp;
    root["sendAsSwitch"] = configuration->sendAsSwitch;
    root["bouncing"] = configuration->bouncing;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    root["idx"] = configuration->domoticz.idx;
#else
    root["MQTTTopic"] = configuration->mqtt.topic;
#endif

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
    JsonObject &mcp23017 = root.createNestedObject("mcp23017");
    mcp23017["id"] = configuration->mcp23017.id;
    mcp23017["gpio"] = configuration->mcp23017.gpio;
#endif // AFE_CONFIG_HARDWARE_MCP23XXX

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_BINARY_SENSOR,
                        jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}
void AFEDataAccess::createBinarySensorConfigurationFile() {
  BINARY_SENSOR configuration;
  configuration.bouncing = AFE_HARDWARE_BINARY_SENSOR_DEFAULT_BOUNCING;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  configuration.domoticz.idx = AFE_DOMOTICZ_DEFAULT_IDX;
#else
  configuration.mqtt.topic[0] = AFE_EMPTY_STRING;
#endif

  configuration.gpio = AFE_HARDWARE_ITEM_NOT_EXIST;
  configuration.revertSignal = AFE_HARDWARE_BINARY_SENSOR_DEFAULT_REVERT_SIGNAL;
  configuration.sendAsSwitch =
      AFE_HARDWARE_BINARY_SENSOR_DEFAULT_SENT_AS_SWITCH;
  configuration.internalPullUp =
      AFE_HARDWARE_BINARY_SENSOR_DEFAULT_INTERNAL_PULLUP_RESISTOR;

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
  configuration.mcp23017.id = AFE_HARDWARE_ITEM_NOT_EXIST;
  configuration.mcp23017.gpio = AFE_HARDWARE_ITEM_NOT_EXIST;
#endif // AFE_CONFIG_HARDWARE_MCP23XXX

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  configuration.domoticz.idx = AFE_DOMOTICZ_DEFAULT_IDX;
#else
  configuration.mqtt.topic[0] = AFE_EMPTY_STRING;
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_BINARY_SENSORS;
       i++) {
#ifdef DEBUG
    Serial << endl
           << F("INFO: Creating file: cfg-binary-sensor-") << i << F(".json");
#endif

    sprintf(configuration.name, "binary-%d", i + 1);

    saveConfiguration(i, &configuration);
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
void AFEDataAccess::getConfiguration(uint8_t id, PN532_SENSOR *configuration) {
  char fileName[27];
  sprintf(fileName, AFE_FILE_PN532_SENSOR_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "r");
#else
  File configFile = SPIFFS.open(fileName, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_PN532_SENSOR> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif
      sprintf(configuration->name, root["name"]);
      configuration->tx = root["tx"].as<int>();
      configuration->rx = root["rx"].as<int>();
      configuration->requestProcessingTime =
          root["requestProcessingTime"].as<int>();
      configuration->interface = root["interface"].as<int>();
      configuration->listenerTimeout = root["listenerTimeout"].as<int>();
#ifdef AFE_ESP32
      configuration->wirePortId = root["wirePortId"];
#endif
      configuration->i2cAddress = root["i2cAddress"].as<int>();

      configuration->timeout = root["timeout"].as<int>();
#ifdef AFE_CONFIG_HARDWARE_LED
      configuration->ledID = root["ledID"].as<int>();
#endif

#if AFE_FIRMWARE_API != AFE_FIRMWARE_API_DOMOTICZ
      sprintf(configuration->mqtt.topic, root["MQTTTopic"] | "");
#endif

#ifdef DEBUG
      printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_PN532_SENSOR,
                          jsonBuffer.size());
#endif
    }
#ifdef DEBUG
    else {
      Serial << F("ERROR: JSON not pharsed");
    }
#endif
    configFile.close();
  }

#ifdef DEBUG
  else {
    Serial << endl
           << F("ERROR: Configuration file: ") << fileName << F(" not opened");
  }
#endif
}

void AFEDataAccess::saveConfiguration(uint8_t id, PN532_SENSOR *configuration) {
  char fileName[27];
  sprintf(fileName, AFE_FILE_PN532_SENSOR_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "w");
#else
  File configFile = SPIFFS.open(fileName, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_PN532_SENSOR> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["name"] = configuration->name;
    root["tx"] = configuration->tx;
    root["rx"] = configuration->rx;
    root["requestProcessingTime"] = configuration->requestProcessingTime;
    root["timeout"] = configuration->timeout;
    root["interface"] = configuration->interface;
    root["listenerTimeout"] = configuration->listenerTimeout;
#ifdef AFE_ESP32
    root["wirePortId"] = configuration->wirePortId;
#endif
    root["i2cAddress"] = configuration->i2cAddress;

#ifdef AFE_CONFIG_HARDWARE_LED
    root["ledID"] = configuration->ledID;
#endif
#if AFE_FIRMWARE_API != AFE_FIRMWARE_API_DOMOTICZ
    root["MQTTTopic"] = configuration->mqtt.topic;
#endif

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_PN532_SENSOR, jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}

void AFEDataAccess::createPN532ConfigurationFile() {
  PN532_SENSOR configuration;
  sprintf(configuration.name, "pn532");
  configuration.interface = AFE_HARDWARE_ITEM_NOT_EXIST;
  configuration.tx = AFE_HARDWARE_ITEM_NOT_EXIST;
  configuration.rx = AFE_HARDWARE_ITEM_NOT_EXIST;
  configuration.timeout = AFE_HARDWARE_PN532_DEFUALT_TIMEOUT;
  configuration.requestProcessingTime =
      AFE_HARDWARE_PN532_DEFUALT_REQUEST_PROCESSING_TIME;
#ifdef AFE_ESP32
  configuration.wirePortId = AFE_HARDWARE_ITEM_NOT_EXIST;
#endif
  configuration.i2cAddress = AFE_HARDWARE_PN532_DEFAULT_INTERFACE;
  configuration.listenerTimeout = AFE_HARDWARE_PN532_DEFUALT_LISTENER_TIMEOUT;

#ifdef AFE_CONFIG_HARDWARE_LED
  configuration.ledID = AFE_HARDWARE_ITEM_NOT_EXIST;
#endif

#if AFE_FIRMWARE_API != AFE_FIRMWARE_API_DOMOTICZ
  configuration.mqtt.topic[0] = AFE_EMPTY_STRING;
#endif

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_PN532; i++) {
#ifdef DEBUG
    Serial << endl
           << F("INFO: Creating file: cfg-pn532-sensor-") << i << F(".json");
#endif

    saveConfiguration(i, &configuration);
  }
}

void AFEDataAccess::getConfiguration(uint8_t id, MIFARE_CARD *configuration) {
  char fileName[26];
  sprintf(fileName, AFE_FILE_MIFARE_CARD_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "r");
#else
  File configFile = SPIFFS.open(fileName, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_MIFARE_CARD> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif
      configuration->action = root["action"].as<int>();
      configuration->sendAsSwitch = root["sendAsSwitch"];
      configuration->relayId = root["relayId"].as<int>();
      configuration->howLongKeepState = root["howLongKeepState"].as<int>();
      sprintf(configuration->cardId, root["cardId"] | "");

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
      for (uint8_t i = 0; i < AFE_HARDWARE_PN532_TAG_SIZE; i++) {
        configuration->domoticz[i].idx = root["idx"][i].as<int>();
      }
#else
      sprintf(configuration->mqtt.topic, root["MQTTTopic"] | "");
#endif

#ifdef DEBUG
      printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_MIFARE_CARD,
                          jsonBuffer.size());
#endif
    }
#ifdef DEBUG
    else {
      Serial << F("ERROR: JSON not pharsed");
    }
#endif
    configFile.close();
  }

#ifdef DEBUG
  else {
    Serial << endl
           << F("ERROR: Configuration file: ") << fileName << F(" not opened");
  }
#endif
}

void AFEDataAccess::saveConfiguration(uint8_t id, MIFARE_CARD *configuration) {
  char fileName[26];
  sprintf(fileName, AFE_FILE_MIFARE_CARD_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "w");
#else
  File configFile = SPIFFS.open(fileName, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_MIFARE_CARD> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    JsonArray &jsonIdx = root.createNestedArray("idx");
#endif
    root["action"] = configuration->action;
    root["cardId"] = configuration->cardId;
    root["relayId"] = configuration->relayId;
    root["sendAsSwitch"] = configuration->sendAsSwitch;
    root["howLongKeepState"] = configuration->howLongKeepState;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    for (uint8_t i = 0; i < AFE_HARDWARE_PN532_TAG_SIZE; i++) {
      jsonIdx.add(configuration->domoticz[i].idx);
    }
#else
    root["MQTTTopic"] = configuration->mqtt.topic;
#endif

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_MIFARE_CARD, jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}

void AFEDataAccess::createMiFareCardConfigurationFile() {
  MIFARE_CARD configuration;
  configuration.relayId = AFE_HARDWARE_ITEM_NOT_EXIST;
  configuration.action = AFE_HARDWARE_ITEM_NOT_EXIST;
  configuration.sendAsSwitch = AFE_HARDWARE_MIFARE_CARD_DEFAULT_SEND_AS;
  configuration.cardId[0] = AFE_EMPTY_STRING;
  configuration.howLongKeepState =
      AFE_HARDWARE_MIFARE_CARD_DEFAULT_HOW_LONG_KEEP_STATE;

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  for (uint8_t i = 0; i < AFE_HARDWARE_PN532_TAG_SIZE; i++) {
    configuration.domoticz[i].idx = AFE_DOMOTICZ_DEFAULT_IDX;
  }
#else
  configuration.mqtt.topic[0] = AFE_EMPTY_STRING;
#endif

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_MIFARE_CARDS; i++) {
#ifdef DEBUG
    Serial << endl
           << F("INFO: Creating file: cfg-mifare-card-") << i << F(".json");
#endif

    saveConfiguration(i, &configuration);
  }
}

#endif

#ifdef AFE_CONFIG_HARDWARE_CLED
boolean AFEDataAccess::getConfiguration(uint8_t id, CLED *configuration) {
  boolean _ret = true;
  char fileName[strlen(AFE_FILE_LED_CONFIGURATION) + 1];
  sprintf(fileName, AFE_FILE_CLED_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  if (!LittleFS.exists(fileName)) {
    createCLEDConfigurationFile();
  }
  File configFile = LittleFS.open(fileName, "r");
#else
  File configFile = SPIFFS.open(fileName, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_CLED> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif
      configuration->gpio = root["gpio"].as<int>();
      configuration->ledNumbers = root["ledNumbers"].as<int>();

      configuration->on.color.red = root["on"]["c"]["r"].as<int>();
      configuration->on.color.green = root["on"]["c"]["g"].as<int>();
      configuration->on.color.blue = root["on"]["c"]["b"].as<int>();
      configuration->on.brightness = root["on"]["l"].as<int>();

      JsonVariant exists = root["on"]["t"];
      configuration->on.changeTime =
          exists.success() ? root["on"]["t"].as<int>()
                           : AFE_CONFIG_HARDWARE_CLED_DEFAULT_CHANGE_TIME;

      exists = root["off"]["t"];
      configuration->off.changeTime =
          exists.success() ? root["off"]["t"].as<int>()
                           : AFE_CONFIG_HARDWARE_CLED_DEFAULT_CHANGE_TIME;

      sprintf(configuration->name, root["name"]);
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ

      exists = root["cIdx"];
      configuration->cled.idx =
          exists.success() ? root["cIdx"] : AFE_DOMOTICZ_DEFAULT_IDX;
      exists = root["eIdx"];
      configuration->effect.idx =
          exists.success() ? root["eIdx"] : AFE_DOMOTICZ_DEFAULT_IDX;
#else
      exists = root["cMqttTopic"];
      if (exists.success()) {
        sprintf(configuration->cled.topic, root["cMqttTopic"]);
      } else {
        configuration->cled.topic[0] = AFE_EMPTY_STRING;
      }

      exists = root["eMqttTopic"];
      if (exists.success()) {
        sprintf(configuration->effect.topic, root["eMqttTopic"]);
      } else {
        configuration->effect.topic[0] = AFE_EMPTY_STRING;
      }

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_STANDARD
      exists = root["bConversion"];
      configuration->brightnessConversion =
          exists.success()
              ? root["bConversion"]
              : AFE_CONFIG_HARDWARE_CLED_DEFAULT_BRIGHTNESS_CONVERSION;
#endif

#endif

#ifdef DEBUG
      printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_CLED, jsonBuffer.size());
#endif
    } else {
      _ret = false;
#ifdef DEBUG
      Serial << F("ERROR: JSON not pharsed");

#endif
    }
    configFile.close();
  } else {
    _ret = false;
#ifdef DEBUG
    Serial << endl
           << F("ERROR: Configuration file: ") << fileName << F(" not opened");
#endif
  }
  return _ret;
}
void AFEDataAccess::saveConfiguration(uint8_t id, CLED *configuration) {
  char fileName[strlen(AFE_FILE_CLED_CONFIGURATION) + 1];
  sprintf(fileName, AFE_FILE_CLED_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "w");
#else
  File configFile = SPIFFS.open(fileName, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_CLED> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    JsonObject &on = root.createNestedObject("on");
    JsonObject &cOn = on.createNestedObject("c");
    JsonObject &off = root.createNestedObject("off");
    root["gpio"] = configuration->gpio;
    root["ledNumbers"] = configuration->ledNumbers;
    cOn["r"] = configuration->on.color.red;
    cOn["g"] = configuration->on.color.green;
    cOn["b"] = configuration->on.color.blue;
    on["l"] = configuration->on.brightness;
    on["t"] = configuration->on.changeTime;
    off["t"] = configuration->off.changeTime;

    root["name"] = configuration->name;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    root["cIdx"] = configuration->cled.idx;
    root["eIdx"] = configuration->effect.idx;
#else
    root["cMqttTopic"] = configuration->cled.topic;
    root["eMqttTopic"] = configuration->effect.topic;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_STANDARD
    root["bConversion"] = configuration->brightnessConversion;
#endif
#endif
    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_CLED, jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}
void AFEDataAccess::createCLEDConfigurationFile() {
  CLED configuration;
  /*
  configuration.chipset = 0;
  configuration.colorOrder = AFE_CONFIG_HARDWARE_CLED_COLORS_ORDER;
  */
  configuration.ledNumbers = AFE_CONFIG_HARDWARE_CLED_MAX_NUMBER_OF_LED;
  configuration.on.color.green = AFE_CONFIG_HARDWARE_CLED_DEFAULT_ON_COLOR;
  configuration.on.color.red = AFE_CONFIG_HARDWARE_CLED_DEFAULT_ON_COLOR;
  configuration.on.color.blue = AFE_CONFIG_HARDWARE_CLED_DEFAULT_ON_COLOR;
  configuration.on.changeTime = AFE_CONFIG_HARDWARE_CLED_DEFAULT_CHANGE_TIME;
  configuration.on.brightness = AFE_CONFIG_HARDWARE_CLED_DEFAULT_ON_BRIGHTNESS;
  configuration.off.changeTime = AFE_CONFIG_HARDWARE_CLED_DEFAULT_CHANGE_TIME;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  configuration.cled.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  configuration.effect.idx = AFE_DOMOTICZ_DEFAULT_IDX;
#else
  configuration.cled.topic[0] = AFE_EMPTY_STRING;
  configuration.effect.topic[0] = AFE_EMPTY_STRING;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_STANDARD
  configuration.brightnessConversion =
      AFE_CONFIG_HARDWARE_CLED_DEFAULT_BRIGHTNESS_CONVERSION;
#endif // AFE_FIRMWARE_API == AFE_FIRMWARE_API_STANDARD
#endif // AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ

#ifdef DEBUG
  Serial << endl << F("INFO: Creating file: cfg-cled-X.json");
#endif

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_CLED_STRIPS; i++) {
    configuration.gpio = i == 0 ? AFE_CONFIG_HARDWARE_CLED_0_GPIO
                                : AFE_CONFIG_HARDWARE_CLED_1_GPIO;
    sprintf(configuration.name, "RGBLED-%d", i + 1);
    saveConfiguration(i, &configuration);
  }
}

/* RGB LED Blinking Effect */
boolean AFEDataAccess::getConfiguration(uint8_t id,
                                        CLED_EFFECT_BLINKING *configuration) {
  boolean _ret = true;
  char fileName[strlen(AFE_FILE_CLED_EFFECT_BLINKING_CONFIGURATION) + 1];
  sprintf(fileName, AFE_FILE_CLED_EFFECT_BLINKING_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  if (!LittleFS.exists(fileName)) {
    createCLEDConfigurationFile();
  }
  File configFile = LittleFS.open(fileName, "r");
#else
  File configFile = SPIFFS.open(fileName, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_CLED_EFFECT_BLINKING> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      configuration->on.color.red = root["on"]["c"]["r"].as<int>();
      configuration->on.color.green = root["on"]["c"]["g"].as<int>();
      configuration->on.color.blue = root["on"]["c"]["b"].as<int>();
      configuration->on.brightness = root["on"]["l"].as<int>();
      configuration->onTimeout = root["on"]["timeout"].as<int>();
      configuration->off.color.red = root["off"]["c"]["r"].as<int>();
      configuration->off.color.green = root["off"]["c"]["g"].as<int>();
      configuration->off.color.blue = root["off"]["c"]["b"].as<int>();
      configuration->off.brightness = root["off"]["l"].as<int>();
      configuration->offTimeout = root["off"]["timeout"].as<int>();
      sprintf(configuration->name, root["name"]);

#ifdef DEBUG
      printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_CLED_EFFECT_BLINKING,
                          jsonBuffer.size());
#endif
    } else {
      _ret = false;
#ifdef DEBUG
      Serial << F("ERROR: JSON not pharsed");

#endif
    }
    configFile.close();
  } else {
    _ret = false;
#ifdef DEBUG
    Serial << endl
           << F("ERROR: Configuration file: ") << fileName << F(" not opened");
#endif
  }
  return _ret;
}
void AFEDataAccess::saveConfiguration(uint8_t id,
                                      CLED_EFFECT_BLINKING *configuration) {
  char fileName[strlen(AFE_FILE_CLED_EFFECT_BLINKING_CONFIGURATION) + 1];
  sprintf(fileName, AFE_FILE_CLED_EFFECT_BLINKING_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "w");
#else
  File configFile = SPIFFS.open(fileName, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_CLED_EFFECT_BLINKING> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    JsonObject &on = root.createNestedObject("on");
    JsonObject &off = root.createNestedObject("off");
    JsonObject &cOn = on.createNestedObject("c");
    JsonObject &cOff = off.createNestedObject("c");

    cOn["r"] = configuration->on.color.red;
    cOn["g"] = configuration->on.color.green;
    cOn["b"] = configuration->on.color.blue;
    on["l"] = configuration->on.brightness;
    cOff["r"] = configuration->off.color.red;
    cOff["g"] = configuration->off.color.green;
    cOff["b"] = configuration->off.color.blue;
    off["l"] = configuration->off.brightness;
    on["timeout"] = configuration->onTimeout;
    off["timeout"] = configuration->offTimeout;
    root["name"] = configuration->name;

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();
#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_CLED_EFFECT_BLINKING,
                        jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}

void AFEDataAccess::createCLEDEffectBlinkingConfigurationFile() {
  CLED_EFFECT_BLINKING configuration;
  configuration.on.color.red = AFE_CONFIG_HARDWARE_CLED_DEFAULT_ON_COLOR;
  configuration.on.color.green = AFE_CONFIG_HARDWARE_CLED_DEFAULT_ON_COLOR;
  configuration.on.color.blue = AFE_CONFIG_HARDWARE_CLED_DEFAULT_ON_COLOR;
  configuration.on.brightness =
      AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING_DEFAULT_ON_BRIGHTNESS;
  configuration.off.color.red = AFE_CONFIG_HARDWARE_CLED_DEFAULT_OFF_COLOR;
  configuration.off.color.green = AFE_CONFIG_HARDWARE_CLED_DEFAULT_OFF_COLOR;
  configuration.off.color.blue = AFE_CONFIG_HARDWARE_CLED_DEFAULT_OFF_COLOR;
  configuration.off.brightness =
      AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING_DEFAULT_OFF_BRIGHTNESS;
  configuration.onTimeout =
      AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING_DEFAULT_ON_TIMER;
  configuration.offTimeout =
      AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING_DEFAULT_OFF_TIMER;

#ifdef DEBUG
  Serial << endl << F("INFO: Creating CLED EFFECT Blinking configuration file");
#endif

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_CLED_STRIPS; i++) {
    sprintf(configuration.name, "BLINK-%d", i + 1);
    saveConfiguration(i, &configuration);
  }
}

/* RGB LED Wave Effect */
boolean AFEDataAccess::getConfiguration(uint8_t id,
                                        CLED_EFFECT_WAVE *configuration) {
  boolean _ret = true;
  char fileName[strlen(AFE_FILE_CLED_EFFECT_WAVE_CONFIGURATION) + 1];
  sprintf(fileName, AFE_FILE_CLED_EFFECT_WAVE_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  if (!LittleFS.exists(fileName)) {
    createCLEDConfigurationFile();
  }
  File configFile = LittleFS.open(fileName, "r");
#else
  File configFile = SPIFFS.open(fileName, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_CLED_EFFECT_WAVE> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      configuration->on.color.red = root["on"]["c"]["r"].as<int>();
      configuration->on.color.green = root["on"]["c"]["g"].as<int>();
      configuration->on.color.blue = root["on"]["c"]["b"].as<int>();
      configuration->on.brightness = root["on"]["l"].as<int>();
      configuration->off.color.red = root["off"]["c"]["r"].as<int>();
      configuration->off.color.green = root["off"]["c"]["g"].as<int>();
      configuration->off.color.blue = root["off"]["c"]["b"].as<int>();
      configuration->timeout = root["timeout"].as<int>();
      sprintf(configuration->name, root["name"]);

#ifdef DEBUG
      printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_CLED_EFFECT_WAVE,
                          jsonBuffer.size());
#endif
    } else {
      _ret = false;
#ifdef DEBUG
      Serial << F("ERROR: JSON not pharsed");

#endif
    }
    configFile.close();
  } else {
    _ret = false;
#ifdef DEBUG
    Serial << endl
           << F("ERROR: Configuration file: ") << fileName << F(" not opened");
#endif
  }
  return _ret;
}
void AFEDataAccess::saveConfiguration(uint8_t id,
                                      CLED_EFFECT_WAVE *configuration) {
  char fileName[strlen(AFE_FILE_CLED_EFFECT_WAVE_CONFIGURATION) + 1];
  sprintf(fileName, AFE_FILE_CLED_EFFECT_WAVE_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "w");
#else
  File configFile = SPIFFS.open(fileName, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_CLED_EFFECT_WAVE> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    JsonObject &on = root.createNestedObject("on");
    JsonObject &off = root.createNestedObject("off");
    JsonObject &cOn = on.createNestedObject("c");
    JsonObject &cOff = off.createNestedObject("c");

    cOn["r"] = configuration->on.color.red;
    cOn["g"] = configuration->on.color.green;
    cOn["b"] = configuration->on.color.blue;
    on["l"] = configuration->on.brightness;
    cOff["r"] = configuration->off.color.red;
    cOff["g"] = configuration->off.color.green;
    cOff["b"] = configuration->off.color.blue;

    root["timeout"] = configuration->timeout;
    root["name"] = configuration->name;

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_CLED_EFFECT_WAVE,
                        jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}

void AFEDataAccess::createCLEDEffectWaveConfigurationFile() {
  CLED_EFFECT_WAVE configuration;
  configuration.on.color.red = AFE_CONFIG_HARDWARE_CLED_DEFAULT_ON_COLOR;
  configuration.on.color.green = AFE_CONFIG_HARDWARE_CLED_DEFAULT_ON_COLOR;
  configuration.on.color.blue = AFE_CONFIG_HARDWARE_CLED_DEFAULT_ON_COLOR;
  configuration.on.brightness =
      AFE_CONFIG_HARDWARE_CLED_EFFECT_WAVE_DEFAULT_BRIGHTNESS;
  configuration.off.color.red = AFE_CONFIG_HARDWARE_CLED_DEFAULT_OFF_COLOR;
  configuration.off.color.green = AFE_CONFIG_HARDWARE_CLED_DEFAULT_OFF_COLOR;
  configuration.off.color.blue = AFE_CONFIG_HARDWARE_CLED_DEFAULT_OFF_COLOR;
  configuration.timeout =
      AFE_CONFIG_HARDWARE_CLED_EFFECT_WAVE_DEFAULT_WAVE_TIMEOUT;

#ifdef DEBUG
  Serial << endl << F("INFO: Creating CLED EFFECT Wave configuration file");
#endif

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_CLED_STRIPS; i++) {
    sprintf(configuration.name, "WAVE-%d", i + 1);
    saveConfiguration(i, &configuration);
  }
}

/* RGB LED Fade In/Out Effect */
boolean AFEDataAccess::getConfiguration(uint8_t id,
                                        CLED_EFFECT_FADE_INOUT *configuration) {
  boolean _ret = true;
  char fileName[strlen(AFE_FILE_CLED_EFFECT_FADE_INOUT_CONFIGURATION) + 1];
  sprintf(fileName, AFE_FILE_CLED_EFFECT_FADE_INOUT_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  if (!LittleFS.exists(fileName)) {
    createCLEDConfigurationFile();
  }
  File configFile = LittleFS.open(fileName, "r");
#else
  File configFile = SPIFFS.open(fileName, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_CLED_EFFECT_FADE_INOUT> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      configuration->in.color.red = root["c"]["r"].as<int>();
      configuration->in.color.green = root["c"]["g"].as<int>();
      configuration->in.color.blue = root["c"]["b"].as<int>();
      configuration->in.brightness = root["maxL"].as<int>();
      configuration->out.brightness = root["minL"].as<int>();
      configuration->timeout = root["timeout"].as<int>();
      sprintf(configuration->name, root["name"]);

#ifdef DEBUG
      printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_CLED_EFFECT_FADE_INOUT,
                          jsonBuffer.size());
#endif
    } else {
      _ret = false;
#ifdef DEBUG
      Serial << F("ERROR: JSON not pharsed");

#endif
    }
    configFile.close();
  } else {
    _ret = false;
#ifdef DEBUG
    Serial << endl
           << F("ERROR: Configuration file: ") << fileName << F(" not opened");
#endif
  }
  return _ret;
}
void AFEDataAccess::saveConfiguration(uint8_t id,
                                      CLED_EFFECT_FADE_INOUT *configuration) {
  char fileName[strlen(AFE_FILE_CLED_EFFECT_FADE_INOUT_CONFIGURATION) + 1];
  sprintf(fileName, AFE_FILE_CLED_EFFECT_FADE_INOUT_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "w");
#else
  File configFile = SPIFFS.open(fileName, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_CLED_EFFECT_FADE_INOUT> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    JsonObject &cOn = root.createNestedObject("c");

    cOn["r"] = configuration->in.color.red;
    cOn["g"] = configuration->in.color.green;
    cOn["b"] = configuration->in.color.blue;
    root["maxL"] = configuration->in.brightness;
    root["minL"] = configuration->out.brightness;

    root["timeout"] = configuration->timeout;
    root["name"] = configuration->name;

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif

    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_CLED_EFFECT_FADE_INOUT,
                        jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}

void AFEDataAccess::createCLEDEffectFadeInOutConfigurationFile() {
  CLED_EFFECT_FADE_INOUT configuration;
  configuration.in.color.red = AFE_CONFIG_HARDWARE_CLED_DEFAULT_ON_COLOR;
  configuration.in.color.green = AFE_CONFIG_HARDWARE_CLED_DEFAULT_ON_COLOR;
  configuration.in.color.blue = AFE_CONFIG_HARDWARE_CLED_DEFAULT_ON_COLOR;
  configuration.in.brightness =
      AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT_DEFAULT_IN_BRIGHTNESS;
  configuration.out.brightness =
      AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT_DEFAULT_OUT_BRIGHTNESS;
  configuration.timeout =
      AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT_DEFAULT_FADE_TIMEOUT;

#ifdef DEBUG
  Serial << endl
         << F("INFO: Creating CLED EFFECT FadeInOut configuration file");
#endif

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_CLED_STRIPS; i++) {
    sprintf(configuration.name, "FADE-%d", i + 1);
    saveConfiguration(i, &configuration);
  }
}
/* @TODO T5
#ifdef AFE_CONFIG_HARDWARE_CLED_LIGHT_CONTROLLED_EFFECT
boolean AFEDataAccess::getConfiguration(uint8_t id,
                                        CLED_BACKLIGHT *configuration) {
  boolean _ret = true;
  char fileName[28];
  sprintf(fileName, AFE_FILE_CLED_BACKLIGHT_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "r");
#else
  File configFile = SPIFFS.open(fileName, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_CLED_BACKIGHT> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      configuration->lightSensorId = root["lightSensorId"].as<int>();

      for (uint8_t i = 0;
           i < AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_BACKLIGHT_LEVELS; i++) {

        configuration->config[i].luxLevel =
            root["configs"][i]["luxLevel"].as<int>();
        configuration->config[i].brightness =
            root["configs"][i]["brightness"].as<int>();
        configuration->config[i].color = root["configs"][i]["color"].as<int>();
        sprintf(configuration->name, root["name"]);
      }

#ifdef DEBUG
      Serial << endl
             << F("INFO: JSON: Buffer size: ")
             << AFE_CONFIG_FILE_BUFFER_CLED_BACKIGHT
             << F(", actual JSON size: ") << jsonBuffer.size();
      if (AFE_CONFIG_FILE_BUFFER_CLED_BACKIGHT < jsonBuffer.size() + 10) {
        Serial << endl << F("WARN: Too small buffer size");
      }
#endif
    } else {
      _ret = false;
#ifdef DEBUG
      Serial << F("ERROR: JSON not pharsed");

#endif
    }
    configFile.close();
  } else {
    _ret = false;
#ifdef DEBUG
    Serial << endl
           << F("ERROR: Configuration file: ") << fileName << F(" not opened");
#endif
  }
  return _ret;
}
void AFEDataAccess::saveConfiguration(uint8_t id,
                                      CLED_BACKLIGHT *configuration) {
  char fileName[28];
  sprintf(fileName, AFE_FILE_CLED_BACKLIGHT_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "w");
#else
  File configFile = SPIFFS.open(fileName, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_CLED_BACKIGHT> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    JsonArray &configs = root.createNestedArray("configs");
    JsonObject &l1 = configs.createNestedObject();
    JsonObject &l2 = configs.createNestedObject();
    JsonObject &l3 = configs.createNestedObject();

    root["lightSensorId"] = configuration->lightSensorId;

    l1["luxLevel"] = configuration->config[0].luxLevel;
    l1["color"] = configuration->config[0].color;
    l1["brightness"] = configuration->config[0].brightness;

    l2["luxLevel"] = configuration->config[1].luxLevel;
    l2["color"] = configuration->config[1].color;
    l2["brightness"] = configuration->config[1].brightness;

    l3["luxLevel"] = configuration->config[2].luxLevel;
    l3["color"] = configuration->config[2].color;
    l3["brightness"] = configuration->config[2].brightness;

    root["name"] = configuration->name;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    root["idx"] = configuration->domoticz.idx;
#else
    root["mqttTopic"] = configuration->mqtt.topic;
#endif

    root.printTo(configFile);

#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    Serial << endl
           << F("INFO: Data saved") << endl
           << F("INFO: JSON: Buffer size: ")
           << AFE_CONFIG_FILE_BUFFER_CLED_BACKIGHT << F(", actual JSON size: ")
           << jsonBuffer.size();
    if (AFE_CONFIG_FILE_BUFFER_CLED_BACKIGHT < jsonBuffer.size() + 10) {
      Serial << endl << F("WARN: Too small buffer size");
    }
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}
void AFEDataAccess::createCLEDBacklightConfigurationFile() {
#ifdef DEBUG
  Serial << endl
         << F("INFO: Creating file: ")
         << F(AFE_FILE_CLED_BACKLIGHT_CONFIGURATION);
#endif
  CLED_BACKLIGHT data;

  data.lightSensorId = AFE_HARDWARE_ITEM_NOT_EXIST;
  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_BACKLIGHT_LEVELS;
       i++) {

    data.config[i].luxLevel = 0;
    data.config[i].color = AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_COLOR;
    data.config[i].brightness = AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_BRIGHTNESS;
  }

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_CLED_STRIPS; i++) {
#ifdef DEBUG
    Serial << endl
           << F("INFO: Creating file: cfg-cled-backlight-") << i << F(".json");
#endif

    saveConfiguration(i, &data);
  }
}

#endif // AFE_CONFIG_HARDWARE_CLED_LIGHT_CONTROLLED_EFFECT
*/
#endif // AFE_CONFIG_HARDWARE_CLED

#ifdef AFE_CONFIG_HARDWARE_TSL2561
boolean AFEDataAccess::getConfiguration(uint8_t id, TSL2561 *configuration) {
  boolean _ret = true;
  char fileName[22];
  sprintf(fileName, AFE_FILE_TSL2561_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "r");
#else
  File configFile = SPIFFS.open(fileName, "r");
#endif
  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_TSL2561> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());

    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      sprintf(configuration->name, root["name"] | "");
      configuration->sensitiveness =
          root["sensitiveness"] |
          AFE_CONFIG_HARDWARE_TSL2561_DEFAULT_SENSITIVENESS;
      configuration->interval =
          root["interval"] | AFE_CONFIG_HARDWARE_TSL2561_DEFAULT_INTERVAL;
      configuration->gain =
          root["gain"] | AFE_CONFIG_HARDWARE_TSL2561_DEFAULT_GAIN;

#ifdef AFE_ESP32
      configuration->wirePortId =
          root["wirePortId"] | AFE_HARDWARE_ITEM_NOT_EXIST;
#endif

      configuration->i2cAddress =
          root["i2cAddress"] |
          AFE_CONFIG_HARDWARE_I2C_DEFAULT_NON_EXIST_ADDRESS;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
      configuration->domoticz.ir.idx =
          root["domoticz"]["ir"] | AFE_DOMOTICZ_DEFAULT_IDX;
      configuration->domoticz.illuminance.idx =
          root["domoticz"]["illuminance"] | AFE_DOMOTICZ_DEFAULT_IDX;
      configuration->domoticz.broadband.idx =
          root["domoticz"]["broadband"] | AFE_DOMOTICZ_DEFAULT_IDX;
#else
      sprintf(configuration->mqtt.topic, root["mqttTopic"] | "");
#endif

#ifdef DEBUG
      printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_TSL2561, jsonBuffer.size());
#endif
    } else {
      _ret = false;
#ifdef DEBUG
      Serial << F("ERROR: JSON not pharsed");
#endif
    }
    configFile.close();
  } else {
    _ret = false;
#ifdef DEBUG
    Serial << endl
           << F("ERROR: Configuration file: ") << fileName << F(" not opened");
#endif
  }

  return _ret;
}
void AFEDataAccess::saveConfiguration(uint8_t id, TSL2561 *configuration) {
  char fileName[22];
  sprintf(fileName, AFE_FILE_TSL2561_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "w");
#else
  File configFile = SPIFFS.open(fileName, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_TSL2561> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    JsonObject &domoticz = root.createNestedObject("domoticz");
#endif

    root["name"] = configuration->name;
    root["sensitiveness"] = configuration->sensitiveness;
    root["gain"] = configuration->gain;
    root["interval"] = configuration->interval;

#ifdef AFE_ESP32
    root["wirePortId"] = configuration->wirePortId;
#endif

    root["i2cAddress"] = configuration->i2cAddress;

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    domoticz["ir"] = configuration->domoticz.ir.idx;
    domoticz["illuminance"] = configuration->domoticz.illuminance.idx;
    domoticz["broadband"] = configuration->domoticz.broadband.idx;
#else
    root["mqttTopic"] = configuration->mqtt.topic;
#endif

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_TSL2561, jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}
void AFEDataAccess::createTSL2561SensorConfigurationFile() {
  TSL2561 configuration;
  configuration.interval = AFE_CONFIG_HARDWARE_TSL2561_DEFAULT_INTERVAL;
  configuration.i2cAddress = AFE_CONFIG_HARDWARE_I2C_DEFAULT_NON_EXIST_ADDRESS;
  configuration.gain = AFE_CONFIG_HARDWARE_TSL2561_DEFAULT_GAIN;
  configuration.sensitiveness =
      AFE_CONFIG_HARDWARE_TSL2561_DEFAULT_SENSITIVENESS;

#ifdef AFE_ESP32
  configuration.wirePortId = AFE_HARDWARE_ITEM_NOT_EXIST;
#endif

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  configuration.domoticz.illuminance.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  configuration.domoticz.broadband.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  configuration.domoticz.ir.idx = AFE_DOMOTICZ_DEFAULT_IDX;
#else
  configuration.mqtt.topic[0] = AFE_EMPTY_STRING;
#endif

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_TSL2561; i++) {
#ifdef DEBUG
    Serial << endl << F("INFO: Creating file: cfg-tls2561-") << i << F(".json");
#endif

    sprintf(configuration.name, "TSL2561-%d", i + 1);
    saveConfiguration(i, &configuration);
  }
}
#endif // AFE_CONFIG_HARDWARE_TSL2561

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
boolean AFEDataAccess::getConfiguration(uint8_t id, MCP23XXX *configuration) {
  boolean _ret = true;
  char fileName[sizeof(AFE_FILE_MCP23XXX_CONFIGURATION)];
  sprintf(fileName, AFE_FILE_MCP23XXX_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "r");
#else
  File configFile = SPIFFS.open(fileName, "r");
#endif
  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_MCP23XXX> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());

    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      sprintf(configuration->name, root["name"] | "");
      configuration->address = root["address"] | AFE_HARDWARE_ITEM_NOT_EXIST;

#ifdef AFE_ESP32
      configuration->wirePortId =
          root["wirePortId"] | AFE_HARDWARE_ITEM_NOT_EXIST;
#endif

#ifdef DEBUG
      printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_MCP23XXX, jsonBuffer.size());
#endif
    } else {
      _ret = false;
#ifdef DEBUG
      Serial << F("ERROR: JSON not pharsed");
#endif
    }
    configFile.close();
  } else {
    _ret = false;
#ifdef DEBUG
    Serial << endl
           << F("ERROR: Configuration file: ") << fileName << F(" not opened");
#endif
  }

  return _ret;
}

void AFEDataAccess::saveConfiguration(uint8_t id, MCP23XXX *configuration) {
  char fileName[sizeof(AFE_FILE_MCP23XXX_CONFIGURATION)];
  sprintf(fileName, AFE_FILE_MCP23XXX_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "w");
#else
  File configFile = SPIFFS.open(fileName, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_MCP23XXX> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();

    root["name"] = configuration->name;
    root["address"] = configuration->address;

#ifdef AFE_ESP32
    root["wirePortId"] = configuration->wirePortId;
#endif

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_TSL2561, jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}
void AFEDataAccess::createMCP23XXXConfigurationFile() {
  MCP23XXX configuration;
  configuration.address = AFE_HARDWARE_ITEM_NOT_EXIST;

#ifdef AFE_ESP32
  configuration.wirePortId = AFE_HARDWARE_ITEM_NOT_EXIST;
#endif

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_MCP23017; i++) {
#ifdef DEBUG
    Serial << endl
           << F("INFO: Creating file: cfg-mcp23xxx-") << i << F(".json");
#endif

    sprintf(configuration.name, "MCP23XXX-%d", i + 1);
    saveConfiguration(i, &configuration);
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_FS3000
boolean AFEDataAccess::getConfiguration(uint8_t id,
                                        FS3000_CONFIG *configuration) {
  boolean _ret = true;
  char fileName[sizeof(AFE_FILE_FS3000_CONFIGURATION)];
  sprintf(fileName, AFE_FILE_FS3000_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "r");
#else
  File configFile = SPIFFS.open(fileName, "r");
#endif
  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_FS3000> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());

    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      sprintf(configuration->name, root["n"] | "");
      configuration->range =
          root["rg"] | AFE_CONFIG_HARDWARE_FS3000_DEFAULT_RANGE;
      configuration->r = root["r"] | AFE_CONFIG_HARDWARE_FS3000_DEFAULT_R;

      configuration->interval =
          root["i"] | AFE_CONFIG_HARDWARE_FS3000_DEFAULT_INTERVAL;

#ifdef AFE_ESP32
      configuration->wirePortId = root["w"] | AFE_HARDWARE_ITEM_NOT_EXIST;
#endif

      configuration->i2cAddress =
          root["a"] | AFE_CONFIG_HARDWARE_I2C_DEFAULT_NON_EXIST_ADDRESS;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
      configuration->domoticz.raw.idx =
          root["d"]["1"] | AFE_DOMOTICZ_DEFAULT_IDX;
      configuration->domoticz.meterPerSecond.idx =
          root["d"]["2"] | AFE_DOMOTICZ_DEFAULT_IDX;
      configuration->domoticz.milesPerHour.idx =
          root["d"]["3"] | AFE_DOMOTICZ_DEFAULT_IDX;
      configuration->domoticz.meters3PerHour.idx =
          root["d"]["4"] | AFE_DOMOTICZ_DEFAULT_IDX;

#else
      sprintf(configuration->mqtt.topic, root["t"] | "");
#endif

#ifdef DEBUG
      printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_FS3000, jsonBuffer.size());
#endif
    } else {
      _ret = false;
#ifdef DEBUG
      Serial << F("ERROR: JSON not pharsed");
#endif
    }
    configFile.close();
  } else {
    _ret = false;
#ifdef DEBUG
    Serial << endl
           << F("ERROR: Configuration file: ") << fileName << F(" not opened");
#endif
  }

  return _ret;
}
void AFEDataAccess::saveConfiguration(uint8_t id,
                                      FS3000_CONFIG *configuration) {
  char fileName[sizeof(AFE_FILE_FS3000_CONFIGURATION)];
  sprintf(fileName, AFE_FILE_FS3000_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "w");
#else
  File configFile = SPIFFS.open(fileName, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_FS3000> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    JsonObject &domoticz = root.createNestedObject("d");
#endif

    root["n"] = configuration->name;
    root["rg"] = configuration->range;
    root["r"] = configuration->r;
    root["i"] = configuration->interval;

#ifdef AFE_ESP32
    root["w"] = configuration->wirePortId;
#endif

    root["a"] = configuration->i2cAddress;

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    domoticz["1"] = configuration->domoticz.raw.idx;
    domoticz["2"] = configuration->domoticz.meterPerSecond.idx;
    domoticz["3"] = configuration->domoticz.milesPerHour.idx;
    domoticz["4"] = configuration->domoticz.meters3PerHour.idx;
#else
    root["t"] = configuration->mqtt.topic;
#endif

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_CONFIG_FILE_BUFFER_FS3000, jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}
void AFEDataAccess::createFS3000SensorConfigurationFile() {
  FS3000_CONFIG configuration;
  configuration.interval = AFE_CONFIG_HARDWARE_FS3000_DEFAULT_INTERVAL;
  configuration.i2cAddress = AFE_CONFIG_HARDWARE_I2C_DEFAULT_NON_EXIST_ADDRESS;
  configuration.range = AFE_CONFIG_HARDWARE_FS3000_DEFAULT_RANGE;
  configuration.r = AFE_CONFIG_HARDWARE_FS3000_DEFAULT_R;

#ifdef AFE_ESP32
  configuration.wirePortId = AFE_HARDWARE_ITEM_NOT_EXIST;
#endif

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  configuration.domoticz.raw.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  configuration.domoticz.meterPerSecond.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  configuration.domoticz.milesPerHour.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  configuration.domoticz.meters3PerHour.idx = AFE_DOMOTICZ_DEFAULT_IDX;
#else
  configuration.mqtt.topic[0] = AFE_EMPTY_STRING;
#endif

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_TSL2561; i++) {
#ifdef DEBUG
    Serial << endl << F("INFO: Creating file: cfg-fs3000-") << i << F(".json");
#endif

    sprintf(configuration.name, "FS3000-%d", i + 1);
    saveConfiguration(i, &configuration);
  }
}
#endif // AFE_CONFIG_HARDWARE_FS3000

unsigned long AFEDataAccess::getRebootCounter(boolean increase) {
  unsigned long _ret = 1;
  char fileName[strlen(AFE_FILE_REBOOTS_COUNTER) + 1];
  sprintf(fileName, AFE_FILE_REBOOTS_COUNTER);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  if (!LittleFS.exists(fileName)) {
    saveRebootCounter(0);
  }
  File configFile = LittleFS.open(fileName, "r");
#else
  if (!SPIFFS.exists(fileName)) {
    saveRebootCounter(0);
  }
  File configFile = SPIFFS.open(fileName, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_FILE_BUFFER_REBOOTS_COUNTER> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      _ret = root["reboots"].as<unsigned long>();

#ifdef DEBUG
      Serial << endl
             << F("INFO: JSON: Buffer size: ")
             << AFE_FILE_BUFFER_REBOOTS_COUNTER << F(", actual JSON size: ")
             << jsonBuffer.size();
      if (AFE_FILE_BUFFER_REBOOTS_COUNTER < jsonBuffer.size() + 10) {
        Serial << endl << F("WARN: Too small buffer size");
      }
#endif
    } else {
      _ret = 0;
#ifdef DEBUG
      Serial << F("ERROR: JSON not pharsed");

#endif
    }
    configFile.close();
  } else {
    _ret = 0;
#ifdef DEBUG
    Serial << endl
           << F("ERROR: Configuration file: ") << fileName << F(" not opened");
#endif
  }
  if (increase) {
    saveRebootCounter(_ret + 1);
  }
  return _ret;
}

void AFEDataAccess::saveRebootCounter(unsigned long counter) {
  char fileName[strlen(AFE_FILE_REBOOTS_COUNTER) + 1];
  sprintf(fileName, AFE_FILE_REBOOTS_COUNTER);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "w");
#else
  File configFile = SPIFFS.open(fileName, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_FILE_BUFFER_REBOOTS_COUNTER> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();

    root["reboots"] = counter;

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    printBufforSizeInfo(AFE_FILE_BUFFER_REBOOTS_COUNTER, jsonBuffer.size());
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif
}

/* Private methods */

#ifdef DEBUG
void AFEDataAccess::printBufforSizeInfo(uint16_t bufferSize,
                                        uint16_t jsonSize) {
  Serial << endl
         << F("INFO: JSON: Buffer size: ") << bufferSize
         << F(", actual JSON size: ") << jsonSize;
  if (bufferSize < jsonSize + 10) {
    Serial << endl << F("WARN: BUFFER TOO SMALL");
  }
}
#endif

boolean AFEDataAccess::initializeFileSystem() {
  boolean _ret;
#ifdef DEBUG
  Serial << endl << F("INFO: FILES SYSTEM: Mounting file system...");
#endif

#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  _ret = LittleFS.begin();
#else
  _ret = SPIFFS.begin();
  if (_ret) {
    yield();
    SPIFFS.gc();
  }
#endif

#ifdef DEBUG
  if (_ret) {
    Serial << F(" Success");
  } else {
    Serial << F(" FAILURE") << endl;
  }
#endif

  return _ret;
}

boolean AFEDataAccess::setDefaultConfiguration() {
  boolean _ret = false;
/* Turning devicve LED on */
#ifdef AFE_CONFIG_HARDWARE_LED
#ifdef DEBUG
  Serial << endl
         << F("Turning on system LED on GPIO ")
         << AFE_CONFIG_HARDWARE_LED_0_DEFAULT_GPIO;
#endif
  pinMode(AFE_CONFIG_HARDWARE_LED_0_DEFAULT_GPIO, OUTPUT);
  digitalWrite(AFE_CONFIG_HARDWARE_LED_0_DEFAULT_GPIO, LOW);
#endif

  if (formatFileSystem()) {

    /* Initializatin of the FS */
    initializeFileSystem();

    saveWelecomeMessage("");
#ifdef AFE_CONFIG_HARDWARE_GATE
    createGateConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
    createLEDConfigurationFile();
    createSystemLedIDConfigurationFile();
#endif

    createDeviceConfigurationFile();
    createFirmwareConfigurationFile();
    createNetworkConfigurationFile();
    createMQTTConfigurationFile();
    createProVersionConfigurationFile();
    createPasswordConfigurationFile();
    /* Setting device mode to Access Point */
    saveDeviceMode(AFE_MODE_NETWORK_NOT_SET);

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    createDomoticzConfigurationFile();
#elif AFE_FIRMWARE_API == AFE_FIRMWARE_API_HOME_ASSISTANT
    createHomeAssistantConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
    createRelayConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
    createSwitchConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
    createADCInputConfigurationFile();
#endif

#if defined(T3_CONFIG)
    PIR PIRConfiguration;
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
    createContractonConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
    createHPMA115S0SensorConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_UART
    createSerialConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_I2C
    createI2CConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
    createBMEX80SensorConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
    createBH1750SensorConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
    createAS3935SensorConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
    createAnemometerSensorConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
    createRainmeterSensorConfigurationFile();
    createRainmeterSensorDataConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
    createDS18B20SensorConfigurationFile();
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
    createRegulatorConfigurationFile();
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
    createThermalProtectorConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_DHT
    createDHTSensorConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
    createBinarySensorConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
    createPN532ConfigurationFile();
    createMiFareCardConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_CLED
    createCLEDConfigurationFile();
    createCLEDEffectBlinkingConfigurationFile();
    createCLEDEffectFadeInOutConfigurationFile();
    createCLEDEffectWaveConfigurationFile();
#endif // AFE_CONFIG_HARDWARE_CLED

#ifdef AFE_CONFIG_HARDWARE_TSL2561
    createTSL2561SensorConfigurationFile();
#endif // AFE_CONFIG_HARDWARE_TSL2561

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
    createMCP23XXXConfigurationFile();
#endif // AFE_CONFIG_HARDWARE_MCP23XXX

    _ret = true;
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: Formating failed");
  }
#endif
  return _ret;
}
