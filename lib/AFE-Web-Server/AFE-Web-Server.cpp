/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#include "AFE-Web-Server.h"

AFEWebServer::AFEWebServer() {}

void AFEWebServer::begin() {
  httpUpdater.setup(&server);
  server.begin();
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

HTTPCOMMAND AFEWebServer::getHTTPCommand() {
  receivedHTTPCommand = false;
  return httpCommand;
}

void AFEWebServer::generate() {
  /* @TODO this method is not writen well */

  if (_refreshConfiguration) {
    _refreshConfiguration = false;
    Device.begin();
  }

  const String optionName = getOptionName();
  uint8_t command = getCommand();

  if (optionName == "language") {
    uint8_t data;
    if (command == SERVER_CMD_SAVE) {
      data = getLanguageData();
    }
    publishHTML(ConfigurationPanel.getLanguageConfigurationSite(command, data));
    if (command == SERVER_CMD_SAVE) {
      Device.reboot(Device.getMode());
    }
  } else if (optionName == "device") {
    DEVICE data;
    if (command == SERVER_CMD_SAVE) {
      data = getDeviceData();
    }
    publishHTML(ConfigurationPanel.getDeviceConfigurationSite(command, data));
  } else if (optionName == "network") {
    NETWORK data;
    if (command == SERVER_CMD_SAVE) {
      data = getNetworkData();
    }
    publishHTML(ConfigurationPanel.getNetworkConfigurationSite(command, data));
  } else if (optionName == "mqtt") {
    MQTT data;
    if (command == SERVER_CMD_SAVE) {
      data = getMQTTData();
    }
    publishHTML(ConfigurationPanel.getMQTTConfigurationSite(command, data));
  } else if (optionName == "domoticz") {
    DOMOTICZ data;
    if (command == SERVER_CMD_SAVE) {
      data = getDomoticzServerData();
    }
    publishHTML(
        ConfigurationPanel.getDomoticzServerConfigurationSite(command, data));
#ifdef CONFIG_HARDWARE_LED
  } else if (optionName == "led") {
    LED data[sizeof(Device.configuration.isLED)] = {};
    uint8_t dataLedID;
    if (command == SERVER_CMD_SAVE) {
      for (uint8_t i = 0; i < sizeof(Device.configuration.isLED); i++) {
        data[i] = getLEDData(i);
      }
      dataLedID = getSystemLEDData();
    }
    publishHTML(
        ConfigurationPanel.getLEDConfigurationSite(command, data, dataLedID));
#endif
#ifdef CONFIG_HARDWARE_UART
  } else if (optionName == "UART") {
    SERIALPORT data;
    if (command == SERVER_CMD_SAVE) {
      data = getSerialPortData();
    }
    publishHTML(
        ConfigurationPanel.getSerialPortConfigurationSite(command, data));
#endif
#ifdef CONFIG_HARDWARE_HPMA115S0
  } else if (optionName == "HPMA115S0") {
    HPMA115S0 data;
    if (command == SERVER_CMD_SAVE) {
      data = getHPMA115S0SensorData();
    }
    publishHTML(
        ConfigurationPanel.getHPMA115S0SensorConfigurationSite(command, data));
#endif
#ifdef CONFIG_HARDWARE_BMX80
  } else if (optionName == "BMx80") {
    BMx80 data;
    if (command == SERVER_CMD_SAVE) {
      data = getBMx80SensorData();
    }
    publishHTML(
        ConfigurationPanel.getBMx80SensorConfigurationSite(command, data));
#endif
#ifdef CONFIG_HARDWARE_BH1750
  } else if (optionName == "BH1750") {
    BH1750 data;
    if (command == SERVER_CMD_SAVE) {
      data = getBH1750SensorData();
    }
    publishHTML(
        ConfigurationPanel.getBH1750SensorConfigurationSite(command, data));
#endif
#ifdef CONFIG_HARDWARE_ADC_VCC
  } else if (optionName == "analogInput") {
    ADCINPUT data;
    if (command == SERVER_CMD_SAVE) {
      data = getAnalogInputData();
    }
    publishHTML(
        ConfigurationPanel.getAnalogInputConfigurationSite(command, data));
#endif
  } else if (optionName == "exit") {
    publishHTML(ConfigurationPanel.getSite(optionName, command, true));
    Device.reboot(MODE_NORMAL);
  } else if (optionName == "reset") {
    publishHTML(ConfigurationPanel.getSite(optionName, command, false));
    if (command == 1) {
      Device.setDevice();
      server.client().stop();
      Device.reboot(MODE_ACCESS_POINT);
    }
  } else if (optionName == "help") {
    publishHTML(ConfigurationPanel.getSite(
        optionName, command, command == SERVER_CMD_NONE ? false : true));
    if (command == 1) {
      server.client().stop();
      Device.reboot(MODE_CONFIGURATION);
    } else if (command == 2) {
      server.client().stop();
      Device.reboot(MODE_ACCESS_POINT);
    }

#ifdef CONFIG_HARDWARE_DS18B20
  } else if (optionName == "ds18b20") {
    DS18B20 data = {};
    if (command == SERVER_CMD_SAVE) {
      data = getDS18B20Data();
    }
    publishHTML(ConfigurationPanel.getDS18B20ConfigurationSite(command, data));
#endif
#ifdef CONFIG_HARDWARE_DHXX
  } else if (optionName == "DHT") {
    DH data = {};
    if (command == SERVER_CMD_SAVE) {
      data = getDHTData();
    }
    publishHTML(ConfigurationPanel.getDHTConfigurationSite(command, data));
#endif

#ifdef CONFIG_FUNCTIONALITY_REGULATOR
  } else if (optionName == "thermostat" || optionName == "humidistat") {
    REGULATOR data = {};
    if (command == SERVER_CMD_SAVE) {
      data = getRegulatorData();
    }
    publishHTML(ConfigurationPanel.getRelayStatConfigurationSite(
        command, data,
        optionName == "thermostat" ? THERMOSTAT_REGULATOR
                                   : HUMIDISTAT_REGULATOR));
#endif

  } else {
    for (uint8_t i = 0; i < sizeof(Device.configuration.isRelay); i++) {
      if (Device.configuration.isRelay[i]) {
        if (optionName == "relay" + String(i)) {
          RELAY data = {};
          if (command == SERVER_CMD_SAVE) {
            data = getRelayData(i);
          }
          publishHTML(
              ConfigurationPanel.getRelayConfigurationSite(command, data, i));
        }
      } else {
        break;
      }
    }

    for (uint8_t i = 0; i < sizeof(Device.configuration.isSwitch); i++) {
      if (Device.configuration.isSwitch[i]) {
        if (optionName == "switch" + String(i)) {
          SWITCH data = {};
          if (command == SERVER_CMD_SAVE) {
            data = getSwitchData(i);
          }
          publishHTML(
              ConfigurationPanel.getSwitchConfigurationSite(command, data, i));
        }
      } else {
        break;
      }
    }

#if defined(T3_CONFIG)
    for (uint8_t i = 0; i < 4; i++) {
      if (Device.configuration.isPIR[i]) {
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
    for (uint8_t i = 0; i < sizeof(Device.configuration.isContactron); i++) {
      if (Device.configuration.isContactron[i]) {
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
      publishHTML(ConfigurationPanel.getGateConfigurationSite(optionName,
                                                              command, data));
    }
#endif
  }
}

String AFEWebServer::getOptionName() {

  if (Device.getMode() == MODE_NORMAL) {
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
      return server.arg("command");

    } else {
      return "help";
    }
  } else {
    if (server.hasArg("option")) {
      return server.arg("option");
    } else {
      return "device";
    }
  }
}

uint8_t AFEWebServer::getCommand() {
  if (server.hasArg("cmd")) {
    return server.arg("cmd").toInt();
  } else {
    return SERVER_CMD_NONE;
  }
}

DEVICE AFEWebServer::getDeviceData() {
  DEVICE data;
  _refreshConfiguration =
      true; // it will cause that device configuration will be refeshed

  if (server.arg("dn").length() > 0) {
    server.arg("dn").toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = '\0';
  }

  server.arg("h").length() > 0 ? data.httpAPI = true : data.httpAPI = false;

  server.arg("m").length() > 0 ? data.mqttAPI = true : data.mqttAPI = false;

  server.arg("d").length() > 0 ? data.domoticzAPI = true
                               : data.domoticzAPI = false;
#ifdef CONFIG_HARDWARE_LED
  for (uint8_t i = 0; i < sizeof(Device.configuration.isLED); i++) {
    server.arg("hl").toInt() > i ? data.isLED[i] = true : data.isLED[i] = false;
  }
#endif

#if defined(T5_CONFIG)
  for (uint8_t i = 0; i < sizeof(Device.configuration.isContactron); i++) {
    server.arg("hc").toInt() > i ? data.isContactron[i] = true
                                 : data.isContactron[i] = false;
  }
#else
  for (uint8_t i = 0; i < sizeof(Device.configuration.isRelay); i++) {
    server.arg("hr").toInt() > i ? data.isRelay[i] = true
                                 : data.isRelay[i] = false;
  }
#endif

  for (uint8_t i = 0; i < sizeof(Device.configuration.isSwitch); i++) {
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
  for (uint8_t i = 0; i < sizeof(Device.configuration.isPIR); i++) {
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
    server.arg("s").toCharArray(data.ssid, sizeof(data.ssid) + 1);
  } else {
    data.ssid[0] = '\0';
  }

  if (server.arg("p").length() > 0) {
    server.arg("p").toCharArray(data.password, sizeof(data.password) + 1);
  } else {
    data.password[0] = '\0';
  }

  if (server.arg("d").length() > 0) {
    data.isDHCP = true;
  } else {
    data.isDHCP = false;
  }

  if (server.arg("d1").length() > 0 && server.arg("d2").length() > 0 &&
      server.arg("d3").length() > 0 && server.arg("d4").length() > 0) {

    data.ip = IPAddress(server.arg("d1").toInt(), server.arg("d2").toInt(),
                        server.arg("d3").toInt(), server.arg("d4").toInt());
  }
  if (server.arg("g1").length() > 0 && server.arg("g2").length() > 0 &&
      server.arg("g3").length() > 0 && server.arg("g4").length() > 0) {

    data.gateway =
        IPAddress(server.arg("g1").toInt(), server.arg("g2").toInt(),
                  server.arg("g3").toInt(), server.arg("g4").toInt());
  }
  if (server.arg("s1").length() > 0 && server.arg("s2").length() > 0 &&
      server.arg("s3").length() > 0 && server.arg("s4").length() > 0) {

    data.subnet = IPAddress(server.arg("s1").toInt(), server.arg("s2").toInt(),
                            server.arg("s3").toInt(), server.arg("s4").toInt());
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

  return data;
}

MQTT AFEWebServer::getMQTTData() {
  MQTT data;
  if (server.arg("h").length() > 0) {
    server.arg("h").toCharArray(data.host, sizeof(data.host) + 1);
  } else {
    data.host[0] = '\0';
  }

  if (server.arg("m1").length() > 0 && server.arg("m2").length() > 0 &&
      server.arg("m3").length() > 0 && server.arg("m4").length() > 0) {

    data.ip = IPAddress(server.arg("m1").toInt(), server.arg("m2").toInt(),
                        server.arg("m3").toInt(), server.arg("m4").toInt());
  }

  if (server.arg("p").length() > 0) {
    data.port = server.arg("p").toInt();
  }

  if (server.arg("u").length() > 0) {
    server.arg("u").toCharArray(data.user, sizeof(data.user) + 1);
  } else {
    data.user[0] = '\0';
  }

  if (server.arg("s").length() > 0) {
    server.arg("s").toCharArray(data.password, sizeof(data.password) + 1);
  } else {
    data.password[0] = '\0';
  }

  if (server.arg("t").length() > 0) {
    server.arg("t").toCharArray(data.topic, sizeof(data.topic) + 1);
  } else {
    data.topic[0] = '\0';
  }

  return data;
}

DOMOTICZ AFEWebServer::getDomoticzServerData() {
  DOMOTICZ data;

  if (server.arg("t").length() > 0) {
    data.protocol = server.arg("t").toInt();
  }

  if (server.arg("h").length() > 0) {
    server.arg("h").toCharArray(data.host, sizeof(data.host) + 1);
  } else {
    data.host[0] = '\0';
  }

  if (server.arg("p").length() > 0) {
    data.port = server.arg("p").toInt();
  }

  if (server.arg("u").length() > 0) {
    server.arg("u").toCharArray(data.user, sizeof(data.user) + 1);
  } else {
    data.user[0] = '\0';
  }
  if (server.arg("s").length() > 0) {
    server.arg("s").toCharArray(data.password, sizeof(data.password) + 1);
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

#if !(defined(T3_CONFIG) || defined(T6_CONFIG))
  if (server.arg("ot" + String(id)).length() > 0) {
    data.timeToOff = server.arg("ot" + String(id)).toFloat();
  }
#endif

#if !defined(T5_CONFIG)
  if (server.arg("pr" + String(id)).length() > 0) {
    data.statePowerOn = server.arg("pr" + String(id)).toInt();
  }

  if (server.arg("n" + String(id)).length() > 0) {
    server.arg("n" + String(id)).toCharArray(data.name, sizeof(data.name) + 1);
  }

  if (server.arg("mc" + String(id)).length() > 0) {
    data.stateMQTTConnected = server.arg("mc" + String(id)).toInt();
  }

#ifdef CONFIG_FUNCTIONALITY_THERMAL_PROTECTION
  if (server.arg("tp" + String(id)).length() > 0) {
    data.thermalProtection = server.arg("tp" + String(id)).toInt();
  }
#endif

  if (server.arg("x" + String(id)).length() > 0) {
    data.idx = server.arg("x" + String(id)).toInt();
  }

#endif

  if (server.arg("l" + String(id)).length() > 0) {
    data.ledID = server.arg("l" + String(id)).toInt();
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

SWITCH AFEWebServer::getSwitchData(uint8_t id) {
  SWITCH data;

  if (server.arg("t" + String(id)).length() > 0) {
    data.type = server.arg("t" + String(id)).toInt();
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

  return data;
}

#if defined(T5_CONFIG)
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
  }

  if (server.arg("x" + String(id)).length() > 0) {
    data.idx = server.arg("x" + String(id)).toInt();
  }

  return data;
}

GATE AFEWebServer::getGateData() {
  GATE data;
  for (uint8_t i = 0; i < sizeof(data.state); i++) {
    if (server.arg("s" + String(i)).length() > 0) {
      data.state[i] = server.arg("s" + String(i)).toInt();
    }
  }

  if (server.arg("x").length() > 0) {
    data.idx = server.arg("x").toInt();
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

#ifdef CONFIG_HARDWARE_LED
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

uint8_t AFEWebServer::getLanguageData() {
  return server.arg("l").length() > 0 ? server.arg("l").toInt() : 1;
}
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

  if (server.arg("r").length() > 0) {
    data.idx.raw = server.arg("r").toInt();
  }

  if (server.arg("p").length() > 0) {
    data.idx.percent = server.arg("p").toInt();
  }

  if (server.arg("v").length() > 0) {
    data.idx.voltage = server.arg("v").toInt();
  }

  return data;
}
#endif
