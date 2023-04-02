/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */
#ifdef AFE_CONFIG_HARDWARE_TSL2561

/* ---------Headers ---------*/
void TSL2561SensorEventsListener(void);

/* --------- Body -----------*/

/* Main code for processing sesnor */
void TSL2561SensorEventsListener(void) {
  if (Firmware->Device->configuration.noOfTSL2561s > 0) {
    /* Sensor: listener */
    for (uint8_t i = 0; i < Firmware->Device->configuration.noOfTSL2561s; i++) {
      if (Hardware->TSL2561Sensor[i]->listener()) {
        MqttAPI->publishTSL2561SensorData(i);
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
        HttpDomoticzAPI->publishTSL2561SensorData(i);
#endif

/* @TODO T5 
#ifdef AFE_CONFIG_HARDWARE_CLED_LIGHT_CONTROLLED_EFFECT
        if (Firmware->Device->configuration.effectFirmware->DeviceLight) {
          if (CLEDStrip->lightSensorType ==
              AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_SENSOR_TYPE_TSL2561) {
            CLEDStrip->backlightEffect(
                AFE_CONFIG_HARDWARE_CLED_LIGHT_CONTROLLED_EFFECT_ID,
                Hardware->TSL2561Sensor[i]->illuminance);
          }
        }
#endif // AFE_CONFIG_HARDWARE_CLED_LIGHT_CONTROLLED_EFFECT         
*/
      }
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_TSL2561
