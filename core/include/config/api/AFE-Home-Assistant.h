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
#define AFE_CONFIG_HA_DEFAULT_DISCOVERY_RETAIN_CONFIGURATION true

/**
 * @brief Item types. required to generated ObjectId
 * 
 */

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
#define AFE_CONFIG_HA_ITEM_SENSOR_HUMIDITY 12
#define AFE_CONFIG_HA_ITEM_SENSOR_ABSOLUTE_HUMIDITY 13
#define AFE_CONFIG_HA_ITEM_SENSOR_HEAT_INDEX 14
#define AFE_CONFIG_HA_ITEM_SENSOR_DEW_POINT 15
#define AFE_CONFIG_HA_ITEM_SENSOR_PERCEPTION 16
#define AFE_CONFIG_HA_ITEM_SENSOR_COMFORT_LEVEL 17
#define AFE_CONFIG_HA_ITEM_SENSOR_RELATIVE_PRESSURE 18
#define AFE_CONFIG_HA_ITEM_SENSOR_ABSOLUTE_PRESSURE 19
#define AFE_CONFIG_HA_ITEM_SENSOR_IAQ 20
#define AFE_CONFIG_HA_ITEM_SENSOR_STATIC_IAQ 21
#define AFE_CONFIG_HA_ITEM_SENSOR_CO2_EQUIVALENT 22
#define AFE_CONFIG_HA_ITEM_SENSOR_BVOC_EQUIVALENT 23
#define AFE_CONFIG_HA_ITEM_SENSOR_GAS_RESISTANCE 24
#define AFE_CONFIG_HA_ITEM_SENSOR_ILLUMINANCE 25
#define AFE_CONFIG_HA_ITEM_SENSOR_IR 26
#define AFE_CONFIG_HA_ITEM_SENSOR_PM10 27
#define AFE_CONFIG_HA_ITEM_SENSOR_PM10_WHO 28
#define AFE_CONFIG_HA_ITEM_SENSOR_PM25 29
#define AFE_CONFIG_HA_ITEM_SENSOR_PM25_WHO 30
#define AFE_CONFIG_HA_ITEM_SENSOR_ANEMOMETER_KMH 31
#define AFE_CONFIG_HA_ITEM_SENSOR_ANEMOMETER_MS 32
#define AFE_CONFIG_HA_ITEM_SENSOR_RAINMETER_MMM 33
#define AFE_CONFIG_HA_ITEM_SENSOR_RAINMETER_MMH 34
#define AFE_CONFIG_HA_ITEM_SENSOR_RAINMETER_MM12H 35
#define AFE_CONFIG_HA_ITEM_SENSOR_RAINMETER_MM24H 36
#define AFE_CONFIG_HA_ITEM_SENSOR_BATTERYMETER_PERCENT 37
#define AFE_CONFIG_HA_ITEM_SENSOR_BATTERYMETER_VOLT 38

/**
 * @brief Hardware Ids required to generated ObjectId
 * 
 */

#define AFE_CONFIG_HA_HARDWARE_RELAY 0
#define AFE_CONFIG_HA_HARDWARE_SWITCH 1
#define AFE_CONFIG_HA_HARDWARE_CLED 2
#define AFE_CONFIG_HA_HARDWARE_ADC 3
#define AFE_CONFIG_HA_HARDWARE_REGULATOR 4
#define AFE_CONFIG_HA_HARDWARE_SENSOR_DS18B20 5
#define AFE_CONFIG_HA_HARDWARE_SENSOR_DHTXX 6
#define AFE_CONFIG_HA_HARDWARE_SENSOR_BMX60 7
#define AFE_CONFIG_HA_HARDWARE_SENSOR_BH1750 8
#define AFE_CONFIG_HA_HARDWARE_SENSOR_HPMA115S0 9
#define AFE_CONFIG_HA_HARDWARE_SENSOR_ANEMOMETER 10
#define AFE_CONFIG_HA_HARDWARE_SENSOR_RAINMETER 11
#define AFE_CONFIG_HA_HARDWARE_SENSOR_TSL2561 12
#define AFE_CONFIG_HA_HARDWARE_SENSOR_BINARY 13


/**
 * @brief Sizes of variables
 *
 */
#define AFE_CONFIG_HA_OBJECT_ID_SIZE 28
#define AFE_CONFIG_HA_LABEL_SIZE 55 //33 + "Temperatura odczuwalna"
#define AFE_CONFIG_HA_OPTIONS_SIZE 144 // Currently only from RGB effect options
#define AFE_CONFIG_HA_DEVICE_TYPE_SIZE sizeof(AFE_CONFIG_HA_DEVICE_CLASS_CO2) // max of DeviceType
/**
 * @brief Size of configuraion topic
 * Max at this stage: MQTT_TOPIC_LENGTH_64/binary_sensor/AFE_CONFIG_HA_OBJECT_ID_SIZE/config
  */
#define AFE_CONFIG_HA_PUBLISH_TOPIC_SIZE AFE_CONFIG_HA_OBJECT_ID_SIZE+64+22


/**
 * @brief Device types 
 * 
 */
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
 * https://www.home-assistant.io/integrations/binary_sensor/#device-class 4 Binary sensot
 */

#define AFE_CONFIG_HA_DEVICE_CLASS_NONE "None"
#define AFE_CONFIG_HA_DEVICE_CLASS_RELAY "switch"
#define AFE_CONFIG_HA_DEVICE_CLASS_VOLTAGE "voltage"
#define AFE_CONFIG_HA_DEVICE_CLASS_OPENING "opening"
#define AFE_CONFIG_HA_DEVICE_CLASS_MOVING "moving"
#define AFE_CONFIG_HA_DEVICE_CLASS_TEMPERATURE "temperature"
#define AFE_CONFIG_HA_DEVICE_CLASS_HUMIDITY "humidity"
#define AFE_CONFIG_HA_DEVICE_CLASS_PRESSURE "pressure"
#define AFE_CONFIG_HA_DEVICE_CLASS_AQI "aqi"
#define AFE_CONFIG_HA_DEVICE_CLASS_CO2 "carbon_dioxide"
#define AFE_CONFIG_HA_DEVICE_CLASS_GAS "gas"
#define AFE_CONFIG_HA_DEVICE_CLASS_ILLUMINANCE "illuminance"
#define AFE_CONFIG_HA_DEVICE_CLASS_PM10 "pm10"
#define AFE_CONFIG_HA_DEVICE_CLASS_PM25 "pm25"
#define AFE_CONFIG_HA_DEVICE_CLASS_BATTERY "battery"
#define AFE_CONFIG_HA_DEVICE_CLASS_SWITCH AFE_CONFIG_HA_DEVICE_CLASS_NONE
#define AFE_CONFIG_HA_DEVICE_CLASS_BINARY_SENSOR AFE_CONFIG_HA_DEVICE_CLASS_OPENING 



/**
 * @brief Home Assistant State Classes
 * https://developers.home-assistant.io/docs/core/entity/sensor/#available-state-classes
 */
#define AFE_CONFIG_HA_STATE_CLASS_MEASUREMENT "measurement"
#define AFE_CONFIG_HA_STATE_CLASS_TOTAL "total"
#define AFE_CONFIG_HA_STATE_CLASS_TOTAL_INCREASING "total_increasing"


/**
 * @brief HA Configuratio JSON items
 * 
 */

#define HA_MQTT_DISCOVERY_TAG_SET_RETAIN_FLAG "{{ret}}"
#define HA_MQTT_DISCOVERY_TAG_SET_AVAILABILITY "{{b.a}}"
#define HA_MQTT_DISCOVERY_TAG_SET_BODY_COMMAND_TOPIC "{{bct}}"
#define HA_MQTT_DISCOVERY_TAG_SET_BODY_STATE_TOPIC "{{bst}}"
#define HA_MQTT_DISCOVERY_TAG_SET_BODY_STATE_ON_OFF_TPL "{{bsp}}"
#define HA_MQTT_DISCOVERY_TAG_SET_BODY_PAYLOAD_ON_OFF_CMD "{{bcp}}"
#define HA_MQTT_DISCOVERY_TAG_SET_BODY_DEVICE_CLASS "{{bdc}}"
#define HA_MQTT_DISCOVERY_TAG_SET_BODY_OPTIONS "{{bdo}}"
#define HA_MQTT_DISCOVERY_TAG_SET_SENSOR "{{sen}}"
#define HA_MQTT_DISCOVERY_TAG_SET_LIGHT_RGB "{{rgb}}"



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




/**
 * @brief Tags
 * 
 */

#define HA_MQTT_DISCOVERY_TAG_DEVICE_ID "{{d.i}}"
#define HA_MQTT_DISCOVERY_TAG_DEVICE_SOFTWARE "{{d.s}}"
#define HA_MQTT_DISCOVERY_TAG_DEVICE_MANUFACTURER "{{d.m}}"
#define HA_MQTT_DISCOVERY_TAG_DEVICE_NAME "{{d.n}}" 
#define HA_MQTT_DISCOVERY_TAG_DEVICE_HARDWARE "{{d.h}}"
#define HA_MQTT_DISCOVERY_TAG_UNIQUE_ID "{{i.i}}"
#define HA_MQTT_DISCOVERY_TAG_NAME "{{i.n}}" 
#define HA_MQTT_DISCOVERY_TAG_STATE_TEMPLATE "{{i.t}}"
#define HA_MQTT_DISCOVERY_TAG_RETAIN_FLAG "{{r.f}}"
#define HA_MQTT_DISCOVERY_TAG_AVAILABILITY_TOPIC "{{a.t}}"
#define HA_MQTT_DISCOVERY_TAG_VALUE_TEMPLATE "{{s.vt}}"
#define HA_MQTT_DISCOVERY_TAG_UNIT_OF_MEASURE "{{s.u}}"
#define HA_MQTT_DISCOVERY_TAG_DEVICE_CLASS "{{s.dc}}"
#define HA_MQTT_DISCOVERY_TAG_OPTIONS "{{o.o}}"
#define HA_MQTT_DISCOVERY_TAG_OPTIMISTIC "{{opt}}"
#define HA_MQTT_DISCOVERY_TAG_LIGHT_BRIGHTNESS "{{bsc}}"
#define HA_MQTT_DISCOVERY_TAG_LIGHT_TOPIC_SUFIX "{{i.ts}}"


/**
 * @brief JSON Value templates
 * 
 */

#define HA_MQTT_DISCOVERY_VALUE_TEMPLATE_TEMPERATURE "{{value_json.temperature.value}}"
#define HA_MQTT_DISCOVERY_VALUE_TEMPLATE_HUMIDITY "{{value_json.humidity.value}}"
#define HA_MQTT_DISCOVERY_VALUE_TEMPLATE_ABSOLUTE_HUMIDITY "{{value_json.absoluteHumidity.value}}"
#define HA_MQTT_DISCOVERY_VALUE_TEMPLATE_DEW_POINT "{{value_json.dewPoint.value}}"
#define HA_MQTT_DISCOVERY_VALUE_TEMPLATE_HEAT_INDEX "{{value_json.heatIndex.value}}"
#define HA_MQTT_DISCOVERY_VALUE_TEMPLATE_PERCEPTION "{{value_json.perception.description}}"
#define HA_MQTT_DISCOVERY_VALUE_TEMPLATE_COMFORT "{{value_json.comfort.description}}"
#define HA_MQTT_DISCOVERY_VALUE_TEMPLATE_RELATIVE_PRESSURE "{{value_json.relativePressure.value}}"
#define HA_MQTT_DISCOVERY_VALUE_TEMPLATE_ABSOLUTE_PRESSURE "{{value_json.pressure.value}}"
#define HA_MQTT_DISCOVERY_VALUE_TEMPLATE_IAQ "{{value_json.iaq.value}}"
#define HA_MQTT_DISCOVERY_VALUE_TEMPLATE_STATIC_IAQ "{{value_json.staticIaq.value}}"
#define HA_MQTT_DISCOVERY_VALUE_TEMPLATE_CO2_EQUIVALENT "{{value_json.co2Equivalent.value}}"
#define HA_MQTT_DISCOVERY_VALUE_TEMPLATE_BVOC_EQUIVALENT "{{value_json.breathVocEquivalent.value}}"
#define HA_MQTT_DISCOVERY_VALUE_TEMPLATE_GAS_RESISTANCE "{{value_json.gasResistance.value}}"
#define HA_MQTT_DISCOVERY_VALUE_TEMPLATE_VOLTAGE "{{value_json.voltage}}"
#define HA_MQTT_DISCOVERY_VALUE_TEMPLATE_VOLTAGE_CALCULATED "{{value_json.voltageCalculated}}"
#define HA_MQTT_DISCOVERY_VALUE_TEMPLATE_RAW "{{value_json.raw}}"
#define HA_MQTT_DISCOVERY_VALUE_TEMPLATE_PERCENT "{{value_json.percent}}"
#define HA_MQTT_DISCOVERY_VALUE_TEMPLATE_ILLUMINANCE "{{value_json.illuminance.value}}"
#define HA_MQTT_DISCOVERY_VALUE_TEMPLATE_IR "{{value_json.IR.value}}"
#define HA_MQTT_DISCOVERY_VALUE_TEMPLATE_PM10 "{{value_json.PM10.value}}"
#define HA_MQTT_DISCOVERY_VALUE_TEMPLATE_PM10_WHO "{{value_json.WHO.PM10.value}}"
#define HA_MQTT_DISCOVERY_VALUE_TEMPLATE_PM25 "{{value_json.PM25.value}}"
#define HA_MQTT_DISCOVERY_VALUE_TEMPLATE_PM25_WHO "{{value_json.WHO.PM25.value}}"
#define HA_MQTT_DISCOVERY_VALUE_TEMPLATE_ANEMOMETER_KMH "{{value_json.anemometer[1].value}}"
//#define HA_MQTT_DISCOVERY_VALUE_TEMPLATE_ANEMOMETER_KMH "{{value_json.anemometer[?(@.unit=='km/h')].value}}"
#define HA_MQTT_DISCOVERY_VALUE_TEMPLATE_ANEMOMETER_MS "{{value_json.anemometer[0].value}}"
//#define HA_MQTT_DISCOVERY_VALUE_TEMPLATE_ANEMOMETER_MS "{{value_json.anemometer[?(@.unit=='m/s')].value}}"
#define HA_MQTT_DISCOVERY_VALUE_TEMPLATE_RAINMETER_MMM "{{value_json.rainmeter[0].value}}"
//#define HA_MQTT_DISCOVERY_VALUE_TEMPLATE_RAINMETER_MMM "{{value_json.rainmeter[?(@.unit=='mm/min')].value}}"
#define HA_MQTT_DISCOVERY_VALUE_TEMPLATE_RAINMETER_MMH "{{value_json.rainmeter[1].value}}"
//#define HA_MQTT_DISCOVERY_VALUE_TEMPLATE_RAINMETER_MMH "{{value_json.rainmeter[?(@.unit=='mm/h')].value}}"
#define HA_MQTT_DISCOVERY_VALUE_TEMPLATE_RAINMETER_MM12H "{{value_json.rainmeter[2].value}}"
//#define HA_MQTT_DISCOVERY_VALUE_TEMPLATE_RAINMETER_MM12H "{{value_json.rainmeter[?(@.unit=='mm/12h')].value}}"
#define HA_MQTT_DISCOVERY_VALUE_TEMPLATE_RAINMETER_MM24H "{{value_json.rainmeter[3].value}}"
//#define HA_MQTT_DISCOVERY_VALUE_TEMPLATE_RAINMETER_MM24H "{{value_json.rainmeter[?(@.unit=='mm/24h')].value}}"

#define HA_MQTT_DISCOVERY_VALUE_TEMPLATE_BATTERY_PERCENT "{{value_json.batterymeter[0].value}}"
#define HA_MQTT_DISCOVERY_VALUE_TEMPLATE_BATTERY_VOLT "{{value_json.batterymeter[1].value}}"





#endif // _AFE_Hardware_api_home_assistant_h