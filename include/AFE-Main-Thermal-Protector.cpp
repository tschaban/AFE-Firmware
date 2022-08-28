/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */
#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR

/* ---------Headers ---------*/

void initializeThermalProtector(void);


/* --------- Body -----------*/

/* Initializing regulator */
void initializeThermalProtector(void) {
  if (Device->configuration.noOfThermalProtectors > 0) {
    for (uint8_t i = 0; i < Device->configuration.noOfThermalProtectors; i++) {
      ThermalProtector[i].begin(Data, i);
    }
  }
}

#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
