/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

/* Method creates JSON respons after processing HTTP API request, and pushes it.
 * The second one method converts float to charString before pushing response */
void sendHTTPAPIRequestStatus(HTTPCOMMAND request, boolean status,
                              const char *value = "") {
  String respond;
  respond = "{";
  if (strlen(request.device) > 0) {
    respond += "\"device\":\"" + String(request.device) + "\",";
  }
  if (strlen(request.name) > 0) {
    respond += "\"name\":\"" + String(request.name) + "\",";
  }
  if (strlen(request.command) > 0) {
    respond += "\"command\":\"" + String(request.command) + "\",";
  }

  if (!strlen(value) == 0) {
    respond += "\"value\":\"";
    respond += value;
    respond += "\",";
  }

  respond += "\"status\":\"";
  respond += status ? "success" : "error";
  respond += "\"}";
  WebServer.sendJSON(respond);
}
void sendHTTPAPIRequestStatus(HTTPCOMMAND request, boolean status, float value,
                              uint8_t width = 2, uint8_t precision = 2) {
  char valueString[10];
  dtostrf(value, width, precision, valueString);
  sendHTTPAPIRequestStatus(request, status, valueString);
}

/* Method converts Relay value to string and invokes sendHTTPAPIRequestStatus
 * method which creates JSON respons and pushes it */
void sendHTTPAPIRelayRequestStatus(HTTPCOMMAND request, boolean status,
                                   byte value) {
  sendHTTPAPIRequestStatus(request, status, value == RELAY_ON ? "on" : "off");
}

/* Method processes HTTP API request */
void processHTTPAPIRequest(HTTPCOMMAND request) {

  Led.on();
  if /* Relay level commands */ (strcmp(request.device, "relay") == 0) {
    /* Checking Relay #0 */
    if (strcmp(request.name, Relay.getName()) == 0) {
      if (strcmp(request.command, "on") == 0) {
        Relay.on();
        sendHTTPAPIRelayRequestStatus(request, Relay.get() == RELAY_ON,
                                      Relay.get());
        MQTTPublishRelayState();                        // MQTT Listener library
      } else if (strcmp(request.command, "off") == 0) { // Off
        Relay.off();
        sendHTTPAPIRelayRequestStatus(request, Relay.get() == RELAY_OFF,
                                      Relay.get());
        MQTTPublishRelayState(); // MQTT Listener library
      } else if (strcmp(request.command, "toggle") == 0) { // toggle
        uint8_t state = Relay.get();
        Relay.toggle();
        sendHTTPAPIRelayRequestStatus(request, state != Relay.get(),
                                      Relay.get());
        MQTTPublishRelayState(); // MQTT Listener library
      } else if (strcmp(request.command, "get") ==
                 0) { // reportStatus or get @TODO Remove after version rc1 is
                      // not used
        sendHTTPAPIRelayRequestStatus(request, true, Relay.get());
      } else if (strcmp(request.command, "enableThermostat") == 0) {
        Relay.Thermostat.on();
        sendHTTPAPIRequestStatus(request, true,
                                 Relay.Thermostat.enabled() ? "on" : "off");
      } else if (strcmp(request.command, "disableThermostat") == 0) {
        Relay.Thermostat.off();
        sendHTTPAPIRequestStatus(request, true,
                                 Relay.Thermostat.enabled() ? "on" : "off");
      } else if (strcmp(request.command, "toggleThermostat") == 0) {
        Relay.Thermostat.enabled() ? Relay.Thermostat.off()
                                   : Relay.Thermostat.on();
        sendHTTPAPIRequestStatus(request, true,
                                 Relay.Thermostat.enabled() ? "on" : "off");
      } else if (strcmp(request.command, "getThermostat") == 0) {
        sendHTTPAPIRequestStatus(request, true,
                                 Relay.Thermostat.enabled() ? "on" : "off");
      } else if (strcmp(request.command, "enableHumidistat") == 0) {
        Relay.Humidistat.on();
        sendHTTPAPIRequestStatus(request, true,
                                 Relay.Humidistat.enabled() ? "on" : "off");
      } else if (strcmp(request.command, "disableHumidistat") == 0) {
        Relay.Humidistat.off();
        sendHTTPAPIRequestStatus(request, true,
                                 Relay.Humidistat.enabled() ? "on" : "off");
      } else if (strcmp(request.command, "toggleHumidistat") == 0) {
        Relay.Humidistat.enabled() ? Relay.Humidistat.off()
                                   : Relay.Humidistat.on();
        sendHTTPAPIRequestStatus(request, true,
                                 Relay.Humidistat.enabled() ? "on" : "off");
      } else if (strcmp(request.command, "getHumidistat") == 0) {
        sendHTTPAPIRequestStatus(request, true,
                                 Relay.Humidistat.enabled() ? "on" : "off");

      } else { /* Commend not implemented */
        sendHTTPAPIRequestStatus(request, false);
      }
    } else { /* No such relay */
      sendHTTPAPIRequestStatus(request, false);
    }
  } /* DHT Sensor */ else if (strcmp(request.device, "dht") == 0) {
    if (strcmp(request.name, "temperature") == 0) {
      strcmp(request.command, "get") == 0
          ? sendHTTPAPIRequestStatus(request, true, SensorDHT.getTemperature())
          : sendHTTPAPIRequestStatus(request, false);
    } else if (strcmp(request.name, "humidity") == 0) {
      strcmp(request.command, "get") == 0
          ? sendHTTPAPIRequestStatus(request, true, SensorDHT.getHumidity())
          : sendHTTPAPIRequestStatus(request, false);
    } else if (strcmp(request.name, "heatIndex") == 0) {
      strcmp(request.command, "get") == 0
          ? sendHTTPAPIRequestStatus(request, true, SensorDHT.getHeatIndex())
          : sendHTTPAPIRequestStatus(request, false);
    } else {
      sendHTTPAPIRequestStatus(request, false);
    }
  } /* Device level commands */ else if (strcmp(request.command, "reboot") ==
                                         0) { // reboot
    sendHTTPAPIRequestStatus(request, true);
    Device.reboot(Device.getMode());
  } else if (strcmp(request.command, "configurationMode") ==
             0) { // configurationMode
    sendHTTPAPIRequestStatus(request, true);
    Device.reboot(MODE_CONFIGURATION);
    /* No such device or commend not implemented */
  } else {
    sendHTTPAPIRequestStatus(request, false);
  }
  Led.off();
}
