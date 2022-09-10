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
  /**
   * @brief structure that keeps effects current state
   *
   */
  struct CLED_EFFECT_CONFIG {
    uint8_t id = AFE_CONFIG_HARDWARE_CLED_EFFECT_NONE;
    boolean stateUpdated = false;
    unsigned long timer;
    int8_t increment;
    uint8_t integer;
  };

  /**
   * @brief data structure that keeps current state and other information
   * releated to a LED strip
   *
   */
  struct CLED_CURRENT_STATE {
    CLED_PARAMETERS config;
    boolean state = false;
    boolean stateUpdated = false;
    CLED_PARAMETERS off;
    CLED_PARAMETERS on;
    CLED_EFFECT_CONFIG effect;
    uint8_t slowChangeTargetBrightness;
  };

  boolean _initialized = false;
  AFEDataAccess *_Data;
  AFEDevice *_Device;

  /**
   * @brief it stories each led color and its brightness
   *
   */
  CRGB leds[AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_STRIPS]
           [AFE_CONFIG_HARDWARE_CLED_MAX_NUMBER_OF_LED];

  /**
   * @brief Turns On/Off LED based on the settings in the currentState variable
   *
   * @param  stripId          ID of a LED strip
   * @param  state            On or Off: AFE_ON, AFE_OFF
   * @param  disableEffects   Is true it runs a code of disabling effects
   */
  void _turnOnOff(uint8_t stripId, boolean state,
                  boolean disableEffects = false);


  /**
   * @brief Set Color for all leds in the strip. It uses last set brightness
   * level
   *
   * @param  stripId          ID of a LED strip
   * @param  color            a color in RGB
   */
  void _setColor(uint8_t stripId, CLED_RGB color);

  /**
   * @brief Set Color and brightness for all leds in the strip
   *
   * @param  stripId          ID of a LED strip
   * @param  ledConfig        RGB + brightness
   */
  void _setColor(uint8_t stripId, CLED_PARAMETERS ledConfig);

  /**
   * @brief set color for led strip. Led parameters defined in currentState
   * variable
   *
   * @param  stripId          ID of a LED strip
   */
  void _setColor(uint8_t stripId);

  /**
   * @brief set color for led strip.
   *
   * @param  stripId          ID of a LED strip
   * @param  color            RGB color
   * @param  brightness       brightness
   */
  void _setColor(uint8_t stripId, CLED_RGB color, uint8_t brightness);


  /**
   * @brief methods required in the main loop to enable effects
   * 
   * @param  stripId          ID strip to deactivate an effect on
   */
  void effectBlinkingListener(uint8_t stripId);
  void effectFadeInOutListener(uint8_t stripId);
  void effectWaveListener(uint8_t stripId);
  void effectSlowBrightness(uint8_t stripId);

public:
  /**
   * @brief Strip LED configuration parameters
   *
   */
  CLED configuration[AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_STRIPS];

  /**
   * @brief Effect: flashing configuration parameters
   *
   */
  CLED_EFFECT_BLINKING
  configurationEffectBlinking[AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_STRIPS];

  /**
   * @brief Effect: wave configuration parameters
   *
   */
  CLED_EFFECT_WAVE
  configurationEffectWave[AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_STRIPS];

  /**
   * @brief Effect: fade in/out configuration parameters
   *
   */
  CLED_EFFECT_FADE_INOUT
  configurationEffectFadeInOut[AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_STRIPS];

  /**
   * @brief Stores LED strip current state parameters
   *
   */
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
  boolean begin(AFEDataAccess *Data, AFEDevice *Device);

  /**
   * @brief Turns ON LED strip: color and brightness from the configuration file
   *
   * @param  stripId          ID of the LED Strip
   * @param  disableEffects   if true methods disables effects
   */
  void on(uint8_t stripId, boolean disableEffects = false);

  /**
   * @brief Turns ON LED strip: color as parameter and brightness the last one
   * used
   *
   * @param  stripId          ID of the LED Strip
   * @param  color            Color RGB
   * @param  disableEffects   if true methods disables effects
   */
  void on(uint8_t stripId, CLED_RGB color, boolean disableEffects = false);

  /**
  * @brief Turns ON LED strip: color and brightness as parameters
  *
  * @param  stripId          ID of the LED Strip
  * @param  ledConfig        Coler RGB + Brightness 0 .. 255
  * @param  disableEffects   if true methods disables effects
  * @param  saveColor        if true color and brigtness is saved
  */
  void on(uint8_t stripId, CLED_PARAMETERS ledConfig,
          boolean disableEffects = false, boolean saveColor = false);

  /**
   * @brief Turns Off LED strip
   * 
   * @param  stripId          ID of the LED Strip
   * @param  disableEffects   if true methods disables effects
   */
  void off(uint8_t stripId, boolean disableEffects = false);

  /**
   * @brief Turns Off LED strip
   * 
   * @param  stripId          ID of the LED Strip
   * @param  color            Color RGB
   * @param  disableEffects   if true methods disables effects
   */
  void off(uint8_t stripId, CLED_RGB color, boolean disableEffects = false);
  
  /**
   * @brief Turns Off LED strip
   * 
   * @param  stripId          ID of the LED Strip
   * @param  ledConfig        Color RGB + brightness
   * @param  disableEffects   if true methods disables effects
   */
  void off(uint8_t stripId, CLED_PARAMETERS ledConfig,
           boolean disableEffects = false);

  /**
   * @brief method turns on/off strip, it can disable or change color
   * 
   * @param  stripId          ID of the LED Strip
   * @param  disableEffects   if true methods disables effects
   */
  void toggle(uint8_t stripId, boolean disableEffects = false);
  void toggle(uint8_t stripId, CLED_RGB color, boolean disableEffects = false);

/**
 * @brief method activate effect
 * 
 * @param  stripId          ID strip to activate an effect on
 * @param  effectId         effect id, ifs in AFE-RGB-LED.h
 */
  void activateEffect(uint8_t stripId, uint8_t effectId);
  
/**
 * @brief method deactivates effects
 * 
 * @param  stripId          ID strip to deactivate an effect on
 * @param  setToOff         if true it turns off leds strip
 */
  void deactivateEffect(uint8_t stripId, boolean setToOff = true);
  
  /**
   * @brief returns true is LED strip changed from ON->OFF or from OFF->ON
   *
   * @param  stripId          ID of a LED Strip
   * @return boolean          true - state changed, false - no change
   */
  boolean isStateUpdated(uint8_t stripId);

  /**
   * @brief returns true is LED strip effect has been changed eg from ON to OFF,
   * effect changed etc
   *
   * @param  stripId          ID of a LED Strip
   * @return boolean          true - state changed, false - no change
   */
  boolean isEffectStateUpdated(uint8_t stripId);

  /**
   * @brief Method must be added to main loop in order to enable effects
   *
   */
  void loop();

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_STANDARD
  /**
   * @brief converts brightness from 0..255 to 0..100 or 0..1 depending on
   * coniguration settings
   *
   * @param  stripId          ID of a LED Strip
   * @param  brightness       brigtness in 0..255
   * @return float            converted value of brightness
   */
  float convertBrigtnessToAPI(uint8_t stripId, uint8_t brightness);

  /**
   * @brief converts brightess from float 0..1 or 0..100 to 0..255 depending on
   * coniguration settings
   *
   * @param  stripId          ID of a LED Strip
   * @param  brightness       input brightness
   * @return uint8_t          converted value of brightness
   */
  uint8_t convertBrightnessFromAPI(uint8_t stripId, float brightness);
#endif // AFE_FIRMWARE_API_STANDARD
};

#endif // AFE_CONFIG_HARDWARE_CLED
#endif // _AFE_CLED_h