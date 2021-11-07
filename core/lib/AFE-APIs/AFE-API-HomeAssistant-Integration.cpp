#include "AFE-API-HomeAssistant-Integration.h";

#ifdef AFE_CONFIG_API_HOME_ASSISTANT_ENABLED

AFEAPIHomeAssistantIntegration::AFEAPIHomeAssistantIntegration(void){};

void AFEAPIHomeAssistantIntegration::begin(AFEDataAccess *Data,
                                           AFEDevice *Device,
                                           AFEAPIMQTTStandard *MqttAPI) {
#ifdef DEBUG
  Serial << endl << F("INFO: HA: Initializing Home Assistant Discovery");
#endif

  _Data = Data;
  _Device = Device;
  _MqttAPI = MqttAPI;
  _Data->getDeviceUID().toCharArray(_deviceID, sizeof(_deviceID) + 1);
}

#ifdef AFE_CONFIG_HARDWARE_RELAY
void AFEAPIHomeAssistantIntegration::publishRelays(void) {
  RELAY configuration;
  for (uint8_t i = 0; i < _Device->configuration.noOfRelays; i++) {
#ifdef DEBUG
    Serial << endl << F("INFO: HA: Setting/Updating RGB Relay: ") << i + 1;
#endif
    _Data->getConfiguration(i, &configuration);
    publishItemToHomeAssistantMQTTDiscovery(i, AFE_CONFIG_HA_ITEM_RELAY,
                                            configuration.mqtt.topic,
                                            configuration.name);
  }
}
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_CLED
void AFEAPIHomeAssistantIntegration::publishCLEDs(void) {

  CLED configuration;
  CLED_EFFECT_BLINKING configurationEffectBlinkng;
  CLED_EFFECT_FADE_INOUT configurationEffectFadeInOut;
  CLED_EFFECT_WAVE configurationEffectWave;

  for (uint8_t i = 0; i < _Device->configuration.noOfCLEDs; i++) {

#ifdef DEBUG
    Serial << endl << F("INFO: HA: Setting/Updating RGB LEDs Strip: ") << i + 1;
#endif

    _Data->getConfiguration(i, &configuration);
    publishItemToHomeAssistantMQTTDiscovery(i, AFE_CONFIG_HA_ITEM_LIGHT,
                                            configuration.mqtt.topic,
                                            configuration.name);

    _Data->getConfiguration(i, &configurationEffectBlinkng);
    publishItemToHomeAssistantMQTTDiscovery(
        i, AFE_CONFIG_HA_ITEM_RGB_LED_EFFECT,
        configurationEffectBlinkng.mqtt.topic, configurationEffectBlinkng.name,
        AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING);

    _Data->getConfiguration(i, &configurationEffectFadeInOut);
    publishItemToHomeAssistantMQTTDiscovery(
        i, AFE_CONFIG_HA_ITEM_RGB_LED_EFFECT,
        configurationEffectFadeInOut.mqtt.topic,
        configurationEffectFadeInOut.name,
        AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT);

    _Data->getConfiguration(i, &configurationEffectWave);
    publishItemToHomeAssistantMQTTDiscovery(
        i, AFE_CONFIG_HA_ITEM_RGB_LED_EFFECT,
        configurationEffectWave.mqtt.topic, configurationEffectWave.name,
        AFE_CONFIG_HARDWARE_CLED_EFFECT_WAVE);
  }
}
#endif // AFE_CONFIG_HARDWARE_CLED

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
void AFEAPIHomeAssistantIntegration::publishAnalogInputs(void) {
  ADCINPUT configuration;

#ifdef AFE_ESP32
  char _label[17 + 2 + sizeof(configuration.name)]; // @TODO HA - check if
                                                    // doesn't increase the size
                                                    // of max label
  for (uint8_t i = 0; i < _Device->configuration.noOfAnalogInputs; i++) {
    _Data->getConfiguration(i, &configuration);
#ifdef DEBUG
    Serial << endl << F("INFO: HA: Setting/Updating Analog Input: ") << i + 1;
#endif
#else // ESP8266
  if (_Device->configuration.isAnalogInput) {
#ifdef DEBUG
    Serial << endl << F("INFO: HA: Setting/Updating Analog Input");
#endif
    uint8_t i = 0;   // needed to have the same code for ESP8266/32
    char _label[17]; // @TODO HA - check if doesn't increase the size of max
                     // label
    _Data->getConfiguration(&configuration);
#endif

#ifdef AFE_ESP32
    sprintf(_label, "%s: %s", configuration.name, L_ADC_HA_VOLTAGE);
#else
    sprintf(_label, "%s", L_ADC_HA_VOLTAGE);
#endif

    publishItemToHomeAssistantMQTTDiscovery(
        i, AFE_CONFIG_HA_ITEM_SENSOR_ADC_VOLTAGE, configuration.mqtt.topic,
        _label);

#ifdef AFE_ESP32
    sprintf(_label, "%s: %s", configuration.name, L_ADC_HA_VOLTAGE_CALCULATED);
#else
    sprintf(_label, "%s", L_ADC_HA_VOLTAGE_CALCULATED);
#endif

    publishItemToHomeAssistantMQTTDiscovery(
        i, AFE_CONFIG_HA_ITEM_SENSOR_ADC_VOLTAGE_CALCULATED,
        configuration.mqtt.topic, _label);

#ifdef AFE_ESP32
    sprintf(_label, "%s: %s", configuration.name,L_ADC_HA_VOLTAGE_RAW);
#else
    sprintf(_label, "%s", L_ADC_HA_VOLTAGE_RAW);
#endif

    publishItemToHomeAssistantMQTTDiscovery(
        i, AFE_CONFIG_HA_ITEM_SENSOR_ADC_RAW, configuration.mqtt.topic, _label);

#ifdef AFE_ESP32
    sprintf(_label, "%s: %s",configuration.name,L_ADC_HA_VOLTAGE_PERCENT);
#else
    sprintf(_label, "%s", L_ADC_HA_VOLTAGE_PERCENT);
#endif

    publishItemToHomeAssistantMQTTDiscovery(
        i, AFE_CONFIG_HA_ITEM_SENSOR_ADC_PERCENT, configuration.mqtt.topic,
        _label);
  }
}
#endif // AFE_CONFIG_HARDWARE_ANALOG_INPUT

#ifdef AFE_CONFIG_HARDWARE_SWITCH
void AFEAPIHomeAssistantIntegration::publishSwitches(void) {
  SWITCH configuration;
  char _label[13];
  for (uint8_t i = 0; i < _Device->configuration.noOfSwitches; i++) {
#ifdef DEBUG
    Serial << endl << F("INFO: HA: Setting/Updating Switch: ") << i + 1;
#endif
    _Data->getConfiguration(i, &configuration);
    sprintf(_label, "%s: %d", L_SWITCH, i + 1);
    publishItemToHomeAssistantMQTTDiscovery(i, AFE_CONFIG_HA_ITEM_SWITCH,
                                            configuration.mqtt.topic, _label);
  }
}
#endif // AFE_CONFIG_HARDWARE_SWITCH

/******* Private Methods *******/

uint8_t
AFEAPIHomeAssistantIntegration::getTypeOfHAEntity(uint8_t deviceClassId) {
  uint8_t _entityId;

  switch (deviceClassId) {
#ifdef AFE_CONFIG_HARDWARE_RELAY
  case AFE_CONFIG_HA_ITEM_RELAY:
    _entityId = AFE_CONFIG_HA_TYPE_OF_ENTITY_SWITCH;
    break;
#endif // AFE_CONFIG_HARDWARE_RELAY
#ifdef AFE_CONFIG_HARDWARE_SWITCH
  case AFE_CONFIG_HA_ITEM_SWITCH:
    _entityId = AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_OPEN_CLOSED;
    break;
#endif // AFE_CONFIG_HARDWARE_SWITCH
#ifdef AFE_CONFIG_HARDWARE_CLED
  case AFE_CONFIG_HA_ITEM_LIGHT:
    _entityId = AFE_CONFIG_HA_TYPE_OF_ENTITY_SWITCH;
    break;
  case AFE_CONFIG_HA_ITEM_RGB_LED_EFFECT:
    _entityId = AFE_CONFIG_HA_TYPE_OF_ENTITY_SWITCH;
    break;
#endif // AFE_CONFIG_HARDWARE_CLED
#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
  case AFE_CONFIG_HA_ITEM_SENSOR_ADC_VOLTAGE:
    _entityId = AFE_CONFIG_HA_TYPE_OF_ENTITY_SENSOR;
    break;
  case AFE_CONFIG_HA_ITEM_SENSOR_ADC_VOLTAGE_CALCULATED:
    _entityId = AFE_CONFIG_HA_TYPE_OF_ENTITY_SENSOR;
    break;
  case AFE_CONFIG_HA_ITEM_SENSOR_ADC_RAW:
    _entityId = AFE_CONFIG_HA_TYPE_OF_ENTITY_SENSOR;
    break;
  case AFE_CONFIG_HA_ITEM_SENSOR_ADC_PERCENT:
    _entityId = AFE_CONFIG_HA_TYPE_OF_ENTITY_SENSOR;
    break;

#endif // AFE_CONFIG_HARDWARE_ANALOG_INPUT
  default:
    _entityId = AFE_NONE;
    break;
  }

  return _entityId;
}

void AFEAPIHomeAssistantIntegration::generateObjectId(char *objectId,
                                                      uint8_t deviceClassId,
                                                      uint8_t id,
                                                      uint8_t supportingId) {
  sprintf(objectId, "%s-%d%d%d", _deviceID, deviceClassId, id, supportingId);
}

void AFEAPIHomeAssistantIntegration::generateTopic(char *topic,
                                                   uint8_t deviceClassId,
                                                   const char *objectId) {
  uint8_t _entityType = getTypeOfHAEntity(deviceClassId);
  sprintf(
      topic, "homeassistant/%s/%s/config",
      _entityType == AFE_CONFIG_HA_TYPE_OF_ENTITY_SWITCH
          ? AFE_CONFIG_HA_TYPE_OF_ENTITY_SWITCH_NAME
          : _entityType == AFE_CONFIG_HA_TYPE_OF_ENTITY_SENSOR
                ? AFE_CONFIG_HA_TYPE_OF_ENTITY_SENSOR_NAME
                : _entityType ==
                              AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_ON_OFF ||
                          _entityType ==
                              AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_OPEN_CLOSED
                      ? AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_NAME
                      : AFE_CONFIG_HA_TYPE_OF_ENTITY_UNKNOWN,
      objectId);
}

void AFEAPIHomeAssistantIntegration::generateDeviceRegistry(char *device) {
  FIRMWARE firmwareConfiguration;
  _Data->getConfiguration(&firmwareConfiguration);
  char _firmwareName[28];
  sprintf(_firmwareName, "AFE Firmware T%d-%s", AFE_FIRMWARE_TYPE,
          firmwareConfiguration.version);

  sprintf(device, "\"device\":{\"ids\":\"%s\",\"name\":\"%s"
                  "\",\"mf\":\"%s\","
                  "\"mdl\":\"%s\",\"sw\":\"%s\",\"via_device\":\"%s\"}",
          _deviceID, _Device->configuration.name, AFE_DEVICE_MANUFACTURER,
          AFE_DEVICE_TYPE_NAME, _firmwareName, _deviceID);
}

void AFEAPIHomeAssistantIntegration::generateAvailability(char *availability) {
  MQTT mqttConfiguration;
  _Data->getConfiguration(&mqttConfiguration);

  if (strlen(mqttConfiguration.lwt.topic) > 0) {
    sprintf(availability,
            "\"avty\":{\"topic\":\"%s\",\"payload_available\":\"connected"
            "\",\"payload_not_available\":\"disconnected\"}",
            mqttConfiguration.lwt.topic);
  } else {
    availability[0] = AFE_EMPTY_STRING;
  }
}

void AFEAPIHomeAssistantIntegration::publishItemToHomeAssistantMQTTDiscovery(
    uint8_t id, uint8_t type, const char *topic, const char *label,
    uint8_t supportingId) {

#ifdef DEBUG
  Serial << endl << F("INFO: HA: Setting/Updating item: ") << label;
#endif

  if (strlen(topic) > 0) {

    String _json = "";

    uint8_t _entityType = getTypeOfHAEntity(type);

    FIRMWARE firmwareConfiguration;
    _Data->getConfiguration(&firmwareConfiguration);
    char _firmwareName[28];
    sprintf(_firmwareName, "AFE Firmware T%d-%s", AFE_FIRMWARE_TYPE,
            firmwareConfiguration.version);

    char _topic[200];
    char _message[AFE_CONFIG_HA_CONFIGURATION_JSON_SIZE];
    char _objectId[AFE_CONFIG_HA_OBJECT_ID_SIZE];

    generateObjectId(_objectId, type, id, supportingId);
    generateTopic(_topic, type, _objectId);

    _json.concat(FPSTR(HA_MQTT_DISCOVERY_JSON_BODY));
    _json.replace("{{d.i}}", _deviceID);
    _json.replace("{{d.s}}", _firmwareName);
    _json.replace("{{d.m}}", AFE_DEVICE_MANUFACTURER);
    _json.replace("{{d.n}}", _Device->configuration.name);
    _json.replace("{{d.h}}", F(AFE_DEVICE_TYPE_NAME));
    _json.replace("{{i.i}}", _objectId);
    _json.replace("{{i.n}}", label);

    /**
     * @brief Checking if LWT configured in order to add availability
     * configuration
     *
     */
    MQTT mqttConfiguration;
    _Data->getConfiguration(&mqttConfiguration);

    if (strlen(mqttConfiguration.lwt.topic) > 0) {
      _json.replace("{{b.a}}", FPSTR(HA_MQTT_DISCOVERY_JSON_AVAILABILITY));
      _json.replace("{{a.t}}", mqttConfiguration.lwt.topic);
    } else {
      _json.replace("{{b.a}}", "");
    }

    /* Adds command_topic, state_on, state_off, retain */
    if (_entityType == AFE_CONFIG_HA_TYPE_OF_ENTITY_SWITCH) {
      _json.replace("{{bct}}", FPSTR(HA_MQTT_DISCOVERY_JSON_COMMAND_TOPIC));
      _json.replace("{{bsp}}", FPSTR(HA_MQTT_DISCOVERY_JSON_STATE_ON_OFF));
      _json.replace("{{b.retain}}", FPSTR(HA_MQTT_DISCOVERY_JSON_RETAIN_FLAG));
    }

    /* adds state_topic */
    if (_entityType == AFE_CONFIG_HA_TYPE_OF_ENTITY_SWITCH ||
        _entityType == AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_ON_OFF ||
        _entityType == AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_OPEN_CLOSED) {
      _json.replace("{{bst}}", FPSTR(HA_MQTT_DISCOVERY_JSON_STATE_TOPIC));
    }

    /* Adds payload_on=on, payload_off=off */
    if (_entityType == AFE_CONFIG_HA_TYPE_OF_ENTITY_SWITCH ||
        _entityType == AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_ON_OFF) {
      _json.replace("{{bcp}}", FPSTR(HA_MQTT_DISCOVERY_JSON_COMMAND_ON_OFF));
    }

    /* Adds payload_on=closed, payload_off=open */
    if (_entityType == AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_OPEN_CLOSED) {
      _json.replace("{{bcp}}",
                    FPSTR(HA_MQTT_DISCOVERY_JSON_COMMAND_OPEN_CLOSED));
    }

    /* Adds device_class */
    /* @TODO HA - check if all sensors will have device_class */
    if (_entityType == AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_ON_OFF ||
        _entityType == AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_OPEN_CLOSED ||
        type == AFE_CONFIG_HA_ITEM_SENSOR_ADC_VOLTAGE_CALCULATED ||
        type == AFE_CONFIG_HA_ITEM_SENSOR_ADC_VOLTAGE ||
        type == AFE_CONFIG_HA_ITEM_SENSOR_ADC_RAW ||
        type == AFE_CONFIG_HA_ITEM_SENSOR_ADC_PERCENT) {
      _json.replace("{{bdc}}", FPSTR(HA_MQTT_DISCOVERY_JSON_DEVICE_CLASS));
    }

    /* Sensor section */
    if (_entityType == AFE_CONFIG_HA_TYPE_OF_ENTITY_SENSOR) {
      _json.replace("{{b.sensor}}",
                    FPSTR(HA_MQTT_DISCOVERY_JSON_SENSOR_COMMON));

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
      /* Sensor ADC: voltage and calculated voltage */
      if (type == AFE_CONFIG_HA_ITEM_SENSOR_ADC_VOLTAGE ||
          type == AFE_CONFIG_HA_ITEM_SENSOR_ADC_VOLTAGE_CALCULATED) {
        _json.replace("{{s.vt}}", type == AFE_CONFIG_HA_ITEM_SENSOR_ADC_VOLTAGE
                                      ? "{{value_json.voltage}}"
                                      : "{{value_json.voltageCalculated}}");
        _json.replace("{{s.u}}", AFE_UNIT_VOLTAGE);
      } else if (type == AFE_CONFIG_HA_ITEM_SENSOR_ADC_RAW) {
        /* Sensor ADC: RAW */
        _json.replace("{{s.vt}}", "{{value_json.raw}}");
      } else if (type == AFE_CONFIG_HA_ITEM_SENSOR_ADC_PERCENT) {
        /* Sensor ADC: RAW Percentage */
        _json.replace("{{s.vt}}", "{{value_json.percent}}");
        _json.replace("{{s.u}}", AFE_UNIT_PERCENT);
      }
#endif // AFE_CONFIG_HARDWARE_ANALOG_INPUT

      /* Removed unused tags */
      _json.replace("{{s.u}}", "");
    }

    /* Device class */
    _json.replace(
        "{{s.dc}}",
        type == AFE_CONFIG_HA_ITEM_SENSOR_ADC_VOLTAGE ||
                type == AFE_CONFIG_HA_ITEM_SENSOR_ADC_VOLTAGE_CALCULATED ||
                type == AFE_CONFIG_HA_ITEM_SENSOR_ADC_RAW ||
                type == AFE_CONFIG_HA_ITEM_SENSOR_ADC_PERCENT
            ? AFE_CONFIG_HA_DEVICE_CLASS_VOLTAGE
            : _entityType == AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_ON_OFF
                  ? AFE_CONFIG_HA_DEVICE_CLASS_MOVING
                  : _entityType ==
                            AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_OPEN_CLOSED
                        ? AFE_CONFIG_HA_DEVICE_CLASS_OPENING
                        : "");

    // @TODO T7

    _json.replace("{{i.t}}", topic);
    _json.replace("{{r.f}}", "false"); // Setting retain flag to false @TODO HA

    /* Removed unused tags */

    _json.replace("{{bct}}", "");
    _json.replace("{{bcp}}", "");
    _json.replace("{{bst}}", "");
    _json.replace("{{bsp}}", "");
    _json.replace("{{b.retain}}", "");
    _json.replace("{{bdc}}", "");
    _json.replace("{{b.sensor}}", "");

    Serial << endl << endl << _json << endl << endl;
    Serial << endl << "length=" << _json.length();

    _json.toCharArray(_message, sizeof(_message));

    _MqttAPI->Mqtt.publish(_topic, _message);
  }
}

#endif // AFE_CONFIG_API_HOME_ASSISTANT_ENABLED