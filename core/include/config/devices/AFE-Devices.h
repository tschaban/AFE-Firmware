/**
 * @file AFE-Configuration-devices.h
 * @author Tschaban-A (github@adrian.czabanowski.com)
 * @brief Important: AFE_DEVICE_TYPE_NAME can't be longer than 30 charts 
 * @version 3.1.0
 * @date 2021-10-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _AFE_devices_h
#define _AFE_devices_h

/**
 * @brief Size of a Device ID + 1 for '\0
 * 
 */
#define AFE_CONFIG_DEVICE_ID_SIZE 18 

#if defined(AFE_DEVICE_SONOFF_BASIC_V1)
#define AFE_DEVICE_MANUFACTURER "Itead"
#define AFE_DEVICE_TYPE_NAME "Sonoff Basic V1"
#define AFE_DEVICE_TYPE_ID 1

#elif defined(AFE_DEVICE_SONOFF_4CH)
#define AFE_DEVICE_MANUFACTURER "Itead"
#define AFE_DEVICE_TYPE_NAME "Sonoff 4CH"
#define AFE_DEVICE_TYPE_ID 2

#elif defined(AFE_DEVICE_SONOFF_DUAL_R2)
#define AFE_DEVICE_MANUFACTURER "Itead"
#define AFE_DEVICE_TYPE_NAME "Sonoff Dual R2"
#define AFE_DEVICE_TYPE_ID 3

#elif defined(AFE_DEVICE_SONOFF_TH10)
#define AFE_DEVICE_MANUFACTURER "Itead"
#define AFE_DEVICE_TYPE_NAME "Sonoff TH10"
#define AFE_DEVICE_TYPE_ID 4

#elif defined(AFE_DEVICE_SONOFF_TH16)
#define AFE_DEVICE_MANUFACTURER "Itead"
#define AFE_DEVICE_TYPE_NAME "Sonoff TH16"
#define AFE_DEVICE_TYPE_ID 5

#elif defined(AFE_DEVICE_SONOFF_TOUCH_1G)
#define AFE_DEVICE_MANUFACTURER "Itead"
#define AFE_DEVICE_TYPE_NAME "Sonoff Touch 1G"
#define AFE_DEVICE_TYPE_ID 6

#elif defined(AFE_DEVICE_SONOFF_TOUCH_2G)
#define AFE_DEVICE_MANUFACTURER "Itead"
#define AFE_DEVICE_TYPE_NAME "Sonoff Touch 2G"
#define AFE_DEVICE_TYPE_ID 7

#elif defined(AFE_DEVICE_SONOFF_TOUCH_3G)
#define AFE_DEVICE_MANUFACTURER "Itead"
#define AFE_DEVICE_TYPE_NAME "Sonoff Touch 3G"
#define AFE_DEVICE_TYPE_ID 8

#elif defined(AFE_DEVICE_SHELLY_1)
#define AFE_DEVICE_MANUFACTURER "Allterco"
#define AFE_DEVICE_TYPE_NAME "Shelly 1"
#define AFE_DEVICE_TYPE_ID 20

#elif defined(AFE_DEVICE_iECS_GATE_DRIVERv2)
#define AFE_DEVICE_MANUFACTURER "iECS"
#define AFE_DEVICE_TYPE_NAME "iECS Gate Controller v2"
#define AFE_DEVICE_TYPE_ID 21

#elif defined(AFE_DEVICE_iECS_WHEATER_STATION_20)
#define AFE_DEVICE_MANUFACTURER "iECS"
#define AFE_DEVICE_TYPE_NAME "iECS Weather Station v2"
#define AFE_DEVICE_TYPE_ID 22

#elif defined(AFE_DEVICE_iECS_WHEATER_STATION_21)
#define AFE_DEVICE_MANUFACTURER "iECS"
#define AFE_DEVICE_TYPE_NAME "iECS Weather Station v2.1"
#define AFE_DEVICE_TYPE_ID 23

#elif defined(AFE_T1_CUSTOM_E2)
#define AFE_DEVICE_MANUFACTURER "Espressif Systems"
#define AFE_DEVICE_TYPE_NAME "T1 Mega pack"
#define AFE_DEVICE_TYPE_ID 15

#elif defined(AFE_T6_CUSTOM_E1)
#define AFE_DEVICE_MANUFACTURER "Espressif Systems"
#define AFE_DEVICE_TYPE_NAME "T6 E1 Custom"
#define AFE_DEVICE_TYPE_ID 16

#elif defined(AFE_T1_CUSTOM_E1)
#define AFE_DEVICE_MANUFACTURER "Espressif Systems"
#define AFE_DEVICE_TYPE_NAME "T1 E1 Custom"
#define AFE_DEVICE_TYPE_ID 11

#elif defined(AFE_T2_CUSTOM_E1)
#define AFE_DEVICE_MANUFACTURER "Espressif Systems"
#define AFE_DEVICE_TYPE_NAME "T2 E1 Custom"
#define AFE_DEVICE_TYPE_ID 11 // @TODO T2 is this Ok, 11th is set to T1 as well

#elif defined(AFE_T5_CUSTOM_E1)
#define AFE_DEVICE_MANUFACTURER "Espressif Systems"

#ifdef AFE_ESP32_30_PINS /* T5 E1 ESP32-30p */
#define AFE_DEVICE_TYPE_NAME "T5 E1 Custom"
#define AFE_DEVICE_TYPE_ID 12
#elif defined(AFE_ESP32_38_PINS) /* T5 E1 ESP32-38p */
#define AFE_DEVICE_TYPE_NAME "T5 E1 Custom"
#define AFE_DEVICE_TYPE_ID 13
#else /* T5 E1 ESP82663 */
#define AFE_DEVICE_TYPE_NAME "T5 E1 Custom"
#define AFE_DEVICE_TYPE_ID 14
#endif

#elif defined(AFE_DEVICE_iECS_GATE_DRIVERv3)
#define AFE_DEVICE_MANUFACTURER "iECS"
#define AFE_DEVICE_TYPE_NAME "iECS Gate Driver v3"
#define AFE_DEVICE_TYPE_ID 24

#elif defined(AFE_ESP32_30_PINS)
#define AFE_DEVICE_MANUFACTURER "Espressif Systems"
#define AFE_DEVICE_TYPE_NAME "ESP32 30Pins"
#define AFE_DEVICE_TYPE_ID 30

#elif defined(AFE_ESP32_38_PINS)
#define AFE_DEVICE_MANUFACTURER "Espressif Systems"
#define AFE_DEVICE_TYPE_NAME "ESP32 38Pins"
#define AFE_DEVICE_TYPE_ID 31

#else
#define AFE_DEVICE_MANUFACTURER "Espressif Systems"
#if defined(ESP8285)
#define AFE_DEVICE_TYPE_NAME "ESP8285"
#elif defined(ESP32)
#define AFE_DEVICE_TYPE_NAME "ESP32"
#else
#define AFE_DEVICE_TYPE_NAME "ESP8266"
#endif // ESP version
#define AFE_DEVICE_TYPE_ID 0
#endif

/**
 * @brief WARN: 16th is the last one - above
 * Next one should be 17th and added below 16th
 * 
 */

#endif // _AFE_devices_h