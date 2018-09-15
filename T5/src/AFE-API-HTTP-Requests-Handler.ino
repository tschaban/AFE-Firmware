/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

/* Method listens for HTTP Requests */
void mainHTTPRequestsHandler() {
  if (Device.configuration.httpAPI) {
    if (WebServer.httpAPIlistener()) {
      Led.on();
      processHTTPAPIRequest(WebServer.getHTTPCommand());
      Led.off();
    }
  }
}

/* Method creates JSON respons after processing HTTP API request, and pushes it.
 * The second one method converts float to charString before pushing response */
void sendHTTPAPIRequestStatus(HTTPCOMMAND request, boolean status,
                              const char *value = "") {
  String respond;
  respond = "{";
  if (strlen(request.device) > 0) {
    respond += "\"device\":\"" + String(request.device) + "\",";
  }
  if (strlen(request.name) > 0) {
    respond += "\"name\":\"" + String(request.name) + "\",";
  }
  if (strlen(request.command) > 0) {
    respond += "\"command\":\"" + String(request.command) + "\",";
  }

  if (!strlen(value) == 0) {
    respond += "\"value\":\"";
    respond += value;
    respond += "\",";
  }

  respond += "\"status\":\"";
  respond += status ? "success" : "error";
  respond += "\"}";
  WebServer.sendJSON(respond);
}

void sendHTTPAPIRequestStatus(HTTPCOMMAND request, boolean status, float value,
                              uint8_t width = 2, uint8_t precision = 2) {
  char valueString[10];
  dtostrf(value, width, precision, valueString);
  sendHTTPAPIRequestStatus(request, status, valueString);
}

/* Method converts Relay value to string and invokes sendHTTPAPIRequestStatus
 * method which creates JSON respons and pushes it */
void sendHTTPAPIRelayRequestStatus(HTTPCOMMAND request, boolean status,
                                   byte value) {
  sendHTTPAPIRequestStatus(request, status, value == RELAY_ON ? "on" : "off");
}

/* It constructs HTTP response related to gate and calls HTTP push */
void sendHTTPAPIGateRequestStatus(HTTPCOMMAND request, boolean status,
                                  byte value) {
  sendHTTPAPIRequestStatus(
      request, status,
      value == GATE_OPEN
          ? "open"
          : value == GATE_CLOSED
                ? "closed"
                : value == GATE_PARTIALLY_OPEN ? "partiallyOpen" : "unknown");
}

/* It constructs HTTP response related to contactron and calls HTTP push */
void sendHTTPAPIContactronRequestStatus(HTTPCOMMAND request, boolean status,
                                        byte value) {
  sendHTTPAPIRequestStatus(request, status,
                           value == CONTACTRON_OPEN ? "open" : "closed");
}

/* Method processes HTTP API request */
void processHTTPAPIRequest(HTTPCOMMAND request) {
  /* Checking of request is about a relay */
  if (strcmp(request.device, "gate") == 0) {
    if (strcmp(request.command, "open") == 0) {
      Relay[0].on();
      sendHTTPAPIRelayRequestStatus(request, Relay[0].get() == RELAY_ON,
                                    Relay[0].get());
      MQTTPublishRelayState(0);
    } else if (strcmp(request.command, "close") == 0) {
      Relay[0].on();
      sendHTTPAPIRelayRequestStatus(request, Relay[0].get() == RELAY_ON,
                                    Relay[0].get());
      MQTTPublishRelayState(0);
    } else if (strcmp(request.command, "get") == 0) { // reportStatus or get
      sendHTTPAPIGateRequestStatus(request, true, Gate.get());
    } else {
      sendHTTPAPIRequestStatus(request, false);
    }
  } else if (strcmp(request.device, "relay") == 0) {
    if (strcmp(request.command, "on") == 0) {
      Relay[0].on();
      sendHTTPAPIRelayRequestStatus(request, Relay[0].get() == RELAY_ON,
                                    Relay[0].get());
      MQTTPublishRelayState(0);
      if (strcmp(request.source, "domoticz") != 0) {
        DomoticzPublishRelayState(0);
      }
    } else if (strcmp(request.command, "off") == 0) {
      Relay[0].off();
      sendHTTPAPIRelayRequestStatus(request, Relay[0].get() == RELAY_OFF,
                                    Relay[0].get());
      MQTTPublishRelayState(0);
      if (strcmp(request.source, "domoticz") != 0) {
        DomoticzPublishRelayState(0);
      }

    } else if (strcmp(request.command, "toggle") == 0) {
      uint8_t state = Relay[0].get();
      Relay[0].toggle();
      sendHTTPAPIRelayRequestStatus(request, state != Relay[0].get(),
                                    Relay[0].get());
      MQTTPublishRelayState(0); // MQTT Listener library
      if (strcmp(request.source, "domoticz") != 0) {
        DomoticzPublishRelayState(0);
      };
    } else if (strcmp(request.command, "get") == 0) { // reportStatus or get
      sendHTTPAPIRelayRequestStatus(request, true, Relay[0].get());
    } else {
      sendHTTPAPIRequestStatus(request, false);
    }
  } else if (strcmp(request.device, "contactron") == 0) {
    boolean noContactron = true;
    for (uint8_t i = 0; i < sizeof(Device.configuration.isContactron); i++) {
      if (Device.configuration.isContactron[i]) {
        if (strcmp(request.name, Gate.Contactron[i].getName()) == 0) {
          noContactron = false;
          if (strcmp(request.command, "get") == 0) {
            sendHTTPAPIContactronRequestStatus(request, true,
                                               Gate.Contactron[i].get());
            /* Command not implemented.Info */
          } else {
            sendHTTPAPIRequestStatus(request, false);
          }
        }
      } else {
        break;
      }
    }
    if (noContactron) {
      sendHTTPAPIRequestStatus(request, false);
    }
  } else if (strcmp(request.name, "temperature") == 0) {
    strcmp(request.command, "get") == 0
        ? sendHTTPAPIRequestStatus(request, true, SensorDHT.getTemperature())
        : sendHTTPAPIRequestStatus(request, false);
  } else if (strcmp(request.name, "humidity") == 0) {
    strcmp(request.command, "get") == 0
        ? sendHTTPAPIRequestStatus(request, true, SensorDHT.getHumidity())
        : sendHTTPAPIRequestStatus(request, false);
  } else if (strcmp(request.name, "heatIndex") == 0) {
    strcmp(request.command, "get") == 0
        ? sendHTTPAPIRequestStatus(request, true, SensorDHT.getHeatIndex())
        : sendHTTPAPIRequestStatus(request, false);
  } else if (strcmp(request.device, "api") == 0) {
    uint8_t _api =
        strcmp(request.name, "http") == 0
            ? API_HTTP
            : strcmp(request.name, "mqtt") == 0
                  ? API_MQTT
                  : strcmp(request.name, "domoticz") == 0 ? API_DOMOTICZ : 9;
    uint8_t _command = strcmp(request.command, "on") == 0
                           ? 1
                           : strcmp(request.command, "off") == 0 ? 0 : 9;

    if (_api != 9 && _command != 9) {
      Data.saveAPI(_api, _command);
      Device.begin();
      if (_command) {
        if (_api == API_MQTT) {
          MQTTInit();
        } else if (_api == API_DOMOTICZ) {
          DomoticzInit();
        }
      } else {
        if (_api == API_MQTT) {
          Mqtt.disconnect();
        } else if (_api == API_DOMOTICZ) {
          Domoticz.disconnect();
        }
      }
      sendHTTPAPIRequestStatus(request, true);
    } else {
      sendHTTPAPIRequestStatus(request, false);
    }
  } else if (strcmp(request.command, "reboot") == 0) { // reboot
    sendHTTPAPIRequestStatus(request, true);
    Device.reboot(Device.getMode());
  } else if (strcmp(request.command, "configurationMode") ==
             0) { // configurationMode
    sendHTTPAPIRequestStatus(request, true);
    Device.reboot(MODE_CONFIGURATION);
    /* No such device or commend not implemented */
  } else {
    sendHTTPAPIRequestStatus(request, false);
  }
}
