#include "AFE-API-HomeAssistant-Integration.h"

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_HOME_ASSISTANT

AFEAPIHomeAssistantIntegration::AFEAPIHomeAssistantIntegration(
    AFEFirmware *Firmware, AFEAPIMQTTStandard *MqttAPI)
{

  _Firmware = Firmware;
#ifdef DEBUG
  _Firmware->Debugger->printInformation(F("Initializing Discovery"), F("HA-API"));
#endif

  _Firmware->API->Flash->getConfiguration(configuration);

  if (strlen(configuration->discovery.topic) > 0 &&
      (configuration->addingComponents || configuration->removeingComponents))
  {
    _initialize = true;
    _MqttAPI = MqttAPI;
    sprintf(_firmwareName, "AFE Firmware T%d-%s", AFE_FIRMWARE_TYPE,
            _Firmware->Configuration->Version->installed_version);
    _Firmware->API->Flash->getConfiguration(&_mqttConfiguration);
  }
#ifdef DEBUG
  else
  {
    _Firmware->Debugger->printWarning(F("Integration not initialize due to lack of configuration"), F("HA-API"));
  }
#endif
};

#ifdef DEBUG
void AFEAPIHomeAssistantIntegration::printPublishDeviceItemMessage(const __FlashStringHelper *deviceItemName, uint8_t id)
{
  _Firmware->Debugger->printInformation(F("Updating configuration: "), F("HA-API"));
  Serial << deviceItemName;
  if (id != AFE_NONE)
  {
    Serial << F(": ") << (id + 1);
  }
}
#endif

void AFEAPIHomeAssistantIntegration::publish()
{

#ifdef DEBUG
  _Firmware->Debugger->printHeader();
  _Firmware->Debugger->printValue(F("Publishing Auto-Discovery configuration to MQTT"));
#endif

  publishFirmwareVersion();

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

#ifdef AFE_CONFIG_HARDWARE_BH1750
  publishBH1750();
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  publishHPMA115S0();
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
  publishRainmeter();
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
  publishAnemometer();
#endif

#ifdef AFE_CONFIG_HARDWARE_FS3000
  publishFS3000();
#endif

#ifdef AFE_CONFIG_HARDWARE_TSL2561
  publishTSL2561();
#endif

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
  publishBinarySensor();
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
  publishGate();
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  publishContactron();
#endif
  _Firmware->Debugger->printHeader();
  _Firmware->Debugger->printValue(F("Auto-Discovery configuration sent to MQTT"));
}

void AFEAPIHomeAssistantIntegration::publishFirmwareVersion(void)
{
  if (!_initialize)
  {
    return;
  }
  resetDeviceConfiguration();
  _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_FIRMWARE_STATUS;
  sprintf(_deviceConfiguration->deviceClass, (const char*)
          F(AFE_CONFIG_HA_DEVICE_CLASS_FIRMWARE));
  _deviceConfiguration->entityId = AFE_CONFIG_HA_TYPE_OF_ENTITY_UPDATE;

  sprintf(_deviceConfiguration->entityCategory, (const char*)
          F(AFE_CONFIG_HA_ENTITY_CATEGORY_CONFIG));

  sprintf(_deviceConfiguration->label, (const char*) F(L_FIRMWARE));
  sprintf(_deviceConfiguration->mqtt.topic,
          _MqttAPI->Mqtt->configuration->status.topic);
  publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);
}

#ifdef AFE_CONFIG_HARDWARE_RELAY
void AFEAPIHomeAssistantIntegration::publishRelays(void)
{

  if (!_initialize)
  {
    return;
  }
  resetDeviceConfiguration();
  RELAY _configuration;
  _deviceConfiguration->hardwareId = AFE_CONFIG_HA_HARDWARE_RELAY;
  _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_RELAY;
  sprintf(_deviceConfiguration->deviceClass, AFE_CONFIG_HA_DEVICE_CLASS_RELAY);
  _deviceConfiguration->entityId = AFE_CONFIG_HA_TYPE_OF_ENTITY_SWITCH;

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS; i++)
  {
#ifdef DEBUG
    printPublishDeviceItemMessage(F("Relay"), i);
#endif

    _deviceConfiguration->id = i;

    if (i < _Firmware->Device->configuration.noOfRelays)
    {
      _Firmware->API->Flash->getConfiguration(i, &_configuration);
      sprintf(_deviceConfiguration->mqtt.topic, _configuration.mqtt.topic);
      sprintf(_deviceConfiguration->label, _configuration.name);

      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);
    }
    else
    {
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_CLED
void AFEAPIHomeAssistantIntegration::publishCLEDs(void)
{
  if (!_initialize)
  {
    return;
  }

  CLED configurationCLED;
  CLED_EFFECT_BLINKING configurationEffectBlinkng;
  CLED_EFFECT_FADE_INOUT configurationEffectFadeInOut;
  CLED_EFFECT_WAVE configurationEffectWave;

  sprintf(_deviceConfiguration->deviceClass, AFE_CONFIG_HA_DEVICE_CLASS_SWITCH);
  _deviceConfiguration->hardwareId = AFE_CONFIG_HA_HARDWARE_CLED;

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_STRIPS; i++)
  {

#ifdef DEBUG
    printPublishDeviceItemMessage(F("RGB LED"), i);
#endif

    _deviceConfiguration->id = i;

    if (i < _Firmware->Device->configuration.noOfCLEDs)
    {

      /**
       * @brief Preparing and publishing RGB LED On/Off
       *
       */

      _Firmware->API->Flash->getConfiguration(i, &configurationCLED);

      sprintf(_deviceConfiguration->mqtt.topic, configurationCLED.cled.topic);
      sprintf(_deviceConfiguration->label, configurationCLED.name);

      sprintf(_deviceConfiguration->brightnessCmd.topic, "%s%s",
              configurationCLED.cled.topic,
              AFE_CONFIG_HARDWARE_CLED_BRIGHTNESS_CMD_TOPIC_SUFIX);
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_LIGHT;
      _deviceConfiguration->entityId = AFE_CONFIG_HA_TYPE_OF_ENTITY_LIGHT;

      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);

      /**
       * @brief Preparing and publishing RGB LED Effects
       *
       */
      _Firmware->API->Flash->getConfiguration(i, &configurationEffectBlinkng);
      _Firmware->API->Flash->getConfiguration(i, &configurationEffectFadeInOut);
      _Firmware->API->Flash->getConfiguration(i, &configurationEffectWave);
      sprintf(_deviceConfiguration->options, "\"%s\",\"%s\",\"%s\",\"%s\"",
              AFE_CONFIG_HARDWARE_CLED_EFFECT_CMD_OFF,
              configurationEffectBlinkng.name,
              configurationEffectFadeInOut.name, configurationEffectWave.name);

      sprintf(_deviceConfiguration->mqtt.topic, configurationCLED.effect.topic);
      sprintf(_deviceConfiguration->label, L_CLED_EFFECT_LABEL);
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_RGB_LED_EFFECT;
      _deviceConfiguration->entityId = AFE_CONFIG_HA_TYPE_OF_ENTITY_SELECT;
      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);
    }
    else
    {

      /**
       * @brief Removing configs for unused device items
       *
       */
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_LIGHT;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_RGB_LED_EFFECT;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_CLED

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
void AFEAPIHomeAssistantIntegration::publishAnalogInputs(void)
{
  if (!_initialize)
  {
    return;
  }

  ADCINPUT _configuration;

#ifdef AFE_ESP32
  char _label[17 + 2 + sizeof(_configuration.name)];
#endif

  _deviceConfiguration->entityId = AFE_CONFIG_HA_TYPE_OF_ENTITY_SENSOR;
  _deviceConfiguration->hardwareId = AFE_CONFIG_HA_HARDWARE_ADC;

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_ADCS; i++)
  {

    _deviceConfiguration->id = i;

#ifdef AFE_ESP32
    if (i < _Firmware->Device->configuration.noOfAnalogInputs)
    {
      _Firmware->API->Flash->getConfiguration(i, &_configuration);
#ifdef DEBUG
      printPublishDeviceItemMessage(F("ADC"), i);
#endif

#else // ESP8266
    if (_Firmware->Device->configuration.isAnalogInput)
    {
#ifdef DEBUG
      printPublishDeviceItemMessage(F("ADC"));
#endif
      _Firmware->API->Flash->getConfiguration(&_configuration);
#endif // ESP32/ESP8266

      sprintf(_deviceConfiguration->deviceClass,
              AFE_CONFIG_HA_DEVICE_CLASS_VOLTAGE);

/* Voltage on input */
#ifdef AFE_ESP32
      sprintf(_deviceConfiguration->label, "%s: %s", _configuration.name,
              L_ADC_HA_VOLTAGE);
#else
      sprintf(_deviceConfiguration->label, "%s", L_ADC_HA_VOLTAGE);
#endif

      sprintf(_deviceConfiguration->mqtt.topic, _configuration.mqtt.topic);
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_ADC_VOLTAGE;
      sprintf(_deviceConfiguration->unit, AFE_UNIT_VOLTAGE);
      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);

/* Voltage calulcated */
#ifdef AFE_ESP32
      sprintf(_deviceConfiguration->label, "%s: %s", _configuration.name,
              L_ADC_HA_VOLTAGE_CALCULATED);
#else
      sprintf(_deviceConfiguration->label, "%s", L_ADC_HA_VOLTAGE_CALCULATED);
#endif

      _deviceConfiguration->type =
          AFE_CONFIG_HA_ITEM_SENSOR_ADC_VOLTAGE_CALCULATED;
      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);

/* Raw value on input */
#ifdef AFE_ESP32
      sprintf(_deviceConfiguration->label, "%s: %s", _configuration.name,
              L_ADC_HA_VOLTAGE_RAW);
#else
      sprintf(_deviceConfiguration->label, "%s", L_ADC_HA_VOLTAGE_RAW);
#endif
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_ADC_RAW;
      // sprintf(_deviceConfiguration->unit, "");
      _deviceConfiguration->unit[0] = AFE_EMPTY_STRING;
      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);

/* Percent value on input using input range */
#ifdef AFE_ESP32
      sprintf(_deviceConfiguration->label, "%s: %s", _configuration.name,
              L_ADC_HA_VOLTAGE_PERCENT);
#else
      sprintf(_deviceConfiguration->label, "%s", L_ADC_HA_VOLTAGE_PERCENT);
#endif

      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_ADC_PERCENT;
      sprintf(_deviceConfiguration->unit, AFE_UNIT_PERCENT);
      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);

/* Battery */
#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
      sprintf(_deviceConfiguration->deviceClass,
              AFE_CONFIG_HA_DEVICE_CLASS_BATTERY);

      sprintf(_deviceConfiguration->mqtt.topic,
              _configuration.battery.mqtt.topic);

      /* Battery Percent */
      _deviceConfiguration->type =
          AFE_CONFIG_HA_ITEM_SENSOR_BATTERYMETER_PERCENT;
      sprintf(_deviceConfiguration->unit, AFE_UNIT_PERCENT);

/* Percent value on input using input range */
#ifdef AFE_ESP32
      sprintf(_deviceConfiguration->label, "%s: %s", _configuration.name,
              L_BATTERY);
#else
      sprintf(_deviceConfiguration->label, "%s", L_BATTERY);
#endif

      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);

      /* Battery Volt */
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_BATTERYMETER_VOLT;
      sprintf(_deviceConfiguration->unit, AFE_UNIT_VOLTAGE);

      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);

#endif
    }
    else
    {
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_ADC_VOLTAGE;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
      _deviceConfiguration->type =
          AFE_CONFIG_HA_ITEM_SENSOR_ADC_VOLTAGE_CALCULATED;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_ADC_RAW;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_ADC_PERCENT;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
      _deviceConfiguration->type =
          AFE_CONFIG_HA_ITEM_SENSOR_BATTERYMETER_PERCENT;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_BATTERYMETER_VOLT;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
#endif
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_ANALOG_INPUT

#ifdef AFE_CONFIG_HARDWARE_SWITCH
void AFEAPIHomeAssistantIntegration::publishSwitches(void)
{

  if (!_initialize)
  {
    return;
  }

  SWITCH _configuration;
  _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SWITCH;
  sprintf(_deviceConfiguration->deviceClass, AFE_CONFIG_HA_DEVICE_CLASS_SWITCH);
  _deviceConfiguration->entityId =
      AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_OPEN_CLOSED;
  _deviceConfiguration->hardwareId = AFE_CONFIG_HA_HARDWARE_SWITCH;

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES; i++)
  {
#ifdef DEBUG
    printPublishDeviceItemMessage(F("Switch"), i);
#endif

    _deviceConfiguration->id = i;

    if (i < _Firmware->Device->configuration.noOfSwitches)
    {
      _Firmware->API->Flash->getConfiguration(i, &_configuration);
      sprintf(_deviceConfiguration->label, "%s: %d", L_SWITCH, i + 1);
      sprintf(_deviceConfiguration->mqtt.topic, _configuration.mqtt.topic);
      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);
    }
    else
    {
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_SWITCH

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
void AFEAPIHomeAssistantIntegration::publishBinarySensor(void)
{

  if (!_initialize)
  {
    return;
  }

  BINARY_SENSOR _configuration;
  _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_BINARY_SENSOR;
  sprintf(_deviceConfiguration->deviceClass,
          AFE_CONFIG_HA_DEVICE_CLASS_BINARY_SENSOR);
  _deviceConfiguration->entityId =
      AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_OPEN_CLOSED;
  _deviceConfiguration->hardwareId = AFE_CONFIG_HA_HARDWARE_SENSOR_BINARY;

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_BINARY_SENSORS; i++)
  {
#ifdef DEBUG
    printPublishDeviceItemMessage(F("Binary Sensor"), i);
#endif

    _deviceConfiguration->id = i;

    if (i < _Firmware->Device->configuration.noOfBinarySensors)
    {
      _Firmware->API->Flash->getConfiguration(i, &_configuration);
      sprintf(_deviceConfiguration->label, "%s", _configuration.name);
      sprintf(_deviceConfiguration->mqtt.topic, _configuration.mqtt.topic);
      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);
    }
    else
    {
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_BINARY_SENSOR

#ifdef AFE_CONFIG_HARDWARE_DS18B20
void AFEAPIHomeAssistantIntegration::publishSensorDS18B20(void)
{
  if (!_initialize)
  {
    return;
  }
  DS18B20 _configuration;
  _deviceConfiguration->entityId = AFE_CONFIG_HA_TYPE_OF_ENTITY_SENSOR;
  _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_TEMPERATURE;
  sprintf(_deviceConfiguration->deviceClass,
          AFE_CONFIG_HA_DEVICE_CLASS_TEMPERATURE);
  _deviceConfiguration->hardwareId = AFE_CONFIG_HA_HARDWARE_SENSOR_DS18B20;

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_DS18B20; i++)
  {

    _deviceConfiguration->id = i;

    if (i < _Firmware->Device->configuration.noOfDS18B20s)
    {
      _Firmware->API->Flash->getConfiguration(i, &_configuration);
#ifdef DEBUG
      printPublishDeviceItemMessage(F("DS18B20"), i);
#endif

      sprintf(_deviceConfiguration->label, _configuration.name);
      sprintf(_deviceConfiguration->mqtt.topic, _configuration.mqtt.topic);
      sprintf(_deviceConfiguration->unit,
              _configuration.unit == AFE_TEMPERATURE_UNIT_CELSIUS
                  ? AFE_UNIT_TEMPERATURE_C
                  : AFE_UNIT_TEMPERATURE_F);
      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);
    }
    else
    {
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_DS18B20

#ifdef AFE_CONFIG_HARDWARE_DHT
/**
 * @brief publishes configuration of all DHT sensors
 *
 */
void AFEAPIHomeAssistantIntegration::publishSensorDHT(void)
{
  if (!_initialize)
  {
    return;
  }
  DHT _configuration;

  _deviceConfiguration->entityId = AFE_CONFIG_HA_TYPE_OF_ENTITY_SENSOR;
  _deviceConfiguration->hardwareId = AFE_CONFIG_HA_HARDWARE_SENSOR_DHTXX;

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_DHT; i++)
  {

    _deviceConfiguration->id = i;

    if (i < _Firmware->Device->configuration.noOfDHTs)
    {
      _Firmware->API->Flash->getConfiguration(i, &_configuration);
#ifdef DEBUG
      printPublishDeviceItemMessage(F("DHTxx"), i);
#endif
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_TEMPERATURE;
      sprintf(_deviceConfiguration->label, "%s: %s", _configuration.name,
              L_TEMPERATURE);
      sprintf(_deviceConfiguration->mqtt.topic, _configuration.mqtt.topic);
      sprintf(_deviceConfiguration->unit,
              _configuration.temperature.unit == AFE_TEMPERATURE_UNIT_CELSIUS
                  ? AFE_UNIT_TEMPERATURE_C
                  : AFE_UNIT_TEMPERATURE_F);
      sprintf(_deviceConfiguration->deviceClass,
              AFE_CONFIG_HA_DEVICE_CLASS_TEMPERATURE);
      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);

      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_HEAT_INDEX;
      sprintf(_deviceConfiguration->label, "%s: %s", _configuration.name,
              L_HEAT_INDEX);
      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);

      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_DEW_POINT;
      sprintf(_deviceConfiguration->label, "%s: %s", _configuration.name,
              L_DEW_POINT);
      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);

      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_HUMIDITY;
      sprintf(_deviceConfiguration->label, "%s: %s", _configuration.name,
              L_HUMIDITY);
      sprintf(_deviceConfiguration->unit, AFE_UNIT_PERCENT);
      sprintf(_deviceConfiguration->deviceClass,
              AFE_CONFIG_HA_DEVICE_CLASS_HUMIDITY);
      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);

      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_ABSOLUTE_HUMIDITY;
      sprintf(_deviceConfiguration->label, "%s: %s", _configuration.name,
              L_ABSOLOUTE_HUMIDITY);
      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);

      _deviceConfiguration->unit[0] = AFE_EMPTY_STRING;

      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_PERCEPTION;
      sprintf(_deviceConfiguration->label, "%s: %s", _configuration.name,
              L_PERCEPTION);
      sprintf(_deviceConfiguration->deviceClass,
              AFE_CONFIG_HA_DEVICE_CLASS_ENUM);
      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);

      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_COMFORT_LEVEL;
      sprintf(_deviceConfiguration->label, "%s: %s", _configuration.name,
              L_COMFORT_LEVEL);
      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);
    }
    else
    {
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_TEMPERATURE;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_HUMIDITY;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_ABSOLUTE_HUMIDITY;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_HEAT_INDEX;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_DEW_POINT;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_PERCEPTION;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_COMFORT_LEVEL;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_DHT

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
void AFEAPIHomeAssistantIntegration::publishThermalProtector(void)
{
  if (!_initialize)
  {
    return;
  }
  THERMAL_PROTECTOR _configuration;

  _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_THERMAL_PROTECTOR;
  sprintf(_deviceConfiguration->deviceClass, AFE_CONFIG_HA_DEVICE_CLASS_SWITCH);
  _deviceConfiguration->entityId = AFE_CONFIG_HA_TYPE_OF_ENTITY_SWITCH;
  _deviceConfiguration->hardwareId = AFE_CONFIG_HA_HARDWARE_REGULATOR;

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_THERMAL_PROTECTORS;
       i++)
  {
#ifdef DEBUG
    printPublishDeviceItemMessage(F("Thermal protector"), i);
#endif

    _deviceConfiguration->id = i;

    if (i < _Firmware->Device->configuration.noOfThermalProtectors)
    {
      _Firmware->API->Flash->getConfiguration(i, &_configuration);
      sprintf(_deviceConfiguration->mqtt.topic, _configuration.mqtt.topic);
      sprintf(_deviceConfiguration->label, _configuration.name);
      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);
    }
    else
    {
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
    }
  }
}
#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
void AFEAPIHomeAssistantIntegration::publishRegulator(void)
{
  if (!_initialize)
  {
    return;
  }
  REGULATOR _configuration;
  _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_REGULATOR;
  sprintf(_deviceConfiguration->deviceClass, AFE_CONFIG_HA_DEVICE_CLASS_SWITCH);
  _deviceConfiguration->entityId = AFE_CONFIG_HA_TYPE_OF_ENTITY_SWITCH;
  _deviceConfiguration->hardwareId = AFE_CONFIG_HA_HARDWARE_REGULATOR;

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_REGULATORS; i++)
  {
#ifdef DEBUG
    printPublishDeviceItemMessage(F("Regulator"), i);
#endif

    _deviceConfiguration->id = i;

    if (i < _Firmware->Device->configuration.noOfRegulators)
    {
      _Firmware->API->Flash->getConfiguration(i, &_configuration);
      sprintf(_deviceConfiguration->mqtt.topic, _configuration.mqtt.topic);
      sprintf(_deviceConfiguration->label, _configuration.name);
      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);
    }
    else
    {
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
    }
  }
}
#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

#ifdef AFE_CONFIG_HARDWARE_BMEX80
void AFEAPIHomeAssistantIntegration::publishBMX80(void)
{
  if (!_initialize)
  {
    return;
  }
  BMEX80 _configuration;
  _deviceConfiguration->entityId = AFE_CONFIG_HA_TYPE_OF_ENTITY_SENSOR;
  _deviceConfiguration->hardwareId = AFE_CONFIG_HA_HARDWARE_SENSOR_BMX60;

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_BMEX80; i++)
  {

    _deviceConfiguration->id = i;

    if (i < _Firmware->Device->configuration.noOfBMEX80s)
    {
      _Firmware->API->Flash->getConfiguration(i, &_configuration);
#ifdef DEBUG
      printPublishDeviceItemMessage(F("Bosch BMx"), i);
#endif

      sprintf(_deviceConfiguration->mqtt.topic, _configuration.mqtt.topic);

      /* Temperature */
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_TEMPERATURE;
      sprintf(_deviceConfiguration->label, "%s: %s", _configuration.name,
              L_TEMPERATURE);
      sprintf(_deviceConfiguration->unit,
              _configuration.temperature.unit == AFE_TEMPERATURE_UNIT_CELSIUS
                  ? AFE_UNIT_TEMPERATURE_C
                  : AFE_UNIT_TEMPERATURE_F);

      sprintf(_deviceConfiguration->deviceClass,
              AFE_CONFIG_HA_DEVICE_CLASS_TEMPERATURE);
      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);

      /* Releative pressure */
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_RELATIVE_PRESSURE;
      sprintf(_deviceConfiguration->label, "%s: %s", _configuration.name,
              L_PRESSURE_RELATIVE);
      sprintf(_deviceConfiguration->unit, AFE_UNIT_PRESSURE);
      sprintf(_deviceConfiguration->deviceClass,
              AFE_CONFIG_HA_DEVICE_CLASS_PRESSURE);
      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);

      /* Absolute pressure */
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_ABSOLUTE_PRESSURE;
      sprintf(_deviceConfiguration->label, "%s: %s", _configuration.name,
              L_PRESSURE);
      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);

      /**
       * @brief Atributes for BME280/680 sensors
       *
       */

      if (_configuration.type != AFE_BMP180_SENSOR &&
          _configuration.type != AFE_BMP280_SENSOR)
      {

        /* Heat index */
        _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_HEAT_INDEX;
        sprintf(_deviceConfiguration->label, "%s: %s", _configuration.name,
                L_HEAT_INDEX);
        sprintf(_deviceConfiguration->unit,
                _configuration.temperature.unit == AFE_TEMPERATURE_UNIT_CELSIUS
                    ? AFE_UNIT_TEMPERATURE_C
                    : AFE_UNIT_TEMPERATURE_F);
        sprintf(_deviceConfiguration->deviceClass,
                AFE_CONFIG_HA_DEVICE_CLASS_TEMPERATURE);
        publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);

        /* Dew point */
        _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_DEW_POINT;
        sprintf(_deviceConfiguration->label, "%s: %s", _configuration.name,
                L_DEW_POINT);
        publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);

        /* Humidity */
        _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_HUMIDITY;
        sprintf(_deviceConfiguration->label, "%s: %s", _configuration.name,
                L_HUMIDITY);
        sprintf(_deviceConfiguration->unit, AFE_UNIT_PERCENT);
        sprintf(_deviceConfiguration->deviceClass,
                AFE_CONFIG_HA_DEVICE_CLASS_HUMIDITY);
        publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);
        /* Absolute humidity */
        _deviceConfiguration->type =
            AFE_CONFIG_HA_ITEM_SENSOR_ABSOLUTE_HUMIDITY;
        sprintf(_deviceConfiguration->label, "%s: %s", _configuration.name,
                L_ABSOLOUTE_HUMIDITY);
        publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);

        /* Air perception */
        _deviceConfiguration->unit[0] = AFE_EMPTY_STRING;
        sprintf(_deviceConfiguration->deviceClass,
                AFE_CONFIG_HA_DEVICE_CLASS_ENUM);
        _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_PERCEPTION;
        sprintf(_deviceConfiguration->label, "%s: %s", _configuration.name,
                L_PERCEPTION);
        publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);

        /* Comfort */
        _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_COMFORT_LEVEL;
        sprintf(_deviceConfiguration->label, "%s: %s", _configuration.name,
                L_COMFORT_LEVEL);
        publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);
      }
      /**
       * @brief Attributes for Bosch 680 sensor
       *
       */

      if (_configuration.type == AFE_BME680_SENSOR)
      {

        /* GAS */
        _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_GAS_RESISTANCE;
        sprintf(_deviceConfiguration->label, "%s: %s", _configuration.name,
                L_BMEX80_GAS);
        /* Gas HA sensor doesn't support custom unit anymore
        sprintf(_deviceConfiguration->deviceClass,
        AFE_CONFIG_HA_DEVICE_CLASS_GAS);
        */
        sprintf(_deviceConfiguration->deviceClass,
                AFE_CONFIG_HA_DEVICE_CLASS_NONE);
        sprintf(_deviceConfiguration->unit, AFE_UNIT_KOM);
        publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);

        /* IAQ */

        _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_IAQ;
        sprintf(_deviceConfiguration->label, "%s: %s", _configuration.name,
                L_BMEX80_IAQ);
        sprintf(_deviceConfiguration->deviceClass,
                AFE_CONFIG_HA_DEVICE_CLASS_AQI);
        publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);

        /* Static IAQ */

        _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_STATIC_IAQ;
        sprintf(_deviceConfiguration->label, "%s: %s", _configuration.name,
                L_BMEX80_STATIC_IAQ);
        publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);

        /* BVOC */

        _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_BVOC_EQUIVALENT;
        sprintf(_deviceConfiguration->label, "%s: %s", _configuration.name,
                L_BMEX80_BVOC);
        sprintf(_deviceConfiguration->deviceClass,
                AFE_CONFIG_HA_DEVICE_CLASS_NONE);
        publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);

        /* CO2 */
        _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_CO2_EQUIVALENT;
        sprintf(_deviceConfiguration->label, "%s: %s", _configuration.name,
                L_BMEX80_CO2);
        sprintf(_deviceConfiguration->unit, AFE_UNIT_CO2);
        sprintf(_deviceConfiguration->deviceClass,
                AFE_CONFIG_HA_DEVICE_CLASS_CO2);
        publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);
      }
    }
    else
    {
      /**
       * @brief rmoves of all sensor attributes dispite of the type of the Bosch
       * sensor
       *
       */
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_TEMPERATURE;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_RELATIVE_PRESSURE;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_ABSOLUTE_PRESSURE;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_HUMIDITY;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_ABSOLUTE_HUMIDITY;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_HEAT_INDEX;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_DEW_POINT;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_PERCEPTION;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_COMFORT_LEVEL;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_IAQ;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_STATIC_IAQ;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_BVOC_EQUIVALENT;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_GAS_RESISTANCE;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_CO2_EQUIVALENT;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_BMEX80

#ifdef AFE_CONFIG_HARDWARE_BH1750
void AFEAPIHomeAssistantIntegration::publishBH1750(void)
{
  if (!_initialize)
  {
    return;
  }
  BH1750_CONFIG _configuration;
  _deviceConfiguration->entityId = AFE_CONFIG_HA_TYPE_OF_ENTITY_SENSOR;
  _deviceConfiguration->hardwareId = AFE_CONFIG_HA_HARDWARE_SENSOR_BH1750;

  _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_ILLUMINANCE;
  sprintf(_deviceConfiguration->deviceClass,
          AFE_CONFIG_HA_DEVICE_CLASS_ILLUMINANCE);

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_BH1750; i++)
  {

    _deviceConfiguration->id = i;

    if (i < _Firmware->Device->configuration.noOfBH1750s)
    {
      _Firmware->API->Flash->getConfiguration(i, &_configuration);
#ifdef DEBUG
      printPublishDeviceItemMessage(F("BH1750"), i);
#endif

      sprintf(_deviceConfiguration->label, _configuration.name);
      sprintf(_deviceConfiguration->mqtt.topic, _configuration.mqtt.topic);
      sprintf(_deviceConfiguration->unit, AFE_UNIT_LUX);
      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);
    }
    else
    {
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_BH1750

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
void AFEAPIHomeAssistantIntegration::publishHPMA115S0(void)
{
  if (!_initialize)
  {
    return;
  }
  HPMA115S0 _configuration;
  _deviceConfiguration->entityId = AFE_CONFIG_HA_TYPE_OF_ENTITY_SENSOR;
  _deviceConfiguration->hardwareId = AFE_CONFIG_HA_HARDWARE_SENSOR_HPMA115S0;
  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_HPMA115S0; i++)
  {

    _deviceConfiguration->id = i;

    if (i < _Firmware->Device->configuration.noOfHPMA115S0s)
    {
      _Firmware->API->Flash->getConfiguration(i, &_configuration);
#ifdef DEBUG
      printPublishDeviceItemMessage(F("HPMA115S0"), i);
#endif

      sprintf(_deviceConfiguration->mqtt.topic, _configuration.mqtt.topic);

      /* PM10 */
      sprintf(_deviceConfiguration->label, "PM10");
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_PM10;
      sprintf(_deviceConfiguration->deviceClass,
              AFE_CONFIG_HA_DEVICE_CLASS_PM10);
      sprintf(_deviceConfiguration->unit, AFE_UNIT_PARTICLE);
      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);

      /* PM10 WHO Norm */
      sprintf(_deviceConfiguration->label, "PM10 WHO");
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_PM10_WHO;
      sprintf(_deviceConfiguration->unit, AFE_UNIT_PERCENT);
      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);

      /* PM25 WHO Norm */
      sprintf(_deviceConfiguration->label, "PM2.5 WHO");
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_PM25_WHO;
      sprintf(_deviceConfiguration->deviceClass,
              AFE_CONFIG_HA_DEVICE_CLASS_PM25);
      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);

      /* PM25 */
      sprintf(_deviceConfiguration->label, "PM2.5");
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_PM25;
      sprintf(_deviceConfiguration->unit, AFE_UNIT_PARTICLE);
      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);
    }
    else
    {
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_PM10;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_PM10_WHO;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_PM25;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_PM25;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_HPMA115S0

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
void AFEAPIHomeAssistantIntegration::publishAnemometer(void)
{
  if (!_initialize)
  {
    return;
  }
  ANEMOMETER _configuration;
  _deviceConfiguration->entityId = AFE_CONFIG_HA_TYPE_OF_ENTITY_SENSOR;
  sprintf(_deviceConfiguration->deviceClass, AFE_CONFIG_HA_DEVICE_CLASS_NONE);
  _deviceConfiguration->hardwareId = AFE_CONFIG_HA_HARDWARE_SENSOR_ANEMOMETER;

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_ANEMOMETER_SENSORS;
       i++)
  {

    _deviceConfiguration->id = i;

    if (i < _Firmware->Device->configuration.noOfAnemometerSensors)
    {
      _Firmware->API->Flash->getConfiguration(&_configuration);
#ifdef DEBUG
      printPublishDeviceItemMessage(F("Anemometer"), i);
#endif

      sprintf(_deviceConfiguration->label, _configuration.name);
      sprintf(_deviceConfiguration->mqtt.topic, _configuration.mqtt.topic);

      /* Wind speed km/h */
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_ANEMOMETER_KMH;

      sprintf(_deviceConfiguration->unit, AFE_UNIT_KMH);
      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);

      /* Wind speed m/s */
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_ANEMOMETER_MS;
      sprintf(_deviceConfiguration->unit, AFE_UNIT_MS);
      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);
    }
    else
    {
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_ANEMOMETER_KMH;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_ANEMOMETER_MS;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_ANEMOMETER

#ifdef AFE_CONFIG_HARDWARE_FS3000
/**
 * @brief publishes FS3000
 *
 */
void AFEAPIHomeAssistantIntegration::publishFS3000(void)
{
  if (!_initialize)
  {
    return;
  }
  FS3000_CONFIG _configuration;
  _deviceConfiguration->entityId = AFE_CONFIG_HA_TYPE_OF_ENTITY_SENSOR;
  sprintf(_deviceConfiguration->deviceClass, AFE_CONFIG_HA_DEVICE_CLASS_NONE);
  _deviceConfiguration->hardwareId = AFE_CONFIG_HA_HARDWARE_SENSOR_FS3000;

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_FS3000; i++)
  {

    _deviceConfiguration->id = i;

    if (i < _Firmware->Device->configuration.noOfFS3000s)
    {
      _Firmware->API->Flash->getConfiguration(i, &_configuration);
#ifdef DEBUG
      printPublishDeviceItemMessage(F("FS3000"), i);
#endif

      sprintf(_deviceConfiguration->label, _configuration.name);
      sprintf(_deviceConfiguration->mqtt.topic, _configuration.mqtt.topic);

      /* raw */
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_FS3000_RAW;
      sprintf(_deviceConfiguration->unit, AFE_UNIT_RAW);
      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);

      /* m/s */
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_FS3000_MS;
      sprintf(_deviceConfiguration->unit, AFE_UNIT_MS);
      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);

      /* mil/h */
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_FS3000_MILH;
      sprintf(_deviceConfiguration->unit, AFE_UNIT_MILH);
      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);

      /* m3/h */
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_FS3000_M3H;
      sprintf(_deviceConfiguration->unit, AFE_UNIT_M3H);
      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);
    }
    else
    {
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_FS3000_RAW;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_FS3000_MS;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_FS3000_MILH;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_FS3000_M3H;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
    }
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
void AFEAPIHomeAssistantIntegration::publishRainmeter(void)
{
  if (!_initialize)
  {
    return;
  }
  RAINMETER _configuration;
  _deviceConfiguration->entityId = AFE_CONFIG_HA_TYPE_OF_ENTITY_SENSOR;
  sprintf(_deviceConfiguration->deviceClass, AFE_CONFIG_HA_DEVICE_CLASS_NONE);
  _deviceConfiguration->hardwareId = AFE_CONFIG_HA_HARDWARE_SENSOR_RAINMETER;
  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_RAINMETER_SENSORS;
       i++)
  {

    _deviceConfiguration->id = i;

    if (i < _Firmware->Device->configuration.noOfRainmeterSensors)
    {
      _Firmware->API->Flash->getConfiguration(&_configuration);
#ifdef DEBUG
      printPublishDeviceItemMessage(F("Rainmeter"), i);
#endif

      sprintf(_deviceConfiguration->label, _configuration.name);
      sprintf(_deviceConfiguration->mqtt.topic, _configuration.mqtt.topic);

      /* Rainmeter mm/min */
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_RAINMETER_MMM;
      sprintf(_deviceConfiguration->unit, AFE_UNIT_MMM);
      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);

      /* Rainmeter mm/h */
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_RAINMETER_MMH;
      sprintf(_deviceConfiguration->unit, AFE_UNIT_MMH);
      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);

      /* Rainmeter mm/12h */
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_RAINMETER_MM12H;
      sprintf(_deviceConfiguration->unit, AFE_UNIT_MM12H);
      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);

      /* Rainmeter mm/24h */
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_RAINMETER_MM24H;
      sprintf(_deviceConfiguration->unit, AFE_UNIT_MM24H);
      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);
    }
    else
    {
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_RAINMETER_MMM;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_RAINMETER_MMH;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_RAINMETER_MM12H;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_RAINMETER_MM24H;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
    }
  }
}

#endif // AFE_CONFIG_HARDWARE_RAINMETER

#ifdef AFE_CONFIG_HARDWARE_TSL2561
void AFEAPIHomeAssistantIntegration::publishTSL2561(void)
{
  if (!_initialize)
  {
    return;
  }
  TSL2561 _configuration;

  _deviceConfiguration->entityId = AFE_CONFIG_HA_TYPE_OF_ENTITY_SENSOR;
  _deviceConfiguration->hardwareId = AFE_CONFIG_HA_HARDWARE_SENSOR_TSL2561;
  sprintf(_deviceConfiguration->deviceClass,
          AFE_CONFIG_HA_DEVICE_CLASS_ILLUMINANCE);

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_TSL2561; i++)
  {
    _deviceConfiguration->id = i;
    if (i < _Firmware->Device->configuration.noOfTSL2561s)
    {
      _Firmware->API->Flash->getConfiguration(i, &_configuration);
#ifdef DEBUG
      printPublishDeviceItemMessage(F("TSL2561"), i);
#endif

      sprintf(_deviceConfiguration->mqtt.topic, _configuration.mqtt.topic);

      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_ILLUMINANCE;
      sprintf(_deviceConfiguration->label, "%s: %s", _configuration.name,
              L_ILUMINANCE);
      sprintf(_deviceConfiguration->unit, AFE_UNIT_LUX);
      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);

      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_IR;
      sprintf(_deviceConfiguration->label, "%s: %s", _configuration.name, "IR");
      _deviceConfiguration->unit[0] = AFE_EMPTY_STRING;
      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);
    }
    else
    {
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_ILLUMINANCE;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
      _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_SENSOR_IR;
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_TSL2561

#ifdef AFE_CONFIG_HARDWARE_GATE
void AFEAPIHomeAssistantIntegration::publishGate(void)
{
  if (!_initialize)
  {
    return;
  }
  resetDeviceConfiguration();
  GATE _configuration;
  _deviceConfiguration->hardwareId = AFE_CONFIG_HA_HARDWARE_GATE;
  _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_GATE;
  sprintf(_deviceConfiguration->deviceClass, AFE_CONFIG_HA_DEVICE_CLASS_GATE);
  _deviceConfiguration->entityId = AFE_CONFIG_HA_TYPE_OF_ENTITY_COVER;

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_GATES; i++)
  {
#ifdef DEBUG
    printPublishDeviceItemMessage(F("Gates"), i);
#endif

    _deviceConfiguration->id = i;

    if (i < _Firmware->Device->configuration.noOfGates)
    {
      _Firmware->API->Flash->getConfiguration(i, &_configuration);
      sprintf(_deviceConfiguration->label, "%s", _configuration.name);
      sprintf(_deviceConfiguration->mqtt.topic, _configuration.mqtt.topic);
      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);
    }
    else
    {
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_GATE

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
void AFEAPIHomeAssistantIntegration::publishContactron(void)
{
  if (!_initialize)
  {
    return;
  }
  CONTACTRON _configuration;
  _deviceConfiguration->type = AFE_CONFIG_HA_ITEM_BINARY_SENSOR;
  sprintf(_deviceConfiguration->deviceClass,
          AFE_CONFIG_HA_DEVICE_CLASS_OPENING);
  _deviceConfiguration->entityId =
      AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_OPEN_CLOSED;
  _deviceConfiguration->hardwareId = AFE_CONFIG_HA_HARDWARE_SENSOR_CONTACTRON;

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_CONTACTRONS; i++)
  {
#ifdef DEBUG
    printPublishDeviceItemMessage(F("Contractons"), i);
#endif

    _deviceConfiguration->id = i;

    if (i < _Firmware->Device->configuration.noOfContactrons)
    {
      _Firmware->API->Flash->getConfiguration(i, &_configuration);
      sprintf(_deviceConfiguration->label, "%s", _configuration.name);
      sprintf(_deviceConfiguration->mqtt.topic, _configuration.mqtt.topic);
      publishItemToHomeAssistantMQTTDiscovery(_deviceConfiguration);
    }
    else
    {
      removeItemRemovedFromHomeAssistantMQTTDiscovery(_deviceConfiguration);
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_CONTACTRON

/******* Private Methods *******/

void AFEAPIHomeAssistantIntegration::generateObjectId(char *objectId,
                                                      uint8_t deviceClassId,
                                                      uint8_t hardwareId,
                                                      uint8_t id)
{
  char _deviceId[AFE_CONFIG_DEVICE_ID_SIZE];
  _Firmware->API->Flash->getDeviceID(_deviceId);
  sprintf(objectId, "%s-%d%d%d", _deviceId, hardwareId, deviceClassId, id);
}

void AFEAPIHomeAssistantIntegration::generateTopic(char *topic,
                                                   uint8_t entityId,
                                                   const char *objectId)
{
  sprintf(
      topic, "%s/%s/%s/config", configuration->discovery.topic,
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
      : entityId ==
              AFE_CONFIG_HA_TYPE_OF_ENTITY_COVER
          ? AFE_CONFIG_HA_TYPE_OF_ENTITY_COVER_NAME
      : entityId ==
              AFE_CONFIG_HA_TYPE_OF_ENTITY_UPDATE
          ? AFE_CONFIG_HA_TYPE_OF_ENTITY_UPDATE_NAME
          : AFE_CONFIG_HA_TYPE_OF_ENTITY_UNKNOWN,
      objectId);
}

void AFEAPIHomeAssistantIntegration::
    removeItemRemovedFromHomeAssistantMQTTDiscovery(
        HA_DEVICE_CONFIG *deviceConfiguration)
{

  if (configuration->removeingComponents)
  {
#ifdef DEBUG
    _Firmware->Debugger->printInformation(F("Removeing item"), F("HA-API"));
#endif
    char _topic[AFE_CONFIG_HA_PUBLISH_TOPIC_SIZE];
    char _objectId[AFE_CONFIG_HA_OBJECT_ID_SIZE];
    generateObjectId(_objectId, deviceConfiguration->type,
                     deviceConfiguration->hardwareId, deviceConfiguration->id);
    generateTopic(_topic, deviceConfiguration->entityId, _objectId);

    boolean _retain = _MqttAPI->Mqtt->configuration->retainAll;
    _MqttAPI->Mqtt->configuration->retainAll =
        configuration->retainConfiguration;
    _MqttAPI->Mqtt->publish(_topic, "");
    _MqttAPI->Mqtt->configuration->retainAll = _retain;
  }
}

void AFEAPIHomeAssistantIntegration::publishItemToHomeAssistantMQTTDiscovery(
    HA_DEVICE_CONFIG *deviceConfiguration)
{

#ifdef DEBUG
  _Firmware->Debugger->printInformation(F("Setting/Updating item: "), F("HA-API"));
  _Firmware->Debugger->printValue(deviceConfiguration->label);
#endif

  if (strlen(deviceConfiguration->mqtt.topic) > 0 &&
      configuration->addingComponents)
  {

    String _json = "";

    char _topic[AFE_CONFIG_HA_PUBLISH_TOPIC_SIZE];
    char _message[AFE_CONFIG_HA_CONFIGURATION_JSON_SIZE];
    char _objectId[AFE_CONFIG_HA_OBJECT_ID_SIZE];

    generateObjectId(_objectId, deviceConfiguration->type,
                     deviceConfiguration->hardwareId, deviceConfiguration->id);
    generateTopic(_topic, deviceConfiguration->entityId, _objectId);

    _json.concat(FPSTR(HA_MQTT_DISCOVERY_JSON_BODY));

    /**
     * @brief Checking if LWT configured in order to add availability
     * configuration
     *
     */

    if (strlen(_mqttConfiguration.lwt.topic) > 0)
    {
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_AVAILABILITY),
                    FPSTR(HA_MQTT_DISCOVERY_JSON_AVAILABILITY));
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_AVAILABILITY_TOPIC),
                    _mqttConfiguration.lwt.topic);
    }
    else
    {
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_AVAILABILITY), "");
    }

    /**
     * @brief Adds command_topic which is MQTT command topic
     *
     */
    if (deviceConfiguration->entityId == AFE_CONFIG_HA_TYPE_OF_ENTITY_SWITCH ||
        deviceConfiguration->entityId == AFE_CONFIG_HA_TYPE_OF_ENTITY_SELECT ||
        deviceConfiguration->entityId == AFE_CONFIG_HA_TYPE_OF_ENTITY_COVER ||
        deviceConfiguration->entityId == AFE_CONFIG_HA_TYPE_OF_ENTITY_LIGHT)
    {
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_BODY_COMMAND_TOPIC),
                    FPSTR(HA_MQTT_DISCOVERY_JSON_COMMAND_TOPIC));
    }

    /**
     * @brief Adds state_topic which is MQTT State topic
     *
     */
    if (deviceConfiguration->entityId == AFE_CONFIG_HA_TYPE_OF_ENTITY_SWITCH ||
        deviceConfiguration->entityId ==
            AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_ON_OFF ||
        deviceConfiguration->entityId ==
            AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_OPEN_CLOSED ||
        deviceConfiguration->entityId == AFE_CONFIG_HA_TYPE_OF_ENTITY_SELECT ||
        deviceConfiguration->entityId == AFE_CONFIG_HA_TYPE_OF_ENTITY_COVER ||
        deviceConfiguration->entityId == AFE_CONFIG_HA_TYPE_OF_ENTITY_LIGHT ||
        deviceConfiguration->type == AFE_CONFIG_HA_ITEM_FIRMWARE_STATUS)
    {
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_BODY_STATE_TOPIC),
                    FPSTR(HA_MQTT_DISCOVERY_JSON_STATE_TOPIC));
    }

    /* Adds state_on, state_off or state_value_template*/
    if (deviceConfiguration->entityId == AFE_CONFIG_HA_TYPE_OF_ENTITY_SWITCH)
    {
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_BODY_STATE_ON_OFF_TPL),
                    FPSTR(HA_MQTT_DISCOVERY_JSON_STATE_ON_OFF));
#ifdef AFE_CONFIG_HARDWARE_CLED
    }
    else if (deviceConfiguration->entityId ==
             AFE_CONFIG_HA_TYPE_OF_ENTITY_LIGHT)
    {
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_BODY_STATE_ON_OFF_TPL),
                    FPSTR(HA_MQTT_DISCOVERY_JSON_STATE_VALUE_TEMPLATE));
#endif
#ifdef AFE_CONFIG_HARDWARE_GATE
    }
    else if (deviceConfiguration->entityId ==
             AFE_CONFIG_HA_TYPE_OF_ENTITY_COVER)
    {
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_BODY_STATE_ON_OFF_TPL),
                    FPSTR(HA_MQTT_DISCOVERY_JSON_STATES_GATE));
#endif
    }
    else if (deviceConfiguration->type ==
             AFE_CONFIG_HA_ITEM_FIRMWARE_STATUS)
    {
      _json.replace(
          F(HA_MQTT_DISCOVERY_TAG_SET_BODY_STATE_ON_OFF_TPL),
          FPSTR(HA_MQTT_DISCOVERY_JSON_FIRMWARE_UPDATE_VALUE_TEMPLATE));
    }

    /**
     * @brief Adding payloads
     *
     */

    /* Adds payload_on=on, payload_off=off or payload command */
    if (deviceConfiguration->entityId == AFE_CONFIG_HA_TYPE_OF_ENTITY_SWITCH ||
        deviceConfiguration->entityId ==
            AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_ON_OFF)
    {
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_BODY_PAYLOAD_ON_OFF_CMD),
                    FPSTR(HA_MQTT_DISCOVERY_JSON_PAYLOAD_ON_OFF));

      /* Adds payload_on=closed, payload_off=open */
    }
    else if (deviceConfiguration->entityId ==
             AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_OPEN_CLOSED)
    {
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_BODY_PAYLOAD_ON_OFF_CMD),
                    FPSTR(HA_MQTT_DISCOVERY_JSON_COMMAND_OPEN_CLOSED));

#ifdef AFE_CONFIG_HARDWARE_CLED
    }
    else if (deviceConfiguration->entityId ==
             AFE_CONFIG_HA_TYPE_OF_ENTITY_LIGHT)
    {
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_BODY_PAYLOAD_ON_OFF_CMD),
                    FPSTR(HA_MQTT_DISCOVERY_JSON_PAYLOAD_TEMPLATE));

#endif
#ifdef AFE_CONFIG_HARDWARE_GATE
    }
    else if (deviceConfiguration->entityId ==
             AFE_CONFIG_HA_TYPE_OF_ENTITY_COVER)
    {
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_BODY_PAYLOAD_ON_OFF_CMD),
                    FPSTR(HA_MQTT_DISCOVERY_JSON_COMMAND_OPEN_CLOSE_STOP));

#endif
    }

    /**
     * @brief tag: device_class
     *
     */

    if (strcmp(deviceConfiguration->deviceClass,
               AFE_CONFIG_HA_DEVICE_CLASS_NONE) == 0)
    {
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_BODY_DEVICE_CLASS), "");
    }
    else
    {
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_BODY_DEVICE_CLASS),
                    FPSTR(HA_MQTT_DISCOVERY_JSON_DEVICE_CLASS));
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_DEVICE_CLASS),
                    deviceConfiguration->deviceClass);
    }

    /**
     * @brief tag: entity_category
     *
     */

    if (deviceConfiguration->type == AFE_CONFIG_HA_ITEM_FIRMWARE_STATUS)
    {
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_ENTITY_CATEGORY),
                    FPSTR(HA_MQTT_DISCOVERY_JSON_ENTITY_CATEGORY));
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_ENTITY_CATEGORY),
                    deviceConfiguration->entityCategory);
    }

    /* Sensor section */
    if (deviceConfiguration->entityId == AFE_CONFIG_HA_TYPE_OF_ENTITY_SENSOR)
    {
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_SENSOR),
                    FPSTR(HA_MQTT_DISCOVERY_JSON_SENSOR_COMMON));
      if (false)
      {
      }
#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
      /* Sensor ADC: voltage and calculated voltage */
      else if (deviceConfiguration->type ==
                   AFE_CONFIG_HA_ITEM_SENSOR_ADC_VOLTAGE ||
               deviceConfiguration->type ==
                   AFE_CONFIG_HA_ITEM_SENSOR_ADC_VOLTAGE_CALCULATED)
      {
        _json.replace(
            F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
            deviceConfiguration->type == AFE_CONFIG_HA_ITEM_SENSOR_ADC_VOLTAGE
                ? F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_VOLTAGE)
                : F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_VOLTAGE_CALCULATED));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_SENSOR_UNIT_OF_MEASURE),
                      FPSTR(HA_MQTT_DISCOVERY_JSON_SENSOR_UNIT_OF_MEASURE));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }
      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_ADC_RAW)
      {
        /* Sensor ADC: RAW */
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_RAW));
      }
      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_ADC_PERCENT)
      {
        /* Sensor ADC: RAW Percentage */
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_PERCENT));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_SENSOR_UNIT_OF_MEASURE),
                      FPSTR(HA_MQTT_DISCOVERY_JSON_SENSOR_UNIT_OF_MEASURE));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }
#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_BATTERYMETER_PERCENT)
      {
        /* Sensor ADC: Battert meter: % */
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_BATTERY_PERCENT));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_SENSOR_UNIT_OF_MEASURE),
                      FPSTR(HA_MQTT_DISCOVERY_JSON_SENSOR_UNIT_OF_MEASURE));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }
      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_BATTERYMETER_VOLT)
      {
        /* Sensor ADC: Battert meter: V */
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_BATTERY_VOLT));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_SENSOR_UNIT_OF_MEASURE),
                      FPSTR(HA_MQTT_DISCOVERY_JSON_SENSOR_UNIT_OF_MEASURE));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }
#endif // Battertmeter
#endif // Analog Input

#if defined(AFE_CONFIG_HARDWARE_DS18B20) || \
    defined(AFE_CONFIG_HARDWARE_DHT) || defined(AFE_CONFIG_HARDWARE_BMEX80)
      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_TEMPERATURE)
      {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_TEMPERATURE));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_SENSOR_UNIT_OF_MEASURE),
                      FPSTR(HA_MQTT_DISCOVERY_JSON_SENSOR_UNIT_OF_MEASURE));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }
#endif // Temperature

#if defined(AFE_CONFIG_HARDWARE_DHT) || defined(AFE_CONFIG_HARDWARE_BMEX80)
      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_HUMIDITY)
      {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_HUMIDITY));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_SENSOR_UNIT_OF_MEASURE),
                      FPSTR(HA_MQTT_DISCOVERY_JSON_SENSOR_UNIT_OF_MEASURE));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }
#endif // Humidity

#if defined(AFE_CONFIG_HARDWARE_DHT) || defined(AFE_CONFIG_HARDWARE_BMEX80)
      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_ABSOLUTE_HUMIDITY)
      {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_ABSOLUTE_HUMIDITY));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_SENSOR_UNIT_OF_MEASURE),
                      FPSTR(HA_MQTT_DISCOVERY_JSON_SENSOR_UNIT_OF_MEASURE));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }
#endif // Absolute Humidity

#if defined(AFE_CONFIG_HARDWARE_DHT) || defined(AFE_CONFIG_HARDWARE_BMEX80)
      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_DEW_POINT)
      {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_DEW_POINT));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_SENSOR_UNIT_OF_MEASURE),
                      FPSTR(HA_MQTT_DISCOVERY_JSON_SENSOR_UNIT_OF_MEASURE));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }
#endif // Dew Point

#if defined(AFE_CONFIG_HARDWARE_DHT) || defined(AFE_CONFIG_HARDWARE_BMEX80)
      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_HEAT_INDEX)
      {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_HEAT_INDEX));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_SENSOR_UNIT_OF_MEASURE),
                      FPSTR(HA_MQTT_DISCOVERY_JSON_SENSOR_UNIT_OF_MEASURE));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }
#endif // Heat Index

#if defined(AFE_CONFIG_HARDWARE_DHT) || defined(AFE_CONFIG_HARDWARE_BMEX80)
      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_PERCEPTION)
      {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_PERCEPTION));
      }
#endif // Perception

#if defined(AFE_CONFIG_HARDWARE_DHT) || defined(AFE_CONFIG_HARDWARE_BMEX80)
      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_COMFORT_LEVEL)
      {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_COMFORT));
      }
#endif // Comfort level

#if defined(AFE_CONFIG_HARDWARE_BMEX80)
      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_RELATIVE_PRESSURE)
      {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_RELATIVE_PRESSURE));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_SENSOR_UNIT_OF_MEASURE),
                      FPSTR(HA_MQTT_DISCOVERY_JSON_SENSOR_UNIT_OF_MEASURE));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }
#endif // Relative pressure

#if defined(AFE_CONFIG_HARDWARE_BMEX80)
      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_ABSOLUTE_PRESSURE)
      {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_ABSOLUTE_PRESSURE));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_SENSOR_UNIT_OF_MEASURE),
                      FPSTR(HA_MQTT_DISCOVERY_JSON_SENSOR_UNIT_OF_MEASURE));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }
#endif // Absolute pressure

#if defined(AFE_CONFIG_HARDWARE_BMEX80)
      else if (deviceConfiguration->type == AFE_CONFIG_HA_ITEM_SENSOR_IAQ)
      {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_IAQ));
      }
#endif // IAQ

#if defined(AFE_CONFIG_HARDWARE_BMEX80)
      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_STATIC_IAQ)
      {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_STATIC_IAQ));
      }
#endif // Static IAQ

#if defined(AFE_CONFIG_HARDWARE_BMEX80)
      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_CO2_EQUIVALENT)
      {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_CO2_EQUIVALENT));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_SENSOR_UNIT_OF_MEASURE),
                      FPSTR(HA_MQTT_DISCOVERY_JSON_SENSOR_UNIT_OF_MEASURE));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }
#endif // CO2 Equivalent

#if defined(AFE_CONFIG_HARDWARE_BMEX80)
      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_BVOC_EQUIVALENT)
      {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_BVOC_EQUIVALENT));
      }
#endif // BVOC Equivalent

#if defined(AFE_CONFIG_HARDWARE_BMEX80)
      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_GAS_RESISTANCE)
      {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_GAS_RESISTANCE));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_SENSOR_UNIT_OF_MEASURE),
                      FPSTR(HA_MQTT_DISCOVERY_JSON_SENSOR_UNIT_OF_MEASURE));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }
#endif // Gas resistance

#if defined(AFE_CONFIG_HARDWARE_BH1750)
      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_ILLUMINANCE)
      {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_ILLUMINANCE));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_SENSOR_UNIT_OF_MEASURE),
                      FPSTR(HA_MQTT_DISCOVERY_JSON_SENSOR_UNIT_OF_MEASURE));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }
#endif // BH1750 Illuminance

#if defined(AFE_CONFIG_HARDWARE_HPMA115S0)
      else if (deviceConfiguration->type == AFE_CONFIG_HA_ITEM_SENSOR_PM10)
      {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_PM10));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_SENSOR_UNIT_OF_MEASURE),
                      FPSTR(HA_MQTT_DISCOVERY_JSON_SENSOR_UNIT_OF_MEASURE));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }
#endif // PM10

#if defined(AFE_CONFIG_HARDWARE_HPMA115S0)
      else if (deviceConfiguration->type == AFE_CONFIG_HA_ITEM_SENSOR_PM25)
      {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_PM25));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_SENSOR_UNIT_OF_MEASURE),
                      FPSTR(HA_MQTT_DISCOVERY_JSON_SENSOR_UNIT_OF_MEASURE));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }
#endif // PM25

#if defined(AFE_CONFIG_HARDWARE_HPMA115S0)
      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_PM10_WHO)
      {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_PM10_WHO));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_SENSOR_UNIT_OF_MEASURE),
                      FPSTR(HA_MQTT_DISCOVERY_JSON_SENSOR_UNIT_OF_MEASURE));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }
#endif // PM10 WHO Norm

#if defined(AFE_CONFIG_HARDWARE_HPMA115S0)
      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_PM25_WHO)
      {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_PM25_WHO));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_SENSOR_UNIT_OF_MEASURE),
                      FPSTR(HA_MQTT_DISCOVERY_JSON_SENSOR_UNIT_OF_MEASURE));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }
#endif // PM25 WHO Norm

#if defined(AFE_CONFIG_HARDWARE_ANEMOMETER)
      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_ANEMOMETER_KMH)
      {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_ANEMOMETER_KMH));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_SENSOR_UNIT_OF_MEASURE),
                      FPSTR(HA_MQTT_DISCOVERY_JSON_SENSOR_UNIT_OF_MEASURE));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }
#endif // Anemometer km/h

#if defined(AFE_CONFIG_HARDWARE_ANEMOMETER)
      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_ANEMOMETER_MS)
      {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_ANEMOMETER_MS));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_SENSOR_UNIT_OF_MEASURE),
                      FPSTR(HA_MQTT_DISCOVERY_JSON_SENSOR_UNIT_OF_MEASURE));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }
#endif // Anemometer m/s

#if defined(AFE_CONFIG_HARDWARE_FS3000)
      else if (deviceConfiguration->type == AFE_CONFIG_HA_ITEM_FS3000_RAW)
      {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_FS3000_RAW));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_SENSOR_UNIT_OF_MEASURE),
                      FPSTR(HA_MQTT_DISCOVERY_JSON_SENSOR_UNIT_OF_MEASURE));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }

      else if (deviceConfiguration->type == AFE_CONFIG_HA_ITEM_FS3000_MS)
      {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_FS3000_MS));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_SENSOR_UNIT_OF_MEASURE),
                      FPSTR(HA_MQTT_DISCOVERY_JSON_SENSOR_UNIT_OF_MEASURE));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }

      else if (deviceConfiguration->type == AFE_CONFIG_HA_ITEM_FS3000_MILH)
      {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_FS3000_MILH));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_SENSOR_UNIT_OF_MEASURE),
                      FPSTR(HA_MQTT_DISCOVERY_JSON_SENSOR_UNIT_OF_MEASURE));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }

      else if (deviceConfiguration->type == AFE_CONFIG_HA_ITEM_FS3000_M3H)
      {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_FS3000_M3H));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_SENSOR_UNIT_OF_MEASURE),
                      FPSTR(HA_MQTT_DISCOVERY_JSON_SENSOR_UNIT_OF_MEASURE));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }
#endif // FS3000 m/s

#if defined(AFE_CONFIG_HARDWARE_RAINMETER)
      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_RAINMETER_MMM)
      {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_RAINMETER_MMM));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_SENSOR_UNIT_OF_MEASURE),
                      FPSTR(HA_MQTT_DISCOVERY_JSON_SENSOR_UNIT_OF_MEASURE));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }
#endif // Rainmeter mm/min

#if defined(AFE_CONFIG_HARDWARE_RAINMETER)
      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_RAINMETER_MMH)
      {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_RAINMETER_MMH));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_SENSOR_UNIT_OF_MEASURE),
                      FPSTR(HA_MQTT_DISCOVERY_JSON_SENSOR_UNIT_OF_MEASURE));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }
#endif // Rainmeter mm/h

#if defined(AFE_CONFIG_HARDWARE_RAINMETER)
      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_RAINMETER_MM12H)
      {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_RAINMETER_MM12H));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_SENSOR_UNIT_OF_MEASURE),
                      FPSTR(HA_MQTT_DISCOVERY_JSON_SENSOR_UNIT_OF_MEASURE));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }
#endif // Rainmeter mm/12h

#if defined(AFE_CONFIG_HARDWARE_RAINMETER)
      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_RAINMETER_MM24H)
      {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_RAINMETER_MM24H));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_SENSOR_UNIT_OF_MEASURE),
                      FPSTR(HA_MQTT_DISCOVERY_JSON_SENSOR_UNIT_OF_MEASURE));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }
#endif // Rainmeter mm/24h

#if defined(AFE_CONFIG_HARDWARE_TSL2561)

      else if (deviceConfiguration->type ==
               AFE_CONFIG_HA_ITEM_SENSOR_ILLUMINANCE)
      {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_ILLUMINANCE));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_SENSOR_UNIT_OF_MEASURE),
                      FPSTR(HA_MQTT_DISCOVERY_JSON_SENSOR_UNIT_OF_MEASURE));
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE),
                      deviceConfiguration->unit);
      }

      else if (deviceConfiguration->type == AFE_CONFIG_HA_ITEM_SENSOR_IR)
      {
        _json.replace(F(HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE),
                      F(HA_MQTT_DISCOVERY_VALUE_TEMPLATE_IR));
      }

#endif // AFE_CONFIG_HARDWARE_TSL2561

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
    if (deviceConfiguration->entityId == AFE_CONFIG_HA_TYPE_OF_ENTITY_SELECT)
    {
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_BODY_OPTIONS),
                    FPSTR(HA_MQTT_DISCOVERY_JSON_OPTIONS));
      _json.replace(F(HA_MQTT_DISCOVERY_TAG_OPTIONS),
                    deviceConfiguration->options);
    }

    /**
     * @brief RGB LED config
     *
     */
    if (deviceConfiguration->entityId == AFE_CONFIG_HA_TYPE_OF_ENTITY_LIGHT)
    {
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

    _json.replace(F(HA_MQTT_DISCOVERY_TAG_DEVICE_CONFIGURAION_URL),
                  WiFi.localIP().toString());
    char _deviceId[AFE_CONFIG_DEVICE_ID_SIZE];
    _Firmware->API->Flash->getDeviceID(_deviceId);
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_DEVICE_ID), _deviceId);
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_DEVICE_SOFTWARE), _firmwareName);
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_DEVICE_MANUFACTURER),
                  AFE_DEVICE_MANUFACTURER);
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_DEVICE_NAME),
                  _Firmware->Device->configuration.name);
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_DEVICE_HARDWARE),
                  F(AFE_DEVICE_TYPE_NAME));
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_UNIQUE_ID), _objectId);
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_NAME), deviceConfiguration->label);
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_STATE_TEMPLATE),
                  deviceConfiguration->mqtt.topic);
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_RETAIN_FLAG),
                  F("false")); // Setting retain flag to false @TODO HA
#if defined(ESP8285)
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_DEVICE_HARDWARE_CHIP), "ESP8285");
#elif defined(ESP8266)
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_DEVICE_HARDWARE_CHIP), "ESP8266");
#else
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_DEVICE_HARDWARE_CHIP), "ESP32");
#endif

    /* Remove unused tags */
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_BODY_COMMAND_TOPIC), "");
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_BODY_PAYLOAD_ON_OFF_CMD), "");
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_BODY_STATE_TOPIC), "");
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_BODY_STATE_ON_OFF_TPL), "");
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_SENSOR), "");
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_BODY_OPTIONS), "");
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_LIGHT_RGB), "");
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_OPTIMISTIC), "");
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_ENTITY_CATEGORY), "");
    _json.replace(F(HA_MQTT_DISCOVERY_TAG_SET_SENSOR_UNIT_OF_MEASURE), "");

    _json.toCharArray(_message, sizeof(_message) + 1);

    boolean _retain = _MqttAPI->Mqtt->configuration->retainAll;
    _MqttAPI->Mqtt->configuration->retainAll =
        configuration->retainConfiguration;
    _MqttAPI->Mqtt->publish(_topic, _message);
    _MqttAPI->Mqtt->configuration->retainAll = _retain;
  }
  else
  {

    removeItemRemovedFromHomeAssistantMQTTDiscovery(deviceConfiguration);
  }
}

void AFEAPIHomeAssistantIntegration::resetDeviceConfiguration(void)
{
  _deviceConfiguration->deviceClass[0] = AFE_EMPTY_STRING;
  _deviceConfiguration->entityCategory[0] = AFE_EMPTY_STRING;
  _deviceConfiguration->entityId = AFE_NONE;
  _deviceConfiguration->hardwareId = AFE_NONE;
  _deviceConfiguration->id = AFE_NONE;
  _deviceConfiguration->label[0] = AFE_EMPTY_STRING;
  _deviceConfiguration->mqtt.topic[0] = AFE_EMPTY_STRING;
  _deviceConfiguration->type = AFE_NONE;
  _deviceConfiguration->unit[0] = AFE_EMPTY_STRING;
}

#endif // AFE_CONFIG_API_HOME_ASSISTANT_ENABLED