
/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-CLED.h"

#ifdef AFE_CONFIG_HARDWARE_CLED

AFECLED::AFECLED() {}

boolean AFECLED::begin(AFEDataAccess *Data) {

#ifdef DEBUG
  Serial << endl << "INFO: CLED: Initializing CLED...";
#endif

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_STRIPS; i++) {
    Data->getConfiguration(i, &configuration[i]);
    Data->getConfiguration(i, &configurationEffectBlinking[i]);
    Data->getConfiguration(i, &configurationEffectWave[i]);
    Data->getConfiguration(i, &configurationEffectFadeInOut[i]);
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
    currentState[i].on.color = configuration[i].on.color;
    currentState[i].on.brightness = configuration[i].on.brightness;
    currentState[i].off.color = configuration[i].off.color;
    currentState[i].off.brightness = configuration[i].off.brightness;

  }

  _initialized = true;

  return _initialized;
}

void AFECLED::on(uint8_t stripId, boolean disableEffects) {
  on(stripId, configuration[stripId].on.color,
     configuration[stripId].on.brightness, disableEffects);
}
void AFECLED::off(uint8_t stripId, boolean disableEffects) {
  off(stripId, configuration[stripId].off.color,
      configuration[stripId].off.brightness, disableEffects);
}

void AFECLED::on(uint8_t stripId, uint32_t color, boolean disableEffects) {
  on(stripId, color, currentState[stripId].on.brightness, disableEffects);
}
void AFECLED::off(uint8_t stripId, uint32_t color, boolean disableEffects) {
  off(stripId, color, currentState[stripId].off.brightness, disableEffects);
}

void AFECLED::on(uint8_t stripId, uint32_t color, uint8_t brightness,
                 boolean disableEffects) {
  currentState[stripId].on.color = color;
  currentState[stripId].on.brightness = brightness;
  _turnOnOff(stripId, true, disableEffects);
}
void AFECLED::off(uint8_t stripId, uint32_t color, uint8_t brightness,
                  boolean disableEffects) {
  currentState[stripId].off.color = color;
  currentState[stripId].off.brightness = brightness;
  _turnOnOff(stripId, false, disableEffects);
}

void AFECLED::activateEffect(uint8_t stripId, uint8_t effectId) {
  currentState[stripId].effect.id = effectId;
  currentState[stripId].effect.timer = 0;
  currentState[stripId].state = false;
  currentState[stripId].stateUpdated = true;
  currentState[stripId].effect.stateUpdated = true;
  switch (currentState[stripId].effect.id) {
  case AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING:
    /* no special configuration required */
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
  currentState[stripId].effect.id = AFE_NONE;
  currentState[stripId].effect.stateUpdated = true;
  if (setToOff) {
    off(stripId, AFE_CONFIG_HARDWARE_CLED_DEFAULT_OFF_COLOR,
        AFE_CONFIG_HARDWARE_CLED_DEFAULT_OFF_BRIGHTNESS, true);
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
        off(stripId, configurationEffectBlinking[stripId].off.color,
            configurationEffectBlinking[stripId].off.brightness);
      } else {
        on(stripId, configurationEffectBlinking[stripId].on.color,
           configurationEffectBlinking[stripId].on.brightness);
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

      _setColor(stripId, configurationEffectFadeInOut[stripId].in.color,
                currentState[stripId].config.brightness);
      currentState[stripId].effect.timer = millis();
    }
  }
}

void AFECLED::effectWaveListener(uint8_t stripId) {
  if (millis() - currentState[stripId].effect.timer >
      configurationEffectWave[stripId].timeout) {

    if (currentState[stripId].effect.integer == FastLED[stripId].size() - 1 ||
        currentState[stripId].effect.integer == 0) {
      currentState[stripId].effect.increment *= -1;
    }

    leds[stripId][currentState[stripId].effect.integer] =
        configurationEffectWave[stripId].off.color;

    currentState[stripId].effect.integer +=
        currentState[stripId].effect.increment;

    leds[stripId][currentState[stripId].effect.integer] =
        configurationEffectWave[stripId].on.color;

    FastLED[stripId].show(leds[stripId], FastLED[stripId].size(),
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

void AFECLED::toggle(uint8_t stripId, uint32_t color) {
  currentState[stripId].state ? off(stripId) : on(stripId);
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
    if (disableEffects && currentState[stripId].effect.id != AFE_NONE) {
      deactivateEffect(stripId,false);
    }
  }
}

void AFECLED::_setBrightness(uint8_t stripId, uint8_t brightness) {
  _setColor(stripId, currentState[stripId].config.color, brightness);
}

void AFECLED::_setColor(uint8_t stripId, uint32_t color) {
  _setColor(stripId, color, currentState[stripId].config.brightness);
}
void AFECLED::_setColor(uint8_t stripId, uint32_t color, uint8_t brightness) {
  currentState[stripId].config.color = color;
  currentState[stripId].config.brightness = brightness;
  FastLED[stripId].showColor(currentState[stripId].config.color,
                             currentState[stripId].config.brightness);
}

#endif // AFE_CONFIG_HARDWARE_CLED