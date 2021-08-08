#ifdef AFE_CONFIG_HARDWARE_CLED

/* ---------Headers ---------*/

void initializeCLed(void);
void CLedEventsListener(void);

/* --------- Body -----------*/

void initializeCLed(void) {

#ifdef AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_EFFECT
  if (Device.configuration.effectDeviceLight) {
    CLEDBacklight.begin(&Data,
                          AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_EFFECT_ID);
  }
#endif // AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_EFFECT

#ifdef AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT
  if (Device.configuration.noOfPN532Sensors > 0 &&
      Device.configuration.effectPN532) {
    CLEDAccessControl.begin(&Data,
                          AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT_ID);
    CLEDAccessControl.effectOn(AFE_CONFIG_HARDWARE_EFFECT_FADE_IN_OUT);
  }
#endif // AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT
};

void CLedEventsListener(void) {

#ifdef AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_EFFECT
  if (Device.configuration.effectDeviceLight) {
    CLEDBacklight.loop();
  }
#endif // AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_EFFECT

#ifdef AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT
  if (Device.configuration.effectPN532 &&
      Device.configuration.noOfPN532Sensors > 0) {
    CLEDAccessControl.loop();
  }
#endif // AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT
};

#endif // AFE_CONFIG_HARDWARE_CLED
