#ifndef _AFE_Hardware_api_standard_h
#define _AFE_Hardware_api_standard_h

#define AFE_FIRMWARE_API AFE_FIRMWARE_API_STANDARD // Type of the firmware API: Stndard

#define AFE_CONFIG_MQTT_TOPIC_STATE_LENGTH 71 // Size of a State topic: MQTT_TOPIC + 6

#ifdef AFE_CONFIG_HARDWARE_CLED
#define AFE_CONFIG_MQTT_TOPIC_CMD_LENGTH AFE_CONFIG_MQTT_TOPIC_LENGTH + 15 // Size of a Command topic: MQTT_TOPIC + /brightness/cmd (15)
#else
#define AFE_CONFIG_MQTT_TOPIC_CMD_LENGTH AFE_CONFIG_MQTT_TOPIC_LENGTH +   4 // Size of a Command topic: MQTT_TOPIC + /cmd (4)
#endif

#endif // _AFE_Hardware_api_standard_h