/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-Sensor-BH1750.h"

#ifdef AFE_CONFIG_HARDWARE_BH1750

#ifdef DEBUG
AFESensorBH1750::AFESensorBH1750(AFEDataAccess *_Data,
                                 AFEWireContainer *_WirePort,
                                 AFEDebugger *_Debugger) {
  Data = _Data;
  WirePort = _WirePort;
  Debugger = _Debugger;
}
#else
AFESensorBH1750::AFESensorBH1750(AFEDataAccess *_Data,
                                 AFEWireContainer *_WirePort) {
  Data = _Data;
  WirePort = _WirePort;
};
#endif

void AFESensorBH1750::begin(uint8_t _id) {

  Data->getConfiguration(_id, configuration);

#ifdef DEBUG
  Debugger->printHeader(2, 1, 30, AFE_DEBUG_HEADER_TYPE_DASH);
  Debugger->printBulletPoint(F("BH1750: Initializing"));
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

#ifdef DEBUG
      Debugger->printBulletPoint(F("Sensor address: 0x"));
      Serial << _HEX(configuration->i2cAddress);
#endif

      _initialized = bh1750->begin(
          configuration->mode == BH1750::ONE_TIME_HIGH_RES_MODE
              ? BH1750::ONE_TIME_HIGH_RES_MODE
              : configuration->mode == BH1750::ONE_TIME_HIGH_RES_MODE_2
                    ? BH1750::ONE_TIME_HIGH_RES_MODE_2
                    : configuration->mode == BH1750::ONE_TIME_LOW_RES_MODE
                          ? BH1750::ONE_TIME_LOW_RES_MODE
                          : configuration->mode ==
                                    BH1750::CONTINUOUS_HIGH_RES_MODE
                                ? BH1750::CONTINUOUS_HIGH_RES_MODE
                                : configuration->mode ==
                                          BH1750::CONTINUOUS_HIGH_RES_MODE_2
                                      ? BH1750::CONTINUOUS_HIGH_RES_MODE_2
                                      : BH1750::CONTINUOUS_LOW_RES_MODE,
          configuration->i2cAddress,
#ifdef AFE_ESP32
          (configuration->wirePortId == 0 ? WirePort->Port0 : WirePort->Port1)
#else
          WirePort->Port0
#endif
              );

    }
#ifdef DEBUG
    else {
      Debugger->printBulletPoint(
          F("Error: Device not found under I2C Address: 0x"));
      Serial << _HEX(configuration->i2cAddress);
    }
#endif
  }
#ifdef DEBUG
  else {
    Debugger->printBulletPoint(F("Error: Address not set"));
  }
#endif

#ifdef DEBUG
  if (_initialized) {
    Debugger->printBulletPoint(F("Name: "));
    Debugger->printValue(configuration->name);
    Debugger->printBulletPoint(F("Mode: "));
    Debugger->printValue(configuration->mode);
    Debugger->printBulletPoint(F("Interval: "));
    Serial << configuration->interval;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    Debugger->printBulletPoint(F("IDX: "));
    Debugger->printValue(configuration->domoticz.idx);
#endif
  }
  Debugger->printBulletPoint(F("Device: "));
  Debugger->printValue(
      (_initialized ? F("Found") : F("Not found: check wiring")));
  Debugger->printHeader(1, 1, 30, AFE_DEBUG_HEADER_TYPE_DASH);
#endif

  delete Data;
  Data = NULL;
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
    if (millis() - startTime >= configuration->interval * 1000) {

#if defined(DEBUG)

      Debugger->printHeader(2, 1, 30, AFE_DEBUG_HEADER_TYPE_DASH);
      Debugger->printBulletPoint(F("BH1750: Reading "));

      Debugger->printBulletPoint(F("Interval: "));
      Serial << (millis() - startTime) / 1000 << F("s");
#endif
      while (!bh1750->measurementReady(true)) {
        yield();
      }

      data = bh1750->readLightLevel();
      if (data >= 0) {
        ready = true;
      }

#if defined(DEBUG)
      Debugger->printBulletPoint(F("Lux: "));
      Debugger->printValue(data, F("lx"), 0);
#endif

      startTime = millis();
      if (configuration->mode >= BH1750::ONE_TIME_HIGH_RES_MODE) {

        bh1750->configure(configuration->mode == BH1750::ONE_TIME_HIGH_RES_MODE
                              ? BH1750::ONE_TIME_HIGH_RES_MODE
                              : configuration->mode ==
                                        BH1750::ONE_TIME_HIGH_RES_MODE_2
                                    ? BH1750::ONE_TIME_HIGH_RES_MODE_2
                                    : BH1750::ONE_TIME_LOW_RES_MODE);
      }
    }
  }
}

void AFESensorBH1750::getJSON(char *json) {
  sprintf(json,
          (const char *)F("{\"illuminance\":{\"value\":%.2f,\"unit\":\"%s\"}}"),
          data, AFE_UNIT_LUX);
}

#endif // AFE_CONFIG_HARDWARE_BH1750