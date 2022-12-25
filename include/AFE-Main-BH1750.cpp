/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */
#ifdef AFE_CONFIG_HARDWARE_BH1750

/* ---------Headers ---------*/

void initializeBH1750Sensor(void);
void BH1750SensorEventsListener(void);

/* --------- Body -----------*/

void initializeBH1750Sensor(void) {
  if (Device->configuration.noOfBH1750s > 0) {
    for (uint8_t i = 0; i < Device->configuration.noOfBH1750s; i++) {
#ifdef AFE_ESP32
      BH1750Sensor[i].begin(i, &WirePort0, &WirePort1);
#else
      BH1750Sensor[i].begin(i, &WirePort0);
#endif
    }
  }
}

/* Main code for processing sesnor */
void BH1750SensorEventsListener(void) {
  if (Device->configuration.noOfBH1750s > 0) {
    /* Sensor: listener */
    for (uint8_t i = 0; i < Device->configuration.noOfBH1750s; i++) {
      BH1750Sensor[i].listener();
      if (BH1750Sensor[i].isReady()) {
        MqttAPI->publishBH1750SensorData(i);
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
        HttpDomoticzAPI->publishBH1750SensorData(i);
#endif

#ifdef AFE_CONFIG_HARDWARE_CLED_LIGHT_CONTROLLED_EFFECT
        if (Device->configuration.effectDeviceLight) {
          if (CLEDStrip->lightSensorType ==
              AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_SENSOR_TYPE_BH1750) {
            CLEDStrip->backlightEffect(
                AFE_CONFIG_HARDWARE_CLED_LIGHT_CONTROLLED_EFFECT_ID,
                BH1750Sensor[i].data);
          }
        }
#endif // AFE_CONFIG_HARDWARE_CLED_LIGHT_CONTROLLED_EFFECT
      }
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_BH1750
