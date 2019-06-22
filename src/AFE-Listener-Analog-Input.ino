#ifdef CONFIG_HARDWARE_ADC_VCC

/* Main code for analog input listener */
void analogInputListner() {
  {
    if (Device.configuration.isAnalogInput) {
      AnalogInput.listener();
      if (AnalogInput.isReady()) {
#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
        Led.on();
#endif
        ADCINPUT_DATA data = AnalogInput.get();
        MQTTPublishAnalogInputData(data);
        DomoticzPublishAnalogInputData(data);
#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
        Led.off();
#endif
      }
    }
  }
}
#endif
