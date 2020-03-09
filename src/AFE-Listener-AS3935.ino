/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */
#ifdef AFE_CONFIG_HARDWARE_AS3935

/* Warning: for now it's hardcoded for one sensor only */


/* Attaches the interaption hander on the GPIO to which Sensor IRQ is connected
 */
ICACHE_RAM_ATTR void handleAS3935Interraption() {
  AS3935Sensor[0].interruptionReported();
}

/* Initializing the sensor */
void initializeAS3935Sensor() {
  if (Device.configuration.noOfAS3935s > 0) {
    if (AS3935Sensor[0].begin(0)) {
      attachInterrupt(AS3935Sensor[0].configuration.irqGPIO,
                      handleAS3935Interraption, RISING);
    }
  }
}

/* Main code for processing sensor */
void AS3935SensorEventsListener() {
  if (Device.configuration.noOfAS3935s > 0) {
    if (AS3935Sensor[0].strikeDetected()) {
      MqttAPI.publishAS3935SensorData(0);
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED      
      HttpDomoticzAPI.publishAS3935SensorData(0);
#endif      
    }
  }
}
#endif
