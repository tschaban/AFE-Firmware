/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

/* Initializing MQTT */
void MQTTInit() {
  if (Device.getMode() != MODE_ACCESS_POINT && Device.configuration.mqttAPI) {
    MQTTConfiguration = Data.getMQTTConfiguration();
    Mqtt.begin();
  }
}

/* Method is launched after MQTT Message is received */
void MQTTMessagesListener(char *topic, byte *payload, unsigned int length) {

  char _mqttTopic[65];
#ifndef T0_SHELLY_1_CONFIG
  Led.on();
#endif

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
#if defined(T1_CONFIG) || defined(T2_CONFIG)
        else {

          sprintf(_mqttTopic, "%sthermostat/cmd", Relay[i].getMQTTTopic());

          if (strcmp(topic, _mqttTopic) == 0) {
            if ((char)payload[0] == 'o' && length == 2) { // on
              Relay[i].Thermostat.on();
              Mqtt.publish(Relay[i].getMQTTTopic(), "thermostat/state",
                           Relay[i].Thermostat.enabled() ? "on" : "off");
            } else if ((char)payload[0] == 'o' && length == 3) { // off
              Relay[i].Thermostat.off();
              Mqtt.publish(Relay[i].getMQTTTopic(), "thermostat/state",
                           Relay[i].Thermostat.enabled() ? "on" : "off");
            } else if ((char)payload[0] == 't' && length == 6) { // toggle
              Relay[i].Thermostat.enabled() ? Relay[i].Thermostat.off()
                                            : Relay[i].Thermostat.on();
              Mqtt.publish(Relay[i].getMQTTTopic(), "thermostat/state",
                           Relay[i].Thermostat.enabled() ? "on" : "off");
            } else if ((char)payload[0] == 'g' && length == 3) { // get
              Mqtt.publish(Relay[i].getMQTTTopic(), "thermostat/state",
                           Relay[i].Thermostat.enabled() ? "on" : "off");
            }
#ifdef T2_CONFIG
          } else {
            /* Checking if Hunidistat related message has been received  */
            sprintf(_mqttTopic, "%shumidistat/cmd", Relay[i].getMQTTTopic());

            if (strcmp(topic, _mqttTopic) == 0) {
              if ((char)payload[0] == 'o' && length == 2) { // on
                Relay[i].Humidistat.on();
                Mqtt.publish(Relay[i].getMQTTTopic(), "humidistat/state",
                             Relay[i].Humidistat.enabled() ? "on" : "off");
              } else if ((char)payload[0] == 'o' && length == 3) { // off
                Relay[i].Humidistat.off();
                Mqtt.publish(Relay[i].getMQTTTopic(), "humidistat/state",
                             Relay[i].Humidistat.enabled() ? "on" : "off");
              } else if ((char)payload[0] == 't' && length == 6) { // toggle
                Relay[i].Humidistat.enabled() ? Relay[i].Humidistat.off()
                                              : Relay[i].Humidistat.on();
                Mqtt.publish(Relay[i].getMQTTTopic(), "humidistat/state",
                             Relay[i].Humidistat.enabled() ? "on" : "off");
              } else if ((char)payload[0] == 'g' && length == 3) { // get
                Mqtt.publish(Relay[i].getMQTTTopic(), "humidistat/state",
                             Relay[i].Humidistat.enabled() ? "on" : "off");
              }
            }
#endif
          }
        }
#endif
      } else {
        break;
      }
    }
    /* Turning On/Off HTTP APIs */
    sprintf(_mqttTopic, "%sconfiguration/api/http/cmd",
            MQTTConfiguration.topic);

    if (strcmp(topic, _mqttTopic) == 0) {
      if ((char)payload[1] == 'n' && length == 2) { // on
        Data.saveAPI(API_HTTP, true);
        Device.begin();
      } else if ((char)payload[1] == 'f' && length == 3) { // off
        Data.saveAPI(API_HTTP, false);
        Device.begin();
      }
    } else {

      /* Turning On/Off Domoticz APIs */
      sprintf(_mqttTopic, "%sconfiguration/api/domoticz/cmd",
              MQTTConfiguration.topic);

      if (strcmp(topic, _mqttTopic) == 0) {
        if ((char)payload[1] == 'n' && length == 2) { // on
          Data.saveAPI(API_DOMOTICZ, true);
          Device.begin();
          DomoticzInit();

        } else if ((char)payload[1] == 'f' && length == 3) { // off
          Data.saveAPI(API_DOMOTICZ, false);
          Device.begin();
          Domoticz.disconnect();
        }
      } else {
        /* Turning Off MQTT APIs */
        sprintf(_mqttTopic, "%sconfiguration/api/mqtt/cmd",
                MQTTConfiguration.topic);

        if (strcmp(topic, _mqttTopic) == 0) {
          if ((char)payload[1] == 'f' && length == 3) { // off
            Data.saveAPI(API_MQTT, false);
            Device.begin();
            Mqtt.disconnect();
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
#if defined(T1_CONFIG) || defined(T2_CONFIG)
            } else if ((char)payload[2] == 't' &&
                       length == 14) { // getTemperature
              char temperatureString[6];
#if defined(T1_CONFIG) || defined(T2_CONFIG)
              dtostrf(Sensor.getTemperature(), 2, 2, temperatureString);
#endif
              Mqtt.publish("temperature", temperatureString);
#endif

#ifdef T2_CONFIG
            } else if ((char)payload[2] == 't' && length == 11) { // getHumidity
              char humidityString[6];
              dtostrf(Sensor.getHumidity(), 2, 2, humidityString);
              Mqtt.publish("humidity", humidityString);
            } else if ((char)payload[3] == 'H' &&
                       length == 12) { // getHeatIndex
              char heatIndex[6];
              dtostrf(Sensor.getHeatIndex(), 2, 2, heatIndex);
              Mqtt.publish("heatIndex", heatIndex);
            } else if ((char)payload[3] == 'D' && length == 12) { // getDewPoint
              char heatIndex[6];
              dtostrf(Sensor.getDewPoint(), 2, 2, heatIndex);
              Mqtt.publish("dewPoint", heatIndex);
#endif
            }
          }
        }
      }
    }
  }
#ifndef T0_SHELLY_1_CONFIG
  Led.off();
#endif
}

/* Metod publishes Relay state (used eg by HTTP API) */
void MQTTPublishRelayState(uint8_t id) {
  if (Device.configuration.mqttAPI) {
    Mqtt.publish(Relay[id].getMQTTTopic(), "state",
                 Relay[id].get() == RELAY_ON ? "on" : "off");
  }
}
#if defined(T1_CONFIG) || defined(T2_CONFIG)
/* Metod publishes temperature */
void MQTTPublishTemperature(float temperature) {
  if (Device.configuration.mqttAPI) {
    Mqtt.publish("temperature", temperature);
  }
}
#endif

#if defined(T2_CONFIG)
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

/* Metod publishes Dew point */
void MQTTPublishDewPoint(float dewPoint) {
  if (Device.configuration.mqttAPI) {
    Mqtt.publish("dewPoint", dewPoint);
  }
}
#endif
