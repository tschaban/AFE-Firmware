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
      if (Regulator[i].configuration.relayId != AFE_HARDWARE_ITEM_NOT_EXIST &&
          Device.configuration.noOfRelays >=
              Regulator[i].configuration.relayId + 1) {
        Regulator[i].addRelayReference(
            &Relay[Regulator[i].configuration.relayId]);
      }
    }
  }
}

#endif
