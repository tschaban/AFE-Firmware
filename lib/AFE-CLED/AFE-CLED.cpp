/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-CLED.h"

#ifdef AFE_CONFIG_HARDWARE_CLED

AFECLED::AFECLED() {}

boolean AFECLED::begin(AFEDataAccess *Data) {

  Data->getConfiguration(
      AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_EFFECT_ID,
      &configuration[AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_EFFECT_ID]);
  Data->getConfiguration(
      AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT_ID,
      &configuration[AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT_ID]);

  Data->getConfiguration(AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT_ID,
                         &effects);

  Data->getConfiguration(AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_EFFECT_ID,
                         &backlight);

  if (backlight.lightSensorId != AFE_HARDWARE_ITEM_NOT_EXIST) {
    if (backlight.lightSensorId >= 50) {
      lightSensorType = AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_SENSOR_TYPE_TLS2561;
      backlight.lightSensorId -= 50;
    } else {
      lightSensorType = AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_SENSOR_TYPE_BH1750;
    }
  }

#ifdef DEBUG
  Serial << endl << "INFO: CLED: Starting CLED....";
#endif

  controllers[AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_EFFECT_ID] =
      &FastLED.addLeds<AFE_CONFIG_HARDWARE_CLED_CHIPSET,
                       AFE_CONFIG_HARDWARE_CLED_0_GPIO,
                       AFE_CONFIG_HARDWARE_CLED_COLORS_ORDER>(
          leds16, AFE_CONFIG_HARDWARE_CLED_16_LEDS);

  controllers[AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT_ID] =
      &FastLED.addLeds<AFE_CONFIG_HARDWARE_CLED_CHIPSET,
                       AFE_CONFIG_HARDWARE_CLED_1_GPIO,
                       AFE_CONFIG_HARDWARE_CLED_COLORS_ORDER>(
          leds8, AFE_CONFIG_HARDWARE_CLED_8_LEDS);

  /* Adding refeence to strip leds */
  controllers[AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_EFFECT_ID]->setLeds(
      leds16, AFE_CONFIG_HARDWARE_CLED_16_LEDS);

  controllers[AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT_ID]->setLeds(
      leds8, AFE_CONFIG_HARDWARE_CLED_8_LEDS);

  /* Clearning leds */
  controllers[AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_EFFECT_ID]->clearLeds(
      AFE_CONFIG_HARDWARE_CLED_16_LEDS);

  controllers[AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT_ID]->clearLeds(
      AFE_CONFIG_HARDWARE_CLED_8_LEDS);

#ifdef DEBUG
  Serial << "completed" << endl
         << "INFO: CLED: Setting default colors for effect";
#endif

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_EFFECTS; i++) {
    _effectColor[i] = effects.effect[i].color;
  }

#ifdef DEBUG
  Serial << endl << "INFO: CLED: Preparing effects";
#endif

  /* Effect: fade in/out calcuating step */
  _fadeStep = round(
      effects.effect[AFE_CONFIG_HARDWARE_EFFECT_FADE_IN_OUT].brightness /
      (effects.effect[AFE_CONFIG_HARDWARE_EFFECT_FADE_IN_OUT].time / 2 /
       AFE_CONFIG_HARDWARE_EFFECT_FADE_IN_OUT_DEFAULT_FADE_INTERNAL_LOOP_INTERVAL));

  _initialized = true;

  return _initialized;
}

void AFECLED::on(uint8_t stripId) {
  if (_initialized) {
    setColor(stripId, _onColor[stripId]);
    state[stripId] = true;
  }
}

void AFECLED::on(uint8_t stripId, uint32_t color) {
  _onColor[stripId] = color;
  on(stripId);
}

void AFECLED::off(uint8_t stripId) {
  if (_initialized) {
    setColor(stripId, _offColor[stripId]);
    state[stripId] = false;
  }
}

void AFECLED::off(uint8_t stripId, uint32_t color) {
  _offColor[stripId] = color;
  off(stripId);
}

void AFECLED::blink(uint8_t stripId, unsigned int duration, uint32_t onColor,
                    uint32_t offColor) {
  if (_initialized) {
    on(stripId, onColor);
    delay(duration);
    off(stripId, offColor);
  }
}

void AFECLED::loop() {
  if (_initialized) {
    if (_currentEffect == AFE_CONFIG_HARDWARE_EFFECT_WAVE) {
      waveEffect(AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT_ID);
    } else if (_currentEffect == AFE_CONFIG_HARDWARE_EFFECT_FADE_IN_OUT) {
      fadeInOutEffect(AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT_ID);
    }
  }
}

void AFECLED::toggle(uint8_t stripId, uint32_t color) {
  state[stripId] ? off(stripId) : on(stripId);
}

void AFECLED::setBrightness(uint8_t stripId, uint8_t level) {
#ifdef DEBUG
  Serial << endl
         << "INFO: CLED[" << stripId
         << "]: Setting brightness level: " << level;
#endif
  _currentBrightness[stripId] = level;
  setColor(stripId, _currentColor[stripId]);
  // FastLED[stripId].showLeds(_currentBrightness[stripId]);
}

void AFECLED::setColor(uint8_t stripId, uint32_t color) {
#ifdef DEBUG
  Serial << endl
         << "INFO: CLED[" << stripId << "]: Setting color: " << color
         << ", brightness: " << _currentBrightness[stripId];
#endif

  fill_solid(stripId == AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_EFFECT_ID ? leds16
                                                                     : leds8,
             controllers[stripId]->size(), color);

  controllers[stripId]->show(
      stripId == AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_EFFECT_ID ? leds16 : leds8,
      controllers[stripId]->size(), _currentBrightness[stripId]);
  controllers[stripId]->showLeds(_currentBrightness[stripId]);

  _currentColor[stripId] = color;
}

void AFECLED::effectOn(uint8_t stripId, uint8_t effectId) {
#ifdef DEBUG
  Serial << endl
         << "INFO: CLED[" << stripId << "] Tuning on effect: " << effectId
         << "...";
#endif
  _currentEffect = effectId;
  if (_currentEffect == AFE_CONFIG_HARDWARE_EFFECT_WAVE) {
    _currentBrightness[stripId] =
        effects.effect[AFE_CONFIG_HARDWARE_EFFECT_WAVE].brightness;
    setColor(stripId, CRGB::Black);
    _increment = 1;
  } else if (_currentEffect == AFE_CONFIG_HARDWARE_EFFECT_FADE_IN_OUT) {
    _currentBrightness[stripId] = _fadeStep + 1;
    setColor(stripId, _effectColor[AFE_CONFIG_HARDWARE_EFFECT_FADE_IN_OUT]);
    _increment = _fadeStep;
  }
  _effectTimer = millis();
}

void AFECLED::effectOff(uint8_t stripId) {
  _currentEffect = AFE_CONFIG_HARDWARE_EFFECT_NO_EFFECTS;
}

void AFECLED::waveEffect(uint8_t stripId) {
  if (millis() - _effectTimer >
      effects.effect[AFE_CONFIG_HARDWARE_EFFECT_WAVE].time) {
    if (_currentLedId == controllers[stripId]->size() - 1 ||
        _currentLedId == 0) {
      _increment *= -1;
    }

    switch (controllers[stripId]->size()) {
    case AFE_CONFIG_HARDWARE_CLED_8_LEDS:
      leds8[_currentLedId] = CRGB::Black;
      break;
    case AFE_CONFIG_HARDWARE_CLED_16_LEDS:
      leds16[_currentLedId] = CRGB::Black;
      break;
    }

    _currentLedId += _increment;

    switch (controllers[stripId]->size()) {
    case AFE_CONFIG_HARDWARE_CLED_8_LEDS:
      leds8[_currentLedId] = _effectColor[AFE_CONFIG_HARDWARE_EFFECT_WAVE];
      break;
    case AFE_CONFIG_HARDWARE_CLED_16_LEDS:
      leds16[_currentLedId] = _effectColor[AFE_CONFIG_HARDWARE_EFFECT_WAVE];
      break;
    }
    controllers[stripId]->show(
        stripId == AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_EFFECT_ID ? leds16
                                                                : leds8,
        controllers[stripId]->size(),
        _currentBrightness[stripId]);
    FastLED[stripId].showLeds(_currentBrightness[stripId]);
    _effectTimer = millis();
  }
}

void AFECLED::fadeInOutEffect(uint8_t stripId) {
  if (millis() - _effectTimer >
      AFE_CONFIG_HARDWARE_EFFECT_FADE_IN_OUT_DEFAULT_FADE_INTERNAL_LOOP_INTERVAL) {
    if (_currentBrightness[stripId] >=
            effects.effect[AFE_CONFIG_HARDWARE_EFFECT_FADE_IN_OUT].brightness -
                _fadeStep - 1 ||
        _currentBrightness[stripId] < _fadeStep + 1) {
      _increment = _increment > 0 ? -1 * _fadeStep : _fadeStep;
    }
    _currentBrightness[stripId] += _increment;

    controllers[stripId]->show(
        stripId == AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_EFFECT_ID ? leds16
                                                                : leds8,
        controllers[stripId]->size(),
        _currentBrightness[stripId]);

    controllers[stripId]->showLeds(_currentBrightness[stripId]);

    _effectTimer = millis();
  }
}

void AFECLED::setCustomEffectColor(uint8_t stripId, uint8_t effectId,
                                   uint32_t color) {
#ifdef DEBUG
  Serial << endl
         << "INFO: CLED[" << stripId << "]: Setting custom color: " << color
         << " for effect: " << effectId;
#endif
  _effectColor[effectId] = color;
}

void AFECLED::backlightEffect(uint8_t stripId, uint32_t lightLevel) {
#ifdef DEBUG
  Serial << endl
         << "INFO: CLED[" << stripId << "]: Backlight: changing light if needed"
         << endl
         << ": light level: " << lightLevel << " lux" << endl
         << ": current color: " << _currentColor[stripId]
         << ", brightness: " << _currentBrightness[stripId];

#endif

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_BACKLIGHT_LEVELS;
       i++) {

#ifdef DEBUG
    Serial << endl << " - Rule: " << i;
#endif

    if (lightLevel <= backlight.config[i].luxLevel) {

#ifdef DEBUG
      Serial << endl << ": Applying rule: " << i;
#endif

      if (_currentColor[stripId] != backlight.config[i].color ||
          _currentBrightness[stripId] != backlight.config[i].brightness) {

#ifdef DEBUG
        if (_currentColor[stripId] != backlight.config[i].color) {
          Serial << endl
                 << ": changing color from  " << _currentColor[stripId]
                 << " to " << backlight.config[i].color;
        }

        if (_currentBrightness[stripId] != backlight.config[i].brightness) {
          Serial << endl
                 << ": changing brigtness from  " << _currentBrightness[stripId]
                 << " to " << backlight.config[i].brightness;
        }
#endif

        _currentBrightness[stripId] = backlight.config[i].brightness;
        setColor(stripId, backlight.config[i].color);
      }

      if (_currentBrightness[stripId] > 0 && state[stripId] == false) {
        state[stripId] = true;
      } else if (backlight.config[i].brightness == 0 &&
                 state[stripId] == true) {
        state[stripId] = false;
      }

      break;
    }
  }
}

#endif // AFE_CONFIG_HARDWARE_CLED