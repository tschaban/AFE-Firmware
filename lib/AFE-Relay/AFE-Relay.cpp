/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Relay.h"

AFERelay::AFERelay() {}

AFERelay::AFERelay(uint8_t id) { begin(id); }

void AFERelay::begin(uint8_t id) {
  _id = id;
   Data.getConfiguration(_id,&configuration);

  pinMode(configuration.gpio, OUTPUT);

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMOSTAT
  /* Initialzing Thermostat functionality for a relay */
  Thermostat.begin(configuration.thermostat);
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTION
  /* Initialzing thermal protection functionality for a relay */
  ThermalProtection.begin(configuration.thermalProtection.temperature);
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_HUMIDISTAT
  Humidistat.begin(configuration.humidistat);
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
  if (configuration.ledID != AFE_HARDWARE_ITEM_NOT_EXIST) {
    Led.begin(configuration.ledID);
  }
#endif

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  /* Defining get and state MQTT Topics */
  if (strlen(configuration.mqtt.topic) > 0) {
    sprintf(mqttCommandTopic, "%s/cmd", configuration.mqtt.topic);
  } else {
    mqttCommandTopic[0] = '\0';
  }

  if (strlen(configuration.mqtt.topic) > 0) {
    sprintf(mqttStateTopic, "%s/state", configuration.mqtt.topic);
  } else {
    mqttStateTopic[0] = '\0';
  }
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
}

byte AFERelay::get() {
  return digitalRead(configuration.gpio) == HIGH ? AFE_RELAY_ON : AFE_RELAY_OFF;
}

/* Set relay to ON */
void AFERelay::on(boolean invert) {

#ifdef DEBUG
  Serial << endl << F("INFO: Relay: ON, inverted: ") << (invert ? F("YES") : F("NO"));
#endif

  if (get() == AFE_RELAY_OFF) {
    digitalWrite(configuration.gpio, HIGH);
#ifdef AFE_CONFIG_HARDWARE_LED
    Led.on();
#endif
    if (!invert &&
        configuration.timeToOff >
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

#ifdef DEBUG
  Serial << endl << F("INFO: Relay: OFF, inverted: ") << (invert ? F("YES") : F("NO"));
#endif

  if (get() == AFE_RELAY_ON) {
    digitalWrite(configuration.gpio, LOW);
#ifdef AFE_CONFIG_HARDWARE_LED
    Led.off();
#endif
    if (invert &&
        configuration.timeToOff >
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
  if (digitalRead(configuration.gpio) == LOW) {
    on();
  } else {
    off();
  }
}

void AFERelay::setRelayAfterRestoringPower() {
  setRelayAfterRestore(configuration.state.powerOn);
}

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
boolean AFERelay::setRelayAfterRestoringMQTTConnection() {
  if (configuration.state.MQTTConnected ==
      5) { // request state from MQTT Broker
    return false;
  } else {
    setRelayAfterRestore(configuration.state.MQTTConnected);
    return true;
  }
}
#endif

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
  if (configuration.timeToOff > 0 && ((invert && get() == AFE_RELAY_OFF) ||
                                      (!invert && get() == AFE_RELAY_ON)) &&
      millis() - turnOffCounter >=
          configuration.timeToOff * (timerUnitInSeconds ? 1000 : 1)) {
    invert ? on(invert) : off(invert);
    return true;
  } else {
    return false;
  }
}
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY_CONTROL_AUTOONOFF_TIME
void AFERelay::setTimer(float timer) {
  if (configuration.timeToOff > 0) {
    turnOffCounter = millis();
  } else {
    configuration.timeToOff = timer;
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_PIR
void AFERelay::clearTimer() { configuration.timeToOff = 0; }
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
void AFERelay::setTimerUnitToSeconds(boolean value) {
  timerUnitInSeconds = value;
}
#endif
