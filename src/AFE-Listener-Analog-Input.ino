#ifdef CONFIG_HARDWARE_ADC_VCC

/* Main code for processing sesnor */
void analogInputListner() {
  {
    if (Device.configuration.isAnalogInput) {
      /* Sensor: listener */
      AnalogInput.listener();
      if (AnalogInput.isReady()) {
#ifdef CONFIG_HARDWARE_LED
        Led.on();
#endif
        // MQTTPublishLightLevel(sensorData);
        DomoticzPublishAnalogInputData(AnalogInput.get());
#ifdef CONFIG_HARDWARE_LED
        Led.off();
#endif
      }
    }
  }
}
#endif
