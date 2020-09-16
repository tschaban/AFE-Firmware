/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_DEVICE_Structure_h
#define _AFE_DEVICE_Structure_h

#include <AFE-Configuration.h>

struct AFE_DEVICE_API {
  boolean http;
  boolean mqtt;
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  boolean domoticz;
  uint8_t domoticzVersion;
#endif
};

struct DEVICE {
  char name[33];
  AFE_DEVICE_API api;
  uint8_t timeToAutoLogOff;
#ifdef AFE_CONFIG_HARDWARE_SWITCH
  uint8_t noOfSwitches;
#endif
#ifdef AFE_CONFIG_HARDWARE_RELAY
  uint8_t noOfRelays;
#endif
#ifdef AFE_CONFIG_HARDWARE_LED
  uint8_t noOfLEDs;
#endif
#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  boolean isAnalogInput;
#endif
#ifdef AFE_CONFIG_HARDWARE_GATE
  uint8_t noOfGates;
#endif
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  uint8_t noOfContactrons;
#endif
#ifdef AFE_CONFIG_HARDWARE_BMEX80
  uint8_t noOfBMEX80s;
#endif
#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  uint8_t noOfHPMA115S0s;
#endif
#ifdef AFE_CONFIG_HARDWARE_BH1750
  uint8_t noOfBH1750s;
#endif
#ifdef AFE_CONFIG_HARDWARE_AS3935
  uint8_t noOfAS3935s;
#endif
#ifdef AFE_CONFIG_HARDWARE_DS18B20
  uint8_t noOfDS18B20s;
#endif
#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER_SENSOR
  uint8_t noOfAnemometerSensors;
#endif
#ifdef AFE_CONFIG_HARDWARE_RAINMETER_SENSOR
  uint8_t noOfRainmeterSensors;
#endif
#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  uint8_t noOfRegulators;
#endif
#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTION
  uint8_t noOfThermalProtectors;
#endif
};

/* Custom DEVICE structure definition
struct DEVICE {
  char name[33];
  AFE_DEVICE_API api;
#if defined(T0_CONFIG)
  uint8_t noOfSwitches;
  uint8_t noOfRelays;
  uint8_t noOfLEDs;
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
#ifdef AFE_CONFIG_HARDWARE_BMEX80
  uint8_t noOfBMEX80s;
#endif
#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  boolean isAnalogInput;
#endif
#elif defined(T6_CONFIG)
  uint8_t noOfSwitches;
  uint8_t noOfRelays;
  uint8_t noOfLEDs;
  uint8_t noOfHPMA115S0s;
  uint8_t noOfBMEX80s;
  uint8_t noOfBH1750s;
  uint8_t noOfAS3935s;
#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  boolean isAnalogInput;
#endif
#endif
};
*/

#ifdef T0_CONFIG
/* Old structure of DEVICE required for upgrade from T0 2.0.0, 2.0.1, 2.0.2 */

struct AFE_DEVICE_API_T0_200 {
  boolean http;
  boolean mqtt;
  boolean domoticz;
};

struct DEVICE_T0_200 {
  char name[33];
  AFE_DEVICE_API_T0_200 api;
#ifdef AFE_CONFIG_HARDWARE_LED
  boolean isLED[AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_LEDS];
#endif
  boolean isSwitch[AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_SWITCHES];
  boolean isRelay[AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_RELAYS];
#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  boolean isAnalogInput;
#endif
};
#endif

#endif
