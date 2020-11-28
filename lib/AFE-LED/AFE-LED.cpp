/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-LED.h"

#ifdef AFE_CONFIG_HARDWARE_LED

AFELED::AFELED() {}

boolean AFELED::begin(AFEDataAccess *_Data, uint8_t id) {
  if (id != AFE_HARDWARE_ITEM_NOT_EXIST) {
    _Data->getConfiguration(id, &LEDConfiguration);

#ifdef AFE_CONFIG_HARDWARE_MCP23017
    // If MCP23017 available in the HW, checking if LED connected using MCP23017
    if (LEDConfiguration.gpio == AFE_HARDWARE_ITEM_NOT_EXIST) {
      if (LEDConfiguration.mcp23017.gpio != AFE_HARDWARE_ITEM_NOT_EXIST &&
          LEDConfiguration.mcp23017.address !=
              AFE_CONFIG_HARDWARE_I2C_DEFAULT_ADDRESS) {

        I2CPORT I2C;
        _Data->getConfiguration(&I2C);

#ifdef DEBUG
        Serial << endl
               << F("INFO: LED: Checking if the expander MCP23017 is connected");
#endif
        AFEI2CScanner I2CScanner;
        I2CScanner.begin();
        if (I2CScanner.scan(LEDConfiguration.mcp23017.address)) {

#ifdef DEBUG
          Serial << endl << F("INFO: LED: Initializing with MCP23017");
#endif

          mcp.begin(LEDConfiguration.mcp23017.address, I2C.SDA, I2C.SCL);
          mcp.pinMode(LEDConfiguration.mcp23017.gpio, OUTPUT);
          expanderUsed = true;
        }
#ifdef DEBUG
        else {
          Serial << endl
                 << F("WARN: LED: MCP23017 not found om I2C Address: 0x")
                 << _HEX(LEDConfiguration.mcp23017.address);
        }
#endif
      }
#ifdef DEBUG
      else {
        Serial << endl << F("WARN: LED: GPIO and MCP23017 not set");
      }
#endif
    } else {
#endif // AFE_CONFIG_HARDWARE_MCP23017

      pinMode(LEDConfiguration.gpio, OUTPUT);
      LEDConfiguration.changeToOppositeValue
          ? digitalWrite(LEDConfiguration.gpio, LOW)
          : digitalWrite(LEDConfiguration.gpio, HIGH);

#ifdef AFE_CONFIG_HARDWARE_MCP23017
    }
#endif // AFE_CONFIG_HARDWARE_MCP23017

    _initialized = true;
  }
  return _initialized;
}

void AFELED::on() {
  if (LEDConfiguration.changeToOppositeValue) {
    set(HIGH);
  } else {
    set(LOW);
  }
}

void AFELED::off() {
  if (LEDConfiguration.changeToOppositeValue) {
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
  interval = blinking_interval;
  blinking = true;
}

void AFELED::blinkingOff() { blinking = false; }

boolean AFELED::isBlinking() { return blinking; }

void AFELED::loop() {
  if (_initialized && blinking) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      toggle();
    }
  }
}

void AFELED::set(uint8_t state) {
  if (_initialized) {


#ifdef AFE_CONFIG_HARDWARE_MCP23017
    if (expanderUsed) {

Serial << endl << "####:  State to set = " << state << " current=" << mcp.digitalRead(LEDConfiguration.mcp23017.gpio);


      if (mcp.digitalRead(LEDConfiguration.mcp23017.gpio) != state) {
        mcp.digitalWrite(LEDConfiguration.mcp23017.gpio, state);
      }
    } else {
#endif

      if (digitalRead(LEDConfiguration.gpio) != state) {
        digitalWrite(LEDConfiguration.gpio, state);
      }

#ifdef AFE_CONFIG_HARDWARE_MCP23017
    }
#endif
  }
}

void AFELED::toggle() {
  if (_initialized) {

#ifdef AFE_CONFIG_HARDWARE_MCP23017
    if (expanderUsed) {
      mcp.digitalRead(LEDConfiguration.mcp23017.gpio) == HIGH
          ? mcp.digitalWrite(LEDConfiguration.mcp23017.gpio, LOW)
          : mcp.digitalWrite(LEDConfiguration.mcp23017.gpio, HIGH);
    } else {
#endif

      digitalRead(LEDConfiguration.gpio) == HIGH
          ? digitalWrite(LEDConfiguration.gpio, LOW)
          : digitalWrite(LEDConfiguration.gpio, HIGH);

#ifdef AFE_CONFIG_HARDWARE_MCP23017
    }
#endif
  }
}

#endif // AFE_CONFIG_HARDWARE_LED