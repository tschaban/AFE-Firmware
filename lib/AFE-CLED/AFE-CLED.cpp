#include "AFE-CLED.h"

#ifdef AFE_CONFIG_HARDWARE_CLED

AFECLED::AFECLED() {}

boolean AFECLED::begin(AFEDataAccess *Data) {

#ifdef DEBUG
  Serial << endl << "INFO: CLED: Initializing CLED...";
#endif
  _Data = Data;
  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_STRIPS; i++) {
    _Data->getConfiguration(i, &configuration[i]);
    _Data->getConfiguration(i, &configurationEffectBlinking[i]);
    _Data->getConfiguration(i, &configurationEffectWave[i]);
    _Data->getConfiguration(i, &configurationEffectFadeInOut[i]);
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

#ifdef DEBUG
  Serial << endl << "INFO: CLED: Setting default parameters....";
#endif

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_STRIPS; i++) {
    _writeColor(currentState[i].on.color, &configuration[i].on.color);
    currentState[i].on.brightness = configuration[i].on.brightness;
    _writeColor(currentState[i].off.color, &configuration[i].off.color);
    currentState[i].off.brightness = configuration[i].off.brightness;
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
  _turnOnOff(stripId, true, disableEffects);
}
void AFECLED::off(uint8_t stripId, boolean disableEffects) {
  _turnOnOff(stripId, false, disableEffects);
}

void AFECLED::on(uint8_t stripId, CLED_RGB color, boolean disableEffects) {
  _writeColor(currentState[stripId].on.color, &color);
  on(stripId, disableEffects);
}
void AFECLED::off(uint8_t stripId, CLED_RGB color, boolean disableEffects) {
  _writeColor(currentState[stripId].off.color, &color);
  off(stripId, disableEffects);
}

void AFECLED::on(uint8_t stripId, CLED_PARAMETERS ledConfig,
                 boolean disableEffects, boolean saveColor) {
  _writeColor(currentState[stripId].on.color, &ledConfig.color);
  currentState[stripId].on.brightness = ledConfig.brightness;
  on(stripId, disableEffects);
  if (saveColor) {
    configuration->on.color.blue = ledConfig.color.blue;
    configuration->on.color.green = ledConfig.color.green;
    configuration->on.color.red = ledConfig.color.red;
    configuration->on.brightness = ledConfig.brightness;
    _Data->saveConfiguration(stripId, configuration);
  }
}
void AFECLED::off(uint8_t stripId, CLED_PARAMETERS ledConfig,
                  boolean disableEffects) {
  _writeColor(currentState[stripId].off.color, &ledConfig.color);
  currentState[stripId].off.brightness = ledConfig.brightness;
  off(stripId, disableEffects);
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
    _writeColor(currentState[stripId].config.color,
                &configurationEffectFadeInOut[stripId].in.color);

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

    Serial << endl
           << "##############: " << currentState[stripId].effect.integer;

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
    off(stripId, true);
  }
}

void AFECLED::effectBlinkingListener(uint8_t stripId) {
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
        off(stripId, configurationEffectBlinking[stripId].off);
      } else {
        on(stripId, configurationEffectBlinking[stripId].on);
      }

      currentState[stripId].effect.timer = millis();
    }
  }
}

void AFECLED::effectFadeInOutListener(uint8_t stripId) {
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

void AFECLED::effectWaveListener(uint8_t stripId) {
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
    for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_STRIPS; i++) {
      switch (currentState[i].effect.id) {
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

void AFECLED::toggle(uint8_t stripId, boolean disableEffects) {
  currentState[stripId].state
      ? off(stripId, currentState[stripId].off.color, disableEffects)
      : on(stripId, currentState[stripId].on.color, disableEffects);
}

void AFECLED::toggle(uint8_t stripId, CLED_RGB color, boolean disableEffects) {
  currentState[stripId].state ? off(stripId, color, disableEffects)
                              : on(stripId, color, disableEffects);
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

void AFECLED::_setBrightness(uint8_t stripId, uint8_t brightness) {
  _setColor(stripId, currentState[stripId].config.color, brightness);
}

void AFECLED::_setColor(uint8_t stripId) {
  FastLED[stripId].showColor(CRGB(currentState[stripId].config.color.red,
                                  currentState[stripId].config.color.green,
                                  currentState[stripId].config.color.blue),
                             configuration[stripId].ledNumbers,
                             currentState[stripId].config.brightness);
}

void AFECLED::_setColor(uint8_t stripId, CLED_RGB color, uint8_t brightness) {
  _writeColor(currentState[stripId].config.color, &color);
  currentState[stripId].config.brightness = brightness;
  _setColor(stripId);
}

void AFECLED::_setColor(uint8_t stripId, CLED_RGB color) {
  _writeColor(currentState[stripId].config.color, &color);
  _setColor(stripId);
}
void AFECLED::_setColor(uint8_t stripId, CLED_PARAMETERS ledConfig) {
  _writeColor(currentState[stripId].config.color, &ledConfig.color);
  currentState[stripId].config.brightness = ledConfig.brightness;
  _setColor(stripId);
}

void AFECLED::_writeColor(CLED_RGB &to, CLED_RGB *from) {
  to.red = from->red;
  to.green = from->green;
  to.blue = from->blue;
}

#endif // AFE_CONFIG_HARDWARE_CLED