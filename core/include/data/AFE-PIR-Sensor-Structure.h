/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_PIR_Structure_h
#define _AFE_PIR_Structure_h

/* Relay states */
#define PIR_OPEN 1
#define PIR_CLOSE 0

#define PIR_NO 0
#define PIR_NC 1

struct PIR {
  uint8_t gpio;
  char name[17];
  boolean state;
  char mqttTopic[50];
  uint8_t ledId;
  uint8_t relayId;
  uint16_t howLongKeepRelayOn;
  boolean invertRelayState;
  byte type;
  uint16_t bouncing;
  unsigned long idx;
};

#endif
