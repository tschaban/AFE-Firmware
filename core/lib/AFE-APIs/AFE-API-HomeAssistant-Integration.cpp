#include "AFE-API-HomeAssistant-Integration.h"

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_HOME_ASSISTANT 

AFEAPIHomeAssistantIntegration::AFEAPIHomeAssistantIntegration(void){};

void AFEAPIHomeAssistantIntegration::begin(AFEDataAccess *Data,
                                           AFEDevice *Device,
                                           AFEAPIMQTTStandard *MqttAPI) {
#ifdef DEBUG
  Serial << endl << F("INFO: HA: Initializing Home Assistant Discovery");
#endif

  _Data = Data;
  _Data->getConfiguration(&configuration);

  if (strlen(configuration.discovery.topic) > 0 &&
      (configuration.addingComponents || configuration.removeingComponents)) {
    _initialize = true;
    _Device = Device;
    _MqttAPI = MqttAPI;
    FIRMWARE firmwareConfiguration;
    _Data->getConfiguration(&firmwareConfiguration);
    sprintf(_firmwareName, "AFE Firmware T%d-%s", AFE_FIRMWARE_TYPE,
            firmwareConfiguration.version);
    firmwareConfiguration = {0};
    _Data->getConfiguration(&_mqttConfiguration);
  }
#ifdef DEBUG
  else {
    Serial << endl
           << F("WARN: HA: Integration not initialize due to configuration");
  }
#endif
}

#ifdef AFE_CONFIG_HARDWARE_RELAY
void AFEAPIHomeAssistantIntegration::publishRelays(void) {

  if (!_initialize) {
    return;
  }

  RELAY relayConfiguration;
  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS; i++) {
#ifdef DEBUG
    Serial << endl << F("INFO: HA: Setting/Updating Relay: ") << i + 1;
#endif

    _deviceConfiguration.id = i;
    _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_RELAY;

    if (i < _Device->configuration.noOfRelays) {
      _Data->getConfiguration(i, &relayConfiguration);
      sprintf(_deviceConfiguration.mqtt.topic, relayConfiguration.mqtt.topic);
      sprintf(_deviceConfiguration.label, relayConfiguration.name);

      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);
    } else {
      removeItemRemovedFromHomeAssistantMQTTDiscovery(&_deviceConfiguration);
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_CLED
void AFEAPIHomeAssistantIntegration::publishCLEDs(void) {
  if (!_initialize) {
    return;
  }

  CLED configurationCLED;
  CLED_EFFECT_BLINKING configurationEffectBlinkng;
  CLED_EFFECT_FADE_INOUT configurationEffectFadeInOut;
  CLED_EFFECT_WAVE configurationEffectWave;

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_STRIPS; i++) {

#ifdef DEBUG
    Serial << endl << F("INFO: HA: Setting/Updating RGB LEDs Strip: ") << i + 1;
#endif

    _deviceConfiguration.id = i;

    if (i < _Device->configuration.noOfCLEDs) {

      /**
       * @brief Preparing and publishing RGB LED On/Off
       *
       */

      _Data->getConfiguration(i, &configurationCLED);

      sprintf(_deviceConfiguration.mqtt.topic, configurationCLED.cled.topic);
      sprintf(_deviceConfiguration.label, configurationCLED.name);

      sprintf(_deviceConfiguration.brightnessCmd.topic, "%s%s",
              configurationCLED.cled.topic,
              AFE_CONFIG_HARDWARE_CLED_BRIGHTNESS_CMD_TOPIC_SUFIX);
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_LIGHT;

      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);

      /**
       * @brief Preparing and publishing RGB LED Effects
       *
       */
      _Data->getConfiguration(i, &configurationEffectBlinkng);
      _Data->getConfiguration(i, &configurationEffectFadeInOut);
      _Data->getConfiguration(i, &configurationEffectWave);
      sprintf(_deviceConfiguration.options, "\"%s\",\"%s\",\"%s\",\"%s\"",
              AFE_CONFIG_HARDWARE_CLED_EFFECT_CMD_OFF,
              configurationEffectBlinkng.name,
              configurationEffectFadeInOut.name, configurationEffectWave.name);

      sprintf(_deviceConfiguration.mqtt.topic, configurationCLED.effect.topic);
      sprintf(_deviceConfiguration.label, L_CLED_EFFECT_LABEL);
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_RGB_LED_EFFECT;

      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);

    } else {

      /**
       * @brief Removing configs for unused device items
       *
       */
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_LIGHT;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(&_deviceConfiguration);
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_RGB_LED_EFFECT;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(&_deviceConfiguration);
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_CLED

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
void AFEAPIHomeAssistantIntegration::publishAnalogInputs(void) {
  if (!_initialize) {
    return;
  }

  ADCINPUT configurationADC;

#ifdef AFE_ESP32
  char _label[17 + 2 + sizeof(configurationADC.name)];
#endif

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_ADCS; i++) {

    _deviceConfiguration.id = i;

#ifdef AFE_ESP32
    if (i < _Device->configuration.noOfAnalogInputs) {
      _Data->getConfiguration(i, &configurationADC);
#ifdef DEBUG
      Serial << endl << F("INFO: HA: Setting/Updating Analog Input: ") << i + 1;
#endif

#else // ESP8266
    if (_Device->configuration.isAnalogInput) {
#ifdef DEBUG
      Serial << endl << F("INFO: HA: Setting/Updating Analog Input");
#endif
      _Data->getConfiguration(&configurationADC);
#endif // ESP32/ESP8266

#ifdef AFE_ESP32
      sprintf(_deviceConfiguration.label, "%s: %s", configurationADC.name,
              L_ADC_HA_VOLTAGE);
#else
      sprintf(_deviceConfiguration.label, "%s", L_ADC_HA_VOLTAGE);
#endif

      sprintf(_deviceConfiguration.mqtt.topic, configurationADC.mqtt.topic);
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_ADC_VOLTAGE;

      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);

#ifdef AFE_ESP32
      sprintf(_deviceConfiguration.label, "%s: %s", configurationADC.name,
              L_ADC_HA_VOLTAGE_CALCULATED);
#else
      sprintf(_deviceConfiguration.label, "%s", L_ADC_HA_VOLTAGE_CALCULATED);
#endif

      _deviceConfiguration.type =
          AFE_CONFIG_HA_ITEM_SENSOR_ADC_VOLTAGE_CALCULATED;
      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);

#ifdef AFE_ESP32
      sprintf(_deviceConfiguration.label, "%s: %s", configurationADC.name,
              L_ADC_HA_VOLTAGE_RAW);
#else
      sprintf(_deviceConfiguration.label, "%s", L_ADC_HA_VOLTAGE_RAW);
#endif
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_ADC_RAW;
      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);

#ifdef AFE_ESP32
      sprintf(_deviceConfiguration.label, "%s: %s", configurationADC.name,
              L_ADC_HA_VOLTAGE_PERCENT);
#else
      sprintf(_deviceConfiguration.label, "%s", L_ADC_HA_VOLTAGE_PERCENT);
#endif

      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_ADC_PERCENT;
      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);
    } else {
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_ADC_VOLTAGE;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(&_deviceConfiguration);
      _deviceConfiguration.type =
          AFE_CONFIG_HA_ITEM_SENSOR_ADC_VOLTAGE_CALCULATED;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(&_deviceConfiguration);
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_ADC_RAW;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(&_deviceConfiguration);
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_ADC_PERCENT;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(&_deviceConfiguration);
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_ANALOG_INPUT

#ifdef AFE_CONFIG_HARDWARE_SWITCH
void AFEAPIHomeAssistantIntegration::publishSwitches(void) {

  if (!_initialize) {
    return;
  }

  SWITCH configurationSwitch;
  char _label[13];
  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_SWITCHES; i++) {
#ifdef DEBUG
    Serial << endl << F("INFO: HA: Setting/Updating Switch: ") << i + 1;
#endif

    _deviceConfiguration.id = i;
    _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SWITCH;

    if (i < _Device->configuration.noOfSwitches) {
      _Data->getConfiguration(i, &configurationSwitch);
      sprintf(_deviceConfiguration.label, "%s: %d", L_SWITCH, i + 1);
      sprintf(_deviceConfiguration.mqtt.topic, configurationSwitch.mqtt.topic);
      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);
    } else {
      removeItemRemovedFromHomeAssistantMQTTDiscovery(&_deviceConfiguration);
    }
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
    _entityId = AFE_CONFIG_HA_TYPE_OF_ENTITY_LIGHT;
    break;
  case AFE_CONFIG_HA_ITEM_RGB_LED_EFFECT:
    _entityId = AFE_CONFIG_HA_TYPE_OF_ENTITY_SELECT;
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
                                                      uint8_t id) {
  sprintf(objectId, "%s-%d%d", _Device->deviceId, deviceClassId, id);
}

void AFEAPIHomeAssistantIntegration::generateTopic(char *topic,
                                                   uint8_t deviceClassId,
                                                   const char *objectId) {
  uint8_t _entityType = getTypeOfHAEntity(deviceClassId);
  sprintf(
      topic, "%s/%s/%s/config", configuration.discovery.topic,
      _entityType == AFE_CONFIG_HA_TYPE_OF_ENTITY_SWITCH
          ? AFE_CONFIG_HA_TYPE_OF_ENTITY_SWITCH_NAME
          : _entityType == AFE_CONFIG_HA_TYPE_OF_ENTITY_SENSOR
                ? AFE_CONFIG_HA_TYPE_OF_ENTITY_SENSOR_NAME
                : _entityType ==
                              AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_ON_OFF ||
                          _entityType ==
                              AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_OPEN_CLOSED
                      ? AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_NAME
                      : _entityType == AFE_CONFIG_HA_TYPE_OF_ENTITY_SELECT
                            ? AFE_CONFIG_HA_TYPE_OF_ENTITY_SELECT_NAME
                            : _entityType == AFE_CONFIG_HA_TYPE_OF_ENTITY_LIGHT
                                  ? AFE_CONFIG_HA_TYPE_OF_ENTITY_LIGHT_NAME
                                  : AFE_CONFIG_HA_TYPE_OF_ENTITY_UNKNOWN,
      objectId);
}

void AFEAPIHomeAssistantIntegration::
    removeItemRemovedFromHomeAssistantMQTTDiscovery(
        HA_DEVICE_CONFIG *deviceConfiguration) {

  if (configuration.removeingComponents) {
#ifdef DEBUG
    Serial << endl << F("INFO: HA: Removeing item");
#endif
    char _topic[AFE_CONFIG_HA_PUBLISH_TOPIC_SIZE];
    char _objectId[AFE_CONFIG_HA_OBJECT_ID_SIZE];
    generateObjectId(_objectId, deviceConfiguration->type,
                     deviceConfiguration->id);
    generateTopic(_topic, deviceConfiguration->type, _objectId);
    _MqttAPI->Mqtt.publish(_topic, "");
  }
}

void AFEAPIHomeAssistantIntegration::publishItemToHomeAssistantMQTTDiscovery(
    HA_DEVICE_CONFIG *deviceConfiguration) {

#ifdef DEBUG
  Serial << endl
         << F("INFO: HA: Setting/Updating item: ")
         << deviceConfiguration->label;
#endif

  if (strlen(deviceConfiguration->mqtt.topic) > 0 &&
      configuration.addingComponents) {

    String _json = "";

    uint8_t _entityType = getTypeOfHAEntity(deviceConfiguration->type);

    char _topic[AFE_CONFIG_HA_PUBLISH_TOPIC_SIZE];
    char _message[AFE_CONFIG_HA_CONFIGURATION_JSON_SIZE];
    char _objectId[AFE_CONFIG_HA_OBJECT_ID_SIZE];

    generateObjectId(_objectId, deviceConfiguration->type,
                     deviceConfiguration->id);
    generateTopic(_topic, deviceConfiguration->type, _objectId);

    _json.concat(FPSTR(HA_MQTT_DISCOVERY_JSON_BODY));

    /**
     * @brief Checking if LWT configured in order to add availability
     * configuration
     *
     */

    if (strlen(_mqttConfiguration.lwt.topic) > 0) {
      _json.replace("{{b.a}}", FPSTR(HA_MQTT_DISCOVERY_JSON_AVAILABILITY));
      _json.replace("{{a.t}}", _mqttConfiguration.lwt.topic);
    } else {
      _json.replace("{{b.a}}", "");
    }

    /* Adds command_topic, retain */
    if (_entityType == AFE_CONFIG_HA_TYPE_OF_ENTITY_SWITCH ||
        _entityType == AFE_CONFIG_HA_TYPE_OF_ENTITY_SELECT ||
        _entityType == AFE_CONFIG_HA_TYPE_OF_ENTITY_LIGHT) {
      _json.replace("{{bct}}", FPSTR(HA_MQTT_DISCOVERY_JSON_COMMAND_TOPIC));
    }

    /* adds state_topic */
    if (_entityType == AFE_CONFIG_HA_TYPE_OF_ENTITY_SWITCH ||
        _entityType == AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_ON_OFF ||
        _entityType == AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_OPEN_CLOSED ||
        _entityType == AFE_CONFIG_HA_TYPE_OF_ENTITY_SELECT ||
        _entityType == AFE_CONFIG_HA_TYPE_OF_ENTITY_LIGHT) {
      _json.replace("{{bst}}", FPSTR(HA_MQTT_DISCOVERY_JSON_STATE_TOPIC));
    }

    /* Adds state_on, state_off or state_value_template*/
    if (_entityType == AFE_CONFIG_HA_TYPE_OF_ENTITY_SWITCH) {
      _json.replace("{{bsp}}", FPSTR(HA_MQTT_DISCOVERY_JSON_STATE_ON_OFF));
    } else if (_entityType == AFE_CONFIG_HA_TYPE_OF_ENTITY_LIGHT) {
      _json.replace("{{bsp}}",
                    FPSTR(HA_MQTT_DISCOVERY_JSON_STATE_VALUE_TEMPLATE));
    }

    /* Adds payload_on=on, payload_off=off or payload command */
    if (_entityType == AFE_CONFIG_HA_TYPE_OF_ENTITY_SWITCH ||
        _entityType == AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_ON_OFF) {
      _json.replace("{{bcp}}", FPSTR(HA_MQTT_DISCOVERY_JSON_PAYLOAD_ON_OFF));
    } else if (_entityType == AFE_CONFIG_HA_TYPE_OF_ENTITY_LIGHT) {
      _json.replace("{{bcp}}", FPSTR(HA_MQTT_DISCOVERY_JSON_PAYLOAD_TEMPLATE));
    }

    /* Adds payload_on=closed, payload_off=open */
    if (_entityType == AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_OPEN_CLOSED) {
      _json.replace("{{bcp}}",
                    FPSTR(HA_MQTT_DISCOVERY_JSON_COMMAND_OPEN_CLOSED));
    }

    /* Adds device_class */
    if (_entityType == AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_ON_OFF ||
        _entityType == AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_OPEN_CLOSED ||
        deviceConfiguration->type ==
            AFE_CONFIG_HA_ITEM_SENSOR_ADC_VOLTAGE_CALCULATED ||
        deviceConfiguration->type == AFE_CONFIG_HA_ITEM_SENSOR_ADC_VOLTAGE ||
        deviceConfiguration->type == AFE_CONFIG_HA_ITEM_SENSOR_ADC_RAW ||
        deviceConfiguration->type == AFE_CONFIG_HA_ITEM_SENSOR_ADC_PERCENT) {
      _json.replace("{{bdc}}", FPSTR(HA_MQTT_DISCOVERY_JSON_DEVICE_CLASS));
    }

    /* Sensor section */
    if (_entityType == AFE_CONFIG_HA_TYPE_OF_ENTITY_SENSOR) {
      _json.replace("{{sen}}", FPSTR(HA_MQTT_DISCOVERY_JSON_SENSOR_COMMON));

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
      /* Sensor ADC: voltage and calculated voltage */
      if (deviceConfiguration->type == AFE_CONFIG_HA_ITEM_SENSOR_ADC_VOLTAGE ||
          deviceConfiguration->type ==
              AFE_CONFIG_HA_ITEM_SENSOR_ADC_VOLTAGE_CALCULATED) {
        _json.replace("{{s.vt}}", deviceConfiguration->type ==
                                          AFE_CONFIG_HA_ITEM_SENSOR_ADC_VOLTAGE
                                      ? "{{value_json.voltage}}"
                                      : "{{value_json.voltageCalculated}}");
        _json.replace("{{s.u}}", AFE_UNIT_VOLTAGE);
      } else if (deviceConfiguration->type ==
                 AFE_CONFIG_HA_ITEM_SENSOR_ADC_RAW) {
        /* Sensor ADC: RAW */
        _json.replace("{{s.vt}}", "{{value_json.raw}}");
      } else if (deviceConfiguration->type ==
                 AFE_CONFIG_HA_ITEM_SENSOR_ADC_PERCENT) {
        /* Sensor ADC: RAW Percentage */
        _json.replace("{{s.vt}}", "{{value_json.percent}}");
        _json.replace("{{s.u}}", AFE_UNIT_PERCENT);
      }
#endif // AFE_CONFIG_HARDWARE_ANALOG_INPUT
      /* Removed unused tags */
      _json.replace("{{s.u}}", "");
    }

    /**
     * @brief tag: retain
     *
     */
    _json.replace("{{ret}}", FPSTR(HA_MQTT_DISCOVERY_JSON_RETAIN_FLAG));

    /**
     * @brief tag: device_class
     * 
     */
    _json.replace(
        "{{s.dc}}",
        deviceConfiguration->type == AFE_CONFIG_HA_ITEM_SENSOR_ADC_VOLTAGE ||
                deviceConfiguration->type ==
                    AFE_CONFIG_HA_ITEM_SENSOR_ADC_VOLTAGE_CALCULATED ||
                deviceConfiguration->type ==
                    AFE_CONFIG_HA_ITEM_SENSOR_ADC_RAW ||
                deviceConfiguration->type ==
                    AFE_CONFIG_HA_ITEM_SENSOR_ADC_PERCENT
            ? AFE_CONFIG_HA_DEVICE_CLASS_VOLTAGE
            : _entityType == AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_ON_OFF
                  ? AFE_CONFIG_HA_DEVICE_CLASS_MOVING
                  : _entityType ==
                            AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_OPEN_CLOSED
                        ? AFE_CONFIG_HA_DEVICE_CLASS_OPENING
                        : "");

/**
 * @brief RGB LED specyfic
 *
 */

#ifdef AFE_CONFIG_HARDWARE_CLED

    /**
     * @brief tag: options
     *
     */
    if (_entityType == AFE_CONFIG_HA_TYPE_OF_ENTITY_SELECT) {
      _json.replace("{{bdo}}", FPSTR(HA_MQTT_DISCOVERY_JSON_OPTIONS));
      _json.replace("{{o.o}}", deviceConfiguration->options);
    }

    /**
     * @brief RGB LED config
     *
     */
    if (_entityType == AFE_CONFIG_HA_TYPE_OF_ENTITY_LIGHT) {
      _json.replace("{{rgb}}", FPSTR(HA_MQTT_DISCOVERY_JSON_RGB_LIGHT));
      _json.replace("{{bsc}}", String(AFE_CONFIG_HARDWARE_CLED_MAX_BRIGHTNESS));
      _json.replace("{{i.ts}}",
                    AFE_CONFIG_HARDWARE_CLED_BRIGHTNESS_CMD_TOPIC_SUFIX);

      _json.replace("{{opt}}", FPSTR(HA_MQTT_DISCOVERY_JSON_OPTIMISTIC));
    }

#endif

    /**
     * @brief Adding common values
     *
     */
    _json.replace("{{d.i}}", _Device->deviceId);
    _json.replace("{{d.s}}", _firmwareName);
    _json.replace("{{d.m}}", AFE_DEVICE_MANUFACTURER);
    _json.replace("{{d.n}}", _Device->configuration.name);
    _json.replace("{{d.h}}", F(AFE_DEVICE_TYPE_NAME));
    _json.replace("{{i.i}}", _objectId);
    _json.replace("{{i.n}}", deviceConfiguration->label);
    _json.replace("{{i.t}}", deviceConfiguration->mqtt.topic);
    _json.replace("{{r.f}}", "false"); // Setting retain flag to false @TODO HA

    /* Remove unused tags */
    _json.replace("{{bct}}", "");
    _json.replace("{{bcp}}", "");
    _json.replace("{{bst}}", "");
    _json.replace("{{bsp}}", "");
    _json.replace("{{bdc}}", "");
    _json.replace("{{sen}}", "");
    _json.replace("{{bdo}}", "");
    _json.replace("{{rgb}}", "");
    _json.replace("{{opt}}", "");

#ifdef DEBUG
    Serial << endl
           << F("INFO: HA: Confoguration JSON: ") << endl
           << _json << endl
           << endl;
#endif
    _json.toCharArray(_message, sizeof(_message));
    _MqttAPI->Mqtt.publish(_topic, _message);
  } else {

    removeItemRemovedFromHomeAssistantMQTTDiscovery(deviceConfiguration);
  }
}

#endif // AFE_CONFIG_API_HOME_ASSISTANT_ENABLED