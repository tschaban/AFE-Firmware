/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_ADC_INPUT_Structure_h
#define _AFE_ADC_INPUT_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#ifdef AFE_CONFIG_API_MQTT_ENABLED
struct ADCINPUT_DOMOTICZ {
  uint32_t raw;
  uint32_t percent;
  uint32_t voltage;
  uint32_t voltageCalculated;
};
#endif

struct ADCINPUT_DATA {
  uint16_t raw;
  float percent;
  double voltage;
  double voltageCalculated;
};

struct VOLTAGE_DIVIDER {
  double Ra;
  double Rb;
};

struct ADCINPUT {
  uint8_t gpio;
  uint32_t interval;
  uint16_t numberOfSamples;
  double maxVCC;
#ifdef AFE_CONFIG_API_MQTT_ENABLED  
  MQTT_BASIC_CONFIG mqtt;
#endif
#ifdef AFE_CONFIG_API_MQTT_ENABLED
  ADCINPUT_DOMOTICZ domoticz;
#endif  
  VOLTAGE_DIVIDER divider;
};

#endif
