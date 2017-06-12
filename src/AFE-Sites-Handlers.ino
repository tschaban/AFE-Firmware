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
}

void handleRoot() {
    AFEConfigurationPanel ConfigurationPanel;
		if (WebServer.getUrlCommand()==SERVER_CMD_SAVE) {
			Serial << endl << "INFO: Saveing";
		}
    String site = ConfigurationPanel.getDeviceConfigurationSite();
		WebServer.publishHTML(site);
}

void handleNotFound() {
	  String site = "404 Page Not Found";
		WebServer.publishHTML(site);
}

void handleMQTTConfiguration() {
	AFEConfigurationPanel ConfigurationPanel;
	String site = ConfigurationPanel.getMQTTConfigurationSite();
	WebServer.publishHTML(site);
}

void handleRelayConfiguration() {
	AFEConfigurationPanel ConfigurationPanel;
	String site = ConfigurationPanel.getRelayConfigurationSite();
	WebServer.publishHTML(site);
}

void handleDS18B20Configuration() {
	AFEConfigurationPanel ConfigurationPanel;
	String site = ConfigurationPanel.getDS18B20ConfigurationSite();
	WebServer.publishHTML(site);
}

void handleSwitchConfiguration() {
	AFEConfigurationPanel ConfigurationPanel;
	String site = ConfigurationPanel.getSwitchConfigurationSite();
	WebServer.publishHTML(site);
}

void handleDomoticzConfiguration() {
	AFEConfigurationPanel ConfigurationPanel;
	String site = ConfigurationPanel.getDomoticzConfigurationSite();
	WebServer.publishHTML(site);
}

void handleUpdateConfiguration() {
	AFEConfigurationPanel ConfigurationPanel;
	String site = ConfigurationPanel.getUpdateConfigurationSite();
	WebServer.publishHTML(site);
}

void handleResetConfiguration() {
	AFEConfigurationPanel ConfigurationPanel;
	String site = ConfigurationPanel.getResetConfigurationSite();
	WebServer.publishHTML(site);
}

void handleExitConfiguration() {
	AFEConfigurationPanel ConfigurationPanel;
	String site = ConfigurationPanel.getExitConfigurationSite();
	WebServer.publishHTML(site);
}

void handleSave() {
	
}

#endif
