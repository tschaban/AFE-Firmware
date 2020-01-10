/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-API-HTTP.h"

AFEAPIHTTP::AFEAPIHTTP() {}

/* Initializing class */
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
void AFEAPIHTTP::begin(AFEDevice *Device, AFEWebServer *WebServer,
                       AFEDataAccess *Data, AFEAPIMQTTDomoticz *MqttAPI,
                       AFEAPIHTTPDomoticz *HttpDomoticzAPI) {
  _Device = Device;
  if (_Device->configuration.api.http) {
    _Data = Data;
    _HTTP = WebServer;
    _MqttAPI = MqttAPI;
    _HttpAPIDomoticz = HttpDomoticzAPI;
    enabled = true;
  }
}
#else
void AFEAPIHTTP::begin(AFEDevice *Device, AFEWebServer *WebServer,
                       AFEDataAccess *Data, AFEAPIMQTTStandard *MqttAPI) {
  _Device = Device;
  if (_Device->configuration.api.http) {
    _Data = Data;
    _HTTP = WebServer;
    _MqttAPI = MqttAPI;
    enabled = true;
  }
}
#endif
/* Listening for HTTP requests - must be in loop() */
void AFEAPIHTTP::listener() {
  if (enabled) {
    if (_HTTP->httpAPIlistener()) {
      HTTPCOMMAND request = _HTTP->getHTTPCommand();
      processRequest(&request);
    }
  }
}

/* Processing HTTP request */
void AFEAPIHTTP::processRequest(HTTPCOMMAND *request) {

#ifdef DEBUG
  Serial << endl << endl << "-------- Got HTTP Request --------";
  Serial << endl << "Device: " << request->device;
  Serial << endl << "Name: " << request->name;
  Serial << endl << "Command: " << request->command;
  Serial << endl << "----------------------------------" << endl;
#endif

  if (false) {
    yield();
  }
/* Checking if Relay request */
#ifdef AFE_CONFIG_HARDWARE_RELAY
  else if (strcmp(request->device, "relay") == 0) {
#ifdef DEBUG
    Serial << endl << endl << "HTTP: Processing relay" << endl;
#endif
    processRelay(request);
  }
#endif
/* Checking if Analog Input request */
#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  else if (strcmp(request->device, "ADC") == 0) {
#ifdef DEBUG
    Serial << endl << endl << "HTTP: Processing ADC" << endl;
#endif
    processAnalogInput(request);
  }
#endif
  /* Checking if reboot command */
  else if (strcmp(request->command, "reboot") == 0) {
    send(request, true);
    _Device->reboot(_Device->getMode());
    /* Checking if configurationMode commad */
  } else if (strcmp(request->command, "configurationMode") == 0) {
    PASSWORD password = _Data->getPasswordConfiguration();
    if (!password.protect) {
      send(request, true);
      _Device->reboot(AFE_MODE_CONFIGURATION);
    } else {
      send(request, false, L_PASWORD_PROTECTED);
    }
    /* No such device or commend not implemented */
  } else {
    send(request, false, L_DEVICE_NOT_EXIST);
  }
#ifdef DEBUG
  Serial << endl << endl << "HTTP: Request processed" << endl;
#endif
}

#ifdef AFE_CONFIG_HARDWARE_RELAY
/* Adding pointer to the Relay class */
void AFEAPIHTTP::addClass(AFERelay *Relay) {
  for (uint8_t i = 0; i < _Device->configuration.noOfRelays; i++) {
    _Relay[i] = Relay + i;
  }
}

/* Processing relay requests */
void AFEAPIHTTP::processRelay(HTTPCOMMAND *request) {
  boolean deviceNotExist = true;

  for (uint8_t i = 0; i < _Device->configuration.noOfRelays; i++) {
#ifdef AFE_CONFIG_HARDWARE_GATE
    /* For the Relay assigned to a gate code below is not needed for execution
     */
    if (_Relay[i]->gateId == AFE_HARDWARE_ITEM_NOT_EXIST) {
#endif
      if (strcmp(request->name, _Relay[i]->configuration.name) == 0) {
        deviceNotExist = false;
        /* Checking if command: on */
        if (strcmp(request->command, "on") == 0) {
          _Relay[i]->on();
          sendRelayStatus(request, _Relay[i]->get() == AFE_RELAY_ON,
                          _Relay[i]->get());
          _MqttAPI->publishRelayState(i);

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
          if (_HttpAPIDomoticz->idxForProcessing(
                  _Relay[i]->configuration.domoticz.idx)) {
            _HttpAPIDomoticz->publishRelayState(i);
          }
#endif
          /* Checking if command: off */
        } else if (strcmp(request->command, "off") == 0) {
          _Relay[i]->off();
          sendRelayStatus(request, _Relay[i]->get() == AFE_RELAY_OFF,
                          _Relay[i]->get());
          _MqttAPI->publishRelayState(i);

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
          if (_HttpAPIDomoticz->idxForProcessing(
                  _Relay[i]->configuration.domoticz.idx)) {
            _HttpAPIDomoticz->publishRelayState(i);
          }
#endif
          /* Checking if command: toggle */
        } else if (strcmp(request->command, "toggle") == 0) {
          uint8_t state = _Relay[i]->get();
          _Relay[i]->toggle();
          sendRelayStatus(request, state != _Relay[i]->get(), _Relay[i]->get());
          _MqttAPI->publishRelayState(i);
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
          if (_HttpAPIDomoticz->idxForProcessing(
                  _Relay[i]->configuration.domoticz.idx)) {
            _HttpAPIDomoticz->publishRelayState(i);
          }
#endif
          /* Checking if command: get */
        } else if (strcmp(request->command, "get") == 0) {
          sendRelayStatus(request, true, _Relay[i]->get());

/* Command not implemented.Info */
#ifdef AFE_CONFIG_FUNCTIONALITY_THERMOSTAT
        } else if (strcmp(request->command, "enableThermostat") == 0) {
          Relay[i].Thermostat.on();
          sendHTTPAPIRequestStatus(
              request, true, Relay[i].Thermostat.enabled() ? "on" : "off");
        } else if (strcmp(request->command, "disableThermostat") == 0) {
          Relay[i].Thermostat.off();
          sendHTTPAPIRequestStatus(
              request, true, Relay[i].Thermostat.enabled() ? "on" : "off");
        } else if (strcmp(request->command, "toggleThermostat") == 0) {
          Relay[i].Thermostat.enabled() ? Relay[i].Thermostat.off()
                                        : Relay[i].Thermostat.on();
          sendHTTPAPIRequestStatus(
              request, true, Relay[i].Thermostat.enabled() ? "on" : "off");
        } else if (strcmp(request->command, "getThermostat") == 0) {
          sendHTTPAPIRequestStatus(
              request, true, Relay[i].Thermostat.enabled() ? "on" : "off");

#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_HUMIDISTAT
        } else if (strcmp(request->command, "enableHumidistat") == 0) {
          Relay[i].Humidistat.on();
          sendHTTPAPIRequestStatus(
              request, true, Relay[i].Humidistat.enabled() ? "on" : "off");
        } else if (strcmp(request->command, "disableHumidistat") == 0) {
          Relay[i].Humidistat.off();
          sendHTTPAPIRequestStatus(
              request, true, Relay[i].Humidistat.enabled() ? "on" : "off");
        } else if (strcmp(request->command, "toggleHumidistat") == 0) {
          Relay[i].Humidistat.enabled() ? Relay[i].Humidistat.off()
                                        : Relay[i].Humidistat.on();
          sendHTTPAPIRequestStatus(
              request, true, Relay[i].Humidistat.enabled() ? "on" : "off");
        } else if (strcmp(request->command, "getHumidistat") == 0) {
          sendHTTPAPIRequestStatus(
              request, true, Relay[i].Humidistat.enabled() ? "on" : "off");

#endif

        } else {
          send(request, false, L_COMMAND_NOT_IMPLEMENTED);
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
    send(request, false, L_DEVICE_NOT_EXIST);
  }
}

/* Formating call for send for Relay and send it */
void AFEAPIHTTP::sendRelayStatus(HTTPCOMMAND *request, boolean status,
                                 byte value) {
  send(request, status,
       value == AFE_RELAY_ON ? "{\"state\":\"on\"}" : "{\"state\":\"off\"}");
}

#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
/* Adding pointer to ADC class */
void AFEAPIHTTP::addClass(AFEAnalogInput *Analog) { _AnalogInput = Analog; }

/* Processing ADC Input request */
void AFEAPIHTTP::processAnalogInput(HTTPCOMMAND *request) {
  if (strcmp(request->command, "get") == 0) {
    char json[AFE_CONFIG_API_JSON_ADC_DATA_LENGTH];
    _AnalogInput->getJSON(json);
    send(request, true, json);
  } else {
    send(request, false, L_COMMAND_NOT_IMPLEMENTED);
  }
}

#endif // AFE_CONFIG_HARDWARE_ADC_VCC

/* Method creates JSON respons after processing HTTP API request, and pushes it.
 * The second one method converts float to charString before pushing response */
void AFEAPIHTTP::send(HTTPCOMMAND *request, boolean status, const char *value) {
  String respond;
  respond = "{\"device\":{";
  respond += "\"type\":\"" + String(request->device) + "\"";
  if (strlen(request->name) > 0) {
    respond += ",\"name\":\"" + String(request->name) + "\"";
  }
  respond += "},\"command\":\"" + String(request->command) + "\",";

  if (!strlen(value) == 0) {
    respond += "\"data\":";
    respond += value;
    respond += ",";
  }
  respond += "\"status\":\"";
  respond += status ? "success" : "error";
  respond += "\"}";
  _HTTP->sendJSON(respond);
}

void AFEAPIHTTP::send(HTTPCOMMAND *request, boolean status, double value,
                      uint8_t width, uint8_t precision) {
  char valueString[10];
  dtostrf(value, width, precision, valueString);
  send(request, status, valueString);
}
