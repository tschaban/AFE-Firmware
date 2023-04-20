/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-API-HTTP.h"

AFEAPIHTTP::AFEAPIHTTP() {}

/* Initializing class */
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
void AFEAPIHTTP::begin(AFEFirmware *Firmware, AFEHardware *Hardware,
                       AFEWebServer *HTTPServer, AFEAPIMQTTDomoticz *MqttAPI,
                       AFEAPIHTTPDomoticz *HttpDomoticzAPI) {
  _Firmware = Firmware;
  if (_Firmware->Device->configuration.api.http) {
    _Hardware = Hardware;
    _HTTP = HTTPServer;
    _MqttAPI = MqttAPI;
    _HttpAPIDomoticz = HttpDomoticzAPI;
    enabled = true;
  }
}
#else
void AFEAPIHTTP::begin(AFEFirmware *Firmware, AFEHardware *Hardware,
                       AFEWebServer *HTTPServer, AFEAPIMQTTStandard *MqttAPI) {
  _Firmware = Firmware;
  if (_Firmware->Device->configuration.api.http) {
    _Hardware = Hardware;
    _HTTP = HTTPServer;
    _MqttAPI = MqttAPI;
    enabled = true;
  }
}
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

/* Listening for HTTP requests - must be in loop() */
void AFEAPIHTTP::listener() {
  if (enabled) {
    if (_HTTP->httpAPIlistener()) {
      processRequest(_HTTP->httpAPICommand);
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

  if (strlen(request->command) == 0) {
#ifdef DEBUG
    Serial << endl << F("WARN: Got empty request");
#endif
    send(request, false, L_COMMNAD_NO_COMMAND);
    return;
  }

/* Checking if Relay request */
#ifdef AFE_CONFIG_HARDWARE_RELAY
  if (strcmp(request->device, AFE_CONFIG_HTTP_DEVICE_RELAY) == 0) {
#ifdef DEBUG
    Serial << endl << F("INFO: Processing Relay HTTP requests");
#endif
    processRelay(request);
  }
#endif
/* Checking if Analog Input request */
#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
  else if (strcmp(request->device, AFE_CONFIG_HTTP_DEVICE_ANALOG_INPUT) == 0) {
#ifdef DEBUG
    Serial << endl << F("INFO: Processing ADC requests");
#endif
    processAnalogInput(request);
  }
#endif
/* Checking if Battery Meter request */
#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
  else if (strcmp(request->device, AFE_CONFIG_HTTP_DEVICE_BATTERY_METER) == 0) {
#ifdef DEBUG
    Serial << endl << F("INFO: Processing ADC requests");
#endif
    processBatteryMeter(request);
  }
#endif
/* Checking if BMx80 Input request */
#ifdef AFE_CONFIG_HARDWARE_BMEX80
  // @TODO T5 T6 change in doc BMX80
  else if (strcmp(request->device, AFE_CONFIG_HTTP_DEVICE_BMEX80) == 0 ||
           strcmp(request->device, AFE_CONFIG_HTTP_DEVICE_BMX80) == 0) {
#ifdef DEBUG
    Serial << endl << F("INFO: Processing BMX80 requests");
#endif
    processBMEX80(request);
  }
#endif // AFE_CONFIG_HARDWARE_BMEX80
#ifdef AFE_CONFIG_HARDWARE_BH1750
  else if (strcmp(request->device, AFE_CONFIG_HTTP_DEVICE_BH1750) == 0) {
#ifdef DEBUG
    Serial << endl << F("INFO: Processing BH1750 requests");
#endif
    processBH1750(request);
  }
#endif // AFE_CONFIG_HARDWARE_BH1750
#ifdef AFE_CONFIG_HARDWARE_AS3935
  else if (strcmp(request->device, AFE_CONFIG_HTTP_DEVICE_AS3935) == 0) {
#ifdef DEBUG
    Serial << endl << F("INFO: Processing AS3935 requests");
#endif
    processAS3935(request);
  }
#endif // AFE_CONFIG_HARDWARE_AS3935
#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  else if (strcmp(request->device, AFE_CONFIG_HTTP_DEVICE_HPMA115S0) == 0) {
#ifdef DEBUG
    Serial << endl << F("INFO: Processing HPMA115S0 requests");
#endif
    processHPMA115S0(request);
  }
#endif // AFE_CONFIG_HARDWARE_HPMA115S0
/* Checking if Anemometer request */
#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
  else if (strcmp(request->device, AFE_CONFIG_HTTP_DEVICE_ANEMOMETER) == 0) {
#ifdef DEBUG
    Serial << endl << F("INFO: Processing Anemometer requests");
#endif
    processAnemometerSensor(request);
  }
#endif // AFE_CONFIG_HARDWARE_ANEMOMETER
/* Checking if Rain request */
#ifdef AFE_CONFIG_HARDWARE_RAINMETER
  else if (strcmp(request->device, AFE_CONFIG_HTTP_DEVICE_RAINMETER) == 0) {
#ifdef DEBUG
    Serial << endl << F("INFO: Processing Rainmeter requests");
#endif
    processRainSensor(request);
  }
#endif // AFE_CONFIG_HARDWARE_RAINMETER
#ifdef AFE_CONFIG_HARDWARE_GATE
  else if (strcmp(request->device, AFE_CONFIG_HTTP_DEVICE_GATE) == 0) {
#ifdef DEBUG
    Serial << endl << F("INFO: Processing GATE requests");
#endif
    processGate(request);
  }
#endif // AFE_CONFIG_HARDWARE_GATE
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  else if (strcmp(request->device, AFE_CONFIG_HTTP_DEVICE_CONTACTRON) == 0) {
#ifdef DEBUG
    Serial << endl << F("INFO: Processing CONTACTRON requests");
#endif
    processContactron(request);
  }
#endif // AFE_CONFIG_HARDWARE_CONTACTRON

#ifdef AFE_CONFIG_HARDWARE_DS18B20
  else if (strcmp(request->device, AFE_CONFIG_HTTP_DEVICE_DS18B20) == 0) {
#ifdef DEBUG
    Serial << endl << F("INFO: Processing DS18B20 sensor requests");
#endif
    processDS18B20(request);
  }
#endif // AFE_CONFIG_HARDWARE_DS18B20

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  else if (strcmp(request->device, AFE_CONFIG_HTTP_DEVICE_REGULATOR) == 0) {
#ifdef DEBUG
    Serial << endl << F("INFO: Processing Regulator requests");
#endif
    processRegulator(request);
  }
#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
  else if (strcmp(request->device, AFE_CONFIG_HTTP_DEVICE_THERMAL_PROTECTOR) ==
           0) {
#ifdef DEBUG
    Serial << endl << F("INFO: Processing Thermal Protector requests");
#endif
    processThermalProtector(request);
  }
#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR

#ifdef AFE_CONFIG_HARDWARE_DHT
  else if (strcmp(request->device, AFE_CONFIG_HTTP_DEVICE_DHT) == 0) {
#ifdef DEBUG
    Serial << endl << F("INFO: Processing DHT sensor requests");
#endif
    processDHT(request);
  }
#endif // AFE_CONFIG_HARDWARE_DS18B20

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
  else if (strcmp(request->device, AFE_CONFIG_HTTP_DEVICE_BINARY_SENSOR) == 0) {
#ifdef DEBUG
    Serial << endl << F("INFO: Processing Binary sensor requests");
#endif
    processBinarySensor(request);
  }
#endif // AFE_CONFIG_HARDWARE_BINARY_SENSOR
#ifdef AFE_CONFIG_HARDWARE_TSL2561
  else if (strcmp(request->device, AFE_CONFIG_HTTP_DEVICE_TSL2561) == 0) {
#ifdef DEBUG
    Serial << endl << F("INFO: Processing TSL2561 requests");
#endif
    processTSL2561(request);
  }
#endif // AFE_CONFIG_HARDWARE_TSL2561

#ifdef AFE_CONFIG_HARDWARE_CLED
  else if (strcmp(request->device, AFE_CONFIG_HTTP_DEVICE_CLED) == 0) {
#ifdef DEBUG
    Serial << endl << F("INFO: Processing CLED requests");
#endif
    processCLED(request);
  } else if (strcmp(request->device, AFE_CONFIG_HTTP_DEVICE_CLED_EFFECT_WAVE) ==
             0) {
#ifdef DEBUG
    Serial << endl << F("INFO: Processing CLED Effect Wave requests");
#endif
    processCLEDEffect(request, AFE_CONFIG_HARDWARE_CLED_EFFECT_WAVE);
  } else if (strcmp(request->device,
                    AFE_CONFIG_HTTP_DEVICE_CLED_EFFECT_FADE_IN_OUT) == 0) {
#ifdef DEBUG
    Serial << endl << F("INFO: Processing CLED Effect FadeInOut requests");
#endif
    processCLEDEffect(request, AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT);
  } else if (strcmp(request->device,
                    AFE_CONFIG_HTTP_DEVICE_CLED_EFFECT_BLINKING) == 0) {
#ifdef DEBUG
    Serial << endl << F("INFO: Processing CLED Blinking requests");
#endif
    processCLEDEffect(request, AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING);
  }
#endif // AFE_CONFIG_HARDWARE_CLED
#ifdef AFE_CONFIG_HARDWARE_FS3000
  else if (strcmp(request->device, AFE_CONFIG_HTTP_DEVICE_FS3000) == 0) {
#ifdef DEBUG
    Serial << endl << F("INFO: Processing FS3000 requests");
#endif
    processFS3000(request);
  }
#endif // AFE_CONFIG_HARDWARE_TSL2561

  /* Checking if reboot command */
  else if (strcmp(request->command, AFE_CONFIG_HTTP_COMMAND_REBOOT) == 0) {
    send(request, true);
    _Firmware->Device->reboot(_Firmware->Device->getMode());
    /* Checking if configurationMode commad */
  } else if (strcmp(request->command,
                    AFE_CONFIG_HTTP_COMMAND_CONFIGURATION_MODE) == 0) {
    PASSWORD password;
    _Firmware->API->Flash->getConfiguration(&password);
    if (!password.protect) {
      send(request, true);
      _Firmware->Device->reboot(AFE_MODE_CONFIGURATION);
    } else {
      send(request, false, L_PASWORD_PROTECTED);
    }
    /* No such device or commend not implemented */
  } else {
    send(request, false, L_DEVICE_NOT_EXIST);
  }
#ifdef DEBUG
  Serial << endl << F("INFO: HTTP Server: Request processed");
#endif
}

#ifdef AFE_CONFIG_HARDWARE_RELAY

/* Processing relay requests */
void AFEAPIHTTP::processRelay(HTTPCOMMAND *request) {
  boolean deviceNotExist = true;
  boolean _relayStateUpdated = false;
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfRelays; i++) {
#ifdef AFE_CONFIG_HARDWARE_GATE
    /* For the Relay assigned to a gate code below is not needed for execution
     */
    if (_Hardware->Relay[i]->gateId == AFE_HARDWARE_ITEM_NOT_EXIST) {
#endif
      if (strcmp(request->name, _Hardware->Relay[i]->configuration->name) ==
          0) {
        deviceNotExist = false;
        /* Checking if command: on */
        if (strcmp(request->command, "on") == 0) {
          _Hardware->Relay[i]->on();
          sendOnOffStatus(request, _Hardware->Relay[i]->get() == AFE_RELAY_ON,
                          _Hardware->Relay[i]->get() == AFE_RELAY_ON ? AFE_ON
                                                                     : AFE_OFF);

          _relayStateUpdated = true;
          /* Checking if command: off */
        } else if (strcmp(request->command, "off") == 0) {
          _Hardware->Relay[i]->off();
          sendOnOffStatus(request, _Hardware->Relay[i]->get() == AFE_RELAY_OFF,
                          _Hardware->Relay[i]->get());
          _relayStateUpdated = true;

          /* Checking if command: toggle */
        } else if (strcmp(request->command, "toggle") == 0) {
          uint8_t state = _Hardware->Relay[i]->get();
          _Hardware->Relay[i]->toggle();
          sendOnOffStatus(request, state != _Hardware->Relay[i]->get(),
                          _Hardware->Relay[i]->get());
          _relayStateUpdated = true;

          /* Checking if command: get */
        } else if (strcmp(request->command, "get") == 0) {
          sendOnOffStatus(request, true, _Hardware->Relay[i]->get());
          _relayStateUpdated = false;

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

          if (_relayStateUpdated) {
            _MqttAPI->publishRelayState(i);

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
            if (strcmp(request->source, "domoticz") != 0) {
              _HttpAPIDomoticz->publishRelayState(i);
            }
#endif
          }

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
             << F(" as it's assigned to a Gate: ")
             << _Hardware->Relay[i]->gateId;
    }
#endif
#endif
  }

  if (deviceNotExist) {
    send(request, false, L_DEVICE_NOT_EXIST);
  }
}

#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
/* Processing ADC Input request */
void AFEAPIHTTP::processAnalogInput(HTTPCOMMAND *request) {
#ifdef AFE_ESP32
  boolean deviceNotExist = true;
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfAnalogInputs;
       i++) {
    if (strcmp(request->name, _Hardware->AnalogInput[i]->configuration->name) ==
        0) {
      deviceNotExist = false;
      if (strcmp(request->command, "get") == 0) {
        char json[AFE_CONFIG_API_JSON_ADC_DATA_LENGTH];
        _Hardware->AnalogInput[i]->getJSON(json);
        send(request, true, json);
      } else {
        send(request, false, L_COMMAND_NOT_IMPLEMENTED);
      }
    }
  }
  if (deviceNotExist) {
    send(request, false, L_DEVICE_NOT_EXIST);
  }
#else  // ESP8266
  if (strcmp(request->command, "get") == 0) {
    char json[AFE_CONFIG_API_JSON_ADC_DATA_LENGTH];
    _Hardware->AnalogInput->getJSON(json);
    send(request, true, json);
  } else {
    send(request, false, L_COMMAND_NOT_IMPLEMENTED);
  }
#endif // AFE_ESP32
}
#endif // AFE_CONFIG_HARDWARE_ANALOG_INPUT

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
void AFEAPIHTTP::processBatteryMeter(HTTPCOMMAND *request) {
#ifdef AFE_ESP32
  boolean deviceNotExist = true;
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfAnalogInputs;
       i++) {
    if (strcmp(request->name, _Hardware->AnalogInput[i]->configuration->name) ==
        0) {
      deviceNotExist = false;
      if (strcmp(request->command, "get") == 0) {
        char json[AFE_CONFIG_API_JSON_BATTERYMETER_DATA_LENGTH];
        _Hardware->AnalogInput[i]->getBatteryMeterJSON(json);
        send(request, true, json);
      } else {
        send(request, false, L_COMMAND_NOT_IMPLEMENTED);
      }
    }
  }
  if (deviceNotExist) {
    send(request, false, L_DEVICE_NOT_EXIST);
  }
#else // ESP8266
  if (strcmp(request->command, "get") == 0) {
    char json[AFE_CONFIG_API_JSON_BATTERYMETER_DATA_LENGTH];
    _Hardware->AnalogInput->getBatteryMeterJSON(json);
    send(request, true, json);
  } else {
    send(request, false, L_COMMAND_NOT_IMPLEMENTED);
  }
#endif
}
#endif // AFE_CONFIG_FUNCTIONALITY_BATTERYMETER

#ifdef AFE_CONFIG_HARDWARE_BMEX80

void AFEAPIHTTP::processBMEX80(HTTPCOMMAND *request) {
  boolean deviceNotExist = true;
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfBMEX80s; i++) {
    if (strcmp(request->name,
               _Hardware->BMEX80Sensor[i]->configuration->name) == 0) {
      deviceNotExist = false;
      if (strcmp(request->command, "get") == 0) {
        char json[AFE_CONFIG_API_JSON_BMEX80_DATA_LENGTH];
        _Hardware->BMEX80Sensor[i]->getJSON(json);
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
void AFEAPIHTTP::processBH1750(HTTPCOMMAND *request) {
  boolean deviceNotExist = true;

  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfBH1750s; i++) {
    if (strcmp(request->name,
               _Hardware->BH1750Sensor[i]->configuration->name) == 0) {
      deviceNotExist = false;
      if (strcmp(request->command, "get") == 0) {
        char json[AFE_CONFIG_API_JSON_BH1750_DATA_LENGTH];
        _Hardware->BH1750Sensor[i]->getJSON(json);

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

#ifdef AFE_CONFIG_HARDWARE_TSL2561
void AFEAPIHTTP::processTSL2561(HTTPCOMMAND *request) {
  boolean deviceNotExist = true;

  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfTSL2561s; i++) {
    if (strcmp(request->name,
               _Hardware->TSL2561Sensor[i]->configuration->name) == 0) {
      deviceNotExist = false;
      if (strcmp(request->command, "get") == 0) {
        char json[AFE_CONFIG_API_JSON_TSL2561_DATA_LENGTH];
        _Hardware->TSL2561Sensor[i]->getJSON(json);
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
#endif // AFE_CONFIG_HARDWARE_TSL2561

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
void AFEAPIHTTP::processHPMA115S0(HTTPCOMMAND *request) {
  boolean deviceNotExist = true;
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfHPMA115S0s;
       i++) {
    if (strcmp(request->name,
               _Hardware->HPMA115S0Sensor[i]->configuration->name) == 0) {
      deviceNotExist = false;
      if (strcmp(request->command, "get") == 0) {
        char json[AFE_CONFIG_API_JSON_HPMA115S0_DATA_LENGTH];
        _Hardware->HPMA115S0Sensor[i]->getJSON(json);
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

void AFEAPIHTTP::processAS3935(HTTPCOMMAND *request) {
  boolean deviceNotExist = true;
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfAS3935s; i++) {
    if (strcmp(request->name,
               _Hardware->AS3935Sensornsor[i]->configuration.name) == 0) {
      deviceNotExist = false;
      if (strcmp(request->command, "get") == 0) {
        char json[AFE_CONFIG_API_JSON_AS3935_DATA_LENGTH]; // @TODO check the
                                                           // size
        _Hardware->AS3935Sensornsor[i]->getJSON(json);
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
#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
void AFEAPIHTTP::processAnemometerSensor(HTTPCOMMAND *request) {
  if (strcmp(request->command, "get") == 0) {
    char json[AFE_CONFIG_API_JSON_ANEMOMETER_DATA_LENGTH];
    _Hardware->AnemometerSensor->getJSON(json);
    send(request, true, json);
  } else {
    send(request, false, L_COMMAND_NOT_IMPLEMENTED);
  }
}
#endif // AFE_CONFIG_HARDWARE_ANEMOMETER

/* Processing Rain Input request */
#ifdef AFE_CONFIG_HARDWARE_RAINMETER
void AFEAPIHTTP::processRainSensor(HTTPCOMMAND *request) {
  if (strcmp(request->command, "get") == 0) {
    char json[AFE_CONFIG_API_JSON_RAINMETER_DATA_LENGTH];
    _Hardware->RainmeterSensor->getJSON(json);
    send(request, true, json);
  } else {
    send(request, false, L_COMMAND_NOT_IMPLEMENTED);
  }
}
#endif // AFE_CONFIG_HARDWARE_RAINMETER

#ifdef AFE_CONFIG_HARDWARE_GATE
void AFEAPIHTTP::processGate(HTTPCOMMAND *request) {
  boolean deviceNotExist = true;
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfGates; i++) {
    if (strcmp(request->name, _Hardware->Gate[i]->configuration->name) == 0) {
      deviceNotExist = false;
      char json[AFE_CONFIG_API_JSON_GATE_DATA_LENGTH];
      if (strcmp(request->command, "toggle") == 0) {
        _Hardware->Gate[i]->toggle();
        _MqttAPI->publishGateState(i);
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
        _HttpAPIDomoticz->publishGateState(i);
#endif
        _Hardware->Gate[i]->getJSON(json);
        send(request, true, json);
      } else if (strcmp(request->command, "get") == 0) {
        _Hardware->Gate[i]->getJSON(json);
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

void AFEAPIHTTP::processContactron(HTTPCOMMAND *request) {
  boolean deviceNotExist = true;
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfContactrons;
       i++) {
    if (strcmp(request->name, _Hardware->Contactron[i]->configuration->name) ==
        0) {
      deviceNotExist = false;
      if (strcmp(request->command, "get") == 0) {
        char json[AFE_CONFIG_API_JSON_CONTACTRON_DATA_LENGTH];
        _Hardware->Contactron[i]->getJSON(json);
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
#endif // AFE_CONFIG_HARDWARE_CONTACTRON

#ifdef AFE_CONFIG_HARDWARE_DS18B20
void AFEAPIHTTP::processDS18B20(HTTPCOMMAND *request) {
  boolean deviceNotExist = true;

  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfDS18B20s; i++) {
    if (strcmp(request->name,
               _Hardware->DS18B20Sensor[i]->configuration->name) == 0) {
      deviceNotExist = false;
      if (strcmp(request->command, "get") == 0) {
        char json[AFE_CONFIG_API_JSON_DS18B20_DATA_LENGTH];
        _Hardware->DS18B20Sensor[i]->getJSON(json);
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

void AFEAPIHTTP::processRegulator(HTTPCOMMAND *request) {
  boolean deviceNotExist = true;

  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfRegulators;
       i++) {
    if (strcmp(request->name, _Hardware->Regulator[i]->configuration->name) ==
        0) {
      deviceNotExist = false;
      char json[AFE_CONFIG_API_JSON_REGULATOR_DATA_LENGTH];
      boolean sendJSON = true;
      if (strcmp(request->command, "get") == 0) {
        yield(); // JSON is sent (code below)
      } else if (strcmp(request->command, "on") == 0) {
        _Hardware->Regulator[i]->on();
      } else if (strcmp(request->command, "off") == 0) {
        _Hardware->Regulator[i]->off();
      } else if (strcmp(request->command, "toggle") == 0) {
        _Hardware->Regulator[i]->toggle();
      } else {
        sendJSON = false;
      }
      if (sendJSON) {
        if (strcmp(request->command, "get") != 0) {
          _MqttAPI->publishRegulatorState(i);
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
          if (strcmp(request->source, "domoticz") != 0) {
            _HttpAPIDomoticz->publishRegulatorState(i);
          }
#endif
        }
        _Hardware->Regulator[i]->getJSON(json);
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
void AFEAPIHTTP::processThermalProtector(HTTPCOMMAND *request) {
  boolean deviceNotExist = true;

  for (uint8_t i = 0;
       i < _Firmware->Device->configuration.noOfThermalProtectors; i++) {
    if (strcmp(request->name,
               _Hardware->ThermalProtector[i]->configuration->name) == 0) {
      deviceNotExist = false;
      char json[AFE_CONFIG_API_JSON_THERMAL_PROTECTOR_DATA_LENGTH];
      boolean sendJSON = true;
      if (strcmp(request->command, "get") == 0) {
        yield(); // JSON is sent (code below)
      } else if (strcmp(request->command, "on") == 0) {
        _Hardware->ThermalProtector[i]->on();
      } else if (strcmp(request->command, "off") == 0) {
        _Hardware->ThermalProtector[i]->off();
      } else if (strcmp(request->command, "toggle") == 0) {
        _Hardware->ThermalProtector[i]->toggle();
      } else {
        sendJSON = false;
      }
      if (sendJSON) {
        if (strcmp(request->command, "get") != 0) {
          _MqttAPI->publishRegulatorState(i);
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
          if (strcmp(request->source, "domoticz") != 0) {
            _HttpAPIDomoticz->publishRegulatorState(i);
          }
#endif
        }
        _Hardware->ThermalProtector[i]->getJSON(json);
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
void AFEAPIHTTP::processDHT(HTTPCOMMAND *request) {
  boolean deviceNotExist = true;

  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfDHTs; i++) {
    if (strcmp(request->name, _Hardware->DHTSensor[i]->configuration->name) ==
        0) {
      deviceNotExist = false;
      if (strcmp(request->command, "get") == 0) {
        char json[AFE_CONFIG_API_JSON_DHT_DATA_LENGTH];
        _Hardware->DHTSensor[i]->getJSON(json);
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

void AFEAPIHTTP::processBinarySensor(HTTPCOMMAND *request) {
  boolean deviceNotExist = true;

  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfBinarySensors;
       i++) {
    if (strcmp(request->name,
               _Hardware->BinarySensor[i]->configuration->name) == 0) {
      deviceNotExist = false;
      if (strcmp(request->command, "get") == 0) {
        char json[AFE_CONFIG_API_JSON_BINARY_SENSOR_DATA_LENGTH];
        _Hardware->BinarySensor[i]->getJSON(json);
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

#ifdef AFE_CONFIG_HARDWARE_CLED

/* Processing CLED requests */
void AFEAPIHTTP::processCLED(HTTPCOMMAND *request) {
  boolean deviceNotExist = true;
  boolean _stateUpdated = true;
  boolean _sendStatus = true;

  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfCLEDs; i++) {
    if (strcmp(request->name, _Hardware->RGBLEDStrip->configuration[i].name) ==
        0) {
      deviceNotExist = false;
      _stateUpdated = true;
      /* Checking if command: on */
      if (strcmp(request->command, "on") == 0) {
        _Hardware->RGBLEDStrip->on(i, true);
        /* Checking if command: off */
      } else if (strcmp(request->command, "off") == 0) {
        _Hardware->RGBLEDStrip->off(i, true);
        /* Checking if command: toggle */
      } else if (strcmp(request->command, "toggle") == 0) {
        _Hardware->RGBLEDStrip->toggle(i, true);
        /* Checking if command: get */
      } else if (strcmp(request->command, "get") == 0) {
        _stateUpdated = false;
      } else {
        _sendStatus = false;
        send(request, false, L_COMMAND_NOT_IMPLEMENTED);
      }
      if (_sendStatus) {
        sendOnOffStatus(
            request, true,
            _Hardware->RGBLEDStrip->currentState[i].state ? AFE_ON : AFE_OFF);
        if (_stateUpdated) {
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
          if (_Hardware->CLED->isEffectStateUpdated(i)) {
            //_MqttAPI->publishCLEDEffectsState(id);
            //   _HttpAPIDomoticz->publishCLEDEffectState(i);
          }
#else
          publishCLEDStates(i);
#endif
        }
      }
      break;
    }
  }
  if (deviceNotExist) {
    send(request, false, L_DEVICE_NOT_EXIST);
  }
}

void AFEAPIHTTP::processCLEDEffect(HTTPCOMMAND *request, uint8_t effectId) {
  boolean deviceNotExist = true;
  boolean _stateUpdated = true;
  boolean _sendStatus = true;

  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfCLEDs; i++) {
    if (strcmp(
            request->name,
            effectId == AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING
                ? _Hardware->RGBLEDStrip->configurationEffectBlinking[i].name
                : effectId == AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT
                      ? _Hardware->RGBLEDStrip->configurationEffectFadeInOut[i]
                            .name
                      : _Hardware->RGBLEDStrip->configurationEffectWave[i]
                            .name) == 0) {
      deviceNotExist = false;
      _stateUpdated = true;
      /* Checking if command: on */
      if (strcmp(request->command, "on") == 0) {
        _Hardware->RGBLEDStrip->activateEffect(i, effectId);
        /* Checking if command: off */
      } else if (strcmp(request->command, "off") == 0) {
        _Hardware->RGBLEDStrip->deactivateEffect(i, effectId);
        /* Checking if command: get */
      } else if (strcmp(request->command, "get") == 0) {
        _stateUpdated = false;
      } else {
        _sendStatus = false;
        send(request, false, L_COMMAND_NOT_IMPLEMENTED);
      }
      if (_sendStatus) {
        sendOnOffStatus(request, true,
                        _Hardware->RGBLEDStrip->currentState[i].effect.id ==
                                effectId
                            ? AFE_ON
                            : AFE_OFF);
        if (_stateUpdated) {
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
          // publishCLEDStates(i, strcmp(request->source, "domoticz") == 0);
          if (_Hardware->CLED->isStateUpdated(i)) {
            //    _HttpAPIDomoticz->publishCLEDState(i);
          }
#else
          publishCLEDStates(i);
#endif
        }
      }
      break;
    }
  }
  if (deviceNotExist) {
    send(request, false, L_DEVICE_NOT_EXIST);
  }
}

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
void AFEAPIHTTP::publishCLEDStates(uint8_t id, boolean fromDomoticz) {
  if (_Hardware->CLED->isStateUpdated(id)) {
    _MqttAPI->publishCLEDState(id);

    /* RGB LED are not supported in HTTP Domoticz API
        if (!fromDomoticz) {
          _HttpAPIDomoticz->publishCLEDState(id);
        }
    */
  }
  if (_Hardware->CLED->isEffectStateUpdated(id)) {
    _MqttAPI->publishCLEDEffectsState(id);
    /* RGB LED are not supported in HTTP Domoticz API
    if (!fromDomoticz) {
      _HttpAPIDomoticz->publishCLEDEffectState(id);
    }
    */
  }
}
#else
void AFEAPIHTTP::publishCLEDStates(uint8_t id) {
  if (_Hardware->RGBLEDStrip->isStateUpdated(id)) {
    _MqttAPI->publishCLEDState(id);
  }
  if (_Hardware->RGBLEDStrip->isEffectStateUpdated(id)) {
    _MqttAPI->publishCLEDEffectsState(id);
  }
}
#endif

#endif // AFE_CONFIG_HARDWARE_CLED

#ifdef AFE_CONFIG_HARDWARE_FS3000
void AFEAPIHTTP::processFS3000(HTTPCOMMAND *request) {
  boolean deviceNotExist = true;

  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfFS3000s; i++) {
    if (strcmp(request->name,
               _Hardware->FS3000Sensor[i]->configuration->name) == 0) {
      deviceNotExist = false;
      if (strcmp(request->command, "get") == 0) {
        char json[AFE_CONFIG_API_JSON_FS3000_DATA_LENGTH];
        _Hardware->FS3000Sensor[i]->getJSON(json);
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
#endif // AFE_CONFIG_HARDWARE_FS3000

#if defined(AFE_CONFIG_HARDWARE_RELAY) || defined(AFE_CONFIG_HARDWARE_CLED)
void AFEAPIHTTP::sendOnOffStatus(HTTPCOMMAND *request, boolean status,
                                 byte value) {
  send(request, status,
       value == AFE_ON ? "{\"state\":\"on\"}" : "{\"state\":\"off\"}");
}
#endif

void AFEAPIHTTP::send(HTTPCOMMAND *request, boolean status, const char *value) {
  String respond;
  respond.concat(FPSTR(JSON_RESPONSE));
  respond.replace(F("{{device.type}}"), request->device);
  respond.replace(F("{{device.name}}"),
                  strlen(request->name) > 0 ? request->name : "");
  respond.replace(F("{{request.command}}"),
                  strlen(request->command) > 0 ? request->command : "");
  respond.replace(F("{{response.data}}"), strlen(value) > 0 ? value : "\"\"");
  respond.replace(F("{{response.status}}"), status ? F("success") : F("error"));
  _HTTP->sendJSON(respond);
}
