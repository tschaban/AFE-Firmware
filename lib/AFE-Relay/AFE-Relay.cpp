/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Relay.h"

#ifdef AFE_CONFIG_HARDWARE_RELAY

AFERelay::AFERelay() {}

void AFERelay::initialize(AFEDataAccess *Data, uint8_t id) {
  _id = id;
  _Data = Data;
  _Data->getConfiguration(_id, &configuration);
}

void AFERelay::begin() {
  if (_id != AFE_HARDWARE_ITEM_NOT_EXIST) {

#ifdef AFE_CONFIG_HARDWARE_MCP23017
    // If MCP23017 available in the HW, checking if LED connected using MCP23017
    if (configuration.gpio == AFE_HARDWARE_ITEM_NOT_EXIST) {
      if (configuration.mcp23017.gpio != AFE_HARDWARE_ITEM_NOT_EXIST &&
          configuration.mcp23017.address !=
              AFE_CONFIG_HARDWARE_I2C_DEFAULT_ADDRESS) {

        if (_MCP23017ReferenceAdded) {
#ifdef DEBUG
          Serial << endl << F("INFO: Relay: Initializing with MCP23017");
#endif
          mcp->pinMode(configuration.mcp23017.gpio, OUTPUT);
          _expanderUsed = true;
        }
#ifdef DEBUG
        else {
          Serial << endl << F("WARN: Reference to MCP23017 has not been added");
        }
#endif

      }
#ifdef DEBUG
      else {
        Serial << endl << F("WARN: RELAY: GPIO and MCP23017 not set");
      }
#endif
    } else {
#endif // AFE_CONFIG_HARDWARE_MCP23017

      pinMode(configuration.gpio, OUTPUT);

#ifdef AFE_CONFIG_HARDWARE_MCP23017
    }
#endif // AFE_CONFIG_HARDWARE_MCP23017

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
    /* Defining get and state MQTT Topics */
    if (strlen(configuration.mqtt.topic) > 0) {
      sprintf(mqttCommandTopic, "%s/cmd", configuration.mqtt.topic);
    } else {
      mqttCommandTopic[0] = AFE_EMPTY_STRING;
    }

    if (strlen(configuration.mqtt.topic) > 0) {
      sprintf(mqttStateTopic, "%s/state", configuration.mqtt.topic);
    } else {
      mqttStateTopic[0] = AFE_EMPTY_STRING;
    }
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

#ifdef AFE_CONFIG_HARDWARE_LED
    if (configuration.ledID != AFE_HARDWARE_ITEM_NOT_EXIST) {
      // @TODO this code doesn't check if the LED is actually set in Device
      // config
      // https://github.com/tschaban/AFE-Firmware/issues/606
      Led.begin(_Data, configuration.ledID);
    }

#endif
  }
}

#ifdef AFE_CONFIG_HARDWARE_MCP23017
void AFERelay::addMCP23017Reference(Adafruit_MCP23017 *_mcp) {
  mcp = _mcp;
  _MCP23017ReferenceAdded = true;
}
#endif

byte AFERelay::get() {
  byte state;

#ifdef AFE_CONFIG_HARDWARE_MCP23017
  if (_expanderUsed) {
    state = mcp->digitalRead(configuration.mcp23017.gpio);
  } else {
#endif

    state = digitalRead(configuration.gpio);

#ifdef AFE_CONFIG_HARDWARE_MCP23017
  }
#endif

  return state == HIGH
             ? (configuration.triggerSignal == AFE_RELAY_SIGNAL_TRIGGER_HIGH
                    ? AFE_RELAY_ON
                    : AFE_RELAY_OFF)
             : (configuration.triggerSignal == AFE_RELAY_SIGNAL_TRIGGER_HIGH
                    ? AFE_RELAY_OFF
                    : AFE_RELAY_ON);
}

/* Set relay to ON */
void AFERelay::on() {
#ifdef DEBUG
  Serial << endl
         << "INFO: Relay: ON, Trigger by "
         << (configuration.triggerSignal == AFE_RELAY_SIGNAL_TRIGGER_HIGH
                 ? "HIGH"
                 : "LOW")
         << " signal";
#endif

// if (get() == AFE_RELAY_OFF) {

#ifdef AFE_CONFIG_HARDWARE_MCP23017
  if (_expanderUsed) {
    mcp->digitalWrite(
        configuration.mcp23017.gpio,
        configuration.triggerSignal == AFE_RELAY_SIGNAL_TRIGGER_HIGH ? HIGH
                                                                     : LOW);
  } else {
#endif

    digitalWrite(configuration.gpio,
                 configuration.triggerSignal == AFE_RELAY_SIGNAL_TRIGGER_HIGH
                     ? HIGH
                     : LOW);

#ifdef AFE_CONFIG_HARDWARE_MCP23017
  }
#endif

  if (configuration.timeToOff >
      0) { // Start counter if relay should be automatically turned off
    turnOffCounter = millis();
  }
//}

#ifdef AFE_CONFIG_HARDWARE_GATE
  /* For the Relay assigned to a gate state is saved conditionally */
  if (gateId == AFE_HARDWARE_ITEM_NOT_EXIST) {
    _Data->saveRelayState(_id, AFE_RELAY_ON);
  };
#else
  _Data->saveRelayState(_id, AFE_RELAY_ON);
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
  Led.toggle();
#endif
}

/* Set relay to OFF */
void AFERelay::off() {
#ifdef DEBUG
  Serial << endl
         << "INFO: Relay: OFF, Trigger by "
         << (configuration.triggerSignal == AFE_RELAY_SIGNAL_TRIGGER_HIGH
                 ? "HIGH"
                 : "LOW")
         << " signal";
#endif

// if (get() == AFE_RELAY_ON) {

#ifdef AFE_CONFIG_HARDWARE_MCP23017
  if (_expanderUsed) {
    mcp->digitalWrite(
        configuration.mcp23017.gpio,
        configuration.triggerSignal == AFE_RELAY_SIGNAL_TRIGGER_HIGH ? LOW
                                                                     : HIGH);
  } else {
#endif

    digitalWrite(configuration.gpio,
                 configuration.triggerSignal == AFE_RELAY_SIGNAL_TRIGGER_HIGH
                     ? LOW
                     : HIGH);

#ifdef AFE_CONFIG_HARDWARE_MCP23017
  }
#endif

  if (configuration.timeToOff >
      0) { // Start counter if relay should be automatically turned off
    turnOffCounter = millis();
  }
// }
#ifdef AFE_CONFIG_HARDWARE_GATE
  /* For the Relay assigned to a gate state is saved conditionally */
  if (gateId == AFE_HARDWARE_ITEM_NOT_EXIST) {
    _Data->saveRelayState(_id, AFE_RELAY_OFF);
  };
#else
  _Data->saveRelayState(_id, AFE_RELAY_OFF);
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
  Led.toggle();
#endif
}

/* Toggle relay */
void AFERelay::toggle() {
  if (get() == AFE_RELAY_OFF) {
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
    _Data->getRelayState(_id) == AFE_RELAY_ON ? on() : off();
  } else if (option == 4) {
    _Data->getRelayState(_id) == AFE_RELAY_ON ? off() : on();
  }
}

#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
boolean AFERelay::autoTurnOff() {
  if (configuration.timeToOff > 0 &&
      millis() - turnOffCounter >=
          configuration.timeToOff * (timerUnitInSeconds ? 1000 : 1) &&
      get() == AFE_RELAY_ON) {
    off();
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

#endif // AFE_CONFIG_HARDWARE_RELAY