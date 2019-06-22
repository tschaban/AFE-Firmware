/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */
#include "AFE-API-MQTT.h"

AFEMQTT::AFEMQTT() {}

void AFEMQTT::begin() {
  NetworkConfiguration = Data.getNetworkConfiguration();
  sprintf(deviceName, "%s", Device.configuration.name);
  Broker.setClient(esp);
  if (strlen(MQTTConfiguration.host) > 0) {
    Broker.setServer(MQTTConfiguration.host, MQTTConfiguration.port);
  } else if (strlen(MQTTConfiguration.ip) > 0) {
    IPAddress ip;
    if (ip.fromString(MQTTConfiguration.ip)) {
      Broker.setServer(MQTTConfiguration.ip, MQTTConfiguration.port);
    }
#ifdef DEBUG
    else {
      Serial << endl
             << "ERROR: Problem with MQTT IP address: " << MQTTConfiguration.ip;
    }
#endif
  } else {
    isConfigured = false;
  }

  Broker.setCallback(MQTTMessagesListener);
  Data = {};

#ifdef DEBUG
  Serial << endl
         << endl
         << "---------------------- MQTT ----------------------" << endl
         << "Host: " << MQTTConfiguration.host << endl
         << "IP: " << MQTTConfiguration.ip << endl
         << "Port: " << MQTTConfiguration.port << endl
         << "User: " << MQTTConfiguration.user << endl
         << "Password: " << MQTTConfiguration.password << endl
         << "LWT Topic: " << MQTTConfiguration.lwt.topic << endl
         << "--------------------------------------------------";
#endif
}

void AFEMQTT::subscribe(const char *topic) {
#ifdef DEBUG
  Serial << endl << "MQTT: Subscribing to : " << topic;
#endif
  if (strlen(topic) > 0) {
    Broker.subscribe(topic);
  }
}

void AFEMQTT::disconnect() {
  if (Broker.connected()) {
    Broker.disconnect();
  }
}

void AFEMQTT::listener() {
  if (Broker.connected()) {
    Broker.loop();
  } else {
    connect();
  }
}

void AFEMQTT::connect() {

  if (isConfigured) {
    if (sleepMode) {
      if (millis() - sleepStartTime >=
          NetworkConfiguration.waitTimeSeries * 1000) {
        sleepMode = false;
      }
    } else {
#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
      if (ledStartTime == 0) {
        ledStartTime = millis();
      }
#endif
      if (delayStartTime == 0) {
        delayStartTime = millis();

        /* Connecing to MQTT Broker depending on LWT topics being set or no */
        boolean _connected =
            strlen(MQTTConfiguration.lwt.topic) > 0
                ? Broker.connect(deviceName, MQTTConfiguration.user,
                                 MQTTConfiguration.password,
                                 MQTTConfiguration.lwt.topic, 2, false,
                                 "disconnected")
                : Broker.connect(deviceName, MQTTConfiguration.user,
                                 MQTTConfiguration.password);

        if (_connected) {
          eventConnectionEstablished = true;
          delayStartTime = 0;
#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
          ledStartTime = 0;
          Led.off();
#endif
          connections = 0;

          return;
        }
      }
#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
      if (millis() > ledStartTime + 500) {
        Led.toggle();
        ledStartTime = 0;
      }
#endif
      if (millis() >

          delayStartTime + (NetworkConfiguration.waitTimeConnections * 1000)) {
        connections++;
#ifdef DEBUG
        Serial << endl
               << "MQTT Connection attempt: " << connections + 1 << " from "
               << NetworkConfiguration.noConnectionAttempts
               << ", connection status: " << Broker.state()
               << ", connection time: " << millis() - delayStartTime << "ms";

#endif
        delayStartTime = 0;
      }

      if (connections >= NetworkConfiguration.noConnectionAttempts) {
        sleepMode = true;
        sleepStartTime = millis();

        delayStartTime = 0;
#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
        ledStartTime = 0;
        Led.off();
#endif
        connections = 0;

#ifdef DEBUG
        Serial << endl
               << "MQTT: WARN: Not able to connect to MQTT.Going to sleep mode "
                  "for "
               << NetworkConfiguration.waitTimeSeries << "sec.";
#endif
      }
    }
  }
}

void AFEMQTT::setReconnectionParams(
    uint8_t no_connection_attempts,
    uint8_t duration_between_connection_attempts,
    uint8_t duration_between_next_connection_attempts_series) {
  NetworkConfiguration.noConnectionAttempts = no_connection_attempts;
  NetworkConfiguration.waitTimeConnections =
      duration_between_connection_attempts;
  NetworkConfiguration.waitTimeSeries =
      duration_between_next_connection_attempts_series;
}

void AFEMQTT::publishTopic(const char *subTopic, const char *message) {
  if (Broker.state() == MQTT_CONNECTED) {
#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
    Led.on();
#endif
#ifdef DEBUG
    Serial << endl << endl << "----------- Publish MQTT -----------";
    Serial << endl << "Topic: " << subTopic;
    Serial << endl << "Message: " << message;
#endif
    if (strlen(subTopic) > 0) {
      Broker.publish(subTopic, message);
#ifdef DEBUG
      Serial << endl << "Status: published";
#endif
    }
#ifdef DEBUG
    else {
      Serial << endl << "Status: failure, not MQTT Topic";
    }
#endif
#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
    Led.off();
#endif
#ifdef DEBUG
    Serial << endl << "------------------------------------";
#endif
  }
}

void AFEMQTT::publishTopic(const char *subTopic, const char *type,
                           const char *message) {
  char _mqttTopic[MAX_MQTT_TOPIC_LENGTH];
  sprintf(_mqttTopic, "%s/%s", subTopic, type);
  publishTopic(_mqttTopic, message);
}

void AFEMQTT::publishTopic(const char *subTopic, const char *type, float value,
                           uint8_t width, uint8_t precision) {
  char message[10];
  dtostrf(value, width, precision, message);
  publishTopic(subTopic, type, message);
}

boolean AFEMQTT::eventConnected() {
  boolean returnValue = eventConnectionEstablished;
  eventConnectionEstablished = false;
  return returnValue;
}

const char *AFEMQTT::getLWTTopic() { return MQTTConfiguration.lwt.topic; }
