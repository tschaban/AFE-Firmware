/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

  

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
