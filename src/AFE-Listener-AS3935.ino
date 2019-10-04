/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */
#ifdef AFE_CONFIG_HARDWARE_AS3935

void initializeAS3935Sensor() { AS3935Sensor.begin(); }

/* Main code for processing sesnor */
void AS3935SensorEventsListener() {
    AS3935Sensor.listener();
}
#endif
