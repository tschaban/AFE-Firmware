/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#ifndef _AFE_Relay_h
#define _AFE_Relay_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>
#include <AFE-Led.h>
// #include <Streaming.h>

class AFERelay {

private:
  uint8_t _id;
  AFEDataAccess Data; // @TODO nie jest konsekwentnie jak np. w switch
  RELAY RelayConfiguration;
  AFELED Led;
  char mqttTopic[50];
  unsigned long turnOffCounter = 0;

  /* Method set relay state after power restore or connection to MQTT is
   * established */
  void setRelayAfterRestore(uint8_t option);

public:
  /* Constructors */
  AFERelay();
  AFERelay(uint8_t id);

  /* Method: initiates relay */
  void begin(uint8_t id);

  /* Method returns MQTT topic for this relay */
  const char *getMQTTTopic();

  /* Method sets relay state after device is turned on / power is restored / or
   * after device has been crash */
  void setRelayAfterRestoringPower();

  /* Method sets relay state after device is connected / reconnected to MQTT
   * Broker. It returns TRUE if relay state has been set, false it relay state
   * should be manged through MQTT Broker*/
  boolean setRelayAfterRestoringMQTTConnection();

  /* Method: Returns RELAY_OFF if relay is OFF, RELAY_ON if relay is ON */
  byte get();

  /* Turns on relay */
  void on(boolean invert = false);

  /* Turns off relay */
  void off(boolean invert = false);

  /* Toggles relay state from ON to OFF or from OFF to ON */
  void toggle();

  /* Methodswhile added to main loop turns off relay automatically. Duration
   * how long relay should be on must be configured */
  boolean autoTurnOff(boolean invert = false);

  /* Methods returns relay name */
  const char *getName();

  /* It sets timer to auto-switch of the relay */
  void setTimer(float timer);

  /* It removed timer for auto-switch of the relay */
  void clearTimer();

  /* It returns ID of the LED that shoud indicated Relay status */
  uint8_t getControlledLedID();
};

#endif
