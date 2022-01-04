/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */
#ifdef AFE_CONFIG_HARDWARE_TSL2561

/* ---------Headers ---------*/

void initializeTSL2561Sensor(void);
void TSL2561SensorEventsListener(void);

/* --------- Body -----------*/

void initializeTSL2561Sensor(void) {
  if (Device.configuration.noOfTSL2561s > 0) {
    for (uint8_t i = 0; i < Device.configuration.noOfTSL2561s; i++) {
#ifdef AFE_ESP32
      TSL2561Sensor[i].begin(i, &WirePort0, &WirePort1);
#else
      TSL2561Sensor[i].begin(i, &WirePort0);
#endif
    }
  }
}

/* Main code for processing sesnor */
void TSL2561SensorEventsListener(void) {
  if (Device.configuration.noOfTSL2561s > 0) {
    /* Sensor: listener */
    for (uint8_t i = 0; i < Device.configuration.noOfTSL2561s; i++) {
      if (TSL2561Sensor[i].listener()) {
        MqttAPI.publishTSL2561SensorData(i);
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
        HttpDomoticzAPI.publishTSL2561SensorData(i);
#endif
#ifdef AFE_CONFIG_HARDWARE_CLED_LIGHT_CONTROLLED_EFFECT
        if (Device.configuration.effectDeviceLight) {
          if (CLEDStrip.lightSensorType ==
              AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_SENSOR_TYPE_TSL2561) {
            CLEDStrip.backlightEffect(
                AFE_CONFIG_HARDWARE_CLED_LIGHT_CONTROLLED_EFFECT_ID,
                TSL2561Sensor[i].illuminance);
          }
        }
#endif // AFE_CONFIG_HARDWARE_CLED_LIGHT_CONTROLLED_EFFECT         
      }
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_TSL2561
