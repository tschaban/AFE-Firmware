/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

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
#define SWITCH_RELAY_2 12
#define SWITCH_RELAY_3 13
#define SWITCH_RELAY_4 14
/* Relay states */
#define RELAY_ON 1
#define RELAY_OFF 0

/* Relay states */
#define PIR_OPEN 1
#define PIR_CLOSE 0

struct FIRMWARE {
  char version[7];
  uint8_t type;
  uint8_t autoUpgrade;
  char upgradeURL[120];
};

struct DEVICE {
  char name[32];
  boolean mqttAPI;
  boolean httpAPI;
  boolean domoticzAPI;
  boolean isLED[5];
  boolean isRelay[4];
  boolean isSwitch[5];
  boolean isPIR[4];
  boolean isDS18B20;
  boolean isDHT;
};

struct NETWORK {
  char ssid[32];
  char password[32];
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

struct LED {
  uint8_t gpio;
  boolean changeToOppositeValue;
};

struct RELAYSTAT {
  boolean enabled;
  float turnOn;
  boolean turnOnAbove;
  float turnOff;
  boolean turnOffAbove;
};

struct RELAY {
  uint8_t gpio;
  float timeToOff;
  uint8_t statePowerOn;
  char name[16];
  uint8_t stateMQTTConnected;
  char mqttTopic[49];
  boolean showStatusUsingLED;
  RELAYSTAT thermostat;
  RELAYSTAT humidistat;
  float thermalProtection;
};

struct SWITCH {
  uint8_t gpio;
  uint8_t type;
  uint16_t sensitiveness;
  uint8_t functionality;
};

struct HTTPCOMMAND {
  char device[16];
  char name[16];
  char command[32];
};

struct DS18B20 {
  uint8_t gpio;
  float correction;
  uint16_t interval;
  uint8_t unit;
};

struct TEMPERATURE {
  float correction;
  unsigned int interval;
  uint8_t unit;
};

struct HUMIDITY {
  float correction;
  uint16_t interval;
};

struct DH {
  uint8_t gpio;
  uint8_t type;
  TEMPERATURE temperature;
  HUMIDITY humidity;
};

struct PIR {
  uint8_t gpio;
  char name[16];
  boolean state;
  uint8_t ledId;
  uint8_t relayId;
  uint16_t howLongKeepRelayOn;
  boolean invertRelayState;
};

struct DOMOTICZ {
  char host[32];
  IPAddress ip;
  uint16_t port;
  char user[32];
  char password[32];
};

#endif
