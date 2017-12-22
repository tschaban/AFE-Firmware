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
  if (getOptionName() == "language") {
    uint8_t data;
    if (getCommand() == SERVER_CMD_SAVE) {
      data = getLanguageData();
    }
    publishHTML(ConfigurationPanel.getLanguageConfigurationSite(
        getOptionName(), getCommand(), data));

    if (getCommand() == SERVER_CMD_SAVE) {
      Device.reboot(Device.getMode());
    }
  } else if (getOptionName() == "device") {
    DEVICE data;
    if (getCommand() == SERVER_CMD_SAVE) {
      data = getDeviceData();
    }
    publishHTML(ConfigurationPanel.getDeviceConfigurationSite(
        getOptionName(), getCommand(), data));
  } else if (getOptionName() == "network") {
    NETWORK data;
    if (getCommand() == SERVER_CMD_SAVE) {
      data = getNetworkData();
    }
    publishHTML(ConfigurationPanel.getNetworkConfigurationSite(
        getOptionName(), getCommand(), data));

  } else if (getOptionName() == "mqtt") {
    MQTT data;
    if (getCommand() == SERVER_CMD_SAVE) {
      data = getMQTTData();
    }
    publishHTML(ConfigurationPanel.getMQTTConfigurationSite(
        getOptionName(), getCommand(), data));
  } else if (getOptionName() == "led") {
    LED data = {};
    if (getCommand() == SERVER_CMD_SAVE) {
      data = getLEDData();
    }
    publishHTML(ConfigurationPanel.getLEDConfigurationSite(getOptionName(),
                                                           getCommand(), data));
  } else if (getOptionName() == "relay") {
    RELAY data = {};
    if (getCommand() == SERVER_CMD_SAVE) {
      data = getRelayData();
    }
    publishHTML(ConfigurationPanel.getRelayConfigurationSite(
        getOptionName(), getCommand(), data));
  } else if (getOptionName() == "switch") {
    SWITCH data1 = {};
    SWITCH data2 = {};
    if (getCommand() == SERVER_CMD_SAVE) {
      data1 = getSwitchData(0);
      data2 = getSwitchData(1);
    }
    publishHTML(ConfigurationPanel.getSwitchConfigurationSite(
        getOptionName(), getCommand(), data1, data2));
  } else if (getOptionName() == "DHT") {
    DH data = {};
    if (getCommand() == SERVER_CMD_SAVE) {
      data = getDHTData();
    }
    publishHTML(ConfigurationPanel.getDHTConfigurationSite(getOptionName(),
                                                           getCommand(), data));
  } else if (getOptionName() == "thermostat" ||
             getOptionName() == "humidistat") {
    RELAYSTAT data = {};
    if (getCommand() == SERVER_CMD_SAVE) {
      getOptionName() == "thermostat" ? data = getThermostateData()
                                      : data = getHumidistatData();
    }
    publishHTML(ConfigurationPanel.getRelayStatConfigurationSite(
        getOptionName(), getCommand(), data,
        getOptionName() == "thermostat" ? true : false));
  } else if (getOptionName() == "exit") {
    publishHTML(
        ConfigurationPanel.getSite(getOptionName(), getCommand(), true));
    Device.reboot(MODE_NORMAL);
  } else if (getOptionName() == "reset") {
    publishHTML(
        ConfigurationPanel.getSite(getOptionName(), getCommand(), false));
    if (getCommand() == 1) {
      Device.setDevice();
      Device.reboot(MODE_ACCESS_POINT);
    }
  } else if (getOptionName() == "help") {
    publishHTML(ConfigurationPanel.getSite(getOptionName(), getCommand(),
                                           getCommand() == 0 ? false : true));
    if (getCommand() == 1) {
      Device.reboot(MODE_CONFIGURATION);
    } else if (getCommand() == 2) {
      Device.reboot(MODE_ACCESS_POINT);
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

  if (server.arg("dn").length() > 0) {
    server.arg("dn").toCharArray(data.name, sizeof(data.name));
  }

  server.arg("h").length() > 0 ? data.httpAPI = true : data.httpAPI = false;

  server.arg("m").length() > 0 ? data.mqttAPI = true : data.mqttAPI = false;

  server.arg("r").length() > 0 ? data.isRelay[0] = true
                               : data.isRelay[0] = false;

  server.arg("s0").length() > 0 ? data.isSwitch[0] = true
                                : data.isSwitch[0] = false;

  server.arg("s1").length() > 0 ? data.isSwitch[1] = true
                                : data.isSwitch[1] = false;

  server.arg("l").length() > 0 ? data.isLED[0] = true : data.isLED[0] = false;

  server.arg("ds").length() > 0 ? data.isDHT = true : data.isDHT = false;

  return data;
}

NETWORK AFEWebServer::getNetworkData() {
  NETWORK data;
  if (server.arg("s").length() > 0) {
    server.arg("s").toCharArray(data.ssid, sizeof(data.ssid));
  }

  if (server.arg("p").length() > 0) {
    server.arg("p").toCharArray(data.password, sizeof(data.password));
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
    server.arg("h").toCharArray(data.host, sizeof(data.host));
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
    server.arg("u").toCharArray(data.user, sizeof(data.user));
  }

  if (server.arg("s").length() > 0) {
    server.arg("s").toCharArray(data.password, sizeof(data.password));
  }

  if (server.arg("t").length() > 0) {
    server.arg("t").toCharArray(data.topic, sizeof(data.topic));
  }

  return data;
}

RELAY AFEWebServer::getRelayData() {
  RELAY data;

  if (server.arg("g").length() > 0) {
    data.gpio = server.arg("g").toInt();
  }

  if (server.arg("ot").length() > 0) {
    data.timeToOff = server.arg("ot").toFloat();
  }

  if (server.arg("pr").length() > 0) {
    data.statePowerOn = server.arg("pr").toInt();
  }

  if (server.arg("n").length() > 0) {
    server.arg("n").toCharArray(data.name, sizeof(data.name));
  }

  if (server.arg("mc").length() > 0) {
    data.stateMQTTConnected = server.arg("mc").toInt();
  }

  if (server.arg("tp").length() > 0) {
    data.thermalProtection = server.arg("tp").toInt();
  }

  return data;
}

RELAYSTAT AFEWebServer::getThermostateData() {
  RELAYSTAT data;
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

RELAYSTAT AFEWebServer::getHumidistatData() {

  RELAYSTAT data;

  server.arg("he").length() > 0 ? data.enabled = true : data.enabled = false;

  if (server.arg("hn").length() > 0) {
    data.turnOn = server.arg("hn").toFloat();
  }

  if (server.arg("hf").length() > 0) {
    data.turnOff = server.arg("hf").toFloat();
  }

  if (server.arg("ha").length() > 0) {
    data.turnOnAbove = server.arg("ha").toInt() == 0 ? false : true;
  }

  if (server.arg("hb").length() > 0) {
    data.turnOffAbove = server.arg("hb").toInt() == 0 ? false : true;
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

  return data;
}

LED AFEWebServer::getLEDData() {
  LED data;
  if (server.arg("l").length() > 0) {
    data.gpio = server.arg("l").toInt();
  }

  server.arg("o").length() > 0 ? data.changeToOppositeValue = true
                               : data.changeToOppositeValue = false;

  return data;
}

uint8_t AFEWebServer::getLanguageData() {
  return server.arg("l").length() > 0 ? server.arg("l").toInt() : 1;
}

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
    data.temperature.interval = server.arg("i").toInt();
  }

  if (server.arg("u").length() > 0) {
    data.temperature.unit = server.arg("u").toInt();
  }

  if (server.arg("d").length() > 0) {
    data.humidity.correction = server.arg("d").toFloat();
  }

  if (server.arg("j").length() > 0) {
    data.humidity.interval = server.arg("j").toInt();
  }

  return data;
}
