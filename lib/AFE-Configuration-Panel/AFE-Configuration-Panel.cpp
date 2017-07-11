#include "AFE-Configuration-Panel.h"

AFEConfigurationPanel::AFEConfigurationPanel() {
}

String AFEConfigurationPanel::getDeviceConfigurationSite() {
        String page = Site.generateHeader();
        page+="<form action=\"/?command=1\"  method=\"post\">";
        page+=Site.addDeviceNameConfiguration();
        page+=Site.addWiFiConfiguration();
        page+=Site.addNetworkConfiguration();
        page+="<input type=\"submit\" class=\"b bs\" value=\"Zapisz\">";
        page+="</form>";
        page+=Site.generateFooter();
        return page;
}

String AFEConfigurationPanel::getMQTTConfigurationSite() {
        String page = Site.generateHeader();
        page+="<form action=\"/mqtt?command=1\"  method=\"post\">";
        page+=Site.addMQTTBrokerConfiguration();
        page+="<input type=\"submit\" class=\"b bs\" value=\"Zapisz\">";
        page+="</form>";
        page+=Site.generateFooter();
        return page;
}


String AFEConfigurationPanel::getRelayConfigurationSite() {
        String page = Site.generateHeader();
        page+="<form action=\"/relay?command=1\"  method=\"post\">";
        page+=Site.addRelayConfiguration();
        page+="<input type=\"submit\" class=\"b bs\" value=\"Zapisz\">";
        page+="</form>";
        page+=Site.generateFooter();
        return page;
}

String AFEConfigurationPanel::getDS18B20ConfigurationSite() {
        String page = Site.generateHeader();
        page+="<form action=\"/ds18b20?command=1\"  method=\"post\">";
        page+=Site.addDS18B20Configuration();
        page+="<input type=\"submit\" class=\"b bs\" value=\"Zapisz\">";
        page+="</form>";
        page+=Site.generateFooter();
        return page;
}

String AFEConfigurationPanel::getSwitchConfigurationSite() {
        String page = Site.generateHeader();
        page+="<form action=\"/switch?command=1\"  method=\"post\">";
        page+=Site.addSwitchConfiguration("1");
        page+=Site.addSwitchConfiguration("2");
        page+="<input type=\"submit\" class=\"b bs\" value=\"Zapisz\">";
        page+="</form>";
        page+=Site.generateFooter();
        return page;
}

String AFEConfigurationPanel::getDomoticzConfigurationSite() {
        String page = Site.generateHeader();
        page+="<form action=\"/domoticz?command=1\"  method=\"post\">";
        page+=Site.addDomoticzConfiguration();
        page+="<input type=\"submit\" class=\"b bs\" value=\"Zapisz\">";
        page+="</form>";
        page+=Site.generateFooter();
        return page;
}

String AFEConfigurationPanel::getUpdateConfigurationSite() {
        String page = Site.generateHeader();
        page+=Site.addUpgradeSection();
        page+=Site.generateFooter();
        return page;
}

String AFEConfigurationPanel::getResetConfigurationSite() {
        String page = Site.generateHeader();
        page+=Site.addResetSection();
        page+=Site.generateFooter();
        return page;
}

String AFEConfigurationPanel::getExitConfigurationSite() {
        String page = Site.generateHeader();
        page+=Site.addExitSection();
        page+=Site.generateFooter();
        return page;
}
