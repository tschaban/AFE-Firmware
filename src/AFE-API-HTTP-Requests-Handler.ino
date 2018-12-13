/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

/* Method listens for HTTP Requests */
void mainHTTPRequestsHandler() {
  if (Device.configuration.httpAPI) {
    if (WebServer.httpAPIlistener()) {
#if !defined(T0_SHELLY_1_CONFIG)
      Led.on();
#endif
      processHTTPAPIRequest(WebServer.getHTTPCommand());
#if !defined(T0_SHELLY_1_CONFIG)
      Led.off();
#endif
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

#if !defined(T5_CONFIG) // Not required for T5
/* Method converts Relay value to string and invokes sendHTTPAPIRequestStatus
 * method which creates JSON respons and pushes it */
void sendHTTPAPIRelayRequestStatus(HTTPCOMMAND request, boolean status,
                                   byte value) {
  sendHTTPAPIRequestStatus(request, status, value == RELAY_ON ? "on" : "off");
}
#endif

/* PIR responses */
#if defined(T3_CONFIG)
void sendHTTPAPIPirRequestStatus(HTTPCOMMAND request, boolean status,
                                 boolean value) {
  sendHTTPAPIRequestStatus(request, status, value ? "open" : "closed");
}
#endif

/* Gate and Contactron responses */
#if defined(T5_CONFIG)
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
#endif

/* Method processes HTTP API request */
void processHTTPAPIRequest(HTTPCOMMAND request) {

#if defined(T5_CONFIG)

  /* Request related to gate */
  if (strcmp(request.device, "gate") == 0) {
    if (strcmp(request.command, "toggle") == 0) {
      Gate.toggle();
      sendHTTPAPIGateRequestStatus(request, true, Gate.get());
    } else if (strcmp(request.command, "get") == 0) { // get
      sendHTTPAPIGateRequestStatus(request, true, Gate.get());
    } else {
      sendHTTPAPIRequestStatus(request, false);
    }
  }
  /* Request relared to contactron */
  else if (strcmp(request.device, "contactron") == 0) {
    boolean noContactron = true;
    for (uint8_t i = 0; i < sizeof(Device.configuration.isContactron); i++) {
      if (Device.configuration.isContactron[i]) {
        if (strcmp(request.name, Gate.Contactron[i].getName()) == 0) {
          noContactron = false;
          if (strcmp(request.command, "get") == 0) {
            sendHTTPAPIContactronRequestStatus(request, true,
                                               Gate.Contactron[i].get());
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
  }

#elif defined(T6_CONFIG)

  /* Request related to gate */
  if (strcmp(request.device, "hpma115s0") == 0) {
    HPMA115S0_DATA sensorData;
    sensorData = ParticleSensor.get();
    if (strcmp(request.name, "PM2.5") == 0) {
      if (strcmp(request.command, "get") == 0) {
        sendHTTPAPIRequestStatus(request, true, (float)sensorData.pm25, 4, 0);
      }
    } else if (strcmp(request.name, "PM10") == 0) {
      if (strcmp(request.command, "get") == 0) {
        sendHTTPAPIRequestStatus(request, true, (float)sensorData.pm10, 4, 0);
      }
    }
  }
  /* BME680 */
  else if (strcmp(request.device, "bme680") == 0) {
    BME680_DATA sensorData;
    sensorData = BME680Sensor.get();
    if (strcmp(request.name, "temperature") == 0) {
      if (strcmp(request.command, "get") == 0) {
        sendHTTPAPIRequestStatus(request, true, sensorData.temperature);
      }
    } else if (strcmp(request.name, "humidity") == 0) {
      if (strcmp(request.command, "get") == 0) {
        sendHTTPAPIRequestStatus(request, true, sensorData.humidity);
      }
    } else if (strcmp(request.name, "pressure") == 0) {
      if (strcmp(request.command, "get") == 0) {
        sendHTTPAPIRequestStatus(request, true, sensorData.pressure);
      }
    } else if (strcmp(request.name, "gasResistance") == 0) {
      if (strcmp(request.command, "get") == 0) {
        sendHTTPAPIRequestStatus(request, true, sensorData.gasResistance);
      }
    }
  }

  /* BH1750 */
  else if (strcmp(request.device, "bh1750") == 0) {
    if (strcmp(request.name, "lux") == 0) {
      if (strcmp(request.command, "get") == 0) {
        float lux = BH1750Sensor.get();
        sendHTTPAPIRequestStatus(request, true, lux);
      }
    }
  }

#else
  /* Request related to relay */
  if (strcmp(request.device, "relay") == 0) {
    uint8_t state;
    boolean noRelay = true;
    for (uint8_t i = 0; i < sizeof(Device.configuration.isRelay); i++) {
      if (Device.configuration.isRelay[i]) {
        if (strcmp(request.name, Relay[i].getName()) == 0) {
          noRelay = false;
          if (strcmp(request.command, "on") == 0) {
            Relay[i].on();
            MQTTPublishRelayState(i); // MQTT Listener library
            if (strcmp(request.source, "domoticz") != 0) {
              DomoticzPublishRelayState(i);
            }
            sendHTTPAPIRelayRequestStatus(request, Relay[i].get() == RELAY_ON,
                                          Relay[i].get());

          } else if (strcmp(request.command, "off") == 0) { // Off
            Relay[i].off();
            MQTTPublishRelayState(i); // MQTT Listener library
            if (strcmp(request.source, "domoticz") != 0) {
              DomoticzPublishRelayState(i);
            }
            sendHTTPAPIRelayRequestStatus(request, Relay[i].get() == RELAY_OFF,
                                          Relay[i].get());
          } else if (strcmp(request.command, "toggle") == 0) { // toggle
            state = Relay[i].get();
            Relay[i].toggle();
            sendHTTPAPIRelayRequestStatus(request, state != Relay[i].get(),
                                          Relay[i].get());
            MQTTPublishRelayState(i); // MQTT Listener library
            if (strcmp(request.source, "domoticz") != 0) {
              DomoticzPublishRelayState(i);
            };
          } else if (strcmp(request.command, "get") == 0) {
            sendHTTPAPIRelayRequestStatus(request, true, Relay[i].get());
/* Command not implemented.Info */
#if defined(T1_CONFIG) || defined(T2_CONFIG)
          } else if (strcmp(request.command, "enableThermostat") == 0) {
            Relay[i].Thermostat.on();
            sendHTTPAPIRequestStatus(
                request, true, Relay[i].Thermostat.enabled() ? "on" : "off");
          } else if (strcmp(request.command, "disableThermostat") == 0) {
            Relay[i].Thermostat.off();
            sendHTTPAPIRequestStatus(
                request, true, Relay[i].Thermostat.enabled() ? "on" : "off");
          } else if (strcmp(request.command, "toggleThermostat") == 0) {
            Relay[i].Thermostat.enabled() ? Relay[i].Thermostat.off()
                                          : Relay[i].Thermostat.on();
            sendHTTPAPIRequestStatus(
                request, true, Relay[i].Thermostat.enabled() ? "on" : "off");
          } else if (strcmp(request.command, "getThermostat") == 0) {
            sendHTTPAPIRequestStatus(
                request, true, Relay[i].Thermostat.enabled() ? "on" : "off");

#endif

#if defined(T2_CONFIG)
          } else if (strcmp(request.command, "enableHumidistat") == 0) {
            Relay[i].Humidistat.on();
            sendHTTPAPIRequestStatus(
                request, true, Relay[i].Humidistat.enabled() ? "on" : "off");
          } else if (strcmp(request.command, "disableHumidistat") == 0) {
            Relay[i].Humidistat.off();
            sendHTTPAPIRequestStatus(
                request, true, Relay[i].Humidistat.enabled() ? "on" : "off");
          } else if (strcmp(request.command, "toggleHumidistat") == 0) {
            Relay[i].Humidistat.enabled() ? Relay[i].Humidistat.off()
                                          : Relay[i].Humidistat.on();
            sendHTTPAPIRequestStatus(
                request, true, Relay[i].Humidistat.enabled() ? "on" : "off");
          } else if (strcmp(request.command, "getHumidistat") == 0) {
            sendHTTPAPIRequestStatus(
                request, true, Relay[i].Humidistat.enabled() ? "on" : "off");

#endif

          } else {
            sendHTTPAPIRequestStatus(request, false);
          }
        }
      } else {
        break;
      }
    }
    if (noRelay) {
      sendHTTPAPIRequestStatus(request, false);
    }
  }
#endif

#if defined(T1_CONFIG)
  /* Request related to ds18b20 */
  else if (strcmp(request.device, "ds18b20") == 0) {
    strcmp(request.command, "get") == 0
        ? sendHTTPAPIRequestStatus(request, true, Sensor.getTemperature())
        : sendHTTPAPIRequestStatus(request, false);
  }
#endif

#if defined(T2_CONFIG) || defined(T5_CONFIG)
  /* Request related to DHT Sensor */
  else if (strcmp(request.device, "dht") == 0) {
    if (strcmp(request.name, "temperature") == 0) {
      strcmp(request.command, "get") == 0
          ? sendHTTPAPIRequestStatus(request, true, Sensor.getTemperature())
          : sendHTTPAPIRequestStatus(request, false);
    } else if (strcmp(request.name, "humidity") == 0) {
      strcmp(request.command, "get") == 0
          ? sendHTTPAPIRequestStatus(request, true, Sensor.getHumidity())
          : sendHTTPAPIRequestStatus(request, false);
    } else if (strcmp(request.name, "heatIndex") == 0) {
      strcmp(request.command, "get") == 0
          ? sendHTTPAPIRequestStatus(request, true, Sensor.getHeatIndex())
          : sendHTTPAPIRequestStatus(request, false);
    } else if (strcmp(request.name, "dewPoint") == 0) {
      strcmp(request.command, "get") == 0
          ? sendHTTPAPIRequestStatus(request, true, Sensor.getDewPoint())
          : sendHTTPAPIRequestStatus(request, false);
    } else {
      sendHTTPAPIRequestStatus(request, false);
    }
  }
#endif

#if defined(T3_CONFIG)
  else if (strcmp(request.device, "pir") == 0) {
    boolean pirSendFailure = true;
    for (uint8_t i = 0; i < sizeof(Device.configuration.isPIR); i++) {
      if (Device.configuration.isPIR[i]) {
        if (strcmp(request.name, Pir[i].getName()) == 0) {
          if (strcmp(request.command, "get") == 0) { // get
            pirSendFailure = false;
            sendHTTPAPIPirRequestStatus(request, true, Pir[i].motionDetected());
          }
        }
      }
    }
    if (pirSendFailure) {
      sendHTTPAPIRequestStatus(request, false);
    }
  }
#endif

  /* Requests related to APIs */
  else if (strcmp(request.device, "api") == 0) {
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
