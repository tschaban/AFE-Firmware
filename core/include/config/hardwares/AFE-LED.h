#ifndef _AFE_Hardware_LED_h
#define _AFE_Hardware_LED_h

#if defined(AFE_DEVICE_SONOFF_BASIC_V1)
#define AFE_CONFIG_HARDWARE_LED_0_DEFAULT_GPIO 13
#define AFE_CONFIG_HARDWARE_LED_1_DEFAULT_GPIO 14
#elif defined(AFE_DEVICE_SONOFF_4CH)
#define AFE_CONFIG_HARDWARE_LED_0_DEFAULT_GPIO 13
#elif defined(AFE_DEVICE_SONOFF_DUAL_R2)
#define AFE_CONFIG_HARDWARE_LED_0_DEFAULT_GPIO 13
#elif defined(AFE_DEVICE_SONOFF_TH10)
#define AFE_CONFIG_HARDWARE_LED_0_DEFAULT_GPIO 13
#elif defined(AFE_DEVICE_SONOFF_TH16)
#define AFE_CONFIG_HARDWARE_LED_0_DEFAULT_GPIO 13
#elif defined(AFE_DEVICE_SONOFF_TOUCH_1G)
#define AFE_CONFIG_HARDWARE_LED_0_DEFAULT_GPIO 13
#elif defined(AFE_DEVICE_SONOFF_TOUCH_2G)
#define AFE_CONFIG_HARDWARE_LED_0_DEFAULT_GPIO 13
#elif defined(AFE_DEVICE_SONOFF_TOUCH_3G)
#define AFE_CONFIG_HARDWARE_LED_0_DEFAULT_GPIO 13
#elif defined(AFE_DEVICE_SONOFF_MINI_R4)
#define AFE_CONFIG_HARDWARE_LED_0_DEFAULT_GPIO 19
#elif defined(AFE_DEVICE_iECS_GATE_DRIVERv2) || defined(AFE_DEVICE_iECS_GATE_DRIVERv3)
#define AFE_CONFIG_HARDWARE_LED_0_DEFAULT_GPIO 2
#elif defined(AFE_DEVICE_WHEATER_STATION_iECSv20) || defined(AFE_DEVICE_WHEATER_STATION_iECSv21)
#define AFE_CONFIG_HARDWARE_LED_0_DEFAULT_GPIO 2
#else
#define AFE_CONFIG_HARDWARE_LED_0_DEFAULT_GPIO 2
#endif

#endif // _AFE_Hardware_LED_h