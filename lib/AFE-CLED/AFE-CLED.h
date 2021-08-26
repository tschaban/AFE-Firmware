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
  boolean state[AFE_CONFIG_HARDWARE_CLED_NUMBER_OF_STRIPS] = {false,false};

  CLEDController *controllers[AFE_CONFIG_HARDWARE_CLED_NUMBER_OF_STRIPS];

  CRGB leds8[AFE_CONFIG_HARDWARE_CLED_8_LEDS];
  CRGB leds16[AFE_CONFIG_HARDWARE_CLED_16_LEDS];

  uint32_t _effectTimer = 0;
  
  /* Effect: wave */
  uint8_t _currentLedId = 1;
  int8_t _increment = 1;

  /* Effect: Fade In/Out */
  uint8_t _fadeStep = 1;
  uint32_t _effectColor[AFE_CONFIG_HARDWARE_EFFECT_NO_EFFECTS];

  uint8_t _currentBrightness[AFE_CONFIG_HARDWARE_CLED_NUMBER_OF_STRIPS] = {0,0};
  uint32_t _currentColor[AFE_CONFIG_HARDWARE_CLED_NUMBER_OF_STRIPS] = {0,0};

  uint32_t _offColor[AFE_CONFIG_HARDWARE_CLED_NUMBER_OF_STRIPS] = {AFE_CONFIG_HARDWARE_CLED_OFF_COLOR,AFE_CONFIG_HARDWARE_CLED_OFF_COLOR};
  uint32_t _onColor[AFE_CONFIG_HARDWARE_CLED_NUMBER_OF_STRIPS] = {AFE_CONFIG_HARDWARE_CLED_ON_COLOR,AFE_CONFIG_HARDWARE_CLED_ON_COLOR};

  /* Set LED brightness */
  void setBrightness(uint8_t stripId, uint8_t level);

  /* Set Color for all leds in the string */
  void setColor(uint8_t stripId, uint32_t color);

  /* Handles Leds wave effect */
  void waveEffect(uint8_t stripId);

  /* Handles Fade In/Out effect */
  void fadeInOutEffect(uint8_t stripId);

public:
  CLED configuration[AFE_CONFIG_HARDWARE_CLED_NUMBER_OF_STRIPS];

  CLED_EFFECTS effects;
  CLED_BACKLIGHT backlight;
  uint8_t lightSensorType = AFE_HARDWARE_ITEM_NOT_EXIST;

  uint8_t _currentEffect = AFE_CONFIG_HARDWARE_EFFECT_NO_EFFECTS;

  /* Constructor */
  AFECLED();
  boolean begin(AFEDataAccess *);

  /* Turns on CLED */
  void on(uint8_t stripId);
  void on(uint8_t stripId, uint32_t color);

  /* Turn off CLED */
  void off(uint8_t stripId);
  void off(uint8_t stripId, uint32_t color);

  /* Method change the CLED to opposite state */
  void toggle(uint8_t stripId, uint32_t color);

  /* Blink CLED. Duration how lon CLED is ON can be set by input parameter (in
   * milli)*/
  void blink(uint8_t stripId, unsigned int duration = 100,
             uint32_t onColor = AFE_CONFIG_HARDWARE_CLED_ON_COLOR,
             uint32_t offColor = AFE_CONFIG_HARDWARE_CLED_OFF_COLOR);

  /* Method must be added to main loop in order to enable effects  */
  void loop();

  void effectOn(uint8_t stripId, uint8_t effectId);
  void effectOff(uint8_t stripId);

  /* It set's custom effect color */
  void setCustomEffectColor(uint8_t stripId, uint8_t effectId, uint32_t color);

  void backlightEffect(uint8_t stripId, uint32_t lightLevel);

};

#endif // AFE_CONFIG_HARDWARE_CLED
#endif // _AFE_CLED_h