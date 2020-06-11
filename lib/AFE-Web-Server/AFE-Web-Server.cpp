/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Web-Server.h"

AFEWebServer::AFEWebServer() {}

void AFEWebServer::begin(AFEDataAccess *_Data, AFEDevice *_Device,
                         AFEFirmwarePro *_FirmwarePro) {
  server.begin();
  Site.begin(_Data, _Device, _FirmwarePro);
  Data = _Data;
  Device = _Device;
  FirmwarePro = _FirmwarePro;
}

#ifdef AFE_CONFIG_HARDWARE_LED
void AFEWebServer::initSystemLED(AFELED *_SystemLED) { SystemLED = _SystemLED; }
#endif

String AFEWebServer::generateSite(AFE_SITE_PARAMETERS *siteConfig,
                                  String &page) {

  if (siteConfig->twoColumns) {
    Site.generateTwoColumnsLayout(page, siteConfig->rebootTime);
  } else {
    Site.generateOneColumnLayout(page, siteConfig->rebootTime);
  }

  if (siteConfig->form) {
    page.concat("<form  method=\"post\" action=\"/?c=");
    page.concat(AFE_SERVER_CMD_SAVE);
    page.concat("&o=");
    page.concat(siteConfig->ID);
    if (siteConfig->deviceID >= 0) {
      page.concat("&i=");
      page.concat(siteConfig->deviceID);
    }
    page.concat("\">");
  }

  switch (siteConfig->ID) {
  case AFE_CONFIG_SITE_INDEX:
    Site.addIndexSection(page, siteConfig->deviceID == -1 ? true : false);
    break;
  case AFE_CONFIG_SITE_FIRST_TIME:
    Site.addNetworkConfiguration(page);
    break;
  case AFE_CONFIG_SITE_FIRST_TIME_CONNECTING:
    Site.addConnectingSite(page);
    break;
  case AFE_CONFIG_SITE_DEVICE:
    Site.addDeviceConfiguration(page);
    break;
  case AFE_CONFIG_SITE_NETWORK:
    Site.addNetworkConfiguration(page);
    break;
  case AFE_CONFIG_SITE_MQTT:
    Site.addMQTTBrokerConfiguration(page);
    break;
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  case AFE_CONFIG_SITE_DOMOTICZ:
    Site.addDomoticzServerConfiguration(page);
    break;
#endif
  case AFE_CONFIG_SITE_PASSWORD:
    Site.addPasswordConfigurationSite(page);
    break;
  case AFE_CONFIG_SITE_PRO_VERSION:
    Site.addProVersionSite(page);
    break;
  case AFE_CONFIG_SITE_EXIT:
    Site.addExitSection(page, siteConfig->rebootMode);
    break;
  case AFE_CONFIG_SITE_RESET:
    Site.addResetSection(page);
    break;
  case AFE_CONFIG_SITE_POST_RESET:
    Site.addPostResetSection(page);
    break;
  case AFE_CONFIG_SITE_UPGRADE:
    Site.addUpgradeSection(page);
    break;
  case AFE_CONFIG_SITE_POST_UPGRADE:
    Site.addPostUpgradeSection(page, upgradeFailed);
    break;
#ifdef AFE_CONFIG_HARDWARE_RELAY
  case AFE_CONFIG_SITE_RELAY:
    Site.addRelayConfiguration(page, siteConfig->deviceID);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_SWITCH
  case AFE_CONFIG_SITE_SWITCH:
    Site.addSwitchConfiguration(page, siteConfig->deviceID);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  case AFE_CONFIG_SITE_ANALOG_INPUT:
    Site.addAnalogInputConfiguration(page);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  case AFE_CONFIG_SITE_CONTACTRON:
    Site.addContactronConfiguration(page, siteConfig->deviceID);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_GATE
  case AFE_CONFIG_SITE_GATE:
    Site.addGateConfiguration(page, siteConfig->deviceID);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_UART
  case AFE_CONFIG_SITE_UART:
    Site.addSerialPortConfiguration(page);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_I2C
  case AFE_CONFIG_SITE_I2C:
    Site.addI2CPortConfiguration(page);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_BMEX80
  case AFE_CONFIG_SITE_BMEX80:
    Site.addBMEX80Configuration(page, siteConfig->deviceID);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  case AFE_CONFIG_SITE_HPMA115S0:
    Site.addHPMA115S0Configuration(page, siteConfig->deviceID);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_BH1750
  case AFE_CONFIG_SITE_BH1750:
    Site.addBH1750Configuration(page, siteConfig->deviceID);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_AS3935
  case AFE_CONFIG_SITE_AS3935:
    Site.addAS3935Configuration(page, siteConfig->deviceID);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_DS18B20
  case AFE_CONFIG_SITE_DS18B20:
    Site.addDS18B20Configuration(page, siteConfig->deviceID);
#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER_SENSOR
  case AFE_CONFIG_SITE_ANEMOMETER_SENSOR:
    Site.addAnemometerSensorConfiguration(page);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_RAINMETER_SENSOR
  case AFE_CONFIG_SITE_RAINMETER_SENSOR:
    Site.addRainmeterSensorConfiguration(page);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_LED
  case AFE_CONFIG_SITE_LED:
    for (uint8_t i = 0; i < Device->configuration.noOfLEDs; i++) {
      Site.addLEDConfiguration(page, i);
    }
    Site.addSystemLEDConfiguration(page);
    break;
#endif
  }

  if (siteConfig->form) {
    if (siteConfig->formButton) {
      page.concat("<input type=\"submit\" class=\"b bs\" value=\"");
      page.concat(L_SAVE);
      page.concat("\">");
    }
    page.concat("</form>");
  }

  Site.generateFooter(page, (Device->getMode() == AFE_MODE_NORMAL ||
                             Device->getMode() == AFE_MODE_CONFIGURATION)
                                ? true
                                : false);

  page.replace("{{s.lang}}", L_LANGUAGE_SHORT);
  page.replace("{{L_DONATE}}", L_DONATE);

  return page;
}

void AFEWebServer::generate(boolean upload) {

  if (getOptionName()) {
    return;
  }

  String page;

  page.reserve(AFE_MAX_PAGE_SIZE);

  if (_refreshConfiguration) {
    _refreshConfiguration = false;
    Device->begin();
  }

  AFE_SITE_PARAMETERS siteConfig;

  siteConfig.ID = getSiteID();
  uint8_t command = getCommand();
  siteConfig.deviceID = getID();

  if (!upload) {

    /* Setting page refresh time if automatic logout is set */
    if ((Device->getMode() == AFE_MODE_CONFIGURATION ||
         Device->getMode() == AFE_MODE_ACCESS_POINT) &&
        Device->configuration.timeToAutoLogOff > 0) {

      siteConfig.rebootTime =
          Device->configuration.timeToAutoLogOff * 60 +
          10; // adds additional 10sec for a reboot to be finished
#ifdef DEBUG
      Serial << endl
             << F("INFO: Setting auto-logout to ") << siteConfig.rebootTime
             << F("seconds");
#endif
    }

    if (command == AFE_SERVER_CMD_SAVE) {
      if (siteConfig.ID == AFE_CONFIG_SITE_FIRST_TIME) {
        NETWORK configuration;
        getNetworkData(&configuration);
        siteConfig.twoColumns = false;
        siteConfig.reboot = true;
        siteConfig.rebootMode = AFE_MODE_CONFIGURATION;
        siteConfig.form = false;
        siteConfig.ID = AFE_CONFIG_SITE_FIRST_TIME_CONNECTING;
        Data->saveConfiguration(&configuration);
        configuration = {0};
      } else if (siteConfig.ID == AFE_CONFIG_SITE_DEVICE) {
        DEVICE configuration;
        getDeviceData(&configuration);
        Data->saveConfiguration(&configuration);
        configuration = {0};
      } else if (siteConfig.ID == AFE_CONFIG_SITE_NETWORK) {
        NETWORK configuration;
        getNetworkData(&configuration);
        Data->saveConfiguration(&configuration);
        configuration = {0};
      } else if (siteConfig.ID == AFE_CONFIG_SITE_PASSWORD) {
        PASSWORD configuration;
        getPasswordData(&configuration);
        Data->saveConfiguration(&configuration);
        configuration = {0};
      } else if (siteConfig.ID == AFE_CONFIG_SITE_RESET) {
        siteConfig.ID = AFE_CONFIG_SITE_POST_RESET;
        siteConfig.reboot = true;
        siteConfig.rebootMode = AFE_MODE_FIRST_TIME_LAUNCH;
        siteConfig.rebootTime = 15;
        siteConfig.form = false;
        siteConfig.twoColumns = false;
      } else if (siteConfig.ID == AFE_CONFIG_SITE_PRO_VERSION) {
        PRO_VERSION configuration;
        getSerialNumberData(&configuration);
        Data->saveConfiguration(&configuration);
        configuration = {0};
        FirmwarePro->callService(AFE_WEBSERVICE_ADD_KEY);
      } else if (siteConfig.ID == AFE_CONFIG_SITE_MQTT) {
        MQTT configuration;
        getMQTTData(&configuration);
        Data->saveConfiguration(&configuration);
        configuration = {0};
      }
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
      else if (siteConfig.ID == AFE_CONFIG_SITE_DOMOTICZ) {
        DOMOTICZ configuration;
        getDomoticzServerData(&configuration);
        Data->saveConfiguration(&configuration);
        configuration = {0};
      }
#endif
#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
      else if (siteConfig.ID == AFE_CONFIG_SITE_ANALOG_INPUT) {
        ADCINPUT configuration;
        getAnalogInputData(&configuration);
        Data->saveConfiguration(&configuration);
        configuration = {0};
      }
#endif
#ifdef AFE_CONFIG_HARDWARE_LED
      else if (siteConfig.ID == AFE_CONFIG_SITE_LED) {
        LED configuration;
        for (uint8_t i = 0; i < Device->configuration.noOfLEDs; i++) {
          getLEDData(i, &configuration);
          Data->saveConfiguration(i, &configuration);
        }
        configuration = {0};
        Data->saveSystemLedID(getSystemLEDData());
      }
#endif
#ifdef AFE_CONFIG_HARDWARE_RELAY
      else if (siteConfig.ID == AFE_CONFIG_SITE_RELAY) {
        RELAY configuration;
        getRelayData(siteConfig.deviceID, &configuration);
        Data->saveConfiguration(siteConfig.deviceID, &configuration);
        configuration = {0};
      }
#endif
#ifdef AFE_CONFIG_HARDWARE_SWITCH
      else if (siteConfig.ID == AFE_CONFIG_SITE_SWITCH) {
        SWITCH configuration;
        getSwitchData(siteConfig.deviceID, &configuration);
        Data->saveConfiguration(siteConfig.deviceID, &configuration);
        configuration = {0};
      }
#endif
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
      else if (siteConfig.ID == AFE_CONFIG_SITE_CONTACTRON) {
        CONTACTRON configuration;
        getContactronData(siteConfig.deviceID, &configuration);
        Data->saveConfiguration(siteConfig.deviceID, &configuration);
        configuration = {0};
      }
#endif
#ifdef AFE_CONFIG_HARDWARE_GATE
      else if (configuration.ID == AFE_CONFIG_SITE_GATE) {
        GATE configuration;
        getGateData(&configuration);
        Data->saveConfiguration(siteConfig.deviceID, &configuration);
        configuration = {0};
      }
#endif
#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
      else if (siteConfig.ID == AFE_CONFIG_SITE_HPMA115S0) {
        HPMA115S0 configuration;
        getHPMA115S0SensorData(&configuration);
        Data->saveConfiguration(siteConfig.deviceID, &configuration);
        configuration = {0};
      }
#endif
#ifdef AFE_CONFIG_HARDWARE_BMEX80
      else if (siteConfig.ID == AFE_CONFIG_SITE_BMEX80) {
        BMEX80 configuration;
        getBMEX80SensorData(&configuration);
        Data->saveConfiguration(siteConfig.deviceID, &configuration);
        configuration = {0};
      }
#endif
#ifdef AFE_CONFIG_HARDWARE_BH1750
      else if (siteConfig.ID == AFE_CONFIG_SITE_BH1750) {
        BH1750 configuration;
        getBH1750SensorData(&configuration);
        Data->saveConfiguration(siteConfig.deviceID, &configuration);
        configuration = {0};
      }
#endif
#ifdef AFE_CONFIG_HARDWARE_AS3935
      else if (siteConfig.ID == AFE_CONFIG_SITE_AS3935) {
        AS3935 configuration;
        getAS3935SensorData(&configuration);
        Data->saveConfiguration(siteConfig.deviceID, &configuration);
        configuration = {0};
      }
#endif
#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER_SENSOR
      else if (siteConfig.ID == AFE_CONFIG_SITE_ANEMOMETER_SENSOR) {
        ANEMOMETER configuration;
        getAnemometerSensorData(&configuration);
        Data->saveConfiguration(&configuration);
        configuration = {0};
      }
#endif
#ifdef AFE_CONFIG_HARDWARE_RAINMETER_SENSOR
      else if (siteConfig.ID == AFE_CONFIG_SITE_RAINMETER_SENSOR) {
        RAINMETER configuration;
        getRainmeterSensorData(&configuration);
        Data->saveConfiguration(&configuration);
        configuration = {0};
      }
#endif
#ifdef AFE_CONFIG_HARDWARE_DS18B20
      case AFE_CONFIG_SITE_DS18B20:
        DS18B20 ds18B20Configuration;
        get(ds18B20Configuration);
        Data.saveConfiguration(siteConfig.deviceID, &ds18B20Configuration);
        break;
#endif
#ifdef AFE_CONFIG_HARDWARE_UART
      else if (siteConfig.ID == AFE_CONFIG_SITE_UART) {
        SERIALPORT configuration;
        getSerialPortData(&configuration);
        Data->saveConfiguration(&configuration);
        configuration = {0};
      }
#endif
#ifdef AFE_CONFIG_HARDWARE_I2C
      else if (siteConfig.ID == AFE_CONFIG_SITE_I2C) {
        I2CPORT configuration;
        getI2CPortData(&configuration);
        Data->saveConfiguration(&configuration);
        configuration = {0};
      }
#endif

    } else if (command == AFE_SERVER_CMD_NONE) {
      if (siteConfig.ID == AFE_CONFIG_SITE_INDEX) {
        siteConfig.form = false;
        siteConfig.twoColumns = false;
        if (siteConfig.deviceID > AFE_MODE_NORMAL) {
          boolean authorize = true;
          PASSWORD accessControl;
          Data->getConfiguration(&accessControl);
          if (accessControl.protect) {
            PASSWORD data;
            getPasswordData(&data);
            if (strcmp(accessControl.password, data.password) != 0) {
              authorize = false;
            }
          }
          if (authorize) {
            siteConfig.rebootMode = siteConfig.deviceID;
            siteConfig.ID = AFE_CONFIG_SITE_EXIT;
            siteConfig.reboot = true;
            siteConfig.rebootTime = 10;
          }
        }
      } else if (siteConfig.ID == AFE_CONFIG_SITE_EXIT) {
        siteConfig.reboot = true;
        siteConfig.rebootMode = AFE_MODE_NORMAL;
        siteConfig.rebootTime = 10;
        siteConfig.form = false;
        siteConfig.twoColumns = false;
      } else if (siteConfig.ID == AFE_CONFIG_SITE_FIRST_TIME) {
        siteConfig.twoColumns = false;
      } else if (siteConfig.ID == AFE_CONFIG_SITE_RESET) {
        siteConfig.formButton = false;
      } else if (siteConfig.ID == AFE_CONFIG_SITE_UPGRADE) {
        siteConfig.form = false;
      } else if (siteConfig.ID == AFE_CONFIG_SITE_POST_UPGRADE) {
        if (!upload) {
          siteConfig.form = false;
          siteConfig.twoColumns = false;
          siteConfig.rebootTime = 15;
          siteConfig.reboot = true;
          siteConfig.rebootMode = Device->getMode();
        }
      }
    }
  }

  if (upload) {
    HTTPUpload &upload = server.upload();
    String _updaterError;
    if (upload.status == UPLOAD_FILE_START) {
      WiFiUDP::stopAll();

#ifdef DEBUG
      Serial << endl
             << F("INFO: Firmware upgrade. Update: ")
             << upload.filename.c_str();
#endif

      uint32_t maxSketchSpace =
          (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;

#ifdef DEBUG
      Serial << endl << F("INFO: Sketch size: ") << ESP.getSketchSize();
      Serial << endl
             << F("INFO: Free sketch size: ") << ESP.getFreeSketchSpace();
      Serial << endl << F("INFO: Max sketch space: ") << maxSketchSpace;
#endif

      if (!Update.begin(maxSketchSpace)) { // start with max available size
#ifdef DEBUG
        Update.printError(Serial);
#endif
        upgradeFailed = true;
      }
    } else if (upload.status == UPLOAD_FILE_WRITE && !_updaterError.length()) {
#ifdef AFE_CONFIG_HARDWARE_LED
      SystemLED->toggle();
#endif
#ifdef DEBUG
      Serial << F(".");
#endif

      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
#ifdef DEBUG
        Update.printError(Serial);
#endif
        upgradeFailed = true;
      }
    } else if (upload.status == UPLOAD_FILE_END && !_updaterError.length()) {
      if (Update.end(true)) { // true to set the size to the current
                              // progress
#ifdef DEBUG
        Serial << endl
               << F("INFO: Update Success. Firmware size: ") << upload.totalSize
               << endl
               << F("INFO: Rebooting...");
#endif
      } else {
#ifdef DEBUG
        Update.printError(Serial);
#endif
        upgradeFailed = true;
      }
    } else if (upload.status == UPLOAD_FILE_ABORTED) {
      Update.end();
#ifdef DEBUG
      Serial << endl << F("ERROR: Update was aborted");
#endif
    }
    delay(0);
  } else {

#ifdef DEBUG
    Serial << endl
           << F("INFO: Generating ")
           << (siteConfig.twoColumns ? F("Two Columns") : F("One Column"))
           << F(" site: ") << siteConfig.ID;
    Serial << F(", device ID: ") << siteConfig.deviceID;
    Serial << F(", Command: ") << command;
    Serial << F(", Reboot: ") << (siteConfig.reboot ? F("Yes") : F("No"));
    Serial << F(", Mode: ") << siteConfig.rebootMode;
    Serial << F(", Time: ") << siteConfig.rebootTime;
#endif

    generateSite(&siteConfig, page);

    publishHTML(page);
  }

  /* Rebooting device */
  if (siteConfig.reboot) {
#ifdef AFE_CONFIG_HARDWARE_LED
    SystemLED->on();
#endif
    Device->reboot(siteConfig.rebootMode);
  }

  /*

  #ifdef AFE_CONFIG_HARDWARE_DS18B20
  }
  else if (optionName == "ds18b20") {
    DS18B20 data = {};
    if (command == AFE_SERVER_CMD_SAVE) {
      data = getDS18B20Data();
    }
    publishHTML(ConfigurationPanel.getDS18B20ConfigurationSite(command,
  data)); #endif #ifdef AFE_CONFIG_HARDWARE_DHXX
  }
  else if (optionName == "DHT") {
    DH data = {};
    if (command == AFE_SERVER_CMD_SAVE) {
      data = getDHTData();
    }
    publishHTML(ConfigurationPanel.getDHTConfigurationSite(command, data));
  #endif

  #ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  }
  else if (optionName == "thermostat" || optionName == "humidistat") {
    if (command == AFE_SERVER_CMD_SAVE) {
        Data->saveConfiguration(getRegulatorData(optionName == "thermostat" ?
  THERMOSTAT_REGULATOR : HUMIDISTAT_REGULATOR);
    }
    publishHTML(ConfigurationPanel.getRelayStatConfigurationSite());
  #endif
  }


  #if defined(T3_CONFIG)
    for (uint8_t i = 0; i < 4; i++) {
      if (Device->configuration.isPIR[i]) {
        if (getOptionName() == "pir" + String(i)) {
          PIR data = {};
          if (getCommand() == AFE_SERVER_CMD_SAVE) {
            data = getPIRData(i);
          }
          publishHTML(ConfigurationPanel.getPIRConfigurationSite(
              getOptionName(), getCommand(), data, i));
        }
      } else {
        break;
      }
    }
  #endif

  #if defined(T5_CONFIG)
    for (uint8_t i = 0; i < sizeof(Device->configuration.isContactron); i++) {
      if (Device->configuration.isContactron[i]) {
        if (optionName == "contactron" + String(i)) {
          CONTACTRON data = {};
          if (command == AFE_SERVER_CMD_SAVE) {
            data = getContactronData(i);
          }
          publishHTML(ConfigurationPanel.getContactronConfigurationSite(
              optionName, command, data, i));
        }
      } else {
        break;
      }
    }


  */
}

/* Methods related to the url request */

boolean AFEWebServer::getOptionName() {
  /* Recived HTTP API Command */
  if (server.hasArg("command")) {
    /* Constructing command */
    server.arg("command").toCharArray(httpCommand.command,
                                      sizeof(httpCommand.command));
    if (server.arg("device")) {
      server.arg("device").toCharArray(httpCommand.device,
                                       sizeof(httpCommand.device));
    } else {
      memset(httpCommand.device, 0, sizeof httpCommand.device);
    }
    if (server.arg("name")) {
      server.arg("name").toCharArray(httpCommand.name,
                                     sizeof(httpCommand.name));
    } else {
      memset(httpCommand.name, 0, sizeof httpCommand.name);
    }

    if (server.arg("source")) {
      server.arg("source").toCharArray(httpCommand.source,
                                       sizeof(httpCommand.source));
    } else {
      memset(httpCommand.source, 0, sizeof httpCommand.source);
    }
    receivedHTTPCommand = true;
  }
  return receivedHTTPCommand;
}

uint8_t AFEWebServer::getSiteID() {

  if (Device->getMode() == AFE_MODE_NETWORK_NOT_SET) {
    return AFE_CONFIG_SITE_FIRST_TIME;
  } else if (Device->getMode() == AFE_MODE_NORMAL) {
    return AFE_CONFIG_SITE_INDEX;
  } else {
    if (server.hasArg("o")) {
      return server.arg("o").toInt();
    } else {
      return AFE_CONFIG_SITE_DEVICE;
    }
  }
}

uint8_t AFEWebServer::getCommand() {
  if (server.hasArg("c")) {
    return server.arg("c").toInt();
  } else {
    return AFE_SERVER_CMD_NONE;
  }
}

uint8_t AFEWebServer::getID() {
  if (server.hasArg("i")) {
    return server.arg("i").toInt();
  } else {
    return -1;
  }
}

/* Server methods */

HTTPCOMMAND AFEWebServer::getHTTPCommand() {
  receivedHTTPCommand = false;
  return httpCommand;
}

void AFEWebServer::listener() {
  server.handleClient();
  /* Code for automatic logoff from the config panel */
  if ((Device->getMode() == AFE_MODE_CONFIGURATION ||
       Device->getMode() == AFE_MODE_ACCESS_POINT) &&
      Device->configuration.timeToAutoLogOff > 0) {
    if (Device->configuration.timeToAutoLogOff * 60000 + howLongInConfigMode <
        millis()) {
#ifdef DEBUG
      Serial << endl
             << endl
             << F("INFO: Automatic logout from the config panel after : ")
             << Device->configuration.timeToAutoLogOff
             << F("min. of idle time");
#endif
      Device->reboot(AFE_MODE_NORMAL);
    }
  }
}

boolean AFEWebServer::httpAPIlistener() { return receivedHTTPCommand; }

void AFEWebServer::publishHTML(String &page) {
  uint16_t pageSize = page.length();
  uint16_t size = 64;

#ifdef DEBUG
  Serial << endl
         << endl
         << F("INFO: Site streaming started. Size : ") << pageSize
         << F(" ... ");

  if (pageSize + 100 > AFE_MAX_PAGE_SIZE) {
    Serial << endl
           << endl
           << F("ERROR: Page size buffor ") << AFE_MAX_PAGE_SIZE
           << F("B too small : ") << pageSize << F(" ... ");
  }
#endif
  server.sendHeader("Content-Length", String(page.length()));
  server.setContentLength(pageSize);
  if (pageSize > size) {
#ifdef DEBUG
    Serial << endl
           << F("INFO: Heap size after sending Header: ")
           << system_get_free_heap_size() / 1024 << F("kB") << endl
           << F("INFO: Transfering site over TCP: ");
#endif
    server.send(200, "text/html", page.substring(0, size));
    uint16_t transfered = size;
    uint16_t nextChunk;
    while (transfered < pageSize) {
      nextChunk = transfered + size < pageSize ? transfered + size : pageSize;
#ifdef DEBUG
      Serial << F(".");
#endif

      server.sendContent(page.substring(transfered, nextChunk));
      transfered = nextChunk;
    }
  } else {
    server.send(200, "text/html", page);
  }

#ifdef DEBUG
  Serial << endl << F("INFO: Site published");
#endif

  if ((Device->getMode() == AFE_MODE_CONFIGURATION ||
       Device->getMode() == AFE_MODE_ACCESS_POINT) &&
      Device->configuration.timeToAutoLogOff > 0) {
    howLongInConfigMode = millis();
  }
}

void AFEWebServer::sendJSON(String json) {
  server.send(200, "application/json", json);
}

void AFEWebServer::handle(const char *uri,
                          ESP8266WebServer::THandlerFunction handler) {
  server.on(uri, handler);
}

void AFEWebServer::handleFirmwareUpgrade(
    const char *uri, ESP8266WebServer::THandlerFunction handlerUpgrade,
    ESP8266WebServer::THandlerFunction handlerUpload) {
  server.on(uri, HTTP_POST, handlerUpgrade, handlerUpload);
}

void AFEWebServer::onNotFound(ESP8266WebServer::THandlerFunction fn) {
  server.onNotFound(fn);
}

/* Reading Server data */

void AFEWebServer::getDeviceData(DEVICE *data) {

  _refreshConfiguration =
      true; // it will cause that device configuration will be refeshed

  if (server.arg("n").length() > 0) {
    server.arg("n").toCharArray(data->name, sizeof(data->name));
  } else {
    data->name[0] = '\0';
  }

  data->api.http = server.arg("h").length() > 0 ? true : false;

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  data->api.domoticz = server.arg("m").length() > 0
                           ? (server.arg("m").toInt() == 1 ? true : false)
                           : false;
  data->api.mqtt = server.arg("m").length() > 0
                       ? (server.arg("m").toInt() == 2 ? true : false)
                       : false;
  data->api.domoticzVersion = server.arg("v").length() > 0
                                  ? server.arg("v").toInt()
                                  : AFE_DOMOTICZ_VERSION_DEFAULT;
#else
  data->api.mqtt = server.arg("m").length() > 0 ? true : false;
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
  data.noOfLEDs = server.arg("l").length() > 0
                      ? server.arg("l").toInt()
                      : AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_LEDS;
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  data->noOfContactrons =
      server.arg("co").length() > 0 ? server.arg("co").toInt() : 0;
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
  data->noOfGates = server.arg("g").length() > 0 ? server.arg("g").toInt() : 0;
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
  data.noOfRelays = server.arg("r").length() > 0
                        ? server.arg("r").toInt()
                        : AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_RELAYS;
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  data.noOfSwitches = server.arg("s").length() > 0
                          ? server.arg("s").toInt()
                          : AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_SWITCHES;
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
  data.noOfDS18B20s = server.arg("ds").length() > 0
                          ? server.arg("ds").toInt()
                          : AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_DS18B20;
#endif

#ifdef AFE_CONFIG_HARDWARE_DHXX
  data->isDHT = server.arg("dh").length() > 0 ? true : false;
#endif

#if defined(T3_CONFIG)
  for (uint8_t i = 0; i < sizeof(Device->configuration.isPIR); i++) {
    data->isPIR[i] = server.arg("p").toInt() > i ? true : false;
  }
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  data->noOfHPMA115S0s =
      server.arg("hp").length() > 0 ? server.arg("hp").toInt() : 0;
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  data->noOfBMEX80s =
      server.arg("b6").length() > 0 ? server.arg("b6").toInt() : 0;
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
  data->noOfBH1750s =
      server.arg("bh").length() > 0 ? server.arg("bh").toInt() : 0;
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
  data->noOfAS3935s =
      server.arg("a3").length() > 0 ? server.arg("a3").toInt() : 0;
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER_SENSOR
  data->noOfAnemometerSensors =
      server.arg("w").length() > 0 ? server.arg("w").toInt() : 0;
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER_SENSOR
  data->noOfRainmeterSensors =
      server.arg("d").length() > 0 ? server.arg("d").toInt() : 0;
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  data->isAnalogInput = server.arg("ad").length() > 0 ? true : false;
#endif

  data->timeToAutoLogOff =
      server.arg("al").length() > 0 ? AFE_AUTOLOGOFF_DEFAULT_TIME : 0;
}

void AFEWebServer::getNetworkData(NETWORK *data) {

  if (server.arg("s").length() > 0) {
    server.arg("s").toCharArray(data->ssid, sizeof(data->ssid));
  } else {
    data->ssid[0] = '\0';
  }

  if (server.arg("p").length() > 0) {
    server.arg("p").toCharArray(data->password, sizeof(data->password));
  } else {
    data->password[0] = '\0';
  }

  if (server.arg("i1").length() > 0) {
    server.arg("i1").toCharArray(data->ip, sizeof(data->ip));
  } else {
    data->ip[0] = '\0';
  }

  if (server.arg("i2").length() > 0) {
    server.arg("i2").toCharArray(data->gateway, sizeof(data->gateway));
  } else {
    data->gateway[0] = '\0';
  }

  if (server.arg("i3").length() > 0) {
    server.arg("i3").toCharArray(data->subnet, sizeof(data->subnet));
  } else {
    data->subnet[0] = '\0';
  }

  if (server.arg("na").length() > 0) {
    data->noConnectionAttempts = server.arg("na").toInt();
  }

  if (server.arg("wc").length() > 0) {
    data->waitTimeConnections = server.arg("wc").toInt();
  }

  if (server.arg("ws").length() > 0) {
    data->waitTimeSeries = server.arg("ws").toInt();
  }

  if (server.arg("d").length() > 0) {
    data.isDHCP = true;
  } else {
    data->isDHCP = false;
  }
}

void AFEWebServer::getMQTTData(MQTT *data) {
  if (server.arg("h").length() > 0) {
    server.arg("h").toCharArray(data->host, sizeof(data->host));
  } else {
    data->host[0] = '\0';
  }

  if (server.arg("a").length() > 0) {
    server.arg("a").toCharArray(data->ip, sizeof(data->ip));

  } else {
    data->ip[0] = '\0';
  }

  if (server.arg("p").length() > 0) {
    data->port = server.arg("p").toInt();
  }

  if (server.arg("t").length() > 0) {
    data->timeout = server.arg("t").toInt();
  }

  if (server.arg("u").length() > 0) {
    server.arg("u").toCharArray(data->user, sizeof(data->user));
  } else {
    data->user[0] = '\0';
  }

  if (server.arg("s").length() > 0) {
    server.arg("s").toCharArray(data->password, sizeof(data->password));
  } else {
    data->password[0] = '\0';
  }
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  data->lwt.idx = server.arg("x").length() > 0 ? server.arg("x").toInt() : 0;
#else
  if (server.arg("t0").length() > 0) {
    server.arg("t0").toCharArray(data->lwt.topic, sizeof(data->lwt.topic));
  } else {
    data->lwt.topic[0] = '\0';
  }
#endif
}

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
void AFEWebServer::getDomoticzServerData(DOMOTICZ *data) {

  if (server.arg("t").length() > 0) {
    data->protocol = server.arg("t").toInt();
  }

  if (server.arg("h").length() > 0) {
    server.arg("h").toCharArray(data->host, sizeof(data->host));
  } else {
    data->host[0] = '\0';
  }

  if (server.arg("p").length() > 0) {
    data->port = server.arg("p").toInt();
  }

  if (server.arg("u").length() > 0) {
    server.arg("u").toCharArray(data->user, sizeof(data->user));
  } else {
    data->user[0] = '\0';
  }
  if (server.arg("s").length() > 0) {
    server.arg("s").toCharArray(data->password, sizeof(data->password));
  } else {
    data->password[0] = '\0';
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
void AFEWebServer::getRelayData(uint8_t id, RELAY *data) {

  data->gpio = server.arg("g").length() ? server.arg("g").toInt() : 0;

#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
  data->timeToOff =
      server.arg("ot").length() > 0 ? server.arg("ot").toFloat() : 0;
#endif

  data->state.powerOn =
      server.arg("pr").length() > 0 ? server.arg("pr").toInt() : 0;

  if (server.arg("n").length() > 0) {
    server.arg("n").toCharArray(data->name, sizeof(data->name));
  } else {
    data->name[0] = '\0';
  }

  data->state.MQTTConnected =
      server.arg("mc").length() > 0 ? server.arg("mc").toInt() : 0;

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTION
  data->thermalProtection = server.arg("tp" + String(id)).length() > 0
                                ? server.arg("tp" + String(id)).toInt()
                                : 0;
#endif

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (server.arg("t").length() > 0) {
    server.arg("t").toCharArray(data->mqtt.topic, sizeof(data->mqtt.topic));
  } else {
    data->mqtt.topic[0] = '\0';
  }
#else
  data->domoticz.idx = server.arg("x").length() > 0 ? server.arg("x").toInt()
                                                    : AFE_DOMOTICZ_DEFAULT_IDX;
#endif

  data->ledID = server.arg("l").length() > 0 ? server.arg("l").toInt()
                                             : AFE_HARDWARE_ITEM_NOT_EXIST;
}
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_SWITCH
void AFEWebServer::getSwitchData(uint8_t id, SWITCH *data) {
  data->type = server.arg("m").length() > 0 ? server.arg("m").toInt()
                                            : AFE_SWITCH_TYPE_MONO;

  data->sensitiveness = server.arg("s").length() > 0
                            ? server.arg("s").toInt()
                            : AFE_HARDWARE_SWITCH_DEFAULT_BOUNCING;

  data->functionality = server.arg("f").length() > 0
                            ? server.arg("f").toInt()
                            : AFE_SWITCH_FUNCTIONALITY_NONE;

  data->gpio = server.arg("g").length() > 0 ? server.arg("g").toInt() : 0;

  data->relayID = server.arg("r").length() > 0 ? server.arg("r").toInt()
                                               : AFE_HARDWARE_ITEM_NOT_EXIST;
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  data->domoticz.idx =
      server.arg("x").length() > 0 ? server.arg("x").toInt() : 0;
#else
  if (server.arg("t").length() > 0) {
    server.arg("t").toCharArray(data->mqtt.topic, sizeof(data->mqtt.topic));
  } else {
    data->mqtt.topic[0] = '\0';
  }
#endif
}
#endif // AFE_CONFIG_HARDWARE_SWITCH

void AFEWebServer::getPasswordData(PASSWORD *data) {
  if (server.arg("p").length() > 0) {
    server.arg("p").toCharArray(data->password, sizeof(data->password));
  } else {
    data->password[0] = '\0';
  }

  data->protect = server.arg("r").length() > 0 ? true : false;
}

void AFEWebServer::getSerialNumberData(PRO_VERSION *data) {
  if (server.arg("k").length() > 0) {
    server.arg("k").toCharArray(data->serial, sizeof(data->serial));
  }

  if (server.arg("v").length() > 0) {
    data->valid = server.arg("v").toInt() == 0 ? false : true;
  }
}

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
server.arg("te").length() > 0 ? data->enabled = true : data->enabled = false;

if (server.arg("tn").length() > 0) {
  data->turnOn = server.arg("tn").toFloat();
}

if (server.arg("tf").length() > 0) {
  data->turnOff = server.arg("tf").toFloat();
}

if (server.arg("ta").length() > 0) {
  data->turnOnAbove = server.arg("ta").toInt() == 0 ? false : true;
}

if (server.arg("tb").length() > 0) {
  data->turnOffAbove = server.arg("tb").toInt() == 0 ? false : true;
}
}
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
void AFEWebServer::getContactronData(uint8_t id, CONTACTRON *data) {
  data->type = server.arg("y").length() > 0
                   ? server.arg("y").toInt()
                   : AFE_CONFIG_HARDWARE_CONTACTRON_DEFAULT_OUTPUT_TYPE;

  data->ledID = server.arg("l").length() > 0 ? server.arg("l").toInt()
                                             : AFE_HARDWARE_ITEM_NOT_EXIST;

  data->bouncing = server.arg("b").length() > 0
                       ? server.arg("b").toInt()
                       : AFE_CONFIG_HARDWARE_CONTACTRON_DEFAULT_BOUNCING;

  data->gpio = server.arg("g").length() > 0 ? server.arg("g").toInt() : 0;

  if (server.arg("n").length() > 0) {
    server.arg("n").toCharArray(data->name, sizeof(data->name));
  } else {
    data->name[0] = '\0';
  }

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  data->domoticz.idx = server.arg("x").length() > 0 ? server.arg("x").toInt()
                                                    : AFE_DOMOTICZ_DEFAULT_IDX;
#else
  if (server.arg("t").length() > 0) {
    server.arg("t").toCharArray(data->mqtt.topic, sizeof(data->mqtt.topic));
  } else {
    data->mqtt.topic[0] = '\0';
  }
#endif
}
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
void AFEWebServer::getGateData(GATE *data) {

  if (server.arg("n").length() > 0) {
    server.arg("n").toCharArray(data->name, sizeof(data->name));
  } else {
    data->name[0] = '\0';
  }

  data->relayId = server.arg("r").length() > 0 ? server.arg("r").toInt()
                                               : AFE_HARDWARE_ITEM_NOT_EXIST;

  data->contactron.id[0] = server.arg("c1").length() > 0
                               ? server.arg("c1").toInt()
                               : AFE_HARDWARE_ITEM_NOT_EXIST;

  data->contactron.id[1] = server.arg("c2").length() > 0
                               ? server.arg("c2").toInt()
                               : AFE_HARDWARE_ITEM_NOT_EXIST;

  for (uint8_t i = 0; i < sizeof(data->states.state); i++) {
    data->states.state[i] = server.arg("s" + String(i)).length() > 0
                                ? server.arg("s" + String(i)).toInt()
                                : AFE_GATE_UNKNOWN;
  }
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  data->domoticz.idx = server.arg("x").length() > 0 ? server.arg("x").toInt()
                                                    : AFE_DOMOTICZ_DEFAULT_IDX;
  data->domoticzControl.idx = server.arg("z").length() > 0
                                  ? server.arg("z").toInt()
                                  : AFE_DOMOTICZ_DEFAULT_IDX;
#else
  if (server.arg("t").length() > 0) {
    server.arg("t").toCharArray(data->mqtt.topic, sizeof(data->mqtt.topic));
  } else {
    data->mqtt.topic[0] = '\0';
  }
#endif
}
#endif

#if defined(T3_CONFIG)
void AFEWebServer::getPIRData(uint8_t id, PIR *data) {

  if (server.arg("g" + String(id)).length() > 0) {
    data->gpio = server.arg("g" + String(id)).toInt();
  }

  if (server.arg("n" + String(id)).length() > 0) {
    server.arg("n" + String(id)).toCharArray(data->name, sizeof(data->name));
  }

  if (server.arg("l" + String(id)).length() > 0) {
    data->ledId = server.arg("l" + String(id)).toInt();
  }

  if (server.arg("r" + String(id)).length() > 0) {
    data->relayId = server.arg("r" + String(id)).toInt();
  }

  if (server.arg("d" + String(id)).length() > 0) {
    data->howLongKeepRelayOn = server.arg("d" + String(id)).toInt();
  }

  server.arg("i" + String(id)).length() > 0 ? data->invertRelayState = true
                                            : data->invertRelayState = false;

  if (server.arg("o" + String(id)).length() > 0) {
    data->type = server.arg("o" + String(id)).toInt();
  }

  if (server.arg("x" + String(id)).length() > 0) {
    data->idx = server.arg("x" + String(id)).toInt();
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
void AFEWebServer::getLEDData(uint8_t id, LED *data) {
  data->gpio = server.arg("g" + String(id)).length() > 0
                   ? server.arg("g" + String(id)).toInt()
                   : 0;

  data->changeToOppositeValue =
      server.arg("o" + String(id)).length() > 0 ? true : false;
}

uint8_t AFEWebServer::getSystemLEDData() {
  return server.arg("s").length() > 0 ? server.arg("s").toInt()
                                      : AFE_HARDWARE_ITEM_NOT_EXIST;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
void AFEWebServer::get(DS18B20 &data) {

  if (server.arg("n").length() > 0) {
    server.arg("n").toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = '\0';
  }

  data.address = server.arg("a").length() > 0
                     ? server.arg("a").toInt()
                     : AFE_CONFIG_HARDWARE_DS18B20_DEFAULT_ADDRESS;

  data.gpio = server.arg("g").length() > 0
                  ? server.arg("g").toInt()
                  : AFE_CONFIG_HARDWARE_DS18B20_DEFAULT_GPIO;

  data.correction =
      server.arg("k").length() > 0
          ? server.arg("k").toFloat()
          : AFE_CONFIG_HARDWARE_DS18B20_DEFAULT_TEMPERATURE_CORRECTION;

  data.interval = server.arg("f").length() > 0
                      ? server.arg("f").toInt()
                      : AFE_CONFIG_HARDWARE_DS18B20_DEFAULT_INTERVAL;

  data.unit = server.arg("u").length() > 0 ? server.arg("u").toInt()
                                           : AFE_TEMPERATURE_UNIT_CELSIUS;

  server.arg("s").length() > 0 ? data.sendOnlyChanges = true
                               : data.sendOnlyChanges = false;

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  data.domoticz.idx = server.arg("x").length() > 0 ? server.arg("x").toInt()
                                                   : AFE_DOMOTICZ_DEFAULT_IDX;
#else
  if (server.arg("t").length() > 0) {
    server.arg("t").toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = '\0';
  }
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
}

#endif

#ifdef AFE_CONFIG_HARDWARE_DHXX
void AFEWebServer::getDHTData(DH *data) {

  if (server.arg("g").length() > 0) {
    data->gpio = server.arg("g").toInt();
  }

  if (server.arg("t").length() > 0) {
    data->type = server.arg("t").toInt();
  }

  if (server.arg("c").length() > 0) {
    data->temperature.correction = server.arg("c").toFloat();
  }

  if (server.arg("i").length() > 0) {
    data->interval = server.arg("i").toInt();
  }

  if (server.arg("u").length() > 0) {
    data->temperature.unit = server.arg("u").toInt();
  }

  if (server.arg("d").length() > 0) {
    data->humidity.correction = server.arg("d").toFloat();
  }

  server.arg("j").length() > 0 ? data->publishDewPoint = true
                               : data->publishDewPoint = false;

  server.arg("o").length() > 0 ? data->sendOnlyChanges = true
                               : data->sendOnlyChanges = false;

  server.arg("p").length() > 0 ? data->publishHeatIndex = true
                               : data->publishHeatIndex = false;

  if (server.arg("xt").length() > 0) {
    data->temperatureIdx = server.arg("xt").toInt();
  }

  if (server.arg("xh").length() > 0) {
    data->humidityIdx = server.arg("xh").toInt();
  }

  if (server.arg("xth").length() > 0) {
    data->temperatureAndHumidityIdx = server.arg("xth").toInt();
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_UART
void AFEWebServer::getSerialPortData(SERIALPORT *data) {

  data->RXD = server.arg("r").length() > 0
                  ? server.arg("r").toInt()
                  : AFE_CONFIG_HARDWARE_UART_DEFAULT_RXD;
  data->TXD = server.arg("t").length() > 0
                  ? server.arg("t").toInt()
                  : AFE_CONFIG_HARDWARE_UART_DEFAULT_TXD;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_I2C
void AFEWebServer::getI2CPortData(I2CPORT *data) {
  data->SDA = server.arg("a").length() > 0
                  ? server.arg("a").toInt()
                  : AFE_CONFIG_HARDWARE_I2C_DEFAULT_SDA;
  data->SCL = server.arg("l").length() > 0
                  ? server.arg("l").toInt()
                  : AFE_CONFIG_HARDWARE_I2C_DEFAULT_SCL;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
void AFEWebServer::getHPMA115S0SensorData(HPMA115S0 *data) {
  data->interval = server.arg("f").length() > 0
                       ? server.arg("f").toInt()
                       : AFE_CONFIG_HARDWARE_HPMA115S_DEFAULT_INTERVAL;

  data->timeToMeasure =
      server.arg("m").length() > 0
          ? server.arg("m").toInt()
          : AFE_CONFIG_HARDWARE_HPMA115S_DEFAULT_TIME_TO_MEASURE;
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  data->domoticz.pm25.idx = server.arg("x2").length() > 0
                                ? server.arg("x2").toInt()
                                : AFE_DOMOTICZ_DEFAULT_IDX;
  data->domoticz.pm10.idx = server.arg("x1").length() > 0
                                ? server.arg("x1").toInt()
                                : AFE_DOMOTICZ_DEFAULT_IDX;
#else
  if (server.arg("t").length() > 0) {
    server.arg("t").toCharArray(data->mqtt.topic, sizeof(data->mqtt.topic));
  } else {
    data->mqtt.topic[0] = '\0';
  }
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

  if (server.arg("n").length() > 0) {
    server.arg("n").toCharArray(data->name, sizeof(data->name));
  } else {
    data->name[0] = '\0';
  }
};
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
void AFEWebServer::getBMEX80SensorData(BMEX80 *data) {
  data->type = server.arg("b").length() > 0 ? server.arg("b").toInt()
                                            : AFE_BMX_UNKNOWN_SENSOR;

  data->i2cAddress = server.arg("a").length() > 0 ? server.arg("a").toInt() : 0;

  data->interval = server.arg("f").length() > 0
                       ? server.arg("f").toInt()
                       : AFE_CONFIG_HARDWARE_BMEX80_DEFAULT_INTERVAL;

  data->resolution = server.arg("r").length() > 0 ? server.arg("r").toInt()
                                                  : BMP085_ULTRAHIGHRES;

  data->seaLevelPressure = server.arg("s").length() > 0
                               ? server.arg("s").toInt()
                               : AFE_CONFIG_DEFAULT_SEA_LEVEL_PRESSURE;

  data->altitude = server.arg("hi").length() > 0 ? server.arg("hi").toInt() : 0;

  data->temperature.unit = server.arg("tu").length() > 0
                               ? server.arg("tu").toInt()
                               : AFE_TEMPERATURE_UNIT_CELSIUS;

  data->temperature.correction =
      server.arg("tc").length() > 0 ? server.arg("tc").toFloat() : 0;

  data->humidity.correction =
      server.arg("hc").length() > 0 ? server.arg("hc").toFloat() : 0;

  data->pressure.unit = server.arg("pu").length() > 0 ? server.arg("pu").toInt()
                                                      : AFE_PRESSURE_UNIT_HPA;
  data->pressure.correction =
      server.arg("pc").length() > 0 ? server.arg("pc").toFloat() : 0;

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  data->domoticz.temperatureHumidityPressure.idx =
      server.arg("i0").length() > 0 ? server.arg("i0").toInt()
                                    : AFE_DOMOTICZ_DEFAULT_IDX;

  data->domoticz.temperature.idx = server.arg("i1").length() > 0
                                       ? server.arg("i1").toInt()
                                       : AFE_DOMOTICZ_DEFAULT_IDX;

  data->domoticz.humidity.idx = server.arg("i2").length() > 0
                                    ? server.arg("i2").toInt()
                                    : AFE_DOMOTICZ_DEFAULT_IDX;

  data->domoticz.dewPoint.idx = server.arg("i3").length() > 0
                                    ? server.arg("i3").toInt()
                                    : AFE_DOMOTICZ_DEFAULT_IDX;

  data->domoticz.heatIndex.idx = server.arg("i4").length() > 0
                                     ? server.arg("i4").toInt()
                                     : AFE_DOMOTICZ_DEFAULT_IDX;

  data->domoticz.pressure.idx = server.arg("i5").length() > 0
                                    ? server.arg("i5").toInt()
                                    : AFE_DOMOTICZ_DEFAULT_IDX;

  data->domoticz.relativePressure.idx = server.arg("i6").length() > 0
                                            ? server.arg("i6").toInt()
                                            : AFE_DOMOTICZ_DEFAULT_IDX;

  data->domoticz.iaq.idx = server.arg("i7").length() > 0
                               ? server.arg("i7").toInt()
                               : AFE_DOMOTICZ_DEFAULT_IDX;

  data->domoticz.staticIaq.idx = server.arg("i8").length() > 0
                                     ? server.arg("i8").toInt()
                                     : AFE_DOMOTICZ_DEFAULT_IDX;

  data->domoticz.co2Equivalent.idx = server.arg("i9").length() > 0
                                         ? server.arg("i9").toInt()
                                         : AFE_DOMOTICZ_DEFAULT_IDX;

  data->domoticz.breathVocEquivalent.idx = server.arg("i10").length() > 0
                                               ? server.arg("i10").toInt()
                                               : AFE_DOMOTICZ_DEFAULT_IDX;

  data->domoticz.gasResistance.idx = server.arg("i11").length() > 0
                                         ? server.arg("i11").toInt()
                                         : AFE_DOMOTICZ_DEFAULT_IDX;

  data->domoticz.temperatureHumidity.idx = server.arg("i12").length() > 0
                                               ? server.arg("i12").toInt()
                                               : AFE_DOMOTICZ_DEFAULT_IDX;
#else
  if (server.arg("t").length() > 0) {
    server.arg("t").toCharArray(data->mqtt.topic, sizeof(data->mqtt.topic));
  } else {
    data->mqtt.topic[0] = '\0';
  }
#endif

  if (server.arg("n").length() > 0) {
    server.arg("n").toCharArray(data->name, sizeof(data->name));
  } else {
    data->name[0] = '\0';
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
void AFEWebServer::getBH1750SensorData(BH1750 *data) {
  data->i2cAddress = server.arg("a").length() > 0 ? server.arg("a").toInt() : 0;

  data->interval = server.arg("f").length() > 0
                       ? server.arg("f").toInt()
                       : AFE_CONFIG_HARDWARE_BH1750_DEFAULT_INTERVAL;

  data->mode = server.arg("m").length() > 0
                   ? server.arg("m").toInt()
                   : AFE_CONFIG_HARDWARE_BH1750_DEFAULT_MODE;
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  data->domoticz.idx = server.arg("d").length() > 0 ? server.arg("d").toInt()
                                                    : AFE_DOMOTICZ_DEFAULT_IDX;
#else
  if (server.arg("t").length() > 0) {
    server.arg("t").toCharArray(data->mqtt.topic, sizeof(data->mqtt.topic));
  } else {
    data->mqtt.topic[0] = '\0';
  }
#endif

  if (server.arg("n").length() > 0) {
    server.arg("n").toCharArray(data->name, sizeof(data->name));
  } else {
    data->name[0] = '\0';
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
void AFEWebServer::getAS3935SensorData(AS3935 *data) {

  if (server.arg("n").length() > 0) {
    server.arg("n").toCharArray(data->name, sizeof(data->name));
  } else {
    data->name[0] = '\0';
  }

  data->i2cAddress = server.arg("a").length() > 0 ? server.arg("a").toInt() : 0;

  data->irqGPIO = server.arg("g").length() > 0 ? server.arg("g").toInt() : 0;

  data->setNoiseFloorAutomatically =
      server.arg("f").length() > 0 ? true : false;

  data->noiseFloor = server.arg("nf").length() > 0
                         ? server.arg("nf").toInt()
                         : AFE_CONFIG_HARDWARE_AS3935_DEFAULT_NOISE_FLOOR;

  data->minimumNumberOfLightningSpikes =
      server.arg("m").length() > 0
          ? server.arg("m").toInt()
          : AFE_CONFIG_HARDWARE_AS3935_DEFAULT_MINIMUM_NO_OF_SPIKES;

  data->watchdogThreshold =
      server.arg("e").length() > 0
          ? server.arg("e").toInt()
          : AFE_CONFIG_HARDWARE_AS3935_DEFAULT_WATCHDOG_THRESHOLD;

  data->spikesRejectionLevel =
      server.arg("s").length() > 0
          ? server.arg("s").toInt()
          : AFE_CONFIG_HARDWARE_AS3935_DEFAULT_SPIKES_REJECTION_LEVEL;

  data->indoor = server.arg("w").length() > 0 && server.arg("w").toInt() == 1
                     ? true
                     : false;

  data->unit =
      server.arg("u").length() > 0 ? server.arg("u").toInt() : AFE_DISTANCE_KM;
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  data->domoticz.idx =
      server.arg("d").length() > 0 ? server.arg("d").toInt() : 0;
#else
  if (server.arg("t").length() > 0) {
    server.arg("t").toCharArray(data->mqtt.topic, sizeof(data->mqtt.topic));
  } else {
    data->mqtt.topic[0] = '\0';
  }
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
}
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER_SENSOR
void AFEWebServer::getAnemometerSensorData(ANEMOMETER *data) {
  if (server.arg("n").length() > 0) {
    server.arg("n").toCharArray(data->name, sizeof(data->name));
  } else {
    data->name[0] = '\0';
  }

  data->gpio = server.arg("g").length() > 0
                   ? server.arg("g").toInt()
                   : AFE_HARDWARE_ANEMOMETER_SENSOR_DEFAULT_GPIO;

  data->interval = server.arg("f").length() > 0
                       ? server.arg("f").toInt()
                       : AFE_HARDWARE_ANEMOMETER_SENSOR_DEFAULT_INTERVAL;

  data->sensitiveness = server.arg("s").length() > 0
                            ? server.arg("s").toInt()
                            : AFE_HARDWARE_ANEMOMETER_SENSOR_DEFAULT_BOUNCING;

  data->impulseDistance =
      server.arg("l").length() > 0
          ? server.arg("l").toFloat()
          : AFE_HARDWARE_ANEMOMETER_SENSOR_DEFAULT_IMPULSE_DISTANCE;

  data->impulseDistanceUnit =
      server.arg("u").length() > 0
          ? server.arg("u").toInt()
          : AFE_HARDWARE_ANEMOMETER_SENSOR_DEFAULT_IMPULSE_DISTANCE_UNIT;

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  data->domoticz.idx =
      server.arg("x").length() > 0 ? server.arg("x").toInt() : 0;
#else
  if (server.arg("t").length() > 0) {
    server.arg("t").toCharArray(data->mqtt.topic, sizeof(data->mqtt.topic));
  } else {
    data->mqtt.topic[0] = '\0';
  }
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
}
#endif // AFE_CONFIG_HARDWARE_ANEMOMETER_SENSOR

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
void AFEWebServer::getAnalogInputData(ADCINPUT *data) {
  data->gpio = server.arg("g").length() > 0
                   ? server.arg("g").toInt()
                   : AFE_CONFIG_HARDWARE_ADC_VCC_DEFAULT_GPIO;

  data->interval = server.arg("v").length() > 0
                       ? server.arg("v").toInt()
                       : AFE_CONFIG_HARDWARE_ADC_VCC_DEFAULT_INTERVAL;

  data->numberOfSamples =
      server.arg("n").length() > 0
          ? server.arg("n").toInt()
          : AFE_CONFIG_HARDWARE_ADC_VCC_DEFAULT_NUMBER_OF_SAMPLES;

  data->maxVCC = server.arg("m").length() > 0
                     ? server.arg("m").toFloat()
                     : AFE_CONFIG_HARDWARE_ADC_VCC_DEFAULT_MAX_VCC;

  data->divider.Ra =
      server.arg("ra").length() > 0 ? server.arg("ra").toFloat() : 0;

  data->divider.Rb =
      server.arg("rb").length() > 0 ? server.arg("rb").toFloat() : 0;

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (server.arg("t").length() > 0) {
    server.arg("t").toCharArray(data->mqtt.topic, sizeof(data->mqtt.topic));
  } else {
    data->mqtt.topic[0] = '\0';
  }
#else
  data->domoticz.raw = server.arg("x0").length() > 0 ? server.arg("x0").toInt()
                                                     : AFE_DOMOTICZ_DEFAULT_IDX;
  data->domoticz.percent = server.arg("x1").length() > 0
                               ? server.arg("x1").toInt()
                               : AFE_DOMOTICZ_DEFAULT_IDX;
  data->domoticz.voltage = server.arg("x2").length() > 0
                               ? server.arg("x2").toInt()
                               : AFE_DOMOTICZ_DEFAULT_IDX;
  data->domoticz.voltageCalculated = server.arg("x3").length() > 0
                                         ? server.arg("x3").toInt()
                                         : AFE_DOMOTICZ_DEFAULT_IDX;
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
  data->battery.minVoltage =
      server.arg("lv").length() > 0
          ? server.arg("lv").toFloat()
          : AFE_CONFIG_HARDWARE_ADC_VCC_DEFAULT_BATTER_MIN_V;

  data->battery.maxVoltage =
      server.arg("hv").length() > 0
          ? server.arg("hv").toFloat()
          : AFE_CONFIG_HARDWARE_ADC_VCC_DEFAULT_BATTER_MAX_V;

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (server.arg("bt").length() > 0) {
    server.arg("bt").toCharArray(data->battery.mqtt.topic,
                                 sizeof(data->battery.mqtt.topic));
  } else {
    data->battery.mqtt.topic[0] = '\0';
  }
#else
  data->battery.domoticz.idx = server.arg("bx").length() > 0 ? server.arg("bx").toInt()
                                                    : AFE_DOMOTICZ_DEFAULT_IDX;
#endif

#endif // AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
}
#endif // AFE_CONFIG_HARDWARE_ADC_VCC

#ifdef AFE_CONFIG_HARDWARE_RAINMETER_SENSOR
void AFEWebServer::getRainmeterSensorData(RAINMETER *data) {

  if (server.arg("n").length() > 0) {
    server.arg("n").toCharArray(data->name, sizeof(data->name));
  } else {
    data->name[0] = '\0';
  }

  data->gpio = server.arg("g").length() > 0
                   ? server.arg("g").toInt()
                   : AFE_HARDWARE_RAINMETER_SENSOR_DEFAULT_GPIO;

  data->interval = server.arg("f").length() > 0
                       ? server.arg("f").toInt()
                       : AFE_HARDWARE_RAINMETER_SENSOR_DEFAULT_INTERVAL;

  data->sensitiveness = server.arg("s").length() > 0
                            ? server.arg("s").toInt()
                            : AFE_HARDWARE_RAINMETER_SENSOR_DEFAULT_BOUNCING;

  data->resolution =
      server.arg("r").length() > 0
          ? server.arg("r").toFloat()
          : (float)AFE_HARDWARE_RAINMETER_SENSOR_DEFAULT_RESOLUTION;

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  data->domoticz.idx =
      server.arg("x").length() > 0 ? server.arg("x").toInt() : 0;
#else
  if (server.arg("t").length() > 0) {
    server.arg("t").toCharArray(data->mqtt.topic, sizeof(data->mqtt.topic));
  } else {
    data->mqtt.topic[0] = '\0';
  }
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
}
#endif // AFE_CONFIG_HARDWARE_RAINMETER_SENSOR
