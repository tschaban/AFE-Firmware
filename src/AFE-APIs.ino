
/* Initializing Domoticz HTTP API */
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED

void initializeHTTPDomoticzAPI() {
  if (Device.getMode() != AFE_MODE_ACCESS_POINT &&
      Device.configuration.api.httpDomoticz) {

#ifdef AFE_CONFIG_HARDWARE_LED
    HttpDomoticzAPI.begin(&Data, &Device, &Led);
#else
    HttpDomoticzAPI.begin(&Data, &Device);
#endif

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
    Serial << endl << "API: MQTT init completed";
#endif
  }
}

#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

/* Initializing MQTT API */
void initializeMQTTAPI() {
  if (Device.getMode() != AFE_MODE_ACCESS_POINT &&
      Device.configuration.api.mqtt) {

#ifdef DEBUG
    Serial << endl << "INFO: API: Initializing MQTT";
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
    MqttAPI.begin(&Data, &Device, &Led);
#else
    MqttAPI.begin(&Data, &Device);
#endif

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
    Serial << endl << "INFO: API: MQTT init completed";
#endif
  }
}

/* Initializing HTTP API */
void initializeHTTPAPI() {
  if (Device.getMode() != AFE_MODE_ACCESS_POINT) {

#ifdef DEBUG
    Serial << endl << "INFO: API: Initializing MQTT";
#endif

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
    HttpAPI.begin(&Device, &WebServer, &Data, &MqttAPI, &HttpDomoticzAPI);
#else
    HttpAPI.begin(&Device, &WebServer, &Data, &MqttAPI);
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
    HttpAPI.addClass(&Relay[0]);
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
    HttpAPI.addClass(&AnalogInput);
#endif

#ifdef DEBUG
    Serial << endl << "INFO: API: HTTP init completed";
#endif
  }
}
