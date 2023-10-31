/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-Sensor-BME680.h"
#ifdef AFE_CONFIG_HARDWARE_BMEX80

AFESensorBME680::AFESensorBME680(){};

#ifdef DEBUG
boolean AFESensorBME680::begin(BMEX80 *_configuration, TwoWire *WirePort, AFEDebugger *_Debugger, BMEX80_DATA *_data)
{
  Debugger = _Debugger;
  return begin(_configuration, WirePort, Debugger, _data);
}
#endif

boolean AFESensorBME680::begin(BMEX80 *_configuration, TwoWire *WirePort, BMEX80_DATA *_data)
{
#ifdef DEBUG
  Debugger->printBulletPoint(F("Type: BME680"));
#endif

  data = _data;

  EEPROM.begin(BSEC_MAX_STATE_BLOB_SIZE + 1); // 1st address for the length
#ifdef DEBUG
  Debugger->printBulletPoint(F("EEPROM initialized: Size reserved: "));
  Debugger->printValue((uint8_t)(BSEC_MAX_STATE_BLOB_SIZE + 1));
#endif

  configuration = _configuration;
  if (configuration->i2cAddress != 0)
  {

#ifdef DEBUG
    Debugger->printValue((uint8_t)(BSEC_MAX_STATE_BLOB_SIZE + 1));
    Serial << _HEX(configuration->i2cAddress);
#endif
    Bme->begin(configuration->i2cAddress, *WirePort);

#ifdef DEBUG
    Debugger->printBulletPoint(F("Bosch BSEC library version: "));
    Serial << Bme->version.major
           << F(".") << Bme->version.minor << F(".")
           << Bme->version.major_bugfix << F(".") << Bme->version.minor_bugfix;

    checkBmeStatus();
    Debugger->printBulletPoint(F("Sensor: configuring"));
#endif

    Bme->setConfig(bsec_config_iaq);
#ifdef DEBUG
    checkBmeStatus();
#endif

    loadState();
#ifdef DEBUG
    Debugger->printBulletPoint(F("Stated loaded from the EEPROM"));
    Debugger->printBulletPoint(F("SUpdating subscription"));
#endif

    bsec_virtual_sensor_t sensorList[10] = {
        BSEC_OUTPUT_RAW_TEMPERATURE,
        BSEC_OUTPUT_RAW_PRESSURE,
        BSEC_OUTPUT_RAW_HUMIDITY,
        BSEC_OUTPUT_RAW_GAS,
        BSEC_OUTPUT_IAQ,
        BSEC_OUTPUT_STATIC_IAQ,
        BSEC_OUTPUT_CO2_EQUIVALENT,
        BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
        BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
        BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY,
    };

    Bme->updateSubscription(sensorList, 10, BSEC_SAMPLE_RATE_LP);
#ifdef DEBUG
    checkBmeStatus();
#endif
  }

  return true;
}

boolean AFESensorBME680::read()
{
  boolean _ret = false;

#ifdef DEBUG
      Debugger->printBulletPoint(F("Sensor: BME680"));
#endif


  if (Bme->run())
  {
    sensorOutputs = Bme->getOutputs();

    if (sensorOutputs != nullptr)
    {

      for (uint8_t i = 0; i < sensorOutputs->nOutputs; i++)
      {
        const bsecData output = sensorOutputs->output[i];
        switch (output.sensor_id)
        {
        case BSEC_OUTPUT_IAQ:
          data->iaq.value = output.signal;
          data->iaq.accuracy = output.accuracy;
          break;
        case BSEC_OUTPUT_RAW_TEMPERATURE:
          data->temperature.value = output.signal;
          break;
        case BSEC_OUTPUT_RAW_PRESSURE:
          data->pressure.value = output.signal;
          break;
        case BSEC_OUTPUT_RAW_HUMIDITY:
          data->humidity.value = output.signal;
          break;
        case BSEC_OUTPUT_RAW_GAS:
          data->gasResistance.value = output.signal;
          break;
        case BSEC_OUTPUT_STATIC_IAQ:
          data->staticIaq.value = output.signal;
          data->staticIaq.accuracy = output.accuracy;
          break;
        case BSEC_OUTPUT_CO2_EQUIVALENT:
          data->co2Equivalent.value = output.signal;
          data->co2Equivalent.accuracy = output.accuracy;
          break;
        case BSEC_OUTPUT_BREATH_VOC_EQUIVALENT:
          data->breathVocEquivalent.value = output.signal;
          data->breathVocEquivalent.accuracy = output.accuracy;
          break;

        case BSEC_OUTPUT_STABILIZATION_STATUS:
          // Serial.println("\tstabilization status = " + String(output.signal));
          break;
        case BSEC_OUTPUT_RUN_IN_STATUS:
          // Serial.println("\trun in status = " + String(output.signal));
          break;
        default:
          break;
        }

        /* @TODO T6 needs to be rebuilt based on the example
        data->temperature.value = Bme->temperature;
        data->pressure.value = Bme->pressure / 100;
        data->humidity.value = Bme->humidity;
        data->gasResistance.value = Bme->gasResistance / 1000;
        data->iaq.value = Bme->iaq;
        data->iaq.accuracy = Bme->iaqAccuracy;
        data->staticIaq.value = Bme->staticIaq;
        data->staticIaq.accuracy = Bme->staticIaqAccuracy;
        data->co2Equivalent.value = Bme->co2Equivalent;
        data->co2Equivalent.accuracy = Bme->co2Accuracy;
        data->breathVocEquivalent.value = Bme->breathVocEquivalent;
        data->breathVocEquivalent.accuracy = Bme->breathVocAccuracy;
        */
      }
      updateState();
      _ret = true;
    }
  }

#ifdef DEBUG
  checkBmeStatus();
#endif

  return _ret;
}

#ifdef DEBUG
void AFESensorBME680::checkBmeStatus()
{

  if (Bme->status != BSEC_OK)
  {
    if (Bme->status < BSEC_OK)
    {

      Debugger->printBulletPoint(F("ERROR: BSEC error code: "));
      Serial << Bme->status;
    }
    else
    {
      Debugger->printBulletPoint(F("WARN: BSEC warning code: "));
      Serial << Bme->status;
    }
  }
  else
  {
    Debugger->printBulletPoint(F("Sensor status: OK"));
  }

  /* @TODO T6

    if (Bme->bme68xStatus != BME68X_OK) {
      if (Bme->bme68xStatus < BME68X_OK) {
      // @TODO  Serial << endl             << F("ERROR: Bosch: BME680 error code : ") << Bme->bme68xStatus;
      } else {
     // @TODO   Serial << endl             << F("WARN: Bosch: BME680 warning code : ") << Bme->bme68xStatus;
      }
    } else {
      //  Serial << endl << F("INFO: Bosch: Health: OK");
    }

  */
}
#endif

void AFESensorBME680::loadState(void)
{

  if (EEPROM.read(0) == BSEC_MAX_STATE_BLOB_SIZE)
  {
#ifdef DEBUG
    Debugger->printBulletPoint(F("Reading state from EEPROM: "));
#endif
    for (uint8_t i = 0; i < BSEC_MAX_STATE_BLOB_SIZE; i++)
    {
      bsecState[i] = EEPROM.read(i + 1);
#ifdef DEBUG
      Serial << _HEX(bsecState[i]);
#endif
    }
    Bme->setState(bsecState);
#ifdef DEBUG
    checkBmeStatus();
#endif
  }
  else
  {
#ifdef DEBUG
    Debugger->printBulletPoint(F("Erasing EEPROM"));
#endif
    for (uint8_t i = 0; i < BSEC_MAX_STATE_BLOB_SIZE + 1; i++)
      EEPROM.write(i, 0);

    EEPROM.commit();
  }
}

void AFESensorBME680::updateState(void)
{
  bool update = false;
  /* Set a trigger to save the state. Here, the state is saved every
   * STATE_SAVE_PERIOD with the first state being saved once the algorithm
   * achieves full calibration, i.e. iaqAccuracy = 3 */
  if (stateUpdateCounter == 0)
  {
    /* @TODO T6
     if (Bme->iaqAccuracy >= 3) {
       update = true;
       stateUpdateCounter++;
     }
     */
  }
  else
  {
    /* Update every STATE_SAVE_PERIOD milliseconds */
    if ((stateUpdateCounter * STATE_SAVE_PERIOD) < millis())
    {
      update = true;
      stateUpdateCounter++;
    }
  }

  if (update)
  {
    Bme->getState(bsecState);
#ifdef DEBUG
    checkBmeStatus();
    Debugger->printBulletPoint(F("Writing state to EEPROM: "));
#endif

    for (uint8_t i = 0; i < BSEC_MAX_STATE_BLOB_SIZE; i++)
    {
      EEPROM.write(i + 1, bsecState[i]);
#ifdef DEBUG
      Serial << _HEX(bsecState[i]);
#endif
    }

    EEPROM.write(0, BSEC_MAX_STATE_BLOB_SIZE);
    EEPROM.commit();
  }
}
#endif // AFE_CONFIG_HARDWARE_BMEX80