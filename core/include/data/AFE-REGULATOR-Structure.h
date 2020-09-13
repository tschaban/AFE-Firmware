/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_REGULATOR_Structure_h
#define _AFE_REGULATOR_Structure_h

#include <AFE-Configuration.h>

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
/*
enum SENSOR_HARDWARE {
#ifdef AFE_CONFIG_HARDWARE_DS18B20
  REGULATOR_SENSOR_DS18B20 = 0,
#endif
#ifdef AFE_CONFIG_HARDWARE_DHT
  REGULATOR_SENSOR_DHT = 1,
#endif
} regulator_sensor_hardware_t;

*/

struct REGULATOR {
  uint8_t relayId;
  uint8_t sensorId;
  uint8_t sensorHardware;
  boolean enabled;
  float turnOn;
  boolean turnOnAbove;
  float turnOff;
  boolean turnOffAbove;
};

#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR
#endif // _AFE_REGULATOR_Structure_h
