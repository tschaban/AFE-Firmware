/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-Sensor-BH1750.h"

#ifdef AFE_CONFIG_HARDWARE_BH1750

AFESensorBH1750::AFESensorBH1750(){};



#ifdef DEBUG
#ifdef AFE_ESP32
  void AFESensorBH1750::begin(uint8_t _id, TwoWire *WirePort0, TwoWire *WirePort1, AFEDebugger *Debugger) {
     _Debugger = Debugger;
     begin(_id,WirePort0,WirePort1);
  }
#else
  void AFESensorBH1750::begin(uint8_t _id, TwoWire *WirePort0, AFEDebugger *Debugger) {
     _Debugger = Debugger;
     begin(_id,WirePort0);
  }
#endif


#endif

#ifdef AFE_ESP32
void AFESensorBH1750::begin(uint8_t _id, TwoWire *WirePort0,
                            TwoWire *WirePort1)
{
  _WirePort1 = WirePort1;
  begin(_id, WirePort0);
}
#endif

void AFESensorBH1750::begin(uint8_t _id, TwoWire *WirePort0)
{
  AFEDataAccess Data;
  Data.getConfiguration(_id, configuration);

#ifdef AFE_ESP32
  /* Setting the WirePort used by the sensor to WirePort0 */
  _WirePort0 = configuration->wirePortId == 0 ? WirePort0 : _WirePort1;
#endif

#ifdef DEBUG
  _Debugger->printHeader(2, 1, 30, AFE_DEBUG_HEADER_TYPE_DASH);
  _Debugger->printBulletPoint(F("BH1750: Initializing"));
#endif
  if (
#ifdef AFE_ESP32
      configuration->wirePortId != AFE_HARDWARE_ITEM_NOT_EXIST &&
#endif
      configuration->i2cAddress != 0)
  {

#ifdef DEBUG
    _Debugger->printBulletPoint(F("Checking if the sensor is connected"));

#endif
    AFEI2CScanner I2CScanner;
#ifdef DEBUG
    I2CScanner.begin(_WirePort0, _Debugger);
#else
    I2CScanner.begin(_WirePort0);
#endif
    if (I2CScanner.scan(configuration->i2cAddress))
    {

#ifdef DEBUG
      _Debugger->printBulletPoint(F("Sensor address: 0x"));
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

          configuration->i2cAddress, _WirePort0);
      /*
            _initialized =
         bh1750->begin(BH1750::CONTINUOUS_HIGH_RES_MODE,
                                        configuration->i2cAddress, WirePort0);
      */
    }
#ifdef DEBUG
    else
    {
      _Debugger->printBulletPoint(F("Error: Device not found under I2C Address: 0x"));
      Serial
          << _HEX(configuration->i2cAddress);
    }
#endif
  }
#ifdef DEBUG
  else
  {
    _Debugger->printBulletPoint(F("Error: Address not set"));
  }
#endif

#ifdef DEBUG
  if (_initialized)
  {
    _Debugger->printBulletPoint(F("Name: "));
    _Debugger->printValue(configuration->name);
    _Debugger->printBulletPoint(F("Mode: "));
    _Debugger->printValue(configuration->mode);
    _Debugger->printBulletPoint(F("Interval: "));
    Serial << configuration->interval;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    _Debugger->printBulletPoint(F("IDX: "));
    _Debugger->printValue(configuration->domoticz.idx);
#endif
  }
  _Debugger->printBulletPoint(F("Device: "));
  _Debugger->printValue((_initialized ? F("Found") : F("Not found: check wiring")));
  _Debugger->printHeader(1, 1, 30, AFE_DEBUG_HEADER_TYPE_DASH);
#endif
}


boolean AFESensorBH1750::isReady()
{
  if (ready)
  {
    ready = false;
    return true;
  }
  else
  {
    return false;
  }
}

void AFESensorBH1750::listener()
{

  if (_initialized)
  {
    if (millis() - startTime >= configuration->interval * 1000)
    {

#if defined(DEBUG)

      _Debugger->printHeader(2, 1, 30, AFE_DEBUG_HEADER_TYPE_DASH);
      _Debugger->printBulletPoint(F("BH1750: Reading "));

      _Debugger->printBulletPoint(F("Interval: "));
      Serial << (millis() - startTime) / 1000 << F("s");
#endif
      while (!bh1750->measurementReady(true))
      {
        yield();
      }

      data = bh1750->readLightLevel();
      if (data >= 0)
      {
        ready = true;
      }

#if defined(DEBUG)
      _Debugger->printBulletPoint(F("Lux: "));
      _Debugger->printValue(data, F("lx"), 0);
#endif

      startTime = millis();
      if (configuration->mode >= BH1750::ONE_TIME_HIGH_RES_MODE)
      {

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

void AFESensorBH1750::getJSON(char *json)
{
  sprintf(json, (const char *)F("{\"illuminance\":{\"value\":%.2f,\"unit\":\"%s\"}}"), data,
          AFE_UNIT_LUX);
}

#endif // AFE_CONFIG_HARDWARE_BH1750