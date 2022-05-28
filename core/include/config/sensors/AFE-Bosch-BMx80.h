#ifndef _AFE_Hardware_Bosch_BMx80_h
#define _AFE_Hardware_Bosch_BMx80_h

#ifndef AFE_CONFIG_TEMPERATURE
#define AFE_CONFIG_TEMPERATURE
#endif
#ifndef AFE_CONFIG_HUMIDITY
#define AFE_CONFIG_HUMIDITY
#endif
#ifndef AFE_CONFIG_PRESSURE
#define AFE_CONFIG_PRESSURE
#endif
#ifndef AFE_CONFIG_DOMOTICZ_CUSTOME_SENSOR
#define AFE_CONFIG_DOMOTICZ_CUSTOME_SENSOR
#endif

#define AFE_CONFIG_HARDWARE_BMEX80_DEFAULT_INTERVAL 60
#define AFE_CONFIG_API_JSON_BMEX80_DATA_LENGTH 1195 // {"temperature":{"value":25.0273,"unit":"C","correction":25.0273},"pressure":{"value":993.6063,"unit":"hPa","correction":993.6063},"relativePressure":{"value":1003.809,"unit":"hPa"},"dewPoint":{"value":5.321408,"unit":"C"},"humidity":{"value":281.10078,"unit":"%H","correction":281.10078,"rating":3},"absoluteHumidity":{"value":6.480335,"unit":"%H"},"heatIndex":{"value":24.31934,"unit":"C"},"perception":{"value":0,"description":"W
       // porządku dla większości, ale wszyscy odczuwają wilgoć przy górnej
       // krawędzi"},"comfort":{"value":4,"ratio":84.7268,"unit":"%","description":"Gorąco
       // i
       // wilgotno"},"iaq":{"value":1110,"rating":1,"accuracy":0},"staticIaq":{"value":1110,"rating":1,"accuracy":0},"co2Equivalent":{"value":1400,"unit":"ppm","rating":1,"accuracy":0},"breathVocEquivalent":{"value":0.342646,"unit":"?","accuracy":0},"gasResistance":{"value":2324.371,"unit":"kOm"}}
#define AFE_BMX_UNKNOWN_SENSOR 255
#define AFE_BMP180_SENSOR 1
#define AFE_BME280_SENSOR 2
#define AFE_BMP280_SENSOR 3
#define AFE_BME680_SENSOR 6

#endif // _AFE_Hardware_Bosch_BMx80_h