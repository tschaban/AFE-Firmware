/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_PN532_Structure_h
#define _AFE_PN532_Structure_h

#include <arduino.h>

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
#include <AFE-DOMOTICZ-Structure.h>
#endif
#include <AFE-MQTT-Structure.h>


struct PN532_SENSOR {
  char name[17];  
};

struct PN532_TAG {
  char value[AFE_HARDWARE_PN532_BLOCK_SIZE];
};

struct PN532_SECTOR {
  PN532_TAG block[AFE_HARDWARE_PN532_NUMBER_OF_BLOCKS_PER_SECTOR];
};

#endif
