#ifndef _AFE_hardware_rgb_led_h
#define _AFE_hardware_rgb_led_h

#include <FastLED.h>

#define AFE_CONFIG_HARDWARE_CLED_MAX_NUMBER_OF_LED 64 // Max number of LEDs

#define AFE_CONFIG_HARDWARE_CLED_0_GPIO 13
#define AFE_CONFIG_HARDWARE_CLED_1_GPIO 14

#define AFE_CONFIG_HARDWARE_CLED_CHIPSET WS2812
#define AFE_CONFIG_HARDWARE_CLED_COLORS_ORDER GRB

#define AFE_CONFIG_HARDWARE_CLED_DEFAULT_BRIGHTNESS 50

/* On/Off configuration */
#define AFE_CONFIG_HARDWARE_CLED_DEFAULT_ON_COLOR CRGB::Green
#define AFE_CONFIG_HARDWARE_CLED_DEFAULT_OFF_COLOR CRGB::Black
#define AFE_CONFIG_HARDWARE_CLED_DEFAULT_ON_BRIGHTNESS AFE_CONFIG_HARDWARE_CLED_COLORS_ORDER
#define AFE_CONFIG_HARDWARE_CLED_DEFAULT_OFF_BRIGHTNESS 0

/* Information: defaults for CLED Effects are in the config creation file */
#define AFE_CONFIG_HARDWARE_CLED_NUMBER_OF_EFFECTS 2 // Total number of CLED effects

/* CLED: Blinking effect */
#define AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING 0 
#define AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING_DEFAULT_ON_TIMER 100 // msec
#define AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING_DEFAULT_OFF_TIMER 1000 // msec
#define AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING_DEFAULT_ON_COLOR CRGB::Red
#define AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING_DEFAULT_ON_BRIGHTNESS AFE_CONFIG_HARDWARE_CLED_DEFAULT_BRIGHTNESS 
#define AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING_DEFAULT_OFF_COLOR CRGB::Black
#define AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING_DEFAULT_OFF_BRIGHTNESS 0 

/* CLED: Fade In/Out effect */
#define AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT 1
#define AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT_DEFAULT_IN_BRIGHTNESS AFE_CONFIG_HARDWARE_CLED_DEFAULT_BRIGHTNESS
#define AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT_DEFAULT_OUT_BRIGHTNESS 0
#define AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT_DEFAULT_FADE_TIMEOUT 2000
#define AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT_DEFAULT_FADE_INTERNAL_LOOP_INTERVAL 20 // Manages the smoothness of the fade. This is a time it takes to do other staff between CLED loop is triggered (msec)
#define AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT_DEFAULT_COLOR CRGB::Yellow

/* CLED: Wave effect */
#define AFE_CONFIG_HARDWARE_CLED_EFFECT_WAVE 2
#define AFE_CONFIG_HARDWARE_CLED_EFFECT_WAVE_DEFAULT_BRIGHTNESS AFE_CONFIG_HARDWARE_CLED_DEFAULT_BRIGHTNESS
#define AFE_CONFIG_HARDWARE_CLED_EFFECT_WAVE_DEFAULT_WAVE_TIMEOUT 50
#define AFE_CONFIG_HARDWARE_CLED_EFFECT_WAVE_DEFAULT_ON_COLOR CRGB::White
#define AFE_CONFIG_HARDWARE_CLED_EFFECT_WAVE_DEFAULT_OFF_COLOR CRGB::Black

#define AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_BACKLIGHT_LEVELS 3
#define AFE_CONFIG_HARDWARE_CLED_COLOR_MIFARE_UNAUTHORIZE CRGB::Red // Color shown if unauthorize card is used
#define AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_COLOR CRGB::White
#define AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_BRIGHTNESS 255

#define AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_SENSOR_TYPE_BH1750 0
#define AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_SENSOR_TYPE_TSL2561 1

#endif // _AFE_hardware_rgb_led_h