#ifdef AFE_CONFIG_HARDWARE_CLED

/* ---------Headers ---------*/

void initializeCLed(void);
void CLedEventsListener(void);

/* --------- Body -----------*/

void initializeCLed(void) {
  for (uint8_t i = 0; i < Device.configuration.noOfCLEDs; i++) {
    CLed[i].begin(&Data, i);
    if (i == AFE_CONFIG_HARDWARE_CLED_ID_PN532_SENSOR) {
      CLed[i].effectOn(AFE_CONFIG_HARDWARE_EFFECT_FADE_IN_OUT);
    }
  }
};

void CLedEventsListener(void) {
  for (uint8_t i = 0; i < Device.configuration.noOfCLEDs; i++) {
    CLed[i].loop();
  }
};

#endif // AFE_CONFIG_HARDWARE_CLED
