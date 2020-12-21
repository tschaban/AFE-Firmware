/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Switch.h"

#ifdef AFE_CONFIG_HARDWARE_SWITCH

AFESwitch::AFESwitch(){};

#ifdef AFE_CONFIG_HARDWARE_LED
void AFESwitch::begin(uint8_t id, AFEDataAccess *_Data, AFELED *_LED) {
  Led = _LED;
  begin(id, _Data);
}
#endif // AFE_CONFIG_HARDWARE_LED

void AFESwitch::begin(uint8_t id, AFEDataAccess *_Data) {
  _Data->getConfiguration(id, &configuration);

#ifdef AFE_CONFIG_HARDWARE_MCP23017
  // If MCP23017 available in the HW, checking if LED connected using MCP23017
  if (configuration.gpio == AFE_HARDWARE_ITEM_NOT_EXIST) {
    if (configuration.mcp23017.gpio != AFE_HARDWARE_ITEM_NOT_EXIST &&
        configuration.mcp23017.address !=
            AFE_CONFIG_HARDWARE_I2C_DEFAULT_ADDRESS) {

      if (_MCP23017ReferenceAdded) {

        _MCP23017Id = _MCP23017Broker->getId(configuration.mcp23017.address);
        if (_MCP23017Id != AFE_HARDWARE_ITEM_NOT_EXIST) {
#ifdef DEBUG
          Serial << endl << F("INFO: SWITCH: Initializing with MCP23017");
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH_GPIO_DIGIT_INPUT
          _MCP23017Broker->MCP[_MCP23017Id].pinMode(configuration.mcp23017.gpio,
                                                    INPUT);
#else
          _MCP23017Broker->MCP[_MCP23017Id].pinMode(configuration.mcp23017.gpio,
                                                    INPUT_PULLUP);
#endif
          state =
              _MCP23017Broker->MCP[_MCP23017Id].digitalRead(configuration.gpio);

          _expanderUsed = true;

        }
#ifdef DEBUG
        else {
          Serial << endl
                 << F("WARN: SWITCH: MCP23017[0x")
                 << _HEX(configuration.mcp23017.address)
                 << F("] not found in cache");
        }
#endif
      }
#ifdef DEBUG
      else {
        Serial << endl
               << F("WARN: SWITCH: Reference to MCP23017 has not been added");
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

#ifdef AFE_CONFIG_HARDWARE_SWITCH_GPIO_DIGIT_INPUT
    pinMode(configuration.gpio, INPUT);
#else
  pinMode(configuration.gpio, INPUT_PULLUP);
#endif
    state = digitalRead(configuration.gpio);

#ifdef AFE_CONFIG_HARDWARE_MCP23017
  }
#endif // AFE_CONFIG_HARDWARE_MCP23017

  previousState = state;
  phisicallyState = state;

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

  _initialized = true;
}

#ifdef AFE_CONFIG_HARDWARE_MCP23017
void AFESwitch::addMCP23017Reference(AFEMCP23017Broker *MCP23017Broker) {
  _MCP23017Broker = MCP23017Broker;
  _MCP23017ReferenceAdded = true;
}
#endif

boolean AFESwitch::getState() { return state; }

boolean AFESwitch::getPhisicalState() { return phisicallyState; }

boolean AFESwitch::isPressed(boolean phisically) {
  if (phisically) {
    if (phisicallyPressed) {
      phisicallyPressed = false;
      return true;
    } else {
      return false;
    }
  } else {
    if (pressed) {
      pressed = false;
      return true;
    } else {
      return false;
    }
  }
}

boolean AFESwitch::is5s() {
  if (pressed4fiveSeconds) {
    pressed4fiveSeconds = false;
    return true;
  } else {
    return false;
  }
}

boolean AFESwitch::is10s() {
  if (pressed4tenSeconds) {
    pressed4tenSeconds = false;
    return true;
  } else {
    return false;
  }
}

boolean AFESwitch::is30s() {
  if (pressed4thirteenSeconds) {
    pressed4thirteenSeconds = false;
    return true;
  } else {
    return false;
  }
}

void AFESwitch::listener() {
  if (_initialized) {
    boolean currentState;
#ifdef AFE_CONFIG_HARDWARE_MCP23017
    if (_expanderUsed) {
      currentState =
          _MCP23017Broker->MCP[_MCP23017Id].digitalRead(configuration.gpio);
    } else {
#endif
      currentState = digitalRead(configuration.gpio);

#ifdef AFE_CONFIG_HARDWARE_MCP23017
    }
#endif

    unsigned long time = millis();

    if (currentState != previousState) { // buttons has been pressed

      if (startTime == 0) { // starting timer. used for switch sensitiveness
        startTime = time;
      }

      if (time - startTime >=
          configuration.sensitiveness) { // switch prssed, sensitiveness
                                         // taken into account, processing
                                         // event
        if (configuration.type == AFE_SWITCH_TYPE_MONO) {

          if (!_pressed) { // This is set only once when switch is pressed
            state = !state;
            _pressed = true;
            pressed = true;
            phisicallyPressed = true;
            phisicallyState = currentState;
          }

          /* Code only for Mulitifunction switch: pressed for 5 and 10 seconds
           */
          if (configuration.functionality == AFE_SWITCH_FUNCTIONALITY_MULTI) {

#ifdef AFE_CONFIG_HARDWARE_LED
            if (time - startTime >= 35000) {
              Led->blink(50);
              delay(50);
            }
#endif
            if (time - startTime >= 30000 && !_pressed4thirteenSeconds) {
#ifdef AFE_CONFIG_HARDWARE_LED
              for (uint8_t i = 0; i < 3; i++) {
                Led->blink(200);
                delay(200);
              }
#endif
              _pressed4thirteenSeconds = true;
            }

            if (time - startTime >= 10000 && !_pressed4tenSeconds) {
#ifdef AFE_CONFIG_HARDWARE_LED
              for (uint8_t i = 0; i < 2; i++) {
                Led->blink(200);
                delay(200);
              }
#endif
              _pressed4tenSeconds = true;
            }

            if (time - startTime >= 5000 && !_pressed4fiveSeconds) {
#ifdef AFE_CONFIG_HARDWARE_LED
              Led->blink(200);
#endif
              _pressed4fiveSeconds = true;
            }
          }
        } else { // This is BI-stable code
          state = !state;
          previousState = currentState;
          pressed = true;
          phisicallyPressed = true;
          phisicallyState = currentState;
        }
      }

      //  Serial << endl << F("press=") << pressed << F(" _press=") << _pressed;

    } else if (currentState == previousState && startTime > 0 &&
               configuration.type == AFE_SWITCH_TYPE_MONO) {
      /* Code only for Mulitifunction switch: pressed for 5 and 10 seconds */
      if (configuration.functionality == AFE_SWITCH_FUNCTIONALITY_MULTI) {

        if (time - startTime >= 5000 && time - startTime < 10000) {
          pressed4fiveSeconds = true;
        }

        if (time - startTime >= 10000 && time - startTime < 30000) {
          pressed4tenSeconds = true;
        }

        if (time - startTime >= 30000 && time - startTime < 35000) {
          pressed4thirteenSeconds = true;
        }

        _pressed4fiveSeconds = false;
        _pressed4tenSeconds = false;
        _pressed4thirteenSeconds = false;
      }

      startTime = 0;
      _pressed = false;

      phisicallyPressed = true;
      phisicallyState = currentState;
    }
  }
}

#endif // AFE_CONFIG_HARDWARE_SWITCH