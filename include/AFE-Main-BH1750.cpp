/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */
#ifdef AFE_CONFIG_HARDWARE_BH1750

/* ---------Headers ---------*/
void BH1750SensorEventsListener(void);

/* --------- Body -----------*/


/* Main code for processing sesnor */
void BH1750SensorEventsListener(void) {
  if (Firmware->Device->configuration.noOfBH1750s > 0) {
    /* Sensor: listener */
    for (uint8_t i = 0; i < Firmware->Device->configuration.noOfBH1750s; i++) {
      Hardware->BH1750Sensor[i]->listener();
      if (Hardware->BH1750Sensor[i]->isReady()) {
        MqttAPI->publishBH1750SensorData(i);
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
        HttpDomoticzAPI->publishBH1750SensorData(i);
#endif

/* @TODO T5 
#ifdef AFE_CONFIG_HARDWARE_CLED_LIGHT_CONTROLLED_EFFECT
        if (Firmware->Device->configuration.effectFirmware->DeviceLight) {
          if (CLEDStrip->lightSensorType ==
              AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_SENSOR_TYPE_BH1750) {
            CLEDStrip->backlightEffect(
                AFE_CONFIG_HARDWARE_CLED_LIGHT_CONTROLLED_EFFECT_ID,
                Hardware->BH1750Sensor[i]->data);
          }
        }
#endif // AFE_CONFIG_HARDWARE_CLED_LIGHT_CONTROLLED_EFFECT
*/
      }
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_BH1750
