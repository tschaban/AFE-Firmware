/* Checking if there was received HTTP API Command */

void mainHTTPRequestsHandler() {
  if (Device.configuration.httpAPI) {
    if (WebServer.httpAPIlistener()) {
      Led.on();
      processHTTPAPIRequest(WebServer.getHTTPCommand());
      Led.off();
    }
  }
}
