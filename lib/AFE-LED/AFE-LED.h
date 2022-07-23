/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_LED_h
#define _AFE_LED_h

#include <AFE-Configuration.h>

#ifdef AFE_CONFIG_HARDWARE_LED

#include <AFE-Data-Access.h>
#include <AFE-Device.h>
#include <AFE-LED-Structure.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
#include <AFE-MCP23017-Broker.h>
#endif

class AFELED {

private:
  boolean _initialized = false;
  boolean _blinking = false;
  unsigned long _interval;
  unsigned long _previousMillis = 0;

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
  AFEMCP23017Broker *_MCP23017Broker;
  boolean _expanderUsed = false;
  uint8_t _MCP23017Id = AFE_HARDWARE_ITEM_NOT_EXIST;
#endif

  /* Method turns on/off LED */
  void set(uint8_t state);

public:
  LED configuration;
  /* Constructor */
  AFELED();
  boolean begin(AFEDataAccess *, uint8_t id);

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
  void addMCP23017Reference(AFEMCP23017Broker *);
#endif

  /* Turns on LED */
  void on();

  /* Turn off LED */
  void off();

  /* Method change the LED to opposite state */
  void toggle();

  /* Blink LED. Duration how lon LED is ON can be set by input parameter (in
   * milli)*/
  void blink(unsigned int duration = 100);

  /* Turns on LED blinking with interval as input paramters. It's in
   * milliseconds */
  void blinkingOn(unsigned long blinking_interval);

  /* Turns off LED blinking */
  void blinkingOff();

  /* Returns true if LED is blinking, false if not */
  boolean isBlinking();

  /* Method must be added to main loop in order to enable continues LED blinking
   */
  void loop();
};

#endif // AFE_CONFIG_HARDWARE_LED
#endif // _AFE_LED_h