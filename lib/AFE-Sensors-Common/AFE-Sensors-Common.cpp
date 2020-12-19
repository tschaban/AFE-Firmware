/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Sensors-Common.h"

AFESensorsCommon::AFESensorsCommon() {

#if defined(AFE_CONFIG_TEMPERATURE) && defined(AFE_CONFIG_HUMIDITY)
  // Set default comfort profile.

  // In computing these constants the following reference was used
  // http://epb.apogee.net/res/refcomf.asp
  // It was simplified as 4 straight lines and added very little skew on
  // the vertical lines (+0.1 on x for C,D)
  // The for points used are(from top left, clock wise)
  // A(30%, 30*C) B(70%, 26.2*C) C(70.1%, 20.55*C) D(30.1%, 22.22*C)
  // On the X axis we have the rel humidity in % and on the Y axis the
  // temperature in *C

  // Too hot line AB
  m_comfort.m_tooHot_m = -0.095;
  m_comfort.m_tooHot_b = 32.85;
  // Too humid line BC
  m_comfort.m_tooHumid_m = -56.5;
  m_comfort.m_tooHumid_b = 3981.2;
  // Too cold line DC
  m_comfort.m_tooCold_m = -0.04175;
  m_comfort.m_tooHCold_b = 23.476675;
  // Too dry line AD
  m_comfort.m_tooDry_m = -77.8;
  m_comfort.m_tooDry_b = 2364;
#endif // defined(AFE_CONFIG_TEMPERATURE) && defined(AFE_CONFIG_HUMIDITY)
};

#ifdef AFE_CONFIG_TEMPERATURE
float AFESensorsCommon::celsiusToFerenheit(float input) {
  return input * 1.8 + 32;
}

float AFESensorsCommon::ferenheitToCelsius(float input) {
  return (input - 32) / 1.8;
}
#endif // AFE_CONFIG_TEMPERATURE

#ifdef AFE_CONFIG_HUMIDITY
afe_humidity_ratings_t AFESensorsCommon::humidityRating(float humidity) {
  if (humidity >= 40 && humidity <= 60) {
    return AFE_HUMIDITY_EXCELLENT;
  } else if (humidity < 10 || humidity > 90) {
    return AFE_HUMIDITY_INADEQUATE;
  } else if (humidity < 20 || humidity > 80) {
    return AFE_HUMIDITY_POOR;
  } else if (humidity < 40 || humidity > 70) {
    return AFE_HUMIDITY_FAIR;
  } else {
    return AFE_HUMIDITY_GOOD;
  }
}
#endif // AFE_CONFIG_HUMIDITY

#if defined(AFE_CONFIG_TEMPERATURE) && defined(AFE_CONFIG_HUMIDITY)
float AFESensorsCommon::dewPoint(float temperature, float humidity,
                                 boolean fahrenheit) {
  float _temp = fahrenheit ? ferenheitToCelsius(temperature) : temperature;

  double A0 = 373.15 / (273.15 + (double)_temp);
  double SUM = -7.90298 * (A0 - 1);
  SUM += 5.02808 * log10(A0);
  SUM += -1.3816e-7 * (pow(10, (11.344 * (1 - 1 / A0))) - 1);
  SUM += 8.1328e-3 * (pow(10, (-3.49149 * (A0 - 1))) - 1);
  SUM += log10(1013.246);
  double VP = pow(10, SUM - 3) * (double)humidity;
  double Td = log(VP / 0.61078); // temp var
  Td = (241.88 * Td) / (17.558 - Td);
  return fahrenheit ? celsiusToFerenheit(Td) : Td;
}

float AFESensorsCommon::heatIndex(double temperature, double humidity,
                                  boolean fahrenheit) {
  /* Based on formula:
   * https://www.wpc.ncep.noaa.gov/html/heatindex_equation.shtml */

  double _temp = fahrenheit ? temperature : celsiusToFerenheit(temperature);

  double HI;

  HI = 0.5 * (_temp + 61.0 + ((_temp - 68.0) * 1.2) + (humidity * 0.094));

  if (HI >= 80) {
    // Full regression needed
    HI = -42.379 + 2.04901523 * _temp + 10.14333127 * humidity -
         0.22475541 * _temp * humidity - 0.00683783 * pow(_temp, 2) -
         0.05481717 * pow(humidity, 2) + 0.00122874 * pow(_temp, 2) * humidity +
         0.00085282 * _temp * pow(humidity, 2) -
         0.00000199 * pow(_temp, 2) * pow(humidity, 2);

    if (humidity < 13 && _temp >= 80 && _temp < 112) {
      HI -= ((13 - humidity) * 0.25) * sqrt((17 - abs(_temp - 95)) * 0.05882);
    } else if (humidity > 85 && _temp >= 80 && _temp <= 87) {
      HI += ((humidity - 85) * 0.1) * ((87 - _temp) * 0.2);
    }
  }

  return fahrenheit ? HI : ferenheitToCelsius(HI);
}

float AFESensorsCommon::absoluteHumidity(float temperature, float humidity,
                                         boolean fahrenheit) {
  // Calculate the absolute humidity in g/m³
  // https://carnotcycle.wordpress.com/2012/08/04/how-to-convert-relative-humidity-to-absolute-humidity/

  float _temperature =
      fahrenheit ? ferenheitToCelsius(temperature) : temperature;

  float absHumidity;
  float absTemperature;
  absTemperature = _temperature + 273.15;

  absHumidity = 6.112;
  absHumidity *= exp((17.67 * _temperature) / (243.5 + _temperature));
  absHumidity *= humidity;
  absHumidity *= 2.1674;
  absHumidity /= absTemperature;

  return absHumidity;
}

byte AFESensorsCommon::perception(float temperature, float humidity,
                                  boolean fahrenheit) {

  /* Computing human perception from dew point
           reference: https://en.wikipedia.org/wiki/Dew_point ==> Relationship
     to human comfort
           reference: Horstmeyer, Steve (2006-08-15). "Relative
     Humidity....Relative to What? The Dew Point Temperature...a better
     approach". Steve Horstmeyer, Meteorologist, WKRC TV, Cincinnati, Ohio, USA.
     Retrieved 2009-08-20.
           Using table
           Return value Dew point    Human perception[6]
              7         Over 26 °C   Severely high, even deadly for asthma
     related illnesses
              6         24–26 °C     Extremely uncomfortable, oppressive
              5         21–24 °C     Very humid, quite uncomfortable
              4         18–21 °C     Somewhat uncomfortable for most people at
     upper edge
              3         16–18 °C     OK for most, but all perceive the humidity
     at upper edge
              2         13–16 °C     Comfortable
              1         10–12 °C     Very comfortable
              0         Under 10 °C  A bit dry for some
  */

  float _temperature =
      fahrenheit ? ferenheitToCelsius(temperature) : temperature;
  float _dewPoint = dewPoint(_temperature, humidity);

  if (_dewPoint < 10.0f) {
    return Perception_Dry;
  } else if (_dewPoint < 13.0f) {
    return Perception_VeryComfy;
  } else if (_dewPoint < 16.0f) {
    return Perception_Comfy;
  } else if (_dewPoint < 18.0f) {
    return Perception_Ok;
  } else if (_dewPoint < 21.0f) {
    return Perception_UnComfy;
  } else if (_dewPoint < 24.0f) {
    return Perception_QuiteUnComfy;
  } else if (_dewPoint < 26.0f) {
    return Perception_VeryUnComfy;
  }
  // else dew >= 26.0
  return Perception_SevereUncomfy;
}

float AFESensorsCommon::comfort(ComfortState &destComfortStatus,
                                float temperature, float humidity,
                                boolean fahrenheit) {

  float ratio = 100; // 100%
  float distance = 0;
  float kTempFactor = 3;    // take into account the slope of the lines
  float kHumidFactor = 0.1; // take into account the slope of the lines
  uint8_t tempComfort = 0;

  float _temperature =
      fahrenheit ? ferenheitToCelsius(temperature) : temperature;

  destComfortStatus = Comfort_OK;

  distance = m_comfort.distanceTooHot(_temperature, humidity);
  if (distance > 0) {
    // update the comfort descriptor
    tempComfort += (uint8_t)Comfort_TooHot;
    // decrease the comfot ratio taking the distance into account
    ratio -= distance * kTempFactor;
  }

  distance = m_comfort.distanceTooHumid(_temperature, humidity);
  if (distance > 0) {
    // update the comfort descriptor
    tempComfort += (uint8_t)Comfort_TooHumid;
    // decrease the comfot ratio taking the distance into account
    ratio -= distance * kHumidFactor;
  }

  distance = m_comfort.distanceTooCold(_temperature, humidity);
  if (distance > 0) {
    // update the comfort descriptor
    tempComfort += (uint8_t)Comfort_TooCold;
    // decrease the comfot ratio taking the distance into account
    ratio -= distance * kTempFactor;
  }

  distance = m_comfort.distanceTooDry(_temperature, humidity);
  if (distance > 0) {
    // update the comfort descriptor
    tempComfort += (uint8_t)Comfort_TooDry;
    // decrease the comfot ratio taking the distance into account
    ratio -= distance * kHumidFactor;
  }

  destComfortStatus = (ComfortState)tempComfort;

  if (ratio < 0)
    ratio = 0;

  return ratio;
}

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
byte AFESensorsCommon::convertPerceptionDomoticz(byte perception) {
  if (perception == Perception_Dry) {
    return domoticzAlertYellow;
  } else if (perception == Perception_VeryComfy) {
    return domoticzAlertGreen;
  } else if (perception == Perception_Comfy) {
    return domoticzAlertGreen;
  } else if (perception == Perception_Ok) {
    return domoticzAlertYellow;
  } else if (perception == Perception_UnComfy) {
    return domoticzAlertYellow;
  } else if (perception == Perception_QuiteUnComfy) {
    return domoticzAlertOrange;
  } else if (perception == Perception_VeryUnComfy) {
    return domoticzAlertRed;
  } else {
    return domoticzAlertGray;
  }
}

byte AFESensorsCommon::convertComfortDomoticz(byte comfort) {
  if (comfort == Comfort_OK) {
    return domoticzAlertGreen;
  } else if (comfort == Comfort_TooHot) {
    return domoticzAlertYellow;
  } else if (comfort == Comfort_TooCold) {
    return domoticzAlertYellow;
  } else if (comfort == Comfort_TooDry) {
    return domoticzAlertYellow;
  } else if (comfort == Comfort_TooHumid) {
    return domoticzAlertYellow;
  } else if (comfort == Comfort_HotAndHumid) {
    return domoticzAlertRed;
  } else if (comfort == Comfort_HotAndDry) {
    return domoticzAlertRed;
  } else if (comfort == Comfort_ColdAndHumid) {
    return domoticzAlertRed;
  } else if (comfort == Comfort_ColdAndDry) {
    return domoticzAlertRed;
  } else {
    return domoticzAlertGray;
  }
}

byte AFESensorsCommon::convertHumidyStatusDomoticz(float humidity) {
  // https://www.airthings.com/resources/everything-you-need-to-know-about-humidity
  if (humidity >= 70) {
    return domoticzHumidityWet;
  } else if (humidity >= 60) {
    return domoticzHumidityNormal;
  } else if (humidity >= 30) {
    return domoticzHumidityComfortable;
  } else if (humidity >= 25) {
    return domoticzHumidityNormal;
  } else {
    return domoticzHumidityDry;
  }
}

#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
#endif // defined(AFE_CONFIG_TEMPERATURE) && defined(AFE_CONFIG_HUMIDITY)

#if defined(AFE_CONFIG_PRESSURE) && defined(AFE_CONFIG_TEMPERATURE)
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
#endif // defined(AFE_CONFIG_PRESSURE) && defined(AFE_CONFIG_TEMPERATURE)

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

#endif // FE_CONFIG_HARDWARE_BMEX80
