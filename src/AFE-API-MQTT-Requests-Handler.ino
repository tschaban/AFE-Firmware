/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

/* Initializing MQTT */
void MQTTInit() {
  if (Device.getMode() != MODE_ACCESS_POINT && Device.configuration.api.mqtt) {
    MQTTConfiguration = Data.getMQTTConfiguration();
    Mqtt.begin();
#ifdef DEBUG
    Serial << endl << "API: MQTT initialized";
#endif
  }
}

/* Method is launched after MQTT Message is received */
void MQTTMessagesListener(char *topic, byte *payload, unsigned int length) {
  char _mqttTopic[65];
#ifdef CONFIG_HARDWARE_LED
  Led.on();
#endif

  if (length >= 1) {
#ifdef DEBUG
    Serial << endl << endl << "--------- Got MQTT request ---------";
    Serial << endl << "Topic: " << topic;
    Serial << endl << "Message: ";
    for (uint8_t _i = 0; _i < length; _i++) {
      Serial << char(payload[_i]);
    }
    Serial << endl << "------------------------------------" << endl;
#endif

#ifdef CONFIG_FUNCTIONALITY_RELAY  /* Relay processing */
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

#ifdef CONFIG_FUNCTIONALITY_THERMOSTAT
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
#ifdef CONFIG_FUNCTIONALITY_HUMIDISTAT
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

#if defined(T3_CONFIG)
    for (uint8_t i = 0; i < sizeof(Device.configuration.isPIR); i++) {
      if (Device.configuration.isPIR[i]) {
        sprintf(_mqttTopic, "%scmd", Pir[i].getMQTTTopic());
        if (strcmp(topic, _mqttTopic) == 0) {
          if ((char)payload[1] == 'e' && length == 3) { // get
            MQTTPublishPIRState(i);
          }
        }
      }
    }
#endif

#ifdef CONFIG_HARDWARE_HPMA115S0
    if (Device.configuration.isHPMA115S0) {
      sprintf(_mqttTopic, "%HPMA115S0/cmd", MQTTConfiguration.mqtt.topic);
      if (strcmp(topic, _mqttTopic) == 0) {
        if ((char)payload[1] == 'e' && length == 3) { // get
          HPMA115S0_DATA sensorData;
          sensorData = ParticleSensor.get();
          MQTTPublishParticleSensorData(sensorData);
        }
      }
    }
#endif

#ifdef CONFIG_HARDWARE_BMX80
    if (Device.configuration.isBMx80) {
      sprintf(_mqttTopic, "%sBMx80/cmd", MQTTConfiguration.mqtt.topic);
      if (strcmp(topic, _mqttTopic) == 0) {
        if ((char)payload[1] == 'e' && length == 3) { // get
          BMx80_DATA sensorData;
          sensorData = BMx80Sensor.get();
          MQTTPublishBMx80SensorData(sensorData);
        }
      }
    }
#endif

#ifdef CONFIG_HARDWARE_ADC_VCC
    if (Device.configuration.isAnalogInput) {
      sprintf(_mqttTopic, "%sADC/cmd", MQTTConfiguration.mqtt.topic);
      if (strcmp(topic, _mqttTopic) == 0) {
        if ((char)payload[1] == 'e' && length == 3) { // get
          ADCINPUT_DATA data = AnalogInput.get();
          MQTTPublishAnalogInputData(data);
        }
      }
    }
#endif

#else /* Gate */

    /* Contactrons */
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

    /* Gate */
    sprintf(_mqttTopic, "%sgate/cmd", MQTTConfiguration.mqtt.topic);
    if (strcmp(topic, _mqttTopic) == 0) {
      if ((char)payload[0] == 't' && length == 6) { // toggle
        Gate.toggle();
      } else if ((char)payload[0] == 'g' && length == 3) { // get
        MQTTPublishGateState();
      }
    } // @TODO powinno byc else here

#endif /* Relay and Gate processing */

    /* Turning On/Off HTTP APIs */
    sprintf(_mqttTopic, "%sconfiguration/api/http/cmd",
            MQTTConfiguration.mqtt.topic);

    if (strcmp(topic, _mqttTopic) == 0) {
      if ((char)payload[1] == 'n' && length == 2) { // on
        Data.saveAPI(API_HTTP, true);
        Device.begin();
      } else if ((char)payload[1] == 'f' && length == 3) { // off
        Data.saveAPI(API_HTTP, false);
        Device.begin();
      }
    } else {

      /* Start: Turning On/Off Domoticz APIs */
      sprintf(_mqttTopic, "%sconfiguration/api/domoticz/cmd",
              MQTTConfiguration.mqtt.topic);

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
        /* Stop: Turning On/Off Domoticz APIs */
      } else {

        /* start: Turning Off MQTT APIs */
        sprintf(_mqttTopic, "%sconfiguration/api/mqtt/cmd",
                MQTTConfiguration.mqtt.topic);

        if (strcmp(topic, _mqttTopic) == 0) {
          if ((char)payload[1] == 'f' && length == 3) { // off
            Data.saveAPI(API_MQTT, false);
            Device.begin();
            Mqtt.disconnect();
          }
          /* Stop: Turning Off MQTT APIs */
        } else {
          /* Start: other mqtt requests
              - reboot,
              - configurationMode
              - getTemperature
              - getHumidity
              - getHeatIndex
              - getDewPoint
           * humifity */
          sprintf(_mqttTopic, "%scmd", MQTTConfiguration.mqtt.topic);

          if (strcmp(topic, _mqttTopic) == 0) {
            /* Reboot */
            if ((char)payload[2] == 'b' && length == 6) {
              Device.reboot(MODE_NORMAL);
            }
            /* configurationMode */
            else if ((char)payload[2] == 'n' && length == 17) {
              Device.reboot(MODE_CONFIGURATION);
#if defined(T1_CONFIG) || defined(T2_CONFIG) || defined(T5_CONFIG)
            }
            /* getTemperature */
            else if ((char)payload[2] == 't' && length == 14) {
              char temperatureString[6];
              dtostrf(Sensor.getTemperature(), 2, 2, temperatureString);
              Mqtt.publish("temperature", temperatureString);
#endif

#ifdef CONFIG_HARDWARE_DHXX
            }
            /* getHumidity */
            else if ((char)payload[2] == 't' && length == 11) {
              char humidityString[6];
              dtostrf(Sensor.getHumidity(), 2, 2, humidityString);
              Mqtt.publish("humidity", humidityString);
            }
            /* getHeatIndex */
            else if ((char)payload[3] == 'H' && length == 12) {
              char heatIndex[6];
              dtostrf(Sensor.getHeatIndex(), 2, 2, heatIndex);
              Mqtt.publish("heatIndex", heatIndex);
            }
            /* getDewPoint */
            else if ((char)payload[3] == 'D' && length == 12) {
              char heatIndex[6];
              dtostrf(Sensor.getDewPoint(), 2, 2, heatIndex);
              Mqtt.publish("dewPoint", heatIndex);
#endif
            }
          }
        } /* End of other methods */
      }
    }
  }
#ifdef CONFIG_HARDWARE_LED
  Led.off();
#endif
} /* End of topics listener */

/* Metod publishes Relay state (used eg by HTTP API) */
void MQTTPublishRelayState(uint8_t id) {
  if (Device.configuration.api.mqtt) {
    Mqtt.publish(Relay[id].getMQTTTopic(), "state",
                 Relay[id].get() == RELAY_ON ? "on" : "off");
  }
}
#ifdef CONFIG_TEMPERATURE
/* Metod publishes temperature */
void MQTTPublishTemperature(float temperature) {
  if (Device.configuration.api.mqtt) {
    Mqtt.publish("temperature", temperature);
  }
}
#endif

/* Humidity, HeatIndex, DewPoint */
#ifdef CONFIG_HUMIDITY
/* Metod publishes humidity */
void MQTTPublishHumidity(float humidity) {
  if (Device.configuration.api.mqtt) {
    Mqtt.publish("humidity", humidity);
  }
}
/* Metod publishes HeatIndex */
void MQTTPublishHeatIndex(float heatIndex) {
  if (Device.configuration.api.mqtt) {
    Mqtt.publish("heatIndex", heatIndex);
  }
}
/* Metod publishes Dew point */
void MQTTPublishDewPoint(float dewPoint) {
  if (Device.configuration.api.mqtt) {
    Mqtt.publish("dewPoint", dewPoint);
  }
}
#endif

#if defined(T3_CONFIG)
/* Metod publishes Relay state (used eg by HTTP API) */
void MQTTPublishPIRState(uint8_t id) {
  if (Device.configuration.api.mqtt) {
    Mqtt.publish(Pir[id].getMQTTTopic(), "state",
                 Pir[id].motionDetected() ? "open" : "closed");
  }
}
#endif

#if defined(T5_CONFIG)
void MQTTPublishContactronState(uint8_t id) {
  if (Device.configuration.api.mqtt) {
    Mqtt.publish(Gate.Contactron[id].getMQTTTopic(), "state",
                 Gate.Contactron[id].get() == CONTACTRON_OPEN ? "open"
                                                              : "closed");
  }
}
void MQTTPublishGateState() {
  if (Device.configuration.api.mqtt) {
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
#endif

#ifdef CONFIG_HARDWARE_HPMA115S0
void MQTTPublishParticleSensorData(HPMA115S0_DATA data) {
  if (Device.configuration.api.mqtt) {
    String messageString = "{'PM25':'";
    messageString += data.pm25;
    messageString += "','PM10':'";
    messageString += data.pm10;
    messageString += "'}";
    char message[messageString.length() + 1];
    messageString.toCharArray(message, messageString.length() + 1);
    Mqtt.publish("HPMA115S0/all", message);
  }
}
#endif

#ifdef CONFIG_HARDWARE_BMX80
void MQTTPublishBMx80SensorData(BMx80_DATA data) {
  if (Device.configuration.api.mqtt) {
    String messageString = "{'temperature':'";
    messageString += data.temperature;
    if (Device.configuration.isBMx80 != TYPE_BMP180_SENSOR) {
      messageString += "','humidity':'";
      messageString += data.humidity;
    }
    if (Device.configuration.isBMx80 == TYPE_BME680_SENSOR) {
      messageString += "','gasResistance':'";
      messageString += data.gasResistance;
    }
    messageString += "','pressure':'";
    messageString += data.pressure;
    messageString += "'}";
    char message[messageString.length() + 1];
    messageString.toCharArray(message, messageString.length() + 1);
    Mqtt.publish("BMx80/all", message);
  }
}
#endif

#ifdef CONFIG_HARDWARE_BH1750
void MQTTPublishLightLevel(float lux) {
  if (Device.configuration.api.mqtt) {
    Mqtt.publish("BH1750/lux", lux);
  }
}
#endif

#ifdef CONFIG_HARDWARE_ADC_VCC
void MQTTPublishAnalogInputData(ADCINPUT_DATA data) {
  if (Device.configuration.api.mqtt) {
    String messageString = "{'raw':'";
    messageString += data.raw;
    messageString += "','percent':'";
    messageString += data.percent;
    messageString += "','voltage':'";
    messageString += data.voltage;
    messageString += "'}";
    char message[messageString.length() + 1];
    messageString.toCharArray(message, messageString.length() + 1);
    Mqtt.publish(AnalogInput.getMQTTTopic(), message);
  }
}
#endif
