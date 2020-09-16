/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Thermal-protection.h"

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTION

AFEThermalProtection::AFEThermalProtection(){};

void AFEThermalProtection::begin(AFEDataAccess *_Data, uint8_t id) {

  _Data->getConfiguration(id, &configuration);

#ifdef DEBUG
  Serial << endl
         << "INFO: Thermal protection enabled. Max temp: "
         << configuration.temperature;
#endif
};

bool AFEThermalProtection::listener(float currentTemperature) {
  if (currentTemperature > configuration.temperature && !protect) {
    protect = true;

#ifdef DEBUG
    Serial
        << endl
        << "INFO: Thermal protection. Temperature has exceeded the max value: "
        << configuration.temperature;
#endif

  } else if (currentTemperature <= configuration.temperature && protect) {
    protect = true;
  }

  return protect;
};

#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTION