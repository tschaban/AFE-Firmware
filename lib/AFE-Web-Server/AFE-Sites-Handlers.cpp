#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Streaming.h>

void handleFavicon() {
  Serial << endl << "INFO: handleFavicon";
  WebServer.generatePage();
}

void handleRoot() {
  Serial << endl << "INFO: handleRoot";
    WebServer.generatePage();
}

void handleNotFound() {
  Serial << endl << "INFO: handleNotFound";
    WebServer.generatePage();
}
