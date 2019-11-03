/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Sensors-Common.h"

AFESensorsCommon::AFESensorsCommon(){};

float AFESensorsCommon::celsiusToFerenheit(float input) {
  return input * 1.8 + 32;
}

float AFESensorsCommon::ferenheitToCelsius(float input) {
  return (input - 32)/1.8;
}

float AFESensorsCommon::dewPoint(float temperature, float humidity) {
  double a = 17.271;
  double b = 237.7;
  double T = (a * (double)temperature) / (b + (double)temperature) +
             log((double)humidity / 100);
  return (float)((b * T) / (a - T));
}

float AFESensorsCommon::relativePressure(float pressure, float alt,
                                         float temperature) {
  /* Based on the formula:
   * http://stacjepogody.waw.pl/index.php/topic,4.msg4.html#msg4 */
  return pressure +
         (alt /
          ((8000 *
            ((1 +
              0.004 *
                  ((temperature + (temperature + ((0.6 * alt) / 100))) / 2)) /
             ((pressure +
               (pressure +
                (alt / (8000 * ((1 + 0.004 * temperature) / pressure))))) /
              2)))));
}

float AFESensorsCommon::heatIndexF(double temperature, double humidity) {
  /* Based on formula: https://www.wpc.ncep.noaa.gov/html/heatindex_equation.shtml */

  double HI;

  HI = 0.5 *
       (temperature + 61.0 + ((temperature - 68.0) * 1.2) + (humidity * 0.094));

  if (HI >= 80) {
    // Full regression needed
    HI = -42.379 + 2.04901523 * temperature + 10.14333127 * humidity -
         0.22475541 * temperature * humidity -
         0.00683783 * temperature * temperature -
         0.05481717 * humidity * humidity +
         0.00122874 * temperature * temperature * humidity +
         0.00085282 * temperature * humidity * humidity -
         0.00000199 * temperature * temperature * humidity * humidity;

    if (humidity < 13 && temperature > 90 && temperature < 112) {
      HI -= ((13 - humidity) / 4) * sqrt((17 - abs(temperature - 95)) / 17);
    } else if (humidity >85 && temperature > 80 && temperature < 87) {
      HI +=  ((humidity-85)/10) * ((87-temperature)/5);
    }
  }

  return HI;

}

float AFESensorsCommon::heatIndexC(double temperature, double humidity) {
  return ferenheitToCelsius(heatIndexF(celsiusToFerenheit(temperature),humidity));
}

#ifdef AFE_CONFIG_HARDWARE_BMEX80
afe_iaq_ratings_t AFESensorsCommon::iaqRating(uint16_t iaq) {
  if (iaq <= 50) {
    return AFE_IAQ_GOOD;
  } else if (iaq <= 100) {
    return AFE_IAQ_AVARAGE;
  } else if (iaq <= 150) {
    return AFE_IAQ_LITTLE_BAD;
  } else if (iaq <= 200) {
    return AFE_IAQ_BAD;
  } else if (iaq <= 300) {
    return AFE_IAQ_WORSE;
  } else if (iaq <= 500) {
    return AFE_IAQ_VERY_BAD;
  } else {
    return AFE_IAQ_UNKNOWN;
  }
}

afe_co2_ratings_t AFESensorsCommon::co2Rating(uint16_t co2) {
  if (co2 > 1801) {
    return AFE_CO2_INADEQUATE;
  } else if (co2 > 1501) {
    return AFE_CO2_POOR;
  } else if (co2 > 801) {
    return AFE_CO2_FAIR;
  } else if (co2 > 601) {
    return AFE_CO2_GOOD;
  } else {
    return AFE_CO2_EXCELLENT;
  }
}

#endif

#ifdef AFE_CONFIG_HUMIDITY
afe_humidity_ratings_t AFESensorsCommon::humidityRating(float humidity) {
  if (humidity >= 40 && humidity <= 60) {
    return AFE_HUMIDITY_EXCELLENT;
  } else if (humidity < 10 || humidity > 90) {
    return AFE_HUMIDITY_INADEQUATE;
  } else if (humidity < 20 || humidity > 80) {
    return AFE_HUMIDITY_POOR;
  } else if (humidity < 30 || humidity > 70) {
    return AFE_HUMIDITY_FAIR;
  } else {
    return AFE_HUMIDITY_GOOD;
  }
}
#endif