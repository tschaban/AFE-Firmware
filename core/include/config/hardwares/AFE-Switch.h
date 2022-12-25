#ifndef _AFE_Hardware_Switch_h
#define _AFE_Hardware_Switch_h

/* Switches, types */
#define AFE_SWITCH_TYPE_MONO 0 // Mono stable switch
#define AFE_SWITCH_TYPE_BI 1   // Bistable switch

/* Switch ON/OFF Values */
#define AFE_SWITCH_ON AFE_ON
#define AFE_SWITCH_OFF AFE_OFF

/* Switches functionalities */
#define AFE_SWITCH_FUNCTIONALITY_NONE 0  // do nothing
#define AFE_SWITCH_FUNCTIONALITY_MULTI 1 // System switch and relay control
#define AFE_SWITCH_FUNCTIONALITY_RELAY 2 // Relay control only
#define AFE_SWITCH_FUNCTIONALITY_RGBLED 3 // RGB LED

/* Switche bouncing */
#define AFE_HARDWARE_SWITCH_DEFAULT_BOUNCING 50 // Bouncing for switch in miliseconds

#if defined(AFE_DEVICE_SONOFF_BASIC_V1)
#define AFE_HARDWARE_SWITCH_0_DEFAULT_GPIO 0
#define AFE_HARDWARE_SWITCH_1_DEFAULT_GPIO 14
#define AFE_HARDWARE_SWITCH_2_DEFAULT_GPIO 1
#define AFE_HARDWARE_SWITCH_3_DEFAULT_GPIO 3
#define AFE_HARDWARE_SWITCH_0_DEFAULT_TYPE AFE_SWITCH_TYPE_MONO
#define AFE_HARDWARE_SWITCH_X_DEFAULT_TYPE AFE_SWITCH_TYPE_BI // Default for all switches exlcuding the first one
#define AFE_HARDWARE_SWITCH_0_DEFAULT_FUNCTIONALITY AFE_SWITCH_FUNCTIONALITY_MULTI
#define AFE_HARDWARE_SWITCH_X_DEFAULT_FUNCTIONALITY AFE_SWITCH_FUNCTIONALITY_RELAY // Default for all switches exlcuding the first one
#elif defined(AFE_DEVICE_SONOFF_4CH)
#define AFE_HARDWARE_SWITCH_0_DEFAULT_GPIO 0
#define AFE_HARDWARE_SWITCH_1_DEFAULT_GPIO 9
#define AFE_HARDWARE_SWITCH_2_DEFAULT_GPIO 10
#define AFE_HARDWARE_SWITCH_3_DEFAULT_GPIO 14
#define AFE_HARDWARE_SWITCH_0_DEFAULT_TYPE AFE_SWITCH_TYPE_MONO
#define AFE_HARDWARE_SWITCH_X_DEFAULT_TYPE AFE_SWITCH_TYPE_MONO // Default for all switches exlcuding the first one
#define AFE_HARDWARE_SWITCH_0_DEFAULT_FUNCTIONALITY AFE_SWITCH_FUNCTIONALITY_MULTI
#define AFE_HARDWARE_SWITCH_X_DEFAULT_FUNCTIONALITY AFE_SWITCH_FUNCTIONALITY_RELAY // Default for all switches exlcuding the first one
#elif defined(AFE_DEVICE_SONOFF_TOUCH_1G)
#define AFE_HARDWARE_SWITCH_0_DEFAULT_GPIO 0
#define AFE_HARDWARE_SWITCH_0_DEFAULT_TYPE AFE_SWITCH_TYPE_MONO
#define AFE_HARDWARE_SWITCH_0_DEFAULT_FUNCTIONALITY AFE_SWITCH_FUNCTIONALITY_MULTI
// actually below 2 doesn't matter, they're needed to pharse the firmware ;)
#define AFE_HARDWARE_SWITCH_X_DEFAULT_TYPE AFE_SWITCH_TYPE_MONO // Default for all switches exlcuding the first one
#define AFE_HARDWARE_SWITCH_X_DEFAULT_FUNCTIONALITY AFE_SWITCH_FUNCTIONALITY_RELAY // Default for all switches exlcuding the first one
#elif defined(AFE_DEVICE_SONOFF_TOUCH_2G)
#define AFE_HARDWARE_SWITCH_0_DEFAULT_GPIO 0
#define AFE_HARDWARE_SWITCH_1_DEFAULT_GPIO 9
#define AFE_HARDWARE_SWITCH_0_DEFAULT_TYPE AFE_SWITCH_TYPE_MONO
#define AFE_HARDWARE_SWITCH_X_DEFAULT_TYPE AFE_SWITCH_TYPE_MONO // Default for all switches exlcuding the first one
#define AFE_HARDWARE_SWITCH_0_DEFAULT_FUNCTIONALITY AFE_SWITCH_FUNCTIONALITY_MULTI
#define AFE_HARDWARE_SWITCH_X_DEFAULT_FUNCTIONALITY AFE_SWITCH_FUNCTIONALITY_RELAY // Default for all switches exlcuding the first one
#elif defined(AFE_DEVICE_SONOFF_TOUCH_3G)
#define AFE_HARDWARE_SWITCH_0_DEFAULT_GPIO 0
#define AFE_HARDWARE_SWITCH_1_DEFAULT_GPIO 9
#define AFE_HARDWARE_SWITCH_2_DEFAULT_GPIO 10
#define AFE_HARDWARE_SWITCH_0_DEFAULT_TYPE AFE_SWITCH_TYPE_MONO
#define AFE_HARDWARE_SWITCH_X_DEFAULT_TYPE  AFE_SWITCH_TYPE_MONO // Default for all switches exlcuding the first one
#define AFE_HARDWARE_SWITCH_0_DEFAULT_FUNCTIONALITY AFE_SWITCH_FUNCTIONALITY_MULTI
#define AFE_HARDWARE_SWITCH_X_DEFAULT_FUNCTIONALITY AFE_SWITCH_FUNCTIONALITY_RELAY // Default for all switches exlcuding the first one
#elif defined(AFE_DEVICE_SHELLY_1)
#define AFE_HARDWARE_SWITCH_0_DEFAULT_GPIO 5
#define AFE_HARDWARE_SWITCH_0_DEFAULT_TYPE AFE_SWITCH_TYPE_BI
#define AFE_HARDWARE_SWITCH_0_DEFAULT_FUNCTIONALITY AFE_SWITCH_FUNCTIONALITY_RELAY
// actually below 2 doesn't matter, they're needed to pharse the firmware ;)
#define AFE_HARDWARE_SWITCH_X_DEFAULT_TYPE AFE_SWITCH_TYPE_MONO // Default for all switches exlcuding the first one
#define AFE_HARDWARE_SWITCH_X_DEFAULT_FUNCTIONALITY AFE_SWITCH_FUNCTIONALITY_RELAY // Default for all switches exlcuding the first one
#elif defined(AFE_DEVICE_iECS_GATE_DRIVERv2)
#define AFE_HARDWARE_SWITCH_0_DEFAULT_GPIO 0
#define AFE_HARDWARE_SWITCH_1_DEFAULT_GPIO 1
#define AFE_HARDWARE_SWITCH_0_DEFAULT_TYPE AFE_SWITCH_TYPE_MONO
#define AFE_HARDWARE_SWITCH_X_DEFAULT_TYPE AFE_SWITCH_TYPE_MONO // Default for all switches exlcuding the first one
#define AFE_HARDWARE_SWITCH_0_DEFAULT_FUNCTIONALITY AFE_SWITCH_FUNCTIONALITY_MULTI
#define AFE_HARDWARE_SWITCH_X_DEFAULT_FUNCTIONALITY AFE_SWITCH_FUNCTIONALITY_NONE // Default for all switches exlcuding the first one
#elif defined(AFE_DEVICE_iECS_GATE_DRIVERv3)
#define AFE_HARDWARE_SWITCH_0_DEFAULT_GPIO 5
//#define AFE_HARDWARE_SWITCH_1_DEFAULT_GPIO 5
#define AFE_HARDWARE_SWITCH_0_DEFAULT_TYPE AFE_SWITCH_TYPE_MONO
#define AFE_HARDWARE_SWITCH_X_DEFAULT_TYPE AFE_SWITCH_TYPE_MONO // Default for all switches exlcuding the first one
#define AFE_HARDWARE_SWITCH_0_DEFAULT_FUNCTIONALITY AFE_SWITCH_FUNCTIONALITY_MULTI
#define AFE_HARDWARE_SWITCH_X_DEFAULT_FUNCTIONALITY AFE_SWITCH_FUNCTIONALITY_NONE // Default for all switches exlcuding the first one
#elif defined(AFE_DEVICE_iECS_WHEATER_STATION_20) || defined(AFE_DEVICE_iECS_WHEATER_STATION_21)
#define AFE_HARDWARE_SWITCH_0_DEFAULT_GPIO 0
#define AFE_HARDWARE_SWITCH_1_DEFAULT_GPIO 14
#define AFE_HARDWARE_SWITCH_2_DEFAULT_GPIO 13
#define AFE_HARDWARE_SWITCH_0_DEFAULT_TYPE AFE_SWITCH_TYPE_MONO
#define AFE_HARDWARE_SWITCH_X_DEFAULT_TYPE AFE_SWITCH_TYPE_MONO // Default for all switches exlcuding the first one
#define AFE_HARDWARE_SWITCH_0_DEFAULT_FUNCTIONALITY AFE_SWITCH_FUNCTIONALITY_MULTI
#define AFE_HARDWARE_SWITCH_X_DEFAULT_FUNCTIONALITY AFE_SWITCH_FUNCTIONALITY_NONE // Default for all switches exlcuding the first one
#else
#define AFE_HARDWARE_SWITCH_0_DEFAULT_GPIO 0
#define AFE_HARDWARE_SWITCH_1_DEFAULT_GPIO 0
#define AFE_HARDWARE_SWITCH_2_DEFAULT_GPIO 0
#define AFE_HARDWARE_SWITCH_3_DEFAULT_GPIO 0
#define AFE_HARDWARE_SWITCH_0_DEFAULT_TYPE AFE_SWITCH_TYPE_MONO
#define AFE_HARDWARE_SWITCH_X_DEFAULT_TYPE AFE_SWITCH_TYPE_BI // Default for all switches exlcuding the first one
#define AFE_HARDWARE_SWITCH_0_DEFAULT_FUNCTIONALITY AFE_SWITCH_FUNCTIONALITY_MULTI
#define AFE_HARDWARE_SWITCH_X_DEFAULT_FUNCTIONALITY AFE_SWITCH_FUNCTIONALITY_NONE // Default for all switches exlcuding the first one
#endif

#endif // _AFE_Hardware_Switch_h