/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Web-Server.h"

AFEWebServer::AFEWebServer() {}

#ifdef AFE_CONFIG_HARDWARE_LED
void AFEWebServer::begin(AFEDataAccess *_Data, AFEDevice *_Device,
                         AFEFirmwarePro *_FirmwarePro, AFELED *_Led,
                         AFEJSONRPC *_RestAPI) {
  SystemLED = _Led;
  begin(_Data, _Device, _FirmwarePro, _RestAPI);
}
#endif // AFE_CONFIG_HARDWARE_LED

void AFEWebServer::begin(AFEDataAccess *_Data, AFEDevice *_Device,
                         AFEFirmwarePro *_FirmwarePro, AFEJSONRPC *_RestAPI) {
  server.begin();
  Site.begin(_Data, _Device, _FirmwarePro, _RestAPI);
  Data = _Data;
  Device = _Device;
  RestAPI = _RestAPI;
  FirmwarePro = _FirmwarePro;
}

String AFEWebServer::generateSite(AFE_SITE_PARAMETERS *siteConfig,
                                  String &page) {

  if (siteConfig->twoColumns) {
    Site.generateMenu(page, siteConfig->rebootTime);
  } else {
    Site.generateEmptyMenu(page, siteConfig->rebootTime);
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
    Site.siteIndex(page, siteConfig->deviceID == -1 ? true : false);
    break;
  case AFE_CONFIG_SITE_FIRST_TIME:
    Site.siteNetwork(page);
    break;
  case AFE_CONFIG_SITE_FIRST_TIME_CONNECTING:
    Site.siteConnecting(page);
    break;
  case AFE_CONFIG_SITE_DEVICE:
    Site.siteDevice(page);
    break;
  case AFE_CONFIG_SITE_NETWORK:
    Site.siteNetwork(page);
    break;
  case AFE_CONFIG_SITE_MQTT:
    Site.siteMQTTBroker(page);
    break;
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  case AFE_CONFIG_SITE_DOMOTICZ:
    Site.siteDomoticzServer(page);
    break;
#endif
  case AFE_CONFIG_SITE_PASSWORD:
    Site.sitePassword(page);
    break;
  case AFE_CONFIG_SITE_PRO_VERSION:
    Site.siteProKey(page);
    break;
  case AFE_CONFIG_SITE_EXIT:
    Site.siteExit(page, siteConfig->rebootMode);
    break;
  case AFE_CONFIG_SITE_RESET:
    Site.siteReset(page);
    break;
  case AFE_CONFIG_SITE_POST_RESET:
    Site.sitePostReset(page);
    break;
#ifndef AFE_CONFIG_OTA_NOT_UPGRADABLE
  case AFE_CONFIG_SITE_UPGRADE:
    Site.siteUpgrade(page);
    break;
  case AFE_CONFIG_SITE_POST_UPGRADE:
    Site.sitePostUpgrade(page, upgradeSuccess);
    break;
  case AFE_CONFIG_SITE_WAN_UPGRADE:
    Site.siteWANUpgrade(page, F(L_UPGRADE_IN_PROGRESS));
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_RELAY
  case AFE_CONFIG_SITE_RELAY:
    Site.siteRelay(page, siteConfig->deviceID);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_SWITCH
  case AFE_CONFIG_SITE_SWITCH:
    Site.siteSwitch(page, siteConfig->deviceID);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  case AFE_CONFIG_SITE_ANALOG_INPUT:
    Site.siteADCInput(page);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  case AFE_CONFIG_SITE_CONTACTRON:
    Site.siteContactron(page, siteConfig->deviceID);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_GATE
  case AFE_CONFIG_SITE_GATE:
    Site.siteGate(page, siteConfig->deviceID);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_UART
  case AFE_CONFIG_SITE_UART:
    Site.siteUARTBUS(page);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_I2C
  case AFE_CONFIG_SITE_I2C:
    Site.siteI2CBUS(page);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_BMEX80
  case AFE_CONFIG_SITE_BMEX80:
    Site.siteBMEX80Sensor(page, siteConfig->deviceID);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  case AFE_CONFIG_SITE_HPMA115S0:
    Site.siteHPMA115S0Sensor(page, siteConfig->deviceID);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_BH1750
  case AFE_CONFIG_SITE_BH1750:
    Site.siteBH1750Sensor(page, siteConfig->deviceID);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_AS3935
  case AFE_CONFIG_SITE_AS3935:
    Site.siteAS3935Sensor(page, siteConfig->deviceID);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_DS18B20
  case AFE_CONFIG_SITE_DS18B20:
    Site.siteDS18B20Sensor(page, siteConfig->deviceID);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_DHT
  case AFE_CONFIG_SITE_DHT:
    Site.siteDHTSensor(page, siteConfig->deviceID);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
  case AFE_CONFIG_SITE_ANEMOMETER_SENSOR:
    Site.siteAnemometerSensor(page);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_RAINMETER
  case AFE_CONFIG_SITE_RAINMETER_SENSOR:
    Site.siteRainmeterSensor(page);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_LED
  case AFE_CONFIG_SITE_LED:
    Site.siteLED(page, siteConfig->deviceID);
    break;
  case AFE_CONFIG_SITE_SYSTEM_LED:
    Site.siteSystemLED(page);
    break;
#endif
#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  case AFE_CONFIG_SITE_REGULATOR:
    Site.siteRegulator(page, siteConfig->deviceID);
    break;
#endif
#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
  case AFE_CONFIG_SITE_THERMAL_PROTECTOR:
    Site.siteThermalProtector(page, siteConfig->deviceID);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
  case AFE_CONFIG_SITE_BINARY_SENSOR:
    Site.siteBinarySensor(page, siteConfig->deviceID);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_PN532
  case AFE_CONFIG_SITE_PN532_SENSOR:
    Site.sitePN532Sensor(page, siteConfig->deviceID);
    break;
  case AFE_CONFIG_SITE_PN532_SENSOR_ADMIN:
    Site.sitePN532SensorAdmin(page, siteConfig->deviceID);
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

  return page;
}

void AFEWebServer::generate(boolean upload) {

  if (getOptionName()) {
    return;
  }

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
             << F("INFO: SITE: Setting auto-logout to ")
             << siteConfig.rebootTime << F("seconds");
#endif
    }

    if (command == AFE_SERVER_CMD_SAVE) {
      if (siteConfig.ID == AFE_CONFIG_SITE_FIRST_TIME) {
        NETWORK configuration;
        get(configuration);
        siteConfig.twoColumns = false;
        siteConfig.reboot = true;
        siteConfig.rebootMode = AFE_MODE_CONFIGURATION;
        siteConfig.form = false;
        siteConfig.ID = AFE_CONFIG_SITE_FIRST_TIME_CONNECTING;
        Data->saveConfiguration(&configuration);
        configuration = {0};
      } else if (siteConfig.ID == AFE_CONFIG_SITE_DEVICE) {
        DEVICE configuration;
        get(configuration);
        Data->saveConfiguration(&configuration);
        configuration = {0};
      } else if (siteConfig.ID == AFE_CONFIG_SITE_NETWORK) {
        NETWORK configuration;
        get(configuration);
        Data->saveConfiguration(&configuration);
        configuration = {0};
      } else if (siteConfig.ID == AFE_CONFIG_SITE_PASSWORD) {
        PASSWORD configuration;
        get(configuration);
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
        get(configuration);
        Data->saveConfiguration(&configuration);
        sprintf(RestAPI->Pro.serial, "%s", configuration.serial);
        sprintf(FirmwarePro->Pro.serial, "%s", configuration.serial);
        FirmwarePro->validate();
        configuration = {0};
      } else if (siteConfig.ID == AFE_CONFIG_SITE_MQTT) {
        MQTT configuration;
        get(configuration);
        Data->saveConfiguration(&configuration);
        configuration = {0};
      }
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
      else if (siteConfig.ID == AFE_CONFIG_SITE_DOMOTICZ) {
        DOMOTICZ configuration;
        get(configuration);
        Data->saveConfiguration(&configuration);
        configuration = {0};
      }
#endif
#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
      else if (siteConfig.ID == AFE_CONFIG_SITE_ANALOG_INPUT) {
        ADCINPUT configuration;
        get(configuration);
        Data->saveConfiguration(&configuration);
        configuration = {0};
      }
#endif
#ifdef AFE_CONFIG_HARDWARE_LED
      else if (siteConfig.ID == AFE_CONFIG_SITE_LED) {
        LED configuration;
        get(configuration);
        Data->saveConfiguration(siteConfig.deviceID, &configuration);
        configuration = {0};
      } else if (siteConfig.ID == AFE_CONFIG_SITE_SYSTEM_LED) {
        Data->saveSystemLedID(getSystemLEDData());
      }
#endif
#ifdef AFE_CONFIG_HARDWARE_RELAY
      else if (siteConfig.ID == AFE_CONFIG_SITE_RELAY) {
        RELAY configuration;
        get(configuration);
        Data->saveConfiguration(siteConfig.deviceID, &configuration);
        configuration = {0};
      }
#endif
#ifdef AFE_CONFIG_HARDWARE_SWITCH
      else if (siteConfig.ID == AFE_CONFIG_SITE_SWITCH) {
        SWITCH configuration;
        get(configuration);
        Data->saveConfiguration(siteConfig.deviceID, &configuration);
        configuration = {0};
      }
#endif
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
      else if (siteConfig.ID == AFE_CONFIG_SITE_CONTACTRON) {
        CONTACTRON configuration;
        get(configuration);
        Data->saveConfiguration(siteConfig.deviceID, &configuration);
        // @TODO why this doesn't work here? => configuration = {0};
      }
#endif
#ifdef AFE_CONFIG_HARDWARE_GATE
      else if (siteConfig.ID == AFE_CONFIG_SITE_GATE) {
        GATE configuration;
        get(configuration);
        Data->saveConfiguration(siteConfig.deviceID, &configuration);
        configuration = {0};
      }
#endif
#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
      else if (siteConfig.ID == AFE_CONFIG_SITE_HPMA115S0) {
        HPMA115S0 configuration;
        get(configuration);
        Data->saveConfiguration(siteConfig.deviceID, &configuration);
        configuration = {0};
      }
#endif
#ifdef AFE_CONFIG_HARDWARE_BMEX80
      else if (siteConfig.ID == AFE_CONFIG_SITE_BMEX80) {
        BMEX80 configuration;
        get(configuration);
        Data->saveConfiguration(siteConfig.deviceID, &configuration);
        configuration = {0};
      }
#endif
#ifdef AFE_CONFIG_HARDWARE_BH1750
      else if (siteConfig.ID == AFE_CONFIG_SITE_BH1750) {
        BH1750 configuration;
        get(configuration);
        Data->saveConfiguration(siteConfig.deviceID, &configuration);
        configuration = {0};
      }
#endif
#ifdef AFE_CONFIG_HARDWARE_AS3935
      else if (siteConfig.ID == AFE_CONFIG_SITE_AS3935) {
        AS3935 configuration;
        get(configuration);
        Data->saveConfiguration(siteConfig.deviceID, &configuration);
        configuration = {0};
      }
#endif
#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
      else if (siteConfig.ID == AFE_CONFIG_SITE_ANEMOMETER_SENSOR) {
        ANEMOMETER configuration;
        get(configuration);
        Data->saveConfiguration(&configuration);
        configuration = {0};
      }
#endif
#ifdef AFE_CONFIG_HARDWARE_RAINMETER
      else if (siteConfig.ID == AFE_CONFIG_SITE_RAINMETER_SENSOR) {
        RAINMETER configuration;
        get(configuration);
        Data->saveConfiguration(&configuration);
        configuration = {0};
      }
#endif
#ifdef AFE_CONFIG_HARDWARE_DS18B20
      else if (siteConfig.ID == AFE_CONFIG_SITE_DS18B20) {
        DS18B20 ds18B20Configuration;
        get(ds18B20Configuration);
        Data->saveConfiguration(siteConfig.deviceID, &ds18B20Configuration);
        ds18B20Configuration = {0};
      }
#endif
#ifdef AFE_CONFIG_HARDWARE_DHT
      else if (siteConfig.ID == AFE_CONFIG_SITE_DHT) {
        DHT dhtConfiguration;
        get(dhtConfiguration);
        Data->saveConfiguration(siteConfig.deviceID, &dhtConfiguration);
        dhtConfiguration = {0};
      }
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
        get(configuration);
        Data->saveConfiguration(&configuration);
        configuration = {0};
      }
#endif
#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
      else if (siteConfig.ID == AFE_CONFIG_SITE_REGULATOR) {
        REGULATOR configuration;
        get(configuration);
        Data->saveConfiguration(siteConfig.deviceID, &configuration);
        configuration = {0};
      }
#endif
#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
      else if (siteConfig.ID == AFE_CONFIG_SITE_THERMAL_PROTECTOR) {
        THERMAL_PROTECTOR configuration;
        get(configuration);
        Data->saveConfiguration(siteConfig.deviceID, &configuration);
        configuration = {0};
      }
#endif
#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
      else if (siteConfig.ID == AFE_CONFIG_SITE_BINARY_SENSOR) {
        BINARY_SENSOR configuration;
        get(configuration);
        Data->saveConfiguration(siteConfig.deviceID, &configuration);
        configuration = {0};
      }
#endif
#ifdef AFE_CONFIG_HARDWARE_PN532
      else if (siteConfig.ID == AFE_CONFIG_SITE_PN532_SENSOR) {
        PN532_SENSOR configuration;
        get(configuration);
        Data->saveConfiguration(0, &configuration);
        configuration = {0};
      } else if (siteConfig.ID == AFE_CONFIG_SITE_PN532_SENSOR_ADMIN) {
        processMiFareCard();
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
            get(data);
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
#ifndef AFE_CONFIG_OTA_NOT_UPGRADABLE
      } else if (siteConfig.ID == AFE_CONFIG_SITE_UPGRADE) {
        siteConfig.form = false;
      } else if (siteConfig.ID == AFE_CONFIG_SITE_WAN_UPGRADE) {
        siteConfig.form = false;
        siteConfig.rebootTime = 30;
        siteConfig.twoColumns = false;
      } else if (siteConfig.ID == AFE_CONFIG_SITE_POST_UPGRADE) {
        if (!upload) {
          siteConfig.form = false;
          siteConfig.twoColumns = false;
          siteConfig.rebootTime = 15;
          siteConfig.reboot = true;
          siteConfig.rebootMode = Device->getMode();
        }
#endif // AFE_CONFIG_OTA_NOT_UPGRADABLE
      }
    }
  }

#ifndef AFE_CONFIG_OTA_NOT_UPGRADABLE
  if (upload) {
    upgradeSuccess = upgradOTAFile();
  } else {
#endif // #ifndef AFE_CONFIG_OTA_NOT_UPGRADABLE

#ifdef DEBUG
    Serial << endl
           << F("INFO: SITE: Starting generating. ") << endl
           << F("INFO: MEMORY: Free: ") << system_get_free_heap_size() / 1024
           << F("kB");
#endif

    String page;
// page.reserve(AFE_MAX_PAGE_SIZE);

#ifdef DEBUG
    Serial << endl
           << F("INFO: MEMORY: Free after allocation for HTTP response : ")
           << system_get_free_heap_size() / 1024 << F("kB");
#endif
    generateSite(&siteConfig, page);

#ifdef DEBUG
    Serial << endl
           << F("INFO: MEMORY: Free after site generated : ")
           << system_get_free_heap_size() / 1024 << F("kB");
#endif

    publishHTML(page);

#ifndef AFE_CONFIG_OTA_NOT_UPGRADABLE
    if (siteConfig.ID == AFE_CONFIG_SITE_WAN_UPGRADE) {
      upgradeSuccess = upgradeOTAWAN(getOTAFirmwareId());
    }
  }
#endif

  /* Rebooting device */
  if (siteConfig.reboot) {
#ifdef AFE_CONFIG_HARDWARE_LED
    SystemLED->on();
#endif
    Device->reboot(siteConfig.rebootMode);
  }
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
             << F("INFO: SITE: Automatic logout from the config panel after : ")
             << Device->configuration.timeToAutoLogOff
             << F("min. of idle time");
#endif
      Device->reboot(AFE_MODE_NORMAL);
    }
  }
}

boolean AFEWebServer::httpAPIlistener() { return receivedHTTPCommand; }

void AFEWebServer::publishHTML(const String &page) {
  uint16_t pageSize = page.length();
// uint16_t size = 1024;

#ifdef DEBUG
  Serial << endl
         << F("INFO: SITE: Streaming started. To transfer: ")
         << (pageSize / 1024) << F("kB");

  if (pageSize + 100 > AFE_MAX_PAGE_SIZE) {
    Serial << endl
           << F("ERROR: SITE: Buffor ") << AFE_MAX_PAGE_SIZE
           << F("B too small : ") << pageSize << F(" ... ");
  }
#endif
  server.sendHeader("Content-Length", String(page.length()));
  server.setContentLength(pageSize);
  server.send(200, "text/html", page);

/*
  if (pageSize > size) {
#ifdef DEBUG
    Serial << endl
           << F("INFO: MEMORY: Free :  size after sending Header: ")
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
*/

#ifdef DEBUG
  Serial << endl << F("INFO: SITE: Published");
#endif

  if ((Device->getMode() == AFE_MODE_CONFIGURATION ||
       Device->getMode() == AFE_MODE_ACCESS_POINT) &&
      Device->configuration.timeToAutoLogOff > 0) {
    howLongInConfigMode = millis();
  }
}

void AFEWebServer::sendJSON(const String &json) {
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

/* Upgrade methods */

#ifndef AFE_CONFIG_OTA_NOT_UPGRADABLE
boolean AFEWebServer::upgradeOTAWAN(uint16_t firmwareId) {
  t_httpUpdate_return ret;
  ESP8266HTTPUpdate OTAServerUpdate;
  WiFiClient WirelessClient;
  boolean _success = false;

#ifdef AFE_CONFIG_HARDWARE_LED
  SystemLED->on();
#endif

  if (firmwareId > 0) {
    OTAServerUpdate.rebootOnUpdate(false);

#ifdef DEBUG
    Serial << endl << F("INFO: WAN UPDATE: Starting upgrade");
#endif
    ret = OTAServerUpdate.update(WirelessClient,
                                 AFE_CONFIG_JSONRPC_DOWNLOAD_API_URL +
                                     String(firmwareId));

#ifdef DEBUG
    Serial << endl
           << F("INFO: WAN UPDATE: Update completed with status: ") << ret;
#endif

    if (ret != HTTP_UPDATE_NO_UPDATES) {
      if (ret == HTTP_UPDATE_OK) {
#ifdef DEBUG
        Serial << endl << F("INFO: WAN UPDATE: success");
#endif
        Data->saveWelecomeMessage(F(L_UPGRADE_SUCCESSFUL));
        delay(1000);
        Device->reboot(Device->getMode());
      } else {
        if (ret == HTTP_UPDATE_FAILED) {
#ifdef DEBUG
          Serial << endl << F("INFO: WAN UPDATE: failure");
#endif
          _success = false;
          Data->saveWelecomeMessage(F(L_UPGRADE_FAILED));
        }
      }
    }
  }
#ifdef AFE_CONFIG_HARDWARE_LED
  SystemLED->off();
#endif

  return _success;
}

boolean AFEWebServer::upgradOTAFile(void) {
  HTTPUpload &upload = server.upload();
  String _updaterError;
  boolean _success = false;
  if (upload.status == UPLOAD_FILE_START) {
    WiFiUDP::stopAll();

#ifdef DEBUG
    Serial << endl
           << F("INFO: UPGRADE: Firmware file name: ")
           << upload.filename.c_str();
#endif

    uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;

#ifdef DEBUG
    Serial << endl
           << F("INFO: UPGRADE: Firmware size: ")
           << (ESP.getSketchSize() / 1024) << "Kb" << endl
           << F("INFO: UPGRADE: Free space size: ")
           << (ESP.getFreeSketchSpace() / 1024) << "Kb" << endl
           << F("INFO: UPGRADE: Max free space size for this hardware:")
           << (maxSketchSpace / 1024) << "Kb" << endl
           << F("INFO: UPGRADE: ");
#endif

    if (!Update.begin(maxSketchSpace)) { // start with max available
#ifdef DEBUG
      Update.printError(Serial);
#endif
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
      Serial << endl;
      Update.printError(Serial);
#endif
    }
  } else if (upload.status == UPLOAD_FILE_END && !_updaterError.length()) {
    if (Update.end(true)) { // true to set the size to the current
      // progress
      _success = true;
#ifdef DEBUG
      Serial << endl
             << F("INFO: UPGRADE: Success. Firmware size: ") << upload.totalSize
             << endl
             << F("INFO: UPGRADE:  Rebooting...");
#endif
    }
#ifdef DEBUG
    else {
      Update.printError(Serial);
    }
#endif
  } else if (upload.status == UPLOAD_FILE_ABORTED) {
    Update.end();
#ifdef DEBUG
    Serial << endl << F("ERROR: UPGRADE: Update was aborted");
#endif
  }
  yield();
  return _success;
}
#endif // AFE_CONFIG_OTA_NOT_UPGRADABLE

/* Reading Server data */

void AFEWebServer::get(DEVICE &data) {

  _refreshConfiguration =
      true; // it will cause that device configuration will be refeshed

  if (server.arg("n").length() > 0) {
    server.arg("n").toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = AFE_EMPTY_STRING;
  }

  data.api.http = server.arg("h").length() > 0 ? true : false;

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  data.api.domoticz = server.arg("m").length() > 0
                          ? (server.arg("m").toInt() == 1 ? true : false)
                          : false;
  data.api.mqtt = server.arg("m").length() > 0
                      ? (server.arg("m").toInt() == 2 ? true : false)
                      : false;
  data.api.domoticzVersion = server.arg("v").length() > 0
                                 ? server.arg("v").toInt()
                                 : AFE_DOMOTICZ_VERSION_DEFAULT;
#else
  data.api.mqtt = server.arg("m").length() > 0 ? true : false;
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
  data.noOfLEDs = server.arg("l").length() > 0
                      ? server.arg("l").toInt()
                      : AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_LEDS;
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  data.noOfContactrons =
      server.arg("co").length() > 0 ? server.arg("co").toInt() : 0;
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
  data.noOfGates = server.arg("g").length() > 0 ? server.arg("g").toInt() : 0;
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

#ifdef AFE_CONFIG_HARDWARE_DHT
  data.noOfDHTs = server.arg("dh").length() > 0
                      ? server.arg("dh").toInt()
                      : AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_DHT;
#endif

#if defined(T3_CONFIG)
  for (uint8_t i = 0; i < sizeof(Device->configuration.isPIR); i++) {
    data.isPIR[i] = server.arg("p").toInt() > i ? true : false;
  }
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  data.noOfHPMA115S0s =
      server.arg("hp").length() > 0 ? server.arg("hp").toInt() : 0;
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  data.noOfBMEX80s =
      server.arg("b6").length() > 0 ? server.arg("b6").toInt() : 0;
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
  data.noOfBH1750s =
      server.arg("bh").length() > 0 ? server.arg("bh").toInt() : 0;
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
  data.noOfAS3935s =
      server.arg("a3").length() > 0 ? server.arg("a3").toInt() : 0;
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
  data.noOfAnemometerSensors =
      server.arg("w").length() > 0 ? server.arg("w").toInt() : 0;
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
  data.noOfRainmeterSensors =
      server.arg("d").length() > 0 ? server.arg("d").toInt() : 0;
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  data.noOfRegulators =
      server.arg("re").length() > 0 ? server.arg("re").toInt() : 0;
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
  data.noOfThermalProtectors =
      server.arg("tp").length() > 0 ? server.arg("tp").toInt() : 0;
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  data.isAnalogInput = server.arg("ad").length() > 0 ? true : false;
#endif

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
  data.noOfBinarySensors =
      server.arg("b").length() > 0 ? server.arg("b").toInt() : 0;
#endif

  data.timeToAutoLogOff =
      server.arg("al").length() > 0 ? AFE_AUTOLOGOFF_DEFAULT_TIME : 0;
}

void AFEWebServer::get(NETWORK &data) {

  if (server.arg("s").length() > 0) {
    server.arg("s").toCharArray(data.ssid, sizeof(data.ssid));
  } else {
    data.ssid[0] = AFE_EMPTY_STRING;
  }

  if (server.arg("sb").length() > 0) {
    server.arg("sb").toCharArray(data.ssidBackup, sizeof(data.ssidBackup));
  } else {
    data.ssidBackup[0] = AFE_EMPTY_STRING;
  }

  if (server.arg("p").length() > 0) {
    server.arg("p").toCharArray(data.password, sizeof(data.password));
  } else {
    data.password[0] = AFE_EMPTY_STRING;
  }

  if (server.arg("pb").length() > 0) {
    server.arg("pb").toCharArray(data.passwordBackup,
                                 sizeof(data.passwordBackup));
  } else {
    data.passwordBackup[0] = AFE_EMPTY_STRING;
  }

  if (server.arg("i1").length() > 0) {
    server.arg("i1").toCharArray(data.ip, sizeof(data.ip));
  } else {
    data.ip[0] = AFE_EMPTY_STRING;
  }

  if (server.arg("i2").length() > 0) {
    server.arg("i2").toCharArray(data.gateway, sizeof(data.gateway));
  } else {
    data.gateway[0] = AFE_EMPTY_STRING;
  }

  if (server.arg("i3").length() > 0) {
    server.arg("i3").toCharArray(data.subnet, sizeof(data.subnet));
  } else {
    data.subnet[0] = AFE_EMPTY_STRING;
  }

  data.noConnectionAttempts =
      server.arg("na").length() > 0
          ? server.arg("na").toInt()
          : AFE_CONFIG_NETWORK_DEFAULT_CONNECTION_ATTEMPTS;

  data.waitTimeConnections = server.arg("wc").length() > 0
                                 ? server.arg("wc").toInt()
                                 : AFE_CONFIG_NETWORK_DEFAULT_WAIT_TIME;

  data.waitTimeSeries = server.arg("ws").length() > 0
                            ? server.arg("ws").toInt()
                            : AFE_CONFIG_NETWORK_DEFAULT_WAIT_SERIES;

  data.noFailuresToSwitchNetwork =
      server.arg("fs").length() > 0
          ? server.arg("fs").toInt()
          : AFE_CONFIG_NETWORK_DEFAULT_SWITCH_NETWORK_AFTER;

  data.isDHCP = server.arg("d").length() > 0 ? true : false;
}

void AFEWebServer::get(MQTT &data) {
  if (server.arg("h").length() > 0) {
    server.arg("h").toCharArray(data.host, sizeof(data.host));
  } else {
    data.host[0] = AFE_EMPTY_STRING;
  }

  if (server.arg("a").length() > 0) {
    server.arg("a").toCharArray(data.ip, sizeof(data.ip));

  } else {
    data.ip[0] = AFE_EMPTY_STRING;
  }

  if (server.arg("p").length() > 0) {
    data.port = server.arg("p").toInt();
  }

  if (server.arg("t").length() > 0) {
    data.timeout = server.arg("t").toInt();
  }

  if (server.arg("u").length() > 0) {
    server.arg("u").toCharArray(data.user, sizeof(data.user));
  } else {
    data.user[0] = AFE_EMPTY_STRING;
  }

  if (server.arg("s").length() > 0) {
    server.arg("s").toCharArray(data.password, sizeof(data.password));
  } else {
    data.password[0] = AFE_EMPTY_STRING;
  }
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  data.lwt.idx = server.arg("x").length() > 0 ? server.arg("x").toInt() : 0;
#else
  if (server.arg("t0").length() > 0) {
    server.arg("t0").toCharArray(data.lwt.topic, sizeof(data.lwt.topic));
  } else {
    data.lwt.topic[0] = AFE_EMPTY_STRING;
  }
#endif

  data.retainLWT = server.arg("rl").length() > 0 ? true : false;
  data.retainAll = server.arg("ra").length() > 0 ? true : false;
  data.pingHostBeforeConnection = server.arg("ph").length() > 0 ? true : false;
}

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
void AFEWebServer::get(DOMOTICZ &data) {

  if (server.arg("t").length() > 0) {
    data.protocol = server.arg("t").toInt();
  }

  if (server.arg("h").length() > 0) {
    server.arg("h").toCharArray(data.host, sizeof(data.host));
  } else {
    data.host[0] = AFE_EMPTY_STRING;
  }

  if (server.arg("p").length() > 0) {
    data.port = server.arg("p").toInt();
  }

  if (server.arg("u").length() > 0) {
    server.arg("u").toCharArray(data.user, sizeof(data.user));
  } else {
    data.user[0] = AFE_EMPTY_STRING;
  }
  if (server.arg("s").length() > 0) {
    server.arg("s").toCharArray(data.password, sizeof(data.password));
  } else {
    data.password[0] = AFE_EMPTY_STRING;
  }
}
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

#ifdef AFE_CONFIG_HARDWARE_RELAY
void AFEWebServer::get(RELAY &data) {

  data.gpio = server.arg("g").length() ? server.arg("g").toInt() : 0;

#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
  data.timeToOff =
      server.arg("ot").length() > 0 ? server.arg("ot").toFloat() : 0;
#endif

  data.state.powerOn =
      server.arg("pr").length() > 0 ? server.arg("pr").toInt() : 0;

  if (server.arg("n").length() > 0) {
    server.arg("n").toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = AFE_EMPTY_STRING;
  }

  data.state.MQTTConnected =
      server.arg("mc").length() > 0 ? server.arg("mc").toInt() : 0;

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (server.arg("t").length() > 0) {
    server.arg("t").toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = AFE_EMPTY_STRING;
  }
#else
  data.domoticz.idx = server.arg("x").length() > 0 ? server.arg("x").toInt()
                                                   : AFE_DOMOTICZ_DEFAULT_IDX;
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
  data.ledID = server.arg("l").length() > 0 ? server.arg("l").toInt()
                                            : AFE_HARDWARE_ITEM_NOT_EXIST;
#endif

  data.triggerSignal = server.arg("ts").length() > 0
                           ? server.arg("ts").toInt()
                           : AFE_CONFIG_HARDWARE_RELAY_DEFAULT_SIGNAL_TRIGGER;

#ifdef AFE_CONFIG_HARDWARE_MCP23017
  data.mcp23017.gpio = server.arg("mg").length() > 0
                           ? server.arg("mg").toInt()
                           : AFE_HARDWARE_ITEM_NOT_EXIST;

  data.mcp23017.address =
      server.arg("a").length() > 0
          ? server.arg("a").toInt()
          : AFE_CONFIG_HARDWARE_I2C_DEFAULT_NON_EXIST_ADDRESS;
#endif // AFE_CONFIG_HARDWARE_MCP23017
}
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_SWITCH
void AFEWebServer::get(SWITCH &data) {
  data.type = server.arg("m").length() > 0 ? server.arg("m").toInt()
                                           : AFE_SWITCH_TYPE_MONO;

  data.sensitiveness = server.arg("s").length() > 0
                           ? server.arg("s").toInt()
                           : AFE_HARDWARE_SWITCH_DEFAULT_BOUNCING;

  data.functionality = server.arg("f").length() > 0
                           ? server.arg("f").toInt()
                           : AFE_SWITCH_FUNCTIONALITY_NONE;

  data.gpio = server.arg("g").length() > 0 ? server.arg("g").toInt() : 0;
#ifdef AFE_CONFIG_HARDWARE_RELAY
  data.relayID = server.arg("r").length() > 0 ? server.arg("r").toInt()
                                              : AFE_HARDWARE_ITEM_NOT_EXIST;
#endif
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  data.domoticz.idx =
      server.arg("x").length() > 0 ? server.arg("x").toInt() : 0;
#else
  if (server.arg("t").length() > 0) {
    server.arg("t").toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = AFE_EMPTY_STRING;
  }
#endif

#ifdef AFE_CONFIG_HARDWARE_MCP23017
  data.mcp23017.gpio = server.arg("mg").length() > 0
                           ? server.arg("mg").toInt()
                           : AFE_HARDWARE_ITEM_NOT_EXIST;

  data.mcp23017.address =
      server.arg("a").length() > 0
          ? server.arg("a").toInt()
          : AFE_CONFIG_HARDWARE_I2C_DEFAULT_NON_EXIST_ADDRESS;
#endif // AFE_CONFIG_HARDWARE_MCP23017
}
#endif // AFE_CONFIG_HARDWARE_SWITCH

void AFEWebServer::get(PASSWORD &data) {
  if (server.arg("p").length() > 0) {
    server.arg("p").toCharArray(data.password, sizeof(data.password));
  } else {
    data.password[0] = AFE_EMPTY_STRING;
  }

  data.protect = server.arg("r").length() > 0 ? true : false;
}

void AFEWebServer::get(PRO_VERSION &data) {
  if (server.arg("k").length() > 0) {
    server.arg("k").toCharArray(data.serial, sizeof(data.serial));
  } else {
    data.serial[0] = AFE_EMPTY_STRING;
  }

  data.valid = false;
}

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
void AFEWebServer::get(REGULATOR &data) {
  if (server.arg("n").length() > 0) {
    server.arg("n").toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = AFE_EMPTY_STRING;
  }

  data.enabled = server.arg("e").length() > 0 ? true : false;
  data.turnOn = server.arg("on").length() > 0 ? server.arg("on").toFloat() : 0;
  data.turnOff =
      server.arg("off").length() > 0 ? server.arg("off").toFloat() : 0;
  data.turnOnAbove =
      server.arg("ta").length() > 0 && server.arg("ta").toInt() == 1 ? true
                                                                     : false;
  data.turnOffAbove =
      server.arg("tb").length() > 0 && server.arg("tb").toInt() == 1 ? true
                                                                     : false;
  data.relayId = server.arg("r").length() > 0 ? server.arg("r").toInt()
                                              : AFE_HARDWARE_ITEM_NOT_EXIST;
  data.sensorId = server.arg("s").length() > 0 ? server.arg("s").toInt()
                                               : AFE_HARDWARE_ITEM_NOT_EXIST;
/* Hardcoded 0 for DS18B20, 1 for DHT */
#ifdef AFE_CONFIG_HARDWARE_DS18B20
  data.sensorHardware =
      server.arg("h").length() > 0 ? server.arg("h").toInt() : 0;
#endif // AFE_CONFIG_HARDWARE_DS18B20

#ifdef AFE_CONFIG_HARDWARE_DHT
  data.sensorHardware =
      server.arg("h").length() > 0 ? server.arg("h").toInt() : 1;

  data.controllingParameter = server.arg("cp").length() > 0
                                  ? server.arg("cp").toInt()
                                  : AFE_HARDWARE_ITEM_NOT_EXIST;

#endif // AFE_CONFIG_HARDWARE_DHT

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (server.arg("t").length() > 0) {
    server.arg("t").toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = AFE_EMPTY_STRING;
  }
#else
  data.domoticz.idx = server.arg("x").length() > 0 ? server.arg("x").toInt()
                                                   : AFE_DOMOTICZ_DEFAULT_IDX;
#endif
}
#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
void AFEWebServer::get(THERMAL_PROTECTOR &data) {
  if (server.arg("n").length() > 0) {
    server.arg("n").toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = AFE_EMPTY_STRING;
  }

  data.enabled = server.arg("e").length() > 0 ? true : false;
  data.temperature =
      server.arg("m").length() > 0
          ? server.arg("m").toFloat()
          : AFE_FUNCTIONALITY_THERMAL_PROTECTOR_DEFAULT_TEMPERATURE;
  data.relayId = server.arg("r").length() > 0 ? server.arg("r").toInt()
                                              : AFE_HARDWARE_ITEM_NOT_EXIST;
  data.sensorId = server.arg("s").length() > 0 ? server.arg("s").toInt()
                                               : AFE_HARDWARE_ITEM_NOT_EXIST;
  /* Hardcoded 0 for DS18B20 */
  data.sensorHardware =
      server.arg("h").length() > 0 ? server.arg("h").toInt() : 0;
#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (server.arg("t").length() > 0) {
    server.arg("t").toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = AFE_EMPTY_STRING;
  }
#else
  data.domoticz.idx = server.arg("x").length() > 0 ? server.arg("x").toInt()
                                                   : AFE_DOMOTICZ_DEFAULT_IDX;
#endif
}
#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
void AFEWebServer::get(CONTACTRON &data) {
  data.type = server.arg("y").length() > 0
                  ? server.arg("y").toInt()
                  : AFE_CONFIG_HARDWARE_CONTACTRON_DEFAULT_OUTPUT_TYPE;

  data.ledID = server.arg("l").length() > 0 ? server.arg("l").toInt()
                                            : AFE_HARDWARE_ITEM_NOT_EXIST;

  data.bouncing = server.arg("b").length() > 0
                      ? server.arg("b").toInt()
                      : AFE_CONFIG_HARDWARE_CONTACTRON_DEFAULT_BOUNCING;

  data.gpio = server.arg("g").length() > 0 ? server.arg("g").toInt() : 0;

  if (server.arg("n").length() > 0) {
    server.arg("n").toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = AFE_EMPTY_STRING;
  }

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  data.domoticz.idx = server.arg("x").length() > 0 ? server.arg("x").toInt()
                                                   : AFE_DOMOTICZ_DEFAULT_IDX;
#else
  if (server.arg("t").length() > 0) {
    server.arg("t").toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = AFE_EMPTY_STRING;
  }
#endif
}
#endif // AFE_CONFIG_HARDWARE_CONTACTRON

#ifdef AFE_CONFIG_HARDWARE_GATE
void AFEWebServer::get(GATE &data) {

  if (server.arg("n").length() > 0) {
    server.arg("n").toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = AFE_EMPTY_STRING;
  }

  data.relayId = server.arg("r").length() > 0 ? server.arg("r").toInt()
                                              : AFE_HARDWARE_ITEM_NOT_EXIST;

  data.contactron.id[0] = server.arg("c1").length() > 0
                              ? server.arg("c1").toInt()
                              : AFE_HARDWARE_ITEM_NOT_EXIST;

  data.contactron.id[1] = server.arg("c2").length() > 0
                              ? server.arg("c2").toInt()
                              : AFE_HARDWARE_ITEM_NOT_EXIST;

  for (uint8_t i = 0; i < sizeof(data.states.state); i++) {
    data.states.state[i] = server.arg("s" + String(i)).length() > 0
                               ? server.arg("s" + String(i)).toInt()
                               : AFE_GATE_UNKNOWN;
  }
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  data.domoticz.idx = server.arg("x").length() > 0 ? server.arg("x").toInt()
                                                   : AFE_DOMOTICZ_DEFAULT_IDX;
  data.domoticzControl.idx = server.arg("z").length() > 0
                                 ? server.arg("z").toInt()
                                 : AFE_DOMOTICZ_DEFAULT_IDX;
#else
  if (server.arg("t").length() > 0) {
    server.arg("t").toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = AFE_EMPTY_STRING;
  }
#endif
}
#endif // AFE_CONFIG_HARDWARE_GATE

#if defined(T3_CONFIG)
void AFEWebServer::getPIRData(uint8_t id, PIR *data) {

  if (server.arg("g" + String(id)).length() > 0) {
    data.gpio = server.arg("g" + String(id)).toInt();
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
void AFEWebServer::get(LED &data) {
  data.gpio = server.arg("g").length() > 0
                  ? server.arg("g").toInt()
                  : AFE_CONFIG_HARDWARE_LED_0_DEFAULT_GPIO;

  data.changeToOppositeValue = server.arg("w").length() > 0 ? true : false;

#ifdef AFE_CONFIG_HARDWARE_MCP23017
  data.mcp23017.gpio = server.arg("mg").length() > 0
                           ? server.arg("mg").toInt()
                           : AFE_HARDWARE_ITEM_NOT_EXIST;

  data.mcp23017.address =
      server.arg("a").length() > 0
          ? server.arg("a").toInt()
          : AFE_CONFIG_HARDWARE_I2C_DEFAULT_NON_EXIST_ADDRESS;
#endif // AFE_CONFIG_HARDWARE_MCP23017
}

uint8_t AFEWebServer::getSystemLEDData() {
  return server.arg("l").length() > 0 ? server.arg("l").toInt()
                                      : AFE_HARDWARE_ITEM_NOT_EXIST;
}
#endif // AFE_CONFIG_HARDWARE_LED

#ifdef AFE_CONFIG_HARDWARE_DS18B20
void AFEWebServer::get(DS18B20 &data) {
  AFESensorDS18B20 _Sensor;
  if (server.arg("n").length() > 0) {
    server.arg("n").toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = AFE_EMPTY_STRING;
  }

  if (server.arg("a").length() > 0) {
    char address[17];
    server.arg("a").toCharArray(address, 17);
    _Sensor.addressToInt(address, data.address);
  } else {
    _Sensor.addressNULL(data.address);
  }
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

  data.sendOnlyChanges = server.arg("s").length() > 0 ? true : false;

  data.resolution = server.arg("r").length() > 0
                        ? server.arg("r").toInt()
                        : AFE_CONFIG_HARDWARE_DS18B20_DEFAULT_RESOLUTION;

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  data.domoticz.idx = server.arg("x").length() > 0 ? server.arg("x").toInt()
                                                   : AFE_DOMOTICZ_DEFAULT_IDX;
#else
  if (server.arg("t").length() > 0) {
    server.arg("t").toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = AFE_EMPTY_STRING;
  }
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
}

#endif // AFE_CONFIG_HARDWARE_DS18B20

#ifdef AFE_CONFIG_HARDWARE_DHT
void AFEWebServer::get(DHT &data) {

  if (server.arg("n").length() > 0) {
    server.arg("n").toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = AFE_EMPTY_STRING;
  }

  data.gpio = server.arg("g").length() > 0
                  ? server.arg("g").toInt()
                  : AFE_CONFIG_HARDWARE_DHT_DEFAULT_GPIO;

  data.type = server.arg("t").length() > 0 ? server.arg("t").toInt()
                                           : AFE_HARDWARE_ITEM_NOT_EXIST;

  data.interval = server.arg("f").length() > 0
                      ? server.arg("f").toInt()
                      : AFE_CONFIG_HARDWARE_DHT_DEFAULT_INTERVAL;

  data.temperature.correction =
      server.arg("tc").length() > 0
          ? server.arg("tc").toFloat()
          : AFE_CONFIG_HARDWARE_DHT_DEFAULT_TEMPERATURE_CORRECTION;

  data.temperature.unit = server.arg("tu").length() > 0
                              ? server.arg("tu").toInt()
                              : AFE_TEMPERATURE_UNIT_CELSIUS;

  data.humidity.correction =
      server.arg("hc").length() > 0
          ? server.arg("hc").toFloat()
          : AFE_CONFIG_HARDWARE_DHT_DEFAULT_HUMIDITY_CORRECTION;

  data.humidity.unit = server.arg("hu").length() > 0 ? server.arg("hu").toInt()
                                                     : AFE_HUMIDITY_UNIT;

  data.sendOnlyChanges = server.arg("s").length() > 0 ? true : false;

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  data.domoticz.temperature.idx = server.arg("i1").length() > 0
                                      ? server.arg("i1").toInt()
                                      : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.humidity.idx = server.arg("i2").length() > 0
                                   ? server.arg("i2").toInt()
                                   : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.dewPoint.idx = server.arg("i3").length() > 0
                                   ? server.arg("i3").toInt()
                                   : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.heatIndex.idx = server.arg("i4").length() > 0
                                    ? server.arg("i4").toInt()
                                    : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.temperatureHumidity.idx = server.arg("i5").length() > 0
                                              ? server.arg("i5").toInt()
                                              : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.perception.idx = server.arg("i6").length() > 0
                                     ? server.arg("i6").toInt()
                                     : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.comfort.idx = server.arg("i7").length() > 0
                                  ? server.arg("i7").toInt()
                                  : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.absoluteHumidity.idx = server.arg("i8").length() > 0
                                           ? server.arg("i8").toInt()
                                           : AFE_DOMOTICZ_DEFAULT_IDX;

#else
  if (server.arg("m").length() > 0) {
    server.arg("m").toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = AFE_EMPTY_STRING;
  }
#endif
}
#endif // AFE_CONFIG_HARDWARE_DHT

#ifdef AFE_CONFIG_HARDWARE_UART
void AFEWebServer::getSerialPortData(SERIALPORT *data) {

  data->RXD = server.arg("r").length() > 0
                  ? server.arg("r").toInt()
                  : AFE_CONFIG_HARDWARE_UART_DEFAULT_RXD;
  data->TXD = server.arg("t").length() > 0
                  ? server.arg("t").toInt()
                  : AFE_CONFIG_HARDWARE_UART_DEFAULT_TXD;
}
#endif // AFE_CONFIG_HARDWARE_UART

#ifdef AFE_CONFIG_HARDWARE_I2C
void AFEWebServer::get(I2CPORT &data) {
  data.SDA = server.arg("a").length() > 0 ? server.arg("a").toInt()
                                          : AFE_CONFIG_HARDWARE_I2C_DEFAULT_SDA;
  data.SCL = server.arg("l").length() > 0 ? server.arg("l").toInt()
                                          : AFE_CONFIG_HARDWARE_I2C_DEFAULT_SCL;
}
#endif // AFE_CONFIG_HARDWARE_I2C

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
void AFEWebServer::get(HPMA115S0 &data) {
  data.interval = server.arg("f").length() > 0
                      ? server.arg("f").toInt()
                      : AFE_CONFIG_HARDWARE_HPMA115S_DEFAULT_INTERVAL;

  data.timeToMeasure =
      server.arg("m").length() > 0
          ? server.arg("m").toInt()
          : AFE_CONFIG_HARDWARE_HPMA115S_DEFAULT_TIME_TO_MEASURE;

  data.whoPM10Norm = server.arg("n1").length() > 0
                         ? server.arg("n1").toFloat()
                         : AFE_CONFIG_HARDWARE_HPMA115S_DEFAULT_WHO_NORM_PM10;

  data.whoPM25Norm = server.arg("n2").length() > 0
                         ? server.arg("n2").toFloat()
                         : AFE_CONFIG_HARDWARE_HPMA115S_DEFAULT_WHO_NORM_PM25;

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  data.domoticz.pm25.idx = server.arg("x2").length() > 0
                               ? server.arg("x2").toInt()
                               : AFE_DOMOTICZ_DEFAULT_IDX;
  data.domoticz.pm10.idx = server.arg("x1").length() > 0
                               ? server.arg("x1").toInt()
                               : AFE_DOMOTICZ_DEFAULT_IDX;
  data.domoticz.whoPM10Norm.idx = server.arg("x3").length() > 0
                                      ? server.arg("x3").toInt()
                                      : AFE_DOMOTICZ_DEFAULT_IDX;
  data.domoticz.whoPM25Norm.idx = server.arg("x4").length() > 0
                                      ? server.arg("x4").toInt()
                                      : AFE_DOMOTICZ_DEFAULT_IDX;
#else
  if (server.arg("t").length() > 0) {
    server.arg("t").toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = AFE_EMPTY_STRING;
  }
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

  if (server.arg("n").length() > 0) {
    server.arg("n").toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = AFE_EMPTY_STRING;
  }
};
#endif // AFE_CONFIG_HARDWARE_HPMA115S0

#ifdef AFE_CONFIG_HARDWARE_BMEX80
void AFEWebServer::get(BMEX80 &data) {
  data.type = server.arg("b").length() > 0 ? server.arg("b").toInt()
                                           : AFE_BMX_UNKNOWN_SENSOR;

  data.i2cAddress = server.arg("a").length() > 0 ? server.arg("a").toInt() : 0;

  data.interval = server.arg("f").length() > 0
                      ? server.arg("f").toInt()
                      : AFE_CONFIG_HARDWARE_BMEX80_DEFAULT_INTERVAL;

  data.resolution = server.arg("r").length() > 0 ? server.arg("r").toInt()
                                                 : BMP085_ULTRAHIGHRES;

  data.seaLevelPressure = server.arg("s").length() > 0
                              ? server.arg("s").toInt()
                              : AFE_CONFIG_DEFAULT_SEA_LEVEL_PRESSURE;

  data.altitude = server.arg("hi").length() > 0 ? server.arg("hi").toInt() : 0;

  data.temperature.unit = server.arg("tu").length() > 0
                              ? server.arg("tu").toInt()
                              : AFE_TEMPERATURE_UNIT_CELSIUS;

  data.temperature.correction =
      server.arg("tc").length() > 0 ? server.arg("tc").toFloat() : 0;

  data.humidity.correction =
      server.arg("hc").length() > 0 ? server.arg("hc").toFloat() : 0;

  data.pressure.unit = server.arg("pu").length() > 0 ? server.arg("pu").toInt()
                                                     : AFE_PRESSURE_UNIT_HPA;
  data.pressure.correction =
      server.arg("pc").length() > 0 ? server.arg("pc").toFloat() : 0;

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  data.domoticz.temperatureHumidityPressure.idx =
      server.arg("i0").length() > 0 ? server.arg("i0").toInt()
                                    : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.temperature.idx = server.arg("i1").length() > 0
                                      ? server.arg("i1").toInt()
                                      : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.humidity.idx = server.arg("i2").length() > 0
                                   ? server.arg("i2").toInt()
                                   : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.dewPoint.idx = server.arg("i3").length() > 0
                                   ? server.arg("i3").toInt()
                                   : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.heatIndex.idx = server.arg("i4").length() > 0
                                    ? server.arg("i4").toInt()
                                    : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.pressure.idx = server.arg("i5").length() > 0
                                   ? server.arg("i5").toInt()
                                   : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.relativePressure.idx = server.arg("i6").length() > 0
                                           ? server.arg("i6").toInt()
                                           : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.iaq.idx = server.arg("i7").length() > 0
                              ? server.arg("i7").toInt()
                              : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.staticIaq.idx = server.arg("i8").length() > 0
                                    ? server.arg("i8").toInt()
                                    : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.co2Equivalent.idx = server.arg("i9").length() > 0
                                        ? server.arg("i9").toInt()
                                        : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.breathVocEquivalent.idx = server.arg("i10").length() > 0
                                              ? server.arg("i10").toInt()
                                              : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.gasResistance.idx = server.arg("i11").length() > 0
                                        ? server.arg("i11").toInt()
                                        : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.temperatureHumidity.idx = server.arg("i12").length() > 0
                                              ? server.arg("i12").toInt()
                                              : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.perception.idx = server.arg("i13").length() > 0
                                     ? server.arg("i13").toInt()
                                     : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.comfort.idx = server.arg("i14").length() > 0
                                  ? server.arg("i14").toInt()
                                  : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.absoluteHumidity.idx = server.arg("i15").length() > 0
                                           ? server.arg("i15").toInt()
                                           : AFE_DOMOTICZ_DEFAULT_IDX;
#else
  if (server.arg("t").length() > 0) {
    server.arg("t").toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = AFE_EMPTY_STRING;
  }
#endif

  if (server.arg("n").length() > 0) {
    server.arg("n").toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = AFE_EMPTY_STRING;
  }
}
#endif // AFE_CONFIG_HARDWARE_BMEX80

#ifdef AFE_CONFIG_HARDWARE_BH1750
void AFEWebServer::get(BH1750 &data) {
  data.i2cAddress = server.arg("a").length() > 0 ? server.arg("a").toInt() : 0;

  data.interval = server.arg("f").length() > 0
                      ? server.arg("f").toInt()
                      : AFE_CONFIG_HARDWARE_BH1750_DEFAULT_INTERVAL;

  data.mode = server.arg("m").length() > 0
                  ? server.arg("m").toInt()
                  : AFE_CONFIG_HARDWARE_BH1750_DEFAULT_MODE;
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  data.domoticz.idx = server.arg("d").length() > 0 ? server.arg("d").toInt()
                                                   : AFE_DOMOTICZ_DEFAULT_IDX;
#else
  if (server.arg("t").length() > 0) {
    server.arg("t").toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = AFE_EMPTY_STRING;
  }
#endif

  if (server.arg("n").length() > 0) {
    server.arg("n").toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = AFE_EMPTY_STRING;
  }
}
#endif // AFE_CONFIG_HARDWARE_BH1750

#ifdef AFE_CONFIG_HARDWARE_AS3935
void AFEWebServer::get(AS3935 &data) {

  if (server.arg("n").length() > 0) {
    server.arg("n").toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = AFE_EMPTY_STRING;
  }

  data.i2cAddress = server.arg("a").length() > 0
                        ? server.arg("a").toInt()
                        : AFE_CONFIG_HARDWARE_I2C_DEFAULT_NON_EXIST_ADDRESS;

  data.irqGPIO = server.arg("g").length() > 0 ? server.arg("g").toInt() : 0;

  data.setNoiseFloorAutomatically = server.arg("f").length() > 0 ? true : false;

  data.noiseFloor = server.arg("nf").length() > 0
                        ? server.arg("nf").toInt()
                        : AFE_CONFIG_HARDWARE_AS3935_DEFAULT_NOISE_FLOOR;

  data.minimumNumberOfLightningSpikes =
      server.arg("m").length() > 0
          ? server.arg("m").toInt()
          : AFE_CONFIG_HARDWARE_AS3935_DEFAULT_MINIMUM_NO_OF_SPIKES;

  data.watchdogThreshold =
      server.arg("e").length() > 0
          ? server.arg("e").toInt()
          : AFE_CONFIG_HARDWARE_AS3935_DEFAULT_WATCHDOG_THRESHOLD;

  data.spikesRejectionLevel =
      server.arg("s").length() > 0
          ? server.arg("s").toInt()
          : AFE_CONFIG_HARDWARE_AS3935_DEFAULT_SPIKES_REJECTION_LEVEL;

  data.indoor = server.arg("w").length() > 0 && server.arg("w").toInt() == 1
                    ? true
                    : false;

  data.unit =
      server.arg("u").length() > 0 ? server.arg("u").toInt() : AFE_DISTANCE_KM;
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  data.domoticz.idx =
      server.arg("d").length() > 0 ? server.arg("d").toInt() : 0;
#else
  if (server.arg("t").length() > 0) {
    server.arg("t").toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = AFE_EMPTY_STRING;
  }
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
}
#endif // AFE_CONFIG_HARDWARE_AS3935

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
void AFEWebServer::get(ANEMOMETER &data) {
  if (server.arg("n").length() > 0) {
    server.arg("n").toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = AFE_EMPTY_STRING;
  }

  data.gpio = server.arg("g").length() > 0
                  ? server.arg("g").toInt()
                  : AFE_HARDWARE_ANEMOMETER_DEFAULT_GPIO;

  data.interval = server.arg("f").length() > 0
                      ? server.arg("f").toInt()
                      : AFE_HARDWARE_ANEMOMETER_DEFAULT_INTERVAL;

  data.sensitiveness = server.arg("s").length() > 0
                           ? server.arg("s").toInt()
                           : AFE_HARDWARE_ANEMOMETER_DEFAULT_BOUNCING;

  data.impulseDistance = server.arg("l").length() > 0
                             ? server.arg("l").toFloat()
                             : AFE_HARDWARE_ANEMOMETER_DEFAULT_IMPULSE_DISTANCE;

  data.impulseDistanceUnit =
      server.arg("u").length() > 0
          ? server.arg("u").toInt()
          : AFE_HARDWARE_ANEMOMETER_DEFAULT_IMPULSE_DISTANCE_UNIT;

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  data.domoticz.idx =
      server.arg("x").length() > 0 ? server.arg("x").toInt() : 0;
#else
  if (server.arg("t").length() > 0) {
    server.arg("t").toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = AFE_EMPTY_STRING;
  }
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
}
#endif // AFE_CONFIG_HARDWARE_ANEMOMETER

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
void AFEWebServer::get(ADCINPUT &data) {
  data.gpio = server.arg("g").length() > 0
                  ? server.arg("g").toInt()
                  : AFE_CONFIG_HARDWARE_ADC_VCC_DEFAULT_GPIO;

  data.interval = server.arg("v").length() > 0
                      ? server.arg("v").toInt()
                      : AFE_CONFIG_HARDWARE_ADC_VCC_DEFAULT_INTERVAL;

  data.numberOfSamples =
      server.arg("n").length() > 0
          ? server.arg("n").toInt()
          : AFE_CONFIG_HARDWARE_ADC_VCC_DEFAULT_NUMBER_OF_SAMPLES;

  data.maxVCC = server.arg("m").length() > 0
                    ? server.arg("m").toFloat()
                    : AFE_CONFIG_HARDWARE_ADC_VCC_DEFAULT_MAX_VCC;

  data.divider.Ra =
      server.arg("ra").length() > 0 ? server.arg("ra").toFloat() : 0;

  data.divider.Rb =
      server.arg("rb").length() > 0 ? server.arg("rb").toFloat() : 0;

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (server.arg("t").length() > 0) {
    server.arg("t").toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = AFE_EMPTY_STRING;
  }
#else
  data.domoticz.raw = server.arg("x0").length() > 0 ? server.arg("x0").toInt()
                                                    : AFE_DOMOTICZ_DEFAULT_IDX;
  data.domoticz.percent = server.arg("x1").length() > 0
                              ? server.arg("x1").toInt()
                              : AFE_DOMOTICZ_DEFAULT_IDX;
  data.domoticz.voltage = server.arg("x2").length() > 0
                              ? server.arg("x2").toInt()
                              : AFE_DOMOTICZ_DEFAULT_IDX;
  data.domoticz.voltageCalculated = server.arg("x3").length() > 0
                                        ? server.arg("x3").toInt()
                                        : AFE_DOMOTICZ_DEFAULT_IDX;
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
  data.battery.minVoltage =
      server.arg("lv").length() > 0
          ? server.arg("lv").toFloat()
          : AFE_CONFIG_HARDWARE_ADC_VCC_DEFAULT_BATTER_MIN_V;

  data.battery.maxVoltage =
      server.arg("hv").length() > 0
          ? server.arg("hv").toFloat()
          : AFE_CONFIG_HARDWARE_ADC_VCC_DEFAULT_BATTER_MAX_V;

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (server.arg("bt").length() > 0) {
    server.arg("bt").toCharArray(data.battery.mqtt.topic,
                                 sizeof(data.battery.mqtt.topic));
  } else {
    data.battery.mqtt.topic[0] = AFE_EMPTY_STRING;
  }
#else
  data.battery.domoticz.idx = server.arg("bx").length() > 0
                                  ? server.arg("bx").toInt()
                                  : AFE_DOMOTICZ_DEFAULT_IDX;
#endif

#endif // AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
}
#endif // AFE_CONFIG_HARDWARE_ADC_VCC

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
void AFEWebServer::get(RAINMETER &data) {

  if (server.arg("n").length() > 0) {
    server.arg("n").toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = AFE_EMPTY_STRING;
  }

  data.gpio = server.arg("g").length() > 0
                  ? server.arg("g").toInt()
                  : AFE_HARDWARE_RAINMETER_DEFAULT_GPIO;

  data.interval = server.arg("f").length() > 0
                      ? server.arg("f").toInt()
                      : AFE_HARDWARE_RAINMETER_DEFAULT_INTERVAL;

  data.sensitiveness = server.arg("s").length() > 0
                           ? server.arg("s").toInt()
                           : AFE_HARDWARE_RAINMETER_DEFAULT_BOUNCING;

  data.resolution = server.arg("r").length() > 0
                        ? server.arg("r").toFloat()
                        : (float)AFE_HARDWARE_RAINMETER_DEFAULT_RESOLUTION;

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  data.domoticz.idx =
      server.arg("x").length() > 0 ? server.arg("x").toInt() : 0;
#else
  if (server.arg("t").length() > 0) {
    server.arg("t").toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = AFE_EMPTY_STRING;
  }
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
}
#endif // AFE_CONFIG_HARDWARE_RAINMETER

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
void AFEWebServer::get(BINARY_SENSOR &data) {
  if (server.arg("n").length() > 0) {
    server.arg("n").toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = AFE_EMPTY_STRING;
  }

  data.bouncing = server.arg("b").length() > 0
                      ? server.arg("b").toInt()
                      : AFE_HARDWARE_BINARY_SENSOR_DEFAULT_BOUNCING;

  data.revertSignal = server.arg("rs").length() > 0 ? true : false;
  data.internalPullUp = server.arg("pr").length() > 0 ? true : false;
  data.sendAsSwitch = server.arg("ss").length() > 0 ? true : false;

  data.gpio = server.arg("g").length() > 0 ? server.arg("g").toInt() : 0;

#ifdef AFE_CONFIG_HARDWARE_MCP23017
  data.mcp23017.gpio = server.arg("mg").length() > 0
                           ? server.arg("mg").toInt()
                           : AFE_HARDWARE_ITEM_NOT_EXIST;

  data.mcp23017.address =
      server.arg("a").length() > 0
          ? server.arg("a").toInt()
          : AFE_CONFIG_HARDWARE_I2C_DEFAULT_NON_EXIST_ADDRESS;
#endif // AFE_CONFIG_HARDWARE_MCP23017

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  data.domoticz.idx =
      server.arg("x").length() > 0 ? server.arg("x").toInt() : 0;
#else
  if (server.arg("t").length() > 0) {
    server.arg("t").toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = AFE_EMPTY_STRING;
  }
#endif
}
#endif // AFE_CONFIG_HARDWARE_BINARY_SENSOR

#ifndef AFE_CONFIG_OTA_NOT_UPGRADABLE
uint16_t AFEWebServer::getOTAFirmwareId() {
  return server.arg("f").length() > 0 ? server.arg("f").toInt() : 0;
}
#endif // AFE_CONFIG_OTA_NOT_UPGRADABLE

#ifdef AFE_CONFIG_HARDWARE_PN532
void AFEWebServer::get(PN532_SENSOR &data) {
  if (server.arg("n").length() > 0) {
    server.arg("n").toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = AFE_EMPTY_STRING;
  }

  data.tx = server.arg("tx").length() > 0 ? server.arg("tx").toInt()
                                          : AFE_HARDWARE_ITEM_NOT_EXIST;

  data.rx = server.arg("rx").length() > 0 ? server.arg("rx").toInt()
                                          : AFE_HARDWARE_ITEM_NOT_EXIST;

  data.requestProcessingTime = server.arg("f").length() > 0
                                   ? server.arg("f").toInt()
                                   : AFE_HARDWARE_ITEM_NOT_EXIST;

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  data.domoticz.idx =
      server.arg("x").length() > 0 ? server.arg("x").toInt() : 0;
#else
  if (server.arg("t").length() > 0) {
    server.arg("t").toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = AFE_EMPTY_STRING;
  }
#endif
}

void AFEWebServer::processMiFareCard() {
  AFESensorPN532 PN532Sensor;
  PN532Sensor.begin(0, Data);

  char tag[AFE_HARDWARE_PN532_BLOCK_SIZE];
  char label[3];

  for (uint8_t i = 0;
       i < AFE_HARDWARE_PN532_NUMBER_OF_WRITABLE_BLOCKS_PER_SECTOR; i++) {
    sprintf(label, "t%d", i);
    if (server.arg(label).length() > 0) {
      server.arg(label).toCharArray(tag, AFE_HARDWARE_PN532_BLOCK_SIZE);
    } else {
      tag[0] = AFE_EMPTY_STRING;
    }

#ifdef DEBUG
    Serial << endl
           << "INFO: PN532: Writing to block: "
           << AFE_HARDWARE_PN532_FIRST_TAG_FIRST_BLOCK + i << " : " << tag;
#endif
    PN532Sensor.writeBlock(AFE_HARDWARE_PN532_FIRST_TAG_FIRST_BLOCK + i, tag);
    
  }

  for (uint8_t i = 0;
       i < AFE_HARDWARE_PN532_NUMBER_OF_WRITABLE_BLOCKS_PER_SECTOR; i++) {
    sprintf(label, "t%d", i + 4);
    if (server.arg(label).length() > 0) {
      server.arg(label).toCharArray(tag, AFE_HARDWARE_PN532_BLOCK_SIZE);
    } else {
      tag[0] = AFE_EMPTY_STRING;
    }

#ifdef DEBUG
    Serial << endl
           << "INFO: PN532: Writing to block: "
           << AFE_HARDWARE_PN532_FIRST_TAG_SECOND_BLOCK + i << " : " << tag;
#endif
    PN532Sensor.writeBlock(AFE_HARDWARE_PN532_FIRST_TAG_SECOND_BLOCK + i, tag);
  }
}
#endif
