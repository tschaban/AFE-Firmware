/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR

/* ---------Headers ---------*/

void binarySensorEventsListener(void);

/* --------- Body -----------*/

void binarySensorEventsListener(void) {
  /* Listens for sensor events */
  for (uint8_t i = 0; i < Firmware->Device->configuration.noOfBinarySensors; i++) {
    if (Hardware->BinarySensor[i]->listener()) {
      MqttAPI->publishBinarySensorState(i);
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
      HttpDomoticzAPI->publishBinarySensorState(i);
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

/* @TODO T5 
#ifdef AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT
      // Changing the CLED Effect card detected, but not authorized yet 
      if (Firmware->Device->configuration.effectPN532) {
        if (CLEDStrip->_currentEffect ==
            AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT) {
          if (Hardware->BinarySensor[i]->get()) {
            CLEDStrip->setCustomEffectColor(
                AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT_ID,
                AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT,
                CLEDStrip->effects.effect[AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT]
                    .color);

          } else {
            CLEDStrip->setCustomEffectColor(
                AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT_ID,
                AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT,
                AFE_CONFIG_HARDWARE_CLED_EFFECT_WAVE_DEFAULT_COLOR);
          }
          CLEDStrip->effectOn(AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT_ID,
                             AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT);
        }
      }
#endif // AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT
*/
    }
  }
}

#endif // AFE_CONFIG_HARDWARE_BINARY_SENSOR