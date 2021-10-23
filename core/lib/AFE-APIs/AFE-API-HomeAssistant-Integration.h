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
#ifndef _AFE_API_HOME_ASSISTANT_INTEGRATION_h
#define _AFE_API_HOME_ASSISTANT_INTEGRATION_h

#include <AFE-Configuration.h>

#ifdef AFE_CONFIG_API_HOME_ASSISTANT_ENABLED

#include <AFE-API-MQTT-Standard.h>
#include <AFE-Data-Access.h>
#include <AFE-Device.h>
#include <ArduinoJson.h>
#include <AFE-Site-components.h>


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
  
  char _deviceID[17];

  void generateObjectId(char *objectId, const char *deviceClass, uint8_t id = AFE_HARDWARE_ITEM_NOT_EXIST);
  void generateTopic(char *topic, const char *deviceClass, const char *objectId);
  void generateAvailability(char *availability);
  void generateDeviceRegistry(char *device);

public:
  /**
   * @brief Construct a new AFEAPIHomeAssistantIntegration object
   *
   */
  AFEAPIHomeAssistantIntegration(void);

  void begin(AFEDataAccess *Data, AFEDevice *Device,
             AFEAPIMQTTStandard *MqttAPI);

#ifdef AFE_CONFIG_HARDWARE_RELAY
  void publishRelay(uint8_t id);
  void publishRelays();
#endif // AFE_CONFIG_HARDWARE_RELAY


};

#endif // AFE_CONFIG_API_HOME_ASSISTANT_ENABLED
#endif //_AFE_API_HOME_ASSISTANT_INTEGRATION_h
