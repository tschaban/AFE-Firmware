#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED

void initializeHTTPDomoticzAPI() {
  if (Device.getMode() != AFE_MODE_ACCESS_POINT &&
      Device.configuration.api.httpDomoticz) {
    HttpDomoticzAPI.begin(&Data, &Device);

#ifdef AFE_CONFIG_HARDWARE_RELAY
      HttpDomoticzAPI.addClass(&Relay[0]);
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
    HttpDomoticzAPI.addClass(&Switch[0]);
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  if (Device.configuration.isAnalogInput) {
    HttpDomoticzAPI.addClass(&AnalogInput);
  }
#endif

#ifdef DEBUG
    Serial << endl << "API: MQTT initialized";
#endif
  }
}

#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
