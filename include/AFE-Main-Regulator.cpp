/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */
#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR

/* ---------Headers ---------*/

void initializeRegulator(void);


/* --------- Body -----------*/

/* Initializing regulator */
void initializeRegulator(void) {
  if (Device.configuration.noOfRegulators > 0) {
    for (uint8_t i = 0; i < Device.configuration.noOfRegulators; i++) {
      Regulator[i].begin(&Data, i);
    }
  }
}

#endif
