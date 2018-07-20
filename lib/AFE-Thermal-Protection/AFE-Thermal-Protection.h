/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_Thermal_Protection_h
#define _AFE_Thermal_Protection_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class AFEThermalProtection {

private:
  int maxTemperature;
  boolean protection = false;

public:
  /* Constructors */
  AFEThermalProtection();

  /* Method initialize protection */
  void begin(int temperature);

  /* Method returns true if current temperature exceedes max allowed temperature */
  boolean protectionOn();

  /* Method monitors current temeprature and allowed one */
  void listener(float currentTemperature);
};

#endif
