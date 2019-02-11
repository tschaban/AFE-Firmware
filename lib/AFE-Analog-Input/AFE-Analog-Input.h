/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_Sensor_ADC_VCC_h
#define _AFE_Sensor_ADC_VCC_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>

class AFEAnalogInput {

private:
  ADCINPUT configuration;
  boolean ready = false;
  unsigned long startTime = 0;
  boolean _initialized = false;
  uint16_t analogData = 0;
  uint16_t temporaryAnalogData = 0;
  uint8_t counterOfSamplings = 0;

public:
  AFEAnalogInput();

  void begin();

  ADCINPUT_DATA get();

  boolean isReady();

  void listener();

  void getDomoticzIDX(ADCINPUT_DOMOTICZ *idx);
};

#endif
