#ifndef _AFE_Hardware_api_home_assistant_h
#define _AFE_Hardware_api_home_assistant_h

#define AFE_FIRMWARE_API AFE_API_HOME_ASSISTANT // Type of the firmware API: Home Assistant
#define AFE_CONFIG_MQTT_TOPIC_CMD_LENGTH 69 // Size of a Command topic: MQTT_BASIC_CONFIG + 4
#define AFE_CONFIG_MQTT_TOPIC_STATE_LENGTH 71 // Size of a State topic: MQTT_BASIC_CONFIG + 6

#define AFE_CONFIG_HA_ITEM_RELAY 0
#define AFE_CONFIG_HA_ITEM_SWITCH 1
#define AFE_CONFIG_HA_ITEM_LIGHT 2
#define AFE_CONFIG_HA_ITEM_RGB_LED_EFFECT 3

#endif // _AFE_Hardware_api_home_assistant_h