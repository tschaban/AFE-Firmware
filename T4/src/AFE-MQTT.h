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

#include <AFE-NETWORK-Structure.h>
#include <PubSubClient.h>
//#include <Streaming.h>
#include <WiFiClient.h>

class AFEMQTT {

private:
  WiFiClient esp;
  PubSubClient Broker;
  char mqttTopicForSubscription[34];
  char deviceName[32];
  NETWORK NetworkConfiguration;
  unsigned long sleepStartTime = 0;
  boolean sleepMode = false;
  boolean isConfigured =
      true; // if it's falsed it does not connect to MQTT Broker

  uint8_t connections = 0;
  unsigned long delayStartTime = 0;
  unsigned long ledStartTime = 0;

  /* Method pushes to the MQTT Broker MQTT Message */
  void publishToMQTTBroker(const char *topic, const char *message);
  /* Connecting to MQTT Broker */
  void connect();

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

  /* Publishing MQTT Message. It calls private method publishToMQTTBroker */
  void publish(const char *type, const char *message);

  /* Publishing MQTT Message to a specific MQTT Topic. It calls private method
   * publish MQTT Broker */
  void publish(const char *topic, const char *type, const char *message);

  /* Methods establishes connection from MQTT Broker, subscribed and set relay
   * default values */
  void listener();
};

#endif
