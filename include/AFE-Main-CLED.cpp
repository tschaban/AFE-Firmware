#ifdef AFE_CONFIG_HARDWARE_CLED

/* ---------Headers ---------*/

void initializeCLed(void);
void CLedEventsListener(void);

/* --------- Body -----------*/

void initializeCLed(void) {

#ifdef AFE_CONFIG_HARDWARE_CLED_DEVICE_LIGHT_EFFECT
  if (Device.configuration.effectDeviceLight) {
    CLedDeviceLight.begin(&Data,
                          AFE_CONFIG_HARDWARE_CLED_DEVICE_LIGHT_EFFECT_ID);
    CLedDeviceLight.effectOn(AFE_CONFIG_HARDWARE_EFFECT_FADE_IN_OUT);
  }
#endif // AFE_CONFIG_HARDWARE_CLED_DEVICE_LIGHT_EFFECT

#ifdef AFE_CONFIG_HARDWARE_CLED_PN532_SENSOR_EFFECT
  if (Device.configuration.noOfPN532Sensors > 0 &&
      Device.configuration.effectPN532) {
    CLedPN532Effect.begin(&Data,
                          AFE_CONFIG_HARDWARE_CLED_PN532_SENSOR_EFFECT_ID);
    CLedPN532Effect.effectOn(AFE_CONFIG_HARDWARE_EFFECT_FADE_IN_OUT);
  }
#endif // AFE_CONFIG_HARDWARE_CLED_PN532_SENSOR_EFFECT
};

void CLedEventsListener(void) {

#ifdef AFE_CONFIG_HARDWARE_CLED_DEVICE_LIGHT_EFFECT
  if (Device.configuration.effectDeviceLight) {
    CLedDeviceLight.loop();
  }
#endif // AFE_CONFIG_HARDWARE_CLED_DEVICE_LIGHT_EFFECT

#ifdef AFE_CONFIG_HARDWARE_CLED_PN532_SENSOR_EFFECT
  if (Device.configuration.effectPN532 &&
      Device.configuration.noOfPN532Sensors > 0) {
    CLedPN532Effect.loop();
  }
#endif // AFE_CONFIG_HARDWARE_CLED_PN532_SENSOR_EFFECT
};

#endif // AFE_CONFIG_HARDWARE_CLED
