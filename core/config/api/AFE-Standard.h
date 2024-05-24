#ifndef _AFE_Hardware_api_standard_h
#define _AFE_Hardware_api_standard_h

#define AFE_FIRMWARE_API AFE_FIRMWARE_API_STANDARD // Type of the firmware API: Stndard

#define AFE_CONFIG_MQTT_TOPIC_STATE_LENGTH 71 // Size of a State topic: MQTT_TOPIC + 6


#ifdef AFE_CONFIG_HARDWARE_CLED
#define AFE_CONFIG_MQTT_TOPIC_CMD_LENGTH AFE_CONFIG_MQTT_TOPIC_LENGTH + 15 // Size of a Command topic: MQTT_TOPIC + /brightness/cmd (15)
#else
#define AFE_CONFIG_MQTT_TOPIC_CMD_LENGTH AFE_CONFIG_MQTT_TOPIC_LENGTH +   4 // Size of a Command topic: MQTT_TOPIC + /cmd (4)
#endif

/**
 * @brief Max message size handed by the API
 * 
 */
#define AFE_CONFIG_MQTT_CMD_MESSAGE_LENGTH 113 // {"color":{"red":255,"green":250,"blue":255} AJ estimated with buffer


#define AFE_CONFIG_MQTT_COMMAND_ON "on"
#define AFE_CONFIG_MQTT_COMMAND_OFF "off"
#define AFE_CONFIG_MQTT_COMMAND_TOGGLE "toggle"
#define AFE_CONFIG_MQTT_COMMAND_GET "get"


#endif // _AFE_Hardware_api_standard_h