/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Data-Access.h"

AFEDataAccess::AFEDataAccess() {}

boolean AFEDataAccess::formatFileSystem() {
#ifdef DEBUG
  Serial << endl << endl << F("INFO: Formatig File System");
#endif
#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
  return LittleFS.format();
#else
  return SPIFFS.format();
#endif
}

boolean AFEDataAccess::fileExist(const char *path) {
#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
  return LittleFS.exists(path);
#else
  return SPIFFS.exists(path);
#endif
}

const String AFEDataAccess::getDeviceUID() {
  String uid;
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << AFE_FILE_DEVICE_UID << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(AFE_FILE_DEVICE_UID, "r");
#else
  File configFile = SPIFFS.open(AFE_FILE_DEVICE_UID, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_DEVICE_UID> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());

    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif
      uid = root.get<char *>("uid");
#ifdef DEBUG
      Serial << endl
             << F("INFO: JSON: Buffer size: ")
             << AFE_CONFIG_FILE_BUFFER_DEVICE_UID << F(", actual JSON size: ")
             << jsonBuffer.size();
      if (AFE_CONFIG_FILE_BUFFER_DEVICE_UID < jsonBuffer.size() + 10) {
        Serial << endl << F("WARN: Too small buffer size");
      }
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
           << F("ERROR: Configuration file: ") << AFE_FILE_DEVICE_UID
           << F(" not opened");
  }
#endif
  return uid;
}

void AFEDataAccess::saveDeviceUID(const char *uid) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << AFE_FILE_DEVICE_UID << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(AFE_FILE_DEVICE_UID, "w");
#else
  File configFile = SPIFFS.open(AFE_FILE_DEVICE_UID, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_DEVICE_UID> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["uid"] = uid;
    root.printTo(configFile);

#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    Serial << endl
           << F("INFO: Data saved") << endl
           << F("INFO: JSON: Buffer size: ")
           << AFE_CONFIG_FILE_BUFFER_DEVICE_UID << F(", actual JSON size: ")
           << jsonBuffer.size();
    if (AFE_CONFIG_FILE_BUFFER_DEVICE_UID < jsonBuffer.size() + 10) {
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
void AFEDataAccess::createDeviceUIDFile() {
#ifdef DEBUG
  Serial << endl << F("INFO: Creating file: ") << AFE_FILE_DEVICE_UID;
#endif

  byte m[6];
  char uid[18];
  WiFi.macAddress(m);
  sprintf(uid, "%X%x%X%x-%X%x%X%x", m[0], m[5], m[1], m[4], m[2], m[3], m[3],
          m[2]);
  saveDeviceUID(uid);
}

void AFEDataAccess::getConfiguration(PRO_VERSION *configuration) {

#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << F(AFE_FILE_PRO_VERSION_CONFIGURATION)
         << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
      Serial << endl
             << F("INFO: JSON: Buffer size: ")
             << AFE_CONFIG_FILE_BUFFER_PRO_VERSION << F(", actual JSON size: ")
             << jsonBuffer.size();
      if (AFE_CONFIG_FILE_BUFFER_PRO_VERSION < jsonBuffer.size() + 10) {
        Serial << endl << F("WARN: Too small buffer size");
      }
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
           << AFE_FILE_PRO_VERSION_CONFIGURATION << F(" not opened");
  }
#endif
}
void AFEDataAccess::saveConfiguration(PRO_VERSION *configuration) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << AFE_FILE_PRO_VERSION_CONFIGURATION
         << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
    Serial << endl
           << F("INFO: Data saved") << endl
           << F("INFO: JSON: Buffer size: ")
           << AFE_CONFIG_FILE_BUFFER_PRO_VERSION << F(", actual JSON size: ")
           << jsonBuffer.size();

    if (AFE_CONFIG_FILE_BUFFER_PRO_VERSION < jsonBuffer.size() + 10) {
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
         << F("INFO: Opening file: ") << AFE_FILE_PASSWORD_CONFIGURATION
         << F(" ... ");
#endif
#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
      Serial << endl
             << F("INFO: JSON: Buffer size: ")
             << AFE_CONFIG_FILE_BUFFER_PASSWORD << F(", actual JSON size: ")
             << jsonBuffer.size();
      if (AFE_CONFIG_FILE_BUFFER_PASSWORD < jsonBuffer.size() + 10) {
        Serial << endl << F("WARN: Too small buffer size");
      }
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
           << AFE_FILE_PASSWORD_CONFIGURATION << F(" not opened");
  }
#endif
}
void AFEDataAccess::saveConfiguration(PASSWORD *configuration) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << AFE_FILE_PASSWORD_CONFIGURATION
         << F(" ... ");
#endif
#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
    Serial << endl
           << F("INFO: Data saved") << endl
           << F("INFO: JSON: Buffer size: ") << AFE_CONFIG_FILE_BUFFER_PASSWORD
           << F(", actual JSON size: ") << jsonBuffer.size();
    if (AFE_CONFIG_FILE_BUFFER_PASSWORD < jsonBuffer.size() + 10) {
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
void AFEDataAccess::createPasswordConfigurationFile() {
#ifdef DEBUG
  Serial << endl
         << F("INFO: Creating file: ") << AFE_FILE_PASSWORD_CONFIGURATION;
#endif
  PASSWORD PasswordConfiguration;
  PasswordConfiguration.protect = false;
  PasswordConfiguration.password[0] = AFE_EMPTY_STRING;
  saveConfiguration(&PasswordConfiguration);
}

void AFEDataAccess::getConfiguration(DEVICE *configuration) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << AFE_FILE_DEVICE_CONFIGURATION
         << F(" ... ");
#endif
#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
      configuration->api.domoticz = root["api"]["domoticz"] | false;
      configuration->api.domoticzVersion =
          root["api"]["domoticzVersion"] | AFE_DOMOTICZ_VERSION_DEFAULT;
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
#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
      configuration->isAnalogInput = root["isAnalogInput"] | false;
#endif

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

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER_SENSOR
      configuration->noOfAnemometerSensors =
          root["noOfAnemometerSensors"] |
          AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_ANEMOMETER_SENSORS;
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER_SENSOR
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

#ifdef DEBUG
      Serial << endl
             << F("INFO: JSON: Buffer size: ") << AFE_CONFIG_FILE_BUFFER_DEVICE
             << F(", actual JSON size: ") << jsonBuffer.size();
      if (AFE_CONFIG_FILE_BUFFER_DEVICE < jsonBuffer.size() + 10) {
        Serial << endl << F("WARN: Too small buffer size");
      }
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
           << F("ERROR: Configuration file: ") << AFE_FILE_DEVICE_CONFIGURATION
           << F(" not opened");
  }
#endif
}
void AFEDataAccess::saveConfiguration(DEVICE *configuration) {

#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << AFE_FILE_DEVICE_CONFIGURATION
         << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
    jsonAPI["domoticz"] = configuration->api.domoticz;
    jsonAPI["domoticzVersion"] = configuration->api.domoticzVersion;
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

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
    root["isAnalogInput"] = configuration->isAnalogInput;
#endif

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

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER_SENSOR
    root["noOfAnemometerSensors"] = configuration->noOfAnemometerSensors;
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER_SENSOR
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

    root.printTo(configFile);

#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    Serial << endl
           << F("INFO: Data saved") << endl
           << F("INFO: JSON: Buffer size: ") << AFE_CONFIG_FILE_BUFFER_DEVICE
           << F(", actual JSON size: ") << jsonBuffer.size();
    if (AFE_CONFIG_FILE_BUFFER_DEVICE < jsonBuffer.size() + 10) {
      Serial << endl << F("WARN: Too small buffer size");
    }
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif

// Removing connection between relay and a gate, if exists reseting changing the
// number of gates
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
  Serial << endl << F("INFO: Creating file: ") << AFE_FILE_DEVICE_CONFIGURATION;
#endif
  DEVICE deviceConfiguration;
  sprintf(deviceConfiguration.name, "AFE-Device");
  deviceConfiguration.timeToAutoLogOff = AFE_AUTOLOGOFF_DEFAULT_TIME;
  /* APIs */
  deviceConfiguration.api.mqtt = false;
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  deviceConfiguration.api.domoticz = false;
  deviceConfiguration.api.domoticzVersion = AFE_DOMOTICZ_VERSION_DEFAULT;
#endif
  deviceConfiguration.api.http = true;

/* Relay presence */
#ifdef AFE_CONFIG_HARDWARE_RELAY
  deviceConfiguration.noOfRelays = AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_RELAYS;
#endif

  /* Switch presence */
  deviceConfiguration.noOfSwitches =
      AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_SWITCHES;

/* LEDs presence */
#ifdef AFE_CONFIG_HARDWARE_LED
  deviceConfiguration.noOfLEDs = AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_LEDS;
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  deviceConfiguration.isAnalogInput = false;
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  deviceConfiguration.noOfContactrons =
      AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_CONTACTRONS;
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
  deviceConfiguration.noOfGates = AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_GATES;
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  deviceConfiguration.noOfHPMA115S0s =
      AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_HPMA115S0;
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  deviceConfiguration.noOfBMEX80s =
      AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_BMEX80;
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
  deviceConfiguration.noOfBH1750s =
      AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_BH1750;
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
  deviceConfiguration.noOfAS3935s =
      AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_AS3935;
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
  deviceConfiguration.noOfDS18B20s =
      AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_DS18B20;
#endif

#ifdef AFE_CONFIG_HARDWARE_DHT
  deviceConfiguration.noOfDHTs = AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_DHT;
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER_SENSOR
  deviceConfiguration.noOfAnemometerSensors =
      AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_ANEMOMETER_SENSORS;
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER_SENSOR
  deviceConfiguration.noOfRainmeterSensors =
      AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_RAINMETER_SENSORS;
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  deviceConfiguration.noOfRegulators =
      AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_REGULATORS;
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
  deviceConfiguration.noOfThermalProtectors =
      AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_THERMAL_PROTECTIORS;
#endif

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
  deviceConfiguration.noOfBinarySensors =
      AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_BINARY_SENSORS;
#endif
  saveConfiguration(&deviceConfiguration);
}

void AFEDataAccess::getConfiguration(FIRMWARE *configuration) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << AFE_FILE_FIRMWARE_CONFIGURATION
         << F(" ... ");
#endif
#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
      sprintf(configuration->version, root["version"]);
      sprintf(configuration->upgradeURL, root["upgradeURL"]);
      configuration->autoUpgrade = root["autoUpgrade"];

#ifdef DEBUG
      Serial << endl
             << F("INFO: JSON: Buffer size: ")
             << AFE_CONFIG_FILE_BUFFER_FIRMWARE << F(", actual JSON size: ")
             << jsonBuffer.size();
      if (AFE_CONFIG_FILE_BUFFER_FIRMWARE < jsonBuffer.size() + 10) {
        Serial << endl << F("WARN: Too small buffer size");
      }
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
           << AFE_FILE_FIRMWARE_CONFIGURATION << F(" not opened");
  }
#endif
}
void AFEDataAccess::saveConfiguration(FIRMWARE *configuration) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << AFE_FILE_FIRMWARE_CONFIGURATION
         << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
    root["version"] = configuration->version;
    root["autoUpgrade"] = configuration->autoUpgrade;
    root["upgradeURL"] = configuration->upgradeURL;
    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    Serial << endl
           << F("INFO: Data saved") << endl
           << F("INFO: JSON: Buffer size: ") << AFE_CONFIG_FILE_BUFFER_FIRMWARE
           << F(", actual JSON size: ") << jsonBuffer.size();
    if (AFE_CONFIG_FILE_BUFFER_FIRMWARE < jsonBuffer.size() + 10) {
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
void AFEDataAccess::createFirmwareConfigurationFile() {
#ifdef DEBUG
  Serial << endl
         << F("INFO: Creating file: ") << AFE_FILE_FIRMWARE_CONFIGURATION;
#endif
  FIRMWARE firmwareConfiguration;
  sprintf(firmwareConfiguration.version, AFE_FIRMWARE_VERSION);
  firmwareConfiguration.type = AFE_FIRMWARE_TYPE;
  firmwareConfiguration.api = AFE_FIRMARE_API;
  firmwareConfiguration.autoUpgrade = 0;
  firmwareConfiguration.upgradeURL[0] = AFE_EMPTY_STRING;
  saveConfiguration(&firmwareConfiguration);
}

void AFEDataAccess::saveFirmwareVersion(const char *version) {
  FIRMWARE configuration;
  getConfiguration(&configuration);
  sprintf(configuration.version, version);
  saveConfiguration(&configuration);
}

void AFEDataAccess::saveFirmwareAPIVersion() {
  FIRMWARE configuration;
  getConfiguration(&configuration);
  configuration.api = AFE_FIRMARE_API;
  saveConfiguration(&configuration);
}

uint8_t AFEDataAccess::getDeviceMode() {
  uint8_t mode = AFE_MODE_FIRST_TIME_LAUNCH;
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << AFE_FILE_DEVICE_MODE << F(" ... ");
#endif
#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
      Serial << endl
             << F("INFO: JSON: Buffer size: ")
             << AFE_CONFIG_FILE_BUFFER_DEVICE_MODE << F(", actual JSON size: ")
             << jsonBuffer.size();
      if (AFE_CONFIG_FILE_BUFFER_DEVICE_MODE < jsonBuffer.size() + 10) {
        Serial << endl << F("WARN: Too small buffer size");
      }
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
           << F("ERROR: Configuration file: ") << AFE_FILE_DEVICE_MODE
           << F(" not opened");
  }
#endif
  return mode;
}
void AFEDataAccess::saveDeviceMode(uint8_t mode) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << AFE_FILE_DEVICE_MODE << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
    Serial << endl
           << F("INFO: Data saved") << endl
           << F("INFO: JSON: Buffer size: ")
           << AFE_CONFIG_FILE_BUFFER_DEVICE_MODE << F(", actual JSON size: ")
           << jsonBuffer.size();
    if (AFE_CONFIG_FILE_BUFFER_DEVICE_MODE < jsonBuffer.size() + 10) {
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

void AFEDataAccess::getConfiguration(NETWORK *configuration) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << AFE_FILE_NETWORK_CONFIGURATION
         << F(" ... ");
#endif
#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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

      sprintf(configuration->ssid, root["ssid"]);
      sprintf(configuration->password, root["password"]);

      configuration->isDHCP = root["isDHCP"];

      sprintf(configuration->ip, root["ip"]);
      sprintf(configuration->gateway, root["gateway"]);
      sprintf(configuration->subnet, root["subnet"]);

      configuration->noConnectionAttempts = root["noConnectionAttempts"];
      configuration->waitTimeConnections = root["waitTimeConnections"];
      configuration->waitTimeSeries = root["waitTimeSeries"];

#ifdef DEBUG
      Serial << endl
             << F("INFO: JSON: Buffer size: ") << AFE_CONFIG_FILE_BUFFER_NETWORK
             << F(", actual JSON size: ") << jsonBuffer.size();
      if (AFE_CONFIG_FILE_BUFFER_NETWORK < jsonBuffer.size() + 10) {
        Serial << endl << F("WARN: Too small buffer size");
      }
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
           << F("ERROR: Configuration file: ") << AFE_FILE_NETWORK_CONFIGURATION
           << F(" not opened");
  }
#endif
}
void AFEDataAccess::saveConfiguration(NETWORK *configuration) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << AFE_FILE_NETWORK_CONFIGURATION
         << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
    root["ssid"] = configuration->ssid;
    root["password"] = configuration->password;
    root["isDHCP"] = configuration->isDHCP;
    root["ip"] = configuration->ip;
    root["gateway"] = configuration->gateway;
    root["subnet"] = configuration->subnet;

    root["noConnectionAttempts"] = configuration->noConnectionAttempts;
    root["waitTimeConnections"] = configuration->waitTimeConnections;
    root["waitTimeSeries"] = configuration->waitTimeSeries;

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    Serial << endl
           << F("INFO: Data saved") << endl
           << F("INFO: JSON: Buffer size: ") << AFE_CONFIG_FILE_BUFFER_NETWORK
           << F(", actual JSON size: ") << jsonBuffer.size();
    if (AFE_CONFIG_FILE_BUFFER_NETWORK < jsonBuffer.size() + 10) {
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
void AFEDataAccess::createNetworkConfigurationFile() {
#ifdef DEBUG
  Serial << endl
         << F("INFO: Creating file: ") << AFE_FILE_NETWORK_CONFIGURATION;
#endif

  NETWORK networkConfiguration;
  /* Network default config */
  networkConfiguration.ssid[0] = AFE_EMPTY_STRING;
  networkConfiguration.password[0] = AFE_EMPTY_STRING;
  networkConfiguration.isDHCP = true;
  networkConfiguration.ip[0] = AFE_EMPTY_STRING;
  networkConfiguration.gateway[0] = AFE_EMPTY_STRING;
  networkConfiguration.subnet[0] = AFE_EMPTY_STRING;
  networkConfiguration.noConnectionAttempts = 30;
  networkConfiguration.waitTimeConnections = 1;
  networkConfiguration.waitTimeSeries = 20;
  saveConfiguration(&networkConfiguration);
}

void AFEDataAccess::getConfiguration(MQTT *configuration) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << AFE_FILE_MQTT_BROKER_CONFIGURATION
         << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
      configuration->lwt.idx = root["lwt"] | AFE_DOMOTICZ_DEFAULT_IDX;
#else
      sprintf(configuration->lwt.topic, root["lwt"] | "");
#endif
      configuration->timeout =
          root["timeout"] | AFE_CONFIG_MQTT_DEFAULT_TIMEOUT;
      configuration->retainLWT =
          root["retainLWT"] | AFE_CONFIG_MQTT_DEFAULT_RETAIN_LWT;
      configuration->retainAll =
          root["retainAll"] | AFE_CONFIG_MQTT_DEFAULT_RETAIN_ALL;
      configuration->pingHostBeforeConnection =
          root["pingHostBeforeConnection"] |
          AFE_CONFIG_MQTT_DEFAULT_HOST_PING_BEFORE_CONNECTION;

#ifdef DEBUG
      Serial << endl
             << F("INFO: JSON: Buffer size: ")
             << AFE_CONFIG_FILE_BUFFER_MQTT_BROKER << F(", actual JSON size: ")
             << jsonBuffer.size();
      if (AFE_CONFIG_FILE_BUFFER_MQTT_BROKER < jsonBuffer.size()) {
        Serial << endl << F("WARN: Too small buffer size");
      }
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
           << AFE_FILE_MQTT_BROKER_CONFIGURATION << F(" not opened");
  }
#endif
}
void AFEDataAccess::saveConfiguration(MQTT *configuration) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << AFE_FILE_MQTT_BROKER_CONFIGURATION
         << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
    root["lwt"] = configuration->lwt.idx;
#else
    root["lwt"] = configuration->lwt.topic;
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

    root["timeout"] = configuration->timeout;
    root["retainAll"] = configuration->retainAll;
    root["retainLWT"] = configuration->retainLWT;
    root["pingHostBeforeConnection"] = configuration->pingHostBeforeConnection;

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    Serial << endl
           << F("INFO: Data saved") << endl
           << F("INFO: JSON: Buffer size: ")
           << AFE_CONFIG_FILE_BUFFER_MQTT_BROKER << F(", actual JSON size: ")
           << jsonBuffer.size();
    if (AFE_CONFIG_FILE_BUFFER_MQTT_BROKER < jsonBuffer.size()) {
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
void AFEDataAccess::createMQTTConfigurationFile() {
#ifdef DEBUG
  Serial << endl
         << F("INFO: Creating file: ") << AFE_FILE_MQTT_BROKER_CONFIGURATION;
#endif
  MQTT configuration;
  /* MQTT Default config */
  configuration.host[0] = AFE_EMPTY_STRING;
  configuration.ip[0] = AFE_EMPTY_STRING;
  configuration.user[0] = AFE_EMPTY_STRING;
  configuration.password[0] = AFE_EMPTY_STRING;
  configuration.port = AFE_CONFIG_MQTT_DEFAULT_PORT;
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  configuration.lwt.idx = AFE_DOMOTICZ_DEFAULT_IDX;
#else
  configuration.lwt.topic[0] = AFE_EMPTY_STRING;
#endif
  configuration.timeout = AFE_CONFIG_MQTT_DEFAULT_TIMEOUT;
  configuration.retainAll = AFE_CONFIG_MQTT_DEFAULT_RETAIN_LWT;
  configuration.retainLWT = AFE_CONFIG_MQTT_DEFAULT_RETAIN_ALL;
  configuration.pingHostBeforeConnection =
      AFE_CONFIG_MQTT_DEFAULT_HOST_PING_BEFORE_CONNECTION;
  saveConfiguration(&configuration);
}

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
void AFEDataAccess::getConfiguration(DOMOTICZ *configuration) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << AFE_FILE_DOMOTICZ_CONFIGURATION
         << F(" ... ");
#endif
#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
    StaticJsonBuffer<98> jsonBuffer;
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
      Serial << endl
             << F("INFO: JSON: Buffer size: ")
             << AFE_CONFIG_FILE_BUFFER_MQTT_BROKER << F(", actual JSON size: ")
             << jsonBuffer.size();
      if (AFE_CONFIG_FILE_BUFFER_MQTT_BROKER < jsonBuffer.size() + 10) {
        Serial << endl << F("WARN: Too small buffer size");
      }
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
           << AFE_FILE_DOMOTICZ_CONFIGURATION << F(" not opened");
  }
#endif
}

void AFEDataAccess::saveConfiguration(DOMOTICZ *configuration) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << AFE_FILE_DOMOTICZ_CONFIGURATION
         << F(" ... ");
#endif
#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(AFE_FILE_DOMOTICZ_CONFIGURATION, "w");
#else
  File configFile = SPIFFS.open(AFE_FILE_DOMOTICZ_CONFIGURATION, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<211> jsonBuffer;
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
    Serial << endl
           << F("INFO: Data saved") << endl
           << F("INFO: JSON: Buffer size: ")
           << AFE_CONFIG_FILE_BUFFER_MQTT_BROKER << F(", actual JSON size: ")
           << jsonBuffer.size();
    if (AFE_CONFIG_FILE_BUFFER_MQTT_BROKER < jsonBuffer.size() + 10) {
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
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

#ifdef AFE_CONFIG_HARDWARE_LED
void AFEDataAccess::getConfiguration(uint8_t id, LED *configuration) {
  char fileName[16];
  sprintf(fileName, AFE_FILE_LED_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
#ifdef AFE_CONFIG_HARDWARE_MCP23017
      configuration->mcp23017.address = root["mcp23017"]["address"];
      configuration->mcp23017.gpio = root["mcp23017"]["gpio"];
#endif

#ifdef DEBUG
      Serial << endl
             << F("INFO: JSON: Buffer size: ") << AFE_CONFIG_FILE_BUFFER_LED
             << F(", actual JSON size: ") << jsonBuffer.size();
      if (AFE_CONFIG_FILE_BUFFER_LED < jsonBuffer.size() + 10) {
        Serial << endl << F("WARN: Too small buffer size");
      }
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
  char fileName[16];
  sprintf(fileName, AFE_FILE_LED_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "w");
#else
#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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

#ifdef AFE_CONFIG_HARDWARE_MCP23017
    JsonObject &mcp23017 = root.createNestedObject("mcp23017");
    mcp23017["address"] = configuration->mcp23017.address;
    mcp23017["gpio"] = configuration->mcp23017.gpio;
#endif // AFE_CONFIG_HARDWARE_MCP23017

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    Serial << endl
           << F("INFO: Data saved") << endl
           << F("INFO: JSON: Buffer size: ") << AFE_CONFIG_FILE_BUFFER_LED
           << F(", actual JSON size: ") << jsonBuffer.size();
    if (AFE_CONFIG_FILE_BUFFER_LED < jsonBuffer.size() + 10) {
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
#elif defined(AFE_DEVICE_iECSv20)
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

#ifdef AFE_CONFIG_HARDWARE_MCP23017
  LEDConfiguration.mcp23017.address =
      AFE_CONFIG_HARDWARE_I2C_DEFAULT_NON_EXIST_ADDRESS;
  LEDConfiguration.mcp23017.gpio = AFE_HARDWARE_ITEM_NOT_EXIST;
#endif // AFE_CONFIG_HARDWARE_MCP23017

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

#ifdef AFE_CONFIG_HARDWARE_MCP23017
  LEDConfiguration.mcp23017.address =
      AFE_CONFIG_HARDWARE_I2C_DEFAULT_NON_EXIST_ADDRESS;
  LEDConfiguration.mcp23017.gpio = AFE_HARDWARE_ITEM_NOT_EXIST;
#endif // AFE_CONFIG_HARDWARE_MCP23017

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
         << F("INFO: Opening file: ") << AFE_FILE_SYSTEM_LED_CONFIGURATION
         << F(" ... ");
#endif
#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
      Serial << endl
             << F("INFO: JSON: Buffer size: ")
             << AFE_CONFIG_FILE_BUFFER_SYSTEM_LED << F(", actual JSON size: ")
             << jsonBuffer.size();
      if (AFE_CONFIG_FILE_BUFFER_SYSTEM_LED < jsonBuffer.size() + 10) {
        Serial << endl << F("WARN: Too small buffer size");
      }
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
         << F("INFO: Opening file: ") << AFE_FILE_SYSTEM_LED_CONFIGURATION
         << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
    Serial << endl
           << F("INFO: Data saved") << endl
           << F("INFO: JSON: Buffer size: ")
           << AFE_CONFIG_FILE_BUFFER_SYSTEM_LED << F(", actual JSON size: ")
           << jsonBuffer.size();
    if (AFE_CONFIG_FILE_BUFFER_SYSTEM_LED < jsonBuffer.size() + 10) {
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
void AFEDataAccess::createSystemLedIDConfigurationFile() {
#ifdef DEBUG
  Serial << endl
         << F("INFO: Creating file: ") << AFE_FILE_SYSTEM_LED_CONFIGURATION;
#endif
  saveSystemLedID(0);
}
#endif // AFE_CONFIG_HARDWARE_LED

#ifdef AFE_CONFIG_HARDWARE_RELAY

void AFEDataAccess::getConfiguration(uint8_t id, RELAY *configuration) {
  char fileName[18];
  sprintf(fileName, AFE_FILE_RELAY_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS

  // if (!LittleFS.exists(fileName)) {
  //    createRelayConfigurationFile(id);
  // }

  File configFile = LittleFS.open(fileName, "r");
#else

  //  if (!SPIFFS.exists(fileName)) {
  //    createRelayConfigurationFile(id);
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

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
      configuration->domoticz.idx = root["idx"] | AFE_DOMOTICZ_DEFAULT_IDX;
#else
      sprintf(configuration->mqtt.topic, root["MQTTTopic"] | "");
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
      configuration->ledID = root["ledID"];
#endif

#ifdef AFE_CONFIG_HARDWARE_MCP23017
      configuration->mcp23017.address = root["mcp23017"]["address"];
      configuration->mcp23017.gpio = root["mcp23017"]["gpio"];
#endif

#ifdef DEBUG

      Serial << endl
             << F("INFO: JSON: Buffer size: ") << AFE_CONFIG_FILE_BUFFER_RELAY
             << F(", actual JSON size: ") << jsonBuffer.size();
      if (AFE_CONFIG_FILE_BUFFER_RELAY < jsonBuffer.size() + 10) {
        Serial << endl << F("WARN: Too small buffer size");
      }
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
void AFEDataAccess::saveConfiguration(uint8_t id, RELAY *configuration) {
  char fileName[18];
  sprintf(fileName, AFE_FILE_RELAY_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
    root["idx"] = configuration->domoticz.idx;
#else
    root["MQTTTopic"] = configuration->mqtt.topic;
#endif

#ifdef AFE_CONFIG_HARDWARE_MCP23017
    JsonObject &mcp23017 = root.createNestedObject("mcp23017");
    mcp23017["address"] = configuration->mcp23017.address;
    mcp23017["gpio"] = configuration->mcp23017.gpio;
#endif // AFE_CONFIG_HARDWARE_MCP23017

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    Serial << endl
           << F("INFO: Data saved") << endl
           << F("INFO: JSON: Buffer size: ") << AFE_CONFIG_FILE_BUFFER_RELAY
           << F(", actual JSON size: ") << jsonBuffer.size();
    if (AFE_CONFIG_FILE_BUFFER_RELAY < jsonBuffer.size() + 10) {
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
void AFEDataAccess::createRelayConfigurationFile() {

  RELAY RelayConfiguration;
  uint8_t index = 0;
/* Relay config */
#ifdef AFE_CONFIG_HARDWARE_LED
  RelayConfiguration.ledID = AFE_HARDWARE_ITEM_NOT_EXIST;
#endif

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  RelayConfiguration.domoticz.idx = AFE_DOMOTICZ_DEFAULT_IDX;
#else
  RelayConfiguration.mqtt.topic[0] = AFE_EMPTY_STRING;
#endif
  RelayConfiguration.state.MQTTConnected =
      AFE_CONFIG_HARDWARE_RELAY_DEFAULT_STATE_MQTT_CONNECTED;

#ifndef AFE_DEVICE_iECSv20
  RelayConfiguration.timeToOff = AFE_CONFIG_HARDWARE_RELAY_DEFAULT_TIME_TO_OFF;
  RelayConfiguration.state.powerOn =
      AFE_CONFIG_HARDWARE_RELAY_DEFAULT_STATE_POWER_ON;
#endif

  RelayConfiguration.triggerSignal =
      AFE_CONFIG_HARDWARE_RELAY_DEFAULT_SIGNAL_TRIGGER;

#ifdef AFE_CONFIG_HARDWARE_MCP23017
  RelayConfiguration.mcp23017.address =
      AFE_CONFIG_HARDWARE_I2C_DEFAULT_NON_EXIST_ADDRESS;
  RelayConfiguration.mcp23017.gpio = AFE_HARDWARE_ITEM_NOT_EXIST;
#endif // AFE_CONFIG_HARDWARE_MCP23017

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
#elif defined(AFE_DEVICE_iECSv20)
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
/*
void AFEDataAccess::createRelayConfigurationFile(uint8_t id) {
  RELAY RelayConfiguration;
// Relay config
#ifdef AFE_CONFIG_HARDWARE_LED
  RelayConfiguration.ledID = AFE_HARDWARE_ITEM_NOT_EXIST;
#endif

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  RelayConfiguration.domoticz.idx = AFE_DOMOTICZ_DEFAULT_IDX;
#else
  RelayConfiguration.mqtt.topic[0] = AFE_EMPTY_STRING;
#endif
  RelayConfiguration.state.MQTTConnected =
      AFE_CONFIG_HARDWARE_RELAY_DEFAULT_STATE_MQTT_CONNECTED;

  RelayConfiguration.timeToOff = AFE_CONFIG_HARDWARE_RELAY_DEFAULT_TIME_TO_OFF;
  RelayConfiguration.state.powerOn =
      AFE_CONFIG_HARDWARE_RELAY_DEFAULT_STATE_POWER_ON;

  RelayConfiguration.triggerSignal =
      AFE_CONFIG_HARDWARE_RELAY_DEFAULT_SIGNAL_TRIGGER;

#ifdef AFE_CONFIG_HARDWARE_MCP23017
  RelayConfiguration.mcp23017.address =
      AFE_CONFIG_HARDWARE_I2C_DEFAULT_NON_EXIST_ADDRESS;
  RelayConfiguration.mcp23017.gpio = AFE_HARDWARE_ITEM_NOT_EXIST;
#endif // AFE_CONFIG_HARDWARE_MCP23017

  // Adding config files for remaining relays
  RelayConfiguration.gpio = AFE_CONFIG_HARDWARE_RELAY_0_DEFAULT_GPIO;
#ifdef DEBUG
  Serial << endl << F("INFO: Creating file: cfg-relay-") << id << F(".json");
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY
  saveRelayState(id, false);
#endif
  sprintf(RelayConfiguration.name, "R%d", id + 1);
  saveConfiguration(id, &RelayConfiguration);
}
*/
/* Relay state methods*/
boolean AFEDataAccess::getRelayState(uint8_t id) {
  boolean state = false;
  char fileName[23];
  sprintf(fileName, AFE_FILE_RELAY_STATE_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
      Serial << endl
             << F("INFO: JSON: Buffer size: ")
             << AFE_CONFIG_FILE_BUFFER_RELAY_STATE << F(", actual JSON size: ")
             << jsonBuffer.size();
      if (AFE_CONFIG_FILE_BUFFER_RELAY_STATE < jsonBuffer.size() + 10) {
        Serial << endl << F("WARN: Too small buffer size");
      }
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
  char fileName[23];
  sprintf(fileName, AFE_FILE_RELAY_STATE_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
    Serial << endl
           << F("INFO: Data saved") << endl
           << F("INFO: JSON: Buffer size: ")
           << AFE_CONFIG_FILE_BUFFER_RELAY_STATE << F(", actual JSON size: ")
           << jsonBuffer.size();
    if (AFE_CONFIG_FILE_BUFFER_RELAY_STATE < jsonBuffer.size() + 10) {
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
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_SWITCH
void AFEDataAccess::getConfiguration(uint8_t id, SWITCH *configuration) {
  char fileName[19];
  sprintf(fileName, AFE_FILE_SWITCH_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
      sprintf(configuration->mqtt.topic, root["MQTTTopic"] | "");
#else
      configuration->domoticz.idx = root["idx"] | AFE_DOMOTICZ_DEFAULT_IDX;
#endif

#ifdef AFE_CONFIG_HARDWARE_MCP23017
      configuration->mcp23017.address = root["mcp23017"]["address"];
      configuration->mcp23017.gpio = root["mcp23017"]["gpio"];
#endif

#ifdef DEBUG
      Serial << endl
             << F("INFO: JSON: Buffer size: ") << AFE_CONFIG_FILE_BUFFER_SWITCH
             << F(", actual JSON size: ") << jsonBuffer.size();
      if (AFE_CONFIG_FILE_BUFFER_SWITCH < jsonBuffer.size() + 10) {
        Serial << endl << F("WARN: Too small buffer size");
      }
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
void AFEDataAccess::saveConfiguration(uint8_t id, SWITCH *configuration) {
  char fileName[19];
  sprintf(fileName, AFE_FILE_SWITCH_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
    root["idx"] = configuration->domoticz.idx;
#else
    root["MQTTTopic"] = configuration->mqtt.topic;
#endif

#ifdef AFE_CONFIG_HARDWARE_MCP23017
    JsonObject &mcp23017 = root.createNestedObject("mcp23017");
    mcp23017["address"] = configuration->mcp23017.address;
    mcp23017["gpio"] = configuration->mcp23017.gpio;
#endif // AFE_CONFIG_HARDWARE_MCP23017

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    Serial << endl
           << F("INFO: Data saved") << endl
           << F("INFO: JSON: Buffer size: ") << AFE_CONFIG_FILE_BUFFER_SWITCH
           << F(", actual JSON size: ") << jsonBuffer.size();
    if (AFE_CONFIG_FILE_BUFFER_SWITCH < jsonBuffer.size() + 10) {
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
#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  SwitchConfiguration.mqtt.topic[0] = AFE_EMPTY_STRING;
#else
  SwitchConfiguration.domoticz.idx = AFE_DOMOTICZ_DEFAULT_IDX;
#endif
  /* Saving first switch. Common for all devices */
  SwitchConfiguration.gpio = AFE_HARDWARE_SWITCH_0_DEFAULT_GPIO;
  SwitchConfiguration.type = AFE_HARDWARE_SWITCH_0_DEFAULT_TYPE;
  SwitchConfiguration.functionality =
      AFE_HARDWARE_SWITCH_0_DEFAULT_FUNCTIONALITY;

#ifdef AFE_CONFIG_HARDWARE_MCP23017
  SwitchConfiguration.mcp23017.address =
      AFE_CONFIG_HARDWARE_I2C_DEFAULT_NON_EXIST_ADDRESS;
  SwitchConfiguration.mcp23017.gpio = AFE_HARDWARE_ITEM_NOT_EXIST;
#endif // AFE_CONFIG_HARDWARE_MCP23017

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
#elif defined(AFE_DEVICE_iECSv20)
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
/*
void AFEDataAccess::createSwitchConfigurationFile(uint8_t id) {
  SWITCH SwitchConfiguration;
  SwitchConfiguration.sensitiveness = AFE_HARDWARE_SWITCH_DEFAULT_BOUNCING;
#ifdef AFE_CONFIG_HARDWARE_RELAY
  SwitchConfiguration.relayID = AFE_HARDWARE_ITEM_NOT_EXIST;
#endif

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  SwitchConfiguration.mqtt.topic[0] = AFE_EMPTY_STRING;
#else
  SwitchConfiguration.domoticz.idx = AFE_DOMOTICZ_DEFAULT_IDX;
#endif

  // Saving first switch. Common for all devices
  SwitchConfiguration.gpio = AFE_HARDWARE_SWITCH_0_DEFAULT_GPIO;
  SwitchConfiguration.type = AFE_HARDWARE_SWITCH_0_DEFAULT_TYPE;
  SwitchConfiguration.functionality =
      AFE_HARDWARE_SWITCH_0_DEFAULT_FUNCTIONALITY;

#ifdef AFE_CONFIG_HARDWARE_MCP23017
  SwitchConfiguration.mcp23017.address =
      AFE_CONFIG_HARDWARE_I2C_DEFAULT_NON_EXIST_ADDRESS;
  SwitchConfiguration.mcp23017.gpio = AFE_HARDWARE_ITEM_NOT_EXIST;
#endif // AFE_CONFIG_HARDWARE_MCP23017

#ifdef DEBUG
  Serial << endl << F("INFO: Creating file: cfg-switch-") << id << F(".json");
#endif
  saveConfiguration(id, &SwitchConfiguration);
}
*/
#endif // AFE_CONFIG_HARDWARE_SWITCH

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
void AFEDataAccess::getConfiguration(ADCINPUT *configuration) {

#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << AFE_FILE_ADC_CONFIGURATION
         << F(" ... ");
#endif
#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(AFE_FILE_ADC_CONFIGURATION, "r");
#else
  File configFile = SPIFFS.open(AFE_FILE_ADC_CONFIGURATION, "r");
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
      configuration->gpio = root["gpio"];
      configuration->interval = root["interval"];
      configuration->numberOfSamples = root["numberOfSamples"];
      configuration->maxVCC = root["maxVCC"];
#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
      sprintf(configuration->mqtt.topic, root["mqttTopic"] | "");
#else
      configuration->domoticz.raw =
          root["idx"]["raw"] | AFE_DOMOTICZ_DEFAULT_IDX;
      configuration->domoticz.percent =
          root["idx"]["percent"] | AFE_DOMOTICZ_DEFAULT_IDX;
      configuration->domoticz.voltage =
          root["idx"]["voltage"] | AFE_DOMOTICZ_DEFAULT_IDX;
      configuration->domoticz.voltageCalculated =
          root["idx"]["voltageCalculated"] | AFE_DOMOTICZ_DEFAULT_IDX;
#endif
      configuration->divider.Ra = root["divider"]["Ra"];
      configuration->divider.Rb = root["divider"]["Rb"];

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
      configuration->battery.maxVoltage =
          root["batteryMeter"]["maxV"].as<float>();
      configuration->battery.minVoltage =
          root["batteryMeter"]["minV"].as<float>();
#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
      sprintf(configuration->battery.mqtt.topic,
              root["batteryMeter"]["mqttTopic"] | "");
#else
      configuration->battery.domoticz.idx =
          root["batteryMeter"]["idx"] | AFE_DOMOTICZ_DEFAULT_IDX;
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
#endif // AFE_CONFIG_FUNCTIONALITY_BATTERYMETER

#ifdef DEBUG
      Serial << endl
             << F("INFO: JSON: Buffer size: ") << AFE_CONFIG_FILE_BUFFER_ADC
             << F(", actual JSON size: ") << jsonBuffer.size();
      if (AFE_CONFIG_FILE_BUFFER_ADC < jsonBuffer.size() + 10) {
        Serial << endl << F("WARN: Too small buffer size");
      }
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
           << F("ERROR: Configuration file: ") << AFE_FILE_ADC_CONFIGURATION
           << F(" not opened");
  }
#endif
}

void AFEDataAccess::saveConfiguration(ADCINPUT *configuration) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << AFE_FILE_ADC_CONFIGURATION
         << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(AFE_FILE_ADC_CONFIGURATION, "w");
#else
  File configFile = SPIFFS.open(AFE_FILE_ADC_CONFIGURATION, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_ADC> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
    JsonObject &idx = root.createNestedObject("idx");
#endif
    JsonObject &divider = root.createNestedObject("divider");

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
    JsonObject &battery = root.createNestedObject("batteryMeter");
#endif

    root["gpio"] = configuration->gpio;
    root["interval"] = configuration->interval;
    root["numberOfSamples"] = configuration->numberOfSamples;
    root["maxVCC"] = configuration->maxVCC;
#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
    root["mqttTopic"] = configuration->mqtt.topic;
#else
    idx["raw"] = configuration->domoticz.raw;
    idx["percent"] = configuration->domoticz.percent;
    idx["voltage"] = configuration->domoticz.voltage;
    idx["voltageCalculated"] = configuration->domoticz.voltageCalculated;
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
    divider["Ra"] = configuration->divider.Ra;
    divider["Rb"] = configuration->divider.Rb;

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
    battery["maxV"] = configuration->battery.maxVoltage;
    battery["minV"] = configuration->battery.minVoltage;
#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
    battery["mqttTopic"] = configuration->battery.mqtt.topic;
#else
    battery["idx"] = configuration->battery.domoticz.idx;
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
#endif // AFE_CONFIG_FUNCTIONALITY_BATTERYMETER

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    Serial << endl
           << F("INFO: Data saved") << endl
           << F("INFO: JSON: Buffer size: ") << AFE_CONFIG_FILE_BUFFER_ADC
           << F(", actual JSON size: ") << jsonBuffer.size();
    if (AFE_CONFIG_FILE_BUFFER_ADC < jsonBuffer.size() + 10) {
      Serial << endl << F("WARN: Too small buffer size");
    }
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
  Serial << endl << F("INFO: Creating file: ") << AFE_FILE_ADC_CONFIGURATION;
#endif
  ADCINPUT AnalogInputConfiguration;
  AnalogInputConfiguration.gpio = AFE_CONFIG_HARDWARE_ADC_VCC_DEFAULT_GPIO;
  AnalogInputConfiguration.interval =
      AFE_CONFIG_HARDWARE_ADC_VCC_DEFAULT_INTERVAL;
  AnalogInputConfiguration.numberOfSamples =
      AFE_CONFIG_HARDWARE_ADC_VCC_DEFAULT_NUMBER_OF_SAMPLES;
  AnalogInputConfiguration.maxVCC = AFE_CONFIG_HARDWARE_ADC_VCC_DEFAULT_MAX_VCC;
#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  AnalogInputConfiguration.mqtt.topic[0] = AFE_EMPTY_STRING;
#else
  AnalogInputConfiguration.domoticz.raw = 0;
  AnalogInputConfiguration.domoticz.voltage = 0;
  AnalogInputConfiguration.domoticz.percent = 0;
  AnalogInputConfiguration.domoticz.voltageCalculated = 0;
#endif
  AnalogInputConfiguration.divider.Ra = 0;
  AnalogInputConfiguration.divider.Rb = 0;

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
  AnalogInputConfiguration.battery.minVoltage =
      AFE_CONFIG_HARDWARE_ADC_VCC_DEFAULT_BATTER_MIN_V;
  AnalogInputConfiguration.battery.maxVoltage =
      AFE_CONFIG_HARDWARE_ADC_VCC_DEFAULT_BATTER_MAX_V;
#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  AnalogInputConfiguration.battery.mqtt.topic[0] = AFE_EMPTY_STRING;
#else
  AnalogInputConfiguration.battery.domoticz.idx = AFE_DOMOTICZ_DEFAULT_IDX;
#endif
#endif // AFE_CONFIG_FUNCTIONALITY_BATTERYMETER

  saveConfiguration(&AnalogInputConfiguration);
}
#endif //  AFE_CONFIG_HARDWARE_ADC_VCC

#ifdef AFE_CONFIG_HARDWARE_DS18B20
void AFEDataAccess::getConfiguration(uint8_t id, DS18B20 *configuration) {

  char fileName[20];
  sprintf(fileName, AFE_FILE_DS18B20_SENSOR_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << "INFO: Opening file: " << fileName << " ... ";
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "r");
#else
  File configFile = SPIFFS.open(fileName, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << "success" << endl << "INFO: JSON: ";
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
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
      configuration->domoticz.idx = root["idx"] | AFE_DOMOTICZ_DEFAULT_IDX;
#else
      sprintf(configuration->mqtt.topic, root["mqttTopic"] | "");
#endif

      for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_DS18B20_ADDRESS_LENGTH; i++) {
        configuration->address[i] = root["address"][i].as<int>();
      }

#ifdef DEBUG
      Serial << endl
             << "INFO: JSON: Buffer size: " << AFE_CONFIG_FILE_BUFFER_DS18B20
             << ", actual JSON size: " << jsonBuffer.size();
      if (AFE_CONFIG_FILE_BUFFER_DS18B20 < jsonBuffer.size() + 10) {
        Serial << endl << "WARN: Too small buffer size";
      }
#endif
    }
#ifdef DEBUG
    else {
      Serial << "ERROR: JSON not pharsed";
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else {
    Serial << endl
           << "ERROR: Configuration file: " << fileName << " not opened";
  }
#endif
}
void AFEDataAccess::saveConfiguration(uint8_t id, DS18B20 *configuration) {

  char fileName[20];
  sprintf(fileName, AFE_FILE_DS18B20_SENSOR_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << "INFO: Opening file: " << fileName << " ... ";
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "w");
#else
  File configFile = SPIFFS.open(fileName, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << "success" << endl << "INFO: Writing JSON: ";
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

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
    root["mqttTopic"] = configuration->mqtt.topic;
#else
    root["idx"] = configuration->domoticz.idx;
#endif
    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    Serial << endl
           << "INFO: Data saved" << endl
           << "INFO: JSON: Buffer size: " << AFE_CONFIG_FILE_BUFFER_DS18B20
           << ", actual JSON size: " << jsonBuffer.size();
    if (AFE_CONFIG_FILE_BUFFER_DS18B20 < jsonBuffer.size() + 10) {
      Serial << endl << "WARN: Too small buffer size";
    }
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << "ERROR: failed to open file for writing";
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
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  configuration.domoticz.idx = AFE_DOMOTICZ_DEFAULT_IDX;
#endif

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_DS18B20; i++) {
#ifdef DEBUG
    Serial << endl << "INFO: Creating file: /cfg-ds18b20-" << i << ".json";
#endif
#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
    configuration.mqtt.topic[0] = AFE_EMPTY_STRING;
#endif
    sprintf(configuration.name, "DS18B20-%d", i + 1);
    saveConfiguration(i, &configuration);
  }
}

#endif // AFE_CONFIG_HARDWARE_DS18B20

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
void AFEDataAccess::getConfiguration(uint8_t id, CONTACTRON *configuration) {
  char fileName[22];
  sprintf(fileName, AFE_FILE_CONTACTRON_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
      configuration->ledID = root["ledID"];
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
      configuration->domoticz.idx = root["idx"] | AFE_DOMOTICZ_DEFAULT_IDX;
#else
      sprintf(configuration->mqtt.topic, root["MQTTTopic"] | "");
#endif

#ifdef DEBUG
      Serial << endl
             << F("INFO: JSON: Buffer size: ")
             << AFE_CONFIG_FILE_BUFFER_CONTACTRON << F(", actual JSON size: ")
             << jsonBuffer.size();
      if (AFE_CONFIG_FILE_BUFFER_CONTACTRON < jsonBuffer.size() + 10) {
        Serial << endl << F("WARN: Too small buffer size");
      }
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
  char fileName[22];
  sprintf(fileName, AFE_FILE_CONTACTRON_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
    root["ledID"] = configuration->ledID;
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
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
    Serial << endl
           << F("INFO: Data saved") << endl
           << F("INFO: JSON: Buffer size: ")
           << AFE_CONFIG_FILE_BUFFER_CONTACTRON << F(", actual JSON size: ")
           << jsonBuffer.size();
    if (AFE_CONFIG_FILE_BUFFER_CONTACTRON < jsonBuffer.size() + 10) {
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

void AFEDataAccess::createContractonConfigurationFile() {
  CONTACTRON ContactronConfiguration;
  uint8 index = 0;
  ContactronConfiguration.bouncing =
      AFE_CONFIG_HARDWARE_CONTACTRON_DEFAULT_BOUNCING;
  ContactronConfiguration.type =
      AFE_CONFIG_HARDWARE_CONTACTRON_DEFAULT_OUTPUT_TYPE;
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  ContactronConfiguration.domoticz.idx = AFE_DOMOTICZ_DEFAULT_IDX;
#else
  ContactronConfiguration.mqtt.topic[0] = AFE_EMPTY_STRING;
#endif

  ContactronConfiguration.ledID = AFE_HARDWARE_ITEM_NOT_EXIST;
#if defined(AFE_DEVICE_iECSv20)
  ContactronConfiguration.gpio = 14;
  sprintf(ContactronConfiguration.name, "C1");
  saveConfiguration(0, &ContactronConfiguration);
  ContactronConfiguration.gpio = 13;
  sprintf(ContactronConfiguration.name, "C2");
  saveConfiguration(1, &ContactronConfiguration);
  ContactronConfiguration.gpio = 3;
  sprintf(ContactronConfiguration.name, "C3");
  saveConfiguration(2, &ContactronConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_CONTACTRONS;
#endif
  ContactronConfiguration.gpio = 0;
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
  char fileName[16];
  sprintf(fileName, AFE_FILE_GATE_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
      configuration->domoticz.idx = root["idx"] | AFE_DOMOTICZ_DEFAULT_IDX;
      configuration->domoticzControl.idx =
          root["idxControl"] | AFE_DOMOTICZ_DEFAULT_IDX;
#else
      sprintf(configuration->mqtt.topic, root["MQTTTopic"] | "");
#endif

#ifdef DEBUG
      Serial << endl
             << F("INFO: JSON: Buffer size: ") << AFE_CONFIG_FILE_BUFFER_GATE
             << F(", actual JSON size: ") << jsonBuffer.size();
      if (AFE_CONFIG_FILE_BUFFER_GATE < jsonBuffer.size() + 10) {
        Serial << endl << F("WARN: Too small buffer size");
      }
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

  char fileName[16];
  sprintf(fileName, AFE_FILE_GATE_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
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
    Serial << endl
           << F("INFO: Data saved") << endl
           << F("INFO: JSON: Buffer size: ") << AFE_CONFIG_FILE_BUFFER_GATE
           << F(", actual JSON size: ") << jsonBuffer.size();
    if (AFE_CONFIG_FILE_BUFFER_GATE < jsonBuffer.size() + 10) {
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
void AFEDataAccess::createGateConfigurationFile() {

  GATE GateConfiguration;

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  GateConfiguration.domoticz.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  GateConfiguration.domoticzControl.idx = AFE_DOMOTICZ_DEFAULT_IDX;
#else
  GateConfiguration.mqtt.topic[0] = AFE_EMPTY_STRING;
#endif

  GateConfiguration.contactron.id[1] = AFE_HARDWARE_ITEM_NOT_EXIST;

#if defined(AFE_DEVICE_iECSv20)
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

  char fileName[22];
  sprintf(fileName, AFE_FILE_GATE_STATE_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
      Serial << endl
             << F("INFO: JSON: Buffer size: ")
             << AFE_CONFIG_FILE_BUFFER_GATE_STATE << F(", actual JSON size: ")
             << jsonBuffer.size();
      if (AFE_CONFIG_FILE_BUFFER_GATE_STATE < jsonBuffer.size() + 10) {
        Serial << endl << F("WARN: Too small buffer size");
      }
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

  char fileName[22];
  sprintf(fileName, AFE_FILE_GATE_STATE_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
    Serial << endl
           << F("INFO: Data saved") << endl
           << F("INFO: JSON: Buffer size: ")
           << AFE_CONFIG_FILE_BUFFER_GATE_STATE << F(", actual JSON size: ")
           << jsonBuffer.size();
    if (AFE_CONFIG_FILE_BUFFER_GATE_STATE < jsonBuffer.size() + 10) {
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
#endif // AFE_CONFIG_HARDWARE_GATE

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
void AFEDataAccess::getConfiguration(uint8_t id, REGULATOR *configuration) {

  char fileName[23];
  sprintf(fileName, AFE_FILE_REGULATOR_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
      configuration->domoticz.idx = root["idx"] | AFE_DOMOTICZ_DEFAULT_IDX;
#else
      sprintf(configuration->mqtt.topic, root["MQTTTopic"] | "");
#endif

#ifdef DEBUG
      Serial << endl
             << F("INFO: JSON: Buffer size: ")
             << AFE_CONFIG_FILE_BUFFER_REGULATOR << F(", actual JSON size: ")
             << jsonBuffer.size();
      if (AFE_CONFIG_FILE_BUFFER_REGULATOR < jsonBuffer.size() + 10) {
        Serial << endl << F("WARN: Too small buffer size");
      }
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
  char fileName[23];
  sprintf(fileName, AFE_FILE_REGULATOR_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
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
    Serial << endl
           << F("INFO: Data saved") << endl
           << F("INFO: JSON: Buffer size: ") << AFE_CONFIG_FILE_BUFFER_REGULATOR
           << F(", actual JSON size: ") << jsonBuffer.size();
    if (AFE_CONFIG_FILE_BUFFER_REGULATOR < jsonBuffer.size() + 10) {
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

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
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

  char fileName[31];
  sprintf(fileName, AFE_FILE_THERMAL_PROTECTOR_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
      configuration->domoticz.idx = root["idx"] | AFE_DOMOTICZ_DEFAULT_IDX;
#else
      sprintf(configuration->mqtt.topic, root["MQTTTopic"] | "");
#endif

#ifdef DEBUG
      Serial << endl
             << F("INFO: JSON: Buffer size: ")
             << AFE_CONFIG_FILE_BUFFER_THERMAL_PROTECTOR
             << F(", actual JSON size: ") << jsonBuffer.size();
      if (AFE_CONFIG_FILE_BUFFER_THERMAL_PROTECTOR < jsonBuffer.size() + 10) {
        Serial << endl << F("WARN: Too small buffer size");
      }
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
  char fileName[31];
  sprintf(fileName, AFE_FILE_THERMAL_PROTECTOR_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
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
    Serial << endl
           << F("INFO: Data saved") << endl
           << F("INFO: JSON: Buffer size: ")
           << AFE_CONFIG_FILE_BUFFER_THERMAL_PROTECTOR
           << F(", actual JSON size: ") << jsonBuffer.size();
    if (AFE_CONFIG_FILE_BUFFER_THERMAL_PROTECTOR < jsonBuffer.size() + 10) {
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
void AFEDataAccess::createThermalProtectorConfigurationFile(void) {
  THERMAL_PROTECTOR configuration;
  configuration.enabled = AFE_FUNCTIONALITY_THERMAL_PROTECTOR_DEFAULT_ENABLED;
  configuration.relayId = AFE_HARDWARE_ITEM_NOT_EXIST;
  configuration.sensorId = AFE_HARDWARE_ITEM_NOT_EXIST;
  configuration.sensorHardware = AFE_HARDWARE_ITEM_NOT_EXIST;
  configuration.temperature =
      AFE_FUNCTIONALITY_THERMAL_PROTECTOR_DEFAULT_TEMPERATURE;
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
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
  char fileName[21];
  sprintf(fileName, AFE_FILE_HPMA114S0_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
      configuration->domoticz.pm25.idx =
          root["idx"]["pm25"] | AFE_DOMOTICZ_DEFAULT_IDX;
      configuration->domoticz.pm10.idx =
          root["idx"]["pm10"] | AFE_DOMOTICZ_DEFAULT_IDX;
#else
      sprintf(configuration->mqtt.topic, root["mqttTopic"] | "");
#endif
      sprintf(configuration->name, root["name"]);

#ifdef DEBUG
      Serial << endl
             << F("INFO: JSON: Buffer size: ")
             << AFE_CONFIG_FILE_BUFFER_HPMA115S0 << F(", actual JSON size: ")
             << jsonBuffer.size();
      if (AFE_CONFIG_FILE_BUFFER_HPMA115S0 < jsonBuffer.size() + 10) {
        Serial << endl << F("WARN: Too small buffer size");
      }
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
  char fileName[21];
  sprintf(fileName, AFE_FILE_HPMA114S0_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
    JsonObject &idx = root.createNestedObject("idx");
#endif

    root["interval"] = configuration->interval;
    root["timeToMeasure"] = configuration->timeToMeasure;
    root["name"] = configuration->name;

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
    idx["pm25"] = configuration->domoticz.pm25.idx;
    idx["pm10"] = configuration->domoticz.pm10.idx;
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
           << F("INFO: JSON: Buffer size: ") << AFE_CONFIG_FILE_BUFFER_HPMA115S0
           << F(", actual JSON size: ") << jsonBuffer.size();
    if (AFE_CONFIG_FILE_BUFFER_HPMA115S0 < jsonBuffer.size() + 10) {
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
void AFEDataAccess::createHPMA115S0SensorConfigurationFile() {

  HPMA115S0 configuration;

  configuration.interval = AFE_CONFIG_HARDWARE_HPMA115S_DEFAULT_INTERVAL;
  configuration.timeToMeasure =
      AFE_CONFIG_HARDWARE_HPMA115S_DEFAULT_TIME_TO_MEASURE;
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  configuration.domoticz.pm25.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  configuration.domoticz.pm10.idx = AFE_DOMOTICZ_DEFAULT_IDX;
#endif
  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_HPMA115S0; i++) {
#ifdef DEBUG
    Serial << endl
           << F("INFO: Creating file: cfg-hpma115s0-") << i << F(".json");
#endif
#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
    sprintf(configuration.mqtt.topic, "HPMA115S0/%d", i + 1);
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
         << F("INFO: Opening file: ") << AFE_FILE_UART_CONFIGURATION
         << F(" ... ");
#endif
#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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

      configuration->RXD = root["RXD"];
      configuration->TXD = root["TXD"];

#ifdef DEBUG
      Serial << endl
             << F("INFO: JSON: Buffer size: ") << AFE_CONFIG_FILE_BUFFER_UART
             << F(", actual JSON size: ") << jsonBuffer.size();
      if (AFE_CONFIG_FILE_BUFFER_UART < jsonBuffer.size() + 10) {
        Serial << endl << F("WARN: Too small buffer size");
      }
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
           << F("ERROR: Configuration file: ") << AFE_FILE_UART_CONFIGURATION
           << F(" not opened");
  }
#endif
}
void AFEDataAccess::saveConfiguration(SERIALPORT *configuration) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << AFE_FILE_UART_CONFIGURATION
         << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
    root["RXD"] = configuration->RXD;
    root["TXD"] = configuration->TXD;
    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();
#ifdef DEBUG
    Serial << endl
           << F("INFO: Data saved") << endl
           << F("INFO: JSON: Buffer size: ") << AFE_CONFIG_FILE_BUFFER_UART
           << F(", actual JSON size: ") << jsonBuffer.size();
    if (AFE_CONFIG_FILE_BUFFER_UART < jsonBuffer.size() + 10) {
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
void AFEDataAccess::createSerialConfigurationFile() {
#ifdef DEBUG
  Serial << endl << F("INFO: Creating file: ") << AFE_FILE_UART_CONFIGURATION;
#endif
  SERIALPORT configuration;
  configuration.RXD = AFE_CONFIG_HARDWARE_UART_DEFAULT_RXD;
  configuration.TXD = AFE_CONFIG_HARDWARE_UART_DEFAULT_TXD;
  saveConfiguration(&configuration);
}
#endif // AFE_CONFIG_HARDWARE_UART

#ifdef AFE_CONFIG_HARDWARE_I2C
void AFEDataAccess::getConfiguration(I2CPORT *configuration) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << AFE_FILE_I2C_CONFIGURATION
         << F(" ... ");
#endif
#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(AFE_FILE_I2C_CONFIGURATION, "r");
#else
  File configFile = SPIFFS.open(AFE_FILE_I2C_CONFIGURATION, "r");
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

#ifdef DEBUG
      Serial << endl
             << F("INFO: JSON: Buffer size: ") << AFE_CONFIG_FILE_BUFFER_I2C
             << F(", actual JSON size: ") << jsonBuffer.size();
      if (AFE_CONFIG_FILE_BUFFER_I2C < jsonBuffer.size() + 10) {
        Serial << endl << F("WARN: Too small buffer size");
      }
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
           << F("ERROR: Configuration file: ") << AFE_FILE_I2C_CONFIGURATION
           << F(" not opened");
  }
#endif
}
void AFEDataAccess::saveConfiguration(I2CPORT *configuration) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << AFE_FILE_I2C_CONFIGURATION
         << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(AFE_FILE_I2C_CONFIGURATION, "w");
#else
  File configFile = SPIFFS.open(AFE_FILE_I2C_CONFIGURATION, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_I2C> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["SDA"] = configuration->SDA;
    root["SCL"] = configuration->SCL;
    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    Serial << endl
           << F("INFO: Data saved") << endl
           << F("INFO: JSON: Buffer size: ") << AFE_CONFIG_FILE_BUFFER_I2C
           << F(", actual JSON size: ") << jsonBuffer.size();
    if (AFE_CONFIG_FILE_BUFFER_I2C < jsonBuffer.size() + 10) {
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
void AFEDataAccess::createI2CConfigurationFile() {
#ifdef DEBUG
  Serial << endl << F("INFO: Creating file: ") << AFE_FILE_I2C_CONFIGURATION;
#endif
  I2CPORT configuration;
  configuration.SDA = AFE_CONFIG_HARDWARE_I2C_DEFAULT_SDA;
  configuration.SCL = AFE_CONFIG_HARDWARE_I2C_DEFAULT_SCL;
  saveConfiguration(&configuration);
}
#endif // AFE_CONFIG_HARDWARE_I2C

#ifdef AFE_CONFIG_HARDWARE_BMEX80
void AFEDataAccess::getConfiguration(uint8_t id, BMEX80 *configuration) {
  char fileName[17];
  sprintf(fileName, AFE_FILE_BMX680_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
      configuration->i2cAddress = root["i2cAddress"];
      sprintf(configuration->name, root["name"]);
      configuration->interval = root["interval"];
      configuration->resolution = root["resolution"];
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
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
      Serial << endl
             << F("INFO: JSON: Buffer size: ") << AFE_CONFIG_FILE_BUFFER_BMEX80
             << F(", actual JSON size: ") << jsonBuffer.size();
      if (AFE_CONFIG_FILE_BUFFER_BMEX80 < jsonBuffer.size() + 10) {
        Serial << endl << F("WARN: Too small buffer size");
      }
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
  char fileName[17];
  sprintf(fileName, AFE_FILE_BMX680_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
    JsonObject &idx = root.createNestedObject("idx");
#endif
    JsonObject &temperature = root.createNestedObject("temperature");
    JsonObject &humidity = root.createNestedObject("humidity");
    JsonObject &pressure = root.createNestedObject("pressure");

    root["type"] = configuration->type;
    root["interval"] = configuration->interval;
    root["name"] = configuration->name;
    root["i2cAddress"] = configuration->i2cAddress;
    root["resolution"] = configuration->resolution;
#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
    root["mqttTopic"] = configuration->mqtt.topic;
#else
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
    Serial << endl
           << F("INFO: Data saved") << endl
           << F("INFO: JSON: Buffer size: ") << AFE_CONFIG_FILE_BUFFER_BMEX80
           << F(", actual JSON size: ") << jsonBuffer.size();
    if (AFE_CONFIG_FILE_BUFFER_BMEX80 < jsonBuffer.size() + 10) {
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
void AFEDataAccess::createBMEX80SensorConfigurationFile() {

  BMEX80 configuration;
  configuration.type = AFE_BMX_UNKNOWN_SENSOR;
  configuration.interval = AFE_CONFIG_HARDWARE_BMEX80_DEFAULT_INTERVAL;
  configuration.i2cAddress = 0;
  configuration.resolution = 3; //  BMP085_ULTRAHIGHRES;
  configuration.temperature.unit = AFE_TEMPERATURE_UNIT_CELSIUS;
  configuration.temperature.correction = 0;
  configuration.humidity.correction = 0;
  configuration.pressure.unit = AFE_PRESSURE_UNIT_HPA;
  configuration.pressure.correction = 0;
  configuration.altitude = 0;
  configuration.seaLevelPressure = AFE_CONFIG_DEFAULT_SEA_LEVEL_PRESSURE;

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
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
#endif

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_BMEX80; i++) {
#ifdef DEBUG
    Serial << endl << F("INFO: Creating file: cfg-BMEX80-") << i << F(".json");
#endif
#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
    sprintf(configuration.mqtt.topic, "BMEX80/%d", i + 1);
#endif
    sprintf(configuration.name, "BMEX80-%d", i + 1);
    saveConfiguration(i, &configuration);
  }
}
#endif // AFE_CONFIG_HARDWARE_BMEX80

#ifdef AFE_CONFIG_HARDWARE_BH1750
void AFEDataAccess::getConfiguration(uint8_t id, BH1750 *configuration) {
  char fileName[18];
  sprintf(fileName, AFE_FILE_BH1750_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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

      sprintf(configuration->name, root["name"]);
      configuration->mode = root["mode"];
      configuration->interval = root["interval"];
      configuration->i2cAddress = root["i2cAddress"];
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
      configuration->domoticz.idx = root["idx"] | AFE_DOMOTICZ_DEFAULT_IDX;
#else
      sprintf(configuration->mqtt.topic, root["mqttTopic"] | "");
#endif

#ifdef DEBUG
      Serial << endl
             << F("INFO: JSON: Buffer size: ") << AFE_CONFIG_FILE_BUFFER_BH1750
             << F(", actual JSON size: ") << jsonBuffer.size();
      if (AFE_CONFIG_FILE_BUFFER_BH1750 < jsonBuffer.size() + 10) {
        Serial << endl << F("WARN: Too small buffer size");
      }
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
void AFEDataAccess::saveConfiguration(uint8_t id, BH1750 *configuration) {
  char fileName[18];
  sprintf(fileName, AFE_FILE_BH1750_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
    root["i2cAddress"] = configuration->i2cAddress;
#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
    root["mqttTopic"] = configuration->mqtt.topic;
#else
    root["idx"] = configuration->domoticz.idx;
#endif

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    Serial << endl
           << F("INFO: Data saved") << endl
           << F("INFO: JSON: Buffer size: ") << AFE_CONFIG_FILE_BUFFER_BH1750
           << F(", actual JSON size: ") << jsonBuffer.size();
    if (AFE_CONFIG_FILE_BUFFER_BH1750 < jsonBuffer.size() + 10) {
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

void AFEDataAccess::createBH1750SensorConfigurationFile() {
  BH1750 configuration;
  configuration.interval = AFE_CONFIG_HARDWARE_BH1750_DEFAULT_INTERVAL;
  configuration.i2cAddress = 0;
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  configuration.domoticz.idx = AFE_DOMOTICZ_DEFAULT_IDX;
#endif
  configuration.mode = AFE_CONFIG_HARDWARE_BH1750_DEFAULT_MODE;
  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_BH1750; i++) {
#ifdef DEBUG
    Serial << endl << F("INFO: Creating file: cfg-bh1750-") << i << F(".json");
#endif
#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
    sprintf(configuration.mqtt.topic, "BH1750/%d", i + 1);
#endif
    sprintf(configuration.name, "BH1750-%d", i + 1);
    saveConfiguration(i, &configuration);
  }
}
#endif // AFE_CONFIG_HARDWARE_BH1750

#ifdef AFE_CONFIG_HARDWARE_AS3935
void AFEDataAccess::getConfiguration(uint8_t id, AS3935 *configuration) {
  char fileName[18];
  sprintf(fileName, AFE_FILE_AS3935_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
      configuration->domoticz.idx = root["idx"] | AFE_DOMOTICZ_DEFAULT_IDX;
#else
      sprintf(configuration->mqtt.topic, root["mqttTopic"] | "");
#endif

#ifdef DEBUG
      Serial << endl
             << F("INFO: JSON: Buffer size: ") << AFE_CONFIG_FILE_BUFFER_AS3935
             << F(", actual JSON size: ") << jsonBuffer.size();
      if (AFE_CONFIG_FILE_BUFFER_AS3935 < jsonBuffer.size() + 10) {
        Serial << endl << F("WARN: Too small buffer size");
      }
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

  char fileName[18];
  sprintf(fileName, AFE_FILE_AS3935_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
    root["mqttTopic"] = configuration->mqtt.topic;
#else
    root["idx"] = configuration->domoticz.idx;
#endif
    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    Serial << endl
           << F("INFO: Data saved") << endl
           << F("INFO: JSON: Buffer size: ") << AFE_CONFIG_FILE_BUFFER_AS3935
           << F(", actual JSON size: ") << jsonBuffer.size();
    if (AFE_CONFIG_FILE_BUFFER_AS3935 < jsonBuffer.size() + 10) {
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
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  configuration.domoticz.idx = AFE_DOMOTICZ_DEFAULT_IDX;
#endif
  configuration.unit = AFE_DISTANCE_KM;

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_AS3935; i++) {
#ifdef DEBUG
    Serial << endl << F("INFO: Creating file: cfg-as3935-") << i << F(".json");
#endif
#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
    sprintf(configuration.mqtt.topic, "AS3935/%d", i + 1);
#endif
    sprintf(configuration.name, "AS3935-%d", i + 1);
    saveConfiguration(i, &configuration);
  }
}
#endif // AFE_CONFIG_HARDWARE_AS3935

#ifdef AFE_CONFIG_HARDWARE_DHT
void AFEDataAccess::getConfiguration(uint8_t id, DHT *configuration) {

  char fileName[16];
  sprintf(fileName, AFE_FILE_DHT_SENSOR_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << "INFO: Opening file: " << fileName << " ... ";
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "r");
#else
  File configFile = SPIFFS.open(fileName, "r");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << "success" << endl << "INFO: JSON: ";
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

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
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
      Serial << endl
             << "INFO: JSON: Buffer size: " << AFE_CONFIG_FILE_BUFFER_DHT
             << ", actual JSON size: " << jsonBuffer.size();
      if (AFE_CONFIG_FILE_BUFFER_DHT < jsonBuffer.size() + 10) {
        Serial << endl << "WARN: Too small buffer size";
      }
#endif
    }
#ifdef DEBUG
    else {
      Serial << "ERROR: JSON not pharsed";
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else {
    Serial << endl
           << "ERROR: Configuration file: " << fileName << " not opened";
  }
#endif
}
void AFEDataAccess::saveConfiguration(uint8_t id, DHT *configuration) {

  char fileName[16];
  sprintf(fileName, AFE_FILE_DHT_SENSOR_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << "INFO: Opening file: " << fileName << " ... ";
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(fileName, "w");
#else
  File configFile = SPIFFS.open(fileName, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << "success" << endl << "INFO: Writing JSON: ";
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_DHT> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    JsonObject &temperature = root.createNestedObject("temperature");
    JsonObject &humidity = root.createNestedObject("humidity");
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
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
#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
    root["mqttTopic"] = configuration->mqtt.topic;
#else
    domoticz["temperature"] = configuration->domoticz.temperature.idx;
    domoticz["humidity"] = configuration->domoticz.humidity.idx;
    domoticz["dewPoint"] = configuration->domoticz.dewPoint.idx;
    domoticz["heatIndex"] = configuration->domoticz.heatIndex.idx;
    domoticz["temperatureHumidity"] =
        configuration->domoticz.temperatureHumidity.idx;
    domoticz["absoluteHumidity"] = configuration->domoticz.absoluteHumidity.idx;
    domoticz["perception"] = configuration->domoticz.perception.idx;
    domoticz["comfort"] = configuration->domoticz.comfort.idx;

#endif
    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    Serial << endl
           << "INFO: Data saved" << endl
           << "INFO: JSON: Buffer size: " << AFE_CONFIG_FILE_BUFFER_DHT
           << ", actual JSON size: " << jsonBuffer.size();
    if (AFE_CONFIG_FILE_BUFFER_DHT < jsonBuffer.size() + 10) {
      Serial << endl << "WARN: Too small buffer size";
    }
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << "ERROR: failed to open file for writing";
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

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  configuration.domoticz.temperatureHumidity.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  configuration.domoticz.temperature.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  configuration.domoticz.humidity.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  configuration.domoticz.dewPoint.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  configuration.domoticz.heatIndex.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  configuration.domoticz.comfort.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  configuration.domoticz.perception.idx = AFE_DOMOTICZ_DEFAULT_IDX;
  configuration.domoticz.absoluteHumidity.idx = AFE_DOMOTICZ_DEFAULT_IDX;
#endif

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_DHT; i++) {
#ifdef DEBUG
    Serial << endl << "INFO: Creating file: /cfg-dht-" << i << ".json";
#endif
#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
    sprintf(configuration.mqtt.topic, "DHT/%d", i + 1);
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

#ifdef T0_CONFIG
DEVICE_T0_200 AFEDataAccess::getDeviceT0v200Configuration() {
  DEVICE_T0_200 configuration;
#ifdef DEBUG
  Serial << endl
         << endl
         << F("----------------- Reading File -------------------");
  Serial << endl << F("Opening file: cfg-device.json : ");
  Serial << endl << F("THIS IS OLD VERSION OF THE DEVICE FILE ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER_SENSOR
void AFEDataAccess::getConfiguration(ANEMOMETER *configuration) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ")
         << AFE_FILE_ANEMOMETER_SENSOR_CONFIGURATION << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_ANEMOMETER_SENSOR> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif
      sprintf(configuration->name, root["name"] | "");
      configuration->gpio =
          root["gpio"] | AFE_HARDWARE_ANEMOMETER_SENSOR_DEFAULT_GPIO;
      configuration->sensitiveness =
          root["sensitiveness"] |
          AFE_HARDWARE_ANEMOMETER_SENSOR_DEFAULT_BOUNCING;
      configuration->interval =
          root["interval"] | AFE_HARDWARE_ANEMOMETER_SENSOR_DEFAULT_INTERVAL;
      configuration->impulseDistance = root["impulseDistance"].as<float>();
      configuration->impulseDistanceUnit =
          root["impulseDistanceUnit"] |
          AFE_HARDWARE_ANEMOMETER_SENSOR_DEFAULT_IMPULSE_DISTANCE_UNIT;

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
      sprintf(configuration->mqtt.topic, root["MQTTTopic"] | "");
#else
      configuration->domoticz.idx = root["idx"] | AFE_DOMOTICZ_DEFAULT_IDX;
#endif

#ifdef DEBUG
      Serial << endl
             << F("INFO: JSON: Buffer size: ")
             << AFE_CONFIG_FILE_BUFFER_ANEMOMETER_SENSOR
             << F(", actual JSON size: ") << jsonBuffer.size();
      if (AFE_CONFIG_FILE_BUFFER_ANEMOMETER_SENSOR < jsonBuffer.size() + 10) {
        Serial << endl << F("WARN: Too small buffer size");
      }
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
           << AFE_FILE_ANEMOMETER_SENSOR_CONFIGURATION << F(" not opened");
  }
#endif
}

void AFEDataAccess::saveConfiguration(ANEMOMETER *configuration) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ")
         << AFE_FILE_ANEMOMETER_SENSOR_CONFIGURATION << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
  File configFile =
      LittleFS.open(AFE_FILE_ANEMOMETER_SENSOR_CONFIGURATION, "w");
#else
  File configFile = SPIFFS.open(AFE_FILE_ANEMOMETER_SENSOR_CONFIGURATION, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_ANEMOMETER_SENSOR> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["name"] = configuration->name;
    root["gpio"] = configuration->gpio;
    root["sensitiveness"] = configuration->sensitiveness;
    root["interval"] = configuration->interval;
    root["impulseDistance"] = configuration->impulseDistance;
    root["impulseDistanceUnit"] = configuration->impulseDistanceUnit;
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
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
    Serial << endl
           << F("INFO: Data saved") << endl
           << F("INFO: JSON: Buffer size: ")
           << AFE_CONFIG_FILE_BUFFER_ANEMOMETER_SENSOR
           << F(", actual JSON size: ") << jsonBuffer.size();
    if (AFE_CONFIG_FILE_BUFFER_ANEMOMETER_SENSOR < jsonBuffer.size() + 10) {
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

void AFEDataAccess::createAnemometerSensorConfigurationFile() {
#ifdef DEBUG
  Serial << endl
         << F("INFO: Creating file: ")
         << AFE_FILE_ANEMOMETER_SENSOR_CONFIGURATION;
#endif
  ANEMOMETER configuration;
  configuration.sensitiveness = AFE_HARDWARE_ANEMOMETER_SENSOR_DEFAULT_BOUNCING;
  configuration.interval = AFE_HARDWARE_ANEMOMETER_SENSOR_DEFAULT_INTERVAL;
#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  configuration.mqtt.topic[0] = AFE_EMPTY_STRING;
#else
  configuration.domoticz.idx = AFE_DOMOTICZ_DEFAULT_IDX;
#endif
  /* Saving first switch. Common for all devices */
  configuration.gpio = AFE_HARDWARE_ANEMOMETER_SENSOR_DEFAULT_GPIO;
  configuration.impulseDistance =
      AFE_HARDWARE_ANEMOMETER_SENSOR_DEFAULT_IMPULSE_DISTANCE;
  configuration.impulseDistanceUnit =
      AFE_HARDWARE_ANEMOMETER_SENSOR_DEFAULT_IMPULSE_DISTANCE_UNIT;

  sprintf(configuration.name, "anemometer");

  saveConfiguration(&configuration);
}
#endif // AFE_CONFIG_HARDWARE_ANEMOMETER_SENSOR

#ifdef AFE_CONFIG_HARDWARE_RAINMETER_SENSOR
void AFEDataAccess::getConfiguration(RAINMETER *configuration) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << AFE_FILE_RAINMETER_SENSOR_CONFIGURATION
         << F(" ... ");
#endif
#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_RAINMETER_SENSOR> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif
      sprintf(configuration->name, root["name"] | "");
      configuration->gpio =
          root["gpio"] | AFE_HARDWARE_RAINMETER_SENSOR_DEFAULT_GPIO;
      configuration->sensitiveness =
          root["sensitiveness"] |
          AFE_HARDWARE_RAINMETER_SENSOR_DEFAULT_BOUNCING;
      configuration->interval =
          root["interval"] | AFE_HARDWARE_RAINMETER_SENSOR_DEFAULT_INTERVAL;
      configuration->resolution = root["resolution"].as<float>();

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
      sprintf(configuration->mqtt.topic, root["MQTTTopic"] | "");
#else
      configuration->domoticz.idx = root["idx"] | AFE_DOMOTICZ_DEFAULT_IDX;
#endif

#ifdef DEBUG
      Serial << endl
             << F("INFO: JSON: Buffer size: ")
             << AFE_CONFIG_FILE_BUFFER_RAINMETER_SENSOR
             << F(", actual JSON size: ") << jsonBuffer.size();
      if (AFE_CONFIG_FILE_BUFFER_RAINMETER_SENSOR < jsonBuffer.size() + 10) {
        Serial << endl << F("WARN: Too small buffer size");
      }
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
           << AFE_FILE_RAINMETER_SENSOR_CONFIGURATION << F(" not opened");
  }
#endif
}

void AFEDataAccess::saveConfiguration(RAINMETER *configuration) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << AFE_FILE_RAINMETER_SENSOR_CONFIGURATION
         << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(AFE_FILE_RAINMETER_SENSOR_CONFIGURATION, "w");
#else
  File configFile = SPIFFS.open(AFE_FILE_RAINMETER_SENSOR_CONFIGURATION, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_RAINMETER_SENSOR> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["name"] = configuration->name;
    root["gpio"] = configuration->gpio;
    root["sensitiveness"] = configuration->sensitiveness;
    root["interval"] = configuration->interval;
    root["resolution"] = configuration->resolution;
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
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
    Serial << endl
           << F("INFO: Data saved") << endl
           << F("INFO: JSON: Buffer size: ")
           << AFE_CONFIG_FILE_BUFFER_RAINMETER_SENSOR
           << F(", actual JSON size: ") << jsonBuffer.size();
    if (AFE_CONFIG_FILE_BUFFER_RAINMETER_SENSOR < jsonBuffer.size() + 10) {
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

void AFEDataAccess::createRainmeterSensorConfigurationFile() {
#ifdef DEBUG
  Serial << endl
         << F("INFO: Creating file: ")
         << AFE_FILE_RAINMETER_SENSOR_CONFIGURATION;
#endif
  RAINMETER configuration;
  configuration.sensitiveness = AFE_HARDWARE_RAINMETER_SENSOR_DEFAULT_BOUNCING;
  configuration.interval = AFE_HARDWARE_RAINMETER_SENSOR_DEFAULT_INTERVAL;
#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  configuration.mqtt.topic[0] = AFE_EMPTY_STRING;
#else
  configuration.domoticz.idx = AFE_DOMOTICZ_DEFAULT_IDX;
#endif
  /* Saving first switch. Common for all devices */
  configuration.gpio = AFE_HARDWARE_RAINMETER_SENSOR_DEFAULT_GPIO;
  configuration.resolution = AFE_HARDWARE_RAINMETER_SENSOR_DEFAULT_RESOLUTION;

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

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_RAINMETER_SENSOR_DATA> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      data->index1h = root["index1h"];
      for (uint8_t i = 0; i < 60; i++) {
        data->last1h[i] = root["last1h"][i].as<float>();
      }

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
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
      Serial << endl
             << F("INFO: JSON: Buffer size: ")
             << AFE_CONFIG_FILE_BUFFER_RAINMETER_SENSOR_DATA
             << F(", actual JSON size: ") << jsonBuffer.size();
      if (AFE_CONFIG_FILE_BUFFER_RAINMETER_SENSOR_DATA <
          jsonBuffer.size() + 10) {
        Serial << endl << F("WARN: Too small buffer size");
      }
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
           << F("ERROR: Configuration file: ") << AFE_FILE_RAINMETER_SENSOR_DATA
           << F(" not opened");
  }
#endif
}
void AFEDataAccess::save(RAINMETER_DATA *data) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Opening file: ") << AFE_FILE_RAINMETER_SENSOR_DATA
         << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
  File configFile = LittleFS.open(AFE_FILE_RAINMETER_SENSOR_DATA, "w");
#else
  File configFile = SPIFFS.open(AFE_FILE_RAINMETER_SENSOR_DATA, "w");
#endif

  if (configFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_RAINMETER_SENSOR_DATA> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    JsonArray &last1h = jsonBuffer.createArray();
#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
    JsonArray &last12h = jsonBuffer.createArray();
    JsonArray &last24h = jsonBuffer.createArray();
#endif

    last1h.copyFrom(data->last1h);
    root["index1h"] = data->index1h;
    root["last1h"] = last1h;
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
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
    Serial << endl
           << F("INFO: Data saved") << endl
           << F("INFO: JSON: Buffer size: ")
           << AFE_CONFIG_FILE_BUFFER_RAINMETER_SENSOR_DATA
           << F(", actual JSON size: ") << jsonBuffer.size();
    if (AFE_CONFIG_FILE_BUFFER_RAINMETER_SENSOR_DATA < jsonBuffer.size() + 10) {
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

void AFEDataAccess::createRainmeterSensorDataConfigurationFile() {
#ifdef DEBUG
  Serial << endl
         << F("INFO: Creating file: ") << AFE_FILE_RAINMETER_SENSOR_DATA;
#endif
  RAINMETER_DATA data;
  data.index1h = 0;
  for (uint8_t i = 0; i < 60; i++) {
    data.last1h[i] = 0;
  }
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
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

#endif // AFE_CONFIG_HARDWARE_RAINMETER_SENSOR

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
void AFEDataAccess::getConfiguration(uint8_t id, BINARY_SENSOR *configuration) {
  char fileName[27];
  sprintf(fileName, AFE_FILE_BINARY_SENSOR_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
      sprintf(configuration->mqtt.topic, root["MQTTTopic"] | "");
#else
      configuration->domoticz.idx = root["idx"] | AFE_DOMOTICZ_DEFAULT_IDX;
#endif

#ifdef AFE_CONFIG_HARDWARE_MCP23017
      configuration->mcp23017.address = root["mcp23017"]["address"];
      configuration->mcp23017.gpio = root["mcp23017"]["gpio"];
#endif

#ifdef DEBUG
      Serial << endl
             << F("INFO: JSON: Buffer size: ")
             << AFE_CONFIG_FILE_BUFFER_BINARY_SENSOR
             << F(", actual JSON size: ") << jsonBuffer.size();
      if (AFE_CONFIG_FILE_BUFFER_BINARY_SENSOR < jsonBuffer.size() + 10) {
        Serial << endl << F("WARN: Too small buffer size");
      }
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
  char fileName[27];
  sprintf(fileName, AFE_FILE_BINARY_SENSOR_CONFIGURATION, id);

#ifdef DEBUG
  Serial << endl << endl << F("INFO: Opening file: ") << fileName << F(" ... ");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
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
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
    root["idx"] = configuration->domoticz.idx;
#else
    root["MQTTTopic"] = configuration->mqtt.topic;
#endif

#ifdef AFE_CONFIG_HARDWARE_MCP23017
    JsonObject &mcp23017 = root.createNestedObject("mcp23017");
    mcp23017["address"] = configuration->mcp23017.address;
    mcp23017["gpio"] = configuration->mcp23017.gpio;
#endif // AFE_CONFIG_HARDWARE_MCP23017

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    Serial << endl
           << F("INFO: Data saved") << endl
           << F("INFO: JSON: Buffer size: ")
           << AFE_CONFIG_FILE_BUFFER_BINARY_SENSOR << F(", actual JSON size: ")
           << jsonBuffer.size();
    if (AFE_CONFIG_FILE_BUFFER_BINARY_SENSOR < jsonBuffer.size() + 10) {
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
void AFEDataAccess::createBinarySensorConfigurationFile() {
  BINARY_SENSOR configuration;
  configuration.bouncing = AFE_HARDWARE_BINARY_SENSOR_DEFAULT_BOUNCING;
#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  configuration.mqtt.topic[0] = AFE_EMPTY_STRING;
#else
  configuration.domoticz.idx = AFE_DOMOTICZ_DEFAULT_IDX;
#endif

  configuration.gpio = AFE_HARDWARE_ITEM_NOT_EXIST;
  configuration.revertSignal = AFE_HARDWARE_BINARY_SENSOR_DEFAULT_REVERT_SIGNAL;
  configuration.sendAsSwitch =
      AFE_HARDWARE_BINARY_SENSOR_DEFAULT_SENT_AS_SWITCH;
  configuration.internalPullUp =
      AFE_HARDWARE_BINARY_SENSOR_DEFAULT_INTERNAL_PULLUP_RESISTOR;

#ifdef AFE_CONFIG_HARDWARE_MCP23017
  configuration.mcp23017.address =
      AFE_CONFIG_HARDWARE_I2C_DEFAULT_NON_EXIST_ADDRESS;
  configuration.mcp23017.gpio = AFE_HARDWARE_ITEM_NOT_EXIST;
#endif // AFE_CONFIG_HARDWARE_MCP23017

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  configuration.domoticz.idx = AFE_DOMOTICZ_DEFAULT_IDX;
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_BINARY_SENSORS;
       i++) {
#ifdef DEBUG
    Serial << endl
           << F("INFO: Creating file: cfg-binary-sensor-") << i << F(".json");
#endif

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
    sprintf(configuration.mqtt.topic, "binary/%d", i + 1);
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
    sprintf(configuration.name, "binary-%d", i + 1);

    saveConfiguration(i, &configuration);
  }
}
#endif
