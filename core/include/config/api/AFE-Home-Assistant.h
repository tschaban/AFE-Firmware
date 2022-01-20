#ifndef _AFE_Hardware_api_home_assistant_h
#define _AFE_Hardware_api_home_assistant_h

#include <AFE-Configuration.h>

#define AFE_FIRMWARE_API AFE_FIRMWARE_API_HOME_ASSISTANT // Type of the firmware API: Home Assistant

#include <Arduino.h>


#define AFE_CONFIG_MQTT_TOPIC_STATE_LENGTH AFE_CONFIG_MQTT_TOPIC_LENGTH + 6 // Size of a State topic: MQTT_TOPIC + 6


#ifdef AFE_CONFIG_HARDWARE_CLED
#define AFE_CONFIG_MQTT_TOPIC_CMD_LENGTH AFE_CONFIG_MQTT_TOPIC_LENGTH + 15 // Size of a Command topic: MQTT_TOPIC + /brightness/cmd (15)
#else
#define AFE_CONFIG_MQTT_TOPIC_CMD_LENGTH AFE_CONFIG_MQTT_TOPIC_LENGTH +   4 // Size of a Command topic: MQTT_TOPIC + /cmd (4)
#endif // AFE_CONFIG_HARDWARE_CLED

/**
 * @brief Max message size handed by the API
 * 
 */
#define AFE_CONFIG_MQTT_CMD_MESSAGE_LENGTH 113 // {"color":{"red":255,"green":250,"blue":255}  AJ estimated with buffer


#define AFE_CONFIG_MQTT_COMMAND_ON "on"
#define AFE_CONFIG_MQTT_COMMAND_OFF "off"
#define AFE_CONFIG_MQTT_COMMAND_TOGGLE "toggle"
#define AFE_CONFIG_MQTT_COMMAND_GET "get"


/**
 * @brief Size of configuraion topic
 * Max at this stage: 1234567890123456789012345678901234567890123456789012345678901234/binary_sensor/E86aDBf2-84e0E084-12/config
 * 106 + 3 buffer
 */
#define AFE_CONFIG_HA_PUBLISH_TOPIC_SIZE 110 

/**
 * @brief Length of JSON Configuraion device
 *
 */
#define AFE_CONFIG_HA_CONFIGURATION_JSON_SIZE 1651 // Based on light 

/**
 * @brief HA Integraion defaults. For the configuration file
 *
 */
#define AFE_CONFIG_HA_DEFAULT_DISCOVERY_TOPIC "homeassistant"
#define AFE_CONFIG_HA_DEFAULT_DISCOVERY_ADDING_COMPONENTS true
#define AFE_CONFIG_HA_DEFAULT_DISCOVERY_REMOVING_COMPONENTS true

#define AFE_CONFIG_HA_ITEM_RELAY 0
#define AFE_CONFIG_HA_ITEM_SWITCH 1
#define AFE_CONFIG_HA_ITEM_LIGHT 2          // CLED
#define AFE_CONFIG_HA_ITEM_RGB_LED_EFFECT 3 // CLED EFFECT
#define AFE_CONFIG_HA_ITEM_SENSOR_ADC_RAW 4
#define AFE_CONFIG_HA_ITEM_SENSOR_ADC_PERCENT 5
#define AFE_CONFIG_HA_ITEM_SENSOR_ADC_VOLTAGE 6
#define AFE_CONFIG_HA_ITEM_SENSOR_ADC_VOLTAGE_CALCULATED 7
#define AFE_CONFIG_HA_ITEM_BINARY_SENSOR 8
#define AFE_CONFIG_HA_ITEM_SENSOR_TEMPERATURE 9
#define AFE_CONFIG_HA_ITEM_THERMAL_PROTECTOR 10
#define AFE_CONFIG_HA_ITEM_REGULATOR 11
/**
 * @brief Sizes of variables
 *
 */
#define AFE_CONFIG_HA_OBJECT_ID_SIZE 25
#define AFE_CONFIG_HA_LABEL_SIZE 33
#define AFE_CONFIG_HA_OPTIONS_SIZE 144 // Currently only from RGB effect options
#define AFE_CONFIG_HA_UNIT_SIZE 5

#define AFE_CONFIG_HA_TYPE_OF_ENTITY_SWITCH 0
#define AFE_CONFIG_HA_TYPE_OF_ENTITY_SWITCH_NAME "switch"
#define AFE_CONFIG_HA_TYPE_OF_ENTITY_SENSOR 1
#define AFE_CONFIG_HA_TYPE_OF_ENTITY_SENSOR_NAME "sensor"
#define AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_ON_OFF 2
#define AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_OPEN_CLOSED 3
#define AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_NAME "binary_sensor"
#define AFE_CONFIG_HA_TYPE_OF_ENTITY_SELECT 4
#define AFE_CONFIG_HA_TYPE_OF_ENTITY_SELECT_NAME "select"
#define AFE_CONFIG_HA_TYPE_OF_ENTITY_LIGHT 5
#define AFE_CONFIG_HA_TYPE_OF_ENTITY_LIGHT_NAME "light"


#define AFE_CONFIG_HA_TYPE_OF_ENTITY_UNKNOWN "unknown"

/**
 * @brief Home assistant Device Classes
 * https://www.home-assistant.io/integrations/sensor/#device-class
 */
#define AFE_CONFIG_HA_DEVICE_CLASS_NONE "None"
#define AFE_CONFIG_HA_DEVICE_CLASS_VOLTAGE "voltage"
#define AFE_CONFIG_HA_DEVICE_CLASS_OPENING "opening"
#define AFE_CONFIG_HA_DEVICE_CLASS_MOVING "moving"
#define AFE_CONFIG_HA_DEVICE_CLASS_TEMPERATURE "temperature"
/**
 * @brief Home Assistant State Classes
 * https://developers.home-assistant.io/docs/core/entity/sensor/#available-state-classes
 */
#define AFE_CONFIG_HA_STATE_CLASS_MEASUREMENT "measurement"
#define AFE_CONFIG_HA_STATE_CLASS_TOTAL "total"
#define AFE_CONFIG_HA_STATE_CLASS_TOTAL_INCREASING "total_increasing"

const char HA_MQTT_DISCOVERY_JSON_BODY[] PROGMEM =
    "{\"device\":{\"ids\":\"{{d.i}}\",\"sw\":\"{{d.s}}\",\"mf\":\"{{d.m}}\","
    "\"name\":\"{{d.n}}\",\"via_device\":\"{{d.i}}\",\"mdl\":\"{{d.h}}\"},"
    "\"uniq_id\":\"{{i.i}}\",\"name\":\"{{i.n}}\"{{ret}}{{b.a}}{{bst}}{{"
    "bsp}}{{bct}}{{bcp}}{{sen}}{{bdo}}{{rgb}}{{opt}}{{bdc}}}";

const char HA_MQTT_DISCOVERY_JSON_AVAILABILITY[] PROGMEM =
    ",\"avty\":{\"topic\":\"{{a.t}}\",\"payload_available\":\"connected\","
    "\"payload_not_available\":\"disconnected\"}";

const char HA_MQTT_DISCOVERY_JSON_STATE_TOPIC[] PROGMEM =
    ",\"stat_t\":\"{{i.t}}/state\"";

const char HA_MQTT_DISCOVERY_JSON_STATE_ON_OFF[] PROGMEM =
    ",\"stat_on\":\"on\",\"stat_off\":\"off\"";

const char HA_MQTT_DISCOVERY_JSON_COMMAND_TOPIC[] PROGMEM =
    ",\"cmd_t\":\"{{i.t}}/cmd\"";

const char HA_MQTT_DISCOVERY_JSON_PAYLOAD_ON_OFF[] PROGMEM =
    ",\"pl_on\":\"on\",\"pl_off\":\"off\"";

const char HA_MQTT_DISCOVERY_JSON_COMMAND_OPEN_CLOSED[] PROGMEM =
    ",\"pl_on\":\"closed\",\"pl_off\":\"open\"";

const char HA_MQTT_DISCOVERY_JSON_SENSOR_COMMON[] PROGMEM =
    ",\"val_tpl\":\"{{s.vt}}\",\"stat_t\":\"{{i.t}}\","
    "\"unit_of_meas\":\"{{s.u}}\"";

const char HA_MQTT_DISCOVERY_JSON_DEVICE_CLASS[] PROGMEM =
    ",\"dev_cla\":\"{{s.dc}}\"";

const char HA_MQTT_DISCOVERY_JSON_RETAIN_FLAG[] PROGMEM = ",\"ret\":{{r.f}}";
/**
 * @brief RGB LED specyfic configuration tags
 * 
 */
#ifdef AFE_CONFIG_HARDWARE_CLED

const char HA_MQTT_DISCOVERY_JSON_STATE_VALUE_TEMPLATE[] PROGMEM =
    ",\"stat_val_tpl\":\"{{value_json.state}}\"";

const char HA_MQTT_DISCOVERY_JSON_PAYLOAD_TEMPLATE[] PROGMEM =
    ",\"pl_on\":\"{\\\"command\\\":\\\"on\\\"}\",\"pl_off\":\"{\\\"command\\\":"
    "\\\"off\\\"}\"";

const char HA_MQTT_DISCOVERY_JSON_OPTIONS[] PROGMEM = ",\"options\":[{{o.o}}]";

const char HA_MQTT_DISCOVERY_JSON_RGB_LIGHT[] PROGMEM =
    ",\"rgb_cmd_t\":\"{{i.t}}/cmd\",\"rgb_stat_t\":\"{{i.t}}/"
    "state\",\"rgb_val_tpl\":\"{{value_json.color.red}},{{value_json.color."
    "green}},{{value_json.color.blue"
    "}}\",\"rgb_cmd_tpl\":\"{\\\"color\\\":{\\\"red\\\":{{red}},\\\"green\\\":{"
    "{green}},\\\"blue\\\":{{blue}}}}\",\"on_cmd_type\":\"last\",\"bri_scl\":"
    "{{bsc}},\"bri_val_tpl\":\"{{value_json.brightness}}\",\"bri_stat_t\":\"{{"
    "i.t}}/state\",\"bri_cmd_t\":\"{{i.t}}{{i.ts}}/cmd\"";

const char HA_MQTT_DISCOVERY_JSON_OPTIMISTIC[] PROGMEM = ",\"opt\":true";
#endif

#endif // _AFE_Hardware_api_home_assistant_h