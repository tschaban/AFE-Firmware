/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-CLED.h"

#ifdef AFE_CONFIG_HARDWARE_CLED

AFECLED::AFECLED() {}

boolean AFECLED::begin(AFEDataAccess *Data, uint8_t id) {
  if (id != AFE_HARDWARE_ITEM_NOT_EXIST) {
    Data->getConfiguration(id, &configuration);
    Data->getConfiguration(id, &effects);

#ifdef DEBUG
    Serial << endl << "INFO: CLED[" << id << "]: Initializing CLED....";
#endif
    if (id == AFE_CONFIG_HARDWARE_CLED_DEVICE_LIGHT_EFFECT_ID) {
      FastLED
          .addLeds<AFE_CONFIG_HARDWARE_CLED_CHIPSET,
                   AFE_CONFIG_HARDWARE_CLED_0_GPIO,
                   AFE_CONFIG_HARDWARE_CLED_COLORS_ORDER>(
              leds16, AFE_CONFIG_HARDWARE_CLED_16_LEDS)
          .setCorrection(TypicalSMD5050);
      configuration.ledNumber = AFE_CONFIG_HARDWARE_CLED_16_LEDS;
    } else {
      FastLED
          .addLeds<AFE_CONFIG_HARDWARE_CLED_CHIPSET,
                   AFE_CONFIG_HARDWARE_CLED_1_GPIO,
                   AFE_CONFIG_HARDWARE_CLED_COLORS_ORDER>(
              leds8, AFE_CONFIG_HARDWARE_CLED_8_LEDS)
          .setCorrection(TypicalSMD5050);
      configuration.ledNumber = AFE_CONFIG_HARDWARE_CLED_8_LEDS;
    }

#ifdef DEBUG
    Serial << "completed" << endl
           << "INFO: CLED[" << id << "]: Setting default colors for effect";
#endif

    for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_EFFECT_NO_EFFECTS; i++) {
      _effectColor[i] = effects.effect[i].color;
    }

#ifdef DEBUG
    Serial << endl << "INFO: CLED[" << id << "]: Preparing effects";
#endif

    /* Effect: fade in/out calcuating step */
    _fadeStep = round(
        effects.effect[1].brightness /
        (effects.effect[1].time / 2 /
         AFE_CONFIG_HARDWARE_EFFECT_FADE_IN_OUT_DEFAULT_FADE_INTERNAL_LOOP_INTERVAL));

    _initialized = true;
  }
  return _initialized;
}

void AFECLED::on() {
  if (_initialized) {
    setColor(_onColor);
    FastLED.show();
    state = true;
  }
}

void AFECLED::on(uint32_t color) {
  _onColor = color;
  on();
}

void AFECLED::off() {
  if (_initialized) {
    setColor(_offColor);
    FastLED.show();
    state = false;
  }
}

void AFECLED::off(uint32_t color) {
  _offColor = color;
  off();
}

void AFECLED::blink(unsigned int duration, uint32_t onColor,
                    uint32_t offColor) {
  if (_initialized) {
    on(onColor);
    delay(duration);
    off(offColor);
  }
}

void AFECLED::loop() {
  if (_initialized) {
    if (_currentEffect == AFE_CONFIG_HARDWARE_EFFECT_WAVE) {
      waveEffect();
    } else if (_currentEffect == AFE_CONFIG_HARDWARE_EFFECT_FADE_IN_OUT) {
      fadeInOutEffect();
    }
  }
}

void AFECLED::toggle(uint32_t color) { state ? off() : on(); }

void AFECLED::setBrightness(uint8_t level) { FastLED.setBrightness(level); }

void AFECLED::setColor(uint32_t color) {
  for (uint8_t i = 0; i < configuration.ledNumber; i++) {
    if (configuration.ledNumber == AFE_CONFIG_HARDWARE_CLED_8_LEDS) {
      leds8[i] = color;
    } else {
      leds16[i] = color;
    }
  }
}

void AFECLED::effectOn(uint8_t effectId) {
#ifdef DEBUG
  Serial << endl << "INFO: CLED Tuning on effect: " << effectId << "...";
#endif
  _currentEffect = effectId;
  if (_currentEffect == AFE_CONFIG_HARDWARE_EFFECT_WAVE) {
    setColor(CRGB::Black);
    FastLED.setBrightness(
        effects.effect[AFE_CONFIG_HARDWARE_EFFECT_WAVE].brightness);
    _increment = 1;
  } else if (_currentEffect == AFE_CONFIG_HARDWARE_EFFECT_FADE_IN_OUT) {
    _currentBrightness = _fadeStep + 1;
    FastLED.setBrightness(_currentBrightness);
    setColor(_effectColor[AFE_CONFIG_HARDWARE_EFFECT_FADE_IN_OUT]);
    _increment = _fadeStep;
  }
  _effectTimer = millis();
#ifdef DEBUG
  Serial << "OK";
#endif
}

void AFECLED::effectOff() {
  _currentEffect = AFE_CONFIG_HARDWARE_EFFECT_NO_EFFECTS;
}

void AFECLED::waveEffect(void) {
  if (millis() - _effectTimer >
      effects.effect[AFE_CONFIG_HARDWARE_EFFECT_WAVE].time) {
    if (_currentLedId == configuration.ledNumber - 1 || _currentLedId == 0) {
      _increment *= -1;
    }

    if (configuration.ledNumber == AFE_CONFIG_HARDWARE_CLED_8_LEDS) {
      leds8[_currentLedId] = CRGB::Black;
    } else {
      leds16[_currentLedId] = CRGB::Black;
    }
    _currentLedId += _increment;

    if (configuration.ledNumber == AFE_CONFIG_HARDWARE_CLED_8_LEDS) {
      leds8[_currentLedId] = _effectColor[AFE_CONFIG_HARDWARE_EFFECT_WAVE];
    } else {
      leds16[_currentLedId] = _effectColor[AFE_CONFIG_HARDWARE_EFFECT_WAVE];
    }

    FastLED.show();
    _effectTimer = millis();
  }
}

void AFECLED::fadeInOutEffect(void) {
  if (millis() - _effectTimer >
      AFE_CONFIG_HARDWARE_EFFECT_FADE_IN_OUT_DEFAULT_FADE_INTERNAL_LOOP_INTERVAL) {
    if (_currentBrightness >=
            effects.effect[AFE_CONFIG_HARDWARE_EFFECT_FADE_IN_OUT].brightness -
                _fadeStep - 1 ||
        _currentBrightness < _fadeStep + 1) {
      _increment = _increment > 0 ? -1 * _fadeStep : _fadeStep;
    }
    _currentBrightness += _increment;
    FastLED.setBrightness(_currentBrightness);
    FastLED.show();
    _effectTimer = millis();
  }
}

void AFECLED::setCustomEffectColor(uint8_t effectId, uint32_t color) {
  _effectColor[effectId] = color;
}

#endif // AFE_CONFIG_HARDWARE_CLED