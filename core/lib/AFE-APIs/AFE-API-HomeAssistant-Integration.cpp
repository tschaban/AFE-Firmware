#include "AFE-API-HomeAssistant-Integration.h"

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_HOME_ASSISTANT

AFEAPIHomeAssistantIntegration::AFEAPIHomeAssistantIntegration(
    AFEDataAccess *Data, AFEDevice *Device, AFEAPIMQTTStandard *MqttAPI) {
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
};

void AFEAPIHomeAssistantIntegration::publish() {

#ifdef AFE_CONFIG_HARDWARE_RELAY
  publishRelays();
#endif

#ifdef AFE_CONFIG_HARDWARE_CLED
  publishCLEDs();
#endif

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
  publishAnalogInputs();
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  publishSwitches();
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
  publishSensorDS18B20();
#endif

#ifdef AFE_CONFIG_HARDWARE_DHT
  publishSensorDHT();
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
  publishThermalProtector();
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  publishRegulator();
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  publishBMX80();
#endif
}

#ifdef AFE_CONFIG_HARDWARE_RELAY
void AFEAPIHomeAssistantIntegration::publishRelays(void) {

  if (!_initialize) {
    return;
  }

  RELAY _configuration;
  _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_RELAY;
  sprintf(_deviceConfiguration.deviceClass, AFE_CONFIG_HA_DEVICE_CLASS_RELAY);
  _deviceConfiguration.entityId = AFE_CONFIG_HA_TYPE_OF_ENTITY_SWITCH;

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS; i++) {
#ifdef DEBUG
    Serial << endl << F("INFO: HA: Setting/Updating Relay: ") << i + 1;
#endif

    _deviceConfiguration.id = i;

    if (i < _Device->configuration.noOfRelays) {
      _Data->getConfiguration(i, &_configuration);
      sprintf(_deviceConfiguration.mqtt.topic, _configuration.mqtt.topic);
      sprintf(_deviceConfiguration.label, _configuration.name);

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

  sprintf(_deviceConfiguration.deviceClass, AFE_CONFIG_HA_DEVICE_CLASS_SWITCH);

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
      _deviceConfiguration.entityId = AFE_CONFIG_HA_TYPE_OF_ENTITY_LIGHT;

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
      _deviceConfiguration.entityId = AFE_CONFIG_HA_TYPE_OF_ENTITY_SELECT;
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

  ADCINPUT _configuration;

#ifdef AFE_ESP32
  char _label[17 + 2 + sizeof(_configuration.name)];
#endif

  sprintf(_deviceConfiguration.deviceClass, AFE_CONFIG_HA_DEVICE_CLASS_VOLTAGE);
  _deviceConfiguration.entityId = AFE_CONFIG_HA_TYPE_OF_ENTITY_SENSOR;

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_ADCS; i++) {

    _deviceConfiguration.id = i;

#ifdef AFE_ESP32
    if (i < _Device->configuration.noOfAnalogInputs) {
      _Data->getConfiguration(i, &_configuration);
#ifdef DEBUG
      Serial << endl << F("INFO: HA: Setting/Updating Analog Input: ") << i + 1;
#endif

#else // ESP8266
    if (_Device->configuration.isAnalogInput) {
#ifdef DEBUG
      Serial << endl << F("INFO: HA: Setting/Updating Analog Input");
#endif
      _Data->getConfiguration(&_configuration);
#endif // ESP32/ESP8266

/* Voltage on input */
#ifdef AFE_ESP32
      sprintf(_deviceConfiguration.label, "%s: %s", _configuration.name,
              L_ADC_HA_VOLTAGE);
#else
      sprintf(_deviceConfiguration.label, "%s", L_ADC_HA_VOLTAGE);
#endif

      sprintf(_deviceConfiguration.mqtt.topic, _configuration.mqtt.topic);
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_ADC_VOLTAGE;
      sprintf(_deviceConfiguration.unit, AFE_UNIT_VOLTAGE);
      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);

/* Voltage calulcated */
#ifdef AFE_ESP32
      sprintf(_deviceConfiguration.label, "%s: %s", _configuration.name,
              L_ADC_HA_VOLTAGE_CALCULATED);
#else
      sprintf(_deviceConfiguration.label, "%s", L_ADC_HA_VOLTAGE_CALCULATED);
#endif

      _deviceConfiguration.type =
          AFE_CONFIG_HA_ITEM_SENSOR_ADC_VOLTAGE_CALCULATED;
      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);

/* Raw value on input */
#ifdef AFE_ESP32
      sprintf(_deviceConfiguration.label, "%s: %s", _configuration.name,
              L_ADC_HA_VOLTAGE_RAW);
#else
      sprintf(_deviceConfiguration.label, "%s", L_ADC_HA_VOLTAGE_RAW);
#endif
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_ADC_RAW;
      // sprintf(_deviceConfiguration.unit, "");
      _deviceConfiguration.unit[0] = AFE_EMPTY_STRING;
      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);

/* Percent value on input using input range */
#ifdef AFE_ESP32
      sprintf(_deviceConfiguration.label, "%s: %s", _configuration.name,
              L_ADC_HA_VOLTAGE_PERCENT);
#else
      sprintf(_deviceConfiguration.label, "%s", L_ADC_HA_VOLTAGE_PERCENT);
#endif

      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_ADC_PERCENT;
      sprintf(_deviceConfiguration.unit, AFE_UNIT_PERCENT);
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

  SWITCH _configuration;
  _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SWITCH;
  sprintf(_deviceConfiguration.deviceClass, AFE_CONFIG_HA_DEVICE_CLASS_SWITCH);
  _deviceConfiguration.entityId =
      AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_OPEN_CLOSED;

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES; i++) {
#ifdef DEBUG
    Serial << endl << F("INFO: HA: Setting/Updating Switch: ") << i + 1;
#endif

    _deviceConfiguration.id = i;

    if (i < _Device->configuration.noOfSwitches) {
      _Data->getConfiguration(i, &_configuration);
      sprintf(_deviceConfiguration.label, "%s: %d", L_SWITCH, i + 1);
      sprintf(_deviceConfiguration.mqtt.topic, _configuration.mqtt.topic);
      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);
    } else {
      removeItemRemovedFromHomeAssistantMQTTDiscovery(&_deviceConfiguration);
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_SWITCH

#ifdef AFE_CONFIG_HARDWARE_DS18B20
void AFEAPIHomeAssistantIntegration::publishSensorDS18B20(void) {
  if (!_initialize) {
    return;
  }
  DS18B20 _configuration;
  _deviceConfiguration.entityId = AFE_CONFIG_HA_TYPE_OF_ENTITY_SENSOR;
  _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_TEMPERATURE;
  sprintf(_deviceConfiguration.deviceClass,
          AFE_CONFIG_HA_DEVICE_CLASS_TEMPERATURE);

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_DS18B20; i++) {

    _deviceConfiguration.id = i;

    if (i < _Device->configuration.noOfDS18B20s) {
      _Data->getConfiguration(i, &_configuration);
#ifdef DEBUG
      Serial << endl << F("INFO: HA: Setting/Updating DS18B20: ") << i + 1;
#endif

      sprintf(_deviceConfiguration.label, _configuration.name);
      sprintf(_deviceConfiguration.mqtt.topic, _configuration.mqtt.topic);
      sprintf(_deviceConfiguration.unit,
              _configuration.unit == AFE_TEMPERATURE_UNIT_CELSIUS
                  ? AFE_UNIT_TEMPERATURE_C
                  : AFE_UNIT_TEMPERATURE_F);
      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);
    } else {
      removeItemRemovedFromHomeAssistantMQTTDiscovery(&_deviceConfiguration);
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_DS18B20

#ifdef AFE_CONFIG_HARDWARE_DHT
/**
 * @brief publishes configuration of all DHT sensors
 *
 */
void AFEAPIHomeAssistantIntegration::publishSensorDHT(void) {
  if (!_initialize) {
    return;
  }
  DHT _configuration;

  _deviceConfiguration.entityId = AFE_CONFIG_HA_TYPE_OF_ENTITY_SENSOR;

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_DHT; i++) {

    _deviceConfiguration.id = i;

    if (i < _Device->configuration.noOfDHTs) {
      _Data->getConfiguration(i, &_configuration);
#ifdef DEBUG
      Serial << endl << F("INFO: HA: Setting/Updating DHT: ") << i + 1;
#endif
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_TEMPERATURE;
      sprintf(_deviceConfiguration.label, "%s: %s", _configuration.name,
              L_TEMPERATURE);
      sprintf(_deviceConfiguration.mqtt.topic, _configuration.mqtt.topic);
      sprintf(_deviceConfiguration.unit,
              _configuration.temperature.unit == AFE_TEMPERATURE_UNIT_CELSIUS
                  ? AFE_UNIT_TEMPERATURE_C
                  : AFE_UNIT_TEMPERATURE_F);
      sprintf(_deviceConfiguration.deviceClass,
              AFE_CONFIG_HA_DEVICE_CLASS_TEMPERATURE);
      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);

      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_HEAT_INDEX;
      sprintf(_deviceConfiguration.label, "%s: %s", _configuration.name,
              L_HEAT_INDEX);
      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);

      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_DEW_POINT;
      sprintf(_deviceConfiguration.label, "%s: %s", _configuration.name,
              L_DEW_POINT);
      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);

      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_HUMIDITY;
      sprintf(_deviceConfiguration.label, "%s: %s", _configuration.name,
              L_HUMIDITY);
      sprintf(_deviceConfiguration.unit, AFE_UNIT_PERCENT);
      sprintf(_deviceConfiguration.deviceClass,
              AFE_CONFIG_HA_DEVICE_CLASS_HUMIDITY);
      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);

      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_ABSOLUTE_HUMIDITY;
      sprintf(_deviceConfiguration.label, "%s: %s", _configuration.name,
              L_ABSOLOUTE_HUMIDITY);
      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);

      _deviceConfiguration.unit[0] = AFE_EMPTY_STRING;

      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_PERCEPTION;
      sprintf(_deviceConfiguration.label, "%s: %s", _configuration.name,
              L_PERCEPTION);
      sprintf(_deviceConfiguration.deviceClass,
              AFE_CONFIG_HA_DEVICE_CLASS_NONE);
      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);

      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_COMFORT_LEVEL;
      sprintf(_deviceConfiguration.label, "%s: %s", _configuration.name,
              L_COMFORT_LEVEL);
      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);

    } else {
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_TEMPERATURE;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(&_deviceConfiguration);
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_HUMIDITY;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(&_deviceConfiguration);
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_ABSOLUTE_HUMIDITY;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(&_deviceConfiguration);
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_HEAT_INDEX;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(&_deviceConfiguration);
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_DEW_POINT;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(&_deviceConfiguration);
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_PERCEPTION;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(&_deviceConfiguration);
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_COMFORT_LEVEL;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(&_deviceConfiguration);
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_DHT

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
void AFEAPIHomeAssistantIntegration::publishThermalProtector(void) {
  if (!_initialize) {
    return;
  }
  THERMAL_PROTECTOR _configuration;

  _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_THERMAL_PROTECTOR;
  sprintf(_deviceConfiguration.deviceClass, AFE_CONFIG_HA_DEVICE_CLASS_SWITCH);
  _deviceConfiguration.entityId = AFE_CONFIG_HA_TYPE_OF_ENTITY_SWITCH;

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_THERMAL_PROTECTORS;
       i++) {
#ifdef DEBUG
    Serial << endl
           << F("INFO: HA: Setting/Updating Thermal protector: ") << i + 1;
#endif

    _deviceConfiguration.id = i;

    if (i < _Device->configuration.noOfThermalProtectors) {
      _Data->getConfiguration(i, &_configuration);
      sprintf(_deviceConfiguration.mqtt.topic, _configuration.mqtt.topic);
      sprintf(_deviceConfiguration.label, _configuration.name);
      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);
    } else {
      removeItemRemovedFromHomeAssistantMQTTDiscovery(&_deviceConfiguration);
    }
  }
}
#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
void AFEAPIHomeAssistantIntegration::publishRegulator(void) {
  if (!_initialize) {
    return;
  }
  REGULATOR _configuration;
  _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_REGULATOR;
  sprintf(_deviceConfiguration.deviceClass, AFE_CONFIG_HA_DEVICE_CLASS_SWITCH);
  _deviceConfiguration.entityId = AFE_CONFIG_HA_TYPE_OF_ENTITY_SWITCH;

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_REGULATORS; i++) {
#ifdef DEBUG
    Serial << endl << F("INFO: HA: Setting/Updating Regulator: ") << i + 1;
#endif

    _deviceConfiguration.id = i;

    if (i < _Device->configuration.noOfRegulators) {
      _Data->getConfiguration(i, &_configuration);
      sprintf(_deviceConfiguration.mqtt.topic, _configuration.mqtt.topic);
      sprintf(_deviceConfiguration.label, _configuration.name);
      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);
    } else {
      removeItemRemovedFromHomeAssistantMQTTDiscovery(&_deviceConfiguration);
    }
  }
}
#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

#ifdef AFE_CONFIG_HARDWARE_BMEX80
void AFEAPIHomeAssistantIntegration::publishBMX80(void) {
  if (!_initialize) {
    return;
  }
  BMEX80 _configuration;
  _deviceConfiguration.entityId = AFE_CONFIG_HA_TYPE_OF_ENTITY_SENSOR;

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_BMEX80; i++) {

    _deviceConfiguration.id = i;

    if (i < _Device->configuration.noOfBMEX80s) {
      _Data->getConfiguration(i, &_configuration);
#ifdef DEBUG
      Serial << endl << F("INFO: HA: Setting/Updating Bosch sesnor: ") << i + 1;
#endif
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_TEMPERATURE;
      sprintf(_deviceConfiguration.mqtt.topic, _configuration.mqtt.topic);

      sprintf(_deviceConfiguration.label, "%s: %s", _configuration.name,
              L_TEMPERATURE);
      sprintf(_deviceConfiguration.unit,
              _configuration.temperature.unit == AFE_TEMPERATURE_UNIT_CELSIUS
                  ? AFE_UNIT_TEMPERATURE_C
                  : AFE_UNIT_TEMPERATURE_F);
      sprintf(_deviceConfiguration.deviceClass,
              AFE_CONFIG_HA_DEVICE_CLASS_TEMPERATURE);
      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);

      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_HEAT_INDEX;
      sprintf(_deviceConfiguration.label, "%s: %s", _configuration.name,
              L_HEAT_INDEX);
      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);

      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_DEW_POINT;
      sprintf(_deviceConfiguration.label, "%s: %s", _configuration.name,
              L_DEW_POINT);
      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);

      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_HUMIDITY;
      sprintf(_deviceConfiguration.label, "%s: %s", _configuration.name,
              L_HUMIDITY);
      sprintf(_deviceConfiguration.unit, AFE_UNIT_PERCENT);
      sprintf(_deviceConfiguration.deviceClass,
              AFE_CONFIG_HA_DEVICE_CLASS_HUMIDITY);
      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);

      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_ABSOLUTE_HUMIDITY;
      sprintf(_deviceConfiguration.label, "%s: %s", _configuration.name,
              L_ABSOLOUTE_HUMIDITY);
      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);

      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_RELATIVE_PRESSURE;
      sprintf(_deviceConfiguration.label, "%s: %s", _configuration.name,
              L_PRESSURE_RELATIVE);
      sprintf(_deviceConfiguration.unit, AFE_UNIT_PRESSURE);
      sprintf(_deviceConfiguration.deviceClass,
              AFE_CONFIG_HA_DEVICE_CLASS_PRESSURE);
      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);

      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_ABSOLUTE_PRESSURE;
      sprintf(_deviceConfiguration.label, "%s: %s", _configuration.name,
              L_PRESSURE);
      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);

      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_CO2_EQUIVALENT;
      sprintf(_deviceConfiguration.label, "%s: %s", _configuration.name,
              L_BMEX80_CO2);
      sprintf(_deviceConfiguration.unit, AFE_UNIT_CO2);
      sprintf(_deviceConfiguration.deviceClass, AFE_CONFIG_HA_DEVICE_CLASS_CO2);
      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);

      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_GAS_RESISTANCE;
      sprintf(_deviceConfiguration.label, "%s: %s", _configuration.name,
              L_BMEX80_GAS);
      sprintf(_deviceConfiguration.deviceClass, AFE_CONFIG_HA_DEVICE_CLASS_GAS);
      sprintf(_deviceConfiguration.unit, AFE_UNIT_KOM);
      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);

      _deviceConfiguration.unit[0] = AFE_EMPTY_STRING;

      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_IAQ;
      sprintf(_deviceConfiguration.label, "%s: %s", _configuration.name,
              L_BMEX80_IAQ);
      sprintf(_deviceConfiguration.deviceClass, AFE_CONFIG_HA_DEVICE_CLASS_AQI);
      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);

      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_STATIC_IAQ;
      sprintf(_deviceConfiguration.label, "%s: %s", _configuration.name,
              L_BMEX80_STATIC_IAQ);
      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);

      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_BVOC_EQUIVALENT;
      sprintf(_deviceConfiguration.label, "%s: %s", _configuration.name,
              L_BMEX80_BVOC);
      sprintf(_deviceConfiguration.deviceClass,
              AFE_CONFIG_HA_DEVICE_CLASS_NONE);
      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);

      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_PERCEPTION;
      sprintf(_deviceConfiguration.label, "%s: %s", _configuration.name,
              L_PERCEPTION);
      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);

      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_COMFORT_LEVEL;
      sprintf(_deviceConfiguration.label, "%s: %s", _configuration.name,
              L_COMFORT_LEVEL);
      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);

    } else {
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_TEMPERATURE;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(&_deviceConfiguration);
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_HUMIDITY;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(&_deviceConfiguration);
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_ABSOLUTE_HUMIDITY;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(&_deviceConfiguration);
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_HEAT_INDEX;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(&_deviceConfiguration);
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_DEW_POINT;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(&_deviceConfiguration);
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_PERCEPTION;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(&_deviceConfiguration);
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_COMFORT_LEVEL;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(&_deviceConfiguration);
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_RELATIVE_PRESSURE;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(&_deviceConfiguration);
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_ABSOLUTE_PRESSURE;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(&_deviceConfiguration);
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_IAQ;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(&_deviceConfiguration);
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_STATIC_IAQ;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(&_deviceConfiguration);
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_BVOC_EQUIVALENT;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(&_deviceConfiguration);
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_GAS_RESISTANCE;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(&_deviceConfiguration);
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_CO2_EQUIVALENT;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(&_deviceConfiguration);
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_BMEX80

#ifdef AFE_CONFIG_HARDWARE_BH1750
void AFEAPIHomeAssistantIntegration::publishBH1750(void) {
  if (!_initialize) {
    return;
  }
  BH1750_CONFIG _configuration;
  _deviceConfiguration.entityId = AFE_CONFIG_HA_TYPE_OF_ENTITY_SENSOR;
  _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_ILLUMINANCE;
  sprintf(_deviceConfiguration.deviceClass,
          AFE_CONFIG_HA_DEVICE_CLASS_ILLUMINANCE);
  _deviceConfiguration.entityId = AFE_CONFIG_HA_TYPE_OF_ENTITY_SENSOR;

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_BH1750; i++) {

    _deviceConfiguration.id = i;

    if (i < _Device->configuration.noOfBH1750s) {
      _Data->getConfiguration(i, &_configuration);
#ifdef DEBUG
      Serial << endl << F("INFO: HA: Setting/Updating BH1750: ") << i + 1;
#endif

      sprintf(_deviceConfiguration.label, _configuration.name);
      sprintf(_deviceConfiguration.mqtt.topic, _configuration.mqtt.topic);
      sprintf(_deviceConfiguration.unit, AFE_UNIT_LUX);
      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);
    } else {
      removeItemRemovedFromHomeAssistantMQTTDiscovery(&_deviceConfiguration);
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_BH1750

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
void AFEAPIHomeAssistantIntegration::publishHPMA115S0(void) {
  if (!_initialize) {
    return;
  }
  HPMA115S0 _configuration;
  _deviceConfiguration.entityId = AFE_CONFIG_HA_TYPE_OF_ENTITY_SENSOR;

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_HPMA115S0; i++) {

    _deviceConfiguration.id = i;

    if (i < _Device->configuration.noOfHPMA115S0s) {
      _Data->getConfiguration(i, &_configuration);
#ifdef DEBUG
      Serial << endl << F("INFO: HA: Setting/Updating HPMA115S0: ") << i + 1;
#endif

      sprintf(_deviceConfiguration.label, _configuration.name);
      sprintf(_deviceConfiguration.mqtt.topic, _configuration.mqtt.topic);

      /* PM10 */
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_PM10;
      sprintf(_deviceConfiguration.deviceClass,
              AFE_CONFIG_HA_DEVICE_CLASS_PM10);
      sprintf(_deviceConfiguration.unit, AFE_UNIT_PARTICLE);
      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);

      /* PM10 WHO Norm */
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_PM10_WHO;
      sprintf(_deviceConfiguration.unit, AFE_UNIT_PERCENT);
      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);

      /* PM25 WHO Norm */
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_PM25_WHO;
      sprintf(_deviceConfiguration.deviceClass,
              AFE_CONFIG_HA_DEVICE_CLASS_PM25);
      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);

      /* PM25 */
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_PM25;
      sprintf(_deviceConfiguration.unit, AFE_UNIT_PARTICLE);
      publishItemToHomeAssistantMQTTDiscovery(&_deviceConfiguration);

    } else {
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_PM10;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(&_deviceConfiguration);
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_PM10_WHO;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(&_deviceConfiguration);
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_PM25;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(&_deviceConfiguration);
      _deviceConfiguration.type = AFE_CONFIG_HA_ITEM_SENSOR_PM25;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(&_deviceConfiguration);
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_HPMA115S0

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
void AFEAPIHomeAssistantIntegration::publishAnemometer(void) {}
#endif // AFE_CONFIG_HARDWARE_ANEMOMETER

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
void AFEAPIHomeAssistantIntegration::publishRainmeter(void) {}
#endif // AFE_CONFIG_HARDWARE_RAINMETER

/******* Private Methods *******/

void AFEAPIHomeAssistantIntegration::generateObjectId(char *objectId,
                                                      uint8_t deviceClassId,
                                                      uint8_t id) {
  sprintf(objectId, "%s-%d%d", _Device->deviceId, deviceClassId, id);
}

void AFEAPIHomeAssistantIntegration::generateTopic(char *topic,
                                                   uint8_t entityId,
                                                   const char *objectId) {
  sprintf(
      topic, "%s/%s/%s/config", configuration.discovery.topic,
      entityId == AFE_CONFIG_HA_TYPE_OF_ENTITY_SWITCH
          ? AFE_CONFIG_HA_TYPE_OF_ENTITY_SWITCH_NAME
          : entityId == AFE_CONFIG_HA_TYPE_OF_ENTITY_SENSOR
                ? AFE_CONFIG_HA_TYPE_OF_ENTITY_SENSOR_NAME
                : entityId == AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_ON_OFF ||
                          entityId ==
                              AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_OPEN_CLOSED
                      ? AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_NAME
                      : entityId == AFE_CONFIG_HA_TYPE_OF_ENTITY_SELECT
                            ? AFE_CONFIG_HA_TYPE_OF_ENTITY_SELECT_NAME
                            : entityId == AFE_CONFIG_HA_TYPE_OF_ENTITY_LIGHT
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
    generateTopic(_topic, deviceConfiguration->entityId, _objectId);

    boolean _retain = _MqttAPI->Mqtt.configuration.retainAll;
    _MqttAPI->Mqtt.configuration.retainAll = configuration.retainConfiguration;
    _MqttAPI->Mqtt.publish(_topic, "");
    _MqttAPI->Mqtt.configuration.retainAll = _retain;
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

    char _topic[AFE_CONFIG_HA_PUBLISH_TOPIC_SIZE];
    char _message[AFE_CONFIG_HA_CONFIGURATION_JSON_SIZE];
    char _objectId[AFE_CONFIG_HA_OBJECT_ID_SIZE];

    generateObjectId(_objectId, deviceConfiguration->type,
                     deviceConfiguration->id);
    generateTopic(_topic, deviceConfiguration->entityId, _objectId);

    _json.concat(FPSTR(HA_MQTT_DISCOVERY_JSON_BODY));

    /**
     * @brief Checking if LWT configured in order to add availability
     * configuration
     *
     */

    if (strlen(_mqttConfiguration.lwt.topic) > 0) {
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_AVAILABILITY),
                    FPSTR(HA_MQTT_DISCOVERY_JSON_AVAILABILITY));
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_AVAILABILITY_TOPIC),
                    _mqttConfiguration.lwt.topic);
    } else {
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_AVAILABILITY), "");
    }

    /**
     * @brief Adds command_topic, retain
     *
     */
    if (_deviceConfiguration.entityId == AFE_CONFIG_HA_TYPE_OF_ENTITY_SWITCH ||
        _deviceConfiguration.entityId == AFE_CONFIG_HA_TYPE_OF_ENTITY_SELECT ||
        _deviceConfiguration.entityId == AFE_CONFIG_HA_TYPE_OF_ENTITY_LIGHT) {
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_BODY_COMMAND_TOPIC),
                    FPSTR(HA_MQTT_DISCOVERY_JSON_COMMAND_TOPIC));
    }

    /* adds state_topic */
    if (_deviceConfiguration.entityId == AFE_CONFIG_HA_TYPE_OF_ENTITY_SWITCH ||
        _deviceConfiguration.entityId ==
            AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_ON_OFF ||
        _deviceConfiguration.entityId ==
            AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_OPEN_CLOSED ||
        _deviceConfiguration.entityId == AFE_CONFIG_HA_TYPE_OF_ENTITY_SELECT ||
        _deviceConfiguration.entityId == AFE_CONFIG_HA_TYPE_OF_ENTITY_LIGHT) {
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_BODY_STATE_TOPIC),
                    FPSTR(HA_MQTT_DISCOVERY_JSON_STATE_TOPIC));
    }

    /* Adds state_on, state_off or state_value_template*/
    if (_deviceConfiguration.entityId == AFE_CONFIG_HA_TYPE_OF_ENTITY_SWITCH) {
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_BODY_STATE_ON_OFF_TPL),
                    FPSTR(HA_MQTT_DISCOVERY_JSON_STATE_ON_OFF));
#ifdef AFE_CONFIG_HARDWARE_CLED
    } else if (_deviceConfiguration.entityId ==
               AFE_CONFIG_HA_TYPE_OF_ENTITY_LIGHT) {
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_BODY_STATE_ON_OFF_TPL),
                    FPSTR(HA_MQTT_DISCOVERY_JSON_STATE_VALUE_TEMPLATE));
#endif
    }

    /* Adds payload_on=on, payload_off=off or payload command */
    if (_deviceConfiguration.entityId == AFE_CONFIG_HA_TYPE_OF_ENTITY_SWITCH ||
        _deviceConfiguration.entityId ==
            AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_ON_OFF) {
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_BODY_PAYLOAD_ON_OFF_CMD),
                    FPSTR(HA_MQTT_DISCOVERY_JSON_PAYLOAD_ON_OFF));
#ifdef AFE_CONFIG_HARDWARE_CLED
    } else if (_deviceConfiguration.entityId ==
               AFE_CONFIG_HA_TYPE_OF_ENTITY_LIGHT) {
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_BODY_PAYLOAD_ON_OFF_CMD),
                    FPSTR(HA_MQTT_DISCOVERY_JSON_PAYLOAD_TEMPLATE));

#endif
    }

    /* Adds payload_on=closed, payload_off=open */
    if (_deviceConfiguration.entityId ==
        AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_OPEN_CLOSED) {
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_BODY_PAYLOAD_ON_OFF_CMD),
                    FPSTR(HA_MQTT_DISCOVERY_JSON_COMMAND_OPEN_CLOSED));
    }

    /**
     * @brief tag: device_class
     *
     */
    if (strcmp(_deviceConfiguration.deviceClass,
               AFE_CONFIG_HA_DEVICE_CLASS_NONE) == 0) {
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_BODY_DEVICE_CLASS), "");
    } else {
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_BODY_DEVICE_CLASS),
                    FPSTR(HA_MQTT_DISCOVERY_JSON_DEVICE_CLASS));
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_DEVICE_CLASS),
                    _deviceConfiguration.deviceClass);
    }

    /* Sensor section */
    if (_deviceConfiguration.entityId == AFE_CONFIG_HA_TYPE_OF_ENTITY_SENSOR) {
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_SENSOR),
                    FPSTR(HA_MQTT_DISCOVERY_JSON_SENSOR_COMMON));

      if (false) {
      }
#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
      /* Sensor ADC: voltage and calculated voltage */
      else if (deviceConfiguration->type ==
                   AFE_CONFIG_HA_ITEM_SENSOR_ADC_VOLTAGE ||
               deviceConfiguration->type ==
                   AFE_CONFIG_HA_ITEM_SENSOR_ADC_VOLTAGE_CALCULATED) {
        _json.replace(
            F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
            deviceConfiguration->type == AFE_CONFIG_HA_ITEM_SENSOR_ADC_VOLTAGE
                ? F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_VOLTAGE)
                : F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_VOLTAGE_CALCULATED));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      } else if (deviceConfiguration->type ==
                 AFE_CONFIG_HA_ITEM_SENSOR_ADC_RAW) {
        /* Sensor ADC: RAW */
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_RAW));
      } else if (deviceConfiguration->type ==
                 AFE_CONFIG_HA_ITEM_SENSOR_ADC_PERCENT) {
        /* Sensor ADC: RAW Percentage */
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_PERCENT));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }
#endif // Analog Input

#if defined(AFE_CONFIG_HARDWARE_DS18B20) ||                                    \
    defined(AFE_CONFIG_HARDWARE_DHT) || defined(AFE_CONFIG_HARDWARE_BMEX80)
      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_TEMPERATURE) {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_TEMPERATURE));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }
#endif // Temperature

#if defined(AFE_CONFIG_HARDWARE_DHT) || defined(AFE_CONFIG_HARDWARE_BMEX80)
      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_HUMIDITY) {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_HUMIDITY));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }
#endif // Humidity

#if defined(AFE_CONFIG_HARDWARE_DHT) || defined(AFE_CONFIG_HARDWARE_BMEX80)
      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_ABSOLUTE_HUMIDITY) {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_ABSOLUTE_HUMIDITY));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }
#endif // Absolute Humidity

#if defined(AFE_CONFIG_HARDWARE_DHT) || defined(AFE_CONFIG_HARDWARE_BMEX80)
      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_DEW_POINT) {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_DEW_POINT));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }
#endif // Dew Point

#if defined(AFE_CONFIG_HARDWARE_DHT) || defined(AFE_CONFIG_HARDWARE_BMEX80)
      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_HEAT_INDEX) {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_HEAT_INDEX));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }
#endif // Heat Index

#if defined(AFE_CONFIG_HARDWARE_DHT) || defined(AFE_CONFIG_HARDWARE_BMEX80)
      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_PERCEPTION) {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_PERCEPTION));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }
#endif // Perception

#if defined(AFE_CONFIG_HARDWARE_DHT) || defined(AFE_CONFIG_HARDWARE_BMEX80)
      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_COMFORT_LEVEL) {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_COMFORT));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }
#endif // Comfort level

#if defined(AFE_CONFIG_HARDWARE_BMEX80)
      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_RELATIVE_PRESSURE) {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_RELATIVE_PRESSURE));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }
#endif // Relative pressure

#if defined(AFE_CONFIG_HARDWARE_BMEX80)
      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_ABSOLUTE_PRESSURE) {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_ABSOLUTE_PRESSURE));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }
#endif // Absolute pressure

#if defined(AFE_CONFIG_HARDWARE_BMEX80)
      else if (deviceConfiguration->type == AFE_CONFIG_HA_ITEM_SENSOR_IAQ) {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_IAQ));
      }
#endif // IAQ

#if defined(AFE_CONFIG_HARDWARE_BMEX80)
      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_STATIC_IAQ) {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_STATIC_IAQ));
      }
#endif // Static IAQ

#if defined(AFE_CONFIG_HARDWARE_BMEX80)
      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_CO2_EQUIVALENT) {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_CO2_EQUIVALENT));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }
#endif // CO2 Equivalent

#if defined(AFE_CONFIG_HARDWARE_BMEX80)
      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_BVOC_EQUIVALENT) {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_BVOC_EQUIVALENT));
      }
#endif // BVOC Equivalent

#if defined(AFE_CONFIG_HARDWARE_BMEX80)
      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_GAS_RESISTANCE) {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_GAS_RESISTANCE));
      }
#endif // Gas resistance

      /* Removed unused tags */
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE), "");
    }

    /**
     * @brief tag: retain
     *
     */
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_RETAIN_FLAG),
                  FPSTR(HA_MQTT_DISCOVERY_JSON_RETAIN_FLAG));

/**
 * @brief RGB LED specyfic
 *
 */

#ifdef AFE_CONFIG_HARDWARE_CLED

    /**
     * @brief tag: options
     *
     */
    if (_deviceConfiguration.entityId == AFE_CONFIG_HA_TYPE_OF_ENTITY_SELECT) {
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_BODY_OPTIONS),
                    FPSTR(HA_MQTT_DISCOVERY_JSON_OPTIONS));
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_OPTIONS),
                    deviceConfiguration->options);
    }

    /**
     * @brief RGB LED config
     *
     */
    if (_deviceConfiguration.entityId == AFE_CONFIG_HA_TYPE_OF_ENTITY_LIGHT) {
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_LIGHT_RGB),
                    FPSTR(HA_MQTT_DISCOVERY_JSON_RGB_LIGHT));
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_LIGHT_BRIGHTNESS),
                    String(AFE_CONFIG_HARDWARE_CLED_MAX_BRIGHTNESS));
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_LIGHT_TOPIC_SUFIX),
                    AFE_CONFIG_HARDWARE_CLED_BRIGHTNESS_CMD_TOPIC_SUFIX);

      _json.replace(F(HA_MQTT_DISCOVERY_TAG_OPTIMISTIC),
                    FPSTR(HA_MQTT_DISCOVERY_JSON_OPTIMISTIC));
    }

#endif

    /**
     * @brief Adding common values
     *
     */
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_DEVICE_ID), _Device->deviceId);
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_DEVICE_SOFTWARE), _firmwareName);
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_DEVICE_MANUFACTURER),
                  AFE_DEVICE_MANUFACTURER);
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_DEVICE_NAME),
                  _Device->configuration.name);
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_DEVICE_HARDWARE),
                  F(AFE_DEVICE_TYPE_NAME));
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIQUE_ID), _objectId);
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_NAME), deviceConfiguration->label);
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_STATE_TEMPLATE),
                  deviceConfiguration->mqtt.topic);
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_RETAIN_FLAG),
                  F("false")); // Setting retain flag to false @TODO HA

    /* Remove unused tags */
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_BODY_COMMAND_TOPIC), "");
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_BODY_PAYLOAD_ON_OFF_CMD), "");
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_BODY_STATE_TOPIC), "");
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_BODY_STATE_ON_OFF_TPL), "");
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_SENSOR), "");
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_BODY_OPTIONS), "");
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_LIGHT_RGB), "");
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_OPTIMISTIC), "");

#ifdef DEBUG
    Serial << endl
           << F("INFO: HA: Configuration JSON: ") << endl
           << _json << endl
           << endl;
#endif
    _json.toCharArray(_message, sizeof(_message) + 1);

    boolean _retain = _MqttAPI->Mqtt.configuration.retainAll;
    _MqttAPI->Mqtt.configuration.retainAll = configuration.retainConfiguration;
    _MqttAPI->Mqtt.publish(_topic, _message);
    _MqttAPI->Mqtt.configuration.retainAll = _retain;
  } else {

    removeItemRemovedFromHomeAssistantMQTTDiscovery(deviceConfiguration);
  }
}

#endif // AFE_CONFIG_API_HOME_ASSISTANT_ENABLED