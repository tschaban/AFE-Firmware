#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER

/* ---------Headers ---------*/

void ICACHE_RAM_ATTR newWindImpulse(void);
void initializeAnemometer(void);
void anemometerEventsListener(void);

/* --------- Body -----------*/


void ICACHE_RAM_ATTR newWindImpulse() {
  static uint32_t lastInterruptionTimeAnemometerSensor = 0;
  uint32_t interruptionTime = millis();
  if (interruptionTime - lastInterruptionTimeAnemometerSensor >
      WindImpulse.bouncing) {
    WindImpulse.newImpulse();
  }
  lastInterruptionTimeAnemometerSensor = interruptionTime;
}

void initializeAnemometer(void) {
  if (Device.configuration.noOfAnemometerSensors > 0) {
    if (AnemometerSensor.begin(&Data, &WindImpulse)) {
      pinMode(AnemometerSensor.configuration.gpio, INPUT_PULLUP);
      attachInterrupt(
          digitalPinToInterrupt(AnemometerSensor.configuration.gpio),
          newWindImpulse, RISING);
#ifdef DEBUG
      Serial << endl
             << F("INFO: ANEMOMETER: Interrupt attached to GPIO: ")
             << AnemometerSensor.configuration.gpio;
#endif
    }
#ifdef DEBUG
    else {
      Serial << endl
             << F("ERROR: ANEMOMETER: Interrupt NOT attached to GPIO: ")
             << AnemometerSensor.configuration.gpio;
    }
#endif
  }
}

void anemometerEventsListener(void) {
  if (Device.configuration.noOfAnemometerSensors > 0) {
    if (AnemometerSensor.listener()) {
      MqttAPI.publishAnemometerSensorData();
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
      HttpDomoticzAPI.publishAnemometerSensorData();
#endif
    }
  }
}

#endif