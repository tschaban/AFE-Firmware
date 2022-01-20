/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_MQTT_Structure_h
#define _AFE_MQTT_Structure_h

#include <AFE-Configuration.h>
#include <arduino.h>



#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
#include <AFE-DOMOTICZ-Structure.h>
#endif // Domoticz


struct MQTT_MESSAGE {
  char topic[AFE_CONFIG_MQTT_TOPIC_CMD_LENGTH];
  char content[AFE_CONFIG_MQTT_CMD_MESSAGE_LENGTH];
};

/*
struct MQTT_MESSAGE {
  char *topic;
  char *content;
  uint16_t length;
};
*/


#if AFE_FIRMWARE_API != AFE_FIRMWARE_API_DOMOTICZ
struct MQTT_TOPIC {
  char topic[AFE_CONFIG_MQTT_TOPIC_LENGTH];
};

struct MQTT_CMD_TOPIC {
  char topic[AFE_CONFIG_MQTT_TOPIC_CMD_LENGTH];
};



/* Types of items in MQTT Topics cache */
typedef enum {
#ifdef AFE_CONFIG_HARDWARE_RELAY
  AFE_MQTT_DEVICE_RELAY = 0,
#endif
#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
  AFE_MQTT_DEVICE_ADC = 1,
#endif
#ifdef AFE_CONFIG_HARDWARE_SWITCH
  AFE_MQTT_DEVICE_SWITCH = 2,
#endif
#ifdef AFE_CONFIG_HARDWARE_BMEX80
  AFE_MQTT_DEVICE_BMX80 = 3,
#endif
#ifdef AFE_CONFIG_HARDWARE_BH1750
  AFE_MQTT_DEVICE_BH1750 = 4,
#endif
#ifdef AFE_CONFIG_HARDWARE_AS3935
  AFE_MQTT_DEVICE_AS3935 = 5,
#endif
#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  AFE_MQTT_DEVICE_HPMA115S0 = 6,
#endif
#ifdef AFE_CONFIG_HARDWARE_GATE
  AFE_MQTT_DEVICE_GATE = 7,
#endif
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  AFE_MQTT_DEVICE_CONTACTRON = 8,
#endif
#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
  AFE_MQTT_DEVICE_ANEMOMETER = 9,
#endif
#ifdef AFE_CONFIG_HARDWARE_RAINMETER
  AFE_MQTT_DEVICE_RAINMETER = 10,
#endif
#ifdef AFE_CONFIG_HARDWARE_DS18B20
  AFE_MQTT_DEVICE_DS18B20 = 11,
#endif
#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  AFE_MQTT_DEVICE_REGULATOR = 12,
#endif
#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
  AFE_MQTT_DEVICE_THERMAL_PROTECTOR = 13,
#endif
#ifdef AFE_CONFIG_HARDWARE_DHT
  AFE_MQTT_DEVICE_DHT = 14,
#endif
#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
  AFE_MQTT_DEVICE_BINARY_SENSOR = 15,
#endif
#ifdef AFE_CONFIG_HARDWARE_CLED
  AFE_MQTT_DEVICE_CLED = 16,
  AFE_MQTT_DEVICE_CLED_EFFECTS = 17,
  AFE_MQTT_DEVICE_CLED_BRIGHTNESS = 18,
#endif
#ifdef AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT
  AFE_MQTT_DEVICE_CLED_EFFECT_PN532_SENSOR = 19,
#endif
#ifdef AFE_CONFIG_HARDWARE_TSL2561
  AFE_MQTT_DEVICE_TSL2561 = 20,
#endif
} afe_mqtt_standard_device_type_t;

/* MQTT Topics cache structure */
struct MQTT_CMD_TOPICS_CACHE {
  MQTT_CMD_TOPIC message;
  uint8_t id;
  afe_mqtt_standard_device_type_t type;
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
  uint16_t timeout;
  boolean retainLWT;
  boolean retainAll;
  boolean pingHostBeforeConnection;
  uint8_t qos;
};

#endif
