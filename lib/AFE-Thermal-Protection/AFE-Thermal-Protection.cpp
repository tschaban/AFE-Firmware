/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#include "AFE-Thermal-protection.h"

AFEThermalProtection::AFEThermalProtection() {};

void AFEThermalProtection::begin(int temperature) {
  maxTemperature = temperature;
};

boolean AFEThermalProtection::protectionOn() {
  return protection;
};

void AFEThermalProtection::listener(float currentTemperature) {
   if (maxTemperature!=0) {
   if (currentTemperature > maxTemperature && !protection) {
     protection = true;
   } else if (currentTemperature <= maxTemperature && protection) {
     protection = true;
   }
   }
};
