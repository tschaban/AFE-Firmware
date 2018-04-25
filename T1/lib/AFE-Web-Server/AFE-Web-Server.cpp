/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#include "AFE-Web-Server.h"

AFEWebServer::AFEWebServer() {}

void AFEWebServer::begin() {
  httpUpdater.setup(&server);
  server.begin();
}

void AFEWebServer::listener() { server.handleClient(); }

boolean AFEWebServer::httpAPIlistener() { return receivedHTTPCommand; }

void AFEWebServer::publishHTML(String page) {
  server.send(200, "text/html", page);
}

void AFEWebServer::sendJSON(String json) {
  server.send(200, "application/json", json);
}

void AFEWebServer::handle(const char *uri,
                          ESP8266WebServer::THandlerFunction handler) {
  // Serial << endl << "INFO: Added url : " << uri << " for listening";
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
    publishHTML(ConfigurationPanel.getLanguageConfigurationSite(optionName,
                                                                command, data));

    if (command == SERVER_CMD_SAVE) {
      Device.reboot(Device.getMode());
    }
  } else if (optionName == "device") {
    DEVICE data;
    if (command == SERVER_CMD_SAVE) {
      data = getDeviceData();
    }
    publishHTML(ConfigurationPanel.getDeviceConfigurationSite(optionName,
                                                              command, data));
  } else if (optionName == "network") {
    NETWORK data;
    if (command == SERVER_CMD_SAVE) {
      data = getNetworkData();
    }
    publishHTML(ConfigurationPanel.getNetworkConfigurationSite(optionName,
                                                               command, data));
  } else if (optionName == "mqtt") {
    MQTT data;
    if (command == SERVER_CMD_SAVE) {
      data = getMQTTData();
    }
    publishHTML(
        ConfigurationPanel.getMQTTConfigurationSite(optionName, command, data));
  } else if (optionName == "domoticz") {
    DOMOTICZ data;
    if (command == SERVER_CMD_SAVE) {
      data = getDomoticzServerData();
    }
    publishHTML(ConfigurationPanel.getDomoticzServerConfigurationSite(
        optionName, command, data));
  } else if (optionName == "led") {
    LED data[sizeof(Device.configuration.isLED)] = {};
    uint8_t dataLedID;
    if (command == SERVER_CMD_SAVE) {
      for (uint8_t i = 0; i < sizeof(Device.configuration.isLED); i++) {
        data[i] = getLEDData(i);
      }
      dataLedID = getSystemLEDData();
    }
    publishHTML(ConfigurationPanel.getLEDConfigurationSite(optionName, command,
                                                           data, dataLedID));
  } else if (optionName == "ds18b20") {
    DS18B20 data1 = {};
    if (command == SERVER_CMD_SAVE) {
      data1 = getDS18B20Data();
    }
    publishHTML(ConfigurationPanel.getDS18B20ConfigurationSite(optionName,
                                                               command, data1));
  } else if (optionName == "thermostat") {
    REGULATOR data = {};
    if (command == SERVER_CMD_SAVE) {
      data = getThermostateData();
    }
    publishHTML(ConfigurationPanel.getRelayStatConfigurationSite(
        optionName, command, data));
  } else if (optionName == "exit") {
    publishHTML(ConfigurationPanel.getSite(optionName, command, true));
    Device.reboot(MODE_NORMAL);
  } else if (optionName == "reset") {
    publishHTML(ConfigurationPanel.getSite(optionName, command, false));
    if (command == 1) {
      Device.setDevice();
      Device.reboot(MODE_ACCESS_POINT);
    }
  } else if (optionName == "help") {
    publishHTML(ConfigurationPanel.getSite(optionName, command,
                                           command == 0 ? false : true));
    if (command == 1) {
      Device.reboot(MODE_CONFIGURATION);
    } else if (command == 2) {
      Device.reboot(MODE_ACCESS_POINT);
    }
  } else {
    for (uint8_t i = 0; i < sizeof(Device.configuration.isRelay); i++) {
      if (Device.configuration.isRelay[i]) {
        if (optionName == "relay" + String(i)) {
          RELAY data = {};
          if (command == SERVER_CMD_SAVE) {
            data = getRelayData(i);
          }
          publishHTML(ConfigurationPanel.getRelayConfigurationSite(
              optionName, command, data, i));
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
          publishHTML(ConfigurationPanel.getSwitchConfigurationSite(
              optionName, command, data, i));
        }
      } else {
        break;
      }
    }
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
      /*
            Serial << endl
                   << "GET: Device=" << httpCommand.device
                   << " Name=" << httpCommand.name << " Source=" <<
         httpCommand.source
                   << " Command=" << httpCommand.command;
      */
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

  for (uint8_t i = 0; i < sizeof(Device.configuration.isLED); i++) {
    server.arg("hl").toInt() > i ? data.isLED[i] = true : data.isLED[i] = false;
  }

  for (uint8_t i = 0; i < sizeof(Device.configuration.isRelay); i++) {
    server.arg("hr").toInt() > i ? data.isRelay[i] = true
                                 : data.isRelay[i] = false;
  }

  for (uint8_t i = 0; i < sizeof(Device.configuration.isSwitch); i++) {
    server.arg("hs").toInt() > i ? data.isSwitch[i] = true
                                 : data.isSwitch[i] = false;
  }
  server.arg("ds").length() > 0 ? data.isDS18B20 = true
                                : data.isDS18B20 = false;

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

  if (server.arg("ot" + String(id)).length() > 0) {
    data.timeToOff = server.arg("ot" + String(id)).toFloat();
  }

  if (server.arg("pr" + String(id)).length() > 0) {
    data.statePowerOn = server.arg("pr" + String(id)).toInt();
  }

  if (server.arg("n" + String(id)).length() > 0) {
    server.arg("n" + String(id)).toCharArray(data.name, sizeof(data.name) + 1);
  }

  if (server.arg("mc" + String(id)).length() > 0) {
    data.stateMQTTConnected = server.arg("mc" + String(id)).toInt();
  }

  if (server.arg("tp" + String(id)).length() > 0) {
    data.thermalProtection = server.arg("tp" + String(id)).toInt();
  }

  if (server.arg("l" + String(id)).length() > 0) {
    data.ledID = server.arg("l" + String(id)).toInt();
  }

  if (server.arg("x" + String(id)).length() > 0) {
    data.idx = server.arg("x" + String(id)).toInt();
  }

  return data;
}

REGULATOR AFEWebServer::getThermostateData() {
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

uint8_t AFEWebServer::getLanguageData() {
  return server.arg("l").length() > 0 ? server.arg("l").toInt() : 1;
}

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

  return data;
}
