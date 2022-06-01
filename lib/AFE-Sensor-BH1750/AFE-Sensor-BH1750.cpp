/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-Sensor-BH1750.h"

#ifdef AFE_CONFIG_HARDWARE_BH1750

AFESensorBH1750::AFESensorBH1750(){};

#ifdef AFE_ESP32
void AFESensorBH1750::begin(uint8_t _id, TwoWire *WirePort0,
                            TwoWire *WirePort1) {
  _WirePort1 = WirePort1;
  begin(_id, WirePort0);
}
#endif

void AFESensorBH1750::begin(uint8_t _id, TwoWire *WirePort0) {
  AFEDataAccess Data;
  Data.getConfiguration(_id, &configuration);

#ifdef AFE_ESP32
  /* Setting the WirePort used by the sensor to WirePort0 */
  _WirePort0 = configuration.wirePortId == 0 ? WirePort0 : _WirePort1;
#endif

#ifdef DEBUG
  Serial << endl << endl << F("----- BH1750: Initializing -----");
#endif
  if (
#ifdef AFE_ESP32
      configuration.wirePortId != AFE_HARDWARE_ITEM_NOT_EXIST &&
#endif
      configuration.i2cAddress != 0) {

#ifdef DEBUG
    Serial << endl << F("Checking if the sensor is connected");
#endif
    AFEI2CScanner I2CScanner;
    I2CScanner.begin(_WirePort0);

    if (I2CScanner.scan(configuration.i2cAddress)) {

#ifdef DEBUG
      Serial << endl
             << F("Sensor address: 0x") << _HEX(configuration.i2cAddress);
#endif

      _initialized = bh1750.begin(
          configuration.mode == BH1750::ONE_TIME_HIGH_RES_MODE
              ? BH1750::ONE_TIME_HIGH_RES_MODE
              : configuration.mode == BH1750::ONE_TIME_HIGH_RES_MODE_2
                    ? BH1750::ONE_TIME_HIGH_RES_MODE_2
                    : configuration.mode == BH1750::ONE_TIME_LOW_RES_MODE
                          ? BH1750::ONE_TIME_LOW_RES_MODE
                          : configuration.mode ==
                                    BH1750::CONTINUOUS_HIGH_RES_MODE
                                ? BH1750::CONTINUOUS_HIGH_RES_MODE
                                : configuration.mode ==
                                          BH1750::CONTINUOUS_HIGH_RES_MODE_2
                                      ? BH1750::CONTINUOUS_HIGH_RES_MODE_2
                                      : BH1750::CONTINUOUS_LOW_RES_MODE,

          configuration.i2cAddress, _WirePort0);
      /*
            _initialized =
         bh1750.begin(BH1750::CONTINUOUS_HIGH_RES_MODE,
                                        configuration.i2cAddress, WirePort0);
      */
    }
#ifdef DEBUG
    else {
      Serial << endl
             << F("Error: Device not found under I2C Address: 0x")
             << _HEX(configuration.i2cAddress);
    }
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("Error: Address not set");
  }
#endif

#ifdef DEBUG
  if (_initialized) {
    Serial << endl << F("Name: ") << configuration.name;
    Serial << endl << F("Mode: ") << configuration.mode;
    Serial << endl << F("Interval: ") << configuration.interval;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    Serial << endl << F("IDX: ") << configuration.domoticz.idx;
#endif
  }
  Serial << endl
         << F("Device: ")
         << (_initialized ? F("Found") : F("Not found: check wiring"));
  Serial << endl << F("---------------------------------");
#endif
}

boolean AFESensorBH1750::isReady() {
  if (ready) {
    ready = false;
    return true;
  } else {
    return false;
  }
}

void AFESensorBH1750::listener() {

  if (_initialized) {
    if (millis() - startTime >= configuration.interval * 1000) {

#if defined(DEBUG)
      Serial << endl << endl << F("----- BH1750: Reading -----");
      Serial << endl << F("Time: ") << (millis() - startTime) / 1000 << F("s");
#endif
      while (!bh1750.measurementReady(true)) {
        yield();
      }

      data = bh1750.readLightLevel();
      if (data >= 0) {
        ready = true;
      }

#if defined(DEBUG)
      Serial << endl << F("Lux: ") << data << F("lx");
      Serial << endl << F("---------------------------");
#endif

      startTime = millis();
      if (configuration.mode >= BH1750::ONE_TIME_HIGH_RES_MODE) {

        bh1750.configure(configuration.mode == BH1750::ONE_TIME_HIGH_RES_MODE
                             ? BH1750::ONE_TIME_HIGH_RES_MODE
                             : configuration.mode ==
                                       BH1750::ONE_TIME_HIGH_RES_MODE_2
                                   ? BH1750::ONE_TIME_HIGH_RES_MODE_2
                                   : BH1750::ONE_TIME_LOW_RES_MODE);
      }
    }
  }
}

void AFESensorBH1750::getJSON(char *json) {
  sprintf(json, "{\"illuminance\":{\"value\":%.2f,\"unit\":\"%s\"}}", data,
          AFE_UNIT_LUX);
}

#endif // AFE_CONFIG_HARDWARE_BH1750