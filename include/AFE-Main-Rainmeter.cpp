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
      Hardware->RainImpulse->bouncing) {
    Hardware->RainImpulse->newImpulse();
  }
  lastInterruptionTimeRainSensor = interruptionTime;
}

void initializeRainmeter(void) {
  if (Firmware->Device->configuration.noOfRainmeterSensors > 0) {
    if (Hardware->RainmeterSensor->begin(Firmware->API->Flash, Hardware->RainImpulse)) {
      pinMode(Hardware->RainmeterSensor->configuration->gpio, INPUT_PULLUP);
      attachInterrupt(digitalPinToInterrupt(Hardware->RainmeterSensor->configuration->gpio),
                      newRainImpulse, RISING);
#ifdef DEBUG
      Serial << endl
             << F("INFO: RAINMETER: Interrupt attached to GPIO: ")
             << Hardware->RainmeterSensor->configuration->gpio;
#endif
    }
#ifdef DEBUG
    else {
      Serial << endl
             << F("ERROR: RAINMETER: Interrupt NOT attached to GPIO: ")
             << Hardware->RainmeterSensor->configuration->gpio;
    }
#endif
  }
}

void rainmeterEventsListener(void) {
  if (Firmware->Device->configuration.noOfRainmeterSensors > 0) {
    if (Hardware->RainmeterSensor->listener()) {
      MqttAPI->publishRainSensorData();
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
      HttpDomoticzAPI->publishRainSensorData();
#endif
    }
  }
}

#endif