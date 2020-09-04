/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Thermal_Protection_h
#define _AFE_Thermal_Protection_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTION

#ifdef DEBUG
#include <Streaming.h>
#endif


class AFEThermalProtection {

private:
  float maxTemperature;
  bool protect = false;

public:
  /* Constructors */
  AFEThermalProtection();

  /* Method initialize protection */
  void begin(float temperature);

  /* Method monitors current temeprature and allowed one */
  bool listener(float currentTemperature);
};

#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTION
#endif // _AFE_Thermal_Protection_h
