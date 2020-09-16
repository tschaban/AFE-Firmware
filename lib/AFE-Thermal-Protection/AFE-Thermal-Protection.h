/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Thermal_Protection_h
#define _AFE_Thermal_Protection_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTION

#include <AFE-Data-Access.h>

#ifdef DEBUG
#include <Streaming.h>
#endif


class AFEThermalProtection {

private:
  bool protect = false;

public:
  THERMAL_PROTECTION configuration;

  /* Constructors */
  AFEThermalProtection();

  /* Method initialize protection */
  void begin(AFEDataAccess *,uint8_t id);

  /* Method monitors current temeprature and allowed one */
  bool listener(float currentTemperature);
};

#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTION
#endif // _AFE_Thermal_Protection_h
