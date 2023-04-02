/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */
#ifdef AFE_CONFIG_HARDWARE_BMEX80

/* ---------Headers ---------*/
void BMX80SensorEventsListener(void);

/* --------- Body -----------*/

void BMX80SensorEventsListener(void) {
  if (Firmware->Device->configuration.noOfBMEX80s > 0) {
    for (uint8_t i = 0; i < Firmware->Device->configuration.noOfBMEX80s; i++) {
      Hardware->BMEX80Sensor[i]->listener();
      if (Hardware->BMEX80Sensor[i]->isReady()) {
        MqttAPI->publishBoschBMSensorData(i);
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
        HttpDomoticzAPI->publishBoschBMSensorData(i);
#endif
      }
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_BMEX80
