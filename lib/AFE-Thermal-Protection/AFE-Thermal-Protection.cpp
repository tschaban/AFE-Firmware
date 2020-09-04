/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Thermal-protection.h"

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTION

AFEThermalProtection::AFEThermalProtection(){};

void AFEThermalProtection::begin(float temperature) {
  maxTemperature = temperature;

#ifdef DEBUG
  Serial << endl
         << "INFO: Thermal protection enabled. Max temp: " << maxTemperature;
#endif
};

bool AFEThermalProtection::listener(float currentTemperature) {
  if (currentTemperature > maxTemperature && !protect) {
    protect = true;

#ifdef DEBUG
    Serial
        << endl
        << "INFO: Thermal protection. Temperature has exceeded the max value: "
        << maxTemperature;
#endif

  } else if (currentTemperature <= maxTemperature && protect) {
    protect = true;
  }

  return protect;
};

#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTION