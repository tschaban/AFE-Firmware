#ifdef AFE_CONFIG_HARDWARE_CLED

/* ---------Headers ---------*/

void initializeCLed(void);
void CLedEventsListener(void);

/* --------- Body -----------*/

void initializeCLed(void) {

  if (Device.configuration.effectDeviceLight ||
      (Device.configuration.effectPN532 &&
       Device.configuration.noOfPN532Sensors)) {
    CLEDStrip.begin(&Data);
  }
};

void CLedEventsListener(void) {
  if (Device.configuration.effectPN532 &&
      Device.configuration.noOfPN532Sensors > 0) {
    CLEDStrip.loop();
  }
};

#endif // AFE_CONFIG_HARDWARE_CLED
