/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_Relay_h
#define _AFE_Relay_h

#include <AFE-Configuration.h>

#ifdef AFE_CONFIG_HARDWARE_RELAY

//#include <arduino.h>
#include <AFE-Data-Access.h>
#include <AFE-MQTT-Structure.h>

#ifdef AFE_CONFIG_HARDWARE_LED
#include <AFE-LED.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_MCP23017
#include <AFE-MCP23017-Broker.h>
#endif

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFERelay {

private:
  uint8_t _id;
  AFEDataAccess *_Data; // @TODO nie jest konsekwentnie jak np. w switch

#ifdef AFE_CONFIG_HARDWARE_LED
  AFELED Led;
#endif

  unsigned long turnOffCounter = 0;
#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
  boolean timerUnitInSeconds = true;
#endif

#ifdef AFE_CONFIG_HARDWARE_MCP23017
  AFEMCP23017Broker *_MCP23017Broker;
  boolean _MCP23017ReferenceAdded = false;
  boolean _expanderUsed = false;
  uint8_t _MCP23017Id = AFE_HARDWARE_ITEM_NOT_EXIST;
#endif

  /* Method set relay state after power restore or connection to MQTT is
   * established */

  void setRelayAfterRestore(uint8_t option);

public:
#ifdef AFE_CONFIG_HARDWARE_GATE
  uint8_t gateId = AFE_HARDWARE_ITEM_NOT_EXIST;
#endif
  RELAY configuration;

  /* Constructors */
  AFERelay();

  /* Method: initiates relay */
  void begin(AFEDataAccess *, uint8_t id);

#ifdef AFE_CONFIG_HARDWARE_MCP23017
  void addMCP23017Reference(AFEMCP23017Broker *);
#endif

  /* Method sets relay state after device is turned on / power is restored / or
   * after device has been crash */
  void setRelayAfterRestoringPower();

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  /* Method sets relay state after device is connected / reconnected to MQTT
   * Broker. It returns TRUE if relay state has been set, false it relay state
   * should be manged through MQTT Broker*/
  boolean setRelayAfterRestoringMQTTConnection();
#endif

  /* Method: Returns AFE_RELAY_OFF if relay is OFF, AFE_RELAY_ON if relay is ON
   */
  byte get();

  /* Turns on relay */
  void on();

  /* Turns off relay */
  void off();

  /* Toggles relay state from ON to OFF or from OFF to ON */
  void toggle();

#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
  /* Methods automatically turns off/on relay */
  boolean autoTurnOff();
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY_CONTROL_AUTOONOFF_TIME
  /* It sets timer to auto-switch of the relay */
  void setTimer(float timer);
#endif

#ifdef AFE_CONFIG_HARDWARE_PIR
  /* It removed timer for auto-switch of the relay */
  void clearTimer();
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
  /* It sets unit of relay to auto turn off timer. Possible options: true -
   * secods, false - miliseconds */
  void setTimerUnitToSeconds(boolean value);
#endif
};

#endif // AFE_CONFIG_HARDWARE_RELAY
#endif // _AFE_Relay_h
