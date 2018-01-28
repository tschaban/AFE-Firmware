/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#include "AFE-MQTT.h"

AFEMQTT::AFEMQTT() {}

void AFEMQTT::begin() {
  NETWORK NetworkConfiguration;
  NetworkConfiguration = Data.getNetworkConfiguration();
  sprintf(deviceName, "%s", Device.configuration.name);

  /* Defaults are taken from WiFi config. They can be set using
   * setReconnectionParams() */
  noConnectionAttempts = NetworkConfiguration.noConnectionAttempts;
  durationBetweenConnectionAttempts = NetworkConfiguration.waitTimeConnections;
  durationBetweenNextConnectionAttemptsSeries =
      NetworkConfiguration.waitTimeSeries;

  Broker.setClient(esp);
  if (strlen(MQTTConfiguration.host) > 0) {
    Broker.setServer(MQTTConfiguration.host, MQTTConfiguration.port);
  } else if (MQTTConfiguration.ip[0] > 0) {
    Broker.setServer(MQTTConfiguration.ip, MQTTConfiguration.port);
  } else {
    isConfigured = false;
  }

  Broker.setCallback(MQTTMessagesListener);
  sprintf(mqttTopicForSubscription, "%s#", MQTTConfiguration.topic);
  Data = {};
  NetworkConfiguration = {};
}

void AFEMQTT::connect() {

  if (isConfigured) {
    if (sleepMode) {
      if (millis() - sleepStartTime >=
          durationBetweenNextConnectionAttemptsSeries * 1000) {
        sleepMode = false;
      }
    } else {
      uint8_t connections = 0;
      /*
            Serial << endl
                   << "INFO: Connecting to MQTT: " << MQTTConfiguration.host
                   << MQTTConfiguration.ip[0] << "." << MQTTConfiguration.ip[1]
         << "."
                   << MQTTConfiguration.ip[2] << "." << MQTTConfiguration.ip[3]
         << ":"
                   << MQTTConfiguration.port << " " << MQTTConfiguration.user <<
         "@"
                   << MQTTConfiguration.password;
      */
      while (!Broker.connected()) {
        if (Broker.connect(deviceName, MQTTConfiguration.user,
                           MQTTConfiguration.password)) {
          /*
                    Serial << endl << "INFO: Connected";
                    Serial << endl
                           << "INFO: Subscribing to : " <<
             mqttTopicForSubscription;
          */
          Broker.subscribe((char *)mqttTopicForSubscription);
          /*
                      Serial << endl << "INFO: Subsribed";
          */
          // Setting Relay state after connection to MQTT

          for (uint8_t i = 0; i < sizeof(Device.configuration.isRelay); i++) {
            if (Device.configuration.isRelay[i]) {
              if (!Relay[i].setRelayAfterRestoringMQTTConnection()) {
                // Requesting state from MQTT Broker / service
                publish(Relay[i].getMQTTTopic(), "get", "defaultState");
              } else {
                // Updating relay state after setting default value after MQTT
                // connected
                publish(Relay[i].getMQTTTopic(), "state",
                        Relay[i].get() == RELAY_ON ? "on" : "off");
              }
            }
          }
        } else {
          connections++;
          /*
                    Serial << endl
                           << "INFO: MQTT Connection attempt: " << connections +
             1
                           << " from " << noConnectionAttempts;
          */
          if (connections >= noConnectionAttempts) {
            sleepMode = true;
            sleepStartTime = millis();
            /*
                        Serial
                            << endl
                            << "WARN: Not able to connect to MQTT.Going to sleep
               mode for "
                            << durationBetweenNextConnectionAttemptsSeries <<
               "sec.";
            */
            break;
          }
          Led.on();
          delay(durationBetweenConnectionAttempts * 500);
          Led.off();
          delay(durationBetweenConnectionAttempts * 500);
          /* Serial << "."; */
        }
      }
      /* Serial << endl << "INFO: MQTT connection status: " << Broker.state();
       */
    }
  }
}

void AFEMQTT::setReconnectionParams(
    uint8_t no_connection_attempts,
    uint8_t duration_between_connection_attempts,
    uint8_t duration_between_next_connection_attempts_series) {
  noConnectionAttempts = no_connection_attempts;
  durationBetweenConnectionAttempts = duration_between_connection_attempts;
  durationBetweenNextConnectionAttemptsSeries =
      duration_between_next_connection_attempts_series;
}

boolean AFEMQTT::connected() { return Broker.connected(); }

void AFEMQTT::loop() { Broker.loop(); }

void AFEMQTT::publish(const char *type, const char *message) {
  char _mqttTopic[50];
  sprintf(_mqttTopic, "%s%s", MQTTConfiguration.topic, type);
  publishToMQTTBroker(_mqttTopic, message);
}

void AFEMQTT::publish(const char *topic, const char *type,
                      const char *message) {
  char _mqttTopic[50];
  sprintf(_mqttTopic, "%s%s", topic, type);
  publishToMQTTBroker(_mqttTopic, message);
}

void AFEMQTT::publishToMQTTBroker(const char *topic, const char *message) {
  if (Broker.state() == MQTT_CONNECTED) {
    //  Serial << endl << "INFO: MQTT publising:  " << topic << "  \\ " <<
    //  message;
    Broker.publish(topic, message);
  }
}
