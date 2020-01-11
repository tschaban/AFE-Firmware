/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-API-MQTT-Domoticz.h"

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED

AFEAPIMQTTDomoticz::AFEAPIMQTTDomoticz() : AFEAPI(){};

#ifdef AFE_CONFIG_HARDWARE_LED
void AFEAPIMQTTDomoticz::begin(AFEDataAccess *Data, AFEDevice *Device,
                               AFELED *Led) {
  AFEAPI::begin(Data, Device, Led);
}

#else
void AFEAPIMQTTDomoticz::begin(AFEDataAccess *Data, AFEDevice *Device) {
  AFEAPI::begin(Data, Device);
}
#endif

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
    publishRelayState(i);
  }
#endif

/* Synchronize: Switch */
#ifdef AFE_CONFIG_HARDWARE_SWITCH
  for (uint8_t i = 0; i < _Device->configuration.noOfSwitches; i++) {
    publishSwitchState(i);
  }
#endif
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
    Serial << endl
           << "ERROR: Domoticz: Problem with JSON pharsing";
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
    for (uint8_t i = 0; i < AFE_CONFIG_API_DOMOTICZ_IDX_CACHE_LENGTH; i++) {
      if (idxCache[i].domoticz.idx == command.domoticz.idx) {
        switch (idxCache[i].type) {
        /* Processing Relay command*/
        case AFE_DOMOTICZ_DEVICE_RELAY:
#ifdef DEBUG
          Serial << endl << "INFO: Domoticz: Found Relay ID: " << idxCache[i].id;
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
            Serial << endl << "WARN: Domoticz: Same state. No change needed";
          }
#endif
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
      Serial << endl << "WARN: Domoticz: No item found with IDX: " << command.domoticz.idx;
    }
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl
           << (command.domoticz.idx > 0 ? "INFO: Domoticz: Bypassing IDX: " : " - no IDX: ")
           << command.domoticz.idx;
  }
#endif
}
#endif

#ifdef AFE_CONFIG_API_PROCESS_REQUESTS
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
  AFEAPI::addClass(Relay);

#ifdef DEBUG
  Serial << endl << "INFO: Caching IDXs for Relays";
#endif

  uint8_t index = 0;
  for (uint8_t i = 0; i < _Device->configuration.noOfRelays; i++) {
    if (_Relay[i]->configuration.domoticz.idx > 0) {
      idxCache[index].domoticz.idx = _Relay[i]->configuration.domoticz.idx;
      idxCache[index].id = i;
      idxCache[index].type = AFE_DOMOTICZ_DEVICE_RELAY;
#ifdef DEBUG
      Serial << endl << " - added IDX: " << idxCache[index].domoticz.idx;
#endif
      index++;
    }
#ifdef DEBUG
    else {
      Serial << endl << " - IDX not set";
    }
#endif
  }
}
#endif

#ifdef AFE_CONFIG_API_PROCESS_REQUESTS
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
  AFEAPI::addClass(Switch);
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
                                             char *value) {
  sprintf(json,
          "{\"command\":\"udevice\",\"idx\":%d,\"nvalue\":0,\"svalue\":\"%s\"}",
          idx, value);
}

#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED