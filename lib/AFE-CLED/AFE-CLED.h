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
  struct CLED_EFFECT_FADE_IN_OUT {
    int8_t increment;
    uint8_t step;
  };

    struct CLED_EFFECT_WAVE {
    int8_t increment;
    uint8_t ledId;
  };

  struct CLED_EFFECT_CONFIG {
    uint8_t id = AFE_NONE;
    unsigned long timer;
    CLED_EFFECT_FADE_IN_OUT fadeInOut;
    CLED_EFFECT_WAVE wave;
  };

  struct CLED_CURRENT_STATE {
    CLED_PARAMETERS config;
    boolean state = false;
    CLED_PARAMETERS off;
    CLED_PARAMETERS on;
    CLED_EFFECT_CONFIG effect;
  };

  boolean _initialized = false;

  CLED configuration[AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_STRIPS];
  CLEDController *controllers[AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_STRIPS];

  CRGB leds[AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_STRIPS]
           [AFE_CONFIG_HARDWARE_CLED_MAX_NUMBER_OF_LED];

  CLED_CURRENT_STATE _currentState[AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_STRIPS];
  

  /* Tunes On/Off CLEDs */
  void _turnOnOff(uint8_t stripId, boolean state);

  /* Set LED brightness */
  void _setBrightness(uint8_t stripId, uint8_t brightness);

  /* Set Color and brightness for all leds in the string */
  void _setColor(uint8_t stripId, uint32_t color);
  void _setColor(uint8_t stripId, uint32_t color, uint8_t brightness);

public:
  //CLED configuration[AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_STRIPS];

#ifdef AFE_CONFIG_HARDWARE_CLED_ON_OFF_EFFECT

#endif

  /* Constructor */
  AFECLED();
  boolean begin(AFEDataAccess *);

  /* Turns on CLED */
  void on(uint8_t stripId);
  void on(uint8_t stripId, uint32_t color);
  void on(uint8_t stripId, uint32_t color, uint8_t brightness);

  /* Turn off CLED */
  void off(uint8_t stripId);
  void off(uint8_t stripId, uint32_t color);
  void off(uint8_t stripId, uint32_t color, uint8_t brightness);

  /* Method change the CLED to opposite state */
  void toggle(uint8_t stripId, uint32_t color);

  /* Effects */
  void activateEffect(uint8_t stripId, uint8_t effectId);
  void deactivateEffect(uint8_t stripId, boolean setToOff = true);
  void effectBlinkingListener(uint8_t stripId);
  void effectFadeInOutListener(uint8_t stripId);
  void effectWaveListener(uint8_t stripId);

  /* Method must be added to main loop in order to enable effects  */
  void loop();
};

#endif // AFE_CONFIG_HARDWARE_CLED
#endif // _AFE_CLED_h