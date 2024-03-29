/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0

/* ---------Headers ---------*/

void HPMA115S0SensorEventsListener(void);

/* --------- Body -----------*/

/* Main code for processing sesnor */
void HPMA115S0SensorEventsListener(void) {
  if (Firmware->Device->configuration.noOfHPMA115S0s > 0) {
    for (uint8_t i = 0; i < Firmware->Device->configuration.noOfHPMA115S0s; i++) {
      if (Hardware->HPMA115S0Sensor[i]->listener()) {
        MqttAPI->publishHPMA115S0SensorData(i);
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
        HttpDomoticzAPI->publishHPMA115S0SensorData(i);
#endif
      }
    }
  }
}
#endif
