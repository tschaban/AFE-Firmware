/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */
#ifdef AFE_CONFIG_HARDWARE_AS3935

void handleAS3935Interraption() { AS3935Sensor.interruptionReported(); }

void initializeAS3935Sensor() {
  AS3935Sensor.begin();
  attachInterrupt(AS3935Sensor.configuration.irqGPIO, handleAS3935Interraption,
                  RISING);
}

/* Main code for processing sesnor */
void AS3935SensorEventsListener() {
  if (AS3935Sensor.strikeDetected()) {
    MQTTPublishAS3935SensorData();
  }
}
#endif
