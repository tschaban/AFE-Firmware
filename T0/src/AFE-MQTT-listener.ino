/*
  AFE Firmware for smart home devices build on ESP8266
  Version: T0
  MQTT Messages listener
  More info: https://github.com/tschaban/AFE-Firmware
  LICENCE: http://opensource.org/licenses/MIT
*/

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

void MQTTMessagesListener(char *topic, byte *payload, unsigned int length) {

  char _mqttTopic[50];
  Led.on();
  // Serial << endl << "INFO: MQTT message recieved: " << topic << " \\ ";

  if (length >= 1) { // command arrived
                     /*
                         for (uint8_t i = 0; i < length; i++) {
                           Serial << (char)payload[i];
                         }
                     */
    sprintf(_mqttTopic, "%scmd", Relay.getMQTTTopic());
    /*
        Serial << endl
               << "DEBUG: "
               << "checking relay messages: " << _mqttTopic;
    */
    if (strcmp(topic, _mqttTopic) == 0) {
      if ((char)payload[1] == 'n') {
        Relay.on();
        Mqtt.publish(Relay.getMQTTTopic(), "state", "on");
      } else if ((char)payload[1] == 'f') {
        Relay.off();
        Mqtt.publish(Relay.getMQTTTopic(), "state", "off");
      } else if ((char)payload[1] == 'e') { // reportState
        Mqtt.publish(Relay.getMQTTTopic(), "state",
                     Relay.get() == RELAY_ON ? "on" : "off");
      } else if ((char)payload[1] == 'o') { // toggle
        Relay.get() == RELAY_ON ? Relay.off() : Relay.on();
        Mqtt.publish(Relay.getMQTTTopic(), "state",
                     Relay.get() == RELAY_ON ? "on" : "off");
      }
    }

    sprintf(_mqttTopic, "%scmd", MQTTConfiguration.topic);
    /*
        Serial << endl
               << "DEBUG: "
               << "checking device level messages: " << _mqttTopic;
    */
    if (strcmp(topic, _mqttTopic) == 0) {
      if ((char)payload[2] == 'b') { // reboot
        //      Serial << endl << "INFO: Process: reboot";
        Device.reboot(MODE_NORMAL);
      } else if ((char)payload[2] == 'n') { // configurationMode
        //    Serial << endl << "INFO: Process: configuration Mode";
        Device.reboot(MODE_CONFIGURATION);
      }
    }
  }
  Led.off();
}
