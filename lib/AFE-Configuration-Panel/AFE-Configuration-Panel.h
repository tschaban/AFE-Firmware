/*
   Configuration Panel
   AFE Firmware for smart home devices build on ESP8266
   More info: https://github.com/tschaban/AFE-Firmware
   LICENCE: http://opensource.org/licenses/MIT
 */

#ifndef _AFE_Configuration_Panel_h
#define _AFE_Configuration_Panel_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Device.h>
#include <AFE-Sites-Generator.h>
#include <Streaming.h>

class AFEConfigurationPanel {

private:
  AFESitesGenerator Site;
  String getDomoticzConfigurationSite();

public:
  /* Constructor: entry parameter is GPIO number where Sensor is connected to */
  AFEConfigurationPanel();

  /* It returns configuration site */
  String getSite(const String option, uint8_t command);
  String getSite(const String option, uint8_t command, NETWORK data);
  String getSite(const String option, uint8_t command, MQTT data);
  String getSite(const String option, uint8_t command, DOMOTICZ data);
  String getSite(const String option, uint8_t command, DS18B20 data);
  String getSite(const String option, uint8_t command, boolean data);
  String getSite(const String option, uint8_t command, RELAY data1,
                 RELAY data2);
  String getSite(const String option, uint8_t command, SWITCH data1,
                 SWITCH data2);
  String firmwareUpgradeSite();
  String postFirmwareUpgradeSite(boolean status);
};

#endif
