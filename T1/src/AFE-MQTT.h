/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#ifndef _AFE_MQTT_h
#define _AFE_MQTT_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Structures.h>
#include <PubSubClient.h>
//#include <Streaming.h>
#include <WiFiClient.h>

class AFEMQTT {

private:
  WiFiClient esp;
  PubSubClient Broker;
  char mqttTopicForSubscription[34];
  char deviceName[32];
  uint8_t noConnectionAttempts;
  uint8_t durationBetweenConnectionAttempts;
  uint8_t durationBetweenNextConnectionAttemptsSeries;
  unsigned long sleepStartTime = 0;
  boolean sleepMode = false;
  boolean isConfigured =
      true; // if it's falsed it does not connect to MQTT Broker

  /* Method pushes to the MQTT Broker MQTT Message */
  void publishToMQTTBroker(const char *topic, const char *message);

public:
  /* Constructor: it sets all necessary parameters */
  AFEMQTT();

  /* Sets parameters related to reconnection to MQTT Broker if there was
   * problems to connect to it */
  void setReconnectionParams(
      uint8_t no_connection_attempts,
      uint8_t duration_between_connection_attempts,
      uint8_t duration_between_next_connection_attempts_series);

  void begin();

  /* Connecting to MQTT Broker */
  void connect();

  /* Publishing MQTT Message. It calls private method publishToMQTTBroker. First one publishes char string, second one float with optional specyfication float format to convert string */
  void publish(const char *type, const char *message);
  void publish(const char *type, float value, uint8_t width=2, uint8_t precision=2);

  /* Publishing MQTT Message to at specyfic MQTT Topic. It calls private method
   * publishToMQTTBroker  */
  void publish(const char *topic, const char *type, const char *message);

  /* Returns TRUE if connected to MQTT Broker */
  boolean connected();

  /* MQTT Loop / Listner loop */
  void loop();
};

#endif
