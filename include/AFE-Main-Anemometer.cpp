#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER

/* ---------Headers ---------*/

void IRAM_ATTR newWindImpulse(void);
void attacheInteruptToAnenoMeter(void);
void anemometerEventsListener(void);

/* --------- Body -----------*/

void IRAM_ATTR newWindImpulse()
{
  static uint32_t lastInterruptionTimeAnemometerSensor = 0;
  uint32_t interruptionTime = millis();
  if (interruptionTime - lastInterruptionTimeAnemometerSensor >
      Hardware->WindImpulse->bouncing)
  {
    Hardware->WindImpulse->newImpulse();
  }
  lastInterruptionTimeAnemometerSensor = interruptionTime;
}

void attacheInteruptToAnenoMeter(void)
{
  {
    if (Firmware->Device->configuration.noOfAnemometerSensors > 0)
    {
      if (Hardware->initAnemometer())
      {
        attachInterrupt(digitalPinToInterrupt(Hardware->AnemometerSensor->configuration->gpio),
                        newWindImpulse, RISING);
#ifdef DEBUG
        Firmware->Debugger->printInformation(F("Interrupt attached to GPIO: "), F("ANENOMETER"));
      }
      else
      {
        Firmware->Debugger->printError(F("Interrupt NOT attached to GPIO: "), F("ANENOMETER"));
#endif
      }
#ifdef DEBUG
      Firmware->Debugger->printValue(Hardware->AnemometerSensor->configuration->gpio);
#endif
    }
  }
}

void anemometerEventsListener(void)
{
  if (Firmware->Device->configuration.noOfAnemometerSensors > 0)
  {
    if (Hardware->AnemometerSensor->listener())
    {
      MqttAPI->publishAnemometerSensorData();
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
      HttpDomoticzAPI->publishAnemometerSensorData();
#endif
    }
  }
}

#endif