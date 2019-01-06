/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_Configuration_h
#define _AFE_Configuration_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#if defined(T0_CONFIG)
#define FIRMWARE_VERSION "1.4.0"
#define FIRMWARE_TYPE 0
#define CONFIG_HARDWARE_LED

#elif defined(T0_SHELLY_1_CONFIG)
#define FIRMWARE_VERSION "1.4.0"
#define FIRMWARE_TYPE 0
#define HARDWARE_SWITCH_GPIO_DIGIT_INPUT // Sets switch DigitialPin to INPUT

#elif defined(T1_CONFIG)
#define FIRMWARE_VERSION "1.4.0"
#define FIRMWARE_TYPE 1
#define CONFIG_HARDWARE_DS18B20
#define CONFIG_HARDWARE_LED
#define CONFIG_FUNCTIONALITY_THERMOSTAT
#define CONFIG_FUNCTIONALITY_THERMAL_PROTECTION

#elif defined(T2_CONFIG)
#define FIRMWARE_VERSION "1.3.4"
#define FIRMWARE_TYPE 2
#define CONFIG_HARDWARE_DHXX
#define CONFIG_HARDWARE_LED
#define CONFIG_FUNCTIONALITY_THERMOSTAT
#define CONFIG_FUNCTIONALITY_HUMIDISTAT
#define CONFIG_FUNCTIONALITY_THERMAL_PROTECTION

#elif defined(T3_CONFIG)
#define FIRMWARE_TYPE 3
#define FIRMWARE_VERSION "1.3.0"
#define CONFIG_HARDWARE_LED

#elif defined(T4_CONFIG)
#define FIRMWARE_VERSION "1.3.1"
#define FIRMWARE_TYPE 4
#define CONFIG_HARDWARE_LED

#elif defined(T5_CONFIG)
#define FIRMWARE_VERSION "1.3.2"
#define FIRMWARE_TYPE 5
#define CONFIG_HARDWARE_DHXX
#define CONFIG_HARDWARE_LED

#elif defined(T6_CONFIG)
#define FIRMWARE_VERSION "1.0.0"
#define FIRMWARE_TYPE 6
#define CONFIG_HARDWARE_BMX80
#define CONFIG_HARDWARE_HPMA115S0
#define CONFIG_HARDWARE_BH1750
#define CONFIG_HARDWARE_LED

#endif

/* Configs related to functionalities */

#ifdef CONFIG_FUNCTIONALITY_THERMOSTAT

#ifndef CONFIG_FUNCTIONALITY_REGULATOR
#define CONFIG_FUNCTIONALITY_REGULATOR
#endif

#endif

#ifdef CONFIG_FUNCTIONALITY_HUMIDISTAT

#ifndef CONFIG_FUNCTIONALITY_REGULATOR
#define CONFIG_FUNCTIONALITY_REGULATOR
#endif

#endif

/* Configs related to specyfic hardware */

#ifdef CONFIG_HARDWARE_DS18B20

#ifndef CONFIG_TEMPERATURE
#define CONFIG_TEMPERATURE
#endif

#endif

#ifdef CONFIG_HARDWARE_DHXX

#ifndef CONFIG_TEMPERATURE
#define CONFIG_TEMPERATURE
#endif

#ifndef CONFIG_HUMIDITY
#define CONFIG_HUMIDITY
#endif

#endif

#ifdef CONFIG_HARDWARE_BMX80
#ifndef CONFIG_TEMPERATURE
#define CONFIG_TEMPERATURE
#endif

#ifndef CONFIG_HUMIDITY
#define CONFIG_HUMIDITY
#endif

#ifndef CONFIG_PRESSURE
#define CONFIG_PRESSURE
#endif

#ifndef CONFIG_HARDWARE_I2C
#define CONFIG_HARDWARE_I2C
#endif

#endif

#ifdef CONFIG_HARDWARE_BH1750
#ifndef CONFIG_HARDWARE_I2C
#define CONFIG_HARDWARE_I2C
#endif
#endif

#ifdef CONFIG_HARDWARE_HPMA115S0

#ifndef CONFIG_HARDWARE_UART
#define CONFIG_HARDWARE_UART
#endif

#endif

#endif
