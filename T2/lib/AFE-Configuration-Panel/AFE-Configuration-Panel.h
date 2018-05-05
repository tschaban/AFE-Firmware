/* AFE Firmware for smart home devices
 LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
 DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#ifndef _AFE_Configuration_Panel_h
#define _AFE_Configuration_Panel_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Common-Configuration.h>
#include <AFE-Device.h>
#include <AFE-Sites-Generator.h>
// #include <Streaming.h>

class AFEConfigurationPanel {

private:
  AFESitesGenerator Site;
  AFEDataAccess Data;
  AFEDevice Device;
  uint8_t language;
  uint16_t siteBufferSize = 7500;

public:
  /* Constructor */
  AFEConfigurationPanel();

  /* It returns configuration site */
  String getSite(const String option, uint8_t command, boolean data);
  String getDeviceConfigurationSite(uint8_t command, DEVICE data);
  String getNetworkConfigurationSite(uint8_t command, NETWORK data);
  String getMQTTConfigurationSite(uint8_t command, MQTT data);
  String getDomoticzServerConfigurationSite(uint8_t command, DOMOTICZ data);
  String getLEDConfigurationSite(uint8_t command,
                                 LED data[sizeof(Device.configuration.isLED)],
                                 uint8_t dataLedID);
  String getRelayConfigurationSite(uint8_t command, RELAY data,
                                   uint8_t relayIndex);

  String getRelayStatConfigurationSite(uint8_t command, REGULATOR data,
                                       boolean thermostat, uint8_t relayIndex);

  String getSwitchConfigurationSite(uint8_t command, SWITCH data,
                                    uint8_t relayIndex);
  String getLanguageConfigurationSite(uint8_t command, uint8_t lang);
  String getDHTConfigurationSite(uint8_t command, DH data);

  /* It generates site for firmware upgrade */
  String firmwareUpgradeSite();

  /* It generates sites post firmware upgrade */
  String postFirmwareUpgradeSite(boolean status);
};

#endif
