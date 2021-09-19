/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-Sensor-TLS2561.h"

#ifdef AFE_CONFIG_HARDWARE_TLS2561

AFESensorTLS2561::AFESensorTLS2561(){};

#ifdef AFE_ESP32
void AFESensorTLS2561::begin(uint8_t _id, TwoWire *WirePort0,
                             TwoWire *WirePort1) {
  _WirePort1 = WirePort1;
  begin(_id, WirePort0);
}

#endif

void AFESensorTLS2561::begin(uint8_t _id, TwoWire *WirePort0) {
  AFEDataAccess Data;
  Data.getConfiguration(_id, &configuration);

#ifdef AFE_ESP32
  /* Setting the WirePort used by the sensor to WirePort0 */
  _WirePort0 = configuration.wirePortId == 0 ? WirePort0 : _WirePort1;
#endif

#ifdef DEBUG
  Serial << endl << endl << F("----- TLS2561: Initializing -----");
#endif
  if (
#ifdef AFE_ESP32
      configuration.wirePortId != AFE_HARDWARE_ITEM_NOT_EXIST &&
#endif
      configuration.i2cAddress != 0) {

#ifdef DEBUG
    Serial << endl << F(": Checking if the sensor is connected");
#endif
    AFEI2CScanner I2CScanner;
    I2CScanner.begin(_WirePort0);

    if (I2CScanner.scan(configuration.i2cAddress)) {

#ifdef DEBUG
      Serial << endl
             << F(": Sensor address: 0x") << _HEX(configuration.i2cAddress);
#endif

      /* Configuring the sensor: setting gain */
      switch (configuration.gain) {
      case TSL2561_GAIN_1X:
        tls2561.setGain(TSL2561_GAIN_1X); /* No gain ... use in bright light to
                                             avoid sensor saturation */
        break;
      case TSL2561_GAIN_16X:
        tls2561.setGain(TSL2561_GAIN_16X); /* 16x gain ... use in low light to
                                              boost sensitivity */
        break;
      default:
        tls2561.enableAutoRange(
            true); /* Auto-gain ... switches automatically between 1x and 16x */
      }

      /* Configuring the sensor: sensitiveness */
      switch (configuration.sensitiveness) {
      case TSL2561_INTEGRATIONTIME_101MS:
        tls2561.setIntegrationTime(
            TSL2561_INTEGRATIONTIME_101MS); /* medium resolution and speed   */
        break;
      case TSL2561_INTEGRATIONTIME_402MS:
        tls2561.setIntegrationTime(
            TSL2561_INTEGRATIONTIME_402MS); /* 16-bit data but slowest
                                               conversions */
        break;
      default:
        tls2561.setIntegrationTime(
            TSL2561_INTEGRATIONTIME_13MS); /* fast but low resolution */
      }

      _initialized = tls2561.begin(configuration.i2cAddress, _WirePort0);

#ifdef DEBUG
    } else {
      Serial << endl
             << F(": Error: Device not found under I2C Address: 0x")
             << _HEX(configuration.i2cAddress);
#endif
    }

#ifdef DEBUG
  } else {
    Serial << endl << F("Error: Address not set");
#endif
  }
#ifdef DEBUG
  if (_initialized) {

    sensor_t sensor;
    tls2561.getSensor(&sensor);
    Serial << endl << F(": Sensor: ") << sensor.name;
    Serial << endl << F(": Driver Ver: ") << sensor.version;
    Serial << endl << F(": Unique ID: ") << sensor.sensor_id;
    Serial << endl << F(": Max Value: ") << sensor.max_value << F("lux");
    Serial << endl << F(": Min Value: ") << sensor.min_value << F("lux");
    Serial << endl << F(": Resolution: ") << sensor.resolution << F("lux");
    Serial << endl << F(": Name: ") << configuration.name;
    Serial << endl << F(": Sensitiveness: ") << configuration.sensitiveness;
    Serial << endl << F(": Gain: ") << configuration.gain;
    Serial << endl << F(": Interval: ") << configuration.interval;
  }
  Serial << endl
         << F(": Device: ")
         << (_initialized ? F("Found") : F("Not found: check wiring"));
  Serial << endl << F("---------------------------------");
#endif
}

boolean AFESensorTLS2561::listener() {
  ready = false;
  if (_initialized) {
    if (millis() - startTime >= configuration.interval * 1000) {

#ifdef DEBUG
      Serial << endl << endl << F("----- TLS2561: Reading -----");
      Serial << endl << F("Time: ") << (millis() - startTime) / 1000 << F("s");
#endif

      if (tls2561.init()) {
        tls2561.getLuminosity(&broadband, &ir);
        illuminance = tls2561.calculateLux(broadband, ir);
#ifdef DEBUG
        Serial << endl
               << F("Lux: ") << illuminance << F(" lux") << endl
               << F("Broadband: ") << broadband << endl
               << F("IR: ") << ir;
#endif

        ready = true;
#ifdef DEBUG
      } else {
        Serial << endl << F("ERROR: TLS2561: Sensor not found and initialized");
#endif
      }

#ifdef DEBUG
      Serial << Serial << endl << F("---------------------------");
#endif

      startTime = millis();
    }
  }
  return ready;
}

void AFESensorTLS2561::getJSON(char *json) {
  sprintf(json, "{\"illuminance\":{\"value\":%d,\"unit\":\"lux\"},"
                "\"broadband\":{\"value\":%d,\"unit\":\"?\"},\"IR\":{\"value\":"
                "%d,\"unit\":\"?\"}}",
          illuminance, broadband, ir);
}

#endif // AFE_CONFIG_HARDWARE_TLS2561