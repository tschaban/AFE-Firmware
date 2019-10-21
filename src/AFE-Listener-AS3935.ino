/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */
#ifdef AFE_CONFIG_HARDWARE_AS3935

/* Attaches the interaption hander on the GPIO to which Sensor IRQ is connected
 */
ICACHE_RAM_ATTR void handleAS3935Interraption() {
  AS3935Sensor.interruptionReported();
}

/* Initializing the sensor */
void initializeAS3935Sensor() {
  if (Device.configuration.noOfAS3935s > 0) {
    Led.on();
    if (AS3935Sensor.begin()) {
      attachInterrupt(AS3935Sensor.configuration.irqGPIO,
                      handleAS3935Interraption, RISING);
    }
    Led.off();
  }
}

/* Main code for processing sensor */
void AS3935SensorEventsListener() {
  if (Device.configuration.noOfAS3935s > 0) {
    if (AS3935Sensor.strikeDetected()) {
      Led.on();
      MQTTPublishAS3935SensorData();
      DomoticzPublishAS3935SesnorData();
      Led.off();
    }
  }
}
#endif
