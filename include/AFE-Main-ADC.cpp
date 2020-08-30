/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC



/* ---------Headers ---------*/

void analogInputEventsListener(void);

/* --------- Body -----------*/

/* Main code for analog input listener */
void analogInputEventsListener(void) {
  if (Device.configuration.isAnalogInput) {
    AnalogInput.listener();
    if (AnalogInput.isReady()) {
      MqttAPI.publishADCValues();

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
      MqttAPI.publishBatteryMeterValues();
#endif

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
      HttpDomoticzAPI.publishADCValues();

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
      HttpDomoticzAPI.publishBatteryMeterValues();
#endif

#endif
    }
  }
}
#endif
