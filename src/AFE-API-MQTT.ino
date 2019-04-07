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
  } else if (MQTTConfiguration.ip[0] > 0) {
    Broker.setServer(MQTTConfiguration.ip, MQTTConfiguration.port);
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
         << "IP: " << MQTTConfiguration.ip[0] << "." << MQTTConfiguration.ip[1]
         << "." << MQTTConfiguration.ip[2] << "." << MQTTConfiguration.ip[3]
         << endl
         << "Port: " << MQTTConfiguration.port << endl
         << "User: " << MQTTConfiguration.user << endl
         << "Password: " << MQTTConfiguration.password << endl
         << "Topic: " << MQTTConfiguration.mqtt.topic << endl
         << "--------------------------------------------------";
#endif
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
#ifdef CONFIG_HARDWARE_LED
      if (ledStartTime == 0) {
        ledStartTime = millis();
      }
#endif
      if (delayStartTime == 0) {
        delayStartTime = millis();

        /* LWT Topic */
        char _mqttSting[37];
        sprintf(_mqttSting, "%s/lwt", MQTTConfiguration.mqtt.topic);

        if (Broker.connect(deviceName, MQTTConfiguration.user,
                           MQTTConfiguration.password, _mqttSting, 2, false,
                           "disconnected")) {

          sprintf(_mqttSting, "%s/#", MQTTConfiguration.mqtt.topic);
#ifdef DEBUG
          Serial << endl
                 << "MQTT: Connected" << endl
                 << "MQTT: Subscribing to : " << _mqttSting;
#endif

          Broker.subscribe((char *)_mqttSting);

#ifdef DEBUG
          Serial << endl << "MQTT: Subsribed";
#endif

          eventConnectionEstablished = true;
          delayStartTime = 0;
#ifdef CONFIG_HARDWARE_LED
          ledStartTime = 0;
          Led.off();
#endif
          connections = 0;

          return;
        }
      }
#ifdef CONFIG_HARDWARE_LED
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
#ifdef CONFIG_HARDWARE_LED
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
    char _mqttTopic[83];
    sprintf(_mqttTopic, "%s/%s", MQTTConfiguration.mqtt.topic, subTopic);
#ifdef DEBUG
    Serial << endl << endl << "----------- Publish MQTT -----------";
    Serial << endl << "Topic: " << _mqttTopic;
    Serial << endl << "Message: " << message;
    Serial << endl << "------------------------------------";
#endif
    Broker.publish(_mqttTopic, message);
  }
}

void AFEMQTT::publishTopic(const char *subTopic, const char *type,
                           const char *message) {
  char _mqttTopic[50];
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
