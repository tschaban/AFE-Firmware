#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER_SENSOR

void ICACHE_RAM_ATTR newWindImpulse() {
  static uint32_t lastInterruptionTimeAnemometerSensor = 0;
  uint32_t interruptionTime = millis();
  if (interruptionTime - lastInterruptionTimeAnemometerSensor >
      WindImpulse.bouncing) {
    WindImpulse.newImpulse();
  }
  lastInterruptionTimeAnemometerSensor = interruptionTime;
}

void initializeAnemometerSensor(void) {
  if (Device.configuration.noOfAnemometerSensors > 0) {
    if (AnemometerSensor.begin(&Data, &WindImpulse)) {
      pinMode(AnemometerSensor.configuration.gpio, INPUT_PULLUP);
      attachInterrupt(
          digitalPinToInterrupt(AnemometerSensor.configuration.gpio),
          newWindImpulse, RISING);
#ifdef DEBUG
      Serial << endl
             << "INFO: Interrupt attached to GPIO: "
             << AnemometerSensor.configuration.gpio;
#endif
    }
#ifdef DEBUG
    else {
      Serial << endl
             << "ERROR: Interrupt NOT attached to GPIO: "
             << AnemometerSensor.configuration.gpio;
    }
#endif
  }
}

void windSensorListener(void) {
  if (Device.configuration.noOfAnemometerSensors > 0) {
    if (AnemometerSensor.listener()) {
      MqttAPI.publishAnemometerSensor();
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
      HttpDomoticzAPI.publishAnemometerSensor();
#endif
    }
  }
}

#endif