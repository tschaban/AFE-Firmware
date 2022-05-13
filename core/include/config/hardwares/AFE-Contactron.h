#ifndef _AFE_Hardware_Contactron_h
#define _AFE_Hardware_Contactron_h

/* Contactron types */
#define AFE_CONTACTRON_NO 0 // Normally open
#define AFE_CONTACTRON_NC 1 // Normally closed
/* Contactron states */
#define AFE_CONTACTRON_OPEN 0   // Open
#define AFE_CONTACTRON_CLOSED 1 // Closed
/* Contactron defaults */
#define AFE_CONFIG_HARDWARE_CONTACTRON_DEFAULT_BOUNCING 200
#define AFE_CONFIG_HARDWARE_CONTACTRON_DEFAULT_OUTPUT_TYPE AFE_CONTACTRON_NO


#define AFE_CONFIG_API_JSON_CONTACTRON_DATA_LENGTH 200  // Not checked. used by HTTP API


#if defined(AFE_DEVICE_iECS_GATE_DRIVERv2) 
#define AFE_CONFIG_HARDWARE_CONTACTRON_1_DEFAULT_GPIO 14
#define AFE_CONFIG_HARDWARE_CONTACTRON_2_DEFAULT_GPIO 13
#define AFE_CONFIG_HARDWARE_CONTACTRON_3_DEFAULT_GPIO 3
#elif defined(AFE_DEVICE_iECS_GATE_DRIVERv3) 
#define AFE_CONFIG_HARDWARE_CONTACTRON_1_DEFAULT_GPIO 14
#define AFE_CONFIG_HARDWARE_CONTACTRON_2_DEFAULT_GPIO 13
#define AFE_CONFIG_HARDWARE_CONTACTRON_3_DEFAULT_GPIO 4
#define AFE_CONFIG_HARDWARE_CONTACTRON_4_DEFAULT_GPIO 5
#endif
#define AFE_CONFIG_HARDWARE_CONTACTRON_X_DEFAULT_GPIO AFE_HARDWARE_ITEM_NOT_EXIST


#endif // _AFE_Hardware_Contactron_h