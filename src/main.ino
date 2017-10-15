#include <AFE-Data-Access.h>
#include <AFE-Data-Structures.h>
#include <AFE-Defaults.h>
#include <AFE-Web-Server.h>
#include <AFE-WiFi.h>
#include <Streaming.h>

AFEWiFi Network;
AFEWebServer WebServer;

void setup() {

  Serial.begin(115200);
  delay(10);

  AFEDataAccess Data;

  FIRMWARE firmwareConfiguration;
  firmwareConfiguration = Data.getFirmwareConfiguration();

  Serial << endl << "INFO: Reading data from EEPROM";
  Serial << endl << " - Version : " << firmwareConfiguration.version;
  Serial << endl << " - Firmware type : " << firmwareConfiguration.type;
  Serial << endl << " - Auto update : " << firmwareConfiguration.autoUpgrade;
  Serial << endl << " - Update url : " << firmwareConfiguration.upgradeURL;

  Serial << endl << " - Language : " << Data.getLanguage();
  Serial << endl << " - Debugger : " << Data.isDebuggable();
  Serial << endl << " - Device mode : " << Data.getDeviceMode();

  NETWORK networkConfiguration;
  networkConfiguration = Data.getNetworkConfiguration();

  Serial << endl << " - Network configuration";
  Serial << endl << "   - WiFi SSID : " << networkConfiguration.ssid;
  Serial << endl << "   - WiFi Password : " << networkConfiguration.password;
  Serial << endl << "   - Host name : " << networkConfiguration.host;
  Serial << endl << "   - LAN by DHCP? : " << networkConfiguration.isDHCP;
  Serial << endl << "   - IP : " << networkConfiguration.ip;
  Serial << endl << "   - Gateway : " << networkConfiguration.gateway;
  Serial << endl << "   - Subnet : " << networkConfiguration.subnet;
  Serial << endl
         << "   - Number of connection attempts : "
         << networkConfiguration.noConnectionAttempts;
  Serial << endl
         << "   - Duration between connecton attempts : "
         << networkConfiguration.waitTimeConnections;
  Serial << endl
         << "   - Duration between next connection attempts series : "
         << networkConfiguration.waitTimeSeries;

  /* @TODO For MQTT only */
  MQTT MQTTConfiguration;
  MQTTConfiguration = Data.getMQTTConfiguration();

  Serial << endl << " - MQTT Broker";
  Serial << endl << "    - Host : " << MQTTConfiguration.host;
  Serial << endl << "    - IP : " << MQTTConfiguration.ip;
  Serial << endl << "    - Port : " << MQTTConfiguration.port;
  Serial << endl << "    - User : " << MQTTConfiguration.user;
  Serial << endl << "    - Password : " << MQTTConfiguration.password;
  Serial << endl << "    - Topic : " << MQTTConfiguration.topic;

  /* @TODO For Domoticz only */
  DOMOTICZ DomoticzConfiguration;
  DomoticzConfiguration = Data.getDomoticzConfiguration();

  Serial << endl << " - Domoticz";
  Serial << endl << "    - Host : " << DomoticzConfiguration.host;
  Serial << endl << "    - IP : " << DomoticzConfiguration.ip;
  Serial << endl << "    - Port : " << DomoticzConfiguration.port;
  Serial << endl << "    - User : " << DomoticzConfiguration.user;
  Serial << endl << "    - Password : " << DomoticzConfiguration.password;

  RELAY RelayConfiguration;
  RelayConfiguration = Data.getRelayConfiguration(0);

  Serial << endl << " - Relay : ";
  Serial << endl << "    - Present : " << RelayConfiguration.present;
  Serial << endl << "    - State : " << Data.getRelayState(0);
  Serial << endl << "    - GPIO : " << RelayConfiguration.gpio;
  Serial << endl << "    - Time to off : " << RelayConfiguration.timeToOff;
  Serial << endl
         << "    - State after power on : " << RelayConfiguration.statePowerOn;
  /* @TODO For MQTT only */
  Serial << endl << "    - Relay name : " << RelayConfiguration.name;
  Serial << endl
         << "    - Relay state after mqtt connected : "
         << RelayConfiguration.stateMQTTConnected;
  /* @TODO For Domoticz only */
  Serial << endl << "    - Domoticz IDX : " << RelayConfiguration.idx;
  Serial << endl
         << "    - Domoticz Publish state : "
         << RelayConfiguration.publishToDomoticz;

  SWITCH SwitchConfiguration;
  SwitchConfiguration = Data.getSwitchConfiguration(0);

  Serial << endl << " - Switch : ";
  Serial << endl << "    - Present : " << SwitchConfiguration.present;
  Serial << endl << "    - GPIO : " << SwitchConfiguration.gpio;
  Serial << endl << "    - Type : " << SwitchConfiguration.type;
  Serial << endl
         << "    - Sensitiveness : " << SwitchConfiguration.sensitiveness;
  Serial << endl
         << "    - Functionality : " << SwitchConfiguration.functionality;

  DS18B20 DS18B20Configuration;
  DS18B20Configuration = Data.getDS18B20Configuration();

  Serial << endl << " - DS18B20 : ";
  Serial << endl << "    - Present : " << DS18B20Configuration.present;
  Serial << endl << "    - GPIO : " << DS18B20Configuration.gpio;
  Serial << endl << "    - Correction by : " << DS18B20Configuration.correction;
  Serial << endl << "    - Read interval : " << DS18B20Configuration.interval;
  Serial << endl << "    - Unit : " << DS18B20Configuration.unit;

  Network.setReconnectionParams(networkConfiguration.noConnectionAttempts,
                                networkConfiguration.waitTimeConnections,
                                networkConfiguration.waitTimeSeries);

  if (!networkConfiguration.isDHCP) {
    Network.setStaticIP(networkConfiguration.ip, networkConfiguration.gateway,
                        networkConfiguration.subnet);
  }

  Network.begin(networkConfiguration.ssid, networkConfiguration.password,
                networkConfiguration.host, WIFI_MODE_SERVER);
  Network.connect();

  WebServer.handle("/", handleHTTPRequests);
  WebServer.handle("/favicon.ico", handleFavicon);
  WebServer.begin();
}

void loop() {
  if (Network.connected()) {
    WebServer.listener();
  } else {
    Network.connect();
  }
}
