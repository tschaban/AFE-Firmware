void initializeMQTTAPI() {
  if (Device.getMode() != AFE_MODE_ACCESS_POINT &&
      Device.configuration.api.mqtt) {
    MqttAPI.begin(&Data, &Device);

#ifdef AFE_CONFIG_HARDWARE_RELAY
      MqttAPI.addClass(&Relay[0]);
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
    MqttAPI.addClass(&Switch[0]);
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  if (Device.configuration.isAnalogInput) {
    MqttAPI.addClass(&AnalogInput);
  }
#endif

#ifdef DEBUG
    Serial << endl << "API: MQTT initialized";
#endif
  }
}
