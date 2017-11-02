#include "AFE-Relay.h"

AFERelay::AFERelay(){};

AFERelay::AFERelay(uint8_t id) { begin(id); }

void AFERelay::begin(uint8_t id) {
  MQTT MQTTConfiguration;
  MQTTConfiguration = Data.getMQTTConfiguration();

  RelayConfiguration = Data.getRelayConfiguration(id);
  pinMode(RelayConfiguration.gpio, OUTPUT);

  sprintf(mqttTopic, "%s%s/", MQTTConfiguration.topic, RelayConfiguration.name);

  Serial << endl << "INFO: Relay #" << id << " has been initialized";
  Serial << endl << "INFO: Relay #" << id << " MQTT Topic: " << mqttTopic;
}

const char *AFERelay::getMQTTTopic() { return RelayConfiguration.mqttTopic; }

byte AFERelay::get() {
  return digitalRead(RelayConfiguration.gpio) == HIGH ? RELAY_ON : RELAY_OFF;
}

/* Set relay to ON */
void AFERelay::on() {
  if (get() == RELAY_OFF) {
    digitalWrite(RelayConfiguration.gpio, HIGH);
    Data.saveRelayState(1, true);
    Serial << endl << "INFO: Relay set to ON";
  }
}

/* Set relay to OFF */
void AFERelay::off() {
  if (get() == RELAY_ON) {
    digitalWrite(RelayConfiguration.gpio, LOW);
    Data.saveRelayState(1, false);
    Serial << endl << "INFO: Relay set to OFF";
  }
}

/* Toggle relay */
void AFERelay::toggle() {
  if (digitalRead(RelayConfiguration.gpio) == LOW) {
    on();
  } else {
    off();
  }
}
