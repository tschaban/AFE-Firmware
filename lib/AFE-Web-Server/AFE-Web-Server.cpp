/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Web-Server.h"

AFEWebServer::AFEWebServer() {}

void AFEWebServer::begin(AFEDevice *_Device, AFEFirmwarePro *_Firmware) {
  // httpUpdater.setup(&server);
  server.begin();
  Device = _Device;
  Firmware = _Firmware;
  Site.begin(_Device, _Firmware);
}

#if AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
  void AFEWebServer::initSystemLED(AFELED *_SystemLED) {
    SystemLED = _SystemLED;
  }
#endif

String AFEWebServer::generateSite(AFE_SITE_PARAMETERS *siteConfig) {
  String page;

  if (siteConfig->twoColumns) {
    page = Site.generateTwoColumnsLayout();
  } else {
    page = Site.generateOneColumnLayout(siteConfig->rebootTime);
  }

  if (siteConfig->form) {
    page += "<form  method=\"post\" action=\"/?c=";
    page += AFE_SERVER_CMD_SAVE;
    page += "&o=";
    page += siteConfig->ID;
    if (siteConfig->deviceID >= 0) {
      page += "&i=";
      page += siteConfig->deviceID;
    }
    page += "\">";
  }

  switch (siteConfig->ID) {
  case AFE_CONFIG_SITE_INDEX:
    page += Site.addIndexSection(siteConfig->deviceID == -1 ? true : false);
    break;
  case AFE_CONFIG_SITE_FIRST_TIME:
    page += Site.addNetworkConfiguration();
    break;
  case AFE_CONFIG_SITE_FIRST_TIME_CONNECTING:
    page += Site.addConnectingSite();
    break;
  case AFE_CONFIG_SITE_DEVICE:
    page += Site.addDeviceConfiguration();
    break;
  case AFE_CONFIG_SITE_NETWORK:
    page += Site.addNetworkConfiguration();
    break;
  case AFE_CONFIG_SITE_MQTT:
    page += Site.addMQTTBrokerConfiguration();
    break;
  case AFE_CONFIG_SITE_DOMOTICZ:
    page += Site.addDomoticzServerConfiguration();
    break;
  case AFE_CONFIG_SITE_PASSWORD:
    page += Site.addPasswordConfigurationSite();
    break;
  case AFE_CONFIG_SITE_PRO_VERSION:
    page += Site.addProVersionSite();
    break;
  case AFE_CONFIG_SITE_EXIT:
    page += Site.addExitSection(siteConfig->rebootMode);
    break;
  case AFE_CONFIG_SITE_RESET:
    page += Site.addResetSection();
    break;
  case AFE_CONFIG_SITE_POST_RESET:
    page += Site.addPostResetSection();
    break;
  case AFE_CONFIG_SITE_UPGRADE:
    page += Site.addUpgradeSection();
    break;
  case AFE_CONFIG_SITE_POST_UPGRADE:
    page += Site.addPostUpgradeSection(upgradeFailed);
    break;
  case AFE_CONFIG_SITE_RELAY:
    page += Site.addRelayConfiguration(siteConfig->deviceID);
    break;
  case AFE_CONFIG_SITE_SWITCH:
    page += Site.addSwitchConfiguration(siteConfig->deviceID);
    break;
#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  case AFE_CONFIG_SITE_ANALOG_INPUT:
    page += Site.addAnalogInputConfiguration();
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  case AFE_CONFIG_SITE_CONTACTRON:
    page += Site.addContactronConfiguration(siteConfig->deviceID);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_GATE
  case AFE_CONFIG_SITE_GATE:
    page += Site.addGateConfiguration(siteConfig->deviceID);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_UART
  case AFE_CONFIG_SITE_UART:
    page += Site.addSerialPortConfiguration();
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_I2C
  case AFE_CONFIG_SITE_I2C:
    page += Site.addI2CPortConfiguration();
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_BMEX80
  case AFE_CONFIG_SITE_BMEX80:
    page += Site.addBMEX80Configuration(siteConfig->deviceID);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  case AFE_CONFIG_SITE_HPMA115S0:
    page += Site.addHPMA115S0Configuration(siteConfig->deviceID);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_BH1750
  case AFE_CONFIG_SITE_BH1750:
    page += Site.addBH1750Configuration(siteConfig->deviceID);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_AS3935
  case AFE_CONFIG_SITE_AS3935:
    page += Site.addAS3935Configuration(siteConfig->deviceID);
    break;
#endif
#if AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
  case AFE_CONFIG_SITE_LED:
    for (uint8_t i = 0; i < Device->configuration.noOfLEDs; i++) {
      page += Site.addLEDConfiguration(i);
    }
    page += Site.addSystemLEDConfiguration();
    break;
#endif
  }

  if (siteConfig->form) {
    if (siteConfig->formButton) {
      page += "<input type=\"submit\" class=\"b bs\" value=\"";
      page += L_SAVE;
      page += "\">";
    }
    page += "</form>";
  }

  page += Site.generateFooter((Device->getMode() == AFE_MODE_NORMAL ||
                               Device->getMode() == AFE_MODE_CONFIGURATION)
                                  ? true
                                  : false);

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

  if (command == AFE_SERVER_CMD_SAVE) {
    switch (siteConfig.ID) {
    case AFE_CONFIG_SITE_FIRST_TIME:
      Data.saveConfiguration(getNetworkData());
      siteConfig.twoColumns = false;
      siteConfig.reboot = true;
      siteConfig.rebootMode = AFE_MODE_CONFIGURATION;
      siteConfig.form = false;
      siteConfig.ID = AFE_CONFIG_SITE_FIRST_TIME_CONNECTING;
      break;
    case AFE_CONFIG_SITE_DEVICE:
      DEVICE configuration;
      configuration = getDeviceData();
      Data.saveConfiguration(&configuration);
      configuration = {0};
      break;
    case AFE_CONFIG_SITE_NETWORK:
      Data.saveConfiguration(getNetworkData());
      break;
    case AFE_CONFIG_SITE_MQTT:
      Data.saveConfiguration(getMQTTData());
      break;
    case AFE_CONFIG_SITE_DOMOTICZ:
      Data.saveConfiguration(getDomoticzServerData());
      break;
    case AFE_CONFIG_SITE_PASSWORD:
      Data.saveConfiguration(getPasswordData());
      break;
#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
    case AFE_CONFIG_SITE_ANALOG_INPUT:
      Data.saveConfiguration(getAnalogInputData());
      break;
#endif
#if AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
    case AFE_CONFIG_SITE_LED:
      for (uint8_t i = 0; i < Device->configuration.noOfLEDs; i++) {
        Data.saveConfiguration(i, getLEDData(i));
      }
      Data.saveSystemLedID(getSystemLEDData());
      break;
#endif
    case AFE_CONFIG_SITE_RELAY:
      Data.saveConfiguration(siteConfig.deviceID,
                             getRelayData(siteConfig.deviceID));
      break;
    case AFE_CONFIG_SITE_SWITCH:
      Data.saveConfiguration(siteConfig.deviceID,
                             getSwitchData(siteConfig.deviceID));
      break;
    case AFE_CONFIG_SITE_RESET:
      siteConfig.ID = AFE_CONFIG_SITE_POST_RESET;
      siteConfig.reboot = true;
      siteConfig.rebootMode = AFE_MODE_FIRST_TIME_LAUNCH;
      siteConfig.rebootTime = 15;
      siteConfig.form = false;
      siteConfig.twoColumns = false;
      break;
    case AFE_CONFIG_SITE_PRO_VERSION:
      Data.saveConfiguration(getSerialNumberData());
      Firmware->begin();
      Firmware->callService(AFE_WEBSERVICE_ADD_KEY);
      break;
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
    case AFE_CONFIG_SITE_CONTACTRON:
      Data.saveConfiguration(siteConfig.deviceID,
                             getContactronData(siteConfig.deviceID));
      break;
#endif
#ifdef AFE_CONFIG_HARDWARE_GATE
    case AFE_CONFIG_SITE_GATE:
      Data.saveConfiguration(siteConfig.deviceID, getGateData());
      break;
#endif
#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
    case AFE_CONFIG_SITE_HPMA115S0:
      Data.saveConfiguration(siteConfig.deviceID, getHPMA115S0SensorData());
      break;
#endif
#ifdef AFE_CONFIG_HARDWARE_BMEX80
    case AFE_CONFIG_SITE_BMEX80:
      Data.saveConfiguration(siteConfig.deviceID, getBMEX80SensorData());
      break;
#endif
#ifdef AFE_CONFIG_HARDWARE_BH1750
    case AFE_CONFIG_SITE_BH1750:
      Data.saveConfiguration(siteConfig.deviceID, getBH1750SensorData());
      break;
#endif
#ifdef AFE_CONFIG_HARDWARE_AS3935
    case AFE_CONFIG_SITE_AS3935:
      Data.saveConfiguration(siteConfig.deviceID,getAS3935SensorData());
      break;
#endif
#ifdef AFE_CONFIG_HARDWARE_UART
    case AFE_CONFIG_SITE_UART:
      Data.saveConfiguration(getSerialPortData());
      break;
#endif
#ifdef AFE_CONFIG_HARDWARE_I2C
    case AFE_CONFIG_SITE_I2C:
      Data.saveConfiguration(getI2CPortData());
      break;
#endif
    }
  } else if (command == AFE_SERVER_CMD_NONE) {
    switch (siteConfig.ID) {
    case AFE_CONFIG_SITE_INDEX:
      siteConfig.form = false;
      siteConfig.twoColumns = false;
      if (siteConfig.deviceID > AFE_MODE_NORMAL) {
        boolean authorize = true;
        PASSWORD accessControl = Data.getPasswordConfiguration();
        if (accessControl.protect) {
          PASSWORD data = getPasswordData();
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
      break;
    case AFE_CONFIG_SITE_EXIT:
      siteConfig.reboot = true;
      siteConfig.rebootMode = AFE_MODE_NORMAL;
      siteConfig.rebootTime = 10;
      siteConfig.form = false;
      siteConfig.twoColumns = false;
      break;
    case AFE_CONFIG_SITE_FIRST_TIME:
      siteConfig.twoColumns = false;
      break;
    case AFE_CONFIG_SITE_RESET:
      siteConfig.formButton = false;
      break;
    case AFE_CONFIG_SITE_UPGRADE:
      siteConfig.form = false;
      break;
    case AFE_CONFIG_SITE_POST_UPGRADE:
      if (!upload) {
        siteConfig.form = false;
        siteConfig.twoColumns = false;
        siteConfig.rebootTime = 15;
        siteConfig.reboot = true;
        siteConfig.rebootMode = Device->getMode();
      }
      break;
    }
  }

  if (upload) {
    HTTPUpload &upload = server.upload();
    String _updaterError;
    if (upload.status == UPLOAD_FILE_START) {
      WiFiUDP::stopAll();

#ifdef DEBUG
      Serial << endl
             << endl
             << "---------------- Firmware upgrade -----------------";
      Serial << endl << "Update: " << upload.filename.c_str();
#endif

      uint32_t maxSketchSpace =
          (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;

#ifdef DEBUG
      Serial << endl << "Sketch size: " << ESP.getSketchSize();
      Serial << endl << "Free sketch size: " << ESP.getFreeSketchSpace();
      Serial << endl << "Max sketch space: " << maxSketchSpace;
#endif

      if (!Update.begin(maxSketchSpace)) { // start with max available size
#ifdef DEBUG
        Update.printError(Serial);
#endif
        upgradeFailed = true;
      }
    } else if (upload.status == UPLOAD_FILE_WRITE && !_updaterError.length()) {
      SystemLED->toggle();
#ifdef DEBUG
      Serial << ".";
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
               << "Update Success. Firmware size: " << upload.totalSize << endl
               << "Rebooting...";
        Serial << endl << "---------------------------------------------------";
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
      Serial << endl << "Update was aborted";
      Serial << endl << "---------------------------------------------------";
#endif
    }
    delay(0);
  } else {

#ifdef DEBUG
    Serial << endl
           << endl
           << "---------------- Site Generated WebSite -----------------";
    Serial << endl << "Site ID: " << siteConfig.ID;
    Serial << endl
           << "Site Type: "
           << (siteConfig.twoColumns ? "Two Columns" : "One Column");
    Serial << endl << "Device ID: " << siteConfig.deviceID;
    Serial << endl << "Command: " << command;
    Serial << endl << "Reboot: " << (siteConfig.reboot ? "Yes" : "No");
    if (siteConfig.reboot) {
      Serial << endl << " - Mode: " << siteConfig.rebootMode;
      Serial << endl << " - Time: " << siteConfig.rebootTime;
    }

    Serial << endl
           << "---------------------------------------------------------"
           << endl;

#endif

    publishHTML(generateSite(&siteConfig));
  }
  /* Post page generation actions */

  /* Reseting device */
  //  if (siteConfig.ID == AFE_CONFIG_SITE_POST_RESET) {
  //    Device->setDevice();
  //  }

  /* Rebooting device */
  if (siteConfig.reboot) {
    #if AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
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
        Data.saveConfiguration(getRegulatorData(optionName == "thermostat" ?
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

void AFEWebServer::listener() { server.handleClient(); }

boolean AFEWebServer::httpAPIlistener() { return receivedHTTPCommand; }

void AFEWebServer::publishHTML(String page) {

#ifdef DEBUG
  Serial << endl << endl << "Site streaming started";
  Serial << endl << " - Page size: " << page.length();
#endif

  server.send(200, "text/html", page);
#ifdef DEBUG
  Serial << endl << " - Completed";
#endif
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

DEVICE AFEWebServer::getDeviceData() {
  DEVICE data;
  _refreshConfiguration =
      true; // it will cause that device configuration will be refeshed

  if (server.arg("n").length() > 0) {
    server.arg("n").toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = '\0';
  }

  data.api.http = server.arg("h").length() > 0 ? true : false;
  data.api.mqtt = server.arg("m").length() > 0 ? true : false;
  data.api.domoticz = server.arg("d").length() > 0 ? true : false;

#if AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
  data.noOfLEDs = server.arg("l").length() > 0 ? server.arg("l").toInt() : 0;
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  data.noOfContactrons =
      server.arg("co").length() > 0 ? server.arg("co").toInt() : 0;
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
  data.noOfGates = server.arg("g").length() > 0 ? server.arg("g").toInt() : 0;
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
  data.noOfRelays = server.arg("r").length() > 0 ? server.arg("r").toInt() : 0;
#endif

  data.noOfSwitches =
      server.arg("s").length() > 0 ? server.arg("s").toInt() : 0;

#ifdef AFE_CONFIG_HARDWARE_DS18B20
  data.isDS18B20 = server.arg("ds").length() > 0 ? true : false;
#endif

#ifdef AFE_CONFIG_HARDWARE_DHXX
  data.isDHT = server.arg("dh").length() > 0 ? true : false;
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


#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  data.isAnalogInput = server.arg("ad").length() > 0 ? true : false;
#endif

  return data;
}

NETWORK AFEWebServer::getNetworkData() {
  NETWORK data;
  if (server.arg("s").length() > 0) {
    server.arg("s").toCharArray(data.ssid, sizeof(data.ssid));
  } else {
    data.ssid[0] = '\0';
  }

  if (server.arg("p").length() > 0) {
    server.arg("p").toCharArray(data.password, sizeof(data.password));
  } else {
    data.password[0] = '\0';
  }

  if (server.arg("i1").length() > 0) {
    server.arg("i1").toCharArray(data.ip, sizeof(data.ip));
  } else {
    data.ip[0] = '\0';
  }

  if (server.arg("i2").length() > 0) {
    server.arg("i2").toCharArray(data.gateway, sizeof(data.gateway));
  } else {
    data.gateway[0] = '\0';
  }

  if (server.arg("i3").length() > 0) {
    server.arg("i3").toCharArray(data.subnet, sizeof(data.subnet));
  } else {
    data.subnet[0] = '\0';
  }

  if (server.arg("na").length() > 0) {
    data.noConnectionAttempts = server.arg("na").toInt();
  }

  if (server.arg("wc").length() > 0) {
    data.waitTimeConnections = server.arg("wc").toInt();
  }

  if (server.arg("ws").length() > 0) {
    data.waitTimeSeries = server.arg("ws").toInt();
  }

  if (server.arg("d").length() > 0 ||
      (server.arg("d").length() == 0 && server.arg("ws").length() == 0)) {
    data.isDHCP = true;
  } else {
    data.isDHCP = false;
  }

  return data;
}

MQTT AFEWebServer::getMQTTData() {
  MQTT data;
  if (server.arg("h").length() > 0) {
    server.arg("h").toCharArray(data.host, sizeof(data.host));
  } else {
    data.host[0] = '\0';
  }

  if (server.arg("i").length() > 0) {
    server.arg("i").toCharArray(data.ip, sizeof(data.ip));
  } else {
    data.ip[0] = '\0';
  }

  if (server.arg("p").length() > 0) {
    data.port = server.arg("p").toInt();
  }

  if (server.arg("u").length() > 0) {
    server.arg("u").toCharArray(data.user, sizeof(data.user));
  } else {
    data.user[0] = '\0';
  }

  if (server.arg("s").length() > 0) {
    server.arg("s").toCharArray(data.password, sizeof(data.password));
  } else {
    data.password[0] = '\0';
  }

  if (server.arg("t0").length() > 0) {
    server.arg("t0").toCharArray(data.lwt.topic, sizeof(data.lwt.topic));
  } else {
    data.lwt.topic[0] = '\0';
  }

  return data;
}

DOMOTICZ AFEWebServer::getDomoticzServerData() {
  DOMOTICZ data;

  if (server.arg("t").length() > 0) {
    data.protocol = server.arg("t").toInt();
  }

  if (server.arg("h").length() > 0) {
    server.arg("h").toCharArray(data.host, sizeof(data.host));
  } else {
    data.host[0] = '\0';
  }

  if (server.arg("p").length() > 0) {
    data.port = server.arg("p").toInt();
  }

  if (server.arg("u").length() > 0) {
    server.arg("u").toCharArray(data.user, sizeof(data.user));
  } else {
    data.user[0] = '\0';
  }
  if (server.arg("s").length() > 0) {
    server.arg("s").toCharArray(data.password, sizeof(data.password));
  } else {
    data.password[0] = '\0';
  }

  return data;
}

RELAY AFEWebServer::getRelayData(uint8_t id) {
  RELAY data;

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
    data.name[0] = '\0';
  }

  if (server.arg("t").length() > 0) {
    server.arg("t").toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = '\0';
  }

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTION
  data.thermalProtection = server.arg("tp" + String(id)).length() > 0
                               ? server.arg("tp" + String(id)).toInt()
                               : 0;
#endif

  data.state.MQTTConnected =
      server.arg("mc").length() > 0 ? server.arg("mc").toInt() : 0;

  data.domoticz.idx =
      server.arg("x").length() > 0 ? server.arg("x").toInt() : 0;

  data.ledID = server.arg("l").length() > 0 ? server.arg("l").toInt()
                                            : AFE_HARDWARE_ITEM_NOT_EXIST;

  return data;
}

SWITCH AFEWebServer::getSwitchData(uint8_t id) {
  SWITCH data;

  data.type = server.arg("m").length() > 0 ? server.arg("m").toInt()
                                           : AFE_SWITCH_TYPE_MONO;

  data.sensitiveness = server.arg("s").length() > 0 ? server.arg("s").toInt()
                                                    : AFE_SWITCH_BOUNCING;

  data.functionality = server.arg("f").length() > 0
                           ? server.arg("f").toInt()
                           : AFE_SWITCH_FUNCTIONALITY_NONE;

  data.gpio = server.arg("g").length() > 0 ? server.arg("g").toInt() : 0;

  data.relayID = server.arg("r").length() > 0 ? server.arg("r").toInt()
                                              : AFE_HARDWARE_ITEM_NOT_EXIST;

  data.domoticz.idx =
      server.arg("x").length() > 0 ? server.arg("x").toInt() : 0;

  if (server.arg("t").length() > 0) {
    server.arg("t").toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = '\0';
  }

  return data;
}

PASSWORD AFEWebServer::getPasswordData() {
  PASSWORD data;

  if (server.arg("p").length() > 0) {
    server.arg("p").toCharArray(data.password, sizeof(data.password));
  } else {
    data.password[0] = '\0';
  }

  data.protect = server.arg("r").length() > 0 ? true : false;

  return data;
}

PRO_VERSION AFEWebServer::getSerialNumberData() {
  PRO_VERSION data;

  if (server.arg("k").length() > 0) {
    server.arg("k").toCharArray(data.serial, sizeof(data.serial));
  }

  if (server.arg("v").length() > 0) {
    data.valid = server.arg("v").toInt() == 0 ? false : true;
  }

  return data;
}

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
REGULATOR AFEWebServer::getRegulatorData() {
  REGULATOR data;
  server.arg("te").length() > 0 ? data.enabled = true : data.enabled = false;

  if (server.arg("tn").length() > 0) {
    data.turnOn = server.arg("tn").toFloat();
  }

  if (server.arg("tf").length() > 0) {
    data.turnOff = server.arg("tf").toFloat();
  }

  if (server.arg("ta").length() > 0) {
    data.turnOnAbove = server.arg("ta").toInt() == 0 ? false : true;
  }

  if (server.arg("tb").length() > 0) {
    data.turnOffAbove = server.arg("tb").toInt() == 0 ? false : true;
  }

  return data;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
CONTACTRON AFEWebServer::getContactronData(uint8_t id) {
  CONTACTRON data;

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
    data.name[0] = '\0';
  }

  data.domoticz.idx =
      server.arg("x").length() > 0 ? server.arg("x").toInt() : 0;

  if (server.arg("t").length() > 0) {
    server.arg("t").toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = '\0';
  }

  return data;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
GATE AFEWebServer::getGateData() {
  GATE data;

  if (server.arg("n").length() > 0) {
    server.arg("n").toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = '\0';
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

  data.domoticz.idx =
      server.arg("x").length() > 0 ? server.arg("x").toInt() : 0;

  if (server.arg("t").length() > 0) {
    server.arg("t").toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = '\0';
  }

  return data;
}
#endif

#if defined(T3_CONFIG)
PIR AFEWebServer::getPIRData(uint8_t id) {
  PIR data;

  if (server.arg("g" + String(id)).length() > 0) {
    data.gpio = server.arg("g" + String(id)).toInt();
  }

  if (server.arg("n" + String(id)).length() > 0) {
    server.arg("n" + String(id)).toCharArray(data.name, sizeof(data.name));
  }

  if (server.arg("l" + String(id)).length() > 0) {
    data.ledId = server.arg("l" + String(id)).toInt();
  }

  if (server.arg("r" + String(id)).length() > 0) {
    data.relayId = server.arg("r" + String(id)).toInt();
  }

  if (server.arg("d" + String(id)).length() > 0) {
    data.howLongKeepRelayOn = server.arg("d" + String(id)).toInt();
  }

  server.arg("i" + String(id)).length() > 0 ? data.invertRelayState = true
                                            : data.invertRelayState = false;

  if (server.arg("o" + String(id)).length() > 0) {
    data.type = server.arg("o" + String(id)).toInt();
  }

  if (server.arg("x" + String(id)).length() > 0) {
    data.idx = server.arg("x" + String(id)).toInt();
  }

  return data;
}
#endif

#if AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
LED AFEWebServer::getLEDData(uint8_t id) {
  LED data;
  data.gpio = server.arg("g" + String(id)).length() > 0
                  ? server.arg("g" + String(id)).toInt()
                  : 0;

  data.changeToOppositeValue =
      server.arg("o" + String(id)).length() > 0 ? true : false;

  return data;
}

uint8_t AFEWebServer::getSystemLEDData() {
  return server.arg("s").length() > 0 ? server.arg("s").toInt()
                                      : AFE_HARDWARE_ITEM_NOT_EXIST;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
DS18B20 AFEWebServer::getDS18B20Data() {
  DS18B20 data;

  if (server.arg("g").length() > 0) {
    data.gpio = server.arg("g").toInt();
  }

  if (server.arg("c").length() > 0) {
    data.correction = server.arg("c").toFloat();
  }

  if (server.arg("i").length() > 0) {
    data.interval = server.arg("i").toInt();
  }

  if (server.arg("u").length() > 0) {
    data.unit = server.arg("u").toInt();
  }

  server.arg("o").length() > 0 ? data.sendOnlyChanges = true
                               : data.sendOnlyChanges = false;

  if (server.arg("x").length() > 0) {
    data.idx = server.arg("x").toInt();
  }

  return data;
}

#endif

#ifdef AFE_CONFIG_HARDWARE_DHXX
DH AFEWebServer::getDHTData() {
  DH data;

  if (server.arg("g").length() > 0) {
    data.gpio = server.arg("g").toInt();
  }

  if (server.arg("t").length() > 0) {
    data.type = server.arg("t").toInt();
  }

  if (server.arg("c").length() > 0) {
    data.temperature.correction = server.arg("c").toFloat();
  }

  if (server.arg("i").length() > 0) {
    data.interval = server.arg("i").toInt();
  }

  if (server.arg("u").length() > 0) {
    data.temperature.unit = server.arg("u").toInt();
  }

  if (server.arg("d").length() > 0) {
    data.humidity.correction = server.arg("d").toFloat();
  }

  server.arg("j").length() > 0 ? data.publishDewPoint = true
                               : data.publishDewPoint = false;

  server.arg("o").length() > 0 ? data.sendOnlyChanges = true
                               : data.sendOnlyChanges = false;

  server.arg("p").length() > 0 ? data.publishHeatIndex = true
                               : data.publishHeatIndex = false;

  if (server.arg("xt").length() > 0) {
    data.temperatureIdx = server.arg("xt").toInt();
  }

  if (server.arg("xh").length() > 0) {
    data.humidityIdx = server.arg("xh").toInt();
  }

  if (server.arg("xth").length() > 0) {
    data.temperatureAndHumidityIdx = server.arg("xth").toInt();
  }

  return data;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_UART
SERIALPORT AFEWebServer::getSerialPortData() {
  SERIALPORT data;

  data.RXD = server.arg("r").length() > 0
                 ? server.arg("r").toInt()
                 : AFE_CONFIG_HARDWARE_UART_DEFAULT_RXD;
  data.TXD = server.arg("t").length() > 0
                 ? server.arg("t").toInt()
                 : AFE_CONFIG_HARDWARE_UART_DEFAULT_TXD;
  return data;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_I2C
I2CPORT AFEWebServer::getI2CPortData() {
  I2CPORT data;

  data.SDA = server.arg("a").length() > 0
                 ? server.arg("a").toInt()
                 : AFE_CONFIG_HARDWARE_I2C_DEFAULT_SDA;
  data.SCL = server.arg("l").length() > 0
                 ? server.arg("l").toInt()
                 : AFE_CONFIG_HARDWARE_I2C_DEFAULT_SCL;
  return data;
}
#endif


#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
HPMA115S0 AFEWebServer::getHPMA115S0SensorData() {
  HPMA115S0 data;
  data.interval = server.arg("f").length() > 0
                      ? server.arg("f").toInt()
                      : AFE_CONFIG_HARDWARE_HPMA115S_DEFAULT_INTERVAL;

  data.timeToMeasure =
      server.arg("m").length() > 0
          ? server.arg("m").toInt()
          : AFE_CONFIG_HARDWARE_HPMA115S_DEFAULT_TIME_TO_MEASURE;

  data.domoticz.pm25.idx =
      server.arg("x2").length() > 0 ? server.arg("x2").toInt() : 0;
  data.domoticz.pm10.idx =
      server.arg("x1").length() > 0 ? server.arg("x1").toInt() : 0;

  if (server.arg("t").length() > 0) {
    server.arg("t").toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = '\0';
  }

  if (server.arg("n").length() > 0) {
    server.arg("n").toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = '\0';
  }

  return data;
};
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
BMEX80 AFEWebServer::getBMEX80SensorData() {
  BMEX80 data;
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

  data.domoticz.temperatureHumidityPressure.idx =
      server.arg("i0").length() > 0 ? server.arg("i0").toInt() : 0;

  data.domoticz.temperature.idx =
      server.arg("i1").length() > 0 ? server.arg("i1").toInt() : 0;

  data.domoticz.humidity.idx =
      server.arg("i2").length() > 0 ? server.arg("i2").toInt() : 0;

  data.domoticz.dewPoint.idx =
      server.arg("i3").length() > 0 ? server.arg("i3").toInt() : 0;

  data.domoticz.heatIndex.idx =
      server.arg("i4").length() > 0 ? server.arg("i4").toInt() : 0;

  data.domoticz.pressure.idx =
      server.arg("i5").length() > 0 ? server.arg("i5").toInt() : 0;

  data.domoticz.relativePressure.idx =
      server.arg("i6").length() > 0 ? server.arg("i6").toInt() : 0;

  data.domoticz.iaq.idx =
      server.arg("i7").length() > 0 ? server.arg("i7").toInt() : 0;

  data.domoticz.staticIaq.idx =
      server.arg("i8").length() > 0 ? server.arg("i8").toInt() : 0;

  data.domoticz.co2Equivalent.idx =
      server.arg("i9").length() > 0 ? server.arg("i9").toInt() : 0;

  data.domoticz.breathVocEquivalent.idx =
      server.arg("i10").length() > 0 ? server.arg("i10").toInt() : 0;

  data.domoticz.gasResistance.idx =
      server.arg("i11").length() > 0 ? server.arg("i11").toInt() : 0;

  data.domoticz.temperatureHumidity.idx =
      server.arg("i12").length() > 0 ? server.arg("i12").toInt() : 0;

  if (server.arg("t").length() > 0) {
    server.arg("t").toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = '\0';
  }

  if (server.arg("n").length() > 0) {
    server.arg("n").toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = '\0';
  }

  return data;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
BH1750 AFEWebServer::getBH1750SensorData() {
  BH1750 data;
  data.i2cAddress = server.arg("a").length() > 0 ? server.arg("a").toInt() : 0;

  data.interval = server.arg("f").length() > 0
                      ? server.arg("f").toInt()
                      : AFE_CONFIG_HARDWARE_BH1750_DEFAULT_INTERVAL;

  data.mode = server.arg("m").length() > 0
                  ? server.arg("m").toInt()
                  : AFE_CONFIG_HARDWARE_BH1750_DEFAULT_MODE;
  data.domoticz.idx =
      server.arg("d").length() > 0 ? server.arg("d").toInt() : 0;

  if (server.arg("t").length() > 0) {
    server.arg("t").toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = '\0';
  }

  if (server.arg("n").length() > 0) {
    server.arg("n").toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = '\0';
  }

  return data;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
AS3935 AFEWebServer::getAS3935SensorData() {
  AS3935 data;

    if (server.arg("n").length() > 0) {
    server.arg("n").toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = '\0';
  }

  data.i2cAddress = server.arg("a").length() > 0 ? server.arg("a").toInt() : 0;

  data.irqGPIO = server.arg("g").length() > 0 ? server.arg("g").toInt() : 0;

  data.setNoiseFloorAutomatically = server.arg("f").length() > 0 ? true : false;

  data.noiseFloor = server.arg("nf").length() > 0 ? server.arg("nf").toInt() : AFE_CONFIG_HARDWARE_AS3935_DEFAULT_NOISE_FLOOR;

  data.minimumNumberOfLightningSpikes = server.arg("m").length() > 0 ? server.arg("m").toInt() : AFE_CONFIG_HARDWARE_AS3935_DEFAULT_MINIMUM_NO_OF_SPIKES;

  data.watchdogThreshold = server.arg("e").length() > 0 ? server.arg("e").toInt() : AFE_CONFIG_HARDWARE_AS3935_DEFAULT_WATCHDOG_THRESHOLD;

  data.spikesRejectionLevel = server.arg("s").length() > 0 ? server.arg("s").toInt() : AFE_CONFIG_HARDWARE_AS3935_DEFAULT_SPIKES_REJECTION_LEVEL;

  data.indoor = server.arg("w").length() > 0 && server.arg("w").toInt() == 1
                    ? true
                    : false;

  data.unit =
      server.arg("u").length() > 0 ? server.arg("u").toInt() : AFE_DISTANCE_KM;

  data.domoticz.idx =
      server.arg("d").length() > 0 ? server.arg("d").toInt() : 0;

  if (server.arg("t").length() > 0) {
    server.arg("t").toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = '\0';
  }

  return data;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
ADCINPUT AFEWebServer::getAnalogInputData() {
  ADCINPUT data;

  data.gpio = server.arg("g").length() > 0
                  ? server.arg("g").toInt()
                  : AFE_CONFIG_HARDWARE_ADC_VCC_DEFAULT_GPIO;

  data.interval = server.arg("i").length() > 0
                      ? server.arg("i").toInt()
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

  if (server.arg("t").length() > 0) {
    server.arg("t").toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = '\0';
  }

  data.domoticz.raw =
      server.arg("x0").length() > 0 ? server.arg("x0").toInt() : 0;
  data.domoticz.percent =
      server.arg("x1").length() > 0 ? server.arg("x1").toInt() : 0;
  data.domoticz.voltage =
      server.arg("x2").length() > 0 ? server.arg("x2").toInt() : 0;
  data.domoticz.voltageCalculated =
      server.arg("x3").length() > 0 ? server.arg("x3").toInt() : 0;

  return data;
}
#endif
