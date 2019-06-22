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

#ifdef CONFIG_FUNCTIONALITY_THERMOSTAT
  /* Initialzing Thermostat functionality for a relay */
  Thermostat.begin(RelayConfiguration.thermostat);
#endif

#ifdef CONFIG_FUNCTIONALITY_THERMAL_PROTECTION
  /* Initialzing thermal protection functionality for a relay */
  ThermalProtection.begin(RelayConfiguration.thermalProtection);
#endif

#ifdef CONFIG_FUNCTIONALITY_HUMIDISTAT
  Humidistat.begin(RelayConfiguration.humidistat);
#endif

#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
  if (RelayConfiguration.ledID > 0) {
    Led.begin(RelayConfiguration.ledID - 1);
  }
#endif
}

const char *AFERelay::getMQTTCommandTopic() {
  if (strlen(RelayConfiguration.mqtt.topic) > 0) {
    sprintf(mqttCommandTopic, "%s/cmd", RelayConfiguration.mqtt.topic);
  } else {
    mqttCommandTopic[0] = '\0';
  }
  return mqttCommandTopic;
}

const char *AFERelay::getMQTTStateTopic() {
  if (strlen(RelayConfiguration.mqtt.topic) > 0) {
    sprintf(mqttStateTopic, "%s/state", RelayConfiguration.mqtt.topic);
  } else {
    mqttStateTopic[0] = '\0';
  }
  return mqttStateTopic;
}

byte AFERelay::get() {
  return digitalRead(RelayConfiguration.gpio) == HIGH ? RELAY_ON : RELAY_OFF;
}

/* Set relay to ON */
void AFERelay::on(boolean invert) {
  if (get() == RELAY_OFF) {
    digitalWrite(RelayConfiguration.gpio, HIGH);
#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
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
#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
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

#ifdef CONFIG_FUNCTIONALITY_RELAY
void AFERelay::setRelayAfterRestoringPower() {
  setRelayAfterRestore(RelayConfiguration.state.powerOn);
}

boolean AFERelay::setRelayAfterRestoringMQTTConnection() {
  if (RelayConfiguration.state.MQTTConnected ==
      5) { // request state from MQTT Broker
    return false;
  } else {
    setRelayAfterRestore(RelayConfiguration.state.MQTTConnected);
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

#ifdef CONFIG_RELAY_AUTOONOFF_LISTENER
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
#endif

#ifdef CONFIG_FUNCTIONALITY_RELAY
const char *AFERelay::getName() { return RelayConfiguration.name; }
#endif

#ifdef CONFIG_FUNCTIONALITY_RELAY_CONTROL_AUTOONOFF_TIME
void AFERelay::setTimer(float timer) {
  if (RelayConfiguration.timeToOff > 0) {
    turnOffCounter = millis();
  } else {
    RelayConfiguration.timeToOff = timer;
  }
}
#endif

#ifdef CONFIG_HARDWARE_PIR
void AFERelay::clearTimer() { RelayConfiguration.timeToOff = 0; }
#endif

#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
uint8_t AFERelay::getControlledLedID() { return RelayConfiguration.ledID; }
#endif

#ifdef CONFIG_FUNCTIONALITY_GATE
void AFERelay::setTimerUnitToSeconds(boolean value) {
  timerUnitInSeconds = value;
}
#endif

#ifdef CONFIG_FUNCTIONALITY_RELAY
unsigned long AFERelay::getDomoticzIDX() {
  return RelayConfiguration.domoticz.idx;
}
#endif
