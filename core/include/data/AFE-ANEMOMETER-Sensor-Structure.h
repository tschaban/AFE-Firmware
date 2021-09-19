/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_ANEMOMETER_SENSOR_Structure_h
#define _AFE_ANEMOMETER_SENSOR_Structure_h


struct ANEMOMETER {
  uint8_t gpio;
  char name[17];
  uint8_t sensitiveness;
  uint32_t interval;
  float impulseDistance;
  uint8_t impulseDistanceUnit;
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  DOMOTICZ_BASIC_CONFIG domoticz;
#else
  MQTT_BASIC_CONFIG mqtt;
#endif
};

#endif
