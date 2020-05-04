/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Data_Structures_h
#define _AFE_Data_Structures_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-DEVICE-Structure.h>
#include <AFE-FIRMWARE-Structure.h>
#include <AFE-HTTP-COMMAND-Structure.h>

/* APIs */
#include <AFE-MQTT-Structure.h>
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
#include <AFE-DOMOTICZ-Structure.h>
#endif

#include <AFE-NETWORK-Structure.h>
#include <AFE-PASSWORD-Structure.h>
#include <AFE-PRO-VERSION-Structure.h>

#ifdef AFE_CONFIG_HARDWARE_RELAY
#include <AFE-RELAY-Structure.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
#include <AFE-SWITCH-Structure.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
#include <AFE-LED-Structure.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
#include <AFE-CONTACTRON-Structure.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
#include <AFE-GATE-Structure.h>
#endif

#ifdef T1_CONFIG
#include <AFE-DS18B20-Sensor-Structure.h>
#endif

#if defined(T2_CONFIG)
#include <AFE-DHT-Sensor-Structure.h>
#endif

#ifdef T3_CONFIG
#include <AFE-PIR-Sensor-Structure.h>
#endif

/* @TODO BUG For unknown reason non T5 version can't be compiled if these are
 * excluded */
#include <AFE-ADC-INPUT-Structure.h>
#include <AFE-AS3935-Sensor-Structure.h>
#include <AFE-BH1750-Sensor-Structure.h>
#include <AFE-BMEX80-Sensor-Structure.h>
#include <AFE-HPMA115S0-Sensor-Structure.h>

#ifdef AFE_CONFIG_HARDWARE_UART
#include <AFE-SERIALPORT-Structure.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_I2C
#include <AFE-I2C-Structure.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER_SENSOR
#include <AFE-ANEMOMETER-Sensor-Structure.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER_SENSOR
#include <AFE-RAINMETER-Sensor-Structure.h>
#endif

#endif
