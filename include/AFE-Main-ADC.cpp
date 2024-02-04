/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT

/* ---------Headers ---------*/

void analogInputEventsListener(void);

/* --------- Body -----------*/

/* Main code for analog input listener */

#ifdef AFE_ESP32

/* Here is version for ESP32.. */
void analogInputEventsListener(void) {

  if (Firmware->Configuration->Pro->valid) {
    if (Firmware->Device->configuration.noOfAnalogInputs > 0) {
      for (uint8_t i = 0; i < Firmware->Device->configuration.noOfAnalogInputs;
           i++) {

        Hardware->AnalogInput[i]->listener();
        if (Hardware->AnalogInput[i]->isReady()) {
          MqttAPI->publishADCValues(i);

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
          MqttAPI->publishBatteryMeterValues(i);
#endif

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
          HttpDomoticzAPI->publishADCValues(i);

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
          HttpDomoticzAPI->publishBatteryMeterValues(i);
#endif

#endif
        }
      }
    }
  }
}

#else
/* Here is version for ESP82.. */
void analogInputEventsListener(void) {
  if (Firmware->Configuration->Pro->valid) {
    if (Firmware->Device->configuration.isAnalogInput) {
      Hardware->AnalogInput->listener();
      if (Hardware->AnalogInput->isReady()) {
        MqttAPI->publishADCValues();

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
        MqttAPI->publishBatteryMeterValues();
#endif

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
        HttpDomoticzAPI->publishADCValues();

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
        HttpDomoticzAPI->publishBatteryMeterValues();
#endif

#endif
      }
    }
  }
}

#endif // AFE_ESP32

#endif // AFE_CONFIG_HARDWARE_ANALOG_INPUT
