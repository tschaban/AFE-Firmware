#ifndef _AFE_Web_Handlers_h
#define _AFE_Web_Handlers_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <AFE-Configuration-Panel.h>
#include <Streaming.h>


void handleFavicon() {
  Serial << endl << "INFO: handleFavicon";
}

void handleRoot() {
    Serial << endl << "INFO: handleRoot";
    AFEConfigurationPanel ConfigurationPanel;
    String site = ConfigurationPanel.generateMainSite();
		WebServer.publishHTML(site);
}

void handleNotFound() {
  Serial << endl << "INFO: handleNotFound";
	  String site = "404 Page Not Found";
		WebServer.publishHTML(site);
}

#endif
