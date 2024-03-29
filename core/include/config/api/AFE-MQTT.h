#ifndef _AFE_Hardware_mqtt_bus_h
#define _AFE_Hardware_mqtt_bus_h

#define AFE_CONFIG_MQTT_TOPIC_LENGTH 65
#define AFE_CONFIG_MQTT_DEFAULT_TIMEOUT 5000 // Timeout to shorten wait time, useful to have it low if MQTT server is down
#define AFE_CONFIG_MQTT_DEFAULT_RECONNECT_TIMEOUT 2000 // Timeout to call broker.connect()
#define AFE_CONFIG_MQTT_DEFAULT_PORT 1883 // Default MQTT Broker port
// #define AFE_CONFIG_MQTT_DEFAULT_BUFFER_SIZE 1687 // 1195 // 768 // Default MQTT Buffer size - must handle entire message, max JSON is BME680, AFE_CONFIG_API_JSON_BMEX80_DATA_LENGTH
#define AFE_CONFIG_FUNCTIONALITY_MQTT_LWT
#define AFE_CONFIG_MQTT_DEFAULT_RETAIN_LWT false
#define AFE_CONFIG_MQTT_DEFAULT_RETAIN_ALL false
#define AFE_CONFIG_MQTT_DEFAULT_HOST_PING_BEFORE_CONNECTION false
#define AFE_CONFIG_MQTT_DEFAULT_HOST_PINGS_NUMBER 1
#define AFE_CONFIG_MQTT_DEFAULT_HOST_PINGS_TIMEOUT 1000 // ms
#define AFE_CONFIG_MQTT_DEFAULT_QOS 1
#ifdef AFE_CONFIG_HARDWARE_RELAY 
#define AFE_CONFIG_MQTT_MESSAGES_BUFFER AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_RELAYS+2
#else
#define AFE_CONFIG_MQTT_MESSAGES_BUFFER 4
#endif

#define AFE_CONFIG_MQTT_DEFAULT_STATE_TOPIC "AFE/%s"

const char MQTT_STATUS_FIRMWARE_JSON_TEMPLATE[] PROGMEM = 
"{\"fw\":{\"iv\":\"{{v}}\",\"lv\":\"{{l}}\",\"t\":\"{{t}}\",\"ru\":\"{{u}}\",\"rs\":\"{{s}}\",\"i\":\"{{i}}\"}}";


#define MQTT_STATUS_FIRMWARE_JSON_TAG_INSTALLED_VERSION  "{{v}}"
#define MQTT_STATUS_FIRMWARE_JSON_TAG_LATEST_VERSION  "{{l}}"
#define MQTT_STATUS_FIRMWARE_JSON_TAG_TITLE  "{{t}}"
#define MQTT_STATUS_FIRMWARE_JSON_TAG_RELEASE_URL  "{{u}}"
#define MQTT_STATUS_FIRMWARE_JSON_TAG_RELEASE_SUMMARY  "{{s}}"
#define MQTT_STATUS_FIRMWARE_JSON_TAG_ICON  "{{i}}"



#endif // _AFE_Hardware_mqtt_bus_h