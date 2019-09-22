/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Sensors-Common.h"

AFESensorsCommon::AFESensorsCommon(){};

float AFESensorsCommon::celsiusToFerenheit(float input) {
  return input * 1.8 + 32;
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