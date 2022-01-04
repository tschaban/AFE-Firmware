/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */
#ifdef AFE_CONFIG_HARDWARE_AS3935

/* Warning: for now it's hardcoded for one sensor only */

boolean AS3935Intrrupted = false;

/* Attaches the interaption hander on the GPIO to which Sensor IRQ is connected
 */
void ICACHE_RAM_ATTR handleAS3935Interraption() {

#ifdef DEBUG
  Serial << endl << F("INFO: AS3935, IRQ triggered");
#endif
  AS3935Intrrupted = true;
}

/* Initializing the sensor */
void initializeAS3935Sensor() {
  if (Device.configuration.noOfAS3935s > 0) {
    if (AS3935Sensor[0].begin(0)) {
#ifdef DEBUG
      Serial << endl
             << F("INFO: AS3935, Attaching interupt on GPIO")
             << AS3935Sensor[0].configuration.irqGPIO;
#endif

      attachInterrupt(AS3935Sensor[0].configuration.irqGPIO,
                      handleAS3935Interraption, RISING);
    }
  }
}

/* Main code for processing sensor */
void AS3935SensorEventsListener() {
  if (Device.configuration.noOfAS3935s > 0) {

    // AS3935Sensor[0].interruptionReported();

    if (AS3935Intrrupted) {
#ifdef DEBUG
      Serial << endl << F("INFO: AS3935, Processing event");
#endif
      AS3935Sensor[0].interruptionReported();
      AS3935Intrrupted = false;
    }
  }

  if (AS3935Sensor[0].strikeDetected()) {
    MqttAPI.publishAS3935SensorData(0);
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    HttpDomoticzAPI.publishAS3935SensorData(0);
#endif
  }
}
#endif
