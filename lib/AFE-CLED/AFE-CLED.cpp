#include "AFE-CLED.h"

#ifdef AFE_CONFIG_HARDWARE_CLED

AFECLED::AFECLED() {}

boolean AFECLED::begin(AFEDataAccess *Data, AFEDevice *Device) {

#ifdef DEBUG
  Serial << endl << F("INFO: CLED: Initializing CLED...");
#endif
  _Data = Data;
  _Device = Device;

  /**
   * @brief reading the RGB LEDs and all effects configurations
   *
   */
  for (uint8_t i = 0; i < _Device->configuration.noOfCLEDs; i++) {
    _Data->getConfiguration(i, &configuration[i]);
    _Data->getConfiguration(i, &configurationEffectBlinking[i]);
    _Data->getConfiguration(i, &configurationEffectWave[i]);
    _Data->getConfiguration(i, &configurationEffectFadeInOut[i]);

    if (i == 0) {
      FastLED.addLeds<AFE_CONFIG_HARDWARE_CLED_CHIPSET,
                      AFE_CONFIG_HARDWARE_CLED_0_GPIO,
                      AFE_CONFIG_HARDWARE_CLED_COLORS_ORDER>(
          leds[i], AFE_CONFIG_HARDWARE_CLED_MAX_NUMBER_OF_LED);
    } else if (i == 1) {
      FastLED.addLeds<AFE_CONFIG_HARDWARE_CLED_CHIPSET,
                      AFE_CONFIG_HARDWARE_CLED_1_GPIO,
                      AFE_CONFIG_HARDWARE_CLED_COLORS_ORDER>(
          leds[i], AFE_CONFIG_HARDWARE_CLED_MAX_NUMBER_OF_LED);
    }
  }

#ifdef DEBUG
  Serial << endl
         << F("INFO: CLED: Setting controlers...") << endl
         << F("INFO: CLED: Setting default parameters....");
#endif

  for (uint8_t i = 0; i < _Device->configuration.noOfCLEDs; i++) {
    currentState[i].on.color = configuration[i].on.color;
    currentState[i].on.brightness = configuration[i].on.brightness;
    currentState[i].off.color.red = AFE_CONFIG_HARDWARE_CLED_DEFAULT_OFF_COLOR;
    currentState[i].off.color.blue = AFE_CONFIG_HARDWARE_CLED_DEFAULT_OFF_COLOR;
    currentState[i].off.color.green =
        AFE_CONFIG_HARDWARE_CLED_DEFAULT_OFF_COLOR;
    currentState[i].off.brightness =
        AFE_CONFIG_HARDWARE_CLED_DEFAULT_OFF_BRIGHTNESS;
  }

  _initialized = true;

#ifdef DEBUG
  leds[0][0] = CRGB(255, 0, 0);
  leds[0][1] = CRGB(0, 255, 0);
  leds[0][2] = CRGB(0, 0, 255);
  leds[0][3] = CRGB(255, 255, 255);
  FastLED.show(20);
#endif

  return _initialized;
}

void AFECLED::on(uint8_t stripId, boolean disableEffects) {
#ifdef DEBUG
  Serial << endl
         << F("INFO: CLED: Turning LED: ") << stripId
         << F(": ON; Disable effects: ") << disableEffects;
#endif
  if (configuration[stripId].on.changeTime > 0) {
    _runSlowChange(stripId, true, disableEffects);
  } else {
    _turnOnOff(stripId, true, disableEffects);
  }
}
void AFECLED::off(uint8_t stripId, boolean disableEffects) {
#ifdef DEBUG
  Serial << endl
         << F("INFO: CLED: Turning LED: ") << stripId
         << F(": OFF; Disable effects: ") << disableEffects;
#endif

  if (configuration[stripId].off.changeTime > 0) {
    _runSlowChange(stripId, false, disableEffects);
  } else {
    _turnOnOff(stripId, false, disableEffects);
  }
}

void AFECLED::on(uint8_t stripId, CLED_RGB color, boolean disableEffects) {
  currentState[stripId].on.color = color;
  on(stripId, disableEffects);
}

void AFECLED::on(uint8_t stripId, CLED_PARAMETERS ledConfig,
                 boolean disableEffects, boolean saveColor) {
  currentState[stripId].on.color = ledConfig.color;
  currentState[stripId].slowChangeTargetBrightness = ledConfig.brightness;
  currentState[stripId].on.brightness = ledConfig.brightness;
  on(stripId, disableEffects);
  if (saveColor) {
    configuration[stripId].on.color.blue = ledConfig.color.blue;
    configuration[stripId].on.color.green = ledConfig.color.green;
    configuration[stripId].on.color.red = ledConfig.color.red;
    configuration[stripId].on.brightness = ledConfig.brightness;
    _Data->saveConfiguration(stripId, &configuration[stripId]);
  }
}

void AFECLED::activateEffect(uint8_t stripId, uint8_t effectId) {
  currentState[stripId].effect.id = effectId;
  currentState[stripId].effect.timer = 0;
  currentState[stripId].state = false;
  currentState[stripId].stateUpdated = true;
  currentState[stripId].effect.stateUpdated = true;
  switch (currentState[stripId].effect.id) {
  case 0:
    /* Effect deactivation */
    deactivateEffect(stripId);
    break;
  case AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING:
    /* No special configuration required */
    break;
  case AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT:
    currentState[stripId].config.brightness =
        configurationEffectFadeInOut[stripId].out.brightness;

    currentState[stripId].config.color =
        configurationEffectFadeInOut[stripId].in.color;

    currentState[stripId].effect.increment = -1;
    currentState[stripId].effect.integer = round(
        (configurationEffectFadeInOut[stripId].in.brightness -
         configurationEffectFadeInOut[stripId].out.brightness) /
        (configurationEffectFadeInOut[stripId].timeout / 2 /
         AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT_DEFAULT_FADE_INTERNAL_LOOP_INTERVAL));

    currentState[stripId].effect.integer =
        currentState[stripId].effect.integer < 1
            ? 1
            : currentState[stripId].effect.integer;

    break;
  case AFE_CONFIG_HARDWARE_CLED_EFFECT_WAVE:
    currentState[stripId].effect.integer = 0;
    currentState[stripId].effect.increment = -1;
    break;
  }
}

void AFECLED::deactivateEffect(uint8_t stripId, boolean setToOff) {
  currentState[stripId].effect.id = AFE_CONFIG_HARDWARE_CLED_EFFECT_NONE;
  currentState[stripId].effect.stateUpdated = true;
  if (setToOff) {
    currentState[stripId].off.color.red =
        AFE_CONFIG_HARDWARE_CLED_DEFAULT_OFF_COLOR;
    currentState[stripId].off.color.green =
        AFE_CONFIG_HARDWARE_CLED_DEFAULT_OFF_COLOR;
    currentState[stripId].off.color.blue =
        AFE_CONFIG_HARDWARE_CLED_DEFAULT_OFF_COLOR;
    currentState[stripId].off.brightness =
        AFE_CONFIG_HARDWARE_CLED_DEFAULT_OFF_BRIGHTNESS;
    _turnOnOff(stripId, false, true);
  }
}

void AFECLED::_effectBlinkingListener(uint8_t stripId) {
  if (_initialized) {
    if (currentState[stripId].effect.timer == 0) {
      currentState[stripId].effect.timer = millis();
    }

    if ((currentState[stripId].state &&
         millis() > currentState[stripId].effect.timer +
                        configurationEffectBlinking[stripId].onTimeout) ||
        (!currentState[stripId].state &&
         millis() > currentState[stripId].effect.timer +
                        configurationEffectBlinking[stripId].offTimeout)) {

      if (currentState[stripId].state) {
        _turnOnOff(stripId, false, false);

      } else {
        _turnOnOff(stripId, true, false);
      }

      currentState[stripId].effect.timer = millis();
    }
  }
}

void AFECLED::_effectFadeInOutListener(uint8_t stripId) {
  if (_initialized) {
    if (millis() - currentState[stripId].effect.timer >
        AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT_DEFAULT_FADE_INTERNAL_LOOP_INTERVAL) {

      if (currentState[stripId].config.brightness >=
              configurationEffectFadeInOut[stripId].in.brightness -
                  currentState[stripId].effect.integer ||
          currentState[stripId].config.brightness -
                  currentState[stripId].effect.integer <
              configurationEffectFadeInOut[stripId].out.brightness) {

        currentState[stripId].effect.increment =
            currentState[stripId].effect.increment > 0
                ? -1 * currentState[stripId].effect.integer
                : currentState[stripId].effect.integer;
      }

      currentState[stripId].config.brightness +=
          currentState[stripId].effect.increment;

      currentState[stripId]
              .config
              .brightness<
                  configurationEffectFadeInOut[stripId].out.brightness
                      ? configurationEffectFadeInOut[stripId].out.brightness
                      : currentState[stripId].config.brightness>
                  configurationEffectFadeInOut[stripId]
              .in.brightness
          ? configurationEffectFadeInOut[stripId].in.brightness
          : currentState[stripId].config.brightness;

      _setColor(stripId);
      currentState[stripId].effect.timer = millis();
    }
  }
}

void AFECLED::_effectWaveListener(uint8_t stripId) {
  if (millis() - currentState[stripId].effect.timer >
      configurationEffectWave[stripId].timeout) {

    if (currentState[stripId].effect.integer ==
            configuration[stripId].ledNumbers - 1 ||
        currentState[stripId].effect.integer == 0) {
      currentState[stripId].effect.increment *= -1;
    }

    leds[stripId][currentState[stripId].effect.integer] =
        CRGB(configurationEffectWave[stripId].off.color.red,
             configurationEffectWave[stripId].off.color.green,
             configurationEffectWave[stripId].off.color.blue);

    currentState[stripId].effect.integer +=
        currentState[stripId].effect.increment;

    leds[stripId][currentState[stripId].effect.integer] =
        CRGB(configurationEffectWave[stripId].on.color.red,
             configurationEffectWave[stripId].on.color.green,
             configurationEffectWave[stripId].on.color.blue);

    FastLED[stripId].show(leds[stripId], configuration[stripId].ledNumbers,
                          configurationEffectWave[stripId].on.brightness);
    FastLED[stripId].showLeds(configurationEffectWave[stripId].on.brightness);
    currentState[stripId].effect.timer = millis();
  }
}

boolean AFECLED::isStateUpdated(uint8_t stripId) {
  boolean _ret = currentState[stripId].stateUpdated;
  currentState[stripId].stateUpdated = false;
  return _ret;
}

boolean AFECLED::isEffectStateUpdated(uint8_t stripId) {
  boolean _ret = currentState[stripId].effect.stateUpdated;
  currentState[stripId].effect.stateUpdated = false;
  return _ret;
}

void AFECLED::loop() {
  if (_initialized) {
    for (uint8_t i = 0; i < _Device->configuration.noOfCLEDs; i++) {
      _effectSlowChange(i);
      switch (currentState[i].effect.id) {
      case AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING:
        _effectBlinkingListener(i);
        break;
      case AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT:
        _effectFadeInOutListener(i);
        break;
      case AFE_CONFIG_HARDWARE_CLED_EFFECT_WAVE:
        _effectWaveListener(i);
        break;
      }
    }
  }
}

void AFECLED::toggle(uint8_t stripId, boolean disableEffects) {
  currentState[stripId].state
      ? off(stripId, disableEffects)
      : on(stripId, currentState[stripId].on.color, disableEffects);
}

void AFECLED::toggle(uint8_t stripId, CLED_RGB color, boolean disableEffects) {
  currentState[stripId].state ? off(stripId, disableEffects)
                              : on(stripId, color, disableEffects);
}

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_STANDARD
float AFECLED::convertBrigtnessToAPI(uint8_t stripId, uint8_t brightness) {
  float _return;
  switch (configuration[stripId].brightnessConversion) {
  case AFE_CONFIG_HARDWARE_CLED_BRIGHTNESS_CONVERSION_0_100:
    _return = 100 * (float)brightness /
              (float)AFE_CONFIG_HARDWARE_CLED_MAX_BRIGHTNESS;
    break;
  case AFE_CONFIG_HARDWARE_CLED_BRIGHTNESS_CONVERSION_0_1:
    _return = (100 * (float)brightness /
               (float)AFE_CONFIG_HARDWARE_CLED_MAX_BRIGHTNESS) /
              100;
    break;
  default:
    _return = (float)brightness;
    break;
  }
  return _return;
}

uint8_t AFECLED::convertBrightnessFromAPI(uint8_t stripId, float brightness) {
  uint8_t _return;
  switch (configuration[stripId].brightnessConversion) {
  case AFE_CONFIG_HARDWARE_CLED_BRIGHTNESS_CONVERSION_0_100:
    _return =
        (uint8_t)(brightness * AFE_CONFIG_HARDWARE_CLED_MAX_BRIGHTNESS / 100);
    break;
  case AFE_CONFIG_HARDWARE_CLED_BRIGHTNESS_CONVERSION_0_1:
    _return = (uint8_t)(brightness * AFE_CONFIG_HARDWARE_CLED_MAX_BRIGHTNESS *
                        100 / 100);
    break;
  default:
    _return = (uint8_t)brightness;
    break;
  }
  return _return;
}
#endif // AFE_FIRMWARE_API_STANDARD

void AFECLED::_effectSlowChange(uint8_t stripId) {
  if (_initialized && _slowEffectParams[stripId].running) {

    unsigned long _currentTimer = millis() - _slowEffectParams[stripId].timer;
    /*
        Serial << endl
               << "running slow change. Step: " << _currentTimer
               << ". Duration of the effect:  "
               << (_slowEffectParams[stripId].state
                       ? configuration[stripId].on.changeTime
                       : configuration[stripId].off.changeTime);
    */
    if (_currentTimer < _slowEffectParams[stripId].state
            ? configuration[stripId].on.changeTime
            : configuration[stripId].off.changeTime) {

      if (_slowEffectParams[stripId].state) {
        _addSlowChangeStep(currentState[stripId].on.color.red, _slowEffectParams[stripId].startFrom.color.red,
                           _slowEffectParams[stripId].stepRed, _currentTimer);

        _addSlowChangeStep(currentState[stripId].on.color.blue, _slowEffectParams[stripId].startFrom.color.blue,
                           _slowEffectParams[stripId].stepBlue, _currentTimer);

        _addSlowChangeStep(currentState[stripId].on.color.green, _slowEffectParams[stripId].startFrom.color.green,
                           _slowEffectParams[stripId].stepGreen, _currentTimer);

        _addSlowChangeStep(currentState[stripId].on.brightness ,_slowEffectParams[stripId].startFrom.brightness,
                           _slowEffectParams[stripId].stepBrightness,
                           _currentTimer);
      } else {
        _addSlowChangeStep(currentState[stripId].off.color.red, _slowEffectParams[stripId].startFrom.color.red,
                           _slowEffectParams[stripId].stepRed, _currentTimer);

        _addSlowChangeStep(currentState[stripId].off.color.blue, _slowEffectParams[stripId].startFrom.color.blue,
                           _slowEffectParams[stripId].stepBlue, _currentTimer);

        _addSlowChangeStep(currentState[stripId].off.color.green, _slowEffectParams[stripId].startFrom.color.green,
                           _slowEffectParams[stripId].stepGreen, _currentTimer);

        _addSlowChangeStep(currentState[stripId].off.brightness ,_slowEffectParams[stripId].startFrom.brightness,
                           _slowEffectParams[stripId].stepBrightness,
                           _currentTimer);
      }

      _setColor(stripId, _slowEffectParams[stripId].state
                             ? currentState[stripId].on.color
                             : currentState[stripId].off.color,
                _slowEffectParams[stripId].state
                    ? currentState[stripId].on.brightness
                    : currentState[stripId].off.brightness);
    }

    if (_currentTimer >= (_slowEffectParams[stripId].state
                              ? configuration[stripId].on.changeTime
                              : configuration[stripId].off.changeTime)) {

      _slowEffectParams[stripId].running = false;
    }
  }
}

void AFECLED::_runSlowChange(uint8_t stripId, boolean state,
                             boolean disableEffects) {
  if (_initialized) {

#ifdef DEBUG
    Serial << endl << F("INFO: CLED: Changing in slow mode");
#endif

    if (disableEffects &&
        currentState[stripId].effect.id !=
            AFE_CONFIG_HARDWARE_CLED_EFFECT_NONE) {
      /**
       * @brief Deactivates effect, doesn't set LED off
       *
       */
      deactivateEffect(stripId, false);
    }

    _slowEffectParams[stripId].running = true;

    _slowEffectParams[stripId].startFrom.color.red =
        currentState[stripId].config.color.red;
    _slowEffectParams[stripId].startFrom.color.green =
        currentState[stripId].config.color.green;
    _slowEffectParams[stripId].startFrom.color.blue =
        currentState[stripId].config.color.blue;
    _slowEffectParams[stripId].startFrom.brightness =
        currentState[stripId].config.brightness;

    _slowEffectParams[stripId].stepRed =
        ((state ? (float)currentState[stripId].on.color.red
                : (float)AFE_CONFIG_HARDWARE_CLED_DEFAULT_OFF_COLOR) -
         (float)_slowEffectParams[stripId].startFrom.color.red) /
        (state ? (float)configuration[stripId].on.changeTime
               : (float)configuration[stripId].off.changeTime);

    _slowEffectParams[stripId].stepBlue =
        ((state ? (float)currentState[stripId].on.color.blue
                : (float)AFE_CONFIG_HARDWARE_CLED_DEFAULT_OFF_COLOR) -
         (float)_slowEffectParams[stripId].startFrom.color.blue) /
        (state ? (float)configuration[stripId].on.changeTime
               : (float)configuration[stripId].off.changeTime);

    _slowEffectParams[stripId].stepGreen =
        ((state ? (float)currentState[stripId].on.color.green
                : (float)AFE_CONFIG_HARDWARE_CLED_DEFAULT_OFF_COLOR) -
         (float)_slowEffectParams[stripId].startFrom.color.green) /
        (state ? (float)configuration[stripId].on.changeTime
               : (float)configuration[stripId].off.changeTime);

    _slowEffectParams[stripId].stepBrightness =
        ((state ? (float)currentState[stripId].on.brightness
                : (float)AFE_CONFIG_HARDWARE_CLED_DEFAULT_OFF_BRIGHTNESS) -
         (float)_slowEffectParams[stripId].startFrom.brightness) /
        (state ? (float)configuration[stripId].on.changeTime
               : (float)configuration[stripId].off.changeTime);

    _slowEffectParams[stripId].state = state;
    currentState[stripId].state = state;
    currentState[stripId].stateUpdated = true;
    _slowEffectParams[stripId].timer = millis();

#ifdef DEBUG
    Serial << endl
           << F(" - Red: ") << _slowEffectParams[stripId].startFrom.color.red
           << F(" => ") << (state ? currentState[stripId].on.color.red
                                  : currentState[stripId].off.color.red)
           << F(" step: ") << _slowEffectParams[stripId].stepRed * 100;
    Serial << endl
           << F(" - Green: ")
           << _slowEffectParams[stripId].startFrom.color.green << F(" => ")
           << (state ? currentState[stripId].on.color.green
                     : currentState[stripId].off.color.green)
           << F(" step: ") << _slowEffectParams[stripId].stepGreen * 100;
    Serial << endl
           << F(" - Blue: ") << _slowEffectParams[stripId].startFrom.color.blue
           << F(" => ") << (state ? currentState[stripId].on.color.blue
                                  : currentState[stripId].off.color.blue)
           << F(" step: ") << _slowEffectParams[stripId].stepBlue * 100;
    Serial << endl
           << F(" - Brightness: ")
           << _slowEffectParams[stripId].startFrom.brightness << F(" => ")
           << (state ? currentState[stripId].on.brightness
                     : currentState[stripId].off.brightness)
           << F(" step: ") << _slowEffectParams[stripId].stepBrightness * 100;
    Serial << endl
           << F(" - State: ")
           << (currentState[stripId].state ? F("ON") : F("OFF"));
#endif
  }
}

void AFECLED::_addSlowChangeStep(uint8_t &next, uint8_t from, float step,
                                 unsigned long increment) {

  if ((from + increment * step) > 255) {
    next = 255;
  } else {
    if ((from + increment * step) < 0) {
      next = 0;
    } else {
      next = from + increment * step;
    }
  }
}

void AFECLED::_turnOnOff(uint8_t stripId, boolean state,
                         boolean disableEffects) {
  if (_initialized) {
    _setColor(stripId, state ? currentState[stripId].on.color
                             : currentState[stripId].off.color,
              state ? currentState[stripId].on.brightness
                    : currentState[stripId].off.brightness);

    currentState[stripId].state = state;
    currentState[stripId].stateUpdated = true;

    if (disableEffects &&
        currentState[stripId].effect.id !=
            AFE_CONFIG_HARDWARE_CLED_EFFECT_NONE) {
      deactivateEffect(stripId, false);
    }
  }
}

void AFECLED::_setColor(uint8_t stripId) {
  /*
  #ifdef DEBUG
    Serial << endl
           << F("INFO: CLED: Setting LED: RGB[")
           << currentState[stripId].config.color.red << F(",")
           << currentState[stripId].config.color.green << F(",")
           << currentState[stripId].config.color.blue << F("]")
           << F(", Brightness: ") << currentState[stripId].config.brightness;
  #endif
  */
  /* @TODO T7 find out why I need to call it twice to turn it on/off */
  FastLED[stripId].showColor(CRGB(currentState[stripId].config.color.red,
                                  currentState[stripId].config.color.green,
                                  currentState[stripId].config.color.blue),
                             configuration[stripId].ledNumbers,
                             currentState[stripId].config.brightness);

  FastLED[stripId].showColor(CRGB(currentState[stripId].config.color.red,
                                  currentState[stripId].config.color.green,
                                  currentState[stripId].config.color.blue),
                             configuration[stripId].ledNumbers,
                             currentState[stripId].config.brightness);

  /**
   * @brief setting state to OFF is RGB=0
   *
   */
  if (currentState[stripId].config.color.red ==
          AFE_CONFIG_HARDWARE_CLED_DEFAULT_OFF_COLOR &&
      currentState[stripId].config.color.green ==
          AFE_CONFIG_HARDWARE_CLED_DEFAULT_OFF_COLOR &&
      currentState[stripId].config.color.blue ==
          AFE_CONFIG_HARDWARE_CLED_DEFAULT_OFF_COLOR &&
      currentState[stripId].state == AFE_ON) {
    currentState[stripId].state = AFE_OFF;
  }
}

void AFECLED::_setColor(uint8_t stripId, CLED_RGB color, uint8_t brightness) {
  currentState[stripId].config.color = color;
  currentState[stripId].config.brightness = brightness;
  _setColor(stripId);
}

void AFECLED::_setColor(uint8_t stripId, CLED_RGB color) {
  currentState[stripId].config.color = color;
  _setColor(stripId);
}
void AFECLED::_setColor(uint8_t stripId, CLED_PARAMETERS ledConfig) {
  currentState[stripId].config.color = ledConfig.color;
  currentState[stripId].config.brightness = ledConfig.brightness;
  _setColor(stripId);
}

#endif // AFE_CONFIG_HARDWARE_CLED