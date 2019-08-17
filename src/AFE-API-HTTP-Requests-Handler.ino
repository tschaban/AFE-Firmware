/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

/* Method listens for HTTP Requests */
void HTTPRequestListener() {
  if (Device.configuration.api.http) {
    if (WebServer.httpAPIlistener()) {
#if AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
      Led.on();
#endif
      processHTTPAPIRequest(WebServer.getHTTPCommand());
#if AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
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

void sendHTTPAPIRequestStatus(HTTPCOMMAND request, boolean status, double value,
                              uint8_t width = 2, uint8_t precision = 2) {
  char valueString[10];
  dtostrf(value, width, precision, valueString);
  sendHTTPAPIRequestStatus(request, status, valueString);
}

#ifdef AFE_CONFIG_HARDWARE_RELAY // Not required for T5
/* Method converts Relay value to string and invokes sendHTTPAPIRequestStatus
 * method which creates JSON respons and pushes it */
void sendHTTPAPIRelayRequestStatus(HTTPCOMMAND request, boolean status,
                                   byte value) {
  sendHTTPAPIRequestStatus(request, status, value == AFE_RELAY_ON ? "on" : "off");
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
#ifdef AFE_CONFIG_HARDWARE_GATE
/* It constructs HTTP response related to gate and calls HTTP push */
void sendHTTPAPIGateRequestStatus(HTTPCOMMAND request, boolean status,
                                  byte value) {
  sendHTTPAPIRequestStatus(
      request, status,
      value == AFE_GATE_OPEN
          ? "open"
          : value == AFE_GATE_CLOSED
                ? "closed"
                : value == AFE_GATE_PARTIALLY_OPEN ? "partiallyOpen" : "unknown");
}
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
/* It constructs HTTP response related to contactron and calls HTTP push */
void sendHTTPAPIContactronRequestStatus(HTTPCOMMAND request, boolean status,
                                        byte value) {
  sendHTTPAPIRequestStatus(request, status,
                           value == AFE_CONTACTRON_OPEN ? "open" : "closed");
}
#endif

/* Method processes HTTP API request */
void processHTTPAPIRequest(HTTPCOMMAND request) {

  boolean deviceNotExist = true;

#ifdef DEBUG
  Serial << endl << endl << "-------- Got HTTP Request --------";
  Serial << endl << "Device: " << request.device;
  Serial << endl << "Name: " << request.name;
  Serial << endl << "Command: " << request.command;
  Serial << endl << "----------------------------------" << endl;

#endif

#ifdef AFE_CONFIG_HARDWARE_GATE

  /* Request related to gate */
  if (strcmp(request.device, "gate") == 0) {
    for (uint8_t i = 0; i < Device.configuration.noOfGates; i++) {
      if (strcmp(request.name, Gate[i].configuration.name) == 0) {
        deviceNotExist = false;
        if (strcmp(request.command, "toggle") == 0) {
          Gate[i].toggle();
          sendHTTPAPIGateRequestStatus(request, true, Gate[i].get());
        } else if (strcmp(request.command, "get") == 0) { // get
          sendHTTPAPIGateRequestStatus(request, true, Gate[i].get());
        } else {
          sendHTTPAPIRequestStatus(request, false);
        }
        break;
      }
    }
    if (deviceNotExist) {
      sendHTTPAPIRequestStatus(request, false);
    }
    return;
  }
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  /* Request related to contactron */
  if (strcmp(request.device, "contactron") == 0) {
    for (uint8_t i = 0; i < Device.configuration.noOfContactrons; i++) {
      if (strcmp(request.name, Contactron[i].configuration.name) == 0) {
        deviceNotExist = false;
        if (strcmp(request.command, "get") == 0) {
          sendHTTPAPIContactronRequestStatus(request, true,
                                             Contactron[i].get());
        } else {
          sendHTTPAPIRequestStatus(request, false);
        }
        break;
      }
    }
    if (deviceNotExist) {
      sendHTTPAPIRequestStatus(request, false);
    }
    return;
  }

#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
  // @TODO should it be included for T5????
  /* Request related to relay */
  if (strcmp(request.device, "relay") == 0) {
    uint8_t state;
    for (uint8_t i = 0; i < Device.configuration.noOfRelays; i++) {
#ifdef AFE_CONFIG_HARDWARE_GATE
      /* For the Relay assigned to a gate code below is not needed for execution
       */
      if (Relay[i].gateId == AFE_HARDWARE_ITEM_NOT_EXIST) {
#endif
        if (strcmp(request.name, Relay[i].getName()) == 0) {
          deviceNotExist = false;
          if (strcmp(request.command, "on") == 0) {
            Relay[i].on();
            MQTTPublishRelayState(i); // MQTT Listener library
            if (strcmp(request.source, "domoticz") != 0) {
              DomoticzPublishRelayState(i);
            }
            sendHTTPAPIRelayRequestStatus(request, Relay[i].get() == AFE_RELAY_ON,
                                          Relay[i].get());

          } else if (strcmp(request.command, "off") == 0) { // Off
            Relay[i].off();
            MQTTPublishRelayState(i); // MQTT Listener library
            if (strcmp(request.source, "domoticz") != 0) {
              DomoticzPublishRelayState(i);
            }
            sendHTTPAPIRelayRequestStatus(request, Relay[i].get() == AFE_RELAY_OFF,
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
#ifdef AFE_CONFIG_FUNCTIONALITY_THERMOSTAT
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

#ifdef AFE_CONFIG_FUNCTIONALITY_HUMIDISTAT
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
          break;
        }

#ifdef AFE_CONFIG_HARDWARE_GATE
        /* Closing the condition for skipping relay if assigned to a gate */
      }
#ifdef DEBUG
      else {
        Serial << endl
               << "Excluding relay: " << i
               << " as it's assigned to a Gate: " << Relay[i].gateId;
      }
#endif
#endif
    }
    if (deviceNotExist) {
      sendHTTPAPIRequestStatus(request, false);
    }
    return;
  }
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
  /* Request related to ds18b20 */
  if (strcmp(request.device, "ds18b20") == 0) {
    strcmp(request.command, "get") == 0
        ? sendHTTPAPIRequestStatus(request, true, Sensor.getTemperature())
        : sendHTTPAPIRequestStatus(request, false);
  }
#endif

#ifdef AFE_CONFIG_HARDWARE_DHXX
  /* Request related to DHT Sensor */
  if (strcmp(request.device, "dht") == 0) {
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

#ifdef AFE_CONFIG_HARDWARE_PIR
  if (strcmp(request.device, "pir") == 0) {
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

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  /* Request related to gate */
  if (strcmp(request.device, "HPMA115S0") == 0) {
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
#endif

#ifdef AFE_CONFIG_HARDWARE_BMX80
  /* BMx80 */
  if (strcmp(request.device, "BMx80") == 0) {
    if (strcmp(request.command, "get") == 0) {
      BMx80_DATA sensorData;
      sensorData = BMx80Sensor.get();
      if (strcmp(request.name, "temperature") == 0) {
        sendHTTPAPIRequestStatus(request, true, sensorData.temperature);
      } else if (strcmp(request.name, "pressure") == 0) {
        sendHTTPAPIRequestStatus(request, true, sensorData.pressure);
      } else if (Device.configuration.isBMx80 != TYPE_BMP180_SENSOR &&
                 strcmp(request.name, "humidity") == 0) {
        sendHTTPAPIRequestStatus(request, true, sensorData.humidity);
      } else if (Device.configuration.isBMx80 != TYPE_BME680_SENSOR &&
                 strcmp(request.name, "gasResistance") == 0) {
        sendHTTPAPIRequestStatus(request, true, sensorData.gasResistance);
      } else {
        sendHTTPAPIRequestStatus(request, false);
      }
    } else {
      sendHTTPAPIRequestStatus(request, false);
    }
  }
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
  /* BH1750 */
  if (strcmp(request.device, "BH1750") == 0) {
    if (strcmp(request.name, "lux") == 0) {
      if (strcmp(request.command, "get") == 0) {
        float lux = BH1750Sensor.get();
        sendHTTPAPIRequestStatus(request, true, lux);
      } else {
        sendHTTPAPIRequestStatus(request, false);
      }
    }
  }
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  /* Analog Input */
  if (strcmp(request.device, "ADC") == 0) {
    if (strcmp(request.command, "get") == 0) {
      ADCINPUT_DATA data;
      if (strcmp(request.name, "raw") == 0) {
        sendHTTPAPIRequestStatus(request, true, data.raw);
      } else if (strcmp(request.name, "percent") == 0) {
        sendHTTPAPIRequestStatus(request, true, data.percent);
      } else if (strcmp(request.name, "voltage") == 0) {
        sendHTTPAPIRequestStatus(request, true, data.voltage, 3, 6);
      } else if (strcmp(request.name, "voltageCalculated") == 0) {
        sendHTTPAPIRequestStatus(request, true, data.voltageCalculated, 3, 6);
      } else {
        sendHTTPAPIRequestStatus(request, false);
      }
    } else {
      sendHTTPAPIRequestStatus(request, false);
    }
  }
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_API_CONTROL
  /* Requests related to APIs */
  if (strcmp(request.device, "api") == 0) {
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
  }
#endif

  if (strcmp(request.command, "reboot") == 0) { // reboot
    sendHTTPAPIRequestStatus(request, true);
    Device.reboot(Device.getMode());
  } else if (strcmp(request.command, "configurationMode") ==
             0) { // configurationMode
    PASSWORD password = Data.getPasswordConfiguration();
    if (!password.protect) {
      sendHTTPAPIRequestStatus(request, true);
      Device.reboot(AFE_MODE_CONFIGURATION);
    } else {
      sendHTTPAPIRequestStatus(request, false);
    }
    /* No such device or commend not implemented */
  } else {
    sendHTTPAPIRequestStatus(request, false);
  }
}
