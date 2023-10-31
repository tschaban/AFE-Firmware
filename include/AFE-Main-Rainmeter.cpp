#ifdef AFE_CONFIG_HARDWARE_RAINMETER

/* ---------Headers ---------*/

void IRAM_ATTR newRainImpulse(void);
void attacheInteruptToRainMeter(void);
void rainmeterEventsListener(void);

/* --------- Body -----------*/

void IRAM_ATTR newRainImpulse(void)
{
  static uint32_t lastInterruptionTimeRainSensor = 0;
  uint32_t interruptionTime = millis();
  if (interruptionTime - lastInterruptionTimeRainSensor >
      Hardware->RainImpulse->bouncing)
  {
    Hardware->RainImpulse->newImpulse();
  }
  lastInterruptionTimeRainSensor = interruptionTime;
}

void attacheInteruptToRainMeter(void)
{
  if (Firmware->Device->configuration.noOfRainmeterSensors > 0)
  {
    if (Hardware->initRainmeter())
    {
      attachInterrupt(digitalPinToInterrupt(Hardware->RainmeterSensor->configuration->gpio),
                      newRainImpulse, RISING);
#ifdef DEBUG
      Firmware->Debugger->printInformation(F("Interrupt attached to GPIO: "), F("RAINMETER"));
    }
    else
    {
      Firmware->Debugger->printError(F("Interrupt NOT attached to GPIO: "), F("RAINMETER"));
#endif
    }
#ifdef DEBUG
    Firmware->Debugger->printValue(Hardware->RainmeterSensor->configuration->gpio);
#endif
  }
}

void rainmeterEventsListener(void)
{
  if (Firmware->Device->configuration.noOfRainmeterSensors > 0)
  {
    if (Hardware->RainmeterSensor->listener())
    {
      MqttAPI->publishRainSensorData();
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
      HttpDomoticzAPI->publishRainSensorData();
#endif
    }
  }
}

#endif