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
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

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
  Serial << endl << endl << F("-------- Got HTTP Request --------");
  Serial << endl << F("Device: ") << request->device;
  Serial << endl << F("Name: ") << request->name;
  Serial << endl << F("Command: ") << request->command;
  Serial << endl << F("Source: ") << request->source;
  Serial << endl << F("----------------------------------") << endl;
#endif

  if (false) {
    yield();
  }
/* Checking if Relay request */
#ifdef AFE_CONFIG_HARDWARE_RELAY
  else if (strcmp(request->device, "relay") == 0) {
#ifdef DEBUG
    Serial << endl << F("INFO: Processing Relay HTTP requests");
#endif
    processRelay(request);
  }
#endif
/* Checking if Analog Input request */
#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  else if (strcmp(request->device, "ADC") == 0) {
#ifdef DEBUG
    Serial << endl << F("INFO: Processing ADC requests");
#endif
    processAnalogInput(request);
  }
#endif
/* Checking if Battery Meter request */
#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
  else if (strcmp(request->device, "batterymeter") == 0) {
#ifdef DEBUG
    Serial << endl << F("INFO: Processing ADC requests");
#endif
    processBatteryMeter(request);
  }
#endif
/* Checking if BMx80 Input request */
#ifdef AFE_CONFIG_HARDWARE_BMEX80
  // @TODO change in doc BMX80
  else if (strcmp(request->device, "BMEX80") == 0 ||
           strcmp(request->device, "BMX80") == 0) {
#ifdef DEBUG
    Serial << endl << F("INFO: Processing BMX80 requests");
#endif
    processBMEX80(request);
  }
#endif // AFE_CONFIG_HARDWARE_BMEX80
#ifdef AFE_CONFIG_HARDWARE_BH1750
  else if (strcmp(request->device, "BH1750") == 0) {
#ifdef DEBUG
    Serial << endl << F("INFO: Processing BH1750 requests");
#endif
    processBH1750(request);
  }
#endif // AFE_CONFIG_HARDWARE_BH1750
#ifdef AFE_CONFIG_HARDWARE_AS3935
  else if (strcmp(request->device, "AS3935") == 0) {
#ifdef DEBUG
    Serial << endl << F("INFO: Processing AS3935 requests");
#endif
    processAS3935(request);
  }
#endif // AFE_CONFIG_HARDWARE_AS3935
#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  else if (strcmp(request->device, "HPMA115S0") == 0) {
#ifdef DEBUG
    Serial << endl << F("INFO: Processing HPMA115S0 requests");
#endif
    processHPMA115S0(request);
  }
#endif // AFE_CONFIG_HARDWARE_HPMA115S0
/* Checking if Anemometer request */
#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER_SENSOR
  else if (strcmp(request->device, "anemometer") == 0) {
#ifdef DEBUG
    Serial << endl << F("INFO: Processing Anemometer requests");
#endif
    processAnemometerSensor(request);
  }
#endif // AFE_CONFIG_HARDWARE_ANEMOMETER_SENSOR
/* Checking if Rain request */
#ifdef AFE_CONFIG_HARDWARE_RAINMETER_SENSOR
  else if (strcmp(request->device, "rainmeter") == 0) {
#ifdef DEBUG
    Serial << endl << F("INFO: Processing Rainmeter requests");
#endif
    processRainSensor(request);
  }
#endif // AFE_CONFIG_HARDWARE_RAINMETER_SENSOR
#ifdef AFE_CONFIG_HARDWARE_GATE
  else if (strcmp(request->device, "gate") == 0) {
#ifdef DEBUG
    Serial << endl << F("INFO: Processing GATE requests");
#endif
    processGate(request);
  }
#endif // AFE_CONFIG_HARDWARE_GATE
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  else if (strcmp(request->device, "contactron") == 0) {
#ifdef DEBUG
    Serial << endl << F("INFO: Processing CONTACTRON requests");
#endif
    processContactron(request);
  }
#endif // AFE_CONFIG_HARDWARE_CONTACTRON

#ifdef AFE_CONFIG_HARDWARE_DS18B20
  else if (strcmp(request->device, "ds18b20") == 0) {
#ifdef DEBUG
    Serial << endl << F("INFO: Processing DS18B20 sensor requests");
#endif
    processDS18B20(request);
  }
#endif // AFE_CONFIG_HARDWARE_DS18B20

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  else if (strcmp(request->device, "regulator") == 0) {
#ifdef DEBUG
    Serial << endl << F("INFO: Processing Regulator requests");
#endif
    processRegulator(request);
  }
#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
  else if (strcmp(request->device, "thermalProtector") == 0) {
#ifdef DEBUG
    Serial << endl << F("INFO: Processing Thermal Protector requests");
#endif
    processThermalProtector(request);
  }
#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR

#ifdef AFE_CONFIG_HARDWARE_DHT
  else if (strcmp(request->device, "dht") == 0) {
#ifdef DEBUG
    Serial << endl << F("INFO: Processing DHT sensor requests");
#endif
    processDHT(request);
  }
#endif // AFE_CONFIG_HARDWARE_DS18B20

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
  else if (strcmp(request->device, "binary") == 0) {
#ifdef DEBUG
    Serial << endl << F("INFO: Processing Binary sensor requests");
#endif
    processBinarySensor(request);
  }
#endif // AFE_CONFIG_HARDWARE_BINARY_SENSOR

  /* Checking if reboot command */
  else if (strcmp(request->command, "reboot") == 0) {
    send(request, true);
    _Device->reboot(_Device->getMode());
    /* Checking if configurationMode commad */
  } else if (strcmp(request->command, "configurationMode") == 0) {
    PASSWORD password;
    _Data->getConfiguration(&password);
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
  Serial << endl << F("INFO: HTTP request processed");
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
          if (strcmp(request->source, "domoticz") != 0) {
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
          if (strcmp(request->source, "domoticz") != 0) {
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
          if (strcmp(request->source, "domoticz") != 0) {
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
             << F("Excluding relay: ") << i
             << F(" as it's assigned to a Gate: ") << _Relay[i]->gateId;
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

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
void AFEAPIHTTP::processBatteryMeter(HTTPCOMMAND *request) {
  if (strcmp(request->command, "get") == 0) {
    char json[AFE_CONFIG_API_JSON_BATTERYMETER_DATA_LENGTH];
    _AnalogInput->getBatteryMeterJSON(json);
    send(request, true, json);
  } else {
    send(request, false, L_COMMAND_NOT_IMPLEMENTED);
  }
}
#endif // AFE_CONFIG_FUNCTIONALITY_BATTERYMETER

#ifdef AFE_CONFIG_HARDWARE_BMEX80
void AFEAPIHTTP::addClass(AFESensorBMEX80 *Sensor) {
  for (uint8_t i = 0; i < _Device->configuration.noOfBMEX80s; i++) {
    _BMx80Sensor[i] = Sensor + i;
  }
}

void AFEAPIHTTP::processBMEX80(HTTPCOMMAND *request) {
  boolean deviceNotExist = true;
  for (uint8_t i = 0; i < _Device->configuration.noOfBMEX80s; i++) {
    if (strcmp(request->name, _BMx80Sensor[i]->configuration.name) == 0) {
      deviceNotExist = false;
      if (strcmp(request->command, "get") == 0) {
        char json[AFE_CONFIG_API_JSON_BMEX80_DATA_LENGTH];
        _BMx80Sensor[i]->getJSON(json);
        send(request, true, json);
      } else {
        send(request, false, L_COMMAND_NOT_IMPLEMENTED);
      }
    }
  }
  if (deviceNotExist) {
    send(request, false, L_DEVICE_NOT_EXIST);
  }
}
#endif // AFE_CONFIG_HARDWARE_BMEX80

#ifdef AFE_CONFIG_HARDWARE_BH1750
void AFEAPIHTTP::addClass(AFESensorBH1750 *Sensor) {
  for (uint8_t i = 0; i < _Device->configuration.noOfBH1750s; i++) {
    _BH1750Sensor[i] = Sensor + i;
  }
}
void AFEAPIHTTP::processBH1750(HTTPCOMMAND *request) {
  boolean deviceNotExist = true;

  for (uint8_t i = 0; i < _Device->configuration.noOfBH1750s; i++) {
    if (strcmp(request->name, _BH1750Sensor[i]->configuration.name) == 0) {
      deviceNotExist = false;
      if (strcmp(request->command, "get") == 0) {
        char json[AFE_CONFIG_API_JSON_BH1750_DATA_LENGTH];
        _BH1750Sensor[i]->getJSON(json);

        send(request, true, json);
      } else {
        send(request, false, L_COMMAND_NOT_IMPLEMENTED);
      }
    }
  }
  if (deviceNotExist) {
    send(request, false, L_DEVICE_NOT_EXIST);
  }
}
#endif // AFE_CONFIG_HARDWARE_BH1750

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
void AFEAPIHTTP::addClass(AFESensorHPMA115S0 *Sensor) {
  for (uint8_t i = 0; i < _Device->configuration.noOfHPMA115S0s; i++) {
    _HPMA115S0Sensor[i] = Sensor + i;
  }
}
void AFEAPIHTTP::processHPMA115S0(HTTPCOMMAND *request) {
  boolean deviceNotExist = true;
  for (uint8_t i = 0; i < _Device->configuration.noOfHPMA115S0s; i++) {
    if (strcmp(request->name, _HPMA115S0Sensor[i]->configuration.name) == 0) {
      deviceNotExist = false;
      if (strcmp(request->command, "get") == 0) {
        char json[AFE_CONFIG_API_JSON_HPMA115S0_DATA_LENGTH];
        _HPMA115S0Sensor[i]->getJSON(json);
        send(request, true, json);
      } else {
        send(request, false, L_COMMAND_NOT_IMPLEMENTED);
      }
    }
  }
  if (deviceNotExist) {
    send(request, false, L_DEVICE_NOT_EXIST);
  }
}
#endif // AFE_CONFIG_HARDWARE_HPMA115S0

#ifdef AFE_CONFIG_HARDWARE_AS3935
void AFEAPIHTTP::addClass(AFESensorAS3935 *Sensor) {
  for (uint8_t i = 0; i < _Device->configuration.noOfAS3935s; i++) {
    _AS3935Sensor[i] = Sensor + i;
  }
}
void AFEAPIHTTP::processAS3935(HTTPCOMMAND *request) {
  boolean deviceNotExist = true;
  for (uint8_t i = 0; i < _Device->configuration.noOfAS3935s; i++) {
    if (strcmp(request->name, _AS3935Sensor[i]->configuration.name) == 0) {
      deviceNotExist = false;
      if (strcmp(request->command, "get") == 0) {
        char json[AFE_CONFIG_API_JSON_AS3935_DATA_LENGTH]; // @TODO check the
                                                           // size
        _AS3935Sensor[i]->getJSON(json);
        send(request, true, json);
      } else {
        send(request, false, L_COMMAND_NOT_IMPLEMENTED);
      }
    }
  }
  if (deviceNotExist) {
    send(request, false, L_DEVICE_NOT_EXIST);
  }
}

#endif // AFE_CONFIG_HARDWARE_AS3935

/* Processing Anemometer Input request */
#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER_SENSOR

/* Adding pointer to Anemometer class */
void AFEAPIHTTP::addClass(AFESensorAnemometer *AnemometerSensor) {
  _AnemometerSensor = AnemometerSensor;
}

void AFEAPIHTTP::processAnemometerSensor(HTTPCOMMAND *request) {
  if (strcmp(request->command, "get") == 0) {
    char json[AFE_CONFIG_API_JSON_ANEMOMETER_DATA_LENGTH];
    _AnemometerSensor->getJSON(json);
    send(request, true, json);
  } else {
    send(request, false, L_COMMAND_NOT_IMPLEMENTED);
  }
}
#endif // AFE_CONFIG_HARDWARE_ANEMOMETER_SENSOR

/* Processing Rain Input request */
#ifdef AFE_CONFIG_HARDWARE_RAINMETER_SENSOR
/* Adding pointer to Rainclass */
void AFEAPIHTTP::addClass(AFESensorRainmeter *RainmeterSensor) {
  _RainmeterSensor = RainmeterSensor;
}

void AFEAPIHTTP::processRainSensor(HTTPCOMMAND *request) {
  if (strcmp(request->command, "get") == 0) {
    char json[AFE_CONFIG_API_JSON_RAINMETER_DATA_LENGTH];
    _RainmeterSensor->getJSON(json);
    send(request, true, json);
  } else {
    send(request, false, L_COMMAND_NOT_IMPLEMENTED);
  }
}
#endif // AFE_CONFIG_HARDWARE_RAINMETER_SENSOR

#ifdef AFE_CONFIG_HARDWARE_GATE
void AFEAPIHTTP::addClass(AFEGate *Item) {
  for (uint8_t i = 0; i < _Device->configuration.noOfGates; i++) {
    _Gate[i] = Item + i;
  }
}
void AFEAPIHTTP::processGate(HTTPCOMMAND *request) {
  boolean deviceNotExist = true;
  for (uint8_t i = 0; i < _Device->configuration.noOfGates; i++) {
    if (strcmp(request->name, _Gate[i]->configuration.name) == 0) {
      deviceNotExist = false;
      char json[AFE_CONFIG_API_JSON_GATE_DATA_LENGTH];
      if (strcmp(request->command, "toggle") == 0) {
        _Gate[i]->toggle();
        _MqttAPI->publishGateState(i);
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
        _HttpAPIDomoticz->publishGateState(i);
#endif
        _Gate[i]->getJSON(json);
        send(request, true, json);
      } else if (strcmp(request->command, "get") == 0) {
        _Gate[i]->getJSON(json);
        send(request, true, json);
      } else {
        send(request, false, L_COMMAND_NOT_IMPLEMENTED);
      }
    }
  }
  if (deviceNotExist) {
    send(request, false, L_DEVICE_NOT_EXIST);
  }
}
#endif // AFE_CONFIG_HARDWARE_GATE

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
void AFEAPIHTTP::addClass(AFEContactron *Item) {
  for (uint8_t i = 0; i < _Device->configuration.noOfContactrons; i++) {
    _Contactron[i] = Item + i;
  }
}
void AFEAPIHTTP::processContactron(HTTPCOMMAND *request) {
  boolean deviceNotExist = true;
  for (uint8_t i = 0; i < _Device->configuration.noOfContactrons; i++) {
    if (strcmp(request->name, _Contactron[i]->configuration.name) == 0) {
      deviceNotExist = false;
      if (strcmp(request->command, "get") == 0) {
        char json[AFE_CONFIG_API_JSON_CONTACTRON_DATA_LENGTH];
        _Contactron[i]->getJSON(json);
        send(request, true, json);
      } else {
        send(request, false, L_COMMAND_NOT_IMPLEMENTED);
      }
    }
  }
  if (deviceNotExist) {
    send(request, false, L_DEVICE_NOT_EXIST);
  }
}
#endif // AFE_CONFIG_HARDWARE_GATE

#ifdef AFE_CONFIG_HARDWARE_DS18B20
void AFEAPIHTTP::addClass(AFESensorDS18B20 *Sensor) {
  for (uint8_t i = 0; i < _Device->configuration.noOfDS18B20s; i++) {
    _DS18B20Sensor[i] = Sensor + i;
  }
}
void AFEAPIHTTP::processDS18B20(HTTPCOMMAND *request) {
  boolean deviceNotExist = true;

  for (uint8_t i = 0; i < _Device->configuration.noOfDS18B20s; i++) {
    if (strcmp(request->name, _DS18B20Sensor[i]->configuration.name) == 0) {
      deviceNotExist = false;
      if (strcmp(request->command, "get") == 0) {
        char json[AFE_CONFIG_API_JSON_DS18B20_DATA_LENGTH];
        _DS18B20Sensor[i]->getJSON(json);
        send(request, true, json);
      } else {
        send(request, false, L_COMMAND_NOT_IMPLEMENTED);
      }
    }
  }
  if (deviceNotExist) {
    send(request, false, L_DEVICE_NOT_EXIST);
  }
}
#endif // AFE_CONFIG_HARDWARE_DS18B20

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
void AFEAPIHTTP::addClass(AFERegulator *Regulator) {
  for (uint8_t i = 0; i < _Device->configuration.noOfRegulators; i++) {
    _Regulator[i] = Regulator + i;
  }
}
void AFEAPIHTTP::processRegulator(HTTPCOMMAND *request) {
  boolean deviceNotExist = true;

  for (uint8_t i = 0; i < _Device->configuration.noOfRegulators; i++) {
    if (strcmp(request->name, _Regulator[i]->configuration.name) == 0) {
      deviceNotExist = false;
      char json[AFE_CONFIG_API_JSON_REGULATOR_DATA_LENGTH];
      boolean sendJSON = true;
      if (strcmp(request->command, "get") == 0) {
        yield(); // JSON is sent (code below)
      } else if (strcmp(request->command, "on") == 0) {
        _Regulator[i]->on();
      } else if (strcmp(request->command, "off") == 0) {
        _Regulator[i]->off();
      } else if (strcmp(request->command, "toggle") == 0) {
        _Regulator[i]->toggle();
      } else {
        sendJSON = false;
      }
      if (sendJSON) {
        if (strcmp(request->command, "get") != 0) {
          _MqttAPI->publishRegulatorState(i);
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
          if (strcmp(request->source, "domoticz") != 0) {
            _HttpAPIDomoticz->publishRegulatorState(i);
          }
#endif
        }
        _Regulator[i]->getJSON(json);
        send(request, true, json);
      } else {
        send(request, false, L_COMMAND_NOT_IMPLEMENTED);
      }
    }
  }
  if (deviceNotExist) {
    send(request, false, L_DEVICE_NOT_EXIST);
  }
}
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
void AFEAPIHTTP::addClass(AFEThermalProtector *Protector) {
  for (uint8_t i = 0; i < _Device->configuration.noOfThermalProtectors; i++) {
    _ThermalProtector[i] = Protector + i;
  }
}
void AFEAPIHTTP::processThermalProtector(HTTPCOMMAND *request) {
  boolean deviceNotExist = true;

  for (uint8_t i = 0; i < _Device->configuration.noOfThermalProtectors; i++) {
    if (strcmp(request->name, _ThermalProtector[i]->configuration.name) == 0) {
      deviceNotExist = false;
      char json[AFE_CONFIG_API_JSON_THERMAL_PROTECTOR_DATA_LENGTH];
      boolean sendJSON = true;
      if (strcmp(request->command, "get") == 0) {
        yield(); // JSON is sent (code below)
      } else if (strcmp(request->command, "on") == 0) {
        _ThermalProtector[i]->on();
      } else if (strcmp(request->command, "off") == 0) {
        _ThermalProtector[i]->off();
      } else if (strcmp(request->command, "toggle") == 0) {
        _ThermalProtector[i]->toggle();
      } else {
        sendJSON = false;
      }
      if (sendJSON) {
        if (strcmp(request->command, "get") != 0) {
          _MqttAPI->publishRegulatorState(i);
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
          if (strcmp(request->source, "domoticz") != 0) {
            _HttpAPIDomoticz->publishRegulatorState(i);
          }
#endif
        }
        _ThermalProtector[i]->getJSON(json);
        send(request, true, json);
      } else {
        send(request, false, L_COMMAND_NOT_IMPLEMENTED);
      }
    }
  }
  if (deviceNotExist) {
    send(request, false, L_DEVICE_NOT_EXIST);
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_DHT
void AFEAPIHTTP::addClass(AFESensorDHT *Sensor) {
  for (uint8_t i = 0; i < _Device->configuration.noOfDHTs; i++) {
    _DHTSensor[i] = Sensor + i;
  }
}
void AFEAPIHTTP::processDHT(HTTPCOMMAND *request) {
  boolean deviceNotExist = true;

  for (uint8_t i = 0; i < _Device->configuration.noOfDHTs; i++) {
    if (strcmp(request->name, _DHTSensor[i]->configuration.name) == 0) {
      deviceNotExist = false;
      if (strcmp(request->command, "get") == 0) {
        char json[AFE_CONFIG_API_JSON_DHT_DATA_LENGTH];
        _DHTSensor[i]->getJSON(json);
        send(request, true, json);
      } else {
        send(request, false, L_COMMAND_NOT_IMPLEMENTED);
      }
    }
  }
  if (deviceNotExist) {
    send(request, false, L_DEVICE_NOT_EXIST);
  }
}
#endif // AFE_CONFIG_HARDWARE_DHT

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
void AFEAPIHTTP::addClass(AFESensorBinary *Sensor) {
  for (uint8_t i = 0; i < _Device->configuration.noOfBinarySensors; i++) {
    _BinarySensor[i] = Sensor + i;
  }
}
void AFEAPIHTTP::processBinarySensor(HTTPCOMMAND *request) {
  boolean deviceNotExist = true;

  for (uint8_t i = 0; i < _Device->configuration.noOfBinarySensors; i++) {
    if (strcmp(request->name, _BinarySensor[i]->configuration.name) == 0) {
      deviceNotExist = false;
      if (strcmp(request->command, "get") == 0) {
        char json[AFE_CONFIG_API_JSON_BINARY_SENSOR_DATA_LENGTH];
        _BinarySensor[i]->getJSON(json);
        send(request, true, json);
      } else {
        send(request, false, L_COMMAND_NOT_IMPLEMENTED);
      }
    }
  }

  if (deviceNotExist) {
    send(request, false, L_DEVICE_NOT_EXIST);
  }
}
#endif // AFE_CONFIG_HARDWARE_BINARY_SENSOR

/* Method creates JSON respons after processing HTTP API request, and pushes
 * it.
 * The second one method converts float to charString before pushing response
 */
void AFEAPIHTTP::send(HTTPCOMMAND *request, boolean status, const char *value) {
  String respond;
  respond.concat(FPSTR(JSON_RESPONSE));
  respond.replace("{{device.type}}", request->device);
  respond.replace("{{device.name}}",
                  strlen(request->name) > 0 ? request->name : "\"\"");
  respond.replace("{{request.command}}",
                  strlen(request->command) > 0 ? request->command : "\"\"");
  respond.replace("{{response.data}}", strlen(value) > 0 ? value : "\"\"");
  respond.replace("{{response.status}}", status ? "success" : "error");
  _HTTP->sendJSON(respond);
}

/* Not used - checked in T1
void AFEAPIHTTP::send(HTTPCOMMAND *request, boolean status, double value,
                      uint8_t width, uint8_t precision) {
  char valueString[10];
  dtostrf(value, width, precision, valueString);
  send(request, status, valueString);
}
*/