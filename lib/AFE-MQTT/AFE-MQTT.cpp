#include "AFE-MQTT.h"

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
  mqttTopicForSubscription = subscribeTo;
  Broker.setClient(esp);
  Broker.setServer(domain, port);
//  Broker.setCallback(callbackMQTT);
  Serial << "done";
}


void AFEMQTT::connect() {
  char  mqttString[60] = "Device Name";
  uint8_t connection_try = 0;

//  sprintf(mqttString, "Sonoff (Device name: %s)", Configuration.device_name);
  Serial << endl << "INFO: Connecting to MQTT";

  while (!Broker.connected()) {
    if (Broker.connect(mqttString, mqttUser, mqttPassword)) {
      Serial << endl << "INFO: Connected";
    //  sprintf(mqttString, "%scmd", Configuration.mqtt_topic);
      Serial << endl << "INFO: Subscribing to : " << mqttTopicForSubscription;
      Broker.subscribe(mqttTopicForSubscription);
      Serial << endl << "INFO: Subsribed";
    } else {
  //    connection_try++;
  //    Serial << endl << "INFO: MQTT Connection attempt: " << connection_try << " from " << Configuration.number_connection_attempts;
  //    if (connection_try == Configuration.number_connection_attempts) {
  //      Sonoff.runSleepMode();
  //      break;
  //    }
//      delay(Configuration.duration_between_connection_attempts*1000);
        delay(1000);
        Serial << ".";
    }
  }
  Serial << endl << "INFO: MQTT connection status: " << Broker.state();
}

boolean AFEMQTT::connected() {
  return Broker.connected();
}
