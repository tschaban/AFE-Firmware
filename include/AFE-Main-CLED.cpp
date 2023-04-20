#ifdef AFE_CONFIG_HARDWARE_CLED

/* ---------Headers ---------*/

void RGBLEDEventsListener(void);

/* --------- Body -----------*/

void RGBLEDEventsListener(void) {
  if (Firmware->Device->configuration.noOfCLEDs > 0) {
    Hardware->RGBLEDStrip->loop();
  }
};

#endif // AFE_CONFIG_HARDWARE_CLED
