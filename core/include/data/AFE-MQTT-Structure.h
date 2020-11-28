/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_MQTT_Structure_h
#define _AFE_MQTT_Structure_h

#include <AFE-Configuration.h>

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
#include <AFE-DOMOTICZ-Structure.h>
#endif

#include <arduino.h>

struct MQTT_MESSAGE {
  char *topic;
  byte *content;
  uint16_t length;
};

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
struct MQTT_BASIC_CONFIG {
  char topic[65];
};

/* Types of items in MQTT Topics cache */
typedef enum {
#ifdef AFE_CONFIG_HARDWARE_RELAY
  AFE_MQTT_DEVICE_RELAY = 0,
#endif
#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
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
#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER_SENSOR
  AFE_MQTT_DEVICE_ANEMOMETER = 9,
#endif
#ifdef AFE_CONFIG_HARDWARE_RAINMETER_SENSOR
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
} afe_mqtt_standard_device_type_t;

/* MQTT Topics cache structure */
struct MQTT_TOPICS_CACHE {
  MQTT_BASIC_CONFIG message;
  uint8_t id;
  afe_mqtt_standard_device_type_t type;
};
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

struct MQTT {
  char host[33];
  char ip[16];
  uint16_t port;
  char user[33];
  char password[33];
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  DOMOTICZ_BASIC_CONFIG lwt;
#else
  MQTT_BASIC_CONFIG lwt;
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
  uint16_t timeout;
};

#endif
