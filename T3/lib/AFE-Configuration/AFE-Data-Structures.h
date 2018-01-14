/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#ifndef _AFE_Data_Structures_h
#define _AFE_Data_Structures_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <ESP8266WiFi.h>

#include <AFE-DEVICE-Structure.h>
#include <AFE-FIRMWARE-Structure.h>
#include <AFE-HTTP-COMMAND-Structure.h>
#include <AFE-LED-Structure.h>
#include <AFE-MQTT-Structure.h>
#include <AFE-NETWORK-Structure.h>
#include <AFE-PIR-Sensor-Structure.h>
#include <AFE-RELAY-Structure.h>
#include <AFE-SWITCH-Structure.h>

#endif
