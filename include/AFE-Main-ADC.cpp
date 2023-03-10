/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT

/* ---------Headers ---------*/
void initializeADC(void);
void analogInputEventsListener(void);

/* --------- Body -----------*/

void initializeADC(void) {
  if (FirmwarePro->Configuration->Pro->valid) {
#ifdef AFE_ESP32
    for (uint8_t i = 0; i < Device->configuration.noOfAnalogInputs; i++) {
      AnalogInput[i].begin(i, FirmwarePro);
    }
#else
    if (FirmwarePro->Device->configuration.isAnalogInput) {
      AnalogInput.begin(FirmwarePro);
    }
#endif
  }
}

/* Main code for analog input listener */

#ifdef AFE_ESP32

/* Here is version for ESP32.. */
void analogInputEventsListener(void) {
  if (Device->configuration.noOfAnalogInputs > 0) {
    for (uint8_t i = 0; i < Device->configuration.noOfAnalogInputs; i++) {

      AnalogInput[i].listener();
      if (AnalogInput[i].isReady()) {
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

#else
/* Here is version for ESP82.. */
void analogInputEventsListener(void) {
  if (FirmwarePro->Device->configuration.isAnalogInput) {
    AnalogInput.listener();
    if (AnalogInput.isReady()) {
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

#endif // AFE_ESP32

#endif // AFE_CONFIG_HARDWARE_ANALOG_INPUT
