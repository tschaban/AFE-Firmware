/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_PN532_Structure_h
#define _AFE_PN532_Structure_h

#include <arduino.h>

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
#include <AFE-DOMOTICZ-Structure.h>
#endif
#include <AFE-MQTT-Structure.h>

struct PN532_SENSOR {
  char name[17];
  uint8_t interface;
  uint8_t tx;
  uint8_t rx;
#ifdef AFE_ESP32
  uint8_t wirePortId;
#endif
  uint8_t i2cAddress;
  uint16_t requestProcessingTime;
  uint16_t listenerTimeout;
  uint16_t timeout;
#ifdef AFE_CONFIG_HARDWARE_LED
  uint8_t ledID;
#endif
#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  MQTT_BASIC_CONFIG mqtt;
#endif
};

struct PN532_TAG {
  char value[AFE_HARDWARE_PN532_BLOCK_SIZE + 1];
};

struct PN532_TAGS {
  PN532_TAG block[AFE_HARDWARE_PN532_TAG_SIZE];
};

struct MIFARE_CARD {
  char cardId[17];
  uint8_t relayId;
  uint8_t action;
  boolean sendAsSwitch;
  uint16_t howLongKeepState;
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  DOMOTICZ_BASIC_CONFIG domoticz[AFE_HARDWARE_PN532_TAG_SIZE];
#else
  MQTT_BASIC_CONFIG mqtt;
#endif
};

#endif
