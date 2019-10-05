/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Data-Access.h"

AFEDataAccess::AFEDataAccess() {}

boolean AFEDataAccess::formatFileSystem()
{
#ifdef DEBUG
  Serial << endl
         << endl
         << "Formatig File System";
#endif
  return SPIFFS.format();
}

const String AFEDataAccess::getDeviceUID()
{
  String uid;

#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Reading File -------------------";
  Serial << endl
         << "Opening file: cfg-device-uid.json : ";
#endif

  File configFile = SPIFFS.open("cfg-device-uid.json", "r");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Reading JSON : ";
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);

    StaticJsonBuffer<30> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());

    if (root.success())
    {
#ifdef DEBUG
      root.printTo(Serial);
#endif
      uid = root.get<char *>("uid");
#ifdef DEBUG
      Serial << endl
             << "success" << endl
             << "JSON Buffer size: " << jsonBuffer.size();
#endif
    }

#ifdef DEBUG
    else
    {
      Serial << "failure";
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else
  {
    Serial << "failure";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif
  return uid;
}
void AFEDataAccess::saveDeviceUID(const char *uid)
{
#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Writing File -------------------";
  Serial << endl
         << "Opening file: cfg-device-uid.json : ";
#endif

  File configFile = SPIFFS.open("cfg-device-uid.json", "w");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Writing JSON : ";
#endif

    StaticJsonBuffer<30> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["uid"] = uid;
    root.printTo(configFile);

#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    Serial << endl
           << "success" << endl
           << "JSON Buffer size: " << jsonBuffer.size();
#endif
  }
#ifdef DEBUG
  else
  {
    Serial << endl
           << "failed to open file for writing";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif
}
void AFEDataAccess::createDeviceUIDFile()
{
#ifdef DEBUG
  Serial << endl
         << "Creating file: cfg-device-uid.json";
#endif

  byte m[6];
  char uid[18];
  WiFi.macAddress(m);
  sprintf(uid, "%X%x%X%x-%X%x%X%x", m[0], m[5], m[1], m[4], m[2], m[3], m[3],
          m[2]);
  saveDeviceUID(uid);
}

PRO_VERSION AFEDataAccess::getProVersionConfiguration()
{
  PRO_VERSION configuration;

#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Reading File -------------------";
  Serial << endl
         << "Opening file: cfg-pro-version.json : ";
#endif

  File configFile = SPIFFS.open("cfg-pro-version.json", "r");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Reading JSON : ";
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<50> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success())
    {
#ifdef DEBUG
      root.printTo(Serial);
#endif
      configuration.valid = root["valid"];
      sprintf(configuration.serial, root["serial"]);
#ifdef DEBUG
      Serial << endl
             << "success" << endl
             << "JSON Buffer size: " << jsonBuffer.size();
#endif
    }

#ifdef DEBUG
    else
    {
      Serial << "failure";
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else
  {
    Serial << "failure";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif

  return configuration;
}
void AFEDataAccess::saveConfiguration(PRO_VERSION configuration)
{
#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Writing File -------------------";
  Serial << endl
         << "Opening file: cfg-pro-version.json : ";
#endif
  File configFile = SPIFFS.open("cfg-pro-version.json", "w");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Writing JSON : ";
#endif

    StaticJsonBuffer<69> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["valid"] = configuration.valid;
    root["serial"] = configuration.serial;

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    Serial << endl
           << "success" << endl
           << "JSON Buffer size: " << jsonBuffer.size();
#endif
  }
#ifdef DEBUG
  else
  {
    Serial << endl
           << "failed to open file for writing";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif
}
void AFEDataAccess::createProVersionConfigurationFile()
{
#ifdef DEBUG
  Serial << endl
         << "Creating file: cfg-pro-version.json";
#endif
  PRO_VERSION ProConfiguration;
  ProConfiguration.serial[0] = '\0';
  ProConfiguration.valid = false;
  saveConfiguration(ProConfiguration);
}

PASSWORD AFEDataAccess::getPasswordConfiguration()
{
  PASSWORD configuration;

#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Reading File -------------------";
  Serial << endl
         << "Opening file: cfg-password.json : ";
#endif

  File configFile = SPIFFS.open("cfg-password.json", "r");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Reading JSON : ";
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<50> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success())
    {
#ifdef DEBUG
      root.printTo(Serial);
#endif
      configuration.protect = root["protect"];
      sprintf(configuration.password, root["password"]);

#ifdef DEBUG
      Serial << endl
             << "success" << endl
             << "JSON Buffer size: " << jsonBuffer.size();
#endif
    }

#ifdef DEBUG
    else
    {
      Serial << "failure";
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else
  {
    Serial << "failure";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif
  return configuration;
}
void AFEDataAccess::saveConfiguration(PASSWORD configuration)
{
#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Writing File -------------------";
  Serial << endl
         << "Opening file: cfg-password.json : ";
#endif

  File configFile = SPIFFS.open("cfg-password.json", "w");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Writing JSON : ";
#endif

    StaticJsonBuffer<59> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["protect"] = configuration.protect;
    root["password"] = configuration.password;
    root.printTo(configFile);

#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    Serial << endl
           << "success" << endl
           << "JSON Buffer size: " << jsonBuffer.size();
#endif
  }
#ifdef DEBUG
  else
  {
    Serial << endl
           << "failed to open file for writing";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif
}
void AFEDataAccess::createPasswordConfigurationFile()
{
#ifdef DEBUG
  Serial << endl
         << "Creating file: cfg-password.json";
#endif
  PASSWORD PasswordConfiguration;
  PasswordConfiguration.protect = false;
  PasswordConfiguration.password[0] = '\0';
  saveConfiguration(PasswordConfiguration);
}

#define AFE_CONFIG_FILE_BUFFER_DEVICE 240
DEVICE AFEDataAccess::getDeviceConfiguration()
{
  DEVICE configuration;
#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Reading File -------------------";
  Serial << endl
         << "Opening file: cfg-device.json : ";
#endif

  File configFile = SPIFFS.open("cfg-device.json", "r");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Reading JSON : ";
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_DEVICE> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success())
    {
#ifdef DEBUG
      root.printTo(Serial);
#endif
      sprintf(configuration.name, root["name"]);
      configuration.api.http = root["api"]["http"];
      configuration.api.mqtt = root["api"]["mqtt"];
      configuration.api.domoticz = root["api"]["domoticz"];
      /* HTTP API must be ON when Domoticz is ON */
      if (configuration.api.domoticz && !configuration.api.http)
      {
        configuration.api.http = true;
      }

      configuration.noOfLEDs = root["noOfLEDs"];
      configuration.noOfSwitches = root["noOfSwitches"];
      configuration.noOfRelays = root["noOfRelays"];

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
      configuration.isAnalogInput = root["isAnalogInput"];
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
      configuration.noOfContactrons = root["noOfContactrons"];
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
      configuration.noOfGates = root["noOfGates"];
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
      configuration.noOfBMEX80s = root["noOfBMEX80s"];
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
      configuration.noOfHPMA115S0s = root["noOfHPMA115S0s"];
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
      configuration.noOfBH1750s = root["noOfBH1750s"];
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
      configuration.noOfAS3935s = root["noOfAS3935s"];
#endif

#ifdef DEBUG
      Serial << endl
             << "success" << endl
             << "JSON Buffer size: " << jsonBuffer.size() << endl
             << (AFE_CONFIG_FILE_BUFFER_DEVICE < jsonBuffer.size() + 10
                     ? "WARNING: Buffor size might be to small"
                     : "Buffor size: OK");
#endif
    }

#ifdef DEBUG
    else
    {
      Serial << "failure";
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else
  {
    Serial << "failure";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif

  return configuration;
  
}
void AFEDataAccess::saveConfiguration(DEVICE *configuration)
{

#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Writing File -------------------";
  Serial << endl
         << "Opening file: cfg-device.json : ";
#endif

  File configFile = SPIFFS.open("cfg-device.json", "w");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Writing JSON : ";
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_DEVICE> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["name"] = configuration->name;

    JsonObject &jsonAPI = root.createNestedObject("api");
    jsonAPI["http"] = configuration->api.http;
    jsonAPI["mqtt"] = configuration->api.mqtt;
    jsonAPI["domoticz"] = configuration->api.domoticz;

    root["noOfLEDs"] = configuration->noOfLEDs;
    root["noOfSwitches"] = configuration->noOfSwitches;

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

#ifdef AFE_CONFIG_HARDWARE_BH1750
    root["noOfAS3935s"] = configuration->noOfAS3935s;
#endif

    root.printTo(configFile);

#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    Serial << endl
           << "success" << endl
           << "JSON Buffer size: " << jsonBuffer.size() << endl
           << (AFE_CONFIG_FILE_BUFFER_DEVICE < jsonBuffer.size() + 10
                   ? "WARNING: Buffor size might be to small"
                   : "Buffor size: OK");
#endif
  }
#ifdef DEBUG
  else
  {
    Serial << endl
           << "failed to open file for writing";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif

  /*
    uint8_t index;
    Eeprom.write(9, 16, configuration.name);

  #if defined(T3_CONFIG)
    index = 21;
  #elif defined(T4_CONFIG)
    index = 27;
  #endif

    for (uint8_t i = 0; i < sizeof(configuration.isRelay); i++) {
  #if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG) || defined(T5_CONFIG)
      Eeprom.write(369, configuration.isRelay[i]);
  #elif defined(T1_CONFIG)
      Eeprom.write(396, configuration.isRelay[i]);
  #elif defined(T2_CONFIG)
      Eeprom.write(404, configuration.isRelay[i]);
  #elif defined(T3_CONFIG)
      Eeprom.write(381 + i * index, configuration.isRelay[i]);
  #elif defined(T4_CONFIG)
      Eeprom.write(382 + i * index, configuration.isRelay[i]);
  #elif defined(T6_CONFIG)
      Eeprom.write(373, configuration.isRelay[i]);
  #endif
    }

  #if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG) || defined(T1_CONFIG)
|| \ defined(T2_CONFIG) || defined(T3_CONFIG) index = 7; #elif
defined(T4_CONFIG)
  || defined(T5_CONFIG) || defined(T6_CONFIG) index = 8; #endif

    for (uint8_t i = 0; i < sizeof(configuration.isSwitch); i++) {
  #if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG) || defined(T6_CONFIG)
      Eeprom.write(395 + i * index, configuration.isSwitch[i]);
  #elif defined(T1_CONFIG)
      Eeprom.write(382 + i * index, configuration.isSwitch[i]);
  #elif defined(T2_CONFIG)
      Eeprom.write(390 + i * index, configuration.isSwitch[i]);
  #elif defined(T3_CONFIG)
      Eeprom.write(470 + i * index, configuration.isSwitch[i]);
  #elif defined(T4_CONFIG)
      Eeprom.write(490 + i * index, configuration.isSwitch[i]);
  #elif defined(T5_CONFIG)
      Eeprom.write(398 + i * index, configuration.isSwitch[i]);
  #endif
    }

  #if AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS > 0

  #if defined(T0_CONFIG)
    index = 52;
  #elif defined(T1_CONFIG)
    index = 77;
  #elif defined(T2_CONFIG)
    index = 98;
  #elif defined(T3_CONFIG) || defined(T4_CONFIG) || defined(T5_CONFIG) || \
      defined(T6_CONFIG)
    index = 3;
  #endif

    for (uint8_t i = 0; i < sizeof(configuration.isLED); i++) {
      Eeprom.write(366 + i * index, configuration.isLED[i]);
    }
  #endif

    saveAPI(API_MQTT, configuration.api.mqtt);
    saveAPI(API_HTTP, configuration.api.http);
    saveAPI(API_DOMOTICZ, configuration.api.domoticz);


  #if defined(T1_CONFIG) || defined(T2_CONFIG) || defined(T5_CONFIG)
  #if defined(T1_CONFIG)
    Eeprom.write(369, configuration.isDS18B20);
  #elif defined(T2_CONFIG)
    Eeprom.write(369, configuration.isDHT);
  #else

    Eeprom.write(376, configuration.isDHT);
    index = 24;
    for (uint8_t i = 0; i < sizeof(configuration.isContactron); i++) {
      Eeprom.write(414 + i * index, configuration.isContactron[i]);
    }
  #endif
  #endif

  #if defined(T3_CONFIG)
    index = 27;
    for (uint8_t i = 0; i < sizeof(configuration.isPIR); i++) {
      Eeprom.write(505 + i * index, configuration.isPIR[i]);
    }
  #endif

  #if defined(T6_CONFIG)
    Eeprom.write(413, configuration.isHPMA115S0);
    Eeprom.writeUInt8(422, configuration.isBMEX80);
    Eeprom.write(429, configuration.isBH1750);
  #endif


  */
}
void AFEDataAccess::createDeviceConfigurationFile()
{
#ifdef DEBUG
  Serial << endl
         << "Creating file: cfg-device.json";
#endif
  DEVICE deviceConfiguration;
  sprintf(deviceConfiguration.name, "AFE-Device");
  /* APIs */
  deviceConfiguration.api.mqtt = false;
  deviceConfiguration.api.domoticz = false;
  deviceConfiguration.api.http = true;

/* Relay presence */
#ifdef AFE_CONFIG_HARDWARE_RELAY
  deviceConfiguration.noOfRelays = AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_RELAYS;
#endif

  /* Switch presence */
  deviceConfiguration.noOfSwitches =
      AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_SWITCHES;

/* LEDs presence */
#if AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
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

  saveConfiguration(&deviceConfiguration);
}

FIRMWARE AFEDataAccess::getFirmwareConfiguration()
{
  FIRMWARE configuration;

#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Reading File -------------------";
  Serial << endl
         << "Opening file: cfg-firmware.json : ";
#endif

  File configFile = SPIFFS.open("cfg-firmware.json", "r");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Reading JSON : ";
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<100> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success())
    {
#ifdef DEBUG
      root.printTo(Serial);
#endif
      configuration.type = root["type"].as<int>();
      sprintf(configuration.version, root["version"]);
      sprintf(configuration.upgradeURL, root["upgradeURL"]);
      configuration.autoUpgrade = root["autoUpgrade"];

#ifdef DEBUG
      Serial << endl
             << "success" << endl
             << "JSON Buffer size: " << jsonBuffer.size();
#endif
    }

#ifdef DEBUG
    else
    {
      Serial << "failure";
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else
  {
    Serial << "failure";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif

  return configuration;
}
void AFEDataAccess::saveConfiguration(FIRMWARE *configuration)
{
#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Writing File -------------------";
  Serial << endl
         << "Opening file: cfg-firmware.json : ";
#endif

  File configFile = SPIFFS.open("cfg-firmware.json", "w");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Writing JSON : ";
#endif

    StaticJsonBuffer<100> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["type"] = configuration->type;
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
           << "success" << endl
           << "JSON Buffer size: " << jsonBuffer.size();
#endif
  }
#ifdef DEBUG
  else
  {
    Serial << endl
           << "failed to open file for writing";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif
}
void AFEDataAccess::createFirmwareConfigurationFile()
{
#ifdef DEBUG
  Serial << endl
         << "Creating file: cfg-firmware.json";
#endif
  FIRMWARE firmwareConfiguration;
  sprintf(firmwareConfiguration.version, AFE_FIRMWARE_VERSION);
  firmwareConfiguration.type = AFE_FIRMWARE_TYPE;
  firmwareConfiguration.autoUpgrade = 0;
  firmwareConfiguration.upgradeURL[0] = '\0';
  saveConfiguration(&firmwareConfiguration);
}

void AFEDataAccess::saveVersion(const char *version)
{
  FIRMWARE configuration = getFirmwareConfiguration();
  sprintf(configuration.version, version);
  saveConfiguration(&configuration);
}

uint8_t AFEDataAccess::getDeviceMode()
{
  uint8_t mode = AFE_MODE_FIRST_TIME_LAUNCH;
#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Reading File -------------------";
  Serial << endl
         << "Opening file: cfg-device-mode.json : ";
#endif

  File configFile = SPIFFS.open("cfg-device-mode.json", "r");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Reading JSON : ";
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<34> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success())
    {
#ifdef DEBUG
      root.printTo(Serial);
      Serial << endl;
#endif
      mode = root["mode"];
#ifdef DEBUG
      Serial << "success";
#endif
    }
#ifdef DEBUG
    else
    {
      Serial << endl
             << "success" << endl
             << "JSON Buffer size: " << jsonBuffer.size();
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else
  {
    Serial << "failure";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif

  return mode;
}
void AFEDataAccess::saveDeviceMode(uint8_t mode)
{
#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Writing File -------------------";
  Serial << endl
         << "Opening file: cfg-device-mode.json : ";
#endif

  File configFile = SPIFFS.open("cfg-device-mode.json", "w");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Writing JSON : ";
#endif

    StaticJsonBuffer<50> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["mode"] = mode;
#ifdef DEBUG
    uint16_t fileSize =
#endif
        root.printTo(configFile);
#ifdef DEBUG
    Serial << endl
           << (fileSize == 0 ? "ERROR: while writing a file. Size="
                             : "File size=")
           << fileSize << endl;
#endif

#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    Serial << endl
           << "success" << endl
           << "JSON Buffer size: " << jsonBuffer.size();
#endif
  }
#ifdef DEBUG
  else
  {
    Serial << endl
           << "failed to open file for writing";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif
}

NETWORK AFEDataAccess::getNetworkConfiguration()
{
  NETWORK configuration;

#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Reading File -------------------";
  Serial << endl
         << "Opening file: cfg-network.json : ";
#endif

  File configFile = SPIFFS.open("cfg-network.json", "r");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Reading JSON : ";
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<162> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success())
    {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      sprintf(configuration.ssid, root["ssid"]);
      sprintf(configuration.password, root["password"]);

      configuration.isDHCP = root["isDHCP"];

      sprintf(configuration.ip, root["ip"]);
      sprintf(configuration.gateway, root["gateway"]);
      sprintf(configuration.subnet, root["subnet"]);

      configuration.noConnectionAttempts = root["noConnectionAttempts"];
      configuration.waitTimeConnections = root["waitTimeConnections"];
      configuration.waitTimeSeries = root["waitTimeSeries"];

#ifdef DEBUG
      Serial << endl
             << "success" << endl
             << "JSON Buffer size: " << jsonBuffer.size();
#endif
    }

#ifdef DEBUG
    else
    {
      Serial << "failure";
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else
  {
    Serial << "failure";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif

  return configuration;
}
void AFEDataAccess::saveConfiguration(NETWORK configuration)
{
#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Writing File -------------------";
  Serial << endl
         << "Opening file: cfg-network.json : ";
#endif

  File configFile = SPIFFS.open("cfg-network.json", "w");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Writing JSON : ";
#endif

    StaticJsonBuffer<282> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["ssid"] = configuration.ssid;
    root["password"] = configuration.password;
    root["isDHCP"] = configuration.isDHCP;
    root["ip"] = configuration.ip;
    root["gateway"] = configuration.gateway;
    root["subnet"] = configuration.subnet;

    root["noConnectionAttempts"] = configuration.noConnectionAttempts;
    root["waitTimeConnections"] = configuration.waitTimeConnections;
    root["waitTimeSeries"] = configuration.waitTimeSeries;

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    Serial << endl
           << "success" << endl
           << "JSON Buffer size: " << jsonBuffer.size();
#endif
  }
#ifdef DEBUG
  else
  {
    Serial << endl
           << "failed to open file for writing";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif
}
void AFEDataAccess::createNetworkConfigurationFile()
{
#ifdef DEBUG
  Serial << endl
         << "Creating file: cfg-network.json";
#endif
  NETWORK networkConfiguration;
  /* Network default config */
  networkConfiguration.ssid[0] = '\0';
  networkConfiguration.password[0] = '\0';
  networkConfiguration.isDHCP = true;
  networkConfiguration.ip[0] = '\0';
  networkConfiguration.gateway[0] = '\0';
  networkConfiguration.subnet[0] = '\0';
  networkConfiguration.noConnectionAttempts = 30;
  networkConfiguration.waitTimeConnections = 1;
  networkConfiguration.waitTimeSeries = 20;
  saveConfiguration(networkConfiguration);
}

MQTT AFEDataAccess::getMQTTConfiguration()
{
  MQTT configuration;

#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Reading File -------------------";
  Serial << endl
         << "Opening file: cfg-mqtt-broker.json : ";
#endif

  File configFile = SPIFFS.open("cfg-mqtt-broker.json", "r");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Reading JSON : ";
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<114> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success())
    {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      sprintf(configuration.host, root["host"]);
      sprintf(configuration.ip, root["ip"]);
      configuration.port = root["port"];
      sprintf(configuration.user, root["user"]);
      sprintf(configuration.password, root["password"]);
      sprintf(configuration.lwt.topic, root["lwt"]);

#ifdef DEBUG
      Serial << endl
             << "success" << endl
             << "JSON Buffer size: " << jsonBuffer.size();
#endif
    }

#ifdef DEBUG
    else
    {
      Serial << "failure";
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else
  {
    Serial << "failure";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif

  return configuration;
}
void AFEDataAccess::saveConfiguration(MQTT configuration)
{
#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Writing File -------------------";
  Serial << endl
         << "Opening file: cfg-mqtt-broker.json : ";
#endif

  File configFile = SPIFFS.open("cfg-mqtt-broker.json", "w");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Writing JSON : ";
#endif

    StaticJsonBuffer<271> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["host"] = configuration.host;
    root["ip"] = configuration.ip;
    root["port"] = configuration.port;
    root["user"] = configuration.user;
    root["password"] = configuration.password;
    root["lwt"] = configuration.lwt.topic;

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    Serial << endl
           << "success" << endl
           << "JSON Buffer size: " << jsonBuffer.size();
#endif
  }
#ifdef DEBUG
  else
  {
    Serial << endl
           << "failed to open file for writing";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif
}
void AFEDataAccess::createMQTTConfigurationFile()
{
#ifdef DEBUG
  Serial << endl
         << "Creating file: cfg-mqtt-broker.json";
#endif
  MQTT MQTTConfiguration;
  /* MQTT Default config */
  MQTTConfiguration.host[0] = '\0';
  MQTTConfiguration.ip[0] = '\0';
  MQTTConfiguration.user[0] = '\0';
  MQTTConfiguration.password[0] = '\0';
  MQTTConfiguration.port = 1883;
  MQTTConfiguration.lwt.topic[0] = '\0';
  saveConfiguration(MQTTConfiguration);
}

DOMOTICZ AFEDataAccess::getDomoticzConfiguration()
{
  DOMOTICZ configuration;
#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Reading File -------------------";
  Serial << endl
         << "Opening file: cfg-domoticz-server.json : ";
#endif

  File configFile = SPIFFS.open("cfg-domoticz-server.json", "r");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Reading JSON : ";
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<98> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success())
    {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      configuration.protocol = root["protocol"];
      sprintf(configuration.host, root["host"]);
      configuration.port = root["port"];
      sprintf(configuration.user, root["user"]);
      sprintf(configuration.password, root["password"]);

#ifdef DEBUG
      Serial << endl
             << "success" << endl
             << "JSON Buffer size: " << jsonBuffer.size();
#endif
    }

#ifdef DEBUG
    else
    {
      Serial << "failure";
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else
  {
    Serial << "failure";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif
  return configuration;
}
void AFEDataAccess::saveConfiguration(DOMOTICZ configuration)
{
#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Writing File -------------------";
  Serial << endl
         << "Opening file: cfg-domoticz-server.json : ";
#endif

  File configFile = SPIFFS.open("cfg-domoticz-server.json", "w");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Writing JSON : ";
#endif

    StaticJsonBuffer<211> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["protocol"] = configuration.protocol;
    root["host"] = configuration.host;
    root["port"] = configuration.port;
    root["user"] = configuration.user;
    root["password"] = configuration.password;
    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    Serial << endl
           << "success" << endl
           << "JSON Buffer size: " << jsonBuffer.size();
#endif
  }
#ifdef DEBUG
  else
  {
    Serial << endl
           << "failed to open file for writing";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif
}
void AFEDataAccess::createDomoticzConfigurationFile()
{
#ifdef DEBUG
  Serial << endl
         << "Creating file: cfg-domoticz-server.json";
#endif
  DOMOTICZ DomoticzConfiguration;
  DomoticzConfiguration.protocol = 0;
  DomoticzConfiguration.host[0] = '\0';
  DomoticzConfiguration.user[0] = '\0';
  DomoticzConfiguration.password[0] = '\0';
  DomoticzConfiguration.port = 8080;
  saveConfiguration(DomoticzConfiguration);
}

#if (AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS > 0)
LED AFEDataAccess::getLEDConfiguration(uint8_t id)
{
  LED configuration;

  char fileName[15];
  sprintf(fileName, "cfg-led-%d.json", id);

#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Reading File -------------------";
  Serial << endl
         << "Opening file: " << fileName << " : ";
#endif

  File configFile = SPIFFS.open(fileName, "r");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Reading JSON : ";
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<50> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success())
    {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      configuration.gpio = root["gpio"];
      configuration.changeToOppositeValue = root["changeToOppositeValue"];

#ifdef DEBUG
      Serial << endl
             << "success" << endl
             << "JSON Buffer size: " << jsonBuffer.size();
#endif
    }

#ifdef DEBUG
    else
    {
      Serial << "failure";
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else
  {
    Serial << "failure";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif
  return configuration;
}
void AFEDataAccess::saveConfiguration(uint8_t id, LED configuration)
{
  char fileName[15];
  sprintf(fileName, "cfg-led-%d.json", id);

#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Writing File -------------------";
  Serial << endl
         << "Opening file: " << fileName << " : ";
#endif

  File configFile = SPIFFS.open(fileName, "w");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Writing JSON : ";
#endif

    StaticJsonBuffer<50> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["gpio"] = configuration.gpio;
    root["changeToOppositeValue"] = configuration.changeToOppositeValue;
    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    Serial << endl
           << "success" << endl
           << "JSON Buffer size: " << jsonBuffer.size();
#endif
  }
#ifdef DEBUG
  else
  {
    Serial << endl
           << "failed to open file for writing";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif
}
void AFEDataAccess::createLEDConfigurationFile()
{
#ifdef DEBUG
  Serial << endl
         << "Creating file: cfg-led.json";
#endif
  LED LEDConfiguration;
  uint8_t index = 0;
  LEDConfiguration.changeToOppositeValue = false;
  LEDConfiguration.gpio = 2;
#if defined(AFE_DEVICE_SONOFF_BASIC_V1)
  LEDConfiguration.gpio = 13;
  saveConfiguration(0, LEDConfiguration);
  LEDConfiguration.gpio = 14;
  saveConfiguration(1, LEDConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS;
#elif defined(AFE_DEVICE_SONOFF_4CH)
  LEDConfiguration.gpio = 13;
  saveConfiguration(0, LEDConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS;
#elif defined(AFE_DEVICE_SONOFF_TOUCH_1G)
  LEDConfiguration.gpio = 13;
  saveConfiguration(0, LEDConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS;
#elif defined(AFE_DEVICE_SONOFF_TOUCH_2G)
  LEDConfiguration.gpio = 13;
  saveConfiguration(0, LEDConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS;
#elif defined(AFE_DEVICE_SONOFF_TOUCH_3G)
  LEDConfiguration.gpio = 13;
  saveConfiguration(0, LEDConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS;
#elif defined(AFE_DEVICE_iECSv20)
  LEDConfiguration.changeToOppositeValue = true;
  saveConfiguration(0, LEDConfiguration);
  LEDConfiguration.changeToOppositeValue = false;
  index = AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_LEDS;
#endif

  for (uint8_t i = index; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_LEDS; i++)
  {
    saveConfiguration(i, LEDConfiguration);
  }
}
uint8_t AFEDataAccess::getSystemLedID()
{
  uint8_t id = 0;

#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Reading File -------------------";
  Serial << endl
         << "Opening file: cfg-system-led.json : ";
#endif

  File configFile = SPIFFS.open("cfg-system-led.json", "r");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Reading JSON : ";
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<34> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success())
    {
#ifdef DEBUG
      root.printTo(Serial);
#endif
      id = root["id"];

#ifdef DEBUG
      Serial << endl
             << "success" << endl
             << "JSON Buffer size: " << jsonBuffer.size();
#endif
    }

#ifdef DEBUG
    else
    {
      Serial << "failure";
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else
  {
    Serial << "failure";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif
  return id;
}
void AFEDataAccess::saveSystemLedID(uint8_t id)
{
#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Writing File -------------------";
  Serial << endl
         << "Opening file: cfg-system-led.json : ";
#endif

  File configFile = SPIFFS.open("cfg-system-led.json", "w");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Writing JSON : ";
#endif

    StaticJsonBuffer<34> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["id"] = id;

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    Serial << endl
           << "success" << endl
           << "JSON Buffer size: " << jsonBuffer.size();
#endif
  }
#ifdef DEBUG
  else
  {
    Serial << endl
           << "failed to open file for writing";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif
}
void AFEDataAccess::createSystemLedIDConfigurationFile()
{
#ifdef DEBUG
  Serial << endl
         << "Creating file: cfg-system-led.json";
#endif
  saveSystemLedID(0);
}
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
#define AFE_CONFIG_FILE_BUFFER_RELAY 240
RELAY AFEDataAccess::getRelayConfiguration(uint8_t id)
{
  RELAY configuration;
  char fileName[17];
  sprintf(fileName, "cfg-relay-%d.json", id);

#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Reading File -------------------";
  Serial << endl
         << "Opening file: " << fileName << " : ";
#endif

  File configFile = SPIFFS.open(fileName, "r");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Reading JSON : ";
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_RELAY> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success())
    {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      configuration.gpio = root["gpio"];
      sprintf(configuration.name, root["name"]);
      configuration.timeToOff = root["timeToOff"];
      configuration.state.powerOn = root["statePowerOn"];
      configuration.state.MQTTConnected = root["stateMQTTConnected"];
      configuration.domoticz.idx = root["idx"];
      sprintf(configuration.mqtt.topic, root["MQTTTopic"]);
      configuration.ledID = root["ledID"];

#ifdef DEBUG
      Serial << endl
             << "success" << endl
             << "JSON Buffer size: " << jsonBuffer.size() << endl
             << (AFE_CONFIG_FILE_BUFFER_RELAY < jsonBuffer.size() + 10
                     ? "WARNING: Buffor size might be to small"
                     : "Buffor size: OK");
#endif
    }

#ifdef DEBUG
    else
    {
      Serial << "failure";
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else
  {
    Serial << "failure";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif
  /*
  #if defined(T3_CONFIG)
    uint8_t nextRelay = 21;
  #elif defined(T4_CONFIG)
    uint8_t nextRelay = 27;
  #endif

  #if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG)
    configuration.gpio = Eeprom.readUInt8(370);
  #elif defined(T1_CONFIG)
    configuration.gpio = Eeprom.readUInt8(397);
  #elif defined(T2_CONFIG)
    configuration.gpio = Eeprom.readUInt8(405);
  #elif defined(T3_CONFIG)
    configuration.gpio = Eeprom.readUInt8(382 + id * nextRelay);
  #elif defined(T4_CONFIG)
    configuration.gpio = Eeprom.readUInt8(383 + id * nextRelay);
  #elif defined(T5_CONFIG)
    configuration.gpio = Eeprom.readUInt8(462);
  #elif defined(T6_CONFIG)
    configuration.gpio = Eeprom.readUInt8(374);
  #endif

  #if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG)
    configuration.timeToOff = Eeprom.read(372, 5).toFloat();
  #elif defined(T1_CONFIG)
    configuration.timeToOff = Eeprom.read(399, 5).toFloat();
  #elif defined(T2_CONFIG)
    configuration.timeToOff = Eeprom.read(407, 5).toFloat();
  #elif defined(T4_CONFIG)
    configuration.timeToOff = Eeprom.read(385 + id * nextRelay, 5).toFloat();
  #elif defined(T5_CONFIG)
    configuration.timeToOff = Eeprom.read(463, 4).toInt();
  #endif


  #if !defined(T5_CONFIG)

  #if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG)
    configuration.statePowerOn = Eeprom.readUInt8(377);
  #elif defined(T1_CONFIG)
    configuration.statePowerOn = Eeprom.readUInt8(404);
  #elif defined(T2_CONFIG)
    configuration.statePowerOn = Eeprom.readUInt8(412);
  #elif defined(T3_CONFIG)
    configuration.statePowerOn = Eeprom.readUInt8(384 + id * nextRelay);
  #elif defined(T4_CONFIG)
    configuration.statePowerOn = Eeprom.readUInt8(390 + id * nextRelay);
  #elif defined(T6_CONFIG)
    configuration.statePowerOn = Eeprom.readUInt8(376);
  #endif

  #if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG)
    Eeprom.read(378, 16).toCharArray(configuration.name,
                                     sizeof(configuration.name));
  #elif defined(T1_CONFIG)
    Eeprom.read(405, 16).toCharArray(configuration.name,
                                     sizeof(configuration.name));
  #elif defined(T2_CONFIG)
    Eeprom.read(413, 16).toCharArray(configuration.name,
                                     sizeof(configuration.name));
  #elif defined(T3_CONFIG)
    Eeprom.read(385 + id * nextRelay, 16)
        .toCharArray(configuration.name, sizeof(configuration.name));
  #elif defined(T4_CONFIG)
    Eeprom.read(391 + id * nextRelay, 16)
        .toCharArray(configuration.name, sizeof(configuration.name));
  #elif defined(T6_CONFIG)
    Eeprom.read(377, 16).toCharArray(configuration.name,
                                     sizeof(configuration.name));
  #endif

  #if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG)
    configuration.stateMQTTConnected = Eeprom.readUInt8(394);
  #elif defined(T1_CONFIG)
    configuration.stateMQTTConnected = Eeprom.readUInt8(421);
  #elif defined(T2_CONFIG)
    configuration.stateMQTTConnected = Eeprom.readUInt8(429);
  #elif defined(T3_CONFIG)
    configuration.stateMQTTConnected = Eeprom.readUInt8(401 + id * nextRelay);
  #elif defined(T4_CONFIG)
    configuration.stateMQTTConnected = Eeprom.readUInt8(407 + id * nextRelay);
  #elif defined(T6_CONFIG)
    configuration.stateMQTTConnected = Eeprom.readUInt8(393);
  #endif

    Eeprom.read(334, 32).toCharArray(configurationMQTT.topic,
                                     sizeof(configurationMQTT.topic));



  #if defined(T0_CONFIG)
    configuration.ledID = Eeprom.readUInt8(421);
  #elif defined(T1_CONFIG)
    configuration.ledID = Eeprom.readUInt8(442);
  #elif defined(T2_CONFIG)
    configuration.ledID = Eeprom.readUInt8(463);
  #elif defined(T3_CONFIG)
    configuration.ledID = Eeprom.readUInt8(618 + id);
  #elif defined(T4_CONFIG)
    configuration.ledID = Eeprom.readUInt8(531 + id);
  #elif defined(T6_CONFIG)
    configuration.ledID = Eeprom.readUInt8(394);
  #endif

  #if !defined(T6_CONFIG)
    configuration.idx = Eeprom.read(930 + 6 * id, 6).toInt();
  #else
    configuration.idx = Eeprom.read(920, 6).toInt();
  #endif

  #if defined(T1_CONFIG) || defined(T2_CONFIG)
  #if defined(T1_CONFIG)
    configuration.thermostat.turnOn = Eeprom.read(423, 5).toFloat();
    configuration.thermostat.turnOff = Eeprom.read(428, 5).toFloat();
    configuration.thermostat.turnOnAbove = Eeprom.read(433);
    configuration.thermostat.turnOffAbove = Eeprom.read(434);
    configuration.thermostat.enabled =
  isRegulatorEnabled(THERMOSTAT_REGULATOR); #else
    configuration.thermostat.turnOn = Eeprom.read(431, 5).toFloat();
    configuration.thermostat.turnOff = Eeprom.read(436, 5).toFloat();
    configuration.thermostat.turnOnAbove = Eeprom.read(441);
    configuration.thermostat.turnOffAbove = Eeprom.read(442);
    configuration.thermostat.enabled =
  isRegulatorEnabled(THERMOSTAT_REGULATOR); #endif #endif

  #if defined(T2_CONFIG)
    configuration.humidistat.turnOn = Eeprom.read(444, 5).toFloat();
    configuration.humidistat.turnOff = Eeprom.read(449, 5).toFloat();
    configuration.humidistat.turnOnAbove = Eeprom.read(454);
    configuration.humidistat.turnOffAbove = Eeprom.read(455);
    configuration.humidistat.enabled =
  isRegulatorEnabled(HUMIDISTAT_REGULATOR); #endif


  #if defined(T1_CONFIG)
    configuration.thermalProtection = Eeprom.read(436, 3).toInt();
  #elif defined(T2_CONFIG)
    configuration.thermalProtection = Eeprom.read(457, 3).toInt();
  #endif

  #endif
  */

  return configuration;
}
void AFEDataAccess::saveConfiguration(uint8_t id, RELAY configuration)
{
  char fileName[17];
  sprintf(fileName, "cfg-relay-%d.json", id);

#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Writing File -------------------";
  Serial << endl
         << "Opening file: " << fileName << " : ";
#endif

  File configFile = SPIFFS.open(fileName, "w");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Writing JSON : ";
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_RELAY> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["gpio"] = configuration.gpio;
    root["name"] = configuration.name;
    root["timeToOff"] = configuration.timeToOff;
    root["statePowerOn"] = configuration.state.powerOn;
    root["stateMQTTConnected"] = configuration.state.MQTTConnected;
    root["ledID"] = configuration.ledID;
    root["idx"] = configuration.domoticz.idx;
    root["MQTTTopic"] = configuration.mqtt.topic;

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    Serial << endl
           << "success" << endl
           << "JSON Buffer size: " << jsonBuffer.size() << endl
           << (AFE_CONFIG_FILE_BUFFER_RELAY < jsonBuffer.size() + 10
                   ? "WARNING: Buffor size might be to small"
                   : "Buffor size: OK");
#endif
  }
#ifdef DEBUG
  else
  {
    Serial << endl
           << "failed to open file for writing";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif
  /*
  #if defined(T3_CONFIG)
    uint8_t nextRelay = 21;
  #elif defined(T4_CONFIG)
    uint8_t nextRelay = 27;
  #endif

  #if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG)
    Eeprom.writeUInt8(370, configuration.gpio);
  #elif defined(T1_CONFIG)
    Eeprom.writeUInt8(397, configuration.gpio);
  #elif defined(T2_CONFIG)
    Eeprom.writeUInt8(405, configuration.gpio);
  #elif defined(T3_CONFIG)
    Eeprom.writeUInt8(382 + id * nextRelay, configuration.gpio);
  #elif defined(T4_CONFIG)
    Eeprom.writeUInt8(383 + id * nextRelay, configuration.gpio);
  #elif defined(T5_CONFIG)
    Eeprom.writeUInt8(462, configuration.gpio);
  #elif defined(T6_CONFIG)
    Eeprom.writeUInt8(374, configuration.gpio);
  #endif

  #if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG)
    Eeprom.write(372, 5, configuration.timeToOff);
  #elif defined(T1_CONFIG)
    Eeprom.write(399, 5, configuration.timeToOff);
  #elif defined(T2_CONFIG)
    Eeprom.write(407, 5, configuration.timeToOff);
  #elif defined(T4_CONFIG)
    Eeprom.write(385 + id * nextRelay, 5, configuration.timeToOff);
  #elif defined(T5_CONFIG)
    Eeprom.write(463, 4, configuration.timeToOff);
  #endif

  #if !defined(T5_CONFIG)

  #if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG)
    Eeprom.writeUInt8(377, configuration.statePowerOn);
  #elif defined(T1_CONFIG)
    Eeprom.writeUInt8(404, configuration.statePowerOn);
  #elif defined(T2_CONFIG)
    Eeprom.writeUInt8(412, configuration.statePowerOn);
  #elif defined(T3_CONFIG)
    Eeprom.writeUInt8(384 + id * nextRelay, configuration.statePowerOn);
  #elif defined(T4_CONFIG)
    Eeprom.writeUInt8(390 + id * nextRelay, configuration.statePowerOn);
  #elif defined(T6_CONFIG)
    Eeprom.writeUInt8(376, configuration.statePowerOn);
  #endif

  #if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG)
    Eeprom.write(378, 16, configuration.name);
  #elif defined(T1_CONFIG)
    Eeprom.write(405, 16, configuration.name);
  #elif defined(T2_CONFIG)
    Eeprom.write(413, 16, configuration.name);
  #elif defined(T3_CONFIG)
    Eeprom.write(385 + id * nextRelay, 16, configuration.name);
  #elif defined(T4_CONFIG)
    Eeprom.write(391 + id * nextRelay, 16, configuration.name);
  #elif defined(T6_CONFIG)
    Eeprom.write(377, 16, configuration.name);
  #endif

  #if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG)
    Eeprom.writeUInt8(394, configuration.stateMQTTConnected);
  #elif defined(T1_CONFIG)
    Eeprom.writeUInt8(421, configuration.stateMQTTConnected);
  #elif defined(T2_CONFIG)
    Eeprom.writeUInt8(429, configuration.stateMQTTConnected);
  #elif defined(T3_CONFIG)
    Eeprom.writeUInt8(401 + id * nextRelay, configuration.stateMQTTConnected);
  #elif defined(T4_CONFIG)
    Eeprom.writeUInt8(407 + id * nextRelay, configuration.stateMQTTConnected);
  #elif defined(T6_CONFIG)
    Eeprom.writeUInt8(393, configuration.stateMQTTConnected);
  #endif

  #if defined(T0_CONFIG)
    Eeprom.writeUInt8(421, configuration.ledID);
  #elif defined(T1_CONFIG)
    Eeprom.writeUInt8(442, configuration.ledID);
  #elif defined(T2_CONFIG)
    Eeprom.writeUInt8(463, configuration.ledID);
  #elif defined(T3_CONFIG)
    Eeprom.writeUInt8(618 + id, configuration.ledID);
  #elif defined(T4_CONFIG)
    Eeprom.writeUInt8(531 + id, configuration.ledID);
  #elif defined(T6_CONFIG)
    Eeprom.writeUInt8(394, configuration.ledID);
  #endif

  #if !defined(T6_CONFIG)
    Eeprom.write(930 + 6 * id, 6, (long)configuration.idx);
  #else
    Eeprom.write(920, 6, (long)configuration.idx);
  #endif

  #if defined(T1_CONFIG)
    Eeprom.write(436, 3, configuration.thermalProtection);
  #elif defined(T2_CONFIG)
    Eeprom.write(457, 3, configuration.thermalProtection);
  #endif

  #endif
  */
}
void AFEDataAccess::createRelayConfigurationFile()
{
#ifdef DEBUG
  Serial << endl
         << "Creating file: cfg-relay-xx.json";
#endif
  RELAY RelayConfiguration;
  uint8_t index = 0;
  /* Relay config */

  RelayConfiguration.timeToOff = 0;
  RelayConfiguration.state.powerOn = 3;
  RelayConfiguration.state.MQTTConnected = 0;
  RelayConfiguration.ledID = AFE_HARDWARE_ITEM_NOT_EXIST;
  RelayConfiguration.domoticz.idx = 0;
  RelayConfiguration.mqtt.topic[0] = '\0';
  sprintf(RelayConfiguration.name, "R1");
#if defined(T1_CONFIG) || defined(T2_CONFIG)
  RelayConfiguration.thermalProtection = 0;
#endif

/* SONOFF Basic v1 */
#if defined(AFE_DEVICE_SONOFF_BASIC_V1)
  RelayConfiguration.gpio = 12;
#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY
  saveRelayState(0, false);
#endif
  saveConfiguration(0, RelayConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS;
/* SONOFF 4CH */
#elif defined(AFE_DEVICE_SONOFF_4CH)
  RelayConfiguration.gpio = 12;
#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY
  saveRelayState(0, false);
#endif
  saveConfiguration(0, RelayConfiguration);

  RelayConfiguration.gpio = 5;
  sprintf(RelayConfiguration.name, "R2");
#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY
  saveRelayState(1, false);
#endif
  saveConfiguration(1, RelayConfiguration);
  RelayConfiguration.gpio = 4;
  sprintf(RelayConfiguration.name, "R3");
#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY
  saveRelayState(2, false);
#endif
  saveConfiguration(2, RelayConfiguration);

  RelayConfiguration.gpio = 15;
  sprintf(RelayConfiguration.name, "R4");
#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY
  saveRelayState(3, false);
#endif
  saveConfiguration(3, RelayConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS;
/* SONOFF Touch 1G */
#elif defined(AFE_DEVICE_SONOFF_TOUCH_1G)
  RelayConfiguration.gpio = 12;
#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY
  saveRelayState(0, false);
#endif
  saveConfiguration(0, RelayConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS;
/* SONOFF Touch 2G */
#elif defined(AFE_DEVICE_SONOFF_TOUCH_2G)
  RelayConfiguration.gpio = 12;
#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY
  saveRelayState(0, false);
#endif
  saveConfiguration(0, RelayConfiguration);

  RelayConfiguration.gpio = 5;
  sprintf(RelayConfiguration.name, "R2");
#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY
  saveRelayState(1, false);
#endif
  saveConfiguration(1, RelayConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS;
/* SONOFF Touch 3G */
#elif defined(AFE_DEVICE_SONOFF_TOUCH_3G)
  RelayConfiguration.gpio = 12;
#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY
  saveRelayState(0, false);
#endif
  saveConfiguration(0, RelayConfiguration);

  RelayConfiguration.gpio = 5;
  sprintf(RelayConfiguration.name, "R2");
#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY
  saveRelayState(1, false);
#endif
  saveConfiguration(1, RelayConfiguration);
  RelayConfiguration.gpio = 4;
  sprintf(RelayConfiguration.name, "R3");
#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY
  saveRelayState(2, false);
#endif
  saveConfiguration(2, RelayConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS;
  /* Shelly-1 */

#elif defined(AFE_DEVICE_SHELLY_1)
  RelayConfiguration.gpio = 4;
#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY
  saveRelayState(0, false);
#endif
  saveConfiguration(0, RelayConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS;
#elif defined(AFE_DEVICE_iECSv20)
  RelayConfiguration.gpio = 15;
  sprintf(RelayConfiguration.name, "R2");
  saveRelayState(1, false);
  saveConfiguration(1, RelayConfiguration);
  RelayConfiguration.gpio = 12;
  sprintf(RelayConfiguration.name, "R1");
  RelayConfiguration.timeToOff = 200;
  RelayConfiguration.state.powerOn = 0;
  saveRelayState(0, false);
  saveConfiguration(0, RelayConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS;
/* Clean */
#endif
  RelayConfiguration.gpio = 12;
  for (uint8_t i = index; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_RELAYS; i++)
  {
#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY
    saveRelayState(i, false);
#endif
    sprintf(RelayConfiguration.name, "R%d", i + 1);
    saveConfiguration(i, RelayConfiguration);
  }
}

/* Relay state methods*/
boolean AFEDataAccess::getRelayState(uint8_t id)
{
  boolean state = false;
  char fileName[23];
  sprintf(fileName, "cfg-relay-state-%d.json", id);

#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Reading File -------------------";
  Serial << endl
         << "Opening file: " << fileName << " : ";
#endif

  File configFile = SPIFFS.open(fileName, "r");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Reading JSON : ";
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<34> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success())
    {
#ifdef DEBUG
      root.printTo(Serial);
#endif
      state = root["state"];

#ifdef DEBUG
      Serial << endl
             << "success" << endl
             << "JSON Buffer size: " << jsonBuffer.size();
#endif
    }

#ifdef DEBUG
    else
    {
      Serial << "failure";
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else
  {
    Serial << "failure";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif
  return state;
}
void AFEDataAccess::saveRelayState(uint8_t id, boolean state)
{
  char fileName[23];
  sprintf(fileName, "cfg-relay-state-%d.json", id);

#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Writing File -------------------";
  Serial << endl
         << "Opening file: " << fileName << " : ";
#endif

  File configFile = SPIFFS.open(fileName, "w");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Writing JSON : ";
#endif

    StaticJsonBuffer<50> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();

    root["state"] = state;

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    Serial << endl
           << "success" << endl
           << "JSON Buffer size: " << jsonBuffer.size();
#endif
  }
#ifdef DEBUG
  else
  {
    Serial << endl
           << "failed to open file for writing";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif
}
#endif /* End: Relay */

#define AFE_CONFIG_FILE_BUFFER_SWITCH 210
SWITCH AFEDataAccess::getSwitchConfiguration(uint8_t id)
{
  SWITCH configuration;

  char fileName[18];
  sprintf(fileName, "cfg-switch-%d.json", id);

#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Reading File -------------------";
  Serial << endl
         << "Opening file: " << fileName << " : ";
#endif

  File configFile = SPIFFS.open(fileName, "r");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Reading JSON : ";
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_SWITCH> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success())
    {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      configuration.gpio = root["gpio"];
      configuration.type = root["type"];
      configuration.sensitiveness = root["sensitiveness"];
      configuration.functionality = root["functionality"];
      configuration.relayID = root["relayID"];
      sprintf(configuration.mqtt.topic, root["MQTTTopic"]);
      configuration.domoticz.idx = root["idx"];

#ifdef DEBUG
      Serial << endl
             << "success" << endl
             << "JSON Buffer size: " << jsonBuffer.size() << endl
             << (AFE_CONFIG_FILE_BUFFER_SWITCH < jsonBuffer.size() + 10
                     ? "WARNING: Buffor size might be to small"
                     : "Buffor size: OK");
#endif
    }

#ifdef DEBUG
    else
    {
      Serial << "failure";
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else
  {
    Serial << "failure";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif

  return configuration;
}
void AFEDataAccess::saveConfiguration(uint8_t id, SWITCH configuration)
{
  char fileName[18];
  sprintf(fileName, "cfg-switch-%d.json", id);

#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Writing File -------------------";
  Serial << endl
         << "Opening file: " << fileName << " : ";
#endif

  File configFile = SPIFFS.open(fileName, "w");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Writing JSON : ";
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_SWITCH> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["gpio"] = configuration.gpio;
    root["type"] = configuration.type;
    root["sensitiveness"] = configuration.sensitiveness;
    root["functionality"] = configuration.functionality;
    root["relayID"] = configuration.relayID;
    root["idx"] = configuration.domoticz.idx;
    root["MQTTTopic"] = configuration.mqtt.topic;

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    Serial << endl
           << "success" << endl
           << "JSON Buffer size: " << jsonBuffer.size() << endl
           << (AFE_CONFIG_FILE_BUFFER_SWITCH < jsonBuffer.size() + 10
                   ? "WARNING: Buffor size might be to small"
                   : "Buffor size: OK");
#endif
  }
#ifdef DEBUG
  else
  {
    Serial << endl
           << "failed to open file for writing";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif
}
void AFEDataAccess::createSwitchConfigurationFile()
{
#ifdef DEBUG
  Serial << endl
         << "Creating file: cfg-switch.json";
#endif
  SWITCH SwitchConfiguration;
  uint8_t index = 0;

  SwitchConfiguration.sensitiveness = AFE_SWITCH_BOUNCING;
  SwitchConfiguration.relayID = 0;
  SwitchConfiguration.mqtt.topic[0] = '\0';
  SwitchConfiguration.domoticz.idx = 0;

#if defined(AFE_DEVICE_SONOFF_BASIC_V1)
  SwitchConfiguration.gpio = 0;
  SwitchConfiguration.type = AFE_SWITCH_TYPE_MONO;
  SwitchConfiguration.functionality = AFE_SWITCH_FUNCTIONALITY_MULTI;
  saveConfiguration(0, SwitchConfiguration);
  SwitchConfiguration.gpio = 14;
  SwitchConfiguration.type = AFE_SWITCH_TYPE_BI;
  SwitchConfiguration.functionality = AFE_SWITCH_FUNCTIONALITY_RELAY;
  saveConfiguration(1, SwitchConfiguration);
  SwitchConfiguration.gpio = 1;
  saveConfiguration(2, SwitchConfiguration);
  SwitchConfiguration.gpio = 3;
  saveConfiguration(3, SwitchConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES;
#elif defined(AFE_DEVICE_SONOFF_4CH)
  SwitchConfiguration.gpio = 0;
  SwitchConfiguration.type = AFE_SWITCH_TYPE_MONO;
  SwitchConfiguration.functionality = AFE_SWITCH_FUNCTIONALITY_MULTI;
  saveConfiguration(0, SwitchConfiguration);
  SwitchConfiguration.gpio = 9;
  SwitchConfiguration.functionality = AFE_SWITCH_FUNCTIONALITY_RELAY;
  SwitchConfiguration.relayID = 1;
  saveConfiguration(1, SwitchConfiguration);
  SwitchConfiguration.gpio = 10;
  SwitchConfiguration.relayID = 2;
  saveConfiguration(2, SwitchConfiguration);
  SwitchConfiguration.gpio = 14;
  SwitchConfiguration.relayID = 3;
  saveConfiguration(3, SwitchConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES;
#elif defined(AFE_DEVICE_SONOFF_TOUCH_1G)
  SwitchConfiguration.gpio = 0;
  SwitchConfiguration.type = AFE_SWITCH_TYPE_MONO;
  SwitchConfiguration.functionality = AFE_SWITCH_FUNCTIONALITY_MULTI;
  saveConfiguration(0, SwitchConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES;
#elif defined(AFE_DEVICE_SONOFF_TOUCH_2G)
  SwitchConfiguration.gpio = 0;
  SwitchConfiguration.type = AFE_SWITCH_TYPE_MONO;
  SwitchConfiguration.functionality = AFE_SWITCH_FUNCTIONALITY_MULTI;
  saveConfiguration(0, SwitchConfiguration);
  SwitchConfiguration.gpio = 9;
  SwitchConfiguration.functionality = AFE_SWITCH_FUNCTIONALITY_RELAY;
  SwitchConfiguration.relayID = 1;
  saveConfiguration(1, SwitchConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES;
#elif defined(AFE_DEVICE_SONOFF_TOUCH_3G)
  SwitchConfiguration.gpio = 0;
  SwitchConfiguration.type = AFE_SWITCH_TYPE_MONO;
  SwitchConfiguration.functionality = AFE_SWITCH_FUNCTIONALITY_MULTI;
  saveConfiguration(0, SwitchConfiguration);
  SwitchConfiguration.gpio = 9;
  SwitchConfiguration.functionality = AFE_SWITCH_FUNCTIONALITY_RELAY;
  SwitchConfiguration.relayID = 1;
  saveConfiguration(1, SwitchConfiguration);
  SwitchConfiguration.gpio = 10;
  SwitchConfiguration.relayID = 2;
  saveConfiguration(2, SwitchConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES;
#elif defined(AFE_DEVICE_SHELLY_1)
  SwitchConfiguration.gpio = 5;
  SwitchConfiguration.type = AFE_SWITCH_TYPE_BI;
  SwitchConfiguration.functionality = AFE_SWITCH_FUNCTIONALITY_RELAY;
  saveConfiguration(0, SwitchConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES;
#elif defined(AFE_DEVICE_iECSv20)
  SwitchConfiguration.gpio = 0;
  SwitchConfiguration.type = AFE_SWITCH_TYPE_MONO;
  SwitchConfiguration.functionality = AFE_SWITCH_FUNCTIONALITY_MULTI;
  saveConfiguration(0, SwitchConfiguration);
  SwitchConfiguration.gpio = 1;
  SwitchConfiguration.functionality = AFE_SWITCH_FUNCTIONALITY_NONE;
  SwitchConfiguration.relayID = AFE_HARDWARE_ITEM_NOT_EXIST;
  saveConfiguration(1, SwitchConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES;
#endif
  if (index == 0)
  {
    SwitchConfiguration.gpio = 0;
    SwitchConfiguration.type = AFE_SWITCH_TYPE_MONO;
    SwitchConfiguration.functionality = AFE_SWITCH_FUNCTIONALITY_MULTI;
    saveConfiguration(0, SwitchConfiguration);
    index = 1;
  }
  if (index < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_SWITCHES)
  {
    SwitchConfiguration.gpio = 0;
    SwitchConfiguration.type = AFE_SWITCH_TYPE_BI;
    SwitchConfiguration.functionality = AFE_SWITCH_FUNCTIONALITY_NONE;
    for (uint8_t i = index; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_SWITCHES;
         i++)
    {
      SwitchConfiguration.relayID = AFE_HARDWARE_ITEM_NOT_EXIST;
      saveConfiguration(i, SwitchConfiguration);
    }
  }
}

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
ADCINPUT AFEDataAccess::getADCInputConfiguration()
{
  ADCINPUT configuration;

#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Reading File -------------------";
  Serial << endl
         << "Opening file: cfg-analog-input.json : ";
#endif

  File configFile = SPIFFS.open("cfg-analog-input.json", "r");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Reading JSON : ";
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<300> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success())
    {
#ifdef DEBUG
      root.printTo(Serial);
#endif
      configuration.gpio = root["gpio"];
      configuration.interval = root["interval"];
      configuration.numberOfSamples = root["numberOfSamples"];
      configuration.maxVCC = root["maxVCC"];
      sprintf(configuration.mqtt.topic, root["mqttTopic"]);
      configuration.domoticz.raw = root["idx"]["raw"];
      configuration.domoticz.percent = root["idx"]["percent"];
      configuration.domoticz.voltage = root["idx"]["voltage"];
      configuration.domoticz.voltageCalculated =
          root["idx"]["voltageCalculated"];
      configuration.divider.Ra = root["divider"]["Ra"];
      configuration.divider.Rb = root["divider"]["Rb"];
#ifdef DEBUG
      Serial << endl
             << "success" << endl
             << "JSON Buffer size: " << jsonBuffer.size();
#endif
    }

#ifdef DEBUG
    else
    {
      Serial << "failure";
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else
  {
    Serial << "failure";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif

  return configuration;
}
void AFEDataAccess::saveConfiguration(ADCINPUT configuration)
{
#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Writing File -------------------";
  Serial << endl
         << "Opening file: cfg-analog-input.json : ";
#endif

  File configFile = SPIFFS.open("cfg-analog-input.json", "w");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Writing JSON : ";
#endif

    StaticJsonBuffer<350> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    JsonObject &idx = root.createNestedObject("idx");
    JsonObject &divider = root.createNestedObject("divider");

    root["gpio"] = configuration.gpio;
    root["interval"] = configuration.interval;
    root["numberOfSamples"] = configuration.numberOfSamples;
    root["maxVCC"] = configuration.maxVCC;
    root["mqttTopic"] = configuration.mqtt.topic;
    idx["raw"] = configuration.domoticz.raw;
    idx["percent"] = configuration.domoticz.percent;
    idx["voltage"] = configuration.domoticz.voltage;
    idx["voltageCalculated"] = configuration.domoticz.voltageCalculated;
    divider["Ra"] = configuration.divider.Ra;
    divider["Rb"] = configuration.divider.Rb;
    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    Serial << endl
           << "success" << endl
           << "JSON Buffer size: " << jsonBuffer.size();
#endif
  }
#ifdef DEBUG
  else
  {
    Serial << endl
           << "failed to open file for writing";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif
};
void AFEDataAccess::createADCInputConfigurationFile()
{
#ifdef DEBUG
  Serial << endl
         << "Creating file: cfg-analog-input.json";
#endif
  ADCINPUT AnalogInputConfiguration;
  AnalogInputConfiguration.gpio = AFE_CONFIG_HARDWARE_ADC_VCC_DEFAULT_GPIO;
  AnalogInputConfiguration.interval =
      AFE_CONFIG_HARDWARE_ADC_VCC_DEFAULT_INTERVAL;
  AnalogInputConfiguration.numberOfSamples =
      AFE_CONFIG_HARDWARE_ADC_VCC_DEFAULT_NUMBER_OF_SAMPLES;
  AnalogInputConfiguration.maxVCC = AFE_CONFIG_HARDWARE_ADC_VCC_DEFAULT_MAX_VCC;
  sprintf(AnalogInputConfiguration.mqtt.topic, "analog");
  AnalogInputConfiguration.domoticz.raw = 0;
  AnalogInputConfiguration.domoticz.voltage = 0;
  AnalogInputConfiguration.domoticz.percent = 0;
  AnalogInputConfiguration.domoticz.voltageCalculated = 0;
  AnalogInputConfiguration.divider.Ra = 0;
  AnalogInputConfiguration.divider.Rb = 0;
  saveConfiguration(AnalogInputConfiguration);
}
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
DS18B20 AFEDataAccess::getSensorConfiguration()
{
  DS18B20 configuration;
  configuration.gpio = Eeprom.readUInt8(370);
  configuration.correction = Eeprom.read(371, 5).toFloat();
  configuration.interval = Eeprom.read(376, 5).toInt();
  configuration.unit = Eeprom.readUInt8(381);
  configuration.sendOnlyChanges = Eeprom.read(446);
  configuration.idx = Eeprom.read(936, 6).toInt();
  return configuration;
}
void AFEDataAccess::saveConfiguration(DS18B20 configuration)
{
  Eeprom.writeUInt8(370, configuration.gpio);
  Eeprom.write(371, 5, (float)configuration.correction);
  Eeprom.write(376, 5, (long)configuration.interval);
  Eeprom.writeUInt8(381, configuration.unit);
  Eeprom.write(446, configuration.sendOnlyChanges);
  Eeprom.write(936, 6, (long)configuration.idx);
}
#endif

#ifdef AFE_CONFIG_HARDWARE_DHXX
DH AFEDataAccess::getSensorConfiguration()
{
  DH configuration;
#if defined(T2_CONFIG)
  configuration.gpio = Eeprom.readUInt8(370);
  configuration.type = Eeprom.readUInt8(371);
  configuration.interval = Eeprom.read(372, 5).toInt();
  configuration.temperature.unit = Eeprom.readUInt8(377);
  configuration.temperature.correction = Eeprom.read(378, 4).toFloat();
  configuration.humidity.correction = Eeprom.read(387, 3).toFloat();
  configuration.temperatureIdx = Eeprom.read(936, 6).toInt();
  configuration.humidityIdx = Eeprom.read(942, 6).toInt();
  configuration.temperatureAndHumidityIdx = Eeprom.read(948, 6).toInt();
  configuration.sendOnlyChanges = Eeprom.read(467);
  configuration.publishHeatIndex = Eeprom.read(974);
  configuration.publishDewPoint = Eeprom.read(382);
#else
  configuration.gpio = Eeprom.readUInt8(377);
  configuration.type = Eeprom.readUInt8(378);
  configuration.interval = Eeprom.read(379, 5).toInt();
  configuration.temperature.unit = Eeprom.readUInt8(384);
  configuration.temperature.correction = Eeprom.read(385, 4).toFloat();
  configuration.humidity.correction = Eeprom.read(394, 3).toFloat();
  configuration.temperatureIdx = Eeprom.read(954, 6).toInt();
  configuration.humidityIdx = Eeprom.read(960, 6).toInt();
  configuration.temperatureAndHumidityIdx = Eeprom.read(966, 6).toInt();
  configuration.sendOnlyChanges = Eeprom.read(397);
  configuration.publishHeatIndex = Eeprom.read(990);
  configuration.publishDewPoint = Eeprom.read(389);
#endif

  return configuration;
}
void AFEDataAccess::saveConfiguration(DH configuration)
{
#if defined(T2_CONFIG)
  Eeprom.writeUInt8(370, configuration.gpio);
  Eeprom.writeUInt8(371, configuration.type);
  Eeprom.write(372, 5, (long)configuration.interval);
  Eeprom.writeUInt8(377, configuration.temperature.unit);
  Eeprom.write(378, 4, (float)configuration.temperature.correction);
  Eeprom.write(387, 3, (float)configuration.humidity.correction);
  Eeprom.write(936, 6, (long)configuration.temperatureIdx);
  Eeprom.write(942, 6, (long)configuration.humidityIdx);
  Eeprom.write(948, 6, (long)configuration.temperatureAndHumidityIdx);
  Eeprom.write(467, configuration.sendOnlyChanges);
  Eeprom.write(974, configuration.publishHeatIndex);
  Eeprom.write(382, configuration.publishDewPoint);
#else
  Eeprom.writeUInt8(377, configuration.gpio);
  Eeprom.writeUInt8(378, configuration.type);
  Eeprom.write(379, 5, (long)configuration.interval);
  Eeprom.writeUInt8(384, configuration.temperature.unit);
  Eeprom.write(385, 4, (float)configuration.temperature.correction);
  Eeprom.write(394, 3, (float)configuration.humidity.correction);
  Eeprom.write(954, 6, (long)configuration.temperatureIdx);
  Eeprom.write(960, 6, (long)configuration.humidityIdx);
  Eeprom.write(966, 6, (long)configuration.temperatureAndHumidityIdx);
  Eeprom.write(397, configuration.sendOnlyChanges);
  Eeprom.write(990, configuration.publishHeatIndex);
  Eeprom.write(389, configuration.publishDewPoint);
#endif
}
#endif

/* T3: PIR */
#if defined(T3_CONFIG)
PIR AFEDataAccess::getPIRConfiguration(uint8_t id)
{
  PIR configuration;
  MQTT configurationMQTT;
  uint8_t nextPIR = 27;
  configuration.gpio = Eeprom.readUInt8(506 + id * nextPIR);

  Eeprom.read(507 + id * nextPIR, 16)
      .toCharArray(configuration.name, sizeof(configuration.name));

  configuration.ledId = Eeprom.readUInt8(524 + id * nextPIR);
  configuration.relayId = Eeprom.readUInt8(525 + id * nextPIR);
  configuration.howLongKeepRelayOn = Eeprom.read(526 + id * nextPIR, 5).toInt();
  configuration.invertRelayState = Eeprom.read(531 + id * nextPIR);

  configuration.type = Eeprom.readUInt8(613 + id);
  configuration.idx = Eeprom.read(954 + id * 6, 6).toInt();

  Eeprom.read(334, 32).toCharArray(configurationMQTT.topic,
                                   sizeof(configurationMQTT.topic));

  sprintf(configuration.mqttTopic, "%s%s/", configurationMQTT.topic,
          configuration.name);

  return configuration;
}
void AFEDataAccess::saveConfiguration(uint8_t id, PIR configuration)
{
  uint8_t nextPIR = 27;
  Eeprom.writeUInt8(506 + id * nextPIR, configuration.gpio);
  Eeprom.write(507 + id * nextPIR, 16, configuration.name);
  Eeprom.writeUInt8(524 + id * nextPIR, configuration.ledId);
  Eeprom.writeUInt8(525 + id * nextPIR, configuration.relayId);
  Eeprom.write(526 + id * nextPIR, 5, (long)configuration.howLongKeepRelayOn);
  Eeprom.write(531 + id * nextPIR, configuration.invertRelayState);

  Eeprom.writeUInt8(613 + id, configuration.type);
  Eeprom.write(954 + id, 6, (long)configuration.idx);
}
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
#define AFE_CONFIG_FILE_BUFFER_CONTACTRON 200
CONTACTRON AFEDataAccess::getContactronConfiguration(uint8_t id)
{
  CONTACTRON configuration;

  char fileName[22];
  sprintf(fileName, "cfg-contactron-%d.json", id);

#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Reading File -------------------";
  Serial << endl
         << "Opening file: " << fileName << " : ";
#endif

  File configFile = SPIFFS.open(fileName, "r");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Reading JSON : ";
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_CONTACTRON> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success())
    {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      configuration.gpio = root["gpio"];
      sprintf(configuration.name, root["name"]);
      configuration.type = root["type"];
      configuration.bouncing = root["bouncing"];
      configuration.ledID = root["ledID"];
      configuration.domoticz.idx = root["idx"];
      sprintf(configuration.mqtt.topic, root["MQTTTopic"]);

#ifdef DEBUG
      Serial << endl
             << "success" << endl
             << "JSON Buffer size: " << jsonBuffer.size() << endl
             << (AFE_CONFIG_FILE_BUFFER_CONTACTRON < jsonBuffer.size() + 10
                     ? "WARNING: Buffor size might be to small"
                     : "Buffor size: OK");
#endif
    }

#ifdef DEBUG
    else
    {
      Serial << "failure";
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else
  {
    Serial << "failure";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif

  return configuration;
}
void AFEDataAccess::saveConfiguration(uint8_t id, CONTACTRON configuration)
{
  char fileName[22];
  sprintf(fileName, "cfg-contactron-%d.json", id);

#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Writing File -------------------";
  Serial << endl
         << "Opening file: " << fileName << " : ";
#endif

  File configFile = SPIFFS.open(fileName, "w");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Writing JSON : ";
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_CONTACTRON> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["gpio"] = configuration.gpio;
    root["name"] = configuration.name;
    root["type"] = configuration.type;
    root["bouncing"] = configuration.bouncing;
    root["ledID"] = configuration.ledID;
    root["idx"] = configuration.domoticz.idx;
    root["MQTTTopic"] = configuration.mqtt.topic;

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    Serial << endl
           << "success" << endl
           << "JSON Buffer size: " << jsonBuffer.size() << endl
           << (AFE_CONFIG_FILE_BUFFER_CONTACTRON < jsonBuffer.size() + 10
                   ? "WARNING: Buffor size might be to small"
                   : "Buffor size: OK");
#endif
  }
#ifdef DEBUG
  else
  {
    Serial << endl
           << "failed to open file for writing";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif
}
void AFEDataAccess::createContractonConfigurationFile()
{
#ifdef DEBUG
  Serial << endl
         << "Creating file: cfg-contactron-xx.json";
#endif
  CONTACTRON ContactronConfiguration;
  uint8 index = 0;
  ContactronConfiguration.bouncing =
      AFE_CONFIG_HARDWARE_CONTACTRON_DEFAULT_BOUNCING;
  ContactronConfiguration.type =
      AFE_CONFIG_HARDWARE_CONTACTRON_DEFAULT_OUTPUT_TYPE;
  ContactronConfiguration.domoticz.idx = 0;
  ContactronConfiguration.mqtt.topic[0] = '\0';
  ContactronConfiguration.ledID = AFE_HARDWARE_ITEM_NOT_EXIST;
#if defined(AFE_DEVICE_iECSv20)
  ContactronConfiguration.gpio = 14;
  sprintf(ContactronConfiguration.name, "C1");
  saveConfiguration(0, ContactronConfiguration);
  ContactronConfiguration.gpio = 13;
  sprintf(ContactronConfiguration.name, "C2");
  saveConfiguration(1, ContactronConfiguration);
  ContactronConfiguration.gpio = 3;
  sprintf(ContactronConfiguration.name, "C3");
  saveConfiguration(2, ContactronConfiguration);
  index = AFE_CONFIG_HARDWARE_NUMBER_OF_CONTACTRONS;
#endif
  ContactronConfiguration.gpio = 0;
  for (uint8_t i = index; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_CONTACTRONS;
       i++)
  {
    sprintf(ContactronConfiguration.name, "C%d", i + 1);
    saveConfiguration(i, ContactronConfiguration);
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
#define AFE_CONFIG_FILE_BUFFER_GATE 210
GATE AFEDataAccess::getGateConfiguration(uint8_t id)
{
  GATE configuration;

  char fileName[16];
  sprintf(fileName, "cfg-gate-%d.json", id);

#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Reading File -------------------";
  Serial << endl
         << "Opening file: " << fileName << " : ";
#endif

  File configFile = SPIFFS.open(fileName, "r");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Reading JSON : ";
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_GATE> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success())
    {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      /*
      for (uint8_t i = 0; i < sizeof(configuration.state); i++) {
        configuration.state[i] = Eeprom.readUInt8(467 + i);
      }
      */

      configuration.relayId = root["relayId"];
      sprintf(configuration.name, root["name"]);
      configuration.contactron.id[0] = root["contactrons"][0];
      configuration.contactron.id[1] = root["contactrons"][1];

      for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_CONTACTRONS;
           i++)
      {
        configuration.states.state[i] = root["states"][i];
      }

      configuration.domoticz.idx = root["idx"];
      sprintf(configuration.mqtt.topic, root["MQTTTopic"]);

#ifdef DEBUG
      Serial << endl
             << "success" << endl
             << "JSON Buffer size: " << jsonBuffer.size() << endl
             << (AFE_CONFIG_FILE_BUFFER_GATE < jsonBuffer.size() + 10
                     ? "WARNING: Buffor size might be to small"
                     : "Buffor size: OK");
#endif
    }

#ifdef DEBUG
    else
    {
      Serial << "failure";
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else
  {
    Serial << "failure";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif

  return configuration;
}
void AFEDataAccess::saveConfiguration(uint8_t id, GATE configuration)
{

  char fileName[16];
  sprintf(fileName, "cfg-gate-%d.json", id);

#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Writing File -------------------";
  Serial << endl
         << "Opening file: " << fileName << " : ";
#endif

  File configFile = SPIFFS.open(fileName, "w");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Writing JSON : ";
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_GATE> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    JsonArray &jsonContactron = root.createNestedArray("contactrons");
    JsonArray &jsonStates = root.createNestedArray("states");
    /*
    for (uint8_t i = 0; i < sizeof(configuration.state); i++) {
      Eeprom.writeUInt8(467 + i, configuration.state[i]);
    }
    */
    root["relayId"] = configuration.relayId;
    root["name"] = configuration.name;

    /* Contractors selection validaton */
    if ((configuration.contactron.id[0] == AFE_HARDWARE_ITEM_NOT_EXIST &&
         configuration.contactron.id[1] != AFE_HARDWARE_ITEM_NOT_EXIST) ||
        (configuration.contactron.id[0] != AFE_HARDWARE_ITEM_NOT_EXIST &&
         configuration.contactron.id[0] == configuration.contactron.id[1]))
    {
      configuration.contactron.id[0] = configuration.contactron.id[1];
      configuration.contactron.id[1] = AFE_HARDWARE_ITEM_NOT_EXIST;
    }

    jsonContactron.add(configuration.contactron.id[0]);
    jsonContactron.add(configuration.contactron.id[1]);

    for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_CONTACTRONS;
         i++)
    {
      jsonStates.add(configuration.states.state[i]);
    }

    root["idx"] = configuration.domoticz.idx;
    root["MQTTTopic"] = configuration.mqtt.topic;

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    Serial << endl
           << "success" << endl
           << "JSON Buffer size: " << jsonBuffer.size() << endl
           << (AFE_CONFIG_FILE_BUFFER_GATE < jsonBuffer.size() + 10
                   ? "WARNING: Buffor size might be to small"
                   : "Buffor size: OK");
#endif
  }
#ifdef DEBUG
  else
  {
    Serial << endl
           << "failed to open file for writing";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif
}
void AFEDataAccess::createGateConfigurationFile()
{
#ifdef DEBUG
  Serial << endl
         << "Creating file: cfg-gate-XX.json";
#endif
  GATE GateConfiguration;

  GateConfiguration.domoticz.idx = 0;
  GateConfiguration.mqtt.topic[0] = '\0';
  GateConfiguration.contactron.id[1] = AFE_HARDWARE_ITEM_NOT_EXIST;

#if defined(AFE_DEVICE_iECSv20)
  GateConfiguration.contactron.id[0] = 0;
  GateConfiguration.relayId = 0;
  GateConfiguration.states.state[0] = AFE_GATE_OPEN;
  GateConfiguration.states.state[1] = AFE_GATE_UNKNOWN;
  GateConfiguration.states.state[2] = AFE_GATE_UNKNOWN;
  GateConfiguration.states.state[3] = AFE_GATE_CLOSED;
  sprintf(GateConfiguration.name, "G1");
  saveConfiguration(0, GateConfiguration);
  saveGateState(0, AFE_GATE_UNKNOWN);
#endif

  GateConfiguration.contactron.id[0] = AFE_HARDWARE_ITEM_NOT_EXIST;
  GateConfiguration.relayId = AFE_HARDWARE_ITEM_NOT_EXIST;
  GateConfiguration.states.state[0] = AFE_GATE_UNKNOWN;
  GateConfiguration.states.state[3] = AFE_GATE_UNKNOWN;

  for (uint8_t i = AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_GATES;
       i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_GATES; i++)
  {
    sprintf(GateConfiguration.name, "G%d", i + 1);
    saveConfiguration(i, GateConfiguration);
    saveGateState(i, AFE_GATE_UNKNOWN);
  }
}

#define AFE_CONFIG_FILE_BUFFER_GATE_STATE 34
uint8_t AFEDataAccess::getGateState(uint8_t id)
{
  uint8_t state = AFE_GATE_CLOSED;

  char fileName[22];
  sprintf(fileName, "cfg-gate-state-%d.json", id);

#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Reading File -------------------";
  Serial << endl
         << "Opening file: " << fileName << " : ";
#endif

  File configFile = SPIFFS.open(fileName, "r");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Reading JSON : ";
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_GATE_STATE> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success())
    {
#ifdef DEBUG
      root.printTo(Serial);
#endif
      state = root["state"];

#ifdef DEBUG
      Serial << endl
             << "success" << endl
             << "JSON Buffer size: " << jsonBuffer.size() << endl
             << (AFE_CONFIG_FILE_BUFFER_GATE_STATE < jsonBuffer.size() + 10
                     ? "WARNING: Buffor size might be to small"
                     : "Buffor size: OK");
#endif
    }

#ifdef DEBUG
    else
    {
      Serial << "failure";
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else
  {
    Serial << "failure";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif
  return state;
}
void AFEDataAccess::saveGateState(uint8_t id, uint8_t state)
{

  char fileName[22];
  sprintf(fileName, "cfg-gate-state-%d.json", id);

#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Writing File -------------------";
  Serial << endl
         << "Opening file: " << fileName << " : ";
#endif

  File configFile = SPIFFS.open(fileName, "w");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Writing JSON : ";
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
           << "success" << endl
           << "JSON Buffer size: " << jsonBuffer.size() << endl
           << (AFE_CONFIG_FILE_BUFFER_GATE < jsonBuffer.size() + 10
                   ? "WARNING: Buffor size might be to small"
                   : "Buffor size: OK");
#endif
  }
#ifdef DEBUG
  else
  {
    Serial << endl
           << "failed to open file for writing";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif
}
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
void AFEDataAccess::saveConfiguration(REGULATOR configuration, uint8_t type)
{

  if (type == THERMOSTAT_REGULATOR)
  {
    saveRegulatorState(configuration.enabled, THERMOSTAT_REGULATOR);
  }
  else
  {
    saveRegulatorState(configuration.enabled, HUMIDISTAT_REGULATOR);
  }

#if defined(T1_CONFIG)
  Eeprom.write(423, 5, configuration.turnOn);
  Eeprom.write(428, 5, configuration.turnOff);
  Eeprom.write(433, configuration.turnOnAbove);
  Eeprom.write(434, configuration.turnOffAbove);
#else
  uint8_t index = type == THERMOSTAT_REGULATOR ? 0 : 13;
  Eeprom.write(431 + index, 5, configuration.turnOn);
  Eeprom.write(436 + index, 5, configuration.turnOff);
  Eeprom.write(441 + index, configuration.turnOnAbove);
  Eeprom.write(442 + index, configuration.turnOffAbove);

#endif
}

boolean AFEDataAccess::isRegulatorEnabled(uint8_t type)
{
  if (type == THERMOSTAT_REGULATOR)
  {
#if defined(T1_CONFIG)
    return Eeprom.read(435);
#else
    return Eeprom.read(443);
#endif
  }
  else
  {
    return Eeprom.read(456);
  }
}

void AFEDataAccess::saveRegulatorState(boolean state, uint8_t type)
{
  if (type == THERMOSTAT_REGULATOR)
  {
#if defined(T1_CONFIG)
    Eeprom.write(435, state);
#else
    Eeprom.write(443, state);
#endif
  }
  else
  {
    Eeprom.write(456, state);
  }
}
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_API_CONTROL
void AFEDataAccess::saveAPI(uint8_t apiID, boolean state)
{
  DEVICE configuration = getDeviceConfiguration();
  if (apiID == API_HTTP)
  {
    configuration.api.http = state;
  }
  else if (apiID == API_MQTT)
  {
    configuration.api.mqtt = state;
  }
  else if (apiID == API_DOMOTICZ)
  {
    configuration.api.domoticz = state;
    if (state)
    {
      configuration.api.http = true;
    }
  }
  saveConfiguration(configuration);
}
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
#define AFE_CONFIG_FILE_BUFFER_HPMA115S0 240
HPMA115S0 AFEDataAccess::getHPMA115S0SensorConfiguration(uint8_t id)
{
  HPMA115S0 configuration;

  char fileName[21];
  sprintf(fileName, "cfg-hpma115s0-%d.json", id);

#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Reading File -------------------";
  Serial << endl
         << "Opening file: " << fileName << " : ";
#endif

  File configFile = SPIFFS.open(fileName, "r");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Reading JSON : ";
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_HPMA115S0> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success())
    {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      configuration.interval = root["interval"];
      configuration.timeToMeasure = root["timeToMeasure"];
      configuration.domoticz.pm25.idx = root["idx"]["pm25"];
      configuration.domoticz.pm10.idx = root["idx"]["pm10"];
      sprintf(configuration.mqtt.topic, root["mqttTopic"]);
      sprintf(configuration.name, root["name"]);

#ifdef DEBUG
      Serial << endl
             << "success" << endl
             << "JSON Buffer size: " << jsonBuffer.size() << endl
             << (AFE_CONFIG_FILE_BUFFER_HPMA115S0 < jsonBuffer.size() + 10
                     ? "WARNING: Buffor size might be to small"
                     : "Buffor size: OK");
#endif
    }

#ifdef DEBUG
    else
    {
      Serial << "failure";
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else
  {
    Serial << "failure";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif

  return configuration;
}
void AFEDataAccess::saveConfiguration(uint8_t id, HPMA115S0 configuration)
{
  char fileName[21];
  sprintf(fileName, "cfg-hpma115s0-%d.json", id);

#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Writing File -------------------";
  Serial << endl
         << "Opening file: " << fileName << " : ";
#endif

  File configFile = SPIFFS.open(fileName, "w");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Writing JSON : ";
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_HPMA115S0> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    JsonObject &idx = root.createNestedObject("idx");

    root["interval"] = configuration.interval;
    root["timeToMeasure"] = configuration.timeToMeasure;
    root["name"] = configuration.name;
    root["mqttTopic"] = configuration.mqtt.topic;
    idx["pm25"] = configuration.domoticz.pm25.idx;
    idx["pm10"] = configuration.domoticz.pm10.idx;

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    Serial << endl
           << "success" << endl
           << "JSON Buffer size: " << jsonBuffer.size() << endl
           << (AFE_CONFIG_FILE_BUFFER_HPMA115S0 < jsonBuffer.size() + 10
                   ? "WARNING: Buffor size might be to small"
                   : "Buffor size: OK");
#endif
  }
#ifdef DEBUG
  else
  {
    Serial << endl
           << "failed to open file for writing";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif
}
void AFEDataAccess::createHPMA115S0SensorConfigurationFile()
{
#ifdef DEBUG
  Serial << endl
         << "Creating file: cfg-hpma115s0-XX.json";
#endif
  HPMA115S0 configuration;

  configuration.interval = AFE_CONFIG_HARDWARE_HPMA115S_DEFAULT_INTERVAL;
  configuration.timeToMeasure =
      AFE_CONFIG_HARDWARE_HPMA115S_DEFAULT_TIME_TO_MEASURE;
  configuration.domoticz.pm25.idx = 0;
  configuration.domoticz.pm10.idx = 0;
  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_HPMA115S0; i++)
  {
    sprintf(configuration.mqtt.topic, "HPMA115S0/%d", i + 1);
    sprintf(configuration.name, "HPMA115S0-%d", i + 1);
    saveConfiguration(i, configuration);
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_UART
#define AFE_CONFIG_FILE_BUFFER_UART 50
SERIALPORT AFEDataAccess::getSerialPortConfiguration()
{
  SERIALPORT configuration;

#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Reading File -------------------";
  Serial << endl
         << "Opening file: cfg-uart.json : ";
#endif

  File configFile = SPIFFS.open("cfg-uart.json", "r");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Reading JSON : ";
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_UART> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success())
    {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      configuration.RXD = root["RXD"];
      configuration.TXD = root["TXD"];

#ifdef DEBUG
      Serial << endl
             << "success" << endl
             << "JSON Buffer size: " << jsonBuffer.size() << endl
             << (AFE_CONFIG_FILE_BUFFER_UART < jsonBuffer.size() + 10
                     ? "WARNING: Buffor size might be to small"
                     : "Buffor size: OK");
#endif
    }

#ifdef DEBUG
    else
    {
      Serial << "failure";
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else
  {
    Serial << "failure";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif

  return configuration;
}
void AFEDataAccess::saveConfiguration(SERIALPORT configuration)
{
#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Writing File -------------------";
  Serial << endl
         << "Opening file: cfg-uart.json : ";
#endif

  File configFile = SPIFFS.open("cfg-uart.json", "w");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Writing JSON : ";
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_UART> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["RXD"] = configuration.RXD;
    root["TXD"] = configuration.TXD;
    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    Serial << endl
           << "success" << endl
           << "JSON Buffer size: " << jsonBuffer.size() << endl
           << (AFE_CONFIG_FILE_BUFFER_UART < jsonBuffer.size() + 10
                   ? "WARNING: Buffor size might be to small"
                   : "Buffor size: OK");
#endif
  }
#ifdef DEBUG
  else
  {
    Serial << endl
           << "failed to open file for writing";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif
}
void AFEDataAccess::createSerialConfigurationFile()
{
#ifdef DEBUG
  Serial << endl
         << "Creating file: cfg-uart.json";
#endif
  SERIALPORT configuration;
  configuration.RXD = AFE_CONFIG_HARDWARE_UART_DEFAULT_RXD;
  configuration.TXD = AFE_CONFIG_HARDWARE_UART_DEFAULT_TXD;
  saveConfiguration(configuration);
}
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
#define AFE_CONFIG_FILE_BUFFER_BMEX80 548
BMEX80 AFEDataAccess::getBMEX80SensorConfiguration(uint8_t id)
{
  BMEX80 configuration;

  char fileName[17];
  sprintf(fileName, "cfg-BMEX80-%d.json", id);

#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Reading File -------------------";
  Serial << endl
         << "Opening file: " << fileName << " : ";
#endif

  File configFile = SPIFFS.open(fileName, "r");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Reading JSON : ";
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_BMEX80> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success())
    {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      configuration.type = root["type"];
      configuration.i2cAddress = root["i2cAddress"];
      sprintf(configuration.name, root["name"]);
      configuration.interval = root["interval"];
      configuration.resolution = root["resolution"];
      configuration.domoticz.temperatureHumidityPressure.idx =
          root["idx"]["temperatureHumidityPressure"];
      configuration.domoticz.temperatureHumidity.idx =
          root["idx"]["temperatureHumidity"];
      configuration.domoticz.gasResistance.idx = root["idx"]["gasResistance"];
      configuration.domoticz.temperature.idx = root["idx"]["temperature"];
      configuration.domoticz.humidity.idx = root["idx"]["humidity"];
      configuration.domoticz.pressure.idx = root["idx"]["pressure"];
      configuration.domoticz.relativePressure.idx =
          root["idx"]["relativePressure"];
      configuration.domoticz.dewPoint.idx = root["idx"]["dewPoint"];
      configuration.domoticz.heatIndex.idx = root["idx"]["heatIndex"];
      configuration.domoticz.iaq.idx = root["idx"]["iaq"];
      configuration.domoticz.staticIaq.idx = root["idx"]["staticIaq"];
      configuration.domoticz.co2Equivalent.idx = root["idx"]["co2Equivalent"];
      configuration.domoticz.breathVocEquivalent.idx =
          root["idx"]["breathVocEquivalent"];
      configuration.temperature.unit = root["temperature"]["unit"];
      configuration.temperature.correction = root["temperature"]["correction"];
      configuration.humidity.correction = root["humidity"]["correction"];
      configuration.pressure.unit = root["pressure"]["unit"];
      configuration.pressure.correction = root["pressure"]["correction"];
      configuration.seaLevelPressure = root["pressure"]["seaLevelPressure"];
      configuration.altitude = root["pressure"]["altitude"];
      sprintf(configuration.mqtt.topic, root["mqttTopic"]);

#ifdef DEBUG
      Serial << endl
             << "success" << endl
             << "JSON Buffer size: " << jsonBuffer.size() << endl
             << (AFE_CONFIG_FILE_BUFFER_BMEX80 < jsonBuffer.size() + 10
                     ? "WARNING: Buffor size might be to small"
                     : "Buffor size: OK");
#endif
    }

#ifdef DEBUG
    else
    {
      Serial << "failure";
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else
  {
    Serial << "failure";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif

  return configuration;
}
void AFEDataAccess::saveConfiguration(uint8_t id, BMEX80 configuration)
{
  char fileName[17];
  sprintf(fileName, "cfg-BMEX80-%d.json", id);

#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Writing File -------------------";
  Serial << endl
         << "Opening file: " << fileName << " : ";
#endif

  File configFile = SPIFFS.open(fileName, "w");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Writing JSON : ";
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_BMEX80> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    JsonObject &idx = root.createNestedObject("idx");
    JsonObject &temperature = root.createNestedObject("temperature");
    JsonObject &humidity = root.createNestedObject("humidity");
    JsonObject &pressure = root.createNestedObject("pressure");

    root["type"] = configuration.type;
    root["interval"] = configuration.interval;
    root["name"] = configuration.name;
    root["i2cAddress"] = configuration.i2cAddress;
    root["mqttTopic"] = configuration.mqtt.topic;
    root["resolution"] = configuration.resolution;
    idx["temperatureHumidity"] =
        configuration.domoticz.temperatureHumidity.idx;
    idx["temperatureHumidityPressure"] =
        configuration.domoticz.temperatureHumidityPressure.idx;
    idx["gasResistance"] = configuration.domoticz.gasResistance.idx;
    idx["temperature"] = configuration.domoticz.temperature.idx;
    idx["humidity"] = configuration.domoticz.humidity.idx;
    idx["pressure"] = configuration.domoticz.pressure.idx;
    idx["relativePressure"] = configuration.domoticz.relativePressure.idx;
    idx["dewPoint"] = configuration.domoticz.dewPoint.idx;
    idx["heatIndex"] = configuration.domoticz.heatIndex.idx;
    idx["iaq"] = configuration.domoticz.iaq.idx;
    idx["staticIaq"] = configuration.domoticz.staticIaq.idx;
    idx["co2Equivalent"] = configuration.domoticz.co2Equivalent.idx;
    idx["breathVocEquivalent"] = configuration.domoticz.breathVocEquivalent.idx;

    temperature["unit"] = configuration.temperature.unit;
    temperature["correction"] = configuration.temperature.correction;
    humidity["correction"] = configuration.humidity.correction;
    pressure["unit"] = configuration.pressure.unit;
    pressure["correction"] = configuration.pressure.correction;
    pressure["seaLevelPressure"] = configuration.seaLevelPressure;
    pressure["altitude"] = configuration.altitude;

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    Serial << endl
           << "success" << endl
           << "JSON Buffer size: " << jsonBuffer.size() << endl
           << (AFE_CONFIG_FILE_BUFFER_BMEX80 < jsonBuffer.size() + 10
                   ? "WARNING: Buffor size might be to small"
                   : "Buffor size: OK");
#endif
  }
#ifdef DEBUG
  else
  {
    Serial << endl
           << "failed to open file for writing";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif
}
void AFEDataAccess::createBMEX80SensorConfigurationFile()
{
#ifdef DEBUG
  Serial << endl
         << "Creating file: cfg-BMEX80-XX.json";
#endif
  BMEX80 configuration;
  configuration.type = AFE_BMX_UNKNOWN_SENSOR;
  configuration.interval = AFE_CONFIG_HARDWARE_BMEX80_DEFAULT_INTERVAL;
  configuration.i2cAddress = 0;
  configuration.resolution = BMP085_ULTRAHIGHRES;
  configuration.temperature.unit = AFE_TEMPERATURE_UNIT_CELSIUS;
  configuration.temperature.correction = 0;
  configuration.humidity.correction = 0;
  configuration.pressure.unit = AFE_PRESSURE_UNIT_HPA;
  configuration.pressure.correction = 0;
  configuration.altitude = 0;
  configuration.seaLevelPressure = AFE_CONFIG_DEFAULT_SEA_LEVEL_PRESSURE;

  configuration.domoticz.temperatureHumidityPressure.idx = 0;
  configuration.domoticz.temperatureHumidity.idx = 0;
  configuration.domoticz.gasResistance.idx = 0;
  configuration.domoticz.temperature.idx = 0;
  configuration.domoticz.humidity.idx = 0;
  configuration.domoticz.pressure.idx = 0;
  configuration.domoticz.relativePressure.idx = 0;
  configuration.domoticz.dewPoint.idx = 0;
  configuration.domoticz.heatIndex.idx = 0;
  configuration.domoticz.iaq.idx = 0;
  configuration.domoticz.staticIaq.idx = 0;
  configuration.domoticz.co2Equivalent.idx = 0;
  configuration.domoticz.breathVocEquivalent.idx = 0;

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_BMEX80; i++)
  {
    sprintf(configuration.mqtt.topic, "BMEX80/%d", i + 1);
    sprintf(configuration.name, "BMEX80-%d", i + 1);
    saveConfiguration(i, configuration);
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
#define AFE_CONFIG_FILE_BUFFER_BH1750 230
BH1750 AFEDataAccess::getBH1750SensorConfiguration(uint8_t id)
{
  BH1750 configuration;
  char fileName[18];
  sprintf(fileName, "cfg-bh1750-%d.json", id);

#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Reading File -------------------";
  Serial << endl
         << "Opening file: " << fileName << " : ";
#endif

  File configFile = SPIFFS.open(fileName, "r");
  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Reading JSON : ";
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_BH1750> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());

    if (root.success())
    {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      sprintf(configuration.name, root["name"]);
      configuration.mode = root["mode"];
      configuration.interval = root["interval"];
      configuration.i2cAddress = root["i2cAddress"];
      configuration.domoticz.idx = root["idx"];
      sprintf(configuration.mqtt.topic, root["mqttTopic"]);

#ifdef DEBUG
      Serial << endl
             << "success" << endl
             << "JSON Buffer size: " << jsonBuffer.size() << endl
             << (AFE_CONFIG_FILE_BUFFER_BH1750 < jsonBuffer.size() + 10
                     ? "WARNING: Buffor size might be to small"
                     : "Buffor size: OK");
#endif
    }

#ifdef DEBUG
    else
    {
      Serial << "failure";
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else
  {
    Serial << "failure";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif

  return configuration;
}
void AFEDataAccess::saveConfiguration(uint8_t id, BH1750 configuration)
{
  char fileName[18];
  sprintf(fileName, "cfg-bh1750-%d.json", id);

#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Writing File -------------------";
  Serial << endl
         << "Opening file: " << fileName << " : ";
#endif

  File configFile = SPIFFS.open(fileName, "w");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Writing JSON : ";
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_BH1750> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();

    root["name"] = configuration.name;
    root["mode"] = configuration.mode;
    root["interval"] = configuration.interval;
    root["i2cAddress"] = configuration.i2cAddress;
    root["mqttTopic"] = configuration.mqtt.topic;
    root["idx"] = configuration.domoticz.idx;

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    Serial << endl
           << "success" << endl
           << "JSON Buffer size: " << jsonBuffer.size() << endl
           << (AFE_CONFIG_FILE_BUFFER_BH1750 < jsonBuffer.size() + 10
                   ? "WARNING: Buffor size might be to small"
                   : "Buffor size: OK");
#endif
  }
#ifdef DEBUG
  else
  {
    Serial << endl
           << "failed to open file for writing";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif
}

void AFEDataAccess::createBH1750SensorConfigurationFile()
{
#ifdef DEBUG
  Serial << endl
         << "Creating file: cfg-bh1750-XX.json";
#endif
  BH1750 configuration;
  configuration.interval = AFE_CONFIG_HARDWARE_BH1750_DEFAULT_INTERVAL;
  configuration.i2cAddress = 0;
  configuration.domoticz.idx = 0;
  configuration.mode = AFE_CONFIG_HARDWARE_BH1750_DEFAULT_MODE;
  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_BH1750; i++)
  {
    sprintf(configuration.mqtt.topic, "BH1750/%d", i + 1);
    sprintf(configuration.name, "BH1750-%d", i + 1);
    saveConfiguration(i, configuration);
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
#define AFE_CONFIG_FILE_BUFFER_AS3935 200
AS3935 AFEDataAccess::getAS3935SensorConfiguration()
{
  AS3935 configuration;
#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Reading File -------------------";
  Serial << endl
         << "Opening file: cfg-as3935.json : ";
#endif

  File configFile = SPIFFS.open("cfg-as3935.json", "r");
  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Reading JSON : ";
#endif

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_AS3935> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());

    if (root.success())
    {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      configuration.i2cAddress = root["i2cAddress"];
      configuration.irqGPIO = root["irqGPIO"];
      configuration.setNoiseFloorAutomatically = root["setNoiseFloorAutomatically"];
      configuration.noiseFloor = root["noiseFloor"];
      configuration.indoor = root["indoor"];
      configuration.unit = root["unit"];
      configuration.domoticz.idx = root["idx"];
      sprintf(configuration.mqtt.topic, root["mqttTopic"]);

#ifdef DEBUG
      Serial << endl
             << "success" << endl
             << "JSON Buffer size: " << jsonBuffer.size() << endl
             << (AFE_CONFIG_FILE_BUFFER_AS3935 < jsonBuffer.size() + 10
                     ? "WARNING: Buffor size might be to small"
                     : "Buffor size: OK");
#endif
    }

#ifdef DEBUG
    else
    {
      Serial << "failure";
    }
#endif

    configFile.close();
  }

#ifdef DEBUG
  else
  {
    Serial << "failure";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif

  return configuration;
}

void AFEDataAccess::saveConfiguration(AS3935 configuration)
{
#ifdef DEBUG
  Serial << endl
         << endl
         << "----------------- Writing File -------------------";
  Serial << endl
         << "Opening file: cfg-as3935.json : ";
#endif

  File configFile = SPIFFS.open("cfg-as3935.json", "w");

  if (configFile)
  {
#ifdef DEBUG
    Serial << "success" << endl
           << "Writing JSON : ";
#endif

    StaticJsonBuffer<AFE_CONFIG_FILE_BUFFER_AS3935> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();

    root["i2cAddress"] = configuration.i2cAddress;
    root["irqGPIO"] = configuration.irqGPIO;
    root["setNoiseFloorAutomatically"] = configuration.setNoiseFloorAutomatically;
    root["noiseFloor"] = configuration.noiseFloor;
    root["indoor"] = configuration.indoor;
    root["unit"] = configuration.unit;
    root["mqttTopic"] = configuration.mqtt.topic;
    root["idx"] = configuration.domoticz.idx;

    root.printTo(configFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    configFile.close();

#ifdef DEBUG
    Serial << endl
           << "success" << endl
           << "JSON Buffer size: " << jsonBuffer.size() << endl
           << (AFE_CONFIG_FILE_BUFFER_AS3935 < jsonBuffer.size() + 10
                   ? "WARNING: Buffor size might be to small"
                   : "Buffor size: OK");
#endif
  }
#ifdef DEBUG
  else
  {
    Serial << endl
           << "failed to open file for writing";
  }
  Serial << endl
         << "--------------------------------------------------";
#endif
}
void AFEDataAccess::createAS3935SensorConfigurationFile()
{
#ifdef DEBUG
  Serial << endl
         << "Creating file: cfg-as3935.json";
#endif
  AS3935 configuration;
  configuration.irqGPIO = 0;
  configuration.setNoiseFloorAutomatically = true;
  configuration.noiseFloor = 0;
  configuration.indoor = true;
  configuration.domoticz.idx = 0;
  configuration.unit = AFE_DISTANCE_KM;
  sprintf(configuration.mqtt.topic, "AS3935");
  saveConfiguration(configuration);
}
#endif

IPAddress AFEDataAccess::IPfromString(const char *address)
{
  IPAddress ip;
  if (!ip.fromString(address))
  {
#ifdef DEBUG
    Serial << endl
           << "ERROR: converting from IP String (" << address
           << ") to IP address";
#endif
  };
  return ip;
}
