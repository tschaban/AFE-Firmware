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

#include <AFE-Device.h>
#include <AFE-Sites-Generator.h>
// include <Streaming.h>

class AFEConfigurationPanel {

private:
  AFESitesGenerator Site;
  AFEDataAccess Data;
  AFEDevice Device;
  uint8_t language;

public:
  /* Constructor */
  AFEConfigurationPanel();

  /* It returns configuration site */
  String getSite(const String option, uint8_t command, boolean data);
  String getDeviceConfigurationSite(const String option, uint8_t command,
                                    DEVICE data);
  String getNetworkConfigurationSite(const String option, uint8_t command,
                                     NETWORK data);
  String getMQTTConfigurationSite(const String option, uint8_t command,
                                  MQTT data);
  String getLEDConfigurationSite(const String option, uint8_t command,
                                 LED data);
  String getRelayConfigurationSite(const String option, uint8_t command,
                                   RELAY data);

  String getRelayStatConfigurationSite(const String option, uint8_t command,
                                       RELAYSTAT data, boolean thermostat);

  String getSwitchConfigurationSite(const String option, uint8_t command,
                                    SWITCH data1, SWITCH data2);
  String getLanguageConfigurationSite(const String option, uint8_t command,
                                      uint8_t lang);
  String getDHTConfigurationSite(const String option, uint8_t command, DH data);

  /* It generates site for firmware upgrade */
  String firmwareUpgradeSite();

  /* It generates sites post firmware upgrade */
  String postFirmwareUpgradeSite(boolean status);
};

#endif
