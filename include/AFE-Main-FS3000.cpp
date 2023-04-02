/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */
#ifdef AFE_CONFIG_HARDWARE_FS3000
/* ---------Headers ---------*/
void FS3000SensorEventsListener(void);

/* --------- Body -----------*/

/* Main code for processing sesnor */
void FS3000SensorEventsListener(void) {
  if (Firmware->Device->configuration.noOfFS3000s > 0) {
    /* Sensor: listener */
    for (uint8_t i = 0; i < Firmware->Device->configuration.noOfFS3000s; i++) {
      if (Hardware->FS3000Sensor[i]->listener()) {

        MqttAPI->publishFS3000SensorData(i);
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
        HttpDomoticzAPI->publishFS3000SensorData(i);
#endif
      }
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_TSL2561
