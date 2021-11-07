#ifndef _AFE_DEVICE_Structure_h
#define _AFE_DEVICE_Structure_h

#include <AFE-Configuration.h>
#include <arduino.h>

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
#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
#ifndef AFE_ESP32
  boolean isAnalogInput;
#else
  uint8_t noOfAnalogInputs;
#endif // AFE_ESP32
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
#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
  uint8_t noOfAnemometerSensors;
#endif
#ifdef AFE_CONFIG_HARDWARE_RAINMETER
  uint8_t noOfRainmeterSensors;
#endif
#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  uint8_t noOfRegulators;
#endif
#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
  uint8_t noOfThermalProtectors;
#endif
#ifdef AFE_CONFIG_HARDWARE_DHT
  uint8_t noOfDHTs;
#endif
#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
  uint8_t noOfBinarySensors;
#endif
#ifdef AFE_CONFIG_HARDWARE_TSL2561
  uint8_t noOfTSL2561s;
#endif
#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
  uint8_t noOfPN532Sensors;
  uint8_t noOfMiFareCards;
#ifdef AFE_CONFIG_HARDWARE_CLED
  boolean effectPN532;
#endif
#endif // AFE_CONFIG_HARDWARE_PN532_SENSOR

#ifdef AFE_CONFIG_HARDWARE_CLED
  uint8_t noOfCLEDs;
  boolean effectDeviceLight;
#endif


#ifdef AFE_CONFIG_HARDWARE_I2C
  uint8_t noOfI2Cs;
#endif
};

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
#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
  boolean isAnalogInput;
#endif
};
#endif

#endif
