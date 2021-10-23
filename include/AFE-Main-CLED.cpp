#ifdef AFE_CONFIG_HARDWARE_CLED

/* ---------Headers ---------*/

void initializeCLed(void);
void CLedEventsListener(void);

/* --------- Body -----------*/

void initializeCLed(void) {
  
  Device.configuration.noOfCLEDs = 1;

  if (Device.configuration.noOfCLEDs > 0) {
    CLEDStrip.begin(&Data);
  }
};

void CLedEventsListener(void) {
  if (Device.configuration.noOfCLEDs > 0) {
    CLEDStrip.loop();
  }
};

#endif // AFE_CONFIG_HARDWARE_CLED
