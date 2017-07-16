#include "AFE-Configuration-Panel.h"

AFEConfigurationPanel::AFEConfigurationPanel() {
}

String AFEConfigurationPanel::getSite(const String option,uint8_t command,boolean data) {

        String page = Site.generateHeader();

        if (option=="upgrade") {
                page+=Site.addUpgradeSection();
        } else if (option=="reset") {
                page+=Site.addResetSection();
        } else if (option=="exit") {
                page+=Site.addExitSection();
        } else {
                page+="<h1>Page Not Found</h1>";
        }
        page+=Site.generateFooter();
        return page;
}

String AFEConfigurationPanel::getSite(const String option,uint8_t command,NETWORK *data){

        if (command==SERVER_CMD_SAVE) {
                AFEDataAccess save;
                save.saveConfiguration(*data);
        }

        String page = Site.generateHeader();
        page+= "<form action=\"/basic?command=1\"  method=\"post\">";
        page+=Site.addNetworkConfiguration();
        page+="<input type=\"submit\" class=\"b bs\" value=\"Zapisz\">";
        page+="</form>";
        page+=Site.generateFooter();
        return page;
}

/* @TODO Only for MQTT */
String AFEConfigurationPanel::getSite(const String option,uint8_t command,MQTT *data){

        if (command==SERVER_CMD_SAVE) {
                AFEDataAccess save;
                save.saveConfiguration(*data);
        }

        String page = Site.generateHeader();
        page+= "<form action=\"/mqtt?command=1\"  method=\"post\">";
        page+=Site.addMQTTBrokerConfiguration();
        page+="<input type=\"submit\" class=\"b bs\" value=\"Zapisz\">";
        page+="</form>";
        page+=Site.generateFooter();
        return page;
}

/* @TODO Only for Domoticz */
String AFEConfigurationPanel::getSite(const String option,uint8_t command,DOMOTICZ *data) {

        if (command==SERVER_CMD_SAVE) {
                AFEDataAccess save;
                save.saveConfiguration(*data);
        }

        String page = Site.generateHeader();
        page+= "<form action=\"/domoticz?command=1\"  method=\"post\">";
        page+=Site.addDomoticzConfiguration();
        page+="<input type=\"submit\" class=\"b bs\" value=\"Zapisz\">";
        page+="</form>";
        page+=Site.generateFooter();
        return page;
}

String AFEConfigurationPanel::getSite(const String option,uint8_t command,RELAY *data){

        if (command==SERVER_CMD_SAVE) {
                AFEDataAccess save;
                save.saveConfiguration(0,*data);
        }

        String page = Site.generateHeader();
        page+= "<form action=\"/relay?command=1\"  method=\"post\">";
        page+=Site.addRelayConfiguration(1);
        page+=Site.addRelayConfiguration(2);
        page+="<input type=\"submit\" class=\"b bs\" value=\"Zapisz\">";
        page+="</form>";
        page+=Site.generateFooter();
        return page;
}

String AFEConfigurationPanel::getSite(const String option,uint8_t command,SWITCH *data){

        if (command==SERVER_CMD_SAVE) {
                AFEDataAccess save;
                save.saveConfiguration(0,*data);
        }

        String page = Site.generateHeader();
        page+= "<form action=\"/switch?command=1\"  method=\"post\">";
        page+=Site.addSwitchConfiguration(1);
        page+=Site.addSwitchConfiguration(2);
        page+="<input type=\"submit\" class=\"b bs\" value=\"Zapisz\">";
        page+="</form>";
        page+=Site.generateFooter();
        return page;
}

String AFEConfigurationPanel::getSite(const String option,uint8_t command,DS18B20 *data) {

        if (command==SERVER_CMD_SAVE) {
                AFEDataAccess save;
                save.saveConfiguration(*data);
        }

        String page = Site.generateHeader();
        page+= "<form action=\"/ds18b20?command=1\"  method=\"post\">";
        page+=Site.addDS18B20Configuration();
        page+="<input type=\"submit\" class=\"b bs\" value=\"Zapisz\">";
        page+="</form>";
        page+=Site.generateFooter();
        return page;
}
