/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_THERMAL_PROTECTION_Structure_h
#define _AFE_THERMAL_PROTECTION_Structure_h

#include <AFE-Configuration.h>

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTION
struct THERMAL_PROTECTION {
  char name[17];
  uint8_t relayId;
  uint8_t sensorId;
  float temperature;
  boolean enabled;
};

#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTION
#endif // _AFE_THERMAL_PROTECTION_Structure_h
