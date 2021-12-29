/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_CONTACTRON_Structure_h
#define _AFE_CONTACTRON_Structure_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
#include <arduino.h>

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
#include <AFE-DOMOTICZ-Structure.h>
#endif
#include <AFE-MQTT-Structure.h>

struct CONTACTRON {
  uint8_t gpio;
  byte type = AFE_CONTACTRON_NO;
  uint8_t ledID;
  uint16_t bouncing = AFE_CONFIG_HARDWARE_CONTACTRON_DEFAULT_BOUNCING;
  char name[17];
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  DOMOTICZ_BASIC_CONFIG domoticz;
#else
  MQTT_TOPIC mqtt;
#endif
};

#endif // AFE_CONFIG_HARDWARE_CONTACTRON
#endif // _AFE_CONTACTRON_Structure_h
