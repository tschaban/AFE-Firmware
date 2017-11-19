/*
   Data structures
   AFE Firmware for smart home devices build on ESP8266
   More info: https://github.com/tschaban/AFE-Firmware
   LICENCE: http://opensource.org/licenses/MIT
 */

#ifndef _AFE_Data_Structures_h
#define _AFE_Data_Structures_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <ESP8266WiFi.h>

#define SERVER_CMD_SAVE 1

/* Functionalities of a switch */
#define SWITCH_MULTI 0
// Not used yet #define SWITCH_CONFIG 1
// Not used yet #define SWITCH_REBOOT 2
#define SWITCH_RELAY_1 11
// Not used yet #define SWITCH_RELAY_2 12
// Not used yet #define SWITCH_RELAY_3 13
// Not used yet #define SWITCH_RELAY_4 14

struct FIRMWARE {
  char version[7];
  uint8_t type;
  uint8_t autoUpgrade;
  char upgradeURL[120];
};

struct LED {
  boolean present;
  uint8_t gpio;
};

struct NETWORK {
  char ssid[32];
  char password[32];
  char host[32];
  uint8_t isDHCP;
  IPAddress ip;
  IPAddress gateway;
  IPAddress subnet;
  uint8_t noConnectionAttempts;
  uint8_t waitTimeConnections;
  uint8_t waitTimeSeries;
};

struct MQTT {
  char host[32];
  IPAddress ip;
  uint16_t port;
  char user[32];
  char password[32];
  char topic[32];
};

struct RELAY {
  boolean present;
  uint8_t gpio;
  float timeToOff;
  uint8_t statePowerOn;
  /* @TODO Only for MQTT */
  char name[16];
  uint8_t stateMQTTConnected;
  char mqttTopic[49];
  /* @TODO Only for Domoticz */
  uint16_t idx;
  boolean publishToDomoticz;
};

struct SWITCH {
  boolean present;
  uint8_t gpio;
  uint8_t type;
  uint16_t sensitiveness;
  uint8_t functionality;
};

/* @TODO DS18B20

struct DS18B20 {
  boolean present;
  uint8_t gpio;
  float correction;
  uint16_t interval;
  uint16_t unit;
};
*/

/* @TODO DOMOTICZ
struct DOMOTICZ {
  char host[32];
  IPAddress ip;
  uint16_t port;
  char user[32];
  char password[32];
};
*/

#endif
