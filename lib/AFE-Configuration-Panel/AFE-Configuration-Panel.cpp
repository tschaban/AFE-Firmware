#include "AFE-Configuration-Panel.h"

AFEConfigurationPanel::AFEConfigurationPanel() {
}

String AFEConfigurationPanel::getSite(const String option,boolean save) {
        Serial << endl << "INFO: Site : " << option << " requested " << (save ? "with save option" : "with no save option");

        String page = Site.generateHeader();

        if (option=="basic") {
                page+=getDeviceConfigurationSite();
        } else if (option=="mqtt") {
                page+=getMQTTConfigurationSite();
        } else if (option=="domoticz") {
                page+=getDomoticzConfigurationSite();
        } else if (option=="relay") {
                page+=getRelayConfigurationSite();
        } else if (option=="ds18b20") {
                page+=getDS18B20ConfigurationSite();
        } else if (option=="switch") {
                page+=getSwitchConfigurationSite();
        } else if (option=="upgrade") {
                page+=getUpdateConfigurationSite();
        } else if (option=="reset") {
                page+=getResetConfigurationSite();
        } else if (option=="exit") {
                page+=getExitConfigurationSite();
        } else {
                page+="<h1>Page Not Found</h1>";
        }
        page+=Site.generateFooter();
        return page;
}

String AFEConfigurationPanel::getDeviceConfigurationSite() {
        String page = "<form action=\"/?command=1\"  method=\"post\">";
        page+=Site.addDeviceNameConfiguration();
        page+=Site.addWiFiConfiguration();
        page+=Site.addNetworkConfiguration();
        page+="<input type=\"submit\" class=\"b bs\" value=\"Zapisz\">";
        page+="</form>";
        return page;
}

String AFEConfigurationPanel::getMQTTConfigurationSite() {
        String page = "<form action=\"/mqtt?command=1\"  method=\"post\">";
        page+=Site.addMQTTBrokerConfiguration();
        page+="<input type=\"submit\" class=\"b bs\" value=\"Zapisz\">";
        page+="</form>";
        return page;
}

String AFEConfigurationPanel::getRelayConfigurationSite() {
        String page = "<form action=\"/relay?command=1\"  method=\"post\">";
        page+=Site.addRelayConfiguration();
        page+="<input type=\"submit\" class=\"b bs\" value=\"Zapisz\">";
        page+="</form>";
        return page;
}

String AFEConfigurationPanel::getDS18B20ConfigurationSite() {
        String page = "<form action=\"/ds18b20?command=1\"  method=\"post\">";
        page+=Site.addDS18B20Configuration();
        page+="<input type=\"submit\" class=\"b bs\" value=\"Zapisz\">";
        page+="</form>";
        return page;
}

String AFEConfigurationPanel::getSwitchConfigurationSite() {
        String page = "<form action=\"/switch?command=1\"  method=\"post\">";
        page+=Site.addSwitchConfiguration("1");
        page+=Site.addSwitchConfiguration("2");
        page+="<input type=\"submit\" class=\"b bs\" value=\"Zapisz\">";
        page+="</form>";
        return page;
}

String AFEConfigurationPanel::getDomoticzConfigurationSite() {
        String page = "<form action=\"/domoticz?command=1\"  method=\"post\">";
        page+=Site.addDomoticzConfiguration();
        page+="<input type=\"submit\" class=\"b bs\" value=\"Zapisz\">";
        page+="</form>";
        return page;
}

String AFEConfigurationPanel::getUpdateConfigurationSite() {
        return Site.addUpgradeSection();
}

String AFEConfigurationPanel::getResetConfigurationSite() {
        return Site.addResetSection();
}

String AFEConfigurationPanel::getExitConfigurationSite() {
        return Site.addExitSection();
}
