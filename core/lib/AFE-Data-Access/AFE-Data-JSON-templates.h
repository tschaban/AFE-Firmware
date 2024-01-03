#ifndef _AFE_Data_JSON_templates_h
#define _AFE_Data_JSON_templates_h

#ifdef AFE_ESP32
const char JSON_CONFIG_NETWORK[] PROGMEM = "{\"noConnectionAttempts\":%d,\"waitTimeConnections\":%d,\"waitTimeSeries\":%d,\"noFailuresToSwitchNetwork\":%d,\"ssid\":\"%s\",\"password\":\"%s\",\"isDHCP\":%d,\"ip\":\"%s\",\"gateway\":\"%s\",\"subnet\":\"%s\",\"dns1\":\"%s\",\"dns2\":\"%s\",\"ssidBackup\":\"%s\",\"passwordBackup\":\"%s\",\"isDHCPBackup\":%d,\"ipBackup\":\"%s\",\"gatewayBackup\":\"%s\",\"subnetBackup\":\"%s\",\"dns1Backup\":\"%s\",\"dns2Backup\":\"%s\",\"m\":%d}";
#else
const char JSON_CONFIG_NETWORK[] PROGMEM = "{\"outputPower\":%.2f,\"radioMode\":%d,\"noConnectionAttempts\":%d,\"waitTimeConnections\":%d,\"waitTimeSeries\":%d,\"noFailuresToSwitchNetwork\":%d,\"ssid\":\"%s\",\"password\":\"%s\",\"isDHCP\":%d,\"ip\":\"%s\",\"gateway\":\"%s\",\"subnet\":\"%s\",\"dns1\":\"%s\",\"dns2\":\"%s\",\"ssidBackup\":\"%s\",\"passwordBackup\":\"%s\",\"isDHCPBackup\":%d,\"ipBackup\":\"%s\",\"gatewayBackup\":\"%s\",\"subnetBackup\":\"%s\",\"dns1Backup\":\"%s\",\"dns2Backup\":\"%s\",\"m\":%d}";
#endif
//#define JSON_CONFIG_NETWORK "{\"outputPower\":%.2f,\"radioMode\":%d,\"noConnectionAttempts\":%d,\"waitTimeConnections\":%d,\"waitTimeSeries\":%d,\"noFailuresToSwitchNetwork\":%d,\"ssid\":\"%s\",\"password\":\"%s\",\"isDHCP\":%d,\"ip\":\"%s\",\"gateway\":\"%s\",\"subnet\":\"%s\",\"dns1\":\"%s\",\"dns2\":\"%s\",\"ssidBackup\":\"%s\",\"passwordBackup\":\"%s\",\"isDHCPBackup\":%d,\"ipBackup\":\"%s\",\"gatewayBackup\":\"%s\",\"subnetBackup\":\"%s\",\"dns1Backup\":\"%s\",\"dns2Backup\":\"%s\"}"


#endif