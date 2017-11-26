/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

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

  /* Checking of request is about a relay */
  if (strcmp(request.device, "relay") == 0) {
    /* Checking Relay #0 */
    if (strcmp(request.name, Relay.getName()) == 0) {
      if (strcmp(request.command, "on") == 0) {
        Relay.on();
        Relay.get() == RELAY_ON ? sendHTTPAPIRequestStatus(request, true)
                                : sendHTTPAPIRequestStatus(request, false);
      } else if (strcmp(request.command, "off") == 0) { // Off
        Relay.off();
        Relay.get() == RELAY_OFF ? sendHTTPAPIRequestStatus(request, true)
                                 : sendHTTPAPIRequestStatus(request, false);
      } else if (strcmp(request.command, "toggle") == 0) { // toggle
        uint8_t state = Relay.get();
        Relay.toggle();
        state != Relay.get() ? sendHTTPAPIRequestStatus(request, true)
                             : sendHTTPAPIRequestStatus(request, false);
      } else if (strcmp(request.command, "reportStatus") == 0) { // reportStatus
        sendHTTPAPIRequestStatus(request, true, Relay.get());
        /* Commend not implemented */
      } else {
        sendHTTPAPIRequestStatus(request, false);
      }
      /* No such relay */
    } else {
      sendHTTPAPIRequestStatus(request, false);
    }
  } else if (strcmp(request.command, "reboot") == 0) { // reboot
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
}
