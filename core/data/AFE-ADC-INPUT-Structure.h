/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_ADC_INPUT_Structure_h
#define _AFE_ADC_INPUT_Structure_h

#include <AFE-Configuration.h>

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
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

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
struct BATTERY_METER {
  float maxVoltage;
  float minVoltage;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  DOMOTICZ_BASIC_CONFIG domoticz;
#else
  MQTT_TOPIC mqtt;
#endif
};
#endif // AFE_CONFIG_FUNCTIONALITY_BATTERYMETER

struct ADCINPUT {
  uint8_t gpio;
  uint32_t interval;
  uint16_t numberOfSamples;
  double maxVCC;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  ADCINPUT_DOMOTICZ domoticz;
#else
  MQTT_TOPIC mqtt;
#endif
  VOLTAGE_DIVIDER divider;
#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
  BATTERY_METER battery;
#endif

#ifdef AFE_ESP32
  char name[33];
#endif
};

#endif // AFE_CONFIG_HARDWARE_ANALOG_INPUT
#endif // _AFE_ADC_INPUT_Structure_h