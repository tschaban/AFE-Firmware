/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_RAINMETER_SENSOR_Structure_h
#define _AFE_RAINMETER_SENSOR_Structure_h


struct RAINMETER {
  uint8_t gpio;
  char name[17];
  uint8_t sensitiveness;
  uint32_t interval;
  float resolution; // ml/m2
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  DOMOTICZ_BASIC_CONFIG domoticz;
#else
  MQTT_TOPIC mqtt;
#endif
};

struct RAINMETER_DATA {
  float last1h[60];
  uint8_t index1h;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  float counter;
#else
  float last12h[12];
  uint8_t index12h;
  float last24h[2];
  uint8_t index24h;
#endif
};

#endif
