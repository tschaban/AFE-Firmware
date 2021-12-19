/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_AS3935_Structure_h
#define _AFE_AS3935_Structure_h


#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED  
#include <AFE-DOMOTICZ-Structure.h>
#endif
#include <AFE-MQTT-Structure.h>


struct AS3935 {
  char name[17];
  uint8_t i2cAddress;
  uint8_t irqGPIO;
  boolean setNoiseFloorAutomatically;
  uint8_t noiseFloor;                     // 0 - 7
  uint8_t watchdogThreshold;              // 0 - 10
  uint8_t minimumNumberOfLightningSpikes; // Range: [0-3], where 0 - 1 spike, 1
                                          // - 5 spikes, 2 - 9 spikes, 3 -
                                          // 16spikes
  uint8_t spikesRejectionLevel;           // 0 - 11
  bool indoor;
  uint8_t unit; // Distance
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED  
  DOMOTICZ_BASIC_CONFIG domoticz;
#else
  MQTT_TOPIC mqtt;
#endif
};

struct AS3935_DATA {
  uint8_t type;
  uint8_t distance;
};

#endif
