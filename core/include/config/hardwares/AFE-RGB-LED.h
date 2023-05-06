#ifndef _AFE_hardware_rgb_led_h
#define _AFE_hardware_rgb_led_h

#include <FastLED.h>

#if AFE_LANGUAGE == 0
#include <pl_PL.h>
#else
#include <en_EN.h>
#endif
/**
 * @brief Max supported LEDs
 * 3 meterw of 144 LEDs per 1m 
 */

#define AFE_CONFIG_HARDWARE_CLED_MAX_NUMBER_OF_LED 4*144

#define AFE_CONFIG_HARDWARE_CLED_0_GPIO 13
#define AFE_CONFIG_HARDWARE_CLED_1_GPIO 14

#define AFE_CONFIG_HARDWARE_CLED_CHIPSET WS2812
#define AFE_CONFIG_HARDWARE_CLED_COLORS_ORDER RGB
 
#define AFE_CONFIG_HARDWARE_CLED_MAX_BRIGHTNESS 255
#define AFE_CONFIG_HARDWARE_CLED_DEFAULT_BRIGHTNESS 50

#define AFE_CONFIG_HARDWARE_CLED_CMD_JSON_LENGTH 144
#define AFE_CONFIG_HARDWARE_CLED_STATE_JSON_LENGTH 170

/**
 * @brief types of conversion for brightness
 * 
 */
#define AFE_CONFIG_HARDWARE_CLED_BRIGHTNESS_CONVERSION_0_255 0
#define AFE_CONFIG_HARDWARE_CLED_BRIGHTNESS_CONVERSION_0_100 1
#define AFE_CONFIG_HARDWARE_CLED_BRIGHTNESS_CONVERSION_0_1 2
#define AFE_CONFIG_HARDWARE_CLED_DEFAULT_BRIGHTNESS_CONVERSION AFE_CONFIG_HARDWARE_CLED_BRIGHTNESS_CONVERSION_0_255

/**
 * @brief Domoticz specyfic control commands
 * 
 */
#define AFE_CONFIG_HARDWARE_CLED_DOMOTICZ_NVALUE_ON 1
#define AFE_CONFIG_HARDWARE_CLED_DOMOTICZ_NVALUE_OFF 0
#define AFE_CONFIG_HARDWARE_CLED_DOMOTICZ_NVALUE_COLOR 10
#define AFE_CONFIG_HARDWARE_CLED_DOMOTICZ_NVALUE_FULL_LIGHT 14
#define AFE_CONFIG_HARDWARE_CLED_DOMOTICZ_NVALUE_BRIGHTNESS 15
#define AFE_CONFIG_HARDWARE_CLED_DOMOTICZ_NVALUE_NIGHT_LIGHT 20

/**
 * @brief Light levels
 * 
 */
#define AFE_CONFIG_HARDWARE_CLED_LIGHT_LEVEL_FULL_LIGHT 255
#define AFE_CONFIG_HARDWARE_CLED_LIGHT_LEVEL_NIGHT_LIGHT 20

/* On/Off configuration */
#define AFE_CONFIG_HARDWARE_CLED_DEFAULT_ON_COLOR 255
#define AFE_CONFIG_HARDWARE_CLED_DEFAULT_OFF_COLOR 0
#define AFE_CONFIG_HARDWARE_CLED_DEFAULT_ON_BRIGHTNESS  AFE_CONFIG_HARDWARE_CLED_DEFAULT_BRIGHTNESS
#define AFE_CONFIG_HARDWARE_CLED_DEFAULT_OFF_BRIGHTNESS 0
#define AFE_CONFIG_HARDWARE_CLED_DEFAULT_CHANGE_TIME 300

#define AFE_CONFIG_HARDWARE_CLED_BRIGHTNESS_CMD_TOPIC_SUFIX "/brightness"

#define AFE_CONFIG_HARDWARE_CLED_EFFECT_NONE 0

#define AFE_CONFIG_HARDWARE_CLED_EFFECT_CMD_OFF L_CLED_EFFECT_OFF_COMMAND

/* CLED: Blinking effect */
#define AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING 10 
#define AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING_DEFAULT_ON_TIMER 100 // msec
#define AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING_DEFAULT_OFF_TIMER 1000 // msec
#define AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING_DEFAULT_ON_BRIGHTNESS AFE_CONFIG_HARDWARE_CLED_DEFAULT_BRIGHTNESS 
#define AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING_DEFAULT_OFF_BRIGHTNESS 0 

/* CLED: Fade In/Out effect */
#define AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT 20
#define AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT_DEFAULT_IN_BRIGHTNESS AFE_CONFIG_HARDWARE_CLED_DEFAULT_BRIGHTNESS
#define AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT_DEFAULT_OUT_BRIGHTNESS 0
#define AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT_DEFAULT_FADE_TIMEOUT 2000
#define AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT_DEFAULT_FADE_INTERNAL_LOOP_INTERVAL 20 // Manages the smoothness of the fade. This is a time it takes to do other staff between CLED loop is triggered (msec)


/* CLED: Wave effect */
#define AFE_CONFIG_HARDWARE_CLED_EFFECT_WAVE 30
#define AFE_CONFIG_HARDWARE_CLED_EFFECT_WAVE_DEFAULT_BRIGHTNESS AFE_CONFIG_HARDWARE_CLED_DEFAULT_BRIGHTNESS
#define AFE_CONFIG_HARDWARE_CLED_EFFECT_WAVE_DEFAULT_WAVE_TIMEOUT 50

#define AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_BACKLIGHT_LEVELS 3
#define AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_BRIGHTNESS 255

#define AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_SENSOR_TYPE_BH1750 0
#define AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_SENSOR_TYPE_TSL2561 1

#endif // _AFE_hardware_rgb_led_h