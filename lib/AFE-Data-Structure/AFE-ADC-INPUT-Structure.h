/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_ADC_INPUT_Structure_h
#define _AFE_ADC_INPUT_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

struct ADCINPUT_DOMOTICZ {
  uint32_t raw;
  uint32_t percent;
  uint32_t voltage;
};

struct ADCINPUT_DATA {
  uint16_t raw;
  float percent;
  double voltage;
};

struct ADCINPUT {
  uint8_t gpio;
  uint32_t interval;
  uint16_t numberOfSamples;
  float maxVCC;
  MQTT_BASIC_CONFIG mqtt;
  ADCINPUT_DOMOTICZ domoticz;
};

#endif
