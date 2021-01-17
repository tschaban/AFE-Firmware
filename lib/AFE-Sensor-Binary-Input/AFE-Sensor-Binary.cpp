/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Sensor-Binary.h"

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR

AFESensorBinary::AFESensorBinary(){};

void AFESensorBinary::begin(uint8_t id, AFEDataAccess *_Data) {
  _Data->getConfiguration(id, &configuration);

#ifdef AFE_CONFIG_HARDWARE_MCP23017
  if (configuration.gpio == AFE_HARDWARE_ITEM_NOT_EXIST) {
    if (configuration.mcp23017.gpio != AFE_HARDWARE_ITEM_NOT_EXIST &&
        configuration.mcp23017.address !=
            AFE_CONFIG_HARDWARE_I2C_DEFAULT_NON_EXIST_ADDRESS) {

      if (_MCP23017ReferenceAdded) {

        _MCP23017Id = _MCP23017Broker->getId(configuration.mcp23017.address);
        if (_MCP23017Id != AFE_HARDWARE_ITEM_NOT_EXIST) {
#ifdef DEBUG
          Serial << endl
                 << F("INFO: BINARY Sensor: Initializing with MCP23017");
#endif

          _MCP23017Broker->MCP[_MCP23017Id].pinMode(configuration.mcp23017.gpio,
                                                    INPUT);

          _MCP23017Broker->MCP[_MCP23017Id].pullUp(
              configuration.mcp23017.gpio,
              configuration.internalPullUp ? HIGH : LOW);

          state = _MCP23017Broker->MCP[_MCP23017Id].digitalRead(
              configuration.mcp23017.gpio);

#ifdef DEBUG
          Serial
              << endl
              << F("INFO: BINARY Sensor: initial state of MCP23017 connected "
                   "sensor: ")
              << state;
#endif

          _expanderUsed = true;

        }
#ifdef DEBUG
        else {
          Serial << endl
                 << F("WARN: BINARY Sensor: MCP23017[0x")
                 << _HEX(configuration.mcp23017.address)
                 << F("] not found in cache");
        }
#endif
      }
#ifdef DEBUG
      else {
        Serial << endl
               << F("WARN: BINARY Sensor: Reference to MCP23017 has not been "
                    "added");
      }
#endif

    }
#ifdef DEBUG
    else {
      Serial << endl << F("WARN: BINARY Sensor: GPIO and MCP23017 not set");
    }
#endif
  } else {
#endif // AFE_CONFIG_HARDWARE_MCP23017

    pinMode(configuration.gpio,
            configuration.internalPullUp ? INPUT_PULLUP : INPUT);

    state = digitalRead(configuration.gpio);

#ifdef AFE_CONFIG_HARDWARE_MCP23017
  }
#endif // AFE_CONFIG_HARDWARE_MCP23017

  _initialized = true;
}

#ifdef AFE_CONFIG_HARDWARE_MCP23017
void AFESensorBinary::addMCP23017Reference(AFEMCP23017Broker *MCP23017Broker) {
  _MCP23017Broker = MCP23017Broker;
  _MCP23017ReferenceAdded = true;
}
#endif

byte AFESensorBinary::get() {
  return configuration.revertSignal ? (state == 0 ? 1 : 0) : state;
}

boolean AFESensorBinary::listener() {
  _detected = false;
  if (_initialized) {
    byte _currentState;
#ifdef AFE_CONFIG_HARDWARE_MCP23017
    if (_expanderUsed) {
      _currentState = _MCP23017Broker->MCP[_MCP23017Id].digitalRead(
          configuration.mcp23017.gpio);
    } else {
#endif

      _currentState = digitalRead(configuration.gpio);

#ifdef AFE_CONFIG_HARDWARE_MCP23017
    }
#endif

    unsigned long time = millis();

    if (_currentState != state) {

      if (startTime == 0) { // starting timer. used for sensor bouncing
        startTime = time;
      }

      if (time - startTime >= configuration.bouncing) {
        _detected = true;
        state = _currentState;
#ifdef DEBUG
        Serial << endl << F("INFO: BINARY Sensor: state changed to: ") << state;
#endif
      }
    }
  }
  return _detected;
}

void AFESensorBinary::getJSON(char *json) {
  sprintf(json, "{\"state\":\"%s\"}",
          get() == 1 ? configuration.sendAsSwitch ? "off" : "open"
                     : configuration.sendAsSwitch ? "on" : "closed");
}

#endif // AFE_CONFIG_HARDWARE_BINARY_SENSOR
