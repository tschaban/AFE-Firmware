/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_DEVICE_Structure_h
#define _AFE_DEVICE_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Configuration.h>

struct AFE_DEVICE_API {
  boolean mqtt;
  boolean http;
  boolean domoticz;
};

/* Custom DEVICE structure definition */
struct DEVICE {
  char name[33];
  AFE_DEVICE_API api;
#if defined(T0_CONFIG)
  boolean isLED[AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_LEDS];
  boolean isSwitch[AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_SWITCHES];
  boolean isRelay[AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_RELAYS];
#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  boolean isAnalogInput;
#endif
#elif defined(T1_CONFIG)
  boolean isLED[2];
  boolean isRelay[1];
  boolean isSwitch[2];
  boolean isDS18B20;
#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  boolean isAnalogInput;
#endif
#elif defined(T2_CONFIG)
  boolean isLED[2];
  boolean isRelay[1];
  boolean isSwitch[2];
  boolean isDHT;
#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  boolean isAnalogInput;
#endif
#elif defined(T3_CONFIG)
  boolean isLED[5];
  boolean isRelay[4];
  boolean isSwitch[5];
  boolean isPIR[4];
#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  boolean isAnalogInput;
#endif
#elif defined(T4_CONFIG)
  boolean isLED[5];
  boolean isSwitch[5];
  boolean isRelay[4];
#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  boolean isAnalogInput;
#endif
#elif defined(T5_CONFIG)
  uint8_t noOfGates;
  uint8_t noOfContactrons;
  uint8_t noOfSwitches;
  uint8_t noOfRelays;
  uint8_t noOfLEDs;
#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  boolean isAnalogInput;
#endif
#elif defined(T6_CONFIG)
  uint8_t noOfSwitches;
  uint8_t noOfRelays;
  uint8_t noOfLEDs;
  uint8_t noOfHPMA115S0s;
  uint8_t noOfBMx80s;
  uint8_t noOfBH1750s;
#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  boolean isAnalogInput;
#endif
#endif
};

#endif
