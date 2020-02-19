
/* Initializing Domoticz HTTP API */
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED

void initializeHTTPDomoticzAPI() {
  if (Device.getMode() != AFE_MODE_ACCESS_POINT &&
      Device.configuration.api.domoticz) {

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


#ifdef AFE_CONFIG_HARDWARE_BMEX80
  HttpDomoticzAPI.addClass(&BMEX80Sensor[0]);
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  HttpDomoticzAPI.addClass(&ParticleSensor[0]);
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
  HttpDomoticzAPI.addClass(&BH1750Sensor[0]);
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
  HttpDomoticzAPI.addClass(&AS3935Sensor[0]);
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

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  MqttAPI.addClass(&BMEX80Sensor[0]);
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  MqttAPI.addClass(&ParticleSensor[0]);
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
  MqttAPI.addClass(&BH1750Sensor[0]);
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
  MqttAPI.addClass(&AS3935Sensor[0]);
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

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  HttpAPI.addClass(&BMEX80Sensor[0]);
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  HttpAPI.addClass(&ParticleSensor[0]);
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
  HttpAPI.addClass(&BH1750Sensor[0]);
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
  HttpAPI.addClass(&AS3935Sensor[0]);
#endif




#ifdef DEBUG
    Serial << endl << "INFO: API: HTTP init completed";
#endif
  }
}
