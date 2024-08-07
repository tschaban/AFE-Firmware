/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_COLOR_Structure_h
#define _AFE_COLOR_Structure_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_CLED

#include <Arduino.h>

struct CLED_RGB {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

struct CLED_HSB {
  uint16_t hue;
  uint8_t saturation;
  uint8_t brightness;
};

struct CLED_PARAMETERS {
  CLED_RGB color;
  uint8_t brightness;
  uint16_t changeTime;
};

#endif // AFE_CONFIG_HARDWARE_CLED
#endif // _AFE_COLOR_Structure_h
