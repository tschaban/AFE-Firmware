/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

/* Initializing MQTT */
void MQTTInit() {
  if (Device.getMode() != MODE_ACCESS_POINT && Device.configuration.mqttAPI) {
    MQTTConfiguration = Data.getMQTTConfiguration();
    Mqtt.begin();
  }
}

/* Method is launched after MQTT Message is received */
void MQTTMessagesListener(char *topic, byte *payload, unsigned int length) {
  char _mqttTopic[70];
  Led.on();
  if (length >= 1) {
    // command arrived
    /* RELAY obsolete
    sprintf(_mqttTopic, "%srelay/cmd", MQTTConfiguration.topic);
    if (strcmp(topic, _mqttTopic) == 0) {
      if ((char)payload[1] == 'n' && length == 2) { // on
        Relay[0].on();
        Mqtt.publish(Relay[0].getMQTTTopic(), "state", "on");
        DomoticzPublishRelayState(0);
      } else if ((char)payload[1] == 'f' && length == 3) { // off
        Relay[0].off();
        Mqtt.publish(Relay[0].getMQTTTopic(), "state", "off");
        DomoticzPublishRelayState(0);
      } else if ((char)payload[1] == 'e' && length == 3) { // get
        MQTTPublishRelayState(0);
      }
    } else {
*/
    /* CONTACTRON */
    for (uint8_t i = 0; i < sizeof(Device.configuration.isContactron); i++) {
      if (Device.configuration.isContactron[i]) {
        sprintf(_mqttTopic, "%scmd", Gate.Contactron[i].getMQTTTopic());
        if (strcmp(topic, _mqttTopic) == 0 && (char)payload[1] == 'e') { // get
          MQTTPublishContactronState(i);
        }
      } else {
        break;
      }
    }

    /* GATE */
    sprintf(_mqttTopic, "%sgate/cmd", MQTTConfiguration.topic);
    if (strcmp(topic, _mqttTopic) == 0) {
      if ((char)payload[2] == 'e' && length == 4) { // open
        // Relay[0].on();
        Gate.toggle();
      } else if ((char)payload[2] == 'o' && length == 5) { // close
        // Relay[0].on();
        Gate.toggle();
      } else if ((char)payload[2] == 't' && length == 3) { // get
        MQTTPublishGateState();
      }
    } else {

      /* Remaining MQTT Messages reboot and configuratonMode
      sprintf(_mqttTopic, "%scmd", MQTTConfiguration.topic);
      if (strcmp(topic, _mqttTopic) == 0) {
        if ((char)payload[2] == 'b' && length == 6) { // reboot
          Device.reboot(MODE_NORMAL);
        } else if ((char)payload[2] == 'n' &&
                   length == 17) { // configurationMode
          Device.reboot(MODE_CONFIGURATION);
        } else if ((char)payload[2] == 't' && length == 14) { // getTemperature
          char temperatureString[6];
          dtostrf(SensorDHT.getTemperature(), 2, 2, temperatureString);
          Mqtt.publish("temperature", temperatureString);
        } else if ((char)payload[2] == 't' && length == 11) { // getHumidity
          char humidityString[6];
          dtostrf(SensorDHT.getHumidity(), 2, 2, humidityString);
          Mqtt.publish("humidity", humidityString);
        } else if ((char)payload[2] == 't' && length == 12) { // getHeatIndex
          char heatIndex[6];
          dtostrf(SensorDHT.getHeatIndex(), 2, 2, heatIndex);
          Mqtt.publish("heatIndex", heatIndex);
        }
      }*/
    }
  }

  Led.off();
}

/* Metod publishes temperature */
void MQTTPublishTemperature(float temperature) {
  if (Device.configuration.mqttAPI) {
    Mqtt.publish("temperature", temperature);
  }
}
/* Metod publishes humidity */
void MQTTPublishHumidity(float humidity) {
  if (Device.configuration.mqttAPI) {
    Mqtt.publish("humidity", humidity);
  }
}
/* Metod publishes HeatIndex */
void MQTTPublishHeatIndex(float heatIndex) {
  if (Device.configuration.mqttAPI) {
    Mqtt.publish("heatIndex", heatIndex);
  }
}

/* Metod publishes Relay state (used eg by HTTP API) */
void MQTTPublishContactronState(uint8_t id) {
  if (Device.configuration.mqttAPI) {
    Mqtt.publish(Gate.Contactron[id].getMQTTTopic(), "state",
                 Gate.Contactron[id].get() == CONTACTRON_OPEN ? "open"
                                                              : "closed");
  }
}

void MQTTPublishGateState() {
  if (Device.configuration.mqttAPI) {
    uint8_t gateState = Gate.get();
    Mqtt.publish("gate/state", gateState == GATE_OPEN
                                   ? "open"
                                   : gateState == GATE_CLOSED
                                         ? "closed"
                                         : gateState == GATE_PARTIALLY_OPEN
                                               ? "partiallyOpen"
                                               : "unknown");
  }
}
