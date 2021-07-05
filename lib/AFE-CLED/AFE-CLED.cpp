/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-CLED.h"

#ifdef AFE_CONFIG_HARDWARE_CLED

AFECLED::AFECLED() {}

boolean AFECLED::begin(AFEDataAccess *Data, uint8_t id) {
  if (id != AFE_HARDWARE_ITEM_NOT_EXIST) {
    Data->getConfiguration(id, &configuration);
    Data->getConfiguration(id, &effects);

    FastLED
        .addLeds<AFE_CONFIG_HARDWARE_CLED_CHIPSET,
                 AFE_CONFIG_HARDWARE_CLED_GPIO,
                 AFE_CONFIG_HARDWARE_CLED_COLOLRS_ORDER>(
            leds, AFE_CONFIG_HARDWARE_CLED_LEDS_NUMBER)
        .setCorrection(TypicalSMD5050);

    /* Effect: fade in/out calcuating step */
    _fadeStep = round(effects.effect[1].brightness / (
                      effects.effect[1].time / 2 /
                      AFE_CONFIG_HARDWARE_EFFECT_FADE_IN_OUT_INTERVAL));
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
    leds[i] = color;
  }
}

void AFECLED::effectOn(uint8_t effectId) {
  _currentEffect = effectId;
  if (_currentEffect == AFE_CONFIG_HARDWARE_EFFECT_WAVE) {
    setColor(CRGB::Black);
    FastLED.setBrightness(effects.effect[0].brightness);
    _increment = 1;
  } else if (_currentEffect == AFE_CONFIG_HARDWARE_EFFECT_FADE_IN_OUT) {
    _currentBrightness = _fadeStep + 1;
    FastLED.setBrightness(_currentBrightness);
    setColor(effects.effect[1].color);
    _increment = _fadeStep;
  }
  _effectTimer = millis();
}

void AFECLED::effectOff() {
  _currentEffect = AFE_CONFIG_HARDWARE_EFFECT_NO_EFFECTS;
}

void AFECLED::waveEffect(void) {
  if (millis() - _effectTimer > effects.effect[0].time) {
    if (_currentLedId == configuration.ledNumber - 1 || _currentLedId == 0) {
      _increment *= -1;
    }
    leds[_currentLedId] = CRGB::Black;
    _currentLedId += _increment;
    leds[_currentLedId] = effects.effect[0].color;
    FastLED.show();
    _effectTimer = millis();
  }
}

void AFECLED::fadeInOutEffect(void) {
  if (millis() - _effectTimer > 50) {
    if (_currentBrightness >=
            effects.effect[1].brightness - _fadeStep - 1 ||
        _currentBrightness < _fadeStep + 1) {
      _increment = _increment > 0 ? -1 * _fadeStep : _fadeStep;
    }

    _currentBrightness += _increment;
    FastLED.setBrightness(_currentBrightness);
    FastLED.show();
    _effectTimer = millis();
  }
}

#endif // AFE_CONFIG_HARDWARE_CLED