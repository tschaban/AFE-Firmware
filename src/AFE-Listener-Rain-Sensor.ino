#ifdef AFE_CONFIG_HARDWARE_RAINMETER_SENSOR

void ICACHE_RAM_ATTR newRainImpulse() {
  static uint32_t lastInterruptionTimeRainSensor = 0;
  uint32_t interruptionTime = millis();
  if (interruptionTime - lastInterruptionTimeRainSensor >
      RainImpulse.bouncing) {
    RainImpulse.newImpulse();
  }
  lastInterruptionTimeRainSensor = interruptionTime;
}

void initializeRainSensor(void) {
  if (Device.configuration.noOfRainmeterSensors > 0) {
    if (RainSensor.begin(&Data, &RainImpulse)) {
      pinMode(RainSensor.configuration.gpio, INPUT_PULLUP);
      attachInterrupt(digitalPinToInterrupt(RainSensor.configuration.gpio),
                      newRainImpulse, RISING);
#ifdef DEBUG
      Serial << endl
             << F("INFO: Interrupt attached to GPIO: ")
             << RainSensor.configuration.gpio;
#endif
    }
#ifdef DEBUG
    else {
      Serial << endl
             << F("ERROR: Interrupt NOT attached to GPIO: ")
             << RainSensor.configuration.gpio;
    }
#endif
  }
}

void rainSensorListener(void) {
  if (Device.configuration.noOfRainmeterSensors > 0) {
    if (RainSensor.listener()) {
      MqttAPI.publishRainSensorData();
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
      HttpDomoticzAPI.publishRainSensorData();
#endif
    }
  }
}

#endif