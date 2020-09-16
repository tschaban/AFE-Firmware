/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */
#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTION

/* ---------Headers ---------*/

void initializeThermalProtection(void);


/* --------- Body -----------*/

/* Initializing regulator */
void initializeThermalProtection(void) {
  if (Device.configuration.noOfThermalProtectors > 0) {
    for (uint8_t i = 0; i < Device.configuration.noOfThermalProtectors; i++) {
      ThermalProtector[i].begin(&Data, i);
    }
  }
}

#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTION
