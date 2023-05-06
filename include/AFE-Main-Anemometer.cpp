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
      Hardware->WindImpulse->bouncing) {
    Hardware->WindImpulse->newImpulse();
  }
  lastInterruptionTimeAnemometerSensor = interruptionTime;
}

void initializeAnemometer(void) {
  if (Firmware->Device->configuration.noOfAnemometerSensors > 0) {
    if (Hardware->AnemometerSensor->begin(Firmware->API->Flash,
                                          Hardware->WindImpulse)) {
      pinMode(Hardware->AnemometerSensor->configuration->gpio, INPUT_PULLUP);
      attachInterrupt(digitalPinToInterrupt(
                          Hardware->AnemometerSensor->configuration->gpio),
                      newWindImpulse, RISING);
#ifdef DEBUG
      Serial << endl
             << F("INFO: ANEMOMETER: Interrupt attached to GPIO: ")
             << Hardware->AnemometerSensor->configuration->gpio;
#endif
    }
#ifdef DEBUG
    else {
      Serial << endl
             << F("ERROR: ANEMOMETER: Interrupt NOT attached to GPIO: ")
             << Hardware->AnemometerSensor->configuration->gpio;
    }
#endif
  }
}

void anemometerEventsListener(void) {
  if (Firmware->Device->configuration.noOfAnemometerSensors > 0) {
    if (Hardware->AnemometerSensor->listener()) {
      MqttAPI->publishAnemometerSensorData();
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
      HttpDomoticzAPI->publishAnemometerSensorData();
#endif
    }
  }
}

#endif