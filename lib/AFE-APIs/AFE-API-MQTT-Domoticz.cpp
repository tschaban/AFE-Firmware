/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-API-MQTT-Domoticz.h"

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED

AFEAPIMQTTDomoticz::AFEAPIMQTTDomoticz() : AFEAPI(){};

#ifdef AFE_CONFIG_HARDWARE_LED
void AFEAPIMQTTDomoticz::begin(AFEDataAccess *Data, AFEDevice *Device,
                               AFELED *Led) {
  AFEAPI::begin(Data, Device, Led);
#ifdef DEBUG
  Serial << endl
         << "INFO: Domoticz version: "
         << (Device->configuration.api.domoticzVersion == AFE_DOMOTICZ_VERSION_0
                 ? L_DOMOTICZ_VERSION_410
                 : L_DOMOTICZ_VERSION_2020);
#endif
}
#else
void AFEAPIMQTTDomoticz::begin(AFEDataAccess *Data, AFEDevice *Device) {
  AFEAPI::begin(Data, Device);
}
#endif // AFE_CONFIG_HARDWARE_LED

void AFEAPIMQTTDomoticz::listener() {
  if (Mqtt.listener()) {
#ifdef AFE_CONFIG_API_PROCESS_REQUESTS
    processRequest();
#endif
  }
}

void AFEAPIMQTTDomoticz::synchronize() {
#ifdef DEBUG
  Serial << endl << "INFO: Sending current device state to MQTT Broker  ...";
#endif

/* Synchronize: Relay */
#ifdef AFE_CONFIG_HARDWARE_RELAY
  for (uint8_t i = 0; i < _Device->configuration.noOfRelays; i++) {
#ifdef DEBUG
    Serial << endl << "INFO: Synchronizing RELAY: " << i;
#endif
    publishRelayState(i);
  }
#endif

/* Synchronize: Switch */
#ifdef AFE_CONFIG_HARDWARE_SWITCH
  for (uint8_t i = 0; i < _Device->configuration.noOfSwitches; i++) {
#ifdef DEBUG
    Serial << endl << "INFO: Synchronizing SWITCH: " << i;
#endif
    publishSwitchState(i);
  }
#endif

/* Synchronize: Contactron */
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  for (uint8_t i = 0; i < _Device->configuration.noOfContactrons; i++) {
#ifdef DEBUG
    Serial << endl << "INFO: Synchronizing CONTACTRON: " << i;
#endif
    publishContactronState(i);
  }
#endif

/* Synchronize: Gate */
#ifdef AFE_CONFIG_HARDWARE_GATE
  for (uint8_t i = 0; i < _Device->configuration.noOfGates; i++) {
#ifdef DEBUG
    Serial << endl << "INFO: Synchronizing GATE: " << i;
#endif
    publishGateState(i);
  }
#endif

#ifdef DEBUG
  Serial << endl << "INFO: Sending message: device is connected ...";
#endif
  if (Mqtt.configuration.lwt.idx > 0) {
    char lwtMessage[100];
    sprintf(
        lwtMessage,
        "{\"command\":\"udevice\",\"idx\":%d,\"nvalue\":1,\"svalue\":\"%s\","
        "\"Battery\":100,\"RSSI\":%d}",
        Mqtt.configuration.lwt.idx, L_CONNECTED, getRSSI());
    Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, lwtMessage);
  }
}

void AFEAPIMQTTDomoticz::subscribe() {
#ifdef DEBUG
  Serial << endl << "INFO: Subsribing to MQTT Topics ...";
#endif
  Mqtt.subscribe(AFE_CONFIG_API_DOMOTICZ_TOPIC_OUT);
}

#ifdef AFE_CONFIG_API_PROCESS_REQUESTS
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
           << "INFO: Domoticz: Got command: " << command.nvalue
           << ", IDX: " << command.domoticz.idx;
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << "ERROR: Domoticz: Problem with JSON pharsing";
  }
#endif
  return command;
}
#endif

#ifdef AFE_CONFIG_API_PROCESS_REQUESTS
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
        case AFE_DOMOTICZ_DEVICE_RELAY:
#ifdef DEBUG
          Serial << endl
                 << "INFO: Domoticz: Found Relay ID: " << idxCache[i].id;
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
            Serial << endl << "WARN: Domoticz: Same state. No change needed";
          }
#endif
          break;
#ifdef AFE_CONFIG_HARDWARE_GATE
        /* Processing Gate command*/
        case AFE_DOMOTICZ_DEVICE_GATE:
#ifdef DEBUG
          Serial << endl << "INFO: Domoticz: Found Gate ID: " << idxCache[i].id;
          _found = true;
#endif
          _Gate[idxCache[i].id]->toggle();
#endif // AFE_CONFIG_HARDWARE_GATE
          break;
        /* Processing Unknown command*/
        default:
#ifdef DEBUG
          Serial << endl
                 << "ERROR: Domoticz: Device type not handled. Type: "
                 << idxCache[i].type;
#endif
          break;
        }
      }
    }
#ifdef DEBUG
    if (!_found) {
      Serial << endl
             << "WARN: Domoticz: No item found with IDX: "
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
#endif // AFE_CONFIG_API_PROCESS_REQUESTS

#ifdef AFE_CONFIG_API_PROCESS_REQUESTS
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
#endif // AFE_CONFIG_API_PROCESS_REQUESTS

#ifdef AFE_CONFIG_HARDWARE_RELAY
void AFEAPIMQTTDomoticz::addClass(AFERelay *Relay) {
  AFEAPI::addClass(Relay);

#ifdef DEBUG
  Serial << endl << "INFO: Caching IDXs for Relays";
#endif

  for (uint8_t i = 0; i < _Device->configuration.noOfRelays; i++) {
    if (_Relay[i]->configuration.domoticz.idx > 0) {
      idxCache[lastIDXChacheIndex].domoticz.idx =
          _Relay[i]->configuration.domoticz.idx;
      idxCache[lastIDXChacheIndex].id = i;
      idxCache[lastIDXChacheIndex].type = AFE_DOMOTICZ_DEVICE_RELAY;
#ifdef DEBUG
      Serial << endl
             << " - added IDX: " << idxCache[lastIDXChacheIndex].domoticz.idx;
#endif
      lastIDXChacheIndex++;
    }
#ifdef DEBUG
    else {
      Serial << endl << " - IDX not set";
    }
#endif
  }
}
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_API_PROCESS_REQUESTS
boolean AFEAPIMQTTDomoticz::publishRelayState(uint8_t id) {
#ifdef DEBUG
  Serial << endl
         << "INFO: Publishing relay: " << id
         << ", IDX: " << idxCache[id].domoticz.idx << " state";
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
    Serial << endl << "INFO: Not published";
  }
#endif
  return publishStatus;
}
#endif // AFE_CONFIG_API_PROCESS_REQUESTS

#ifdef AFE_CONFIG_HARDWARE_SWITCH
void AFEAPIMQTTDomoticz::addClass(AFESwitch *Switch) {
  AFEAPI::addClass(Switch);
}
#endif // AFE_CONFIG_HARDWARE_SWITCH

#ifdef AFE_CONFIG_HARDWARE_SWITCH
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
#endif

void AFEAPIMQTTDomoticz::generateSwitchMessage(char *json, uint32_t idx,
                                               boolean state) {
  sprintf(json, "{\"command\":\"switchlight\",\"idx\":%d,\"switchcmd\":\"%s\"}",
          idx, state ? "On" : "Off");
}

void AFEAPIMQTTDomoticz::generateDeviceValue(char *json, uint32_t idx,
                                             char *svalue, uint8_t nvalue) {

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
    if (_BMx80Sensor[id]->configuration.domoticz.temperature.idx > 0) {
      sprintf(value, "%-.2f", _BMx80Sensor[id]->data.temperature.value);
      generateDeviceValue(
          json, _BMx80Sensor[id]->configuration.domoticz.temperature.idx,
          value);
      Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
    if (_BMx80Sensor[id]->configuration.domoticz.pressure.idx > 0) {
      sprintf(value, "%-.2f", _BMx80Sensor[id]->data.pressure.value);
      generateDeviceValue(
          json, _BMx80Sensor[id]->configuration.domoticz.pressure.idx, value);
      Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
    if (_BMx80Sensor[id]->configuration.domoticz.relativePressure.idx > 0) {
      sprintf(value, "%-.2f", _BMx80Sensor[id]->data.relativePressure.value);
      generateDeviceValue(
          json, _BMx80Sensor[id]->configuration.domoticz.relativePressure.idx,
          value);
      Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }

    if (_BMx80Sensor[id]->configuration.type != AFE_BMP180_SENSOR) {

      if (_BMx80Sensor[id]->configuration.domoticz.temperatureHumidity.idx >
          0) {
        sprintf(value, "%-.2f;%-.2f;%-d",
                _BMx80Sensor[id]->data.temperature.value,
                _BMx80Sensor[id]->data.humidity.value,
                _BMx80Sensor[id]->getDomoticzHumidityState(
                    _BMx80Sensor[id]->data.humidity.value));
        generateDeviceValue(
            json,
            _BMx80Sensor[id]->configuration.domoticz.temperatureHumidity.idx,
            value);
        Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
      }

      if (_BMx80Sensor[id]
              ->configuration.domoticz.temperatureHumidityPressure.idx > 0) {
        sprintf(value, "%-.2f;%-.2f;%-d;%-.2f;0",
                _BMx80Sensor[id]->data.temperature.value,
                _BMx80Sensor[id]->data.humidity.value,
                _BMx80Sensor[id]->getDomoticzHumidityState(
                    _BMx80Sensor[id]->data.humidity.value),
                _BMx80Sensor[id]->data.pressure.value);
        generateDeviceValue(
            json, _BMx80Sensor[id]
                      ->configuration.domoticz.temperatureHumidityPressure.idx,
            value);
        Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
      }

      if (_BMx80Sensor[id]->configuration.domoticz.humidity.idx > 0) {
        sprintf(value, "%d", _BMx80Sensor[id]->getDomoticzHumidityState(
                                 _BMx80Sensor[id]->data.humidity.value));
        generateDeviceValue(
            json, _BMx80Sensor[id]->configuration.domoticz.humidity.idx, value,
            (uint8_t)_BMx80Sensor[id]->data.humidity.value);
        Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
      }

      if (_BMx80Sensor[id]->configuration.domoticz.dewPoint.idx > 0) {
        sprintf(value, "%-.2f", _BMx80Sensor[id]->data.dewPoint.value);
        generateDeviceValue(
            json, _BMx80Sensor[id]->configuration.domoticz.dewPoint.idx, value);
        Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
      }
      if (_BMx80Sensor[id]->configuration.domoticz.heatIndex.idx > 0) {
        sprintf(value, "%-.2f", _BMx80Sensor[id]->data.heatIndex.value);
        generateDeviceValue(
            json, _BMx80Sensor[id]->configuration.domoticz.heatIndex.idx,
            value);
        Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
      }
    }
    if (_BMx80Sensor[id]->configuration.type == AFE_BME680_SENSOR) {
      if (_BMx80Sensor[id]->configuration.domoticz.gasResistance.idx > 0) {
        sprintf(value, "%-.2f", _BMx80Sensor[id]->data.gasResistance.value);
        generateDeviceValue(
            json, _BMx80Sensor[id]->configuration.domoticz.gasResistance.idx,
            value);
        Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
      }
      if (_BMx80Sensor[id]->configuration.domoticz.iaq.idx > 0) {
        sprintf(value, "%-.0f", _BMx80Sensor[id]->data.iaq.value);
        generateDeviceValue(
            json, _BMx80Sensor[id]->configuration.domoticz.iaq.idx, value);
        Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
      }
      if (_BMx80Sensor[id]->configuration.domoticz.staticIaq.idx > 0) {
        sprintf(value, "%-.0f", _BMx80Sensor[id]->data.staticIaq.value);
        generateDeviceValue(
            json, _BMx80Sensor[id]->configuration.domoticz.staticIaq.idx,
            value);
        Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
      }
      if (_BMx80Sensor[id]->configuration.domoticz.co2Equivalent.idx > 0) {
        sprintf(value, "%-.0f", _BMx80Sensor[id]->data.co2Equivalent.value);
        generateDeviceValue(
            json, _BMx80Sensor[id]->configuration.domoticz.co2Equivalent.idx,
            value);
        Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
      }
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
    if (_HPMA115S0Sensor[id]->configuration.domoticz.pm25.idx > 0) {

      sprintf(value, "%-d", _HPMA115S0Sensor[id]->data.pm25);
      generateDeviceValue(
          json, _HPMA115S0Sensor[id]->configuration.domoticz.pm25.idx, value);
      Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }

    if (_HPMA115S0Sensor[id]->configuration.domoticz.pm10.idx > 0) {

      sprintf(value, "%-d", _HPMA115S0Sensor[id]->data.pm10);
      generateDeviceValue(
          json, _HPMA115S0Sensor[id]->configuration.domoticz.pm10.idx, value);
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

#ifdef AFE_CONFIG_HARDWARE_GATE
void AFEAPIMQTTDomoticz::addClass(AFEGate *Item) {
  AFEAPI::addClass(Item);

#ifdef DEBUG
  Serial << endl << "INFO: Caching IDXs for Gates";
#endif

  for (uint8_t i = 0; i < _Device->configuration.noOfGates; i++) {
    if (_Gate[i]->configuration.domoticzControl.idx > 0) {
      idxCache[lastIDXChacheIndex].domoticz.idx =
          _Gate[i]->configuration.domoticzControl.idx;
      idxCache[lastIDXChacheIndex].id = i;
      idxCache[lastIDXChacheIndex].type = AFE_DOMOTICZ_DEVICE_GATE;
#ifdef DEBUG
      Serial << endl
             << " - added IDX: " << idxCache[lastIDXChacheIndex].domoticz.idx;
#endif
      lastIDXChacheIndex++;
    }
#ifdef DEBUG
    else {
      Serial << endl << " - IDX not set";
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

#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED