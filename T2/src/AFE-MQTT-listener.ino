/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

/* Method is launched after MQTT Message is received */
void MQTTMessagesListener(char *topic, byte *payload, unsigned int length) {

  char _mqttTopic[70];
  Led.on();

//  Serial << endl << "INFO: MQTT message recieved: " << topic << " \\ ";

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
      if ((char)payload[0] == 'o' && length==2) { // on
        Relay.on();
        Mqtt.publish(Relay.getMQTTTopic(), "state", "on");
      } else if ((char)payload[0] == 'o' && length==3) { // off
        Relay.off();
        Mqtt.publish(Relay.getMQTTTopic(), "state", "off");
      } else if (((char)payload[0] == 'r' && length==12) || ((char)payload[0] == 'g' && length==3)) { // reportState or get @TODO remove onve rc1 is no longer used
        Mqtt.publish(Relay.getMQTTTopic(), "state",
                     Relay.get() == RELAY_ON ? "on" : "off");
      } else if ((char)payload[0] == 't' && length==6) { // toggle
        Relay.get() == RELAY_ON ? Relay.off() : Relay.on();
        Mqtt.publish(Relay.getMQTTTopic(), "state",
                     Relay.get() == RELAY_ON ? "on" : "off");
      }
    } else {


    sprintf(_mqttTopic, "%sthermostat/cmd", Relay.getMQTTTopic());
/*
    Serial << endl
           << "DEBUG: "
           << "checking thermostat messages: " << _mqttTopic;
*/
    if (strcmp(topic, _mqttTopic) == 0) {
    if ((char)payload[0] == 'o' && length==2) { // on
      Relay.Thermostat.on();
      Mqtt.publish(Relay.getMQTTTopic(), "thermostat/state",
                    Relay.Thermostat.enabled() ? "on" : "off");
    } else if ((char)payload[0] == 'o' && length==3) { // off
      Relay.Thermostat.off();
      Mqtt.publish(Relay.getMQTTTopic(), "thermostat/state",
                    Relay.Thermostat.enabled() ? "on" : "off");
    } else if ((char)payload[0] == 't' && length==6) { // toggle
      Relay.Thermostat.enabled() ? Relay.Thermostat.off()
                                  : Relay.Thermostat.on();
      Mqtt.publish(Relay.getMQTTTopic(), "thermostat/state",
                    Relay.Thermostat.enabled() ? "on" : "off");
    } else if ((char)payload[0] == 'g' && length==3) { // get
      Mqtt.publish(Relay.getMQTTTopic(), "thermostat/state",
                    Relay.Thermostat.enabled() ? "on" : "off");
    }
  } else {

    sprintf(_mqttTopic, "%scmd", MQTTConfiguration.topic);
/*
        Serial << endl
               << "DEBUG: "
               << "checking device level messages: " << _mqttTopic;
*/
    if (strcmp(topic, _mqttTopic) == 0) {
      if ((char)payload[2] == 'b' && length==6 ) { // reboot
        //      Serial << endl << "INFO: Process: reboot";
        Device.reboot(MODE_NORMAL);
      } else if ((char)payload[2] == 'n' && length==17) { // configurationMode
        //    Serial << endl << "INFO: Process: configuration Mode";
        Device.reboot(MODE_CONFIGURATION);
      } else if ((char)payload[2] == 't' && length==14) { // getTemperature
        char  temperatureString[6];
        dtostrf(SensorDHT.getTemperature(), 2, 2, temperatureString);
        Mqtt.publish("temperature",temperatureString);
      } else if ((char)payload[2] == 't' && length==11) { // getHumidity
        char  humidityString[6];
        dtostrf(SensorDHT.getHumidity(), 2, 2, humidityString);
        Mqtt.publish("humidity",humidityString);
      } else if ((char)payload[2] == 't' && length==12) { // getHeatIndex
        char heatIndex[6];
        dtostrf(SensorDHT.getHeatIndex(), 2, 2, heatIndex);
        Mqtt.publish("heatIndex",heatIndex);
      }
    }
  }
}
}
  Led.off();
}

/* Metod publishes Relay state (used eg by HTTP API) */
void MQTTPublishRelayState() {
  if (Device.configuration.mqttAPI) {
    Led.on();
    if (Relay.get() == RELAY_ON) {
      Mqtt.publish(Relay.getMQTTTopic(), "state", "on");
    } else {
      Mqtt.publish(Relay.getMQTTTopic(), "state", "off");
    }
    Led.off();
  }
}
