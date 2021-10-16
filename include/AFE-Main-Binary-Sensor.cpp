/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR

/* ---------Headers ---------*/

void initializeBinarySensor(void);
void binarySensorEventsListener(void);

/* --------- Body -----------*/

/* Initializing Binary Sensor */
void initializeBinarySensor(void) {
  for (uint8_t i = 0; i < Device.configuration.noOfBinarySensors; i++) {

#ifdef AFE_CONFIG_HARDWARE_MCP23017
    BinarySensor[i].addMCP23017Reference(&MCP23017Broker);
#endif // AFE_CONFIG_HARDWARE_MCP23017
    BinarySensor[i].begin(i, &Data);
  }
#ifdef DEBUG
  Serial << endl << F("INFO: BOOT: Binary sensors initialized");
#endif
}

void binarySensorEventsListener(void) {
  /* Listens for sensor events */
  for (uint8_t i = 0; i < Device.configuration.noOfBinarySensors; i++) {
    if (BinarySensor[i].listener()) {
      MqttAPI.publishBinarySensorState(i);
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
      HttpDomoticzAPI.publishBinarySensorState(i);
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

#ifdef AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT
      /* Changing the CLED Effect card detected, but not authorized yet */
      if (Device.configuration.effectPN532) {
        if (CLEDStrip._currentEffect ==
            AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT) {
          if (BinarySensor[i].get()) {
            CLEDStrip.setCustomEffectColor(
                AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT_ID,
                AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT,
                CLEDStrip.effects.effect[AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT]
                    .color);

          } else {
            CLEDStrip.setCustomEffectColor(
                AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT_ID,
                AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT,
                AFE_CONFIG_HARDWARE_CLED_EFFECT_WAVE_DEFAULT_COLOR);
          }
          CLEDStrip.effectOn(AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT_ID,
                             AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT);
        }
      }
#endif // AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT
    }
  }
}

#endif // AFE_CONFIG_HARDWARE_BINARY_SENSOR