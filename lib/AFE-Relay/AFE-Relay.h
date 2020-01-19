/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Relay_h
#define _AFE_Relay_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>
#include <AFE-MQTT-Structure.h>

#ifdef AFE_CONFIG_HARDWARE_LED
#include <AFE-LED.h>
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTION
#include <AFE-Thermal-Protection.h>
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMOSTAT
#include <AFE-Thermostat.h>
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_HUMIDISTAT
#include <AFE-Humidistat.h>
#endif

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFERelay {

public:
#ifdef AFE_CONFIG_FUNCTIONALITY_THERMOSTAT
  AFEThermostat Thermostat;
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTION
  AFEThermalProtection ThermalProtection;
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_HUMIDISTAT
  AFEHumidistat Humidistat;
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
  uint8_t gateId = AFE_HARDWARE_ITEM_NOT_EXIST;
#endif
  RELAY configuration;

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  char mqttCommandTopic[sizeof(configuration.mqtt.topic) + 4];
  char mqttStateTopic[sizeof(configuration.mqtt.topic) + 6];
#endif

  /* Constructors */
  AFERelay();
  AFERelay(uint8_t id);

  /* Method: initiates relay */
  void begin(uint8_t id);

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
  void on(boolean invert = false);

  /* Turns off relay */
  void off(boolean invert = false);

  /* Toggles relay state from ON to OFF or from OFF to ON */
  void toggle();

#ifdef AFE_CONFIG_RELAY_AUTOONOFF_LISTENER
  /* Methods automatically turns off/on relay */
  boolean autoTurnOff(boolean invert = false);
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

private:
  uint8_t _id;
  AFEDataAccess Data; // @TODO nie jest konsekwentnie jak np. w switch

#ifdef AFE_CONFIG_HARDWARE_LED
  AFELED Led;
#endif

  unsigned long turnOffCounter = 0;
#ifdef AFE_CONFIG_RELAY_AUTOONOFF_LISTENER
  boolean timerUnitInSeconds = true;
#endif

  /* Method set relay state after power restore or connection to MQTT is
   * established */

  void setRelayAfterRestore(uint8_t option);
};

#endif
