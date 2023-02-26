/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_MQTT_Structure_h
#define _AFE_MQTT_Structure_h

#include <AFE-Configuration.h>
#include <arduino.h>

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
#include <AFE-DOMOTICZ-Structure.h>
struct MQTT_MESSAGE {
  DOMOTICZ_MQTT_COMMAND command;
};
#else
struct MQTT_MESSAGE {
  char topic[AFE_CONFIG_MQTT_TOPIC_CMD_LENGTH];
  char content[AFE_CONFIG_MQTT_CMD_MESSAGE_LENGTH];
};
#endif // Domoticz

/*
struct MQTT_MESSAGE {
  char *topic;
  char *content;
  uint16_t length;
};
*/


struct MQTT_TOPIC {
  char topic[AFE_CONFIG_MQTT_TOPIC_LENGTH];
};

#if AFE_FIRMWARE_API != AFE_FIRMWARE_API_DOMOTICZ
struct MQTT_CMD_TOPIC {
  char topic[AFE_CONFIG_MQTT_TOPIC_CMD_LENGTH];
};

#endif // !AFE_CONFIG_API_DOMOTICZ_ENABLED

struct MQTT {
  char host[33];
  char ip[16];
  uint16_t port;
  char user[33];
  char password[65];
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  DOMOTICZ_BASIC_CONFIG lwt;
#else
  MQTT_TOPIC lwt;  
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
  MQTT_TOPIC status;
  uint16_t timeout;
  boolean retainLWT;
  boolean retainAll;
  boolean pingHostBeforeConnection;
  uint8_t qos;
};

#endif
