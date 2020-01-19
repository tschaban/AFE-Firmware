/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC

/* Main code for analog input listener */
void analogInputEventsListener() {
  if (Device.configuration.isAnalogInput) {
    AnalogInput.listener();
    if (AnalogInput.isReady()) {
      MqttAPI.publishADCValues();
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
      HttpDomoticzAPI.publishADCValues();
#endif
    }
  }
}
#endif
