/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_DEVICE_Structure_h
#define _AFE_DEVICE_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Configuration.h>

struct DEVICE_API {
  boolean mqtt;
  boolean http;
  boolean domoticz;
};

/* Custom DEVICE structure definition */
struct DEVICE {
  char name[33];
  DEVICE_API api;
#if defined(T0_CONFIG)
  boolean isLED[CONFIG_HARDWARE_MAX_NUMBER_OF_LEDS];
  boolean isSwitch[CONFIG_HARDWARE_MAX_NUMBER_OF_SWITCHES];
  boolean isRelay[CONFIG_HARDWARE_MAX_NUMBER_OF_RELAYS];
#ifdef CONFIG_HARDWARE_ADC_VCC
  boolean isAnalogInput;
#endif
#elif defined(T1_CONFIG)
  boolean isLED[2];
  boolean isRelay[1];
  boolean isSwitch[2];
  boolean isDS18B20;
#ifdef CONFIG_HARDWARE_ADC_VCC
  boolean isAnalogInput;
#endif
#elif defined(T2_CONFIG)
  boolean isLED[2];
  boolean isRelay[1];
  boolean isSwitch[2];
  boolean isDHT;
#ifdef CONFIG_HARDWARE_ADC_VCC
  boolean isAnalogInput;
#endif
#elif defined(T3_CONFIG)
  boolean isLED[5];
  boolean isRelay[4];
  boolean isSwitch[5];
  boolean isPIR[4];
#ifdef CONFIG_HARDWARE_ADC_VCC
  boolean isAnalogInput;
#endif
#elif defined(T4_CONFIG)
  boolean isLED[5];
  boolean isSwitch[5];
  boolean isRelay[4];
#ifdef CONFIG_HARDWARE_ADC_VCC
  boolean isAnalogInput;
#endif
#elif defined(T5_CONFIG)
  boolean isLED[CONFIG_HARDWARE_MAX_NUMBER_OF_LEDS];
  boolean isSwitch[CONFIG_HARDWARE_MAX_NUMBER_OF_SWITCHES];
  boolean isRelay[CONFIG_HARDWARE_MAX_NUMBER_OF_RELAYS];
  boolean isContactron[CONFIG_HARDWARE_MAX_NUMBER_OF_CONTACTRONS];
  boolean isDHT;
#ifdef CONFIG_HARDWARE_ADC_VCC
  boolean isAnalogInput;
#endif
#elif defined(T6_CONFIG)
  boolean isLED[2];
  boolean isSwitch[2];
  boolean isRelay[1];
  boolean isHPMA115S0;
  uint8_t isBMx80;
  boolean isBH1750;
#ifdef CONFIG_HARDWARE_ADC_VCC
  boolean isAnalogInput;
#endif
#endif
};

#endif
