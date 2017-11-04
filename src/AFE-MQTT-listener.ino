#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

void MQTTMessagesListener(char *topic, byte *payload, unsigned int length) {

  char _mqttTopic[50];

  Serial << endl << "INFO: MQTT message recieved: " << topic << " \\ ";

  if (length >= 1) { // command arrived
    for (uint8_t i = 0; i < length; i++) {
      Serial << (char)payload[i];
    }

    // Relay

    sprintf(_mqttTopic, "%scmd", Relay.getMQTTTopic());

    Serial << endl
           << "DEBUG: "
           << "checking relay messages: " << _mqttTopic;

    if (String(topic) == String(_mqttTopic)) {
      if ((char)payload[1] == 'N') {
        Relay.on();
        Mqtt.publish("state", "ON");
      } else if ((char)payload[1] == 'F') {
        Relay.off();
        Mqtt.publish("state", "OFF");
      } else if ((char)payload[2] == 'p') { // reportState
        Relay.get() ? Mqtt.publish("state", "ON")
                    : Mqtt.publish("state", "OFF");
      }
    }

    sprintf(_mqttTopic, "%scmd", MQTTConfiguration.topic);
    Serial << endl
           << "DEBUG: "
           << "checking device level messages: " << _mqttTopic;

    if (String(topic) == String(_mqttTopic)) {
      if ((char)payload[2] == 'b') { // reboot
        Serial << endl << "INFO: Process: reboot";
        Device.reboot(MODE_NORMAL);
      } else if ((char)payload[2] == 'n') { // configurationMode
        Serial << endl << "INFO: Process: configuration Mode";
        Device.reboot(MODE_ACCESS_POINT);
      }
    }
  }
}
