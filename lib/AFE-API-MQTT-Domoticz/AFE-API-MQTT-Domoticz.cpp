/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-API-MQTT-Domoticz.h"

AFEAPIMQTTDomoticz::AFEAPIMQTTDomoticz(){

};

void AFEAPIMQTTDomoticz::begin(AFEDataAccess *Data, AFEDevice *Device) {
  _Data = Data;
  _Device = Device;
  Mqtt.begin(_Data, _Device->configuration.name);
  enabled = true;
}

void AFEAPIMQTTDomoticz::listener() {
  if (Mqtt.listener()) {
#ifdef AFE_CONFIG_API_PROCESS_MQTT_REQUESTS
    processRequest();
#endif
  }
}

#ifdef AFE_CONFIG_API_PROCESS_MQTT_REQUESTS
DOMOTICZ_MQTT_COMMAND AFEAPIMQTTDomoticz::getCommand() {
  DOMOTICZ_MQTT_COMMAND command;
  char json[Mqtt.message.length];
  for (uint16_t i = 0; i < Mqtt.message.length; i++) {
    json[i] = Mqtt.message.content[i];
  }
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject &root = jsonBuffer.parseObject(json);

  if (root.success()) {
    command.domoticz.idx = root["idx"];
    command.nvalue = root["nvalue"];
#ifdef DEBUG
    Serial << endl
           << endl
           << "------------------------------------" << endl
           << "DOMOTICZ: Got command: " << command.nvalue
           << ", IDX: " << command.domoticz.idx;
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl
           << "Error with JSON PHarsing"
           << "------------------------------------";
  }
#endif
  return command;
}
#endif

#ifdef AFE_CONFIG_API_PROCESS_MQTT_REQUESTS
void AFEAPIMQTTDomoticz::processRequest() {
  DOMOTICZ_MQTT_COMMAND command = getCommand();
  if (command.domoticz.idx > 0 && idxForProcessing(command.domoticz.idx)) {
#ifdef DEBUG
    Serial << endl << "Processing ... ";
#endif

#ifdef DEBUG
    uint8_t _found = false;
#endif
    for (uint8_t i = 0; i < AFE_CONFIG_API_DOMOTICZ_IDX_CACHE_LENGTH; i++) {
      if (idxCache[i].domoticz.idx == command.domoticz.idx) {
        switch (idxCache[i].type) {
        /* Processing Relay command*/
        case AFE_DOMOTICZ_DEVICE_RELAY:
#ifdef DEBUG
          Serial << endl << " - found Relay ID: " << idxCache[i].id;
          _found = true;
#endif
          if (_Relay[idxCache[i].id]->get() != (byte)command.nvalue) {
            if (command.nvalue == AFE_CONFIG_API_DOMOTICZ_SWITCH_OFF) {
              _Relay[i]->off();
            } else {
              _Relay[i]->on();
            }
            publishRelayState(i);
          }
#ifdef DEBUG
          else {
            Serial << endl << " - same state. No change needed";
          }
#endif
          break;
        /* Processing Unknown command*/
        default:
#ifdef DEBUG
          Serial << endl
                 << " - ERROR: Device type not handled. Type: "
                 << idxCache[i].type;
#endif
          break;
        }
      }
    }
#ifdef DEBUG
    if (!_found) {
      Serial << endl << " - no item found with idx: " << command.domoticz.idx;
    }
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl
           << (command.domoticz.idx > 0 ? " - bypassing IDX: " : " - no IDX: ")
           << command.domoticz.idx;
  }
  Serial << endl << "------------------------------------";
#endif
}
#endif

#ifdef AFE_CONFIG_API_PROCESS_MQTT_REQUESTS
boolean AFEAPIMQTTDomoticz::idxForProcessing(uint32_t idx) {
  boolean _ret = true;
  if (idx == bypassProcessing.idx) {
    bypassProcessing.idx = 0;
    _ret = false;
  }
  return _ret;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
void AFEAPIMQTTDomoticz::addClass(AFERelay *Relay) {
#ifdef DEBUG
  Serial << endl << endl << "INFO: adding Relay's IDXs to Cache";
#endif

  // Cache idx
  uint8_t index = 0;
  for (uint8_t i = 0; i < _Device->configuration.noOfRelays; i++) {
    _Relay[i] = Relay + i;
#ifdef DEBUG
    Serial << endl << " - Relay[" << i << "]: " << _Relay[i]->getName() << " ";
#endif
    if (_Relay[i]->configuration.domoticz.idx > 0) {
      idxCache[index].domoticz.idx = _Relay[i]->configuration.domoticz.idx;
      idxCache[index].id = i;
      idxCache[index].type = AFE_DOMOTICZ_DEVICE_RELAY;
#ifdef DEBUG
      Serial << "added IDX: " << idxCache[index].domoticz.idx;
#endif
      index++;
    }
#ifdef DEBUG
    else {
      Serial << endl << "IDX not set";
    }
#endif
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
boolean AFEAPIMQTTDomoticz::publishRelayState(uint8_t id) {
  boolean publishStatus = false;
  if (enabled && _Relay[id]->configuration.domoticz.idx > 0) {
    char json[AFE_CONFIG_API_JSON_SWITCH_COMMAND_LENGTH];
    generateSwitchMessage(json, _Relay[id]->configuration.domoticz.idx,
                          _Relay[id]->get());
    publishStatus = Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    bypassProcessing.idx = _Relay[id]->configuration.domoticz.idx;
  }
  return publishStatus;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
void AFEAPIMQTTDomoticz::addClass(AFESwitch *Switch) {
  for (uint8_t i = 0; i < _Device->configuration.noOfSwitches; i++) {
    _Switch[i] = Switch + i;
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
boolean AFEAPIMQTTDomoticz::publishSwitchState(uint8_t id) {
  boolean publishStatus = false;
  if (enabled && _Switch[id]->configuration.domoticz.idx) {
    char json[AFE_CONFIG_API_JSON_SWITCH_COMMAND_LENGTH];

    generateSwitchMessage(json, _Switch[id]->configuration.domoticz.idx,
                          _Switch[id]->getPhisicalState() == 1
                              ? AFE_CONFIG_API_DOMOTICZ_SWITCH_OFF
                              : AFE_CONFIG_API_DOMOTICZ_SWITCH_ON);
    publishStatus = Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
  }
  return publishStatus;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
void AFEAPIMQTTDomoticz::addClass(AFEAnalogInput *Analog) { _Analog = Analog; }
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
void AFEAPIMQTTDomoticz::publishADCValues() {
  if (enabled) {
    char json[AFE_CONFIG_API_JSON_DEVICE_COMMAND_LENGTH];
    char value[20];
    if (_Analog->configuration.domoticz.percent > 0) {
      sprintf(value, "%-.2f", _Analog->data.percent);
      generateDeviceValue(json, _Analog->configuration.domoticz.percent, value);
      Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
    if (_Analog->configuration.domoticz.voltage > 0) {
      sprintf(value, "%-.4f", _Analog->data.voltage);
      generateDeviceValue(json, _Analog->configuration.domoticz.voltage, value);
      Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
    if (_Analog->configuration.domoticz.voltageCalculated > 0) {
      sprintf(value, "%-.4f", _Analog->data.voltageCalculated);
      generateDeviceValue(
          json, _Analog->configuration.domoticz.voltageCalculated, value);
      Mqtt.publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
    if (_Analog->configuration.domoticz.raw > 0) {
      sprintf(value, "%-d", _Analog->data.raw);
      generateDeviceValue(json, _Analog->configuration.domoticz.raw, value);
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
                                             char *value) {
  sprintf(json,
          "{\"command\":\"udevice\",\"idx\":%d,\"nvalue\":0,\"svalue\":\"%s\"}",
          idx, value);
}
