/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_LED_Structure_h
#define _AFE_LED_Structure_h

struct LED {
  uint8_t gpio;
  boolean changeToOppositeValue;
};

#endif
