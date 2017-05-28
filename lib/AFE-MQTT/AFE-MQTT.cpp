#include "AFE-MQTT.h"
#include "AFE-MQTT-callback.cpp"

AFEMQTT::AFEMQTT() {

}

void AFEMQTT::begin(
  const char * domain,
  uint16_t port,
  const char* user,
  const char* password,
  const char* subscribeTo) {

  Serial << endl << "INFO: Initializaing MQTT: ";
  mqttUser = user;
  mqttPassword = password;
//  sprintf(mqttTopicForSubscription, "%s#", subscribeTo);
  mqttTopicForSubscription = (char*)subscribeTo;
  Broker.setClient(esp);
  Broker.setServer(domain, port);
  Broker.setCallback(callbackMQTT);
  Serial << "done";
}


void AFEMQTT::connect() {

 if (sleepMode) {
   if (millis() - sleepStartTime >= durationBetweenNextConnectionAttemptsSeries*1000) {
      sleepMode = false;
   }
 } else {

  char  mqttString[60] = "Device Name";
  uint8_t connections = 0;

//  sprintf(mqttString, "Sonoff (Device name: %s)", Configuration.device_name);
  Serial << endl << "INFO: Connecting to MQTT";

  while (!Broker.connected()) {
    if (Broker.connect(mqttString, mqttUser, mqttPassword)) {
      Serial << endl << "INFO: Connected";
      Serial << endl << "INFO: Subscribing to : " << mqttTopicForSubscription;
      Broker.subscribe((char*)mqttTopicForSubscription);
      Serial << endl << "INFO: Subsribed";
    } else {
      connections++;
      Serial << endl << "INFO: MQTT Connection attempt: " << connections << " from " << noConnectionAttempts;
      if (connections >= noConnectionAttempts) {
        sleepMode = true;
        sleepStartTime = millis();
        break;
      }
        delay(durationBetweenConnectionAttempts*1000);
        Serial << ".";
    }
  }
  Serial << endl << "INFO: MQTT connection status: " << Broker.state();
  }
}

boolean AFEMQTT::connected() {
  return Broker.connected();
}

void  AFEMQTT::loop() {
  Broker.loop();
}

void AFEMQTT::publish(const char* type, const char* message) {
    char _mqttTopic[50];
    sprintf(_mqttTopic, "%s%s", mqttTopic, type);
    publishToMQTTBroker(_mqttTopic, message);
}

void AFEMQTT::publish(const char* topic, const char* type, const char* message) {
  char _mqttTopic[50];
  sprintf(_mqttTopic, "%s%s", topic, type);
  publishToMQTTBroker(_mqttTopic, message);
}


void AFEMQTT::publishToMQTTBroker(const char* topic, const char* message) {
  if (Broker.state() == MQTT_CONNECTED) {
    Serial << endl << "INFO: MQTT publising:  " << topic << "  \\ " << message;
    Broker.publish(topic, message);
  } else {
    Serial << endl << "WARN: MQTT not connected. State:  " << Broker.state();
  }
}
