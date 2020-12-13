/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/

  --------------------------------------------------------------
  Few methods take and customized based on the DHTesp library
  Information about: DHTesp library
  DHT Temperature & Humidity Sensor library for Arduino & ESP32.
  https://github.com/beegee-tokyo/arduino-DHTesp
  Written by Mark Ruys, mark@paracas.nl.
  Updated to work with ESP32 by Bernd Giesecke, bernd@giesecke.tk
  GNU General Public License, check LICENSE for more information.
  All text above must be included in any redistribution.
 --------------------------------------------------------------

*/

#ifndef _AFE_Sensors_Common_h
#define _AFE_Sensors_Common_h

#include <AFE-Configuration.h>
#include <arduino.h>
#ifdef DEBUG
#include <Streaming.h>
#endif

#if defined(AFE_CONFIG_TEMPERATURE) && defined(AFE_CONFIG_HUMIDITY)
// Reference: http://epb.apogee.net/res/refcomf.asp (References invalid)
enum ComfortState {
  Comfort_OK = 0,
  Comfort_TooHot = 1,
  Comfort_TooCold = 2,
  Comfort_TooDry = 4,
  Comfort_TooHumid = 8,
  Comfort_HotAndHumid = 9,
  Comfort_HotAndDry = 5,
  Comfort_ColdAndHumid = 10,
  Comfort_ColdAndDry = 6
};

// References https://en.wikipedia.org/wiki/Dew_point ==> Relationship to human
// comfort
enum PerceptionState {
  Perception_Dry = 0,
  Perception_VeryComfy = 1,
  Perception_Comfy = 2,
  Perception_Ok = 3,
  Perception_UnComfy = 4,
  Perception_QuiteUnComfy = 5,
  Perception_VeryUnComfy = 6,
  Perception_SevereUncomfy = 7
};

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
enum DomoticzAlert {
  alertRed = 4,
  alertOrange = 3,
  alertYellow = 2,
  alertGreen = 1,
  alertGray = 0
};
#endif

struct ComfortProfile {
  // Represent the 4 line equations:
  // dry, humid, hot, cold, using the y = mx + b formula
  float m_tooHot_m, m_tooHot_b;
  float m_tooCold_m, m_tooHCold_b;
  float m_tooDry_m, m_tooDry_b;
  float m_tooHumid_m, m_tooHumid_b;

  inline bool isTooHot(float temp, float humidity) {
    return (temp > (humidity * m_tooHot_m + m_tooHot_b));
  }
  inline bool isTooHumid(float temp, float humidity) {
    return (temp > (humidity * m_tooHumid_m + m_tooHumid_b));
  }
  inline bool isTooCold(float temp, float humidity) {
    return (temp < (humidity * m_tooCold_m + m_tooHCold_b));
  }
  inline bool isTooDry(float temp, float humidity) {
    return (temp < (humidity * m_tooDry_m + m_tooDry_b));
  }

  inline float distanceTooHot(float temp, float humidity) {
    return temp - (humidity * m_tooHot_m + m_tooHot_b);
  }
  inline float distanceTooHumid(float temp, float humidity) {
    return temp - (humidity * m_tooHumid_m + m_tooHumid_b);
  }
  inline float distanceTooCold(float temp, float humidity) {
    return (humidity * m_tooCold_m + m_tooHCold_b) - temp;
  }
  inline float distanceTooDry(float temp, float humidity) {
    return (humidity * m_tooDry_m + m_tooDry_b) - temp;
  }
};

#endif // defined(AFE_CONFIG_TEMPERATURE) && defined(AFE_CONFIG_HUMIDITY)

class AFESensorsCommon {

private:
#if defined(AFE_CONFIG_TEMPERATURE) && defined(AFE_CONFIG_HUMIDITY)
  ComfortProfile m_comfort;
#endif // defined(AFE_CONFIG_TEMPERATURE) && defined(AFE_CONFIG_HUMIDITY)

public:
  /* Constructor */
  AFESensorsCommon();

#ifdef AFE_CONFIG_TEMPERATURE
  float celsiusToFerenheit(float input);
  float ferenheitToCelsius(float input);
#endif

#ifdef AFE_CONFIG_HUMIDITY
  afe_humidity_ratings_t humidityRating(float humidity);
#endif

#if defined(AFE_CONFIG_TEMPERATURE) && defined(AFE_CONFIG_HUMIDITY)
  float dewPoint(float temperature, float humidity, boolean fahrenheit = false);
  float heatIndex(double temperature, double humidity,
                  boolean fahrenheit = false);
  float absoluteHumidity(float temperature, float humidity,
                         boolean fahrenheit = false);
  byte perception(float temperature, float humidity,
                  boolean fahrenheit = false);

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  byte convertPerceptionDomoticz(byte perception);
  byte convertComfortDomoticz(byte comfort);
#endif

  float comfort(ComfortState &destComfortStatus, float temperature,
                float humidity, boolean fahrenheit = false);
  ComfortProfile getComfortProfile() { return m_comfort; }
  void setComfortProfile(ComfortProfile &c) { m_comfort = c; }
  inline bool isTooHot(float temperature, float humidity) {
    return m_comfort.isTooHot(temperature, humidity);
  }
  inline bool isTooHumid(float temperature, float humidity) {
    return m_comfort.isTooHumid(temperature, humidity);
  }
  inline bool isTooCold(float temperature, float humidity) {
    return m_comfort.isTooCold(temperature, humidity);
  }
  inline bool isTooDry(float temperature, float humidity) {
    return m_comfort.isTooDry(temperature, humidity);
  }

#endif // defined(AFE_CONFIG_TEMPERATURE) && defined(AFE_CONFIG_HUMIDITY)

#if defined(AFE_CONFIG_PRESSURE) && defined(AFE_CONFIG_TEMPERATURE)
  float relativePressure(float pressure, float alt, float temperature);
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  afe_iaq_ratings_t iaqRating(uint16_t iaq);
  afe_co2_ratings_t co2Rating(uint16_t co2);
#endif
};

#endif
