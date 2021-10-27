#ifndef _AFE_Generic_Sensors_h
#define _AFE_Generic_Sensors_h

/* Temperature */
#ifdef AFE_CONFIG_TEMPERATURE
#define AFE_TEMPERATURE_UNIT_CELSIUS 1
#define AFE_TEMPERATURE_UNIT_FAHRENHEIT 2
#define AFE_TEMPERATURE_UNIT_KELVIN 3
#endif


/* Humidity */
#ifdef AFE_CONFIG_HUMIDITY
typedef enum {
  AFE_HUMIDITY_EXCELLENT = 1,
  AFE_HUMIDITY_GOOD = 2,
  AFE_HUMIDITY_FAIR = 3,
  AFE_HUMIDITY_POOR = 4,
  AFE_HUMIDITY_INADEQUATE = 5
} afe_humidity_ratings_t;

#define AFE_HUMIDITY_UNIT 1
#endif 

/* Pressure */
#ifdef AFE_CONFIG_PRESSURE
#define AFE_CONFIG_DEFAULT_SEA_LEVEL_PRESSURE 1013.25
#define AFE_PRESSURE_UNIT_HPA 1
#endif 

/* IAQ */
#ifdef AFE_CONFIG_HARDWARE_BMEX80
typedef enum {
  AFE_IAQ_GOOD = 1,
  AFE_IAQ_AVARAGE = 2,
  AFE_IAQ_LITTLE_BAD = 3,
  AFE_IAQ_BAD = 4,
  AFE_IAQ_WORSE = 5,
  AFE_IAQ_VERY_BAD = 5,
  AFE_IAQ_UNKNOWN = 255
} afe_iaq_ratings_t;

typedef enum {
  AFE_CO2_EXCELLENT = 1,
  AFE_CO2_GOOD = 2,
  AFE_CO2_FAIR = 3,
  AFE_CO2_POOR = 4,
  AFE_CO2_INADEQUATE = 5
} afe_co2_ratings_t;
#endif


#endif // _AFE_Generic_Sensors_h