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
    LED data[5] = {};
    if (getCommand() == SERVER_CMD_SAVE) {
      for (uint8_t i = 0; i < 5; i++) {
        data[i] = getLEDData(i);
      }
    }
    publishHTML(ConfigurationPanel.getLEDConfigurationSite(getOptionName(),
                                                           getCommand(), data));
  } else if (getOptionName() == "relay") {
    RELAY data[4] = {};
    if (getCommand() == SERVER_CMD_SAVE) {
      for (uint8_t i = 0; i < 4; i++) {
        data[i] = getRelayData(i);
      }
    }
    publishHTML(ConfigurationPanel.getRelayConfigurationSite(
        getOptionName(), getCommand(), data));
  } else if (getOptionName() == "switch") {
    SWITCH data[5];
    if (getCommand() == SERVER_CMD_SAVE) {
      for (uint8_t i = 0; i < 5; i++) {
        data[i] = getSwitchData(i);
      }
    }
    publishHTML(ConfigurationPanel.getSwitchConfigurationSite(
        getOptionName(), getCommand(), data));
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

  if (server.arg("n").length() > 0) {
    server.arg("n").toCharArray(data.name, sizeof(data.name));
  }

  server.arg("h").length() > 0 ? data.httpAPI = true : data.httpAPI = false;

  server.arg("m").length() > 0 ? data.mqttAPI = true : data.mqttAPI = false;

  server.arg("r0").length() > 0 ? data.isRelay[0] = true
                                : data.isRelay[0] = false;

  server.arg("r1").length() > 0 ? data.isRelay[1] = true
                                : data.isRelay[1] = false;

  server.arg("r2").length() > 0 ? data.isRelay[2] = true
                                : data.isRelay[2] = false;

  server.arg("r3").length() > 0 ? data.isRelay[3] = true
                                : data.isRelay[3] = false;

  server.arg("s0").length() > 0 ? data.isSwitch[0] = true
                                : data.isSwitch[0] = false;
  server.arg("s1").length() > 0 ? data.isSwitch[1] = true
                                : data.isSwitch[1] = false;
  server.arg("s2").length() > 0 ? data.isSwitch[2] = true
                                : data.isSwitch[2] = false;
  server.arg("s3").length() > 0 ? data.isSwitch[3] = true
                                : data.isSwitch[3] = false;
  server.arg("s4").length() > 0 ? data.isSwitch[4] = true
                                : data.isSwitch[4] = false;

  server.arg("p0").length() > 0 ? data.isPIR[0] = true : data.isPIR[0] = false;
  server.arg("p1").length() > 0 ? data.isPIR[1] = true : data.isPIR[1] = false;
  server.arg("p2").length() > 0 ? data.isPIR[2] = true : data.isPIR[2] = false;
  server.arg("p3").length() > 0 ? data.isPIR[3] = true : data.isPIR[3] = false;

  server.arg("l0").length() > 0 ? data.isLED[0] = true : data.isLED[0] = false;
  server.arg("l1").length() > 0 ? data.isLED[1] = true : data.isLED[1] = false;
  server.arg("l2").length() > 0 ? data.isLED[2] = true : data.isLED[2] = false;
  server.arg("l3").length() > 0 ? data.isLED[3] = true : data.isLED[3] = false;
  server.arg("l4").length() > 0 ? data.isLED[4] = true : data.isLED[4] = false;

  return data;
}

NETWORK AFEWebServer::getNetworkData() {
  NETWORK data;
  if (server.arg("n").length() > 0) {
    server.arg("n").toCharArray(data.ssid, sizeof(data.ssid));
  }

  if (server.arg("p").length() > 0) {
    server.arg("p").toCharArray(data.password, sizeof(data.password));
  }

  if (server.arg("d").length() > 0) {
    data.isDHCP = true;
  } else {
    data.isDHCP = false;
  }

  if (server.arg("i1").length() > 0 && server.arg("i2").length() > 0 &&
      server.arg("i3").length() > 0 && server.arg("i4").length() > 0) {

    data.ip = IPAddress(server.arg("i1").toInt(), server.arg("i2").toInt(),
                        server.arg("i3").toInt(), server.arg("i4").toInt());
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

RELAY AFEWebServer::getRelayData(uint8_t id) {
  RELAY data;

  if (server.arg("g" + String(id)).length() > 0) {
    data.gpio = server.arg("g" + String(id)).toInt();
  }

  if (server.arg("r" + String(id)).length() > 0) {
    data.statePowerOn = server.arg("r" + String(id)).toInt();
  }

  if (server.arg("n" + String(id)).length() > 0) {
    server.arg("n" + String(id)).toCharArray(data.name, sizeof(data.name));
  }

  if (server.arg("c" + String(id)).length() > 0) {
    data.stateMQTTConnected = server.arg("c" + String(id)).toInt();
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

  return data;
}

uint8_t AFEWebServer::getLanguageData() {
  return server.arg("l").length() > 0 ? server.arg("l").toInt() : 1;
}
