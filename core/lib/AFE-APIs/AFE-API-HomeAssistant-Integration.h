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

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_HOME_ASSISTANT
#include <AFE-Firmware.h>
#include <AFE-API-MQTT-Standard.h>
#include <AFE-MQTT-Structure.h>
#include <AFE-Site-components.h>
#include <ArduinoJson.h>

#ifdef AFE_CONFIG_HARDWARE_CLED
#include <hardwares/AFE-RGB-LED.h>
#endif

#if AFE_LANGUAGE == 0
#include <pl_PL.h>
#else
#include <en_EN.h>
#endif

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEAPIHomeAssistantIntegration
{

private:
  AFEFirmware *_Firmware;
  AFEAPIMQTTStandard *_MqttAPI;

  HOME_ASSISTANT_CONFIG *configuration = new HOME_ASSISTANT_CONFIG;

  char _firmwareName[28];
  MQTT _mqttConfiguration;

  boolean _initialize = false;

  struct HA_DEVICE_CONFIG
  {
    /**
     * @brief Unique ID of the type of the hardware. Require to generate the
     * hardware item ID in HA
     *
     */
    uint8_t id;
    /**
     * @brief Hardware item type required to generated item ID in HA
     *
     */
    uint8_t type;
    uint8_t hardwareId;
    char label[AFE_CONFIG_HA_LABEL_SIZE];
    MQTT_TOPIC mqtt;
#ifdef AFE_CONFIG_HARDWARE_CLED
    MQTT_TOPIC brightnessCmd;
    char options[AFE_CONFIG_HA_OPTIONS_SIZE];
#endif
    char unit[AFE_CONFIG_ATTRIBUTE_SIZE_UNIT];
    char deviceClass[AFE_CONFIG_HA_DEVICE_TYPE_SIZE];
    uint8_t entityId;
    char entityCategory[sizeof("diagnostic")];
  };

  HA_DEVICE_CONFIG *_deviceConfiguration = new HA_DEVICE_CONFIG;

  /**
   * @brief Resets deviceConfiguration variable
   *
   */
  void resetDeviceConfiguration(void);

  /**
   * @brief generates HA object ID
   *
   * @param  objectId         returned value
   * @param  deviceClassId    HA device class Id
   * @param  id               hardware item Id
   */
  void generateObjectId(char *objectId, uint8_t deviceClassId,
                        uint8_t hardwareId,
                        uint8_t id = AFE_HARDWARE_ITEM_NOT_EXIST);

  /**
   * @brief generates configuration mqtt topic
   *
   * @param  topic            returned mqtt topic
   * @param  deviceClassId    device class Id
   * @param  objectId         object Id / generated by generateObjectId()
   */
  void generateTopic(char *topic, uint8_t entityId, const char *objectId);

  /**
   * @brief method generates and publishes configuration parameters for device
   * items
   * into HA
   *
   * @param  deviceConfiguration configuration data
   */
  void publishItemToHomeAssistantMQTTDiscovery(
      HA_DEVICE_CONFIG *deviceConfiguration);

  /**
   * @brief method removes device item from Home Assistant
   *
   * @param  deviceConfigurationdesc
   */
  void removeItemRemovedFromHomeAssistantMQTTDiscovery(
      HA_DEVICE_CONFIG *deviceConfiguration);

#ifdef AFE_CONFIG_HARDWARE_RELAY
  /**
   * @brief publishes configuration for all relays
   *
   */
  void publishRelays(void);
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_CLED
  /**
   * @brief publishes configuration for all RGB LEDs
   *
   */
  void publishCLEDs(void);
#endif // AFE_CONFIG_HARDWARE_CLED

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  /**
   * @brief publishes configuration for all switches
   *
   */
  void publishSwitches(void);
#endif // AFE_CONFIG_HARDWARE_SWITCH

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
  /**
   * @brief publishes configuration for all binary sensors
   *
   */
  void publishBinarySensor(void);
#endif // AFE_CONFIG_HARDWARE_BINARY_SENSOR

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
  /**
   * @brief publishes configuration of all analog inputs
   *
   */
  void publishAnalogInputs(void);
#endif // AFE_CONFIG_HARDWARE_ANALOG_INPUT

#ifdef AFE_CONFIG_HARDWARE_DS18B20
  /**
   * @brief publishes configuration of all DS18B20 sensors
   *
   */
  void publishSensorDS18B20(void);
#endif // AFE_CONFIG_HARDWARE_DS18B20

#ifdef AFE_CONFIG_HARDWARE_DHT
  /**
   * @brief publishes configuration of all DHT sensors
   *
   */
  void publishSensorDHT(void);
#endif // AFE_CONFIG_HARDWARE_DHT

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
  /**
   * @brief publishes thermal protector
   *
   */
  void publishThermalProtector(void);
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  /**
   * @brief publishes regulator
   *
   */
  void publishRegulator(void);
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  /**
   * @brief publishes Bosch sensor
   *
   */
  void publishBMX80(void);
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
  /**
   * @brief publishes BH1750 sensor
   *
   */
  void publishBH1750(void);
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  /**
   * @brief publishes HPMA115S0 sensor
   *
   */
  void publishHPMA115S0(void);
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
  /**
   * @brief publishes Anemometer sensor
   *
   */
  void publishAnemometer(void);
#endif
/**
 * @brief publishes rainmeter sensor
 *
 */
#ifdef AFE_CONFIG_HARDWARE_RAINMETER

  void publishRainmeter(void);
#endif

#ifdef AFE_CONFIG_HARDWARE_TSL2561
  /**
   * @brief publishes TSL2561 sensor
   *
   */
  void publishTSL2561(void);
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
  /**
   * @brief publishes gate
   *
   */
  void publishGate(void);
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  /**
   * @brief publishes contracton
   *
   */
  void publishContactron(void);
#endif

#ifdef AFE_CONFIG_HARDWARE_FS3000
  /**
   * @brief publishes FS3000
   *
   */
  void publishFS3000(void);
#endif

  /**
   * @brief publishes firmware version
   *
   */
  void publishFirmwareVersion(void);

#ifdef DEBUG
  void printPublishDeviceItemMessage(const __FlashStringHelper *deviceItemName, uint8_t id = AFE_NONE);
#endif

public:
  /**
   * @brief Construct a new AFEAPIHomeAssistantIntegration object
   *
   */
  AFEAPIHomeAssistantIntegration(AFEFirmware *_Firmware,
                                 AFEAPIMQTTStandard *_MqttAPI);

  /**
   * @brief method to be called to initialize the
   *
   */
  void publish();
};

#endif // AFE_CONFIG_API_HOME_ASSISTANT_ENABLED
#endif //_AFE_FIRMWARE_API_HOME_ASSISTANT_INTEGRATION_h
