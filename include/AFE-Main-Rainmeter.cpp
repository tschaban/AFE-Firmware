#ifdef AFE_CONFIG_HARDWARE_RAINMETER


/* ---------Headers ---------*/

void ICACHE_RAM_ATTR newRainImpulse(void);
void initializeRainmeter(void);
void rainmeterEventsListener(void);

/* --------- Body -----------*/
 

void ICACHE_RAM_ATTR newRainImpulse(void) {
  static uint32_t lastInterruptionTimeRainSensor = 0;
  uint32_t interruptionTime = millis();
  if (interruptionTime - lastInterruptionTimeRainSensor >
      RainImpulse.bouncing) {
    RainImpulse.newImpulse();
  }
  lastInterruptionTimeRainSensor = interruptionTime;
}

void initializeRainmeter(void) {
  if (Device.configuration.noOfRainmeterSensors > 0) {
    if (RainSensor.begin(&Data, &RainImpulse)) {
      pinMode(RainSensor.configuration.gpio, INPUT_PULLUP);
      attachInterrupt(digitalPinToInterrupt(RainSensor.configuration.gpio),
                      newRainImpulse, RISING);
#ifdef DEBUG
      Serial << endl
             << F("INFO: RAINMETER: Interrupt attached to GPIO: ")
             << RainSensor.configuration.gpio;
#endif
    }
#ifdef DEBUG
    else {
      Serial << endl
             << F("ERROR: RAINMETER: Interrupt NOT attached to GPIO: ")
             << RainSensor.configuration.gpio;
    }
#endif
  }
}

void rainmeterEventsListener(void) {
  if (Device.configuration.noOfRainmeterSensors > 0) {
    if (RainSensor.listener()) {
      MqttAPI.publishRainSensorData();
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
      HttpDomoticzAPI.publishRainSensorData();
#endif
    }
  }
}

#endif