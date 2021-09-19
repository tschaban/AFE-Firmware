/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-API-MQTT-Domoticz.h"

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED

AFEAPIMQTTDomoticz::AFEAPIMQTTDomoticz() : AFEAPI(){};

#ifdef AFE_CONFIG_HARDWARE_LED
void AFEAPIMQTTDomoticz::begin(AFEDataAccess *Data, AFEDevice *Device,
                               AFELED *Led) {
  AFEAPI::begin(Data, Device, Led);
#ifdef DEBUG
  Serial << endl
         << F("INFO: Domoticz version: ")
         << (Device->configuration.api.domoticzVersion == AFE_DOMOTICZ_VERSION_0
                 ? L_DEVICE_DOMOTICZ_VERSION_410
                 : L_DEVICE_DOMOTICZ_VERSION_2020);
#endif
}
#else
void AFEAPIMQTTDomoticz::begin(AFEDataAccess *Data, AFEDevice *Device) {
  AFEAPI::begin(Data, Device);
}
#endif // AFE_CONFIG_HARDWARE_LED

void AFEAPIMQTTDomoticz::listener() {
  if (Mqtt.listener()) {
    processRequest();
  }
}

void AFEAPIMQTTDomoticz::synchronize() {
#ifdef DEBUG
  Serial << endl << F("INFO: Sending current device state to MQTT Broker  ...");
#endif

/* Synchronize: Relay */
#ifdef AFE_CONFIG_HARDWARE_RELAY
  for (uint8_t i = 0; i < _Device->configuration.noOfRelays; i++) {
#ifdef DEBUG
    Serial << endl << F("INFO: Synchronizing RELAY: ") << i;
#endif
    publishRelayState(i);
  }
#endif

/* Synchronize: Switch */
#ifdef AFE_CONFIG_HARDWARE_SWITCH
  for (uint8_t i = 0; i < _Device->configuration.noOfSwitches; i++) {
#ifdef DEBUG
    Serial << endl << F("INFO: Synchronizing SWITCH: ") << i;
#endif
    publishSwitchState(i);
  }
#endif

/* Synchronize: Contactron */
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  for (uint8_t i = 0; i < _Device->configuration.noOfContactrons; i++) {
#ifdef DEBUG
    Serial << endl << F("INFO: Synchronizing CONTACTRON: ") << i;
#endif
    publishContactronState(i);
  }
#endif

/* Synchronize: Gate */
#ifdef AFE_CONFIG_HARDWARE_GATE
  for (uint8_t i = 0; i < _Device->configuration.noOfGates; i++) {
#ifdef DEBUG
    Serial << endl << F("INFO: Synchronizing GATE: ") << i;
#endif
    publishGateState(i);
  }
#endif

/* Synchronize: Regulator */
#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  for (uint8_t i = 0; i < _Device->configuration.noOfRegulators; i++) {
#ifdef DEBUG
    Serial << endl << F("INFO: Synchronizing Regulator: ") << i;
#endif
    publishRegulatorState(i);
  }
#endif

/* Synchronize: Regulator */
#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
  for (uint8_t i = 0; i < _Device->configuration.noOfThermalProtectors; i++) {
#ifdef DEBUG
    Serial << endl << F("INFO: Synchronizing Thermal Protector: ") << i;
#endif
    publishThermalProtectorState(i);
  }
#endif

/* Synchronize: Binary sensor */
#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
  for (uint8_t i = 0; i < _Device->configuration.noOfBinarySensors; i++) {
#ifdef DEBUG
    Serial << endl << F("INFO: Synchronizing Binary sensor: ") << i;
#endif
    publishBinarySensorState(i);
  }
#endif

#ifdef DEBUG
  Serial << endl << F("INFO: Sending message: device is connected ...");
#endif
  if (Mqtt.configuration.lwt.idx > 0) {
    char lwtMessage[100];
    sprintf(
        lwtMessage,
        "{\"command\":\"udevice\",\"idx\":%d,\"nvalue\":1,\"svalue\":\"%s\","
        "\"Battery\":100,\"RSSI\":%d}",
        Mqtt.configuration.lwt.idx, L_NETWORK_CONNECTED, getRSSI());

    // Workaround for retainLWT
    boolean _retainAll = Mqtt.configuration.retainAll;
    Mqtt.configuration.retainAll = Mqtt.configuration.retainLWT;
    Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, lwtMessage);
    Mqtt.configuration.retainAll = _retainAll;
  }
}

void AFEAPIMQTTDomoticz::subscribe() {
#ifdef DEBUG
  Serial << endl << F("INFO: Subsribing to MQTT Topics ...");
#endif
  Mqtt.subscribe(AFE_CONFIG_API_DOMOTICZ_TOPIC_OUT);
}

DOMOTICZ_MQTT_COMMAND AFEAPIMQTTDomoticz::getCommand() {
  DOMOTICZ_MQTT_COMMAND command;
  char json[Mqtt.message.length];

  for (uint16_t i = 0; i < Mqtt.message.length; i++) {
    json[i] = Mqtt.message.content[i];
  }
  StaticJsonBuffer<AFE_CONFIG_API_JSON_BUFFER_SIZE> jsonBuffer;
  JsonObject &root = jsonBuffer.parseObject(json);

  if (root.success()) {
    command.domoticz.idx = root["idx"];
    command.nvalue = root["nvalue"];
#ifdef DEBUG
    Serial << endl
           << F("INFO: Domoticz: Got command: ") << command.nvalue
           << F(", IDX: ") << command.domoticz.idx;
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: Domoticz: Problem with JSON pharsing");
  }
#endif
  return command;
}

void AFEAPIMQTTDomoticz::processRequest() {
  DOMOTICZ_MQTT_COMMAND command = getCommand();
  if (command.domoticz.idx > 0 && idxForProcessing(command.domoticz.idx)) {
#ifdef DEBUG
    uint8_t _found = false;
#endif
    for (uint8_t i = 0; i < lastIDXChacheIndex; i++) {
      if (idxCache[i].domoticz.idx == command.domoticz.idx) {
        switch (idxCache[i].type) {
/* Processing Relay command*/
#ifdef AFE_CONFIG_HARDWARE_RELAY
        case AFE_DOMOTICZ_DEVICE_RELAY:
#ifdef DEBUG
          Serial << endl
                 << F("INFO: Domoticz: Found Relay ID: ") << idxCache[i].id;
          _found = true;
#endif
          if (_Relay[idxCache[i].id]->get() != (byte)command.nvalue) {
            if (command.nvalue == AFE_SWITCH_OFF) {
              _Relay[idxCache[i].id]->off();
            } else {
              _Relay[idxCache[i].id]->on();
            }
            publishRelayState(idxCache[i].id);
          }
#ifdef DEBUG
          else {
            Serial << endl << F("WARN: Domoticz: Same state. No change needed");
          }
#endif
          break;
#endif // AFE_CONFIG_HARDWARE_RELAY
#ifdef AFE_CONFIG_HARDWARE_GATE
        /* Processing Gate command*/
        case AFE_DOMOTICZ_DEVICE_GATE:
#ifdef DEBUG
          Serial << endl
                 << F("INFO: Domoticz: Found Gate ID: ") << idxCache[i].id;
          _found = true;
#endif

          if (command.nvalue == AFE_SWITCH_ON) {
            _Gate[idxCache[i].id]->toggle();
          }
#ifdef DEBUG
          else {
            Serial << endl << F("INFO: OFF Command, skipping");
          }
#endif
          break;
#endif // AFE_CONFIG_HARDWARE_GATE

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
        case AFE_DOMOTICZ_DEVICE_REGULATOR:
#ifdef DEBUG
          Serial << endl
                 << F("INFO: Domoticz: Found Regulator ID: ") << idxCache[i].id;
          _found = true;
#endif
          if (_Regulator[idxCache[i].id]->configuration.enabled !=
              (byte)command.nvalue) {
            if (command.nvalue == AFE_SWITCH_OFF) {
              _Regulator[idxCache[i].id]->off();
            } else {
              _Regulator[idxCache[i].id]->on();
            }
            publishRegulatorState(idxCache[i].id);
          }
#ifdef DEBUG
          else {
            Serial << endl << F("WARN: Domoticz: Same state. No change needed");
          }
#endif
          break;
#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
        case AFE_DOMOTICZ_DEVICE_THERMAL_PROTECTOR:
#ifdef DEBUG
          Serial << endl
                 << F("INFO: Domoticz: Found Thermal Protector ID: ")
                 << idxCache[i].id;
          _found = true;
#endif
          if (_ThermalProtector[idxCache[i].id]->configuration.enabled !=
              (byte)command.nvalue) {
            if (command.nvalue == AFE_SWITCH_OFF) {
              _ThermalProtector[idxCache[i].id]->off();
            } else {
              _ThermalProtector[idxCache[i].id]->on();
            }
            publishThermalProtectorState(idxCache[i].id);
          }
#ifdef DEBUG
          else {
            Serial << endl << F("WARN: Domoticz: Same state. No change needed");
          }
#endif
          break;
#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR

        /* Processing Unknown command*/
        default:
#ifdef DEBUG
          Serial << endl
                 << F("ERROR: Domoticz: Device type not handled. Type: ")
                 << idxCache[i].type;
#endif
          break;
        }
      }
    }
#ifdef DEBUG
    if (!_found) {
      Serial << endl
             << F("WARN: Domoticz: No item found with IDX: ")
             << command.domoticz.idx;
    }
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl
           << (command.domoticz.idx > 0 ? "INFO: Domoticz: Bypassing IDX: "
                                        : " - no IDX: ")
           << command.domoticz.idx;
  }
#endif
}

boolean AFEAPIMQTTDomoticz::idxForProcessing(uint32_t idx) {
  boolean _ret = true;
  // Returns true if Domoticz is in version 2020.x. All requests are processed
  if (idx == bypassProcessing.idx &&
      _Device->configuration.api.domoticzVersion == AFE_DOMOTICZ_VERSION_0) {
    bypassProcessing.idx = 0;
    _ret = false;
  }
  return _ret;
}

#ifdef AFE_CONFIG_HARDWARE_RELAY
void AFEAPIMQTTDomoticz::addClass(AFERelay *Relay) {
  AFEAPI::addClass(Relay);

#ifdef DEBUG
  Serial << endl << F("INFO: Caching IDXs for Relays");
#endif

  for (uint8_t i = 0; i < _Device->configuration.noOfRelays; i++) {
    if (_Relay[i]->configuration.domoticz.idx > 0) {
      idxCache[lastIDXChacheIndex].domoticz.idx =
          _Relay[i]->configuration.domoticz.idx;
      idxCache[lastIDXChacheIndex].id = i;
      idxCache[lastIDXChacheIndex].type = AFE_DOMOTICZ_DEVICE_RELAY;
#ifdef DEBUG
      Serial << endl
             << F(" - added IDX: ")
             << idxCache[lastIDXChacheIndex].domoticz.idx;
#endif
      lastIDXChacheIndex++;
    }
#ifdef DEBUG
    else {
      Serial << endl << F(" - IDX not set");
    }
#endif
  }
}

boolean AFEAPIMQTTDomoticz::publishRelayState(uint8_t id) {
#ifdef DEBG
  Serial << endl
         << F("INFO: Publishing relay: ") << id << F(", IDX: ")
         << idxCache[id].domoticz.idx << F(" state");
#endif
  boolean publishStatus = false;
  if (enabled && _Relay[id]->configuration.domoticz.idx > 0) {
    char json[AFE_CONFIG_API_JSON_SWITCH_COMMAND_LENGTH];
    generateSwitchMessage(json, _Relay[id]->configuration.domoticz.idx,
                          _Relay[id]->get());
    publishStatus = Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    bypassProcessing.idx = _Relay[id]->configuration.domoticz.idx;
  }
#ifdef DEBUG
  else {
    Serial << endl << F("INFO: Not published");
  }
#endif
  return publishStatus;
}
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_SWITCH
void AFEAPIMQTTDomoticz::addClass(AFESwitch *Switch) {
  AFEAPI::addClass(Switch);
}

boolean AFEAPIMQTTDomoticz::publishSwitchState(uint8_t id) {
  boolean publishStatus = false;
  if (enabled && _Switch[id]->configuration.domoticz.idx) {
    char json[AFE_CONFIG_API_JSON_SWITCH_COMMAND_LENGTH];

    generateSwitchMessage(json, _Switch[id]->configuration.domoticz.idx,
                          _Switch[id]->getPhisicalState() == 1 ? AFE_SWITCH_OFF
                                                               : AFE_SWITCH_ON);
    publishStatus = Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
  }
  return publishStatus;
}
#endif // AFE_CONFIG_HARDWARE_SWITCH

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
void AFEAPIMQTTDomoticz::addClass(AFEAnalogInput *Analog) {
  AFEAPI::addClass(Analog);
}
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
#ifdef AFE_ESP32
void AFEAPIMQTTDomoticz::publishADCValues(uint8_t id) {
  if (enabled) {
    char json[AFE_CONFIG_API_JSON_DEVICE_COMMAND_LENGTH];
    char value[20];
    if (_AnalogInput[id]->configuration.domoticz.percent > 0) {
      sprintf(value, "%-.2f", _AnalogInput[id]->data.percent);
      generateDeviceValue(
          json, _AnalogInput[id]->configuration.domoticz.percent, value);
      Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
    if (_AnalogInput[id]->configuration.domoticz.voltage > 0) {
      sprintf(value, "%-.4f", _AnalogInput[id]->data.voltage);
      generateDeviceValue(
          json, _AnalogInput[id]->configuration.domoticz.voltage, value);
      Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
    if (_AnalogInput[id]->configuration.domoticz.voltageCalculated > 0) {
      sprintf(value, "%-.4f", _AnalogInput[id]->data.voltageCalculated);
      generateDeviceValue(
          json, _AnalogInput[id]->configuration.domoticz.voltageCalculated,
          value);
      Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
    if (_AnalogInput[id]->configuration.domoticz.raw > 0) {
      sprintf(value, "%-d", _AnalogInput[id]->data.raw);
      generateDeviceValue(json, _AnalogInput[id]->configuration.domoticz.raw,
                          value);
      Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
  }
}
#else  // ESP8266
void AFEAPIMQTTDomoticz::publishADCValues() {
  if (enabled) {
    char json[AFE_CONFIG_API_JSON_DEVICE_COMMAND_LENGTH];
    char value[20];
    if (_AnalogInput->configuration.domoticz.percent > 0) {
      sprintf(value, "%-.2f", _AnalogInput->data.percent);
      generateDeviceValue(json, _AnalogInput->configuration.domoticz.percent,
                          value);
      Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
    if (_AnalogInput->configuration.domoticz.voltage > 0) {
      sprintf(value, "%-.4f", _AnalogInput->data.voltage);
      generateDeviceValue(json, _AnalogInput->configuration.domoticz.voltage,
                          value);
      Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
    if (_AnalogInput->configuration.domoticz.voltageCalculated > 0) {
      sprintf(value, "%-.4f", _AnalogInput->data.voltageCalculated);
      generateDeviceValue(
          json, _AnalogInput->configuration.domoticz.voltageCalculated, value);
      Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
    if (_AnalogInput->configuration.domoticz.raw > 0) {
      sprintf(value, "%-d", _AnalogInput->data.raw);
      generateDeviceValue(json, _AnalogInput->configuration.domoticz.raw,
                          value);
      Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
  }
}
#endif // ESP32/ESP8266
#endif // AFE_CONFIG_HARDWARE_ADC_VCC

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
void AFEAPIMQTTDomoticz::publishBatteryMeterValues() {
  if (enabled) {
    char json[AFE_CONFIG_API_JSON_BATTERYMETER_COMMAND_LENGTH];
    char value[8];
    if (_AnalogInput->configuration.battery.domoticz.idx > 0) {
      sprintf(value, "%-.3f", _AnalogInput->batteryPercentage);
      generateDeviceValue(
          json, _AnalogInput->configuration.battery.domoticz.idx, value);
      Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
  }
}
#endif // AFE_CONFIG_FUNCTIONALITY_BATTERYMETER

void AFEAPIMQTTDomoticz::generateSwitchMessage(char *json, uint32_t idx,
                                               boolean state) {
  sprintf(json, "{\"command\":\"switchlight\",\"idx\":%d,\"switchcmd\":\"%s\"}",
          idx, state ? "On" : "Off");
}

void AFEAPIMQTTDomoticz::generateDeviceValue(char *json, uint32_t idx,
                                             const char *svalue,
                                             uint16_t nvalue) {

  sprintf(
      json,
      "{\"command\":\"udevice\",\"idx\":%d,\"nvalue\":%d,\"svalue\":\"%s\"}",
      idx, nvalue, svalue);
}

uint8_t AFEAPIMQTTDomoticz::getRSSI() {
  uint8_t _ret;
  long current = WiFi.RSSI();
  if (current > -50) {
    _ret = 10;
  } else if (current < -98) {
    _ret = 0;
  } else {
    _ret = ((current + 97) / 5) + 1;
  }

  return _ret;
}

#ifdef AFE_CONFIG_HARDWARE_BMEX80
void AFEAPIMQTTDomoticz::addClass(AFESensorBMEX80 *Sensor) {
  AFEAPI::addClass(Sensor);
}
boolean AFEAPIMQTTDomoticz::publishBMx80SensorData(uint8_t id) {
  if (enabled) {
    char json[AFE_CONFIG_API_JSON_DEVICE_COMMAND_LENGTH];
    char value[50];
    /* Temperatura */
    if (_BMx80Sensor[id]->configuration.domoticz.temperature.idx > 0) {
      sprintf(value, "%-.2f", _BMx80Sensor[id]->data.temperature.value);
      generateDeviceValue(
          json, _BMx80Sensor[id]->configuration.domoticz.temperature.idx,
          value);
      Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
    /* Pressure */
    if (_BMx80Sensor[id]->configuration.domoticz.pressure.idx > 0) {
      sprintf(value, "%-.2f;0", _BMx80Sensor[id]->data.pressure.value);
      generateDeviceValue(
          json, _BMx80Sensor[id]->configuration.domoticz.pressure.idx, value);
      Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
    /* Relative pressure */
    if (_BMx80Sensor[id]->configuration.domoticz.relativePressure.idx > 0) {
      sprintf(value, "%-.2f;0", _BMx80Sensor[id]->data.relativePressure.value);
      generateDeviceValue(
          json, _BMx80Sensor[id]->configuration.domoticz.relativePressure.idx,
          value);
      Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }

    if (_BMx80Sensor[id]->configuration.type != AFE_BMP180_SENSOR) {
      /* Temp and Hum */
      if (_BMx80Sensor[id]->configuration.domoticz.temperatureHumidity.idx >
          0) {
        sprintf(value, "%-.2f;%-.2f;%-d",
                _BMx80Sensor[id]->data.temperature.value,
                _BMx80Sensor[id]->data.humidity.value,
                _BMx80Sensor[id]->convertHumidyStatusDomoticz(
                    _BMx80Sensor[id]->data.humidity.value));
        generateDeviceValue(
            json,
            _BMx80Sensor[id]->configuration.domoticz.temperatureHumidity.idx,
            value);
        Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
      }
      /* Temp + Hum + Press */
      if (_BMx80Sensor[id]
              ->configuration.domoticz.temperatureHumidityPressure.idx > 0) {
        sprintf(value, "%-.2f;%-.2f;%-d;%-.2f;0",
                _BMx80Sensor[id]->data.temperature.value,
                _BMx80Sensor[id]->data.humidity.value,
                _BMx80Sensor[id]->convertHumidyStatusDomoticz(
                    _BMx80Sensor[id]->data.humidity.value),
                _BMx80Sensor[id]->data.pressure.value);
        generateDeviceValue(
            json, _BMx80Sensor[id]
                      ->configuration.domoticz.temperatureHumidityPressure.idx,
            value);
        Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
      }
      /* Humididty */
      if (_BMx80Sensor[id]->configuration.domoticz.humidity.idx > 0) {
        sprintf(value, "%d", _BMx80Sensor[id]->convertHumidyStatusDomoticz(
                                 _BMx80Sensor[id]->data.humidity.value));
        generateDeviceValue(
            json, _BMx80Sensor[id]->configuration.domoticz.humidity.idx, value,
            (uint8_t)_BMx80Sensor[id]->data.humidity.value);
        Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
      }

      /* Absolute Humidity */
      if (_BMx80Sensor[id]->configuration.domoticz.absoluteHumidity.idx > 0) {
        sprintf(value, "%d", _BMx80Sensor[id]->convertHumidyStatusDomoticz(
                                 _BMx80Sensor[id]->data.humidity.value));
        generateDeviceValue(
            json, _BMx80Sensor[id]->configuration.domoticz.absoluteHumidity.idx,
            value, (uint8_t)_BMx80Sensor[id]->absoluteHumidity(
                       _BMx80Sensor[id]->data.temperature.value,
                       _BMx80Sensor[id]->data.humidity.value,
                       _BMx80Sensor[id]->configuration.temperature.unit ==
                           AFE_TEMPERATURE_UNIT_FAHRENHEIT));
        Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
      }

      /* DewPoint */
      if (_BMx80Sensor[id]->configuration.domoticz.dewPoint.idx > 0) {
        sprintf(value, "%-.2f", _BMx80Sensor[id]->data.dewPoint.value);
        generateDeviceValue(
            json, _BMx80Sensor[id]->configuration.domoticz.dewPoint.idx, value);
        Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
      }

      /* Heat Index */
      if (_BMx80Sensor[id]->configuration.domoticz.heatIndex.idx > 0) {
        sprintf(value, "%-.2f", _BMx80Sensor[id]->data.heatIndex.value);
        generateDeviceValue(
            json, _BMx80Sensor[id]->configuration.domoticz.heatIndex.idx,
            value);
        Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
      }

      /* Perception */
      if (_BMx80Sensor[id]->configuration.domoticz.perception.idx > 0) {
        char _perception[22]; // Max size of Perception from lang.pack
        byte _perceptionId = _BMx80Sensor[id]->perception(
            _BMx80Sensor[id]->data.temperature.value,
            _BMx80Sensor[id]->data.humidity.value,
            _BMx80Sensor[id]->configuration.temperature.unit ==
                AFE_TEMPERATURE_UNIT_FAHRENHEIT);
        strcpy_P(_perception,
                 (char *)pgm_read_dword(&(dewPointPerception[_perceptionId])));
        generateDeviceValue(
            json, _BMx80Sensor[id]->configuration.domoticz.perception.idx,
            _perception,
            _BMx80Sensor[id]->convertPerceptionDomoticz(_perceptionId));
        Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
      }

      /* Comfort */
      if (_BMx80Sensor[id]->configuration.domoticz.comfort.idx > 0) {
        char _comfort[80]; // Max size of Comfort from lang.pack
        ComfortState comfortStatus;
        _BMx80Sensor[id]->comfort(
            comfortStatus, _BMx80Sensor[id]->data.temperature.value,
            _BMx80Sensor[id]->data.humidity.value,
            _BMx80Sensor[id]->configuration.temperature.unit ==
                AFE_TEMPERATURE_UNIT_FAHRENHEIT);
        strcpy_P(_comfort, (char *)pgm_read_dword(&(Comfort[comfortStatus])));
        generateDeviceValue(
            json, _BMx80Sensor[id]->configuration.domoticz.comfort.idx,
            _comfort, _BMx80Sensor[id]->convertComfortDomoticz(comfortStatus));
        Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
      }
    }
    if (_BMx80Sensor[id]->configuration.type == AFE_BME680_SENSOR) {

      /* Gas Sensor */
      if (_BMx80Sensor[id]->configuration.domoticz.gasResistance.idx > 0) {
        sprintf(value, "%-.2f", _BMx80Sensor[id]->data.gasResistance.value);
        generateDeviceValue(
            json, _BMx80Sensor[id]->configuration.domoticz.gasResistance.idx,
            value);
        Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
      }
      /* IAQ */
      if (_BMx80Sensor[id]->configuration.domoticz.iaq.idx > 0) {
        sprintf(value, "%-.0f", _BMx80Sensor[id]->data.iaq.value);
        generateDeviceValue(
            json, _BMx80Sensor[id]->configuration.domoticz.iaq.idx, value);
        Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
      }
      /* Static IAQ */
      if (_BMx80Sensor[id]->configuration.domoticz.staticIaq.idx > 0) {
        sprintf(value, "%-.0f", _BMx80Sensor[id]->data.staticIaq.value);
        generateDeviceValue(
            json, _BMx80Sensor[id]->configuration.domoticz.staticIaq.idx,
            value);
        Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
      }
      /* CO2 */
      if (_BMx80Sensor[id]->configuration.domoticz.co2Equivalent.idx > 0) {
        sprintf(value, "%-.0f", _BMx80Sensor[id]->data.co2Equivalent.value);
        generateDeviceValue(
            json, _BMx80Sensor[id]->configuration.domoticz.co2Equivalent.idx,
            value);
        Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
      }
      /* BVOC */
      if (_BMx80Sensor[id]->configuration.domoticz.breathVocEquivalent.idx >
          0) {
        sprintf(value, "%-.1f",
                _BMx80Sensor[id]->data.breathVocEquivalent.value);
        generateDeviceValue(
            json,
            _BMx80Sensor[id]->configuration.domoticz.breathVocEquivalent.idx,
            value);
        Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
      }
    }
  }
  return true;
}
#endif // AFE_CONFIG_HARDWARE_BMEX80

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
void AFEAPIMQTTDomoticz::addClass(AFESensorHPMA115S0 *Sensor) {
  AFEAPI::addClass(Sensor);
}

boolean AFEAPIMQTTDomoticz::publishHPMA115S0SensorData(uint8_t id) {
  if (enabled) {
    char json[AFE_CONFIG_API_JSON_DEVICE_COMMAND_LENGTH];
    char value[5];

    if (_HPMA115S0Sensor[id]->configuration.domoticz.pm10.idx > 0) {

      sprintf(value, "%-.1f", _HPMA115S0Sensor[id]->data.pm10);
      generateDeviceValue(json,
                          _HPMA115S0Sensor[id]->configuration.domoticz.pm10.idx,
                          value, _HPMA115S0Sensor[id]->data.pm10);
      Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }

    if (_HPMA115S0Sensor[id]->configuration.domoticz.pm25.idx > 0) {
      sprintf(value, "%-.1f", _HPMA115S0Sensor[id]->data.pm25);
      generateDeviceValue(json,
                          _HPMA115S0Sensor[id]->configuration.domoticz.pm25.idx,
                          value, _HPMA115S0Sensor[id]->data.pm25);
      Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
    if (_HPMA115S0Sensor[id]->configuration.domoticz.whoPM10Norm.idx > 0) {

      sprintf(value, "%-.1f", _HPMA115S0Sensor[id]->data.whoPM10Norm);
      generateDeviceValue(
          json, _HPMA115S0Sensor[id]->configuration.domoticz.whoPM10Norm.idx,
          value, _HPMA115S0Sensor[id]->data.whoPM10Norm);
      Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }

    if (_HPMA115S0Sensor[id]->configuration.domoticz.whoPM25Norm.idx > 0) {
      sprintf(value, "%-.1f", _HPMA115S0Sensor[id]->data.whoPM25Norm);
      generateDeviceValue(
          json, _HPMA115S0Sensor[id]->configuration.domoticz.whoPM25Norm.idx,
          value, _HPMA115S0Sensor[id]->data.whoPM25Norm);
      Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
  }
  return true;
}
#endif // AFE_CONFIG_HARDWARE_HPMA115S0

#ifdef AFE_CONFIG_HARDWARE_BH1750
void AFEAPIMQTTDomoticz::addClass(AFESensorBH1750 *Sensor) {
  AFEAPI::addClass(Sensor);
}
boolean AFEAPIMQTTDomoticz::publishBH1750SensorData(uint8_t id) {
  if (enabled) {
    if (_BH1750Sensor[id]->configuration.domoticz.idx > 0) {
      char json[AFE_CONFIG_API_JSON_DEVICE_COMMAND_LENGTH];
      char value[6];
      sprintf(value, "%-.0f", _BH1750Sensor[id]->data);
      generateDeviceValue(json, _BH1750Sensor[id]->configuration.domoticz.idx,
                          value);
      Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
  }
  return true;
}
#endif // AFE_CONFIG_HARDWARE_BH1750

#ifdef AFE_CONFIG_HARDWARE_TLS2561
void AFEAPIMQTTDomoticz::addClass(AFESensorTLS2561 *Sensor) {
  AFEAPI::addClass(Sensor);
}
boolean AFEAPIMQTTDomoticz::publishTLS2561SensorData(uint8_t id) {
  if (enabled) {
    char json[AFE_CONFIG_API_JSON_DEVICE_COMMAND_LENGTH];
    char value[6];
    if (_TLS2561Sensor[id]->configuration.domoticz.ir.idx > 0) {
      sprintf(value, "%d", _TLS2561Sensor[id]->ir);
      generateDeviceValue(json, _TLS2561Sensor[id]->configuration.domoticz.ir.idx,
                          value);
      Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
    if (_TLS2561Sensor[id]->configuration.domoticz.illuminance.idx > 0) {
      sprintf(value, "%d", _TLS2561Sensor[id]->illuminance);
      generateDeviceValue(
          json, _TLS2561Sensor[id]->configuration.domoticz.illuminance.idx, value);
      Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
    if (_TLS2561Sensor[id]->configuration.domoticz.broadband.idx > 0) {
      sprintf(value, "%d", _TLS2561Sensor[id]->broadband);
      generateDeviceValue(
          json, _TLS2561Sensor[id]->configuration.domoticz.broadband.idx, value);
      Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
  }
  return true;
}
#endif // AFE_CONFIG_HARDWARE_TLS2561

#ifdef AFE_CONFIG_HARDWARE_AS3935
void AFEAPIMQTTDomoticz::addClass(AFESensorAS3935 *Sensor) {
  AFEAPI::addClass(Sensor);
}
boolean AFEAPIMQTTDomoticz::publishAS3935SensorData(uint8_t id) {
  if (enabled) {
    if (_AS3935Sensor[id]->configuration.domoticz.idx > 0) {
      char json[AFE_CONFIG_API_JSON_DEVICE_COMMAND_LENGTH];
      char value[4];
      sprintf(value, "%-d", _AS3935Sensor[id]->distance);
      generateDeviceValue(json, _AS3935Sensor[id]->configuration.domoticz.idx,
                          value);
      Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
  }
  return true;
}
#endif // AFE_CONFIG_HARDWARE_AS3935

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
void AFEAPIMQTTDomoticz::addClass(AFEAnemometer *Sensor) {
  AFEAPI::addClass(Sensor);
}

void AFEAPIMQTTDomoticz::publishAnemometerSensorData() {
  if (enabled) {
    char json[AFE_CONFIG_API_JSON_ANEMOMETER_COMMAND_LENGTH];
    char value[20];
    if (_AnemometerSensor->configuration.domoticz.idx > 0) {
      sprintf(value, "0;N;%-.2f;0;?;?", 10 * _AnemometerSensor->lastSpeedMS);
      generateDeviceValue(json, _AnemometerSensor->configuration.domoticz.idx,
                          value);
      Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_ANEMOMETER

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
void AFEAPIMQTTDomoticz::addClass(AFERainmeter *Sensor) {
  AFEAPI::addClass(Sensor);
}

void AFEAPIMQTTDomoticz::publishRainSensorData() {
  if (enabled) {
    char json[AFE_CONFIG_API_JSON_RAINMETER_COMMAND_LENGTH];
    char value[20];
    if (_RainmeterSensor->configuration.domoticz.idx > 0) {
      sprintf(value, "%-.2f;%-.2f", _RainmeterSensor->rainLevelLastHour * 100,
              _RainmeterSensor->current.counter);
      generateDeviceValue(json, _RainmeterSensor->configuration.domoticz.idx,
                          value);
      Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_ANEMOMETER

#ifdef AFE_CONFIG_HARDWARE_GATE
void AFEAPIMQTTDomoticz::addClass(AFEGate *Item) {
  AFEAPI::addClass(Item);

#ifdef DEBUG
  Serial << endl << F("INFO: Caching IDXs for Gates");
#endif

  for (uint8_t i = 0; i < _Device->configuration.noOfGates; i++) {
    if (_Gate[i]->configuration.domoticzControl.idx > 0) {
      idxCache[lastIDXChacheIndex].domoticz.idx =
          _Gate[i]->configuration.domoticzControl.idx;
      idxCache[lastIDXChacheIndex].id = i;
      idxCache[lastIDXChacheIndex].type = AFE_DOMOTICZ_DEVICE_GATE;
#ifdef DEBUG
      Serial << endl
             << F(" - added IDX: ")
             << idxCache[lastIDXChacheIndex].domoticz.idx;
#endif
      lastIDXChacheIndex++;
    }
#ifdef DEBUG
    else {
      Serial << endl << F(" - IDX not set");
    }
#endif
  }
}

boolean AFEAPIMQTTDomoticz::publishGateState(uint8_t id) {
  if (enabled && _Gate[id]->configuration.domoticz.idx > 0) {
    char json[AFE_CONFIG_API_JSON_GATE_COMMAND_LENGTH];
    generateSwitchMessage(json, _Gate[id]->configuration.domoticz.idx,
                          _Gate[id]->get() == AFE_GATE_CLOSED ? false : true);
    Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
  }
  return true;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
void AFEAPIMQTTDomoticz::addClass(AFEContactron *Item) {
  AFEAPI::addClass(Item);
}
boolean AFEAPIMQTTDomoticz::publishContactronState(uint8_t id) {
  if (enabled && _Contactron[id]->configuration.domoticz.idx > 0) {
    char json[AFE_CONFIG_API_JSON_CONTACTRON_COMMAND_LENGTH];
    generateSwitchMessage(
        json, _Contactron[id]->configuration.domoticz.idx,
        _Contactron[id]->get() == AFE_CONTACTRON_OPEN ? true : false);
    Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
  }
  return true;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
void AFEAPIMQTTDomoticz::addClass(AFESensorDS18B20 *Sensor) {
  AFEAPI::addClass(Sensor);
}
boolean AFEAPIMQTTDomoticz::publishDS18B20SensorData(uint8_t id) {
  if (enabled) {
    if (_DS18B20Sensor[id]->configuration.domoticz.idx > 0) {
      char json[AFE_CONFIG_API_JSON_DS18B20_COMMAND_LENGTH];
      char value[9];
      sprintf(value, "%-.3f", _DS18B20Sensor[id]->getTemperature());
      generateDeviceValue(json, _DS18B20Sensor[id]->configuration.domoticz.idx,
                          value);
      Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
  }
  return true;
}
#endif // AFE_CONFIG_HARDWARE_DS18B20

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
void AFEAPIMQTTDomoticz::addClass(AFERegulator *Regulator) {
  AFEAPI::addClass(Regulator);

#ifdef DEBUG
  Serial << endl << F("INFO: Caching IDXs for Regulators");
#endif

  for (uint8_t i = 0; i < _Device->configuration.noOfRegulators; i++) {
    if (_Regulator[i]->configuration.domoticz.idx > 0) {
      idxCache[lastIDXChacheIndex].domoticz.idx =
          _Regulator[i]->configuration.domoticz.idx;
      idxCache[lastIDXChacheIndex].id = i;
      idxCache[lastIDXChacheIndex].type = AFE_DOMOTICZ_DEVICE_REGULATOR;
#ifdef DEBUG
      Serial << endl
             << F(" - added IDX: ")
             << idxCache[lastIDXChacheIndex].domoticz.idx;
#endif
      lastIDXChacheIndex++;
    }
#ifdef DEBUG
    else {
      Serial << endl << F(" - IDX not set");
    }
#endif
  }
}

boolean AFEAPIMQTTDomoticz::publishRegulatorState(uint8_t id) {
#ifdef DEBG
  Serial << endl
         << F("INFO: Publishing regulator: ") << id << F(", IDX: ")
         << idxCache[id].domoticz.idx << F(" state");
#endif
  boolean publishStatus = false;
  if (enabled && _Regulator[id]->configuration.domoticz.idx > 0) {
    char json[AFE_CONFIG_API_JSON_REGULATOR_COMMAND_LENGTH];
    generateSwitchMessage(json, _Regulator[id]->configuration.domoticz.idx,
                          _Regulator[id]->configuration.enabled);
    publishStatus = Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    bypassProcessing.idx = _Regulator[id]->configuration.domoticz.idx;
  }
#ifdef DEBUG
  else {
    Serial << endl << F("INFO: Not published");
  }
#endif
  return publishStatus;
}
#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
void AFEAPIMQTTDomoticz::addClass(AFEThermalProtector *ThermalProtector) {
  AFEAPI::addClass(ThermalProtector);

#ifdef DEBUG
  Serial << endl << F("INFO: Caching IDXs for Thermal Protector");
#endif

  for (uint8_t i = 0; i < _Device->configuration.noOfThermalProtectors; i++) {
    if (_ThermalProtector[i]->configuration.domoticz.idx > 0) {
      idxCache[lastIDXChacheIndex].domoticz.idx =
          _ThermalProtector[i]->configuration.domoticz.idx;
      idxCache[lastIDXChacheIndex].id = i;
      idxCache[lastIDXChacheIndex].type = AFE_DOMOTICZ_DEVICE_THERMAL_PROTECTOR;
#ifdef DEBUG
      Serial << endl
             << F(" - added IDX: ")
             << idxCache[lastIDXChacheIndex].domoticz.idx;
#endif
      lastIDXChacheIndex++;
    }
#ifdef DEBUG
    else {
      Serial << endl << F(" - IDX not set");
    }
#endif
  }
}

boolean AFEAPIMQTTDomoticz::publishThermalProtectorState(uint8_t id) {
#ifdef DEBG
  Serial << endl
         << F("INFO: Publishing thermal protector: ") << id << F(", IDX: ")
         << idxCache[id].domoticz.idx << F(" state");
#endif
  boolean publishStatus = false;
  if (enabled && _ThermalProtector[id]->configuration.domoticz.idx > 0) {
    char json[AFE_CONFIG_API_JSON_THERMAL_PROTECTOR_COMMAND_LENGTH];
    generateSwitchMessage(json,
                          _ThermalProtector[id]->configuration.domoticz.idx,
                          _ThermalProtector[id]->configuration.enabled);
    publishStatus = Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    bypassProcessing.idx = _ThermalProtector[id]->configuration.domoticz.idx;
  }
#ifdef DEBUG
  else {
    Serial << endl << F("INFO: Not published");
  }
#endif
  return publishStatus;
}
#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR

#ifdef AFE_CONFIG_HARDWARE_DHT
void AFEAPIMQTTDomoticz::addClass(AFESensorDHT *Sensor) {
  AFEAPI::addClass(Sensor);
}
boolean AFEAPIMQTTDomoticz::publishDHTSensorData(uint8_t id) {
  if (enabled) {
    char json[AFE_CONFIG_API_JSON_DHT_COMMAND_LENGTH];
    char value[15];
    /* Temperature */
    if (_DHTSensor[id]->configuration.domoticz.temperature.idx > 0) {
      sprintf(value, "%-.1f", _DHTSensor[id]->currentTemperature);
      generateDeviceValue(
          json, _DHTSensor[id]->configuration.domoticz.temperature.idx, value);
      Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
    /* Humidity */
    if (_DHTSensor[id]->configuration.domoticz.humidity.idx > 0) {
      sprintf(value, "%d", _DHTSensor[id]->convertHumidyStatusDomoticz(
                               _DHTSensor[id]->currentHumidity));
      generateDeviceValue(json,
                          _DHTSensor[id]->configuration.domoticz.humidity.idx,
                          value, (uint8_t)_DHTSensor[id]->currentHumidity);

      Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }

    /* Temperature and Humidity */
    if (_DHTSensor[id]->configuration.domoticz.temperatureHumidity.idx > 0) {
      sprintf(value, "%-.1f;%-.1f;%-d", _DHTSensor[id]->currentTemperature,
              _DHTSensor[id]->currentHumidity,
              _DHTSensor[id]->convertHumidyStatusDomoticz(
                  _DHTSensor[id]->currentHumidity));
      generateDeviceValue(
          json, _DHTSensor[id]->configuration.domoticz.temperatureHumidity.idx,
          value);
      Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }

    /* Absolute Humidity */
    if (_DHTSensor[id]->configuration.domoticz.absoluteHumidity.idx > 0) {
      sprintf(value, "%d", _DHTSensor[id]->convertHumidyStatusDomoticz(
                               _DHTSensor[id]->currentHumidity));
      generateDeviceValue(
          json, _DHTSensor[id]->configuration.domoticz.absoluteHumidity.idx,
          value, (uint8_t)_DHTSensor[id]->absoluteHumidity(
                     _DHTSensor[id]->currentTemperature,
                     _DHTSensor[id]->currentHumidity,
                     _DHTSensor[id]->configuration.temperature.unit ==
                         AFE_TEMPERATURE_UNIT_FAHRENHEIT));
      Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }

    /* HeatIndex */
    if (_DHTSensor[id]->configuration.domoticz.heatIndex.idx > 0) {
      sprintf(value, "%-.2f",
              _DHTSensor[id]->heatIndex(
                  _DHTSensor[id]->currentTemperature,
                  _DHTSensor[id]->currentHumidity,
                  _DHTSensor[id]->configuration.temperature.unit ==
                      AFE_TEMPERATURE_UNIT_FAHRENHEIT));
      generateDeviceValue(
          json, _DHTSensor[id]->configuration.domoticz.heatIndex.idx, value);
      Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }

    /* DewPoint */
    if (_DHTSensor[id]->configuration.domoticz.dewPoint.idx > 0) {
      sprintf(value, "%-.2f",
              _DHTSensor[id]->dewPoint(
                  _DHTSensor[id]->currentTemperature,
                  _DHTSensor[id]->currentHumidity,
                  _DHTSensor[id]->configuration.temperature.unit ==
                      AFE_TEMPERATURE_UNIT_FAHRENHEIT));
      generateDeviceValue(
          json, _DHTSensor[id]->configuration.domoticz.dewPoint.idx, value);
      Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }

    /* Perception */
    if (_DHTSensor[id]->configuration.domoticz.perception.idx > 0) {
      char _perception[22]; // Max size of Perception from lang.pack
      byte _perceptionId = _DHTSensor[id]->perception(
          _DHTSensor[id]->currentTemperature, _DHTSensor[id]->currentHumidity,
          _DHTSensor[id]->configuration.temperature.unit ==
              AFE_TEMPERATURE_UNIT_FAHRENHEIT);
      strcpy_P(_perception,
               (char *)pgm_read_dword(&(dewPointPerception[_perceptionId])));
      generateDeviceValue(
          json, _DHTSensor[id]->configuration.domoticz.perception.idx,
          _perception,
          _DHTSensor[id]->convertPerceptionDomoticz(_perceptionId));
      Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }

    /* Comfort */
    if (_DHTSensor[id]->configuration.domoticz.comfort.idx > 0) {
      char _comfort[80]; // Max size of Comfort from lang.pack
      ComfortState comfortStatus;
      _DHTSensor[id]->comfort(comfortStatus, _DHTSensor[id]->currentTemperature,
                              _DHTSensor[id]->currentHumidity,
                              _DHTSensor[id]->configuration.temperature.unit ==
                                  AFE_TEMPERATURE_UNIT_FAHRENHEIT);
      strcpy_P(_comfort, (char *)pgm_read_dword(&(Comfort[comfortStatus])));
      generateDeviceValue(
          json, _DHTSensor[id]->configuration.domoticz.comfort.idx, _comfort,
          _DHTSensor[id]->convertComfortDomoticz(comfortStatus));
      Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
  }
  return true;
}
#endif // AFE_CONFIG_HARDWARE_DS18B20

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
void AFEAPIMQTTDomoticz::addClass(AFESensorBinary *Sensor) {
  AFEAPI::addClass(Sensor);
}

boolean AFEAPIMQTTDomoticz::publishBinarySensorState(uint8_t id) {
  boolean publishStatus = false;
  if (enabled && _BinarySensor[id]->configuration.domoticz.idx) {
    char json[AFE_CONFIG_API_JSON_BINARY_SENSOR_COMMAND_LENGTH];

    generateSwitchMessage(json, _BinarySensor[id]->configuration.domoticz.idx,
                          _BinarySensor[id]->get() == 1 ? AFE_SWITCH_OFF
                                                        : AFE_SWITCH_ON);
    publishStatus = Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
  }
  return publishStatus;
}
#endif // AFE_CONFIG_HARDWARE_BINARY_SENSOR

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
void AFEAPIMQTTDomoticz::addClass(AFEMiFareCard *Sensor) {
  AFEAPI::addClass(Sensor);
}

boolean AFEAPIMQTTDomoticz::publishMiFareCardState(uint8_t id, uint8_t tagId,
                                                   uint8_t state,
                                                   const char *user) {
  boolean publishStatus = false;
  if (enabled && _MiFareCard[id]->configuration.domoticz[tagId].idx) {
    char json[AFE_CONFIG_API_JSON_MIFARE_CARD_COMMAND_LENGTH];

    generateDeviceValue(
        json, _MiFareCard[id]->configuration.domoticz[tagId].idx, user, state);
    publishStatus = Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
  }
  return publishStatus;
}
#endif // AFE_CONFIG_HARDWARE_PN532_SENSOR

#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED