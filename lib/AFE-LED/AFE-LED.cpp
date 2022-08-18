/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-LED.h"

#ifdef AFE_CONFIG_HARDWARE_LED

AFELED::AFELED() {}

boolean AFELED::begin(AFEDataAccess *Data, uint8_t id) {
  if (id != AFE_HARDWARE_ITEM_NOT_EXIST) {
    Data->getConfiguration(id, configuration);
#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
    // If MCP23017 available in the HW, checking if LED connected using MCP23017
    if (configuration->gpio == AFE_HARDWARE_ITEM_NOT_EXIST) {
      if (configuration->mcp23017.gpio != AFE_HARDWARE_ITEM_NOT_EXIST &&
          configuration->mcp23017.id != AFE_HARDWARE_ITEM_NOT_EXIST) {

#ifdef DEBUG
        Serial << endl << F("INFO: LED: Initializing with MCP23017");
#endif
        _MCP23017Broker->MCP[configuration->mcp23017.id].pinMode(configuration->mcp23017.gpio,
                                                  OUTPUT);
        _expanderUsed = true;
        _initialized = true;
      }
#ifdef DEBUG
      else {
        Serial << endl << F("WARN: LED: GPIO and MCP23017 not set");
      }
#endif
    } else {
#endif // AFE_CONFIG_HARDWARE_MCP23XXX

      pinMode(configuration->gpio, OUTPUT);
      configuration->changeToOppositeValue
          ? digitalWrite(configuration->gpio, LOW)
          : digitalWrite(configuration->gpio, HIGH);

      _initialized = true;

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
    }
#endif // AFE_CONFIG_HARDWARE_MCP23XXX
  }
  return _initialized;
}

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
void AFELED::addMCP23017Reference(AFEMCP23017Broker *MCP23017Broker) {
#ifdef DEBUG
        Serial << endl << F("INFO: LED: Reference added to MCP23017");
#endif
    _MCP23017Broker = MCP23017Broker;
}
#endif

void AFELED::on() {
  if (configuration->changeToOppositeValue) {
    set(HIGH);
  } else {
    set(LOW);
  }
}

void AFELED::off() {
  if (configuration->changeToOppositeValue) {
    set(LOW);
  } else {
    set(HIGH);
  }
}

void AFELED::blink(unsigned int duration) {
  on();
  delay(duration);
  off();
}

void AFELED::blinkingOn(unsigned long blinking_interval) {
  _interval = blinking_interval;
  _blinking = true;
}

void AFELED::blinkingOff() { _blinking = false; }

boolean AFELED::isBlinking() { return _blinking; }

void AFELED::loop() {
  if (_initialized && _blinking) {
    unsigned long currentMillis = millis();
    if (currentMillis - _previousMillis >= _interval) {
      _previousMillis = currentMillis;
      toggle();
    }
  }
}

void AFELED::set(uint8_t state) {
  if (_initialized) {

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
    if (_expanderUsed) {
      if (_MCP23017Broker->MCP[configuration->mcp23017.id].digitalRead(
              configuration->mcp23017.gpio) != state) {
        _MCP23017Broker->MCP[configuration->mcp23017.id].digitalWrite(
            configuration->mcp23017.gpio, state);
      }
    } else {
#endif

      if (digitalRead(configuration->gpio) != state) {
        digitalWrite(configuration->gpio, state);
      }

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
    }
#endif
  }
}

void AFELED::toggle() {
  if (_initialized) {

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
    if (_expanderUsed) {
      _MCP23017Broker->MCP[configuration->mcp23017.id].digitalRead(
          configuration->mcp23017.gpio) == HIGH
          ? _MCP23017Broker->MCP[configuration->mcp23017.id].digitalWrite(
                configuration->mcp23017.gpio, LOW)
          : _MCP23017Broker->MCP[configuration->mcp23017.id].digitalWrite(
                configuration->mcp23017.gpio, HIGH);
    } else {
#endif

      digitalRead(configuration->gpio) == HIGH
          ? digitalWrite(configuration->gpio, LOW)
          : digitalWrite(configuration->gpio, HIGH);

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
    }
#endif
  }
}

#endif // AFE_CONFIG_HARDWARE_LED