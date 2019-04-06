/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

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
  WebServer.publishHTML("<head><meta http-equiv=\"refresh\" content=\"0; "
                        "url=http://192.168.5.1/\" "
                        "/></head><body><p>Otwieram panel konfiguracyjny AFE "
                        "Firmware...</p></body>");
}

#endif
