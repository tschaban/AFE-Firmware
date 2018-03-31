/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

/* Method is launched after MQTT Message is received */
void MQTTMessagesListener(char *topic, byte *payload, unsigned int length) {

  char _mqttTopic[65];
  Led.on();

  if (length >= 1) {

    for (uint8_t i = 0; i < sizeof(Device.configuration.isRelay); i++) {
      if (Device.configuration.isRelay[i]) {
        sprintf(_mqttTopic, "%scmd", Relay[i].getMQTTTopic());

        if (strcmp(topic, _mqttTopic) == 0) {
          if ((char)payload[1] == 'n' && length == 2) { // on
            Relay[i].on();
            Mqtt.publish(Relay[i].getMQTTTopic(), "state", "on");
            DomoticzPublishRelayState(i);
          } else if ((char)payload[1] == 'f' && length == 3) { // off
            Relay[i].off();
            Mqtt.publish(Relay[i].getMQTTTopic(), "state", "off");
            DomoticzPublishRelayState(i);
          } else if ((char)payload[1] == 'e' && length == 3) { // get
            MQTTPublishRelayState(i);
          } else if ((char)payload[1] == 'o' && length == 6) { // toggle
            Relay[i].get() == RELAY_ON ? Relay[i].off() : Relay[i].on();
            MQTTPublishRelayState(i);
            DomoticzPublishRelayState(i);
          }
        }
      } else {
        break;
      }
    }

    /* Turning On/Off HTTP APIs */
    sprintf(_mqttTopic, "%sconfiguration/api/http", MQTTConfiguration.topic);

    if (strcmp(topic, _mqttTopic) == 0) {
      if ((char)payload[1] == 'n' && length == 2) { // on
        Data.saveAPI(API_HTTP, true);
      } else if ((char)payload[1] == 'f' && length == 3) { // off
        Data.saveAPI(API_HTTP, false);
      }
    } else {

      /* Turning On/Off Domoticz APIs */
      sprintf(_mqttTopic, "%sconfiguration/api/domoticz",
              MQTTConfiguration.topic);

      if (strcmp(topic, _mqttTopic) == 0) {
        if ((char)payload[1] == 'n' && length == 2) { // on
          Data.saveAPI(API_DOMOTICZ, true);
        } else if ((char)payload[1] == 'f' && length == 3) { // off
          Data.saveAPI(API_DOMOTICZ, false);
        }
      } else {
        /* Turning Off MQTT APIs */
        sprintf(_mqttTopic, "%sconfiguration/api/mqtt",
                MQTTConfiguration.topic);

        if (strcmp(topic, _mqttTopic) == 0) {
          if ((char)payload[1] == 'f' && length == 3) { // off
            Data.saveAPI(API_MQTT, false);
          }
        } else {

          /* Remaining MQTT Messages reboot and configuratonMode */
          sprintf(_mqttTopic, "%scmd", MQTTConfiguration.topic);

          if (strcmp(topic, _mqttTopic) == 0) {
            if ((char)payload[2] == 'b' && length == 6) { // reboot
              Device.reboot(MODE_NORMAL);
            } else if ((char)payload[2] == 'n' &&
                       length == 17) { // configurationMode
              Device.reboot(MODE_CONFIGURATION);
            }
          }
        }
      }
    }
  }
  Led.off();
}

/* Metod publishes Relay state (used eg by HTTP API) */
void MQTTPublishRelayState(uint8_t id) {
  if (Device.configuration.mqttAPI) {
    Mqtt.publish(Relay[id].getMQTTTopic(), "state",
                 Relay[id].get() == RELAY_ON ? "on" : "off");
  }
}
