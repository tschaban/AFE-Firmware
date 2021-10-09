/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_Data_Structures_h
#define _AFE_Data_Structures_h

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

#ifdef AFE_CONFIG_HARDWARE_MCP23017
#include <AFE-MCP23017-Structure.h>
#endif

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

#ifdef AFE_CONFIG_HARDWARE_DS18B20
#include <AFE-DS18B20-Sensor-Structure.h>
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
#include <AFE-REGULATOR-Structure.h>
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
#include <AFE-THERMAL-PROTECTION-Structure.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_DHT
#include <AFE-DHT-Sensor-Structure.h>
#endif

#ifdef T3_CONFIG
#include <AFE-PIR-Sensor-Structure.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
#include <AFE-ADC-INPUT-Structure.h>
#endif 

/* @TODO BUG For unknown reason non T5 version can't be compiled if these are
 * excluded */

#ifdef AFE_CONFIG_HARDWARE_AS3935
#include <AFE-AS3935-Sensor-Structure.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
#include <AFE-BH1750-Sensor-Structure.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
#include <AFE-BMEX80-Sensor-Structure.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
#include <AFE-HPMA115S0-Sensor-Structure.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_UART
#include <AFE-SERIALPORT-Structure.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_I2C
#include <AFE-I2C-Structure.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
#include <AFE-ANEMOMETER-Sensor-Structure.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
#include <AFE-RAINMETER-Sensor-Structure.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
#include <AFE-BINARY-Sensor-Structure.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
#include <AFE-PN532-Sensor-Structure.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_CLED
#include <AFE-CLED-Structure.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_TSL2561
#include <AFE-TSL2561-Sensor-Structure.h>
#endif 
 
#endif // _AFE_Data_Structures_h
