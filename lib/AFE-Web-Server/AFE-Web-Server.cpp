#include "AFE-Web-Server.h"

AFEWebServer::AFEWebServer() {}

void AFEWebServer::begin() {
  httpUpdater.setup(&server);
  server.begin();
}

void AFEWebServer::listener() { server.handleClient(); }

void AFEWebServer::publishHTML(String page) {
  server.send(200, "text/html", page);
}

void AFEWebServer::handle(const char *uri,
                          ESP8266WebServer::THandlerFunction handler) {
  // Serial << endl << "INFO: Added url : " << uri << " for listening";
  server.on(uri, handler);
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

  } else if (getOptionName() == "basic") {
    NETWORK data;
    if (getCommand() == SERVER_CMD_SAVE) {
      data = getNetworkData();
    }
    publishHTML(ConfigurationPanel.getBasicConfigurationSite(
        getOptionName(), getCommand(), data));

  } else if (getOptionName() == "mqtt") {
    MQTT data;
    if (getCommand() == SERVER_CMD_SAVE) {
      data = getMQTTData();
    }
    publishHTML(ConfigurationPanel.getMQTTConfigurationSite(
        getOptionName(), getCommand(), data));
    /* @TODO DOMOTICZ
  } else if (getOptionName() == "domoticz") {
    DOMOTICZ data;
    if (getCommand() == SERVER_CMD_SAVE) {
      data = getDomoticzData();
    }
    publishHTML(
        ConfigurationPanel.getSite(getOptionName(), getCommand(), data)); */
  } else if (getOptionName() == "relay") {
    RELAY data1 = {};
    RELAY data2 = {};
    if (getCommand() == SERVER_CMD_SAVE) {
      data1 = getRelayData(0);
      //  data2 = getRelayData(1);
    }
    publishHTML(ConfigurationPanel.getRelayConfigurationSite(
        getOptionName(), getCommand(), data1, data2));
  } /* @TODO DS18B20 else if (getOptionName() == "ds18b20") {
    DS18B20 data;
    if (getCommand() == SERVER_CMD_SAVE) {
      data = getDS18B20Data();
    }
    publishHTML(
        ConfigurationPanel.getSite(getOptionName(), getCommand(), data));
  } */ else if (getOptionName() == "switch") {
    SWITCH data1 = {};
    SWITCH data2 = {};
    if (getCommand() == SERVER_CMD_SAVE) {
      data1 = getSwitchData(0);
      //    data2 = getSwitchData(1);
    }
    publishHTML(ConfigurationPanel.getSwitchConfigurationSite(
        getOptionName(), getCommand(), data1, data2));
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
    return "help";
  } else {
    if (server.hasArg("option")) {
      return server.arg("option");
    } else {
      return "basic";
    }
  }
}

uint8_t AFEWebServer::getCommand() {
  if (server.hasArg("command")) {
    return server.arg("command").toInt();
  }
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

  if (server.arg("hostname").length() > 0) {
    server.arg("hostname").toCharArray(data.host, sizeof(data.host));
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

/* @TODO For MQTT only*/
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

  if (server.arg("relay" + String(id) + "_present").length() > 0) {
    data.present = true;
  } else {
    data.present = false;
  }

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

  // @TODO For MQTT only

  if (server.arg("relay" + String(id) + "_name").length() > 0) {
    server.arg("relay" + String(id) + "_name")
        .toCharArray(data.name, sizeof(data.name));
  }

  if (server.arg("relay" + String(id) + "_mqtt_connected").length() > 0) {
    data.stateMQTTConnected =
        server.arg("relay" + String(id) + "_mqtt_connected").toInt();
  }

  /* @TODO DOMOTICZ
  if (server.arg("relay" + String(id) + "_idx").length() > 0) {
    data.idx = server.arg("relay" + String(id) + "_idx").toInt();
  }
  if (server.arg("relay" + String(id) + "_publish_to_domoticz").length() > 0) {
    data.publishToDomoticz =
        server.arg("relay" + String(id) + "_publish_to_domoticz").toInt();
  }
  */

  return data;
}

SWITCH AFEWebServer::getSwitchData(uint8_t id) {
  SWITCH data;
  if (server.arg("switch" + String(id) + "_present").length() > 0) {
    data.present = true;
  } else {
    data.present = false;
  }
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

uint8_t AFEWebServer::getLanguageData() {
  return server.arg("language").length() > 0 ? server.arg("language").toInt()
                                             : 1;
}

/* @TODO DS18B20
DS18B20 AFEWebServer::getDS18B20Data() {
  DS18B20 data;

  if (server.arg("ds18b20_present").length() > 0) {
    data.present = true;
  } else {
    data.present = false;
  }

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
*/
/* @TODO DOMOTICZ
DOMOTICZ AFEWebServer::getDomoticzData() {
  DOMOTICZ data;

  if (server.arg("domoticz_host").length() > 0) {
    server.arg("domoticz_host").toCharArray(data.host, sizeof(data.host));
  }

  if (server.arg("domoticz_ip1").length() > 0 &&
      server.arg("domoticz_ip2").length() > 0 &&
      server.arg("domoticz_ip3").length() > 0 &&
      server.arg("domoticz_ip4").length() > 0) {

    data.ip = IPAddress(
        server.arg("domoticz_ip1").toInt(), server.arg("domoticz_ip2").toInt(),
        server.arg("domoticz_ip3").toInt(), server.arg("domoticz_ip4").toInt());
  }

  if (server.arg("domoticz_port").length() > 0) {
    data.port = server.arg("domoticz_port").toInt();
  }

  if (server.arg("domoticz_user").length() > 0) {
    server.arg("domoticz_user").toCharArray(data.user, sizeof(data.user));
  }

  if (server.arg("domoticz_password").length() > 0) {
    server.arg("domoticz_password")
        .toCharArray(data.password, sizeof(data.password));
  }
  return data;
}
*/
