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
void AFEAPIHomeAssistantIntegration::publishRelay(uint8_t id) {

#ifdef DEBUG
  Serial << endl
         << F("INFO: HA: Publishing Relay Id: ") << id
         << F(" to Home Assistant");
#endif

  RELAY configuration;
  _Data->getConfiguration(id, &configuration);
  char _topic[200];
  char _message[500];
  char _objectId[50];

  generateObjectId(_objectId, "switch", id);
  generateTopic(_topic, "switch", _objectId);

  char _deviceRegistry[200];
  generateDeviceRegistry(_deviceRegistry);
  sprintf(
      _message,
      "{\"uniq_id\":\"%s\",\"name\":\"%s\",\"cmd_t\":\"%s/cmd\",\"stat_t\":\"%"
      "s/state\",\"pl_on\":\"on\",\"pl_off\":\"off\",\"stat_"
      "on\":\"on\",\"stat_off\":\"off\",\"ret\":false,%s}",
      _objectId, configuration.name, configuration.mqtt.topic,
      configuration.mqtt.topic, _deviceRegistry);

  _MqttAPI->Mqtt.publish(_topic,_message);

}
#endif // AFE_CONFIG_HARDWARE_RELAY

void AFEAPIHomeAssistantIntegration::generateObjectId(char *objectId,
                                                      const char *deviceClass,
                                                      uint8_t id) {
  sprintf(objectId, "%s-%s-%d", _deviceID, deviceClass, id);
}

void AFEAPIHomeAssistantIntegration::generateTopic(char *topic,
                                                   const char *deviceClass,
                                                   const char *objectId) {

  sprintf(topic, "homeassistant/%s/%s/config", deviceClass, objectId);
  
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
          _deviceID, _Device->configuration.name,
          AFE_DEVICE_MANUFACTURER, AFE_DEVICE_TYPE_NAME, _firmwareName,
          _deviceID);      
}

#endif // AFE_CONFIG_API_HOME_ASSISTANT_ENABLED