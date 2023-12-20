/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-Sensor-TSL2561.h"

#ifdef AFE_CONFIG_HARDWARE_TSL2561

#ifdef DEBUG
AFESensorTSL2561::AFESensorTSL2561(AFEDataAccess *_Data,
                                   AFEWireContainer *_WirePort,
                                   AFEDebugger *_Debugger) {
  Data = _Data;
  WirePort = _WirePort;
  Debugger = _Debugger;
};
#else
AFESensorTSL2561::AFESensorTSL2561(AFEDataAccess *_Data,
                                   AFEWireContainer *_WirePort) {
  Data = _Data;
  WirePort = _WirePort;
};
#endif

void AFESensorTSL2561::begin(uint8_t _id) {
  Data->getConfiguration(_id, configuration);
  delete Data;
  Data = NULL;

#ifdef DEBUG
  Debugger->printHeader(2, 1, 50, AFE_DEBUG_HEADER_TYPE_DASH);
  Debugger->printBulletPoint(F("TSL2561: Initializing"));
  Debugger->printBulletPoint(F("Name: "));
  Debugger->printValue(configuration->name);
  Debugger->printBulletPoint(F("Interval: "));
  Serial << configuration->interval;
  Debugger->printBulletPoint(F("Sensitiveness: "));
  Debugger->printValue(configuration->sensitiveness);
  Debugger->printBulletPoint(F("Gain: "));
  Debugger->printValue(configuration->gain);
  Debugger->printBulletPoint(F("I2C Address: 0x"));
  Serial << _HEX(configuration->i2cAddress);
#endif
  if (
#ifdef AFE_ESP32
      configuration->wirePortId != AFE_HARDWARE_ITEM_NOT_EXIST &&
#endif
      configuration->i2cAddress != 0) {

#ifdef DEBUG
    Debugger->printBulletPoint(F("Checking if the sensor is connected"));
#endif

#ifdef AFE_ESP32
    WirePort->Scanner->setWire(
        (configuration->wirePortId == 0 ? WirePort->Port0 : WirePort->Port1));
#else
    WirePort->Scanner->setWire(WirePort->Port0);
#endif

    if (WirePort->Scanner->scan(configuration->i2cAddress)) {

#ifdef AFE_ESP32
      _initialized = tls2561->begin(
          configuration->i2cAddress,
          (configuration->wirePortId == 0 ? WirePort->Port0 : WirePort->Port1));
#else
      _initialized = tls2561->begin(configuration->i2cAddress, WirePort->Port0);
#endif

      /* Configuring the sensor: setting gain */
      switch (configuration->gain) {
      case TSL2561_GAIN_1X:
        tls2561->setGain(TSL2561_GAIN_1X); /* No gain ... use in bright light to
                                             avoid sensor saturation */
        break;
      case TSL2561_GAIN_16X:
        tls2561->setGain(TSL2561_GAIN_16X); /* 16x gain ... use in low light to
                                              boost sensitivity */
        break;
      default:
        tls2561->enableAutoRange(
            true); /* Auto-gain ... switches automatically between 1x and 16x */
      }

      /* Configuring the sensor: sensitiveness */
      switch (configuration->sensitiveness) {
      case TSL2561_INTEGRATIONTIME_101MS:
        tls2561->setIntegrationTime(
            TSL2561_INTEGRATIONTIME_101MS); /* medium resolution and speed   */
        break;
      case TSL2561_INTEGRATIONTIME_402MS:
        tls2561->setIntegrationTime(
            TSL2561_INTEGRATIONTIME_402MS); /* 16-bit data but slowest
                                               conversions */
        break;
      default:
        tls2561->setIntegrationTime(
            TSL2561_INTEGRATIONTIME_13MS); /* fast but low resolution */
      }

#ifdef DEBUG
    } else {
      Debugger->printBulletPoint(
          F("Error: Device not found under I2C Address: 0x: "));
      Serial << _HEX(configuration->i2cAddress);
#endif
    }

#ifdef DEBUG
  } else {
    Debugger->printBulletPoint(F("Error: Address not set"));
#endif
  }

#ifdef DEBUG
  if (_initialized) {

    sensor_t sensor;
    tls2561->getSensor(&sensor);

    Debugger->printBulletPoint(F("Sensor: "));
    Debugger->printValue(sensor.name);
    Debugger->printBulletPoint(F("Driver Ver: "));
    Serial << sensor.version;
    Debugger->printBulletPoint(F("ID: "));
    Serial << sensor.sensor_id;
    Debugger->printBulletPoint(F("Max Value [Lux]: "));
    Debugger->printValue(sensor.max_value);
    Debugger->printBulletPoint(F("Min Value [Lux]: "));
    Debugger->printValue(sensor.min_value);
    Debugger->printBulletPoint(F("Resolution: "));
    Debugger->printValue(sensor.resolution);
    Debugger->printBulletPoint(F("Device: "));
    Debugger->printValue(_initialized ? F("Found")
                                      : F("Not found: check wiring"));
    Debugger->printHeader(1, 2, 50, AFE_DEBUG_HEADER_TYPE_DASH);
  }
#endif
}

boolean AFESensorTSL2561::listener() {
  ready = false;
  if (_initialized) {
    if (millis() - startTime >= configuration->interval * 1000) {

#ifdef DEBUG
      Debugger->printHeader(2, 1, 30, AFE_DEBUG_HEADER_TYPE_DASH);
      Debugger->printBulletPoint(F("TSL2561: Reading"));
      Debugger->printBulletPoint(F("Interval: "));
      Serial << (millis() - startTime) / 1000 << F("s");
#endif

      if (tls2561->init()) {
        tls2561->getLuminosity(&broadband, &ir);
        illuminance = tls2561->calculateLux(broadband, ir);
#ifdef DEBUG
        Debugger->printBulletPoint(F("Lux: "));
        Serial << illuminance;
        Debugger->printBulletPoint(F("Broadband: "));
        Serial << broadband;
        Debugger->printBulletPoint(F("IR: "));
        Serial << ir;
#endif
        ready = true;
#ifdef DEBUG
      } else {
        Debugger->printBulletPoint(
            F("ERROR: TSL2561: Sensor not found and initialized"));
#endif
      }

#ifdef DEBUG
      Debugger->printHeader(1, 2, 30, AFE_DEBUG_HEADER_TYPE_DASH);
#endif

      startTime = millis();
    }
  }
  return ready;
}

void AFESensorTSL2561::getJSON(char *json) {
  sprintf(json,
          (const char *)F(
              "{\"illuminance\":{\"value\":%d,\"unit\":\"%s\"},"
              "\"broadband\":{\"value\":%d,\"unit\":\"?\"},\"IR\":{\"value\":"
              "%d,\"unit\":\"?\"}}"),
          illuminance, AFE_UNIT_LUX, broadband, ir);
}

#endif // AFE_CONFIG_HARDWARE_TSL2561