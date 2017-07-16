#include "AFE-Web-Server.h"

AFEWebServer::AFEWebServer() {
}

void AFEWebServer::begin() {
        Serial << endl << "INFO: Starting web server";
        server.begin();
        Serial << endl << "INFO: Web server is working";
        Serial << endl << "INFO: Ready for configuration. Open http://" << WiFi.localIP();
}

void AFEWebServer::listener() {
        server.handleClient();
}

void AFEWebServer::publishHTML(String page) {
        server.send(200, "text/html", page);
}

void AFEWebServer::handle(const char* uri,ESP8266WebServer::THandlerFunction handler) {
        Serial << endl << "INFO: Added url : " << uri << " for listening";
        server.on(uri, handler);
}

void AFEWebServer::generate() {
        boolean data; /* @TODO No so good coding: just used to overload AFEConfigurationPanel.getSite() method */

        Serial << endl << "INFO: Site : " << getOptionName() << " requested. Command :  " << getCommand();

        if (getOptionName()=="basic") {
                NETWORK *data;
                if (getCommand()==SERVER_CMD_SAVE) {
                        getData(data);
                }
        } else if (getOptionName()=="mqtt") {
                MQTT *data;
                if (getCommand()==SERVER_CMD_SAVE) {
                        getData(data);
                }
        } else if (getOptionName()=="domoticz") {
                // @TODO Domoticz Config page+=getDomoticzConfigurationSite();
        } else if (getOptionName()=="relay") {
                RELAY *data;
                if (getCommand()==SERVER_CMD_SAVE) {
                        getData(0,data);
                        if (getCommand()==SERVER_CMD_SAVE) {
                                getData(0,data);
                        }
                }
        } else if (getOptionName()=="ds18b20") {
                DS18B20 *data;
                if (getCommand()==SERVER_CMD_SAVE) {
                        getData(data);
                }
        } else if (getOptionName()=="switch") {
                SWITCH *data;
                if (getCommand()==SERVER_CMD_SAVE) {
                        getData(0,data);
                }
        }

        publishHTML(ConfigurationPanel.getSite(getOptionName(),getCommand(),data));
}

String AFEWebServer::getOptionName() {
        if (server.hasArg("option")) {
                return server.arg("option");
        } else {
                return "basic";
        }
}

uint8_t AFEWebServer::getCommand() {
        if (server.hasArg("command")) {
                return server.arg("command").toInt();
        }
}

void AFEWebServer::getData(NETWORK *data) {

        if (server.arg("wifi_ssid").length() > 0 ) {
                server.arg("wifi_ssid").toCharArray(data->ssid,sizeof(data->ssid));
        }

        if (server.arg("wifi_password").length() > 0 ) {
                server.arg("wifi_password").toCharArray(data->password,sizeof(data->password));
        }

        if (server.arg("hostname").length() > 0 ) {
                server.arg("hostname").toCharArray(data->host,sizeof(data->host));
        }

        if (server.arg("dhcp").length() > 0 ) {
                data->dhcp = true;
        } else {
                data->dhcp = false;
        }

        if (server.arg("device_ip1").length() > 0 &&
            server.arg("device_ip2").length() > 0 &&
            server.arg("device_ip3").length() > 0 &&
            server.arg("device_ip4").length() > 0) {

                data->ip = IPAddress(server.arg("device_ip1").toInt(),
                                     server.arg("device_ip2").toInt(),
                                     server.arg("device_ip3").toInt(),
                                     server.arg("device_ip4").toInt());

        }

        if (server.arg("gateway_ip1").length() > 0 &&
            server.arg("gateway_ip2").length() > 0 &&
            server.arg("gateway_ip3").length() > 0 &&
            server.arg("gateway_ip4").length() > 0) {

                data->gateway = IPAddress(server.arg("gateway_ip1").toInt(),
                                          server.arg("gateway_ip2").toInt(),
                                          server.arg("gateway_ip3").toInt(),
                                          server.arg("gateway_ip4").toInt());

        }

        if (server.arg("subnet_ip1").length() > 0 &&
            server.arg("subnet_ip2").length() > 0 &&
            server.arg("subnet_ip3").length() > 0 &&
            server.arg("subnet_ip4").length() > 0) {

                data->subnet = IPAddress(server.arg("subnet_ip1").toInt(),
                                         server.arg("subnet_ip2").toInt(),
                                         server.arg("subnet_ip3").toInt(),
                                         server.arg("subnet_ip4").toInt());

        }

        if (server.arg("no_connection_attempts").length() > 0 ) {
                data->noConnectionAttempts = server.arg("no_connection_attempts").toInt();
        }

        if (server.arg("wait_time_connections").length() > 0 ) {
                data->waitTimeConnections= server.arg("wait_time_connections").toInt();
        }

        if (server.arg("wait_time_series").length() > 0 ) {
                data->waitTimeSeries = server.arg("wait_time_series").toInt();
        }
}

void AFEWebServer::getData(MQTT *data) {

        if (server.arg("mqtt_host").length() > 0 ) {
                server.arg("mqtt_host").toCharArray(data->host,sizeof(data->host));
        }

        if (server.arg("mqtt_ip1").length() > 0 &&
            server.arg("mqtt_ip2").length() > 0 &&
            server.arg("mqtt_ip3").length() > 0 &&
            server.arg("mqtt_ip4").length() > 0) {

                data->ip = IPAddress(server.arg("mqtt_ip1").toInt(),
                                     server.arg("mqtt_ip2").toInt(),
                                     server.arg("mqtt_ip3").toInt(),
                                     server.arg("mqtt_ip4").toInt());

        }

        if (server.arg("mqtt_port").length() > 0 ) {
                data->port=server.arg("mqtt_port").toInt();
        }

        if (server.arg("mqtt_user").length() > 0 ) {
                server.arg("mqtt_user").toCharArray(data->user,sizeof(data->user));
        }

        if (server.arg("mqtt_password").length() > 0 ) {
                server.arg("mqtt_password").toCharArray(data->password,sizeof(data->password));
        }

        if (server.arg("mqtt_topic").length() > 0 ) {
                server.arg("mqtt_topic").toCharArray(data->topic,sizeof(data->topic));
        }



}

void AFEWebServer::getData(uint8_t id, RELAY *data) {

        if (server.arg("relay"+String(id)+"_present").length() > 0 ) {
                data->present = true;
        } else {
                data->present = false;
        }

        if (server.arg("relay"+String(id)+"_gpio").length() > 0 ) {
                data->gpio = server.arg("relay"+String(id)+"_gpio").toInt();
        }

        if (server.arg("relay"+String(id)+"_off_time").length() > 0 ) {
                data->timeToOff = server.arg("relay"+String(id)+"_off_time").toFloat();
        }
        if (server.arg("relay"+String(id)+"_power_restored").length() > 0 ) {
                data->statePowerOn  = server.arg("relay"+String(id)+"_power_restored").toInt();
        }
        if (server.arg("mqtt_host").length() > 0 ) {
                server.arg("mqtt_host").toCharArray(data->name,sizeof(data->name));
        }

        if (server.arg("relay"+String(id)+"_mqtt_connected").length() > 0 ) {
                data->stateMQTTConnected = server.arg("relay"+String(id)+"_mqtt_connected").toInt();
        }

}

void AFEWebServer::getData(uint8_t id, SWITCH *data) {
        if (server.arg("switch"+String(id)+"_present").length() > 0 ) {
                data->present = true;
        } else {
                data->present = false;
        }

        if (server.arg("switch"+String(id)+"_type").length() > 0 ) {
                data->type = server.arg("switch"+String(id)+"_type").toInt();
        }

        if (server.arg("switch"+String(id)+"_sensitiveness").length() > 0 ) {
                data->sensitiveness  = server.arg("switch"+String(id)+"_sensitiveness").toInt();
        }

        if (server.arg("switch"+String(id)+"_functionality").length() > 0 ) {
                data->functionality = server.arg("switch"+String(id)+"_functionality").toInt();
        }


}

void AFEWebServer::getData(DS18B20 *data) {

        if (server.arg("ds18b20_present").length() > 0 ) {
                data->present = true;
        } else {
                data->present = false;
        }

        if (server.arg("ds18b20_gpio").length() > 0 ) {
                data->gpio = server.arg("ds18b20_gpio").toInt();
        }

        if (server.arg("ds18b20_correction").length() > 0 ) {
                data->correction = server.arg("ds18b20_correction").toFloat();
        }

        if (server.arg("ds18b20_interval").length() > 0 ) {
                data->interval = server.arg("ds18b20_interval").toInt();
        }

        if (server.arg("ds18b20_unit").length() > 0 ) {
                data->unit = server.arg("ds18b20_unit").toInt();
        }

}
