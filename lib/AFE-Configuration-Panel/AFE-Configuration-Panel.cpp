#include "AFE-Configuration-Panel.h"

AFEConfigurationPanel::AFEConfigurationPanel() {
}

String AFEConfigurationPanel::getDeviceConfigurationSite() {
        String page = Site.generateHeader();
        page+=Site.addDeviceNameConfiguration();
        page+=Site.addWiFiConfiguration();
        page+=Site.generateFooter();
        return page;
}

String AFEConfigurationPanel::getMQTTConfigurationSite() {
        String page = Site.generateHeader();
        page+=Site.addMQTTBrokerConfiguration();
        page+=Site.generateFooter();
        return page;
}


String AFEConfigurationPanel::getRelayConfigurationSite() {
        String page = Site.generateHeader();
        page+=Site.addRelayConfiguration();
        page+=Site.generateFooter();
        return page;
}

String AFEConfigurationPanel::getDS18B20ConfigurationSite() {
        String page = Site.generateHeader();
        page+=Site.addDS18B20Configuration();
        page+=Site.generateFooter();
        return page;
}

String AFEConfigurationPanel::getSwitchConfigurationSite() {
        String page = Site.generateHeader();
        page+=Site.addSwitchConfiguration();
        page+=Site.generateFooter();
        return page;
}

String AFEConfigurationPanel::getDomoticzConfigurationSite() {
        String page = Site.generateHeader();
        page+=Site.addDomoticzConfiguration();
        page+=Site.generateFooter();
        return page;
}

String AFEConfigurationPanel::getUpdateConfigurationSite() {
        String page = Site.generateHeader();
        page+="saveing";
        page+=Site.generateFooter();
        return page;
}

String AFEConfigurationPanel::getResetConfigurationSite() {
        String page = Site.generateHeader();
        page+="reset";
        page+=Site.generateFooter();
        return page;
}

String AFEConfigurationPanel::getExitConfigurationSite() {
        String page = Site.generateHeader();
        page+="Exit";
        page+=Site.generateFooter();
        return page;
}
