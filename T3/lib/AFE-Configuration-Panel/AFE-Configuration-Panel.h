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
                                 LED data[5]);
  String getRelayConfigurationSite(const String option, uint8_t command,
                                   RELAY data, uint8_t relayIndex);
  String getSwitchConfigurationSite(const String option, uint8_t command,
                                    SWITCH data, uint8_t relayIndex);
  String getPIRConfigurationSite(const String option, uint8_t command, PIR data,
                                 uint8_t PIRIndex);

  String getLanguageConfigurationSite(const String option, uint8_t command,
                                      uint8_t lang);
  /* It generates site for firmware upgrade */
  String firmwareUpgradeSite();

  /* It generates sites post firmware upgrade */
  String postFirmwareUpgradeSite(boolean status);
};

#endif
