/**
 * @file AFE-CLED.h
 * @author Tschaban-A (github@adrian.czabanowski.com)
 * @brief Controls RGB LEDs WS281x
 * @version 3.1.0
 * @date 2021-10-17
 *
 * @copyright AFE-FIRMWARE (c) 2021
 *
 */

#ifndef _AFE_CLED_h
#define _AFE_CLED_h

#include <AFE-Configuration.h>

#ifdef AFE_CONFIG_HARDWARE_CLED

#include <AFE-CLED-Structure.h>
#include <AFE-Data-Access.h>
#include <AFE-Device.h>
#include <FastLED.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFECLED {

private:
  struct CLED_EFFECT_CONFIG {
    uint8_t id = AFE_NONE;
    boolean stateUpdated = false;
    unsigned long timer;
    int8_t increment;
    uint8_t integer;
  };

  struct CLED_CURRENT_STATE {
    CLED_PARAMETERS config;
    boolean state = false;
    boolean stateUpdated = false;
    CLED_PARAMETERS off;
    CLED_PARAMETERS on;
    CLED_EFFECT_CONFIG effect;
  };

  boolean _initialized = false;

  CLEDController *controllers[AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_STRIPS];

  CRGB leds[AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_STRIPS]
           [AFE_CONFIG_HARDWARE_CLED_MAX_NUMBER_OF_LED];

  /* Tunes On/Off CLEDs */
  void _turnOnOff(uint8_t stripId, boolean state,
                  boolean disableEffects = false);

  /* Set LED brightness */
  void _setBrightness(uint8_t stripId, uint8_t brightness);

  /* Set Color and brightness for all leds in the string */
  void _setColor(uint8_t stripId, uint32_t color);
  void _setColor(uint8_t stripId, uint32_t color, uint8_t brightness);

public:
  CLED configuration[AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_STRIPS];
  CLED_EFFECT_BLINKING
  configurationEffectBlinking[AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_STRIPS];
  CLED_EFFECT_WAVE
  configurationEffectWave[AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_STRIPS];
  CLED_EFFECT_FADE_INOUT
  configurationEffectFadeInOut[AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_STRIPS];
  CLED_CURRENT_STATE currentState[AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_STRIPS];

  /**
   * @brief Construct a new AFECLED object
   *
   */
  AFECLED();

  /**
   * @brief Initialize all RGB LED string
   *
   * @param  Data             Reference to data access API
   * @return boolean          True: when succesfully initialized
   */
  boolean begin(AFEDataAccess *Data);

  /**
   * @brief Turns ON LED strip: color and brightness from the configuration file
   *
   * @param  stripId          ID of the LED Strip
   */
  void on(uint8_t stripId, boolean disableEffects = false);

  /**
   * @brief Turns ON LED strip: color as parameter and brightness the last one
   * used
   *
   * @param  stripId          ID of the LED Strip
   * @param  color            Color ID
   */
  void on(uint8_t stripId, uint32_t color, boolean disableEffects = false);

  /**
   * @brief Turns ON LED strip: color and brightness as parameters
   *
   * @param  stripId          ID of the LED Strip
   * @param  color            Color ID
   * @param  brightness       Brightness 0 .. 255
   */
  void on(uint8_t stripId, uint32_t color, uint8_t brightness,
          boolean disableEffects = false);

  /* Turn off CLED */
  void off(uint8_t stripId, boolean disableEffects = false);
  void off(uint8_t stripId, uint32_t color, boolean disableEffects = false);
  void off(uint8_t stripId, uint32_t color, uint8_t brightness,
           boolean disableEffects = false);

  /* Method change the CLED to opposite state */
  void toggle(uint8_t stripId, uint32_t color);

  /* Effects */
  void activateEffect(uint8_t stripId, uint8_t effectId);
  void deactivateEffect(uint8_t stripId, boolean setToOff = true);
  void effectBlinkingListener(uint8_t stripId);
  void effectFadeInOutListener(uint8_t stripId);
  void effectWaveListener(uint8_t stripId);

  /* Events triggeres */
  boolean isStateUpdated(uint8_t stripId);
  boolean isEffectStateUpdated(uint8_t stripId);

  /* Method must be added to main loop in order to enable effects  */
  void loop();
};

#endif // AFE_CONFIG_HARDWARE_CLED
#endif // _AFE_CLED_h