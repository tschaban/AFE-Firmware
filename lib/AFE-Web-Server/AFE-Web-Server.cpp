/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#include "AFE-Web-Server.h"

AFEWebServer::AFEWebServer() {}

void AFEWebServer::begin(AFEDevice *_Device, AFEFirmware *_Firmware) {
  // httpUpdater.setup(&server);
  server.begin();
  Device = _Device;
  Firmware = _Firmware;
  Site.begin(_Device, _Firmware);
}

String AFEWebServer::generateSite(AFE_SITE_PARAMETERS *siteConfig) {
  String page;

  if (siteConfig->twoColumns) {
    page = Site.generateTwoColumnsLayout();
  } else {
    page = Site.generateOneColumnLayout(siteConfig->rebootTime);
  }

  if (siteConfig->form) {
    page += "<form  method=\"post\" action=\"/?c=";
    page += SERVER_CMD_SAVE;
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
#ifdef CONFIG_HARDWARE_ADC_VCC
  case AFE_CONFIG_SITE_ANALOG_INPUT:
    page += Site.addAnalogInputConfiguration();
    break;
#endif
#ifdef CONFIG_HARDWARE_CONTACTRON
  case AFE_CONFIG_SITE_CONTACTRON:
    page += Site.addContactronConfiguration(siteConfig->deviceID);
    break;
#endif
#ifdef CONFIG_HARDWARE_GATE
  case AFE_CONFIG_SITE_GATE:
    page += Site.addGateConfiguration();
    break;
#endif
#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
  case AFE_CONFIG_SITE_LED:
    for (uint8_t i = 0; i < CONFIG_HARDWARE_NUMBER_OF_LEDS; i++) {
      if (Device->configuration.isLED[i]) {
        page += Site.addLEDConfiguration(i);
      } else {
        break;
      }
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

  page += Site.generateFooter((Device->getMode() == MODE_NORMAL ||
                               Device->getMode() == MODE_CONFIGURATION)
                                  ? true
                                  : false);

  return page;
}

/*
String AFEWebServer::getFirstLaunchConfigurationSite() {
  String page;
  page = Site.generateOneColumnLayout();
  page += "<form action=\"/?option=0&cmd=1\" method=\"post\">";
  page += Site.addFirstLaunchConfiguration();
  page += "<input type=\"submit\" class=\"b bs\" value=\"";
  page += L_CONNECT;
  page += "\"></form>";
  page += Site.generateFooter();
  return page;
}


String AFEWebServer::getConnectingSite() {
  String page;
  page = Site.generateOneColumnLayout();
  page += Site.addConnectingSite();
  page += Site.generateFooter();
  return page;
}

String AFEWebServer::getIndexSite(boolean authorized) {
  String page;
  //  page.reserve(siteBufferSize);
  page = Site.generateOneColumnLayout(0);

  page += Site.addIndexSection(authorized);
  page += "";

#ifdef DEBUG
  Serial << endl << "Index page launch in the mode: " << Device->getMode();
#endif

  page += Site.generateFooter();
  return page;
}

String AFEWebServer::getSite(uint8_t option, uint8_t command) {

  String page;

    if (option == "upgrade") {
      page += Site.generateTwoColumnsLayout(0);
      page += Site.addUpgradeSection();
    } else if (option == "reset") {
      page += Site.generateOneColumnLayout(10);
      page += Site.addResetSection(command);
    } else if (option == "exit") {
      page += Site.generateOneColumnLayout(10);
      page += Site.addExitSection(command);
    } else {
      page += "<h1>Page Not Found</h1>";
    }

    page +=
        Site.generateFooter((option == "index" && command == 0) ? true : false);
    delay(10);
    return page;
}
*/

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

  if (command == SERVER_CMD_SAVE) {
    switch (siteConfig.ID) {
    case AFE_CONFIG_SITE_FIRST_TIME:
      Data.saveConfiguration(getNetworkData());
      siteConfig.twoColumns = false;
      siteConfig.reboot = true;
      siteConfig.rebootMode = MODE_CONFIGURATION;
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
#ifdef CONFIG_HARDWARE_ADC_VCC
    case AFE_CONFIG_SITE_ANALOG_INPUT:
      Data.saveConfiguration(getAnalogInputData());
      break;
#endif
#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
    case AFE_CONFIG_SITE_LED:
      for (uint8_t i = 0; i < CONFIG_HARDWARE_NUMBER_OF_LEDS; i++) {
        if (Device->configuration.isLED[i]) {
          Data.saveConfiguration(i, getLEDData(i));
        }
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
      siteConfig.rebootMode = MODE_FIRST_TIME_LAUNCH;
      siteConfig.rebootTime = 15;
      siteConfig.form = false;
      siteConfig.twoColumns = false;
      break;
    case AFE_CONFIG_SITE_PRO_VERSION:
      Data.saveConfiguration(getSerialNumberData());
      Firmware->begin();
      Firmware->callService(AFE_WEBSERVICE_ADD_KEY);
      break;
#ifdef CONFIG_HARDWARE_CONTACTRON
    case AFE_CONFIG_SITE_CONTACTRON:
      Data.saveConfiguration(siteConfig.deviceID,
                             getContactronData(siteConfig.deviceID));
      break;

#endif
#ifdef CONFIG_HARDWARE_GATE
    case AFE_CONFIG_SITE_GATE:
      Data.saveConfiguration(getGateData());
      break;
#endif
    }
  } else if (command == SERVER_CMD_NONE) {
    switch (siteConfig.ID) {
    case AFE_CONFIG_SITE_INDEX:
      siteConfig.form = false;
      siteConfig.twoColumns = false;
      if (siteConfig.deviceID > MODE_NORMAL) {
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
      siteConfig.rebootMode = MODE_NORMAL;
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
      Serial << endl << "Max sketch space: " << maxSketchSpace << endl;
#endif

      if (!Update.begin(maxSketchSpace)) { // start with max available size
#ifdef DEBUG
        Update.printError(Serial);
#endif
        upgradeFailed = true;
      }
    } else if (upload.status == UPLOAD_FILE_WRITE && !_updaterError.length()) {
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
    Device->reboot(siteConfig.rebootMode);
  }

  /*
  #ifdef CONFIG_HARDWARE_UART
  }
  else if (optionName == "UART") {
    SERIALPORT data;
    if (command == SERVER_CMD_SAVE) {
      data = getSerialPortData();
    }
    publishHTML(ConfigurationPanel.getSerialPortConfigurationSite(command,
  data)); #endif #ifdef CONFIG_HARDWARE_HPMA115S0
  }
  else if (optionName == "HPMA115S0") {
    HPMA115S0 data;
    if (command == SERVER_CMD_SAVE) {
      data = getHPMA115S0SensorData();
    }
    publishHTML(
        ConfigurationPanel.getHPMA115S0SensorConfigurationSite(command,
  data)); #endif #ifdef CONFIG_HARDWARE_BMX80
  }
  else if (optionName == "BMx80") {
    BMx80 data;
    if (command == SERVER_CMD_SAVE) {
      data = getBMx80SensorData();
    }
    publishHTML(
        ConfigurationPanel.getBMx80SensorConfigurationSite(command, data));
  #endif
  #ifdef CONFIG_HARDWARE_BH1750
  }
  else if (optionName == "BH1750") {
    BH1750 data;
    if (command == SERVER_CMD_SAVE) {
      data = getBH1750SensorData();
    }
    publishHTML(
        ConfigurationPanel.getBH1750SensorConfigurationSite(command, data));
  #endif
  }


  else if (optionName == "index") {
    PASSWORD data;
    boolean authorize = true;
    if (command != MODE_NORMAL) {
      AFEDataAccess Data;
      PASSWORD accessControl = Data.getPasswordConfiguration();
      if (accessControl.protect) {
        data = getPasswordData();
        if (strcmp(accessControl.password, data.password) != 0) {
          authorize = false;
        }
      }

      if (authorize) {
        if (command == 1) {
          publishHTML(ConfigurationPanel.getSite("exit", MODE_CONFIGURATION));
          server.client().stop();
          Device->reboot(MODE_CONFIGURATION);
        } else {
          publishHTML(ConfigurationPanel.getSite("exit", MODE_ACCESS_POINT));
          server.client().stop();
          Device->reboot(MODE_ACCESS_POINT);
        }
      }
    }
    publishHTML(ConfigurationPanel.getIndexSite(authorize));
    AFEFirmware Firmware;
    Firmware.begin();
    Firmware.callService(AFE_WEBSERVICE_VALIDATE_KEY);

  #ifdef CONFIG_HARDWARE_DS18B20
  }
  else if (optionName == "ds18b20") {
    DS18B20 data = {};
    if (command == SERVER_CMD_SAVE) {
      data = getDS18B20Data();
    }
    publishHTML(ConfigurationPanel.getDS18B20ConfigurationSite(command,
  data)); #endif #ifdef CONFIG_HARDWARE_DHXX
  }
  else if (optionName == "DHT") {
    DH data = {};
    if (command == SERVER_CMD_SAVE) {
      data = getDHTData();
    }
    publishHTML(ConfigurationPanel.getDHTConfigurationSite(command, data));
  #endif

  #ifdef CONFIG_FUNCTIONALITY_REGULATOR
  }
  else if (optionName == "thermostat" || optionName == "humidistat") {
    if (command == SERVER_CMD_SAVE) {
        Data.saveConfiguration(getRegulatorData(optionName == "thermostat" ?
  THERMOSTAT_REGULATOR : HUMIDISTAT_REGULATOR);
    }
    publishHTML(ConfigurationPanel.getRelayStatConfigurationSite());
  #endif
  }
  else if (optionName == "start") {
    NETWORK data;

    if (command == SERVER_CMD_SAVE) {
      data = getNetworkData();
      AFEDataAccess Data;
      Data.saveConfiguration(data);
      publishHTML(ConfigurationPanel.getConnectingSite());
      Device->reboot(MODE_CONFIGURATION);
    } else {
      publishHTML(
          ConfigurationPanel.getFirstLaunchConfigurationSite(command, data));
    }
  }
  else {
    for (uint8_t i = 0; i < CONFIG_HARDWARE_NUMBER_OF_RELAYS; i++) {
      if (Device->configuration.isRelay[i]) {
        if (optionName == "relay" + String(i)) {
          if (command == SERVER_CMD_SAVE) {
            Data.saveConfiguration(i, getRelayData(i));
          }
          publishHTML(ConfigurationPanel.getRelayConfigurationSite(i));
        }
      } else {
        break;
      }
    }

    for (uint8_t i = 0; i < CONFIG_HARDWARE_NUMBER_OF_SWITCHES; i++) {
      if (Device->configuration.isSwitch[i]) {
        if (optionName == "switch" + String(i)) {
          SWITCH data = {};
          if (command == SERVER_CMD_SAVE) {
            Data.saveConfiguration(i, getSwitchData(i));
          }
          publishHTML(ConfigurationPanel.getSwitchConfigurationSite(i));
        }
      } else {
        break;
      }
    }

  #if defined(T3_CONFIG)
    for (uint8_t i = 0; i < 4; i++) {
      if (Device->configuration.isPIR[i]) {
        if (getOptionName() == "pir" + String(i)) {
          PIR data = {};
          if (getCommand() == SERVER_CMD_SAVE) {
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
          if (command == SERVER_CMD_SAVE) {
            data = getContactronData(i);
          }
          publishHTML(ConfigurationPanel.getContactronConfigurationSite(
              optionName, command, data, i));
        }
      } else {
        break;
      }
    }

    if (optionName == "gate") {
      GATE data = {};
      if (command == SERVER_CMD_SAVE) {
        data = getGateData();
      }
      publishHTML(
          ConfigurationPanel.getGateConfigurationSite(optionName, command,
  data));
    }
  #endif
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

  if (Device->getMode() == MODE_NETWORK_NOT_SET) {
    return AFE_CONFIG_SITE_FIRST_TIME;
  } else if (Device->getMode() == MODE_NORMAL) {
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
    return SERVER_CMD_NONE;
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
  uint16_t pageSize = page.length();
  uint16_t size = 1024;
  server.setContentLength(pageSize);
  if (pageSize > size) {
    server.send(200, "text/html", page.substring(0, size));
    uint16_t transfered = size;
    uint16_t nextChunk;
    while (transfered < pageSize) {
      nextChunk = transfered + size < pageSize ? transfered + size : pageSize;
      server.sendContent(page.substring(transfered, nextChunk));
      transfered = nextChunk;
    }
  } else {
    server.send(200, "text/html", page);
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

DEVICE AFEWebServer::getDeviceData() {
  DEVICE data;
  _refreshConfiguration =
      true; // it will cause that device configuration will be refeshed

  if (server.arg("dn").length() > 0) {
    server.arg("dn").toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = '\0';
  }

  server.arg("h").length() > 0 ? data.api.http = true : data.api.http = false;

  server.arg("m").length() > 0 ? data.api.mqtt = true : data.api.mqtt = false;

  server.arg("d").length() > 0 ? data.api.domoticz = true
                               : data.api.domoticz = false;
#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
  for (uint8_t i = 0; i < CONFIG_HARDWARE_NUMBER_OF_LEDS; i++) {
    server.arg("hl").toInt() > i ? data.isLED[i] = true : data.isLED[i] = false;
  }
#endif

#if defined(T5_CONFIG)
  for (uint8_t i = 0; i < sizeof(Device->configuration.isContactron); i++) {
    server.arg("hc").toInt() > i ? data.isContactron[i] = true
                                 : data.isContactron[i] = false;
  }
#else
  for (uint8_t i = 0; i < CONFIG_HARDWARE_NUMBER_OF_RELAYS; i++) {
    server.arg("hr").toInt() > i ? data.isRelay[i] = true
                                 : data.isRelay[i] = false;
  }
#endif

  for (uint8_t i = 0; i < CONFIG_HARDWARE_NUMBER_OF_SWITCHES; i++) {
    server.arg("hs").toInt() > i ? data.isSwitch[i] = true
                                 : data.isSwitch[i] = false;
  }
#ifdef CONFIG_HARDWARE_DS18B20
  server.arg("ds").length() > 0 ? data.isDS18B20 = true
                                : data.isDS18B20 = false;
#endif

#ifdef CONFIG_HARDWARE_DHXX
  server.arg("ds").length() > 0 ? data.isDHT = true : data.isDHT = false;
#endif

#if defined(T3_CONFIG)
  for (uint8_t i = 0; i < sizeof(Device->configuration.isPIR); i++) {
    server.arg("hp").toInt() > i ? data.isPIR[i] = true : data.isPIR[i] = false;
  }
#endif

#ifdef CONFIG_HARDWARE_HPMA115S0
  server.arg("ds").length() > 0 ? data.isHPMA115S0 = true
                                : data.isHPMA115S0 = false;
#endif

#ifdef CONFIG_HARDWARE_BMX80
  if (server.arg("b6").length() > 0) {
    data.isBMx80 = server.arg("b6").toInt();
  }
#endif

#ifdef CONFIG_HARDWARE_BH1750
  server.arg("bh").length() > 0 ? data.isBH1750 = true : data.isBH1750 = false;
#endif

#ifdef CONFIG_HARDWARE_ADC_VCC
  server.arg("ai").length() > 0 ? data.isAnalogInput = true
                                : data.isAnalogInput = false;
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

  if (server.arg("g" + String(id)).length() > 0) {
    data.gpio = server.arg("g" + String(id)).toInt();
  }

#ifdef CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
  if (server.arg("ot" + String(id)).length() > 0) {
    data.timeToOff = server.arg("ot" + String(id)).toFloat();
  }
#endif

#ifdef CONFIG_FUNCTIONALITY_RELAY

  if (server.arg("pr" + String(id)).length() > 0) {
    data.state.powerOn = server.arg("pr" + String(id)).toInt();
  }

  if (server.arg("n" + String(id)).length() > 0) {
    server.arg("n" + String(id)).toCharArray(data.name, sizeof(data.name));
  }

  if (server.arg("t" + String(id)).length() > 0) {
    server.arg("t" + String(id))
        .toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = '\0';
  }

  data.state.MQTTConnected = server.arg("mc" + String(id)).length() > 0
                                 ? server.arg("mc" + String(id)).toInt()
                                 : 0;

#ifdef CONFIG_FUNCTIONALITY_THERMAL_PROTECTION
  if (server.arg("tp" + String(id)).length() > 0) {
    data.thermalProtection = server.arg("tp" + String(id)).toInt();
  }
#endif

  data.domoticz.idx = server.arg("x" + String(id)).length() > 0
                          ? server.arg("x" + String(id)).toInt()
                          : 0;

#endif

  if (server.arg("l" + String(id)).length() > 0) {
    data.ledID = server.arg("l" + String(id)).toInt();
  }

  return data;
}

SWITCH AFEWebServer::getSwitchData(uint8_t id) {
  SWITCH data;

  if (server.arg("m" + String(id)).length() > 0) {
    data.type = server.arg("m" + String(id)).toInt();
  }

  if (server.arg("s" + String(id)).length() > 0) {
    data.sensitiveness = server.arg("s" + String(id)).toInt();
  }

  if (server.arg("f" + String(id)).length() > 0) {
    data.functionality = server.arg("f" + String(id)).toInt();
  }

  if (server.arg("g" + String(id)).length() > 0) {
    data.gpio = server.arg("g" + String(id)).toInt();
  }

  if (server.arg("r" + String(id)).length() > 0) {
    data.relayID = server.arg("r" + String(id)).toInt();
  }

  data.domoticz.idx = server.arg("x" + String(id)).length() > 0
                          ? server.arg("x" + String(id)).toInt()
                          : 0;

  if (server.arg("t" + String(id)).length() > 0) {
    server.arg("t" + String(id))
        .toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
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

#ifdef CONFIG_FUNCTIONALITY_REGULATOR
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

#ifdef CONFIG_HARDWARE_CONTACTRON
CONTACTRON AFEWebServer::getContactronData(uint8_t id) {
  CONTACTRON data;

  if (server.arg("o" + String(id)).length() > 0) {
    data.outputDefaultState = server.arg("o" + String(id)).toInt();
  }

  if (server.arg("l" + String(id)).length() > 0) {
    data.ledID = server.arg("l" + String(id)).toInt();
  }

  if (server.arg("b" + String(id)).length() > 0) {
    data.bouncing = server.arg("b" + String(id)).toInt();
  }

  if (server.arg("g" + String(id)).length() > 0) {
    data.gpio = server.arg("g" + String(id)).toInt();
  }

  if (server.arg("n" + String(id)).length() > 0) {
    server.arg("n" + String(id)).toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = '\0';
  }

  if (server.arg("x" + String(id)).length() > 0) {
    data.domoticz.idx = server.arg("x" + String(id)).toInt();
  }

  if (server.arg("t" + String(id)).length() > 0) {
    server.arg("t" + String(id))
        .toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = '\0';
  }

  return data;
}
#endif

#ifdef CONFIG_HARDWARE_GATE
GATE AFEWebServer::getGateData() {
  GATE data;
  for (uint8_t i = 0; i < sizeof(data.state); i++) {
    if (server.arg("s" + String(i)).length() > 0) {
      data.state[i] = server.arg("s" + String(i)).toInt();
    }
  }

  if (server.arg("x0").length() > 0) {
    data.domoticz.idx = server.arg("x0").toInt();
  }

  if (server.arg("t0").length() > 0) {
    server.arg("t0").toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
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
    data.outputDefaultState = server.arg("o" + String(id)).toInt();
  }

  if (server.arg("x" + String(id)).length() > 0) {
    data.idx = server.arg("x" + String(id)).toInt();
  }

  return data;
}
#endif

#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
LED AFEWebServer::getLEDData(uint8_t id) {
  LED data;
  if (server.arg("g" + String(id)).length() > 0) {
    data.gpio = server.arg("g" + String(id)).toInt();
  }

  server.arg("o" + String(id)).length() > 0
      ? data.changeToOppositeValue = true
      : data.changeToOppositeValue = false;

  return data;
}

uint8_t AFEWebServer::getSystemLEDData() {
  uint8_t data;

  if (server.arg("i").length() > 0) {
    data = server.arg("i").toInt();
  }

  return data;
}
#endif

#ifdef CONFIG_HARDWARE_DS18B20
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

#ifdef CONFIG_HARDWARE_DHXX
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

#ifdef CONFIG_HARDWARE_UART
SERIALPORT AFEWebServer::getSerialPortData() {
  SERIALPORT data;
  if (server.arg("r").length() > 0) {
    data.RXD = server.arg("r").toInt();
  }
  if (server.arg("t").length() > 0) {
    data.TXD = server.arg("t").toInt();
  }
  return data;
}
#endif

#ifdef CONFIG_HARDWARE_HPMA115S0
HPMA115S0 AFEWebServer::getHPMA115S0SensorData() {
  HPMA115S0 data;
  if (server.arg("i").length() > 0) {
    data.interval = server.arg("i").toInt();
  }

  if (server.arg("t").length() > 0) {
    data.timeToMeasure = server.arg("t").toInt();
  }

  if (server.arg("x2").length() > 0) {
    data.idx.pm25 = server.arg("x2").toInt();
  }

  if (server.arg("x1").length() > 0) {
    data.idx.pm10 = server.arg("x1").toInt();
  }
  return data;
};
#endif

#ifdef CONFIG_HARDWARE_BMX80
BMx80 AFEWebServer::getBMx80SensorData() {
  BMx80 data;

  if (server.arg("a").length() > 0) {
    data.i2cAddress = server.arg("a").toInt();
  }

  if (server.arg("i").length() > 0) {
    data.interval = server.arg("i").toInt();
  }

  if (server.arg("t").length() > 0) {
    data.idx.temperatureHumidityPressure = server.arg("t").toInt();
  }

  if (server.arg("g").length() > 0) {
    data.idx.gasResistance = server.arg("g").toInt();
  }

  if (server.arg("e").length() > 0) {
    data.idx.temperature = server.arg("e").toInt();
  }

  if (server.arg("h").length() > 0) {
    data.idx.humidity = server.arg("h").toInt();
  }

  if (server.arg("p").length() > 0) {
    data.idx.pressure = server.arg("p").toInt();
  }

  return data;
}
#endif

#ifdef CONFIG_HARDWARE_BH1750
BH1750 AFEWebServer::getBH1750SensorData() {
  BH1750 data;
  if (server.arg("a").length() > 0) {
    data.i2cAddress = server.arg("a").toInt();
  }

  if (server.arg("i").length() > 0) {
    data.interval = server.arg("i").toInt();
  }

  if (server.arg("m").length() > 0) {
    data.mode = server.arg("m").toInt();
  }

  if (server.arg("d").length() > 0) {
    data.idx = server.arg("d").toInt();
  }
  return data;
}
#endif

#ifdef CONFIG_HARDWARE_ADC_VCC
ADCINPUT AFEWebServer::getAnalogInputData() {
  ADCINPUT data;
  if (server.arg("g").length() > 0) {
    data.gpio = server.arg("g").toInt();
  }

  if (server.arg("i").length() > 0) {
    data.interval = server.arg("i").toInt();
  }

  if (server.arg("n").length() > 0) {
    data.numberOfSamples = server.arg("n").toInt();
  }

  if (server.arg("m").length() > 0) {
    data.maxVCC = server.arg("m").toFloat();
  }

  if (server.arg("ra").length() > 0) {
    data.divider.Ra = server.arg("ra").toFloat();
  }

  if (server.arg("rb").length() > 0) {
    data.divider.Rb = server.arg("rb").toFloat();
  }

  if (server.arg("t0").length() > 0) {
    server.arg("t0").toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
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
