/**
 * @file AFE-API-HomeAssistant-Integration.h
 * @author Tschaban-A (github@adrian.czabanowski.com)
 * @brief Responsible for Home Assistant MQTT Discovery
 * @version 3.2.0
 * @date 2021-10-23
 *
 * @copyright AFE Firmware @2021
 *
 */
#ifndef _AFE_FIRMWARE_API_HOME_ASSISTANT_INTEGRATION_h
#define _AFE_FIRMWARE_API_HOME_ASSISTANT_INTEGRATION_h

#include <AFE-Configuration.h>

#ifdef AFE_CONFIG_API_HOME_ASSISTANT_ENABLED

#include <AFE-API-MQTT-Standard.h>
#include <AFE-Data-Access.h>
#include <AFE-Device.h>
#include <AFE-Site-components.h>
#include <ArduinoJson.h>

#if AFE_LANGUAGE == 0
#include <pl_PL.h>
#else
#include <en_EN.h>
#endif

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEAPIHomeAssistantIntegration {

private:
  AFEDataAccess *_Data;
  AFEDevice *_Device;
  AFEAPIMQTTStandard *_MqttAPI;

  char _deviceID[18];
  char _firmwareName[28];
  MQTT _mqttConfiguration;

  boolean _initialize = false;

  void generateObjectId(char *objectId, uint8_t deviceClassId,
                        uint8_t id = AFE_HARDWARE_ITEM_NOT_EXIST,
                        uint8_t supportingId = AFE_NONE);
  void generateTopic(char *topic, uint8_t type, const char *objectId);
  void generateAvailability(char *availability);
  void generateDeviceRegistry(char *device);
  void publishItemToHomeAssistantMQTTDiscovery(uint8_t id, uint8_t type,
                                               const char *topic,
                                               const char *label,
                                               uint8_t supportingId = AFE_NONE);

  void removeItemRemovedFromHomeAssistantMQTTDiscovery(
      uint8_t id, uint8_t type, uint8_t supportingId = AFE_NONE);

  uint8_t getTypeOfHAEntity(uint8_t deviceClassId);

public:
  HOME_ASSISTANT_CONFIG configuration;

  /**
   * @brief Construct a new AFEAPIHomeAssistantIntegration object
   *
   */
  AFEAPIHomeAssistantIntegration(void);

  void begin(AFEDataAccess *Data, AFEDevice *Device,
             AFEAPIMQTTStandard *MqttAPI);

#ifdef AFE_CONFIG_HARDWARE_RELAY
  void publishRelays(void);
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_CLED
  void publishCLEDs(void);
#endif // AFE_CONFIG_HARDWARE_CLED

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  void publishSwitches(void);
#endif // AFE_CONFIG_HARDWARE_SWITCH

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
  void publishAnalogInputs(void);
#endif // AFE_CONFIG_HARDWARE_ANALOG_INPUT
};

#endif // AFE_CONFIG_API_HOME_ASSISTANT_ENABLED
#endif //_AFE_FIRMWARE_API_HOME_ASSISTANT_INTEGRATION_h
