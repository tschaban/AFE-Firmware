#ifndef _AFE_Hardware_Regulator_h
#define _AFE_Hardware_Regulator_h

#define AFE_FUNCTIONALITY_REGULATOR_DEFAULT_ENABLED true
#define AFE_FUNCTIONALITY_REGULATOR_DEFAULT_ON 0
#define AFE_FUNCTIONALITY_REGULATOR_DEFAULT_OFF 0
#define AFE_FUNCTIONALITY_REGULATOR_DEFAULT_ON_INDICATOR false
#define AFE_FUNCTIONALITY_REGULATOR_DEFAULT_OFF_INDICATOR true
#define AFE_CONFIG_API_JSON_REGULATOR_DATA_LENGTH 18 // {"enabled":false}

#ifdef AFE_CONFIG_HARDWARE_DHT
#define AFE_FUNCTIONALITY_REGULATOR_CP_TEMPERATURE 0
#define AFE_FUNCTIONALITY_REGULATOR_CP_HEAT_INDEX 1
#define AFE_FUNCTIONALITY_REGULATOR_CP_HUMIDITY 2
#define AFE_FUNCTIONALITY_REGULATOR_CP_ABSOLOUTE_HUMIDITY 3
#define AFE_FUNCTIONALITY_REGULATOR_CP_DEW_POINT 4
#endif // AFE_CONFIG_HARDWARE_DHT

#endif // _AFE_Hardware_Regulator_h