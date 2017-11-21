/*
  AFE Firmware for smart home devices build on ESP8266
  More info: https://github.com/tschaban/AFE-Firmware
  LICENCE: http://opensource.org/licenses/MIT
*/

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

void sendHTTPAPIRequestStatus(HTTPCOMMAND request, boolean status,
                              byte state = 9) {
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

  if (state != 9) {
    respond += "\"state\":\"";
    respond += state == RELAY_ON ? "on" : "off";
    respond += "\",";
  }

  respond += "\"status\":\"";
  respond += status ? "success" : "failure";
  respond += "\"}";
  WebServer.sendJSON(respond);
}

void processHTTPAPIRequest(HTTPCOMMAND request) {
  if ((char)request.command[1] == 'n') { // On
    Relay.on();
    Relay.get() == RELAY_ON ? sendHTTPAPIRequestStatus(request, true)
                            : sendHTTPAPIRequestStatus(request, false);
  } else if ((char)request.command[2] == 'f') { // Off
    Relay.off();
    Relay.get() == RELAY_OFF ? sendHTTPAPIRequestStatus(request, true)
                             : sendHTTPAPIRequestStatus(request, false);
  } else if ((char)request.command[2] == 'g') { // toggle
    uint8_t state = Relay.get();
    Relay.toggle();
    state != Relay.get() ? sendHTTPAPIRequestStatus(request, true)
                         : sendHTTPAPIRequestStatus(request, false);
  } else if ((char)request.command[2] == 'p') { // reportStatus
    sendHTTPAPIRequestStatus(request, true, Relay.get());
  } else if ((char)request.command[2] == 'b') { // reboot
    sendHTTPAPIRequestStatus(request, true);
    Device.reboot(Device.getMode());
  } else if ((char)request.command[2] == 'n') { // configurationMode
    sendHTTPAPIRequestStatus(request, true);
    Device.reboot(MODE_CONFIGURATION);
  }
}
