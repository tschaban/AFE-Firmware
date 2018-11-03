/* AFE Firmware for smart home devices
 LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

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

#ifdef DEBUG
#include <Streaming.h>
#endif

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

#ifndef T0_SHELLY_1_CONFIG
  String getLEDConfigurationSite(uint8_t command,
                                 LED data[sizeof(Device.configuration.isLED)],
                                 uint8_t dataLedID);
#endif

  String getRelayConfigurationSite(uint8_t command, RELAY data,
                                   uint8_t relayIndex);

#if defined(T1_CONFIG) || defined(T2_CONFIG)
  String getRelayStatConfigurationSite(uint8_t command, REGULATOR data,
                                       uint8_t regulatorType);
#endif

  String getSwitchConfigurationSite(uint8_t command, SWITCH data,
                                    uint8_t relayIndex);
  String getLanguageConfigurationSite(uint8_t command, uint8_t lang);

#ifdef T1_CONFIG
  String getDS18B20ConfigurationSite(uint8_t command, DS18B20 data);
#endif

#if defined(T2_CONFIG) || defined(T5_CONFIG)
  String getDHTConfigurationSite(uint8_t command, DH data);
#endif

#if defined(T5_CONFIG)
  String getContactronConfigurationSite(const String option, uint8_t command,
                                        CONTACTRON data,
                                        uint8_t contactronIndex);
  String getGateConfigurationSite(const String option, uint8_t command,
                                  GATE data);
#endif

  /* It generates site for firmware upgrade */
  String firmwareUpgradeSite();

  /* It generates sites post firmware upgrade */
  String postFirmwareUpgradeSite(boolean status);
};
#endif
