/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */
#ifndef _AFE_Web_Handlers_h
#define _AFE_Web_Handlers_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

/* Method handles favicon.ico request */
void handleFavicon() {}

/* Method handles all HTTP request */
void handleHTTPRequests() { WebServer.generate(); }
void handleUpload() { WebServer.generate(true); }

void handleOnNotFound() {
  String page = "<head><meta http-equiv=\"refresh\" content=\"0; "
                "url=http://192.168.5.1/\" /></head><body><p>";
  page += L_OPENING_CONFIG_PANEL;
  page += "</p></body>";
  WebServer.publishHTML(page);
}

#endif
