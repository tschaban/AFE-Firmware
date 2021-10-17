
/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-CLED.h"

#ifdef AFE_CONFIG_HARDWARE_CLED

AFECLED::AFECLED() {}

boolean AFECLED::begin(AFEDataAccess *Data) {

#ifdef DEBUG
  Serial << endl << "INFO: CLED: Initializing CLED...";
#endif

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_STRIPS; i++) {
    Data->getConfiguration(i, configuration);
  }

  FastLED.addLeds<AFE_CONFIG_HARDWARE_CLED_CHIPSET,
                  AFE_CONFIG_HARDWARE_CLED_0_GPIO,
                  AFE_CONFIG_HARDWARE_CLED_COLORS_ORDER>(
      leds[0], AFE_CONFIG_HARDWARE_CLED_MAX_NUMBER_OF_LED);

  FastLED.addLeds<AFE_CONFIG_HARDWARE_CLED_CHIPSET,
                  AFE_CONFIG_HARDWARE_CLED_1_GPIO,
                  AFE_CONFIG_HARDWARE_CLED_COLORS_ORDER>(
      leds[1], AFE_CONFIG_HARDWARE_CLED_MAX_NUMBER_OF_LED);

#ifdef DEBUG
  Serial << endl << "INFO: CLED: Setting controlers...";
#endif
/*
  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_STRIPS; i++) {
    controllers[i]->setLeds(leds[i],
                            AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_STRIPS);
    controllers[i]->clearLedData();
  }
*/
#ifdef DEBUG
  Serial << endl << "INFO: CLED: Setting default parameters....";
#endif

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_STRIPS; i++) {
    _currentState[i].on.color = configuration[i].on.color;
    _currentState[i].on.brightness = configuration[i].on.brightness;
    _currentState[i].off.color = configuration[i].off.color;
    _currentState[i].off.brightness = configuration[i].off.brightness;
    //_currentState[i].config.color = _off[i].color;
    //_currentState[i].config.brightness = _off[i].brightness;
  }

  _initialized = true;

  on(0);
  delay(2000);
  off(0);

  // activateEffect(0, AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING);

  return _initialized;
}

void AFECLED::on(uint8_t stripId) {
  on(stripId, configuration[stripId].on.color,
     configuration[stripId].on.brightness);
}
void AFECLED::off(uint8_t stripId) {
  off(stripId, configuration[stripId].off.color,
      configuration[stripId].off.brightness);
}

void AFECLED::on(uint8_t stripId, uint32_t color) {
  on(stripId, color, _currentState[stripId].on.brightness);
}
void AFECLED::off(uint8_t stripId, uint32_t color) {
  off(stripId, color, _currentState[stripId].off.brightness);
}

void AFECLED::on(uint8_t stripId, uint32_t color, uint8_t brightness) {
  _currentState[stripId].on.color = color;
  _currentState[stripId].on.brightness = brightness;
  _turnOnOff(stripId, true);
}
void AFECLED::off(uint8_t stripId, uint32_t color, uint8_t brightness) {
  _currentState[stripId].off.color = color;
  _currentState[stripId].off.brightness = brightness;
  _turnOnOff(stripId, false);
}

void AFECLED::activateEffect(uint8_t stripId, uint8_t effectId) {
  _currentState[stripId].effect.id = effectId;
  _currentState[stripId].effect.timer = 0;
  switch (_currentState[stripId].effect.id) {
  case AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING:
    _currentState[stripId].config.brightness =
        AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING_DEFAULT_OFF_BRIGHTNESS;
    _currentState[stripId].config.color =
        AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING_DEFAULT_OFF_COLOR;
    break;
  case AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT:
    _currentState[stripId].config.brightness =
        AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT_DEFAULT_MIN_BRIGHTNESS;
    _currentState[stripId].config.color =
        AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT_DEFAULT_COLOR;
    _currentState[stripId].effect.fadeInOut.increment = -1;
    _currentState[stripId].effect.fadeInOut.step = round(
        (AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT_DEFAULT_MAX_BRIGHTNESS -
         AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT_DEFAULT_MIN_BRIGHTNESS) /
        (AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT_DEFAULT_FADE_INTERVAL / 2 /
         AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT_DEFAULT_FADE_INTERNAL_LOOP_INTERVAL));

    _currentState[stripId].effect.fadeInOut.step =
        _currentState[stripId].effect.fadeInOut.step < 1
            ? 1
            : _currentState[stripId].effect.fadeInOut.step;
    break;
  case AFE_CONFIG_HARDWARE_CLED_EFFECT_WAVE:
    _currentState[stripId].config.brightness =
        AFE_CONFIG_HARDWARE_CLED_EFFECT_WAVE_DEFAULT_BRIGHTNESS;
    _currentState[stripId].effect.wave.ledId = 0;
    _currentState[stripId].effect.wave.increment = -1;
    break;
  }
}

void AFECLED::deactivateEffect(uint8_t stripId, boolean setToOff) {
  _currentState[stripId].effect.id = AFE_NONE;
  if (setToOff) {
    off(stripId, AFE_CONFIG_HARDWARE_CLED_DEFAULT_OFF_COLOR,
        AFE_CONFIG_HARDWARE_CLED_DEFAULT_OFF_BRIGHTNESS);
  }
}

void AFECLED::effectBlinkingListener(uint8_t stripId) {
  if (_initialized) {
    if (_currentState[stripId].effect.timer == 0) {
      _currentState[stripId].effect.timer = millis();
    }

    if ((_currentState[stripId].state &&
         millis() >
             _currentState[stripId].effect.timer +
                 AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING_DEFAULT_ON_TIMER) ||
        (!_currentState[stripId].state &&
         millis() >
             _currentState[stripId].effect.timer +
                 AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING_DEFAULT_OFF_TIMER)) {
      if (_currentState[stripId].state) {
        off(stripId, AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING_DEFAULT_OFF_COLOR,
            AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING_DEFAULT_OFF_BRIGHTNESS);
      } else {
        on(stripId, AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING_DEFAULT_ON_COLOR,
           AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING_DEFAULT_ON_BRIGHTNESS);
      }
      _currentState[stripId].effect.timer = millis();
    }
  }
}

void AFECLED::effectFadeInOutListener(uint8_t stripId) {
  if (_initialized) {
    if (millis() - _currentState[stripId].effect.timer >
        AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT_DEFAULT_FADE_INTERNAL_LOOP_INTERVAL) {

      if (_currentState[stripId].config.brightness >=
              AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT_DEFAULT_MAX_BRIGHTNESS -
                  _currentState[stripId].effect.fadeInOut.step ||
          _currentState[stripId].config.brightness -
                  _currentState[stripId].effect.fadeInOut.step <
              AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT_DEFAULT_MIN_BRIGHTNESS) {

        _currentState[stripId].effect.fadeInOut.increment =
            _currentState[stripId].effect.fadeInOut.increment > 0
                ? -1 * _currentState[stripId].effect.fadeInOut.step
                : _currentState[stripId].effect.fadeInOut.step;
      }

      _currentState[stripId].config.brightness +=
          _currentState[stripId].effect.fadeInOut.increment;

      _currentState[stripId]
              .config.brightness<
                  AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT_DEFAULT_MIN_BRIGHTNESS
                      ? AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT_DEFAULT_MIN_BRIGHTNESS
                      : _currentState[stripId].config.brightness>
                  AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT_DEFAULT_MAX_BRIGHTNESS
          ? AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT_DEFAULT_MAX_BRIGHTNESS
          : _currentState[stripId].config.brightness;

      _setColor(stripId,
                AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT_DEFAULT_COLOR,
                _currentState[stripId].config.brightness);
      _currentState[stripId].effect.timer = millis();
    }
  }
}

void AFECLED::effectWaveListener(uint8_t stripId) {
  if (millis() - _currentState[stripId].effect.timer >
      AFE_CONFIG_HARDWARE_CLED_EFFECT_WAVE_DEFAULT_WAVE_TIME) {

    if (_currentState[stripId].effect.wave.ledId ==
            FastLED[stripId].size() - 1 ||
        _currentState[stripId].effect.wave.ledId == 0) {
      _currentState[stripId].effect.wave.increment *= -1;
    }

    leds[stripId][_currentState[stripId].effect.wave.ledId] =
        AFE_CONFIG_HARDWARE_CLED_EFFECT_WAVE_DEFAULT_OFF_COLOR;

    _currentState[stripId].effect.wave.ledId +=
        _currentState[stripId].effect.wave.increment;

    leds[stripId][_currentState[stripId].effect.wave.ledId] =
        AFE_CONFIG_HARDWARE_CLED_EFFECT_WAVE_DEFAULT_ON_COLOR;

    FastLED[stripId].show(leds[stripId], FastLED[stripId].size(),
                          _currentState[stripId].config.brightness);
    FastLED[stripId].showLeds(_currentState[stripId].config.brightness);
    _currentState[stripId].effect.timer = millis();
    Serial << endl << _currentState[stripId].effect.wave.ledId;
  }
}

void AFECLED::loop() {
  if (_initialized) {
    for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_STRIPS; i++) {
      switch (_currentState[i].effect.id) {
      case AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING:
        effectBlinkingListener(i);
        break;
      case AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT:
        effectFadeInOutListener(i);
        break;
      case AFE_CONFIG_HARDWARE_CLED_EFFECT_WAVE:
        effectWaveListener(i);
        break;
      }
    }
  }
}

void AFECLED::toggle(uint8_t stripId, uint32_t color) {
  _currentState[stripId].state ? off(stripId) : on(stripId);
}

void AFECLED::_turnOnOff(uint8_t stripId, boolean state) {
  if (_initialized) {
    _setColor(stripId, state ? _currentState[stripId].on.color : _currentState[stripId].off.color,
              state ? _currentState[stripId].on.brightness : _currentState[stripId].off.brightness);
    _currentState[stripId].state = state;
  }
}

void AFECLED::_setBrightness(uint8_t stripId, uint8_t brightness) {
  _setColor(stripId, _currentState[stripId].config.color, brightness);
}

void AFECLED::_setColor(uint8_t stripId, uint32_t color) {
  _setColor(stripId, color, _currentState[stripId].config.brightness);
}
void AFECLED::_setColor(uint8_t stripId, uint32_t color, uint8_t brightness) {
#ifdef DEBUG
  Serial << endl
         << "INFO: CLED[" << stripId << "]: Setting color: " << color
         << ", brightness: " << brightness;
#endif
  _currentState[stripId].config.color = color;
  _currentState[stripId].config.brightness = brightness;
  FastLED[stripId].showColor(_currentState[stripId].config.color,
                             _currentState[stripId].config.brightness);
}

#endif // AFE_CONFIG_HARDWARE_CLED