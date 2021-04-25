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


#endif
