/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

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
  boolean _initialized = false;
  boolean state = false;

  CRGB leds8[AFE_CONFIG_HARDWARE_CLED_8_LEDS];
  CRGB leds16[AFE_CONFIG_HARDWARE_CLED_16_LEDS];

  uint32_t _effectTimer = 0;
  uint8_t _currentEffect = AFE_CONFIG_HARDWARE_EFFECT_NO_EFFECTS;

  /* Effect: wave */
  uint8_t _currentLedId = 1;
  int8_t _increment = 1;

  /* Effect: Fade In/Out */
  uint8_t _currentBrightness = 0;
  uint8_t _fadeStep = 1;

  uint32_t _effectColor[AFE_CONFIG_HARDWARE_EFFECT_NO_EFFECTS];

  uint32_t _offColor = AFE_CONFIG_HARDWARE_CLED_OFF_COLOR;
  uint32_t _onColor = AFE_CONFIG_HARDWARE_CLED_ON_COLOR;

  /* Set LED brightness */
  void setBrightness(uint8_t level);

  /* Set Color for all leds in the string */
  void setColor(uint32_t color);

  /* Handles Leds wave effect */
  void waveEffect(void);

  /* Handles Fade In/Out effect */
  void fadeInOutEffect(void);


public:
  CLED configuration;
  CLED_EFFECTS effects;

  /* Constructor */
  AFECLED();
  boolean begin(AFEDataAccess *, uint8_t id);

  /* Turns on CLED */
  void on();
  void on(uint32_t color);

  /* Turn off CLED */
  void off();
  void off(uint32_t color);

  /* Method change the CLED to opposite state */
  void toggle(uint32_t color);

  /* Blink CLED. Duration how lon CLED is ON can be set by input parameter (in
   * milli)*/
  void blink(unsigned int duration = 100,
             uint32_t onColor = AFE_CONFIG_HARDWARE_CLED_ON_COLOR,
             uint32_t offColor = AFE_CONFIG_HARDWARE_CLED_OFF_COLOR);

  /* Method must be added to main loop in order to enable effects  */
  void loop();
  void effectOn(uint8_t effectId);
  void effectOff(void);

/* It set's custom effect color */
  void setCustomEffectColor(uint8_t effectId, uint32_t color);

};

#endif // AFE_CONFIG_HARDWARE_CLED
#endif // _AFE_CLED_h