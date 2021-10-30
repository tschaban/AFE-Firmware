#include "AFE-API-HomeAssistant-Integration.h"
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
void AFEAPIHomeAssistantIntegration::publishRelays() {

  RELAY configuration;
  for (uint8_t i = 0; i < _Device->configuration.noOfRelays; i++) {
#ifdef DEBUG
    Serial << endl << F("INFO: HA: Setting/Updating RGB Relay: ") << i + 1;
#endif
    _Data->getConfiguration(i, &configuration);
    generateSwitch(i, AFE_CONFIG_HA_ITEM_RELAY, configuration.mqtt.topic,
                   configuration.name);
  }
}
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_CLED
void AFEAPIHomeAssistantIntegration::publishCLEDs() {

  CLED configuration;
  CLED_EFFECT_BLINKING configurationEffectBlinkng;
  CLED_EFFECT_FADE_INOUT configurationEffectFadeInOut;
  CLED_EFFECT_WAVE configurationEffectWave;

  for (uint8_t i = 0; i < _Device->configuration.noOfCLEDs; i++) {

#ifdef DEBUG
    Serial << endl << F("INFO: HA: Setting/Updating RGB LEDs Strip: ") << i + 1;
#endif

    _Data->getConfiguration(i, &configuration);
    generateSwitch(i, AFE_CONFIG_HA_ITEM_LIGHT, configuration.mqtt.topic,
                   configuration.name);

    _Data->getConfiguration(i, &configurationEffectBlinkng);
    generateSwitch(i,
                   AFE_CONFIG_HA_ITEM_RGB_LED_EFFECT,
                   configurationEffectBlinkng.mqtt.topic,
                   configurationEffectBlinkng.name, AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING);

    _Data->getConfiguration(i, &configurationEffectFadeInOut);
    generateSwitch(i,
                   AFE_CONFIG_HA_ITEM_RGB_LED_EFFECT,
                   configurationEffectFadeInOut.mqtt.topic,
                   configurationEffectFadeInOut.name, AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT);

    _Data->getConfiguration(i, &configurationEffectWave);
    generateSwitch(i,
                   AFE_CONFIG_HA_ITEM_RGB_LED_EFFECT,
                   configurationEffectWave.mqtt.topic,
                   configurationEffectWave.name, AFE_CONFIG_HARDWARE_CLED_EFFECT_WAVE);
  }
}
#endif // AFE_CONFIG_HARDWARE_CLED

/******* Private Methods *******/

void AFEAPIHomeAssistantIntegration::generateObjectId(char *objectId,
                                                      uint8_t deviceClassId,
                                                      uint8_t id, uint8_t supportingId) {
  sprintf(objectId, "%s-%d%d%d", _deviceID, deviceClassId, id, supportingId);
}

void AFEAPIHomeAssistantIntegration::generateTopic(char *topic,
                                                   uint8_t deviceClassId,
                                                   const char *objectId) {

  sprintf(topic, "homeassistant/%s/%s/config",
          deviceClassId == AFE_CONFIG_HA_ITEM_RELAY ||
                  deviceClassId == AFE_CONFIG_HA_ITEM_LIGHT ||
                  deviceClassId == AFE_CONFIG_HA_ITEM_RGB_LED_EFFECT
              ? "switch"
              : "contact",
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
            ",\"avty\":{\"topic\":\"%s\",\"payload_available\":\"connected"
            "\",\"payload_not_available\":\"disconnected\"}",
            mqttConfiguration.lwt.topic);
  } else {
    availability[0] = AFE_EMPTY_STRING;
  }
}

void AFEAPIHomeAssistantIntegration::generateSwitch(uint8_t id, uint8_t type,
                                                    const char *topic,
                                                    const char *label, uint8_t supportingId) {

#ifdef DEBUG
  Serial << endl << F("INFO: HA: Setting/Updating item: ") << label;
#endif

  if (strlen(topic) > 0) {
    char _topic[200];
    char _message[500];
    char _objectId[50];
    char _availability[200];

    generateObjectId(_objectId, type, id, supportingId);
    generateTopic(_topic, type, _objectId);
    generateAvailability(_availability);

    char _deviceRegistry[200];
    generateDeviceRegistry(_deviceRegistry);
    sprintf(_message, "{\"uniq_id\":\"%s\",\"name\":\"%s\",\"cmd_t\":\"%s/"
                      "cmd\",\"stat_t\":\"%"
                      "s/state\",\"pl_on\":\"on\",\"pl_off\":\"off\",\"stat_"
                      "on\":\"on\",\"stat_off\":\"off\",\"ret\":false,%s%s}",
            _objectId, label, topic, topic, _deviceRegistry, _availability);

    _MqttAPI->Mqtt.publish(_topic, _message);
  }
}

#endif // AFE_CONFIG_API_HOME_ASSISTANT_ENABLED