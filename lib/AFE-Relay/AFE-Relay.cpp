/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Relay.h"

AFERelay::AFERelay() {}

AFERelay::AFERelay(uint8_t id) { begin(id); }

void AFERelay::begin(uint8_t id) {
  _id = id;
  RelayConfiguration = Data.getRelayConfiguration(_id);

  pinMode(RelayConfiguration.gpio, OUTPUT);

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMOSTAT
  /* Initialzing Thermostat functionality for a relay */
  Thermostat.begin(RelayConfiguration.thermostat);
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTION
  /* Initialzing thermal protection functionality for a relay */
  ThermalProtection.begin(RelayConfiguration.thermalProtection);
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_HUMIDISTAT
  Humidistat.begin(RelayConfiguration.humidistat);
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
  if (RelayConfiguration.ledID != AFE_HARDWARE_ITEM_NOT_EXIST) {
    Led.begin(RelayConfiguration.ledID);
  }
#endif
}

byte AFERelay::get() {
  return digitalRead(RelayConfiguration.gpio) == HIGH ? AFE_RELAY_ON : AFE_RELAY_OFF;
}

/* Set relay to ON */
void AFERelay::on(boolean invert) {
  if (get() == AFE_RELAY_OFF) {
    digitalWrite(RelayConfiguration.gpio, HIGH);
#ifdef AFE_CONFIG_HARDWARE_LED
    Led.on();
#endif
    if (!invert &&
        RelayConfiguration.timeToOff >
            0) { // Start counter if relay should be automatically turned off
      turnOffCounter = millis();
    }
  }

#ifdef AFE_CONFIG_HARDWARE_GATE
  /* For the Relay assigned to a gate state is saved conditionally */
  if (gateId == AFE_HARDWARE_ITEM_NOT_EXIST) {
    Data.saveRelayState(_id, AFE_RELAY_ON);
  };
#else
  Data.saveRelayState(_id, AFE_RELAY_ON);
#endif
}

/* Set relay to OFF */
void AFERelay::off(boolean invert) {
  if (get() == AFE_RELAY_ON) {
    digitalWrite(RelayConfiguration.gpio, LOW);
#ifdef AFE_CONFIG_HARDWARE_LED
    Led.off();
#endif
    if (invert &&
        RelayConfiguration.timeToOff >
            0) { // Start counter if relay should be automatically turned off
      turnOffCounter = millis();
    }
  }
#ifdef AFE_CONFIG_HARDWARE_GATE
  /* For the Relay assigned to a gate state is saved conditionally */
  if (gateId == AFE_HARDWARE_ITEM_NOT_EXIST) {
    Data.saveRelayState(_id, AFE_RELAY_OFF);
  };
#else
  Data.saveRelayState(_id, AFE_RELAY_OFF);
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
    Data.getRelayState(_id) == AFE_RELAY_ON ? on() : off();
  } else if (option == 4) {
    Data.getRelayState(_id) == AFE_RELAY_ON ? off() : on();
  }
}

#ifdef AFE_CONFIG_RELAY_AUTOONOFF_LISTENER
boolean AFERelay::autoTurnOff(boolean invert) {
  if (RelayConfiguration.timeToOff > 0 &&
      ((invert && get() == AFE_RELAY_OFF) || (!invert && get() == AFE_RELAY_ON)) &&
      millis() - turnOffCounter >=
          RelayConfiguration.timeToOff * (timerUnitInSeconds ? 1000 : 1)) {
    invert ? on(invert) : off(invert);
    return true;
  } else {
    return false;
  }
}
#endif

const char *AFERelay::getName() { return RelayConfiguration.name; }

#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY_CONTROL_AUTOONOFF_TIME
void AFERelay::setTimer(float timer) {
  if (RelayConfiguration.timeToOff > 0) {
    turnOffCounter = millis();
  } else {
    RelayConfiguration.timeToOff = timer;
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_PIR
void AFERelay::clearTimer() { RelayConfiguration.timeToOff = 0; }
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
uint8_t AFERelay::getControlledLedID() { return RelayConfiguration.ledID; }
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
void AFERelay::setTimerUnitToSeconds(boolean value) {
  timerUnitInSeconds = value;
}
#endif

unsigned long AFERelay::getDomoticzIDX() {
  return RelayConfiguration.domoticz.idx;
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
