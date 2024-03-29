/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-Sensor-BME680.h"
#ifdef AFE_CONFIG_HARDWARE_BMEX80

AFESensorBME680::AFESensorBME680(){};

boolean AFESensorBME680::begin(BMEX80 *_configuration, TwoWire *WirePort, BMEX80_DATA *_data)
{
#ifdef DEBUG
  Serial << endl
         << F("INFO: Initializing: Sensor type: BME680");
#endif

  data = _data;

  EEPROM.begin(BSEC_MAX_STATE_BLOB_SIZE + 1); // 1st address for the length
#ifdef DEBUG
  Serial << endl
         << F("INFO: EEPROM initialized: Size reserved: ")
         << BSEC_MAX_STATE_BLOB_SIZE + 1;
#endif

  configuration = _configuration;
  if (configuration->i2cAddress != 0)
  {

#ifdef DEBUG
    Serial << endl
           << F("INFO: Sensor address: 0x") << _HEX(configuration->i2cAddress);
#endif
    Bme->begin(configuration->i2cAddress, *WirePort);

#ifdef DEBUG
    Serial << endl
           << F("INFO: Bosch BSEC library version ") << Bme->version.major
           << F(".") << Bme->version.minor << F(".")
           << Bme->version.major_bugfix << F(".") << Bme->version.minor_bugfix;

    checkBmeStatus();
    Serial << endl
           << F("INFO: Bosch: Setting config");

#endif

    Bme->setConfig(bsec_config_iaq);
#ifdef DEBUG
    checkBmeStatus();
#endif

    loadState();
#ifdef DEBUG
    Serial << endl
           << F("INFO: Bosch: Stated loaded from the EEPROM") << endl
           << F("INFO: Bosch: Updating subscription");
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
          Serial.println("\tiaq = " + String(output.signal));
          Serial.println("\tiaq accuracy = " + String((int)output.accuracy));
          data->iaq.value = output.signal;
          data->iaq.accuracy = output.accuracy;
          break;
        case BSEC_OUTPUT_RAW_TEMPERATURE:
          Serial.println("\ttemperature = " + String(output.signal));
          data->temperature.value = output.signal;
          break;
        case BSEC_OUTPUT_RAW_PRESSURE:
          Serial.println("\tpressure = " + String(output.signal));
          data->pressure.value = output.signal;
          break;
        case BSEC_OUTPUT_RAW_HUMIDITY:
          Serial.println("\thumidity = " + String(output.signal));
          data->humidity.value = output.signal;
          break;
        case BSEC_OUTPUT_RAW_GAS:
          Serial.println("\tgas resistance = " + String(output.signal));
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
          Serial.println("\tstabilization status = " + String(output.signal));
          break;
        case BSEC_OUTPUT_RUN_IN_STATUS:
          Serial.println("\trun in status = " + String(output.signal));
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
      Serial << endl
             << F("ERROR: Bosch: BSEC error code : ") << Bme->status;
    }
    else
    {
      Serial << endl
             << F("WARN: Bosch: BSEC warning code : ") << Bme->status;
    }
  }
  else
  {
    Serial << endl
           << F("INFO: Bosch: Health: OK");
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
    Serial << endl
           << F("INFO: Bosch: Reading state from EEPROM: ");
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
    Serial << endl
           << F("INFO: Bosch: Erasing EEPROM");
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
    Serial << endl
           << F("INFO: Bosch: Writing state to EEPROM: ");
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