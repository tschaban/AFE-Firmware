#ifdef CONFIG_HARDWARE_ADC_VCC

/* Main code for analog input listener */
void analogInputListner() {
  {
    if (Device.configuration.isAnalogInput) {
      AnalogInput.listener();
      if (AnalogInput.isReady()) {
#ifdef CONFIG_HARDWARE_LED
        Led.on();
#endif
        ADCINPUT_DATA data = AnalogInput.get();
        MQTTPublishAnalogInputData(data);
        DomoticzPublishAnalogInputData(data);
#ifdef CONFIG_HARDWARE_LED
        Led.off();
#endif
      }
    }
  }
}
#endif
