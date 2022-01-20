#ifndef _AFE_Hardware_api_domoticz_h
#define _AFE_Hardware_api_domoticz_h

#define AFE_FIRMWARE_API AFE_FIRMWARE_API_DOMOTICZ // Type of the firmware API: DOMOTICZ
#define AFE_DOMOTICZ_VERSION_0 0 // 4.10x
#define AFE_DOMOTICZ_VERSION_1 1 // 2020.x
#define AFE_DOMOTICZ_VERSION_DEFAULT AFE_DOMOTICZ_VERSION_0
#define AFE_DOMOTICZ_IDX_NOT_PROCESSED 0
#define AFE_DOMOTICZ_DEFAULT_IDX 0 // Default value for IDX
#define AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT "0"
#define AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT "999999"
#define AFE_CONFIG_API_HTTP_TIMEOUT 200 // Time for HTTP response. If 5000 than device goes to config mode if Domoticz is Off, button is pressed and HTTP Domoticz API is on
#define AFE_CONFIG_API_DOMOTICZ_TOPIC_IN "domoticz/in" // MQTT Topic for outgoing from AFE messages
#define AFE_CONFIG_API_DOMOTICZ_TOPIC_OUT "domoticz/out" // MQTT Topic Domoticz uses for publishing messages
/**
 * @brief Size of the incoming JSON from Domoticz
 * currently based on SetColort JSON
 * Warning: if description of item in Domoticz is too large it can cause a problem here
 * It represents the size of incomming command
 * 
 */
#define AFE_CONFIG_MQTT_CMD_MESSAGE_LENGTH 574 // AJ Checked


/**
 * @brief Domoticz 3.1.8 uses MQTT dicovery so the command topics are same as in standard API
 * 
 */
#ifdef AFE_CONFIG_HARDWARE_CLED
#define AFE_CONFIG_MQTT_TOPIC_CMD_LENGTH AFE_CONFIG_MQTT_TOPIC_LENGTH + 15 // Size of a Command topic: MQTT_TOPIC + /brightness/cmd (15)
#else
#define AFE_CONFIG_MQTT_TOPIC_CMD_LENGTH AFE_CONFIG_MQTT_TOPIC_LENGTH +   4 // Size of a Command topic: MQTT_TOPIC + /cmd (4)
#endif // AFE_CONFIG_HARDWARE_CLED

#define AFE_CONFIG_API_JSON_SWITCH_COMMAND_LENGTH 57 // Outgoing MQTT message size for switch
#define AFE_CONFIG_API_JSON_DEVICE_COMMAND_LENGTH 90 // Outgoing MQTT message size for custom sensor
#define AFE_CONFIG_API_JSON_SET_LEVEL_COMMAND_LENGTH 134 // Outgoing MQTT message size for selector item

#ifdef AFE_CONFIG_HARDWARE_GATE
#define AFE_CONFIG_API_JSON_GATE_COMMAND_LENGTH AFE_CONFIG_API_JSON_SWITCH_COMMAND_LENGTH
#endif // AFE_CONFIG_HARDWARE_GATE

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
#define AFE_CONFIG_API_JSON_CONTACTRON_COMMAND_LENGTH AFE_CONFIG_API_JSON_SWITCH_COMMAND_LENGTH
#endif // AFE_CONFIG_HARDWARE_CONTACTRON

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
#define AFE_CONFIG_API_JSON_ANEMOMETER_COMMAND_LENGTH 100 // @TODO T6 to check
#endif // AFE_CONFIG_HARDWARE_ANEMOMETER

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
#define AFE_CONFIG_API_JSON_RAINMETER_COMMAND_LENGTH 100 // @TODO T6 to check
#endif // AFE_CONFIG_HARDWARE_RAINMETER

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
#define AFE_CONFIG_API_JSON_BATTERYMETER_COMMAND_LENGTH 100 // @TODO T6 to check
#endif // AFE_CONFIG_FUNCTIONALITY_BATTERYMETER

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
#define AFE_CONFIG_API_JSON_REGULATOR_COMMAND_LENGTH AFE_CONFIG_API_JSON_SWITCH_COMMAND_LENGTH // it's a switch
#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
#define AFE_CONFIG_API_JSON_THERMAL_PROTECTOR_COMMAND_LENGTH  AFE_CONFIG_API_JSON_SWITCH_COMMAND_LENGTH // it's a switch
#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
#define AFE_CONFIG_API_JSON_BINARY_SENSOR_COMMAND_LENGTH  AFE_CONFIG_API_JSON_SWITCH_COMMAND_LENGTH
#endif

#define AFE_CONFIG_API_MQTT_LWT_MESSAGE_LENGTH 96

#define AFE_CONFIG_API_JSON_SET_COLOR_COMMAND_LENGTH 291

#define AFE_CONFIG_API_DOMOTICZ_URL_LENGTH 190 // Outgoing url size for statuses updates to Domoticz

#ifdef AFE_CONFIG_HUMIDITY
/* Humidity Domoticz state*/
typedef enum {
  AFE_HUMIDITY_NORMAL = 0,
  AFE_HUMIDITY_COMFORTABLE = 1,
  AFE_HUMIDITY_DRY = 2,
  AFE_HUMIDITY_WET = 3,
} afe_humidity_domoticz_state_t;
#endif

#endif // _AFE_Hardware_api_domoticz_h