#ifdef AFE_CONFIG_HARDWARE_WIND_SENSOR

void ICACHE_RAM_ATTR newWindImpulse() {
  static uint32_t lastInterruptionTimeWindSensor = 0;
  uint32_t interruptionTime = millis();
  if (interruptionTime - lastInterruptionTimeWindSensor >
      WindImpulse.bouncing) {
    WindImpulse.newImpulse();
  }
  lastInterruptionTimeWindSensor = interruptionTime;
}

void initializeWindSensor(void) {
  if (Device.configuration.noOfWindSensors > 0) {
    if (WindSensor.begin(&Data, &WindImpulse)) {
      pinMode(WindSensor.configuration.gpio, INPUT_PULLUP);
      attachInterrupt(digitalPinToInterrupt(WindSensor.configuration.gpio),
                      newWindImpulse, RISING);
#ifdef DEBUG
      Serial << endl
             << "INFO: Interrupt attached to GPIO: "
             << WindSensor.configuration.gpio;
#endif
    }
#ifdef DEBUG
    else {
      Serial << endl
             << "ERROR: Interrupt NOT attached to GPIO: "
             << WindSensor.configuration.gpio;
    }
#endif
  }
}

void windSensorListener(void) {
  if (Device.configuration.noOfWindSensors > 0) {
    if (WindSensor.listener()) {
      float speedMS = 0;
      float speedKMH = 0;
      WindSensor.get(speedMS, speedKMH);
    }
  }
}

#endif