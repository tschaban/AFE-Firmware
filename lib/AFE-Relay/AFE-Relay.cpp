/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#include "AFE-Relay.h"

AFERelay::AFERelay(){};

AFERelay::AFERelay(uint8_t id) { begin(id); }

void AFERelay::begin(uint8_t id) {
  _id = id;
  RelayConfiguration = Data.getRelayConfiguration(_id);

  pinMode(RelayConfiguration.gpio, OUTPUT);

#if !defined(T5_CONFIG) // Not required for T5
  MQTT MQTTConfiguration;
  MQTTConfiguration = Data.getMQTTConfiguration();
  sprintf(mqttTopic, "%s%s/", MQTTConfiguration.topic, RelayConfiguration.name);
#endif

#if defined(T1_CONFIG) || defined(T2_CONFIG)
  /* Initialzing Thermostat functionality for a relay */
  Thermostat.begin(RelayConfiguration.thermostat);
  /* Initialzing thermal protection functionality for a relay */
  ThermalProtection.begin(RelayConfiguration.thermalProtection);
#endif

#if defined(T2_CONFIG)
  Humidistat.begin(RelayConfiguration.humidistat);
#endif

#ifndef T0_SHELLY_1_CONFIG
  if (RelayConfiguration.ledID > 0) {
    Led.begin(RelayConfiguration.ledID - 1);
  }
#endif
}

const char *AFERelay::getMQTTTopic() { return RelayConfiguration.mqttTopic; }

byte AFERelay::get() {
  return digitalRead(RelayConfiguration.gpio) == HIGH ? RELAY_ON : RELAY_OFF;
}

/* Set relay to ON */
void AFERelay::on(boolean invert) {
  if (get() == RELAY_OFF) {
    digitalWrite(RelayConfiguration.gpio, HIGH);
#ifndef T0_SHELLY_1_CONFIG
    Led.on();
#endif
    if (!invert &&
        RelayConfiguration.timeToOff >
            0) { // Start counter if relay should be automatically turned off
      turnOffCounter = millis();
    }
  }
#if !defined(T5_CONFIG) // Not required for T5
  Data.saveRelayState(_id, RELAY_ON);
#endif
}

/* Set relay to OFF */
void AFERelay::off(boolean invert) {
  if (get() == RELAY_ON) {
    digitalWrite(RelayConfiguration.gpio, LOW);
#ifndef T0_SHELLY_1_CONFIG
    Led.off();
#endif
    if (invert &&
        RelayConfiguration.timeToOff >
            0) { // Start counter if relay should be automatically turned off
      turnOffCounter = millis();
    }
  }
#if !defined(T5_CONFIG) // Not required for T5
  Data.saveRelayState(_id, RELAY_OFF);
#endif
}

/* Toggle relay */
void AFERelay::toggle() {
  if (digitalRead(RelayConfiguration.gpio) == LOW) {
    on();
  } else {
    off();
  }
}

#if !defined(T5_CONFIG) // Not required for T5
void AFERelay::setRelayAfterRestoringPower() {
  setRelayAfterRestore(RelayConfiguration.statePowerOn);
}

boolean AFERelay::setRelayAfterRestoringMQTTConnection() {
  if (RelayConfiguration.stateMQTTConnected ==
      5) { // request state from MQTT Broker
    return false;
  } else {
    setRelayAfterRestore(RelayConfiguration.stateMQTTConnected);
    return true;
  }
}

void AFERelay::setRelayAfterRestore(uint8_t option) {

  if (option == 1) {
    off();
  } else if (option == 2) {
    on();
  } else if (option == 3) {
    Data.getRelayState(_id) == RELAY_ON ? on() : off();
  } else if (option == 4) {
    Data.getRelayState(_id) == RELAY_ON ? off() : on();
  }
}
#endif

boolean AFERelay::autoTurnOff(boolean invert) {

  if (RelayConfiguration.timeToOff > 0 &&
      ((invert && get() == RELAY_OFF) || (!invert && get() == RELAY_ON)) &&
      millis() - turnOffCounter >=
          RelayConfiguration.timeToOff * (timerUnitInSeconds ? 1000 : 1)) {

    invert ? on(invert) : off(invert);
    return true;
  } else {
    return false;
  }
}

#if !defined(T5_CONFIG) // Not required for T5
const char *AFERelay::getName() { return RelayConfiguration.name; }
#endif

void AFERelay::setTimer(float timer) {
  if (RelayConfiguration.timeToOff > 0) {
    turnOffCounter = millis();
  } else {
    RelayConfiguration.timeToOff = timer;
  }
}

void AFERelay::clearTimer() { RelayConfiguration.timeToOff = 0; }

#ifndef T0_SHELLY_1_CONFIG
uint8_t AFERelay::getControlledLedID() { return RelayConfiguration.ledID; }
#endif

void AFERelay::setTimerUnitToSeconds(boolean value) {
  timerUnitInSeconds = value;
}

#if !defined(T5_CONFIG) // Not required for T5
unsigned long AFERelay::getDomoticzIDX() { return RelayConfiguration.idx; }
#endif
