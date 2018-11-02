/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_Data_Structures_h
#define _AFE_Data_Structures_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-DEVICE-Structure.h>
#include <AFE-DOMOTICZ-Structure.h>
#include <AFE-FIRMWARE-Structure.h>
#include <AFE-HTTP-COMMAND-Structure.h>

#include <AFE-MQTT-Structure.h>
#include <AFE-NETWORK-Structure.h>
#include <AFE-RELAY-Structure.h>
#include <AFE-SWITCH-Structure.h>

#ifndef T0_SHELLY_1_CONFIG
#include <AFE-LED-Structure.h>
#endif

#ifdef T1_CONFIG
#include <AFE-DS18B20-Sensor-Structure.h>
#endif

#ifdef T2_CONFIG
#include <AFE-DHT-Sensor-Structure.h>
#endif

#endif
