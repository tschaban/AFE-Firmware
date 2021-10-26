#ifndef _AFE_Configuration_T_h
#define _AFE_Configuration_T_h

#define AFE_FIRMWARE_VERSION "3.0.2"
#define AFE_FIRMWARE_TYPE 2

/* Functionalities */
#define AFE_CONFIG_FUNCTIONALITY_RELAY
#define AFE_CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
#define AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
#define AFE_CONFIG_FUNCTIONALITY_REGULATOR
#define AFE_CONFIG_FUNCTIONALITY_ADC

/* Define Hardware */
#define AFE_CONFIG_HARDWARE_SWITCH
#define AFE_CONFIG_HARDWARE_DHT
#define AFE_CONFIG_HARDWARE_LED

/* Max number of hardware items, per AFE version */
#ifdef AFE_CONFIG_T1_USE_MAX_HARDWARE
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_RELAYS 8
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_SWITCHES 8
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_LEDS 8
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_DHT 8
#else
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_RELAYS 4
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_SWITCHES 5
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_LEDS 5
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_DHT 4
#endif

#define AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_RELAYS
#define AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_SWITCHES
#define AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_LEDS
#define AFE_CONFIG_HARDWARE_NUMBER_OF_DHT AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_DHT

#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_RELAYS 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_SWITCHES 1
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_LEDS 1
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_DHT 0

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_REGULATORS AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS
#define AFE_CONFIG_HARDWARE_NUMBER_OF_REGULATORS AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_REGULATORS 0
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_THERMAL_PROTECTOR AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS
#define AFE_CONFIG_HARDWARE_NUMBER_OF_THERMAL_PROTECTORS AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_THERMAL_PROTECTIORS 0
#endif

#endif // _AFE_Configuration_T_h