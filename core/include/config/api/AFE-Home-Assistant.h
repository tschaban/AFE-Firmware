#ifndef _AFE_Hardware_api_home_assistant_h
#define _AFE_Hardware_api_home_assistant_h

#define AFE_FIRMWARE_API AFE_FIRMWARE_API_HOME_ASSISTANT // Type of the firmware API: Home Assistant

#define AFE_CONFIG_MQTT_TOPIC_CMD_LENGTH  69 // Size of a Command topic: MQTT_BASIC_CONFIG + 4
#define AFE_CONFIG_MQTT_TOPIC_STATE_LENGTH 71 // Size of a State topic: MQTT_BASIC_CONFIG + 6 

#define AFE_CONFIG_HA_PUBLISH_TOPIC_SIZE 117 // For device: binary_sensor @TODO HA once new devices added

#define AFE_CONFIG_HA_DEFAULT_DISCOVERY_TOPIC "homeassistant"
#define AFE_CONFIG_HA_DEFAULT_DISCOVERY_ADDING_COMPONENTS true 
#define AFE_CONFIG_HA_DEFAULT_DISCOVERY_REMOVING_COMPONENTS true 


#define AFE_CONFIG_HA_ITEM_RELAY 0
#define AFE_CONFIG_HA_ITEM_SWITCH 1
#define AFE_CONFIG_HA_ITEM_LIGHT 2          // CLED
#define AFE_CONFIG_HA_ITEM_RGB_LED_EFFECT 3 // CLED
#define AFE_CONFIG_HA_ITEM_SENSOR_ADC_RAW 4
#define AFE_CONFIG_HA_ITEM_SENSOR_ADC_PERCENT 5
#define AFE_CONFIG_HA_ITEM_SENSOR_ADC_VOLTAGE 6
#define AFE_CONFIG_HA_ITEM_SENSOR_ADC_VOLTAGE_CALCULATED 7
#define AFE_CONFIG_HA_ITEM_BINARY_SENSOR 8

#define AFE_CONFIG_HA_OBJECT_ID_SIZE 25
#define AFE_CONFIG_HA_CONFIGURATION_JSON_SIZE 700

#define AFE_CONFIG_HA_TYPE_OF_ENTITY_SWITCH 0
#define AFE_CONFIG_HA_TYPE_OF_ENTITY_SWITCH_NAME "switch"
#define AFE_CONFIG_HA_TYPE_OF_ENTITY_SENSOR 1
#define AFE_CONFIG_HA_TYPE_OF_ENTITY_SENSOR_NAME "sensor"
#define AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_ON_OFF 2
#define AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_OPEN_CLOSED 3
#define AFE_CONFIG_HA_TYPE_OF_ENTITY_BINARY_SENSOR_NAME "binary_sensor"

#define AFE_CONFIG_HA_TYPE_OF_ENTITY_UNKNOWN "unknown"

/**
 * @brief Home assistant Device Classes
 * https://www.home-assistant.io/integrations/sensor/#device-class
 */
#define AFE_CONFIG_HA_DEVICE_CLASS_NONE "None"
#define AFE_CONFIG_HA_DEVICE_CLASS_VOLTAGE "voltage"
#define AFE_CONFIG_HA_DEVICE_CLASS_OPENING "opening"
#define AFE_CONFIG_HA_DEVICE_CLASS_MOVING "moving"
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
    "\"uniq_id\":\"{{i.i}}\",\"name\":\"{{i.n}}\"{{b.retain}}{{b.a}}{{bst}}{{"
    "bsp}}"
    "{{bct}}{{bcp}}{{b.sensor}}{{bdc}}}";

const char HA_MQTT_DISCOVERY_JSON_AVAILABILITY[] PROGMEM =
    ",\"avty\":{\"topic\":\"{{a.t}}\",\"payload_available\":\"connected\","
    "\"payload_not_available\":\"disconnected\"}";

const char HA_MQTT_DISCOVERY_JSON_STATE_TOPIC[] PROGMEM =
    ",\"stat_t\":\"{{i.t}}/state\"";

const char HA_MQTT_DISCOVERY_JSON_STATE_ON_OFF[] PROGMEM =
    ",\"stat_on\":\"on\",\"stat_off\":\"off\"";

const char HA_MQTT_DISCOVERY_JSON_COMMAND_TOPIC[] PROGMEM =
    ",\"cmd_t\":\"{{i.t}}/cmd\"";

const char HA_MQTT_DISCOVERY_JSON_COMMAND_ON_OFF[] PROGMEM =
    ",\"pl_on\":\"on\",\"pl_off\":\"off\"";

const char HA_MQTT_DISCOVERY_JSON_COMMAND_OPEN_CLOSED[] PROGMEM =
    ",\"pl_on\":\"closed\",\"pl_off\":\"open\"";

const char HA_MQTT_DISCOVERY_JSON_SENSOR_COMMON[] PROGMEM =
    ",\"val_tpl\":\"{{s.vt}}\",\"stat_t\":\"{{i.t}}\","
    "\"unit_of_meas\":\"{{s.u}}\"";

const char HA_MQTT_DISCOVERY_JSON_DEVICE_CLASS[] PROGMEM =
    ",\"dev_cla\":\"{{s.dc}}\"";

const char HA_MQTT_DISCOVERY_JSON_RETAIN_FLAG[] PROGMEM = ",\"ret\":{{r.f}}";

#endif // _AFE_Hardware_api_home_assistant_h