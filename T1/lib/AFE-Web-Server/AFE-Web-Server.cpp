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
    LED data1 = {};
    if (getCommand() == SERVER_CMD_SAVE) {
      data1 = getLEDData(0);
    }
    publishHTML(ConfigurationPanel.getLEDConfigurationSite(
        getOptionName(), getCommand(), data1));
  } else if (getOptionName() == "relay") {
    RELAY data1 = {};
    RELAY data2 = {};
    if (getCommand() == SERVER_CMD_SAVE) {
      data1 = getRelayData(0);
      //  data2 = getRelayData(1);
    }
    publishHTML(ConfigurationPanel.getRelayConfigurationSite(
        getOptionName(), getCommand(), data1, data2));
  } else if (getOptionName() == "switch") {
    SWITCH data1 = {};
    SWITCH data2 = {};
    if (getCommand() == SERVER_CMD_SAVE) {
      data1 = getSwitchData(0);
      data2 = getSwitchData(1);
    }
    publishHTML(ConfigurationPanel.getSwitchConfigurationSite(
        getOptionName(), getCommand(), data1, data2));
  } else if (getOptionName() == "ds18b20") {
        DS18B20 data1 = {};
        if (getCommand() == SERVER_CMD_SAVE) {
          data1 = getDS18B20Data();
        }
        publishHTML(ConfigurationPanel.getDS18B20ConfigurationSite(
            getOptionName(), getCommand(), data1));
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

  if (server.arg("deviceName").length() > 0) {
    server.arg("deviceName").toCharArray(data.name, sizeof(data.name));
  }

  server.arg("httpAPI").length() > 0 ? data.httpAPI = true
                                     : data.httpAPI = false;

  server.arg("mqttAPI").length() > 0 ? data.mqttAPI = true
                                     : data.mqttAPI = false;

  server.arg("relay0").length() > 0 ? data.isRelay[0] = true
                                    : data.isRelay[0] = false;

  server.arg("switch0").length() > 0 ? data.isSwitch[0] = true
                                     : data.isSwitch[0] = false;

  server.arg("switch1").length() > 0 ? data.isSwitch[1] = true
                                     : data.isSwitch[1] = false;

  server.arg("led0").length() > 0 ? data.isLED[0] = true
                                  : data.isLED[0] = false;

  server.arg("ds18b20").length() > 0 ? data.isDS18B20 = true
                                     : data.isDS18B20 = false;

  return data;
}

NETWORK AFEWebServer::getNetworkData() {
  NETWORK data;
  if (server.arg("wifi_ssid").length() > 0) {
    server.arg("wifi_ssid").toCharArray(data.ssid, sizeof(data.ssid));
  }

  if (server.arg("wifi_password").length() > 0) {
    server.arg("wifi_password")
        .toCharArray(data.password, sizeof(data.password));
  }

  if (server.arg("dhcp_config").length() > 0) {
    data.isDHCP = true;
  } else {
    data.isDHCP = false;
  }

  if (server.arg("device_ip1").length() > 0 &&
      server.arg("device_ip2").length() > 0 &&
      server.arg("device_ip3").length() > 0 &&
      server.arg("device_ip4").length() > 0) {

    data.ip = IPAddress(
        server.arg("device_ip1").toInt(), server.arg("device_ip2").toInt(),
        server.arg("device_ip3").toInt(), server.arg("device_ip4").toInt());
  }
  if (server.arg("gateway_ip1").length() > 0 &&
      server.arg("gateway_ip2").length() > 0 &&
      server.arg("gateway_ip3").length() > 0 &&
      server.arg("gateway_ip4").length() > 0) {

    data.gateway = IPAddress(
        server.arg("gateway_ip1").toInt(), server.arg("gateway_ip2").toInt(),
        server.arg("gateway_ip3").toInt(), server.arg("gateway_ip4").toInt());
  }
  if (server.arg("subnet_ip1").length() > 0 &&
      server.arg("subnet_ip2").length() > 0 &&
      server.arg("subnet_ip3").length() > 0 &&
      server.arg("subnet_ip4").length() > 0) {

    data.subnet = IPAddress(
        server.arg("subnet_ip1").toInt(), server.arg("subnet_ip2").toInt(),
        server.arg("subnet_ip3").toInt(), server.arg("subnet_ip4").toInt());
  }
  if (server.arg("no_connection_attempts").length() > 0) {
    data.noConnectionAttempts = server.arg("no_connection_attempts").toInt();
  }
  if (server.arg("wait_time_connections").length() > 0) {
    data.waitTimeConnections = server.arg("wait_time_connections").toInt();
  }
  if (server.arg("wait_time_series").length() > 0) {
    data.waitTimeSeries = server.arg("wait_time_series").toInt();
  }

  return data;
}

MQTT AFEWebServer::getMQTTData() {
  MQTT data;
  if (server.arg("mqtt_host").length() > 0) {
    server.arg("mqtt_host").toCharArray(data.host, sizeof(data.host));
  }

  if (server.arg("mqtt_ip1").length() > 0 &&
      server.arg("mqtt_ip2").length() > 0 &&
      server.arg("mqtt_ip3").length() > 0 &&
      server.arg("mqtt_ip4").length() > 0) {

    data.ip = IPAddress(
        server.arg("mqtt_ip1").toInt(), server.arg("mqtt_ip2").toInt(),
        server.arg("mqtt_ip3").toInt(), server.arg("mqtt_ip4").toInt());
  }

  if (server.arg("mqtt_port").length() > 0) {
    data.port = server.arg("mqtt_port").toInt();
  }

  if (server.arg("mqtt_user").length() > 0) {
    server.arg("mqtt_user").toCharArray(data.user, sizeof(data.user));
  }

  if (server.arg("mqtt_password").length() > 0) {
    server.arg("mqtt_password")
        .toCharArray(data.password, sizeof(data.password));
  }

  if (server.arg("mqtt_topic").length() > 0) {
    server.arg("mqtt_topic").toCharArray(data.topic, sizeof(data.topic));
  }

  return data;
}

RELAY AFEWebServer::getRelayData(uint8_t id) {
  RELAY data;

  if (server.arg("relay" + String(id) + "_gpio").length() > 0) {
    data.gpio = server.arg("relay" + String(id) + "_gpio").toInt();
  }
  if (server.arg("relay" + String(id) + "_off_time").length() > 0) {
    data.timeToOff = server.arg("relay" + String(id) + "_off_time").toFloat();
  }
  if (server.arg("relay" + String(id) + "_power_restored").length() > 0) {
    data.statePowerOn =
        server.arg("relay" + String(id) + "_power_restored").toInt();
  }

  if (server.arg("relay" + String(id) + "_name").length() > 0) {
    server.arg("relay" + String(id) + "_name")
        .toCharArray(data.name, sizeof(data.name));
  }

  if (server.arg("relay" + String(id) + "_mqtt_connected").length() > 0) {
    data.stateMQTTConnected =
        server.arg("relay" + String(id) + "_mqtt_connected").toInt();
  }

  return data;
}

SWITCH AFEWebServer::getSwitchData(uint8_t id) {
  SWITCH data;

  if (server.arg("switch" + String(id) + "_type").length() > 0) {
    data.type = server.arg("switch" + String(id) + "_type").toInt();
  }

  if (server.arg("switch" + String(id) + "_sensitivity").length() > 0) {
    data.sensitiveness =
        server.arg("switch" + String(id) + "_sensitivity").toInt();
  }

  if (server.arg("switch" + String(id) + "_functionality").length() > 0) {
    data.functionality =
        server.arg("switch" + String(id) + "_functionality").toInt();
  }

  if (server.arg("switch" + String(id) + "_gpio").length() > 0) {
    data.gpio = server.arg("switch" + String(id) + "_gpio").toInt();
  }

  return data;
}

LED AFEWebServer::getLEDData(uint8_t id) {
  LED data;
  if (server.arg("led" + String(id) + "_gpio").length() > 0) {
    data.gpio = server.arg("led" + String(id) + "_gpio").toInt();
  }

  server.arg("led" + String(id) + "_opposite").length() > 0
      ? data.changeToOppositeValue = true
      : data.changeToOppositeValue = false;

  return data;
}

uint8_t AFEWebServer::getLanguageData() {
  return server.arg("language").length() > 0 ? server.arg("language").toInt()
                                             : 1;
}

DS18B20 AFEWebServer::getDS18B20Data() {
  DS18B20 data;

  if (server.arg("ds18b20_gpio").length() > 0) {
    data.gpio = server.arg("ds18b20_gpio").toInt();
  }

  if (server.arg("ds18b20_correction").length() > 0) {
    data.correction = server.arg("ds18b20_correction").toFloat();
  }

  if (server.arg("ds18b20_interval").length() > 0) {
    data.interval = server.arg("ds18b20_interval").toInt();
  }

  if (server.arg("ds18b20_unit").length() > 0) {
    data.unit = server.arg("ds18b20_unit").toInt();
  }
  return data;
}
