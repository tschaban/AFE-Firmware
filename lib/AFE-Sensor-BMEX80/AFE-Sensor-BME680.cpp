/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Sensor-BME680.h"

AFESensorBME680::AFESensorBME680(){};

boolean AFESensorBME680::begin(BMEX80 *_configuration) {
  configuration = _configuration;

#ifdef DEBUG
  Serial << endl << "Sensor type: BME680";
#endif

  if (configuration->i2cAddress != 0) {
#ifdef DEBUG
    Serial << endl << "Address: 0x" << _HEX(configuration->i2cAddress);
#endif
    Wire.begin();
    Bme.begin(configuration->i2cAddress, Wire);

#ifdef DEBUG
    Serial << endl
           << "BSEC library version " << Bme.version.major << "."
           << Bme.version.minor << "." << Bme.version.major_bugfix << "."
           << Bme.version.minor_bugfix;

    checkBmeStatus();
#endif

    bsec_virtual_sensor_t sensorList[8] = {
        BSEC_OUTPUT_RAW_TEMPERATURE,
        BSEC_OUTPUT_RAW_PRESSURE,
        BSEC_OUTPUT_RAW_HUMIDITY,
        BSEC_OUTPUT_RAW_GAS,
        BSEC_OUTPUT_IAQ,
        BSEC_OUTPUT_STATIC_IAQ,
        BSEC_OUTPUT_CO2_EQUIVALENT,
        BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
    };

    Bme.updateSubscription(sensorList, 8, BSEC_SAMPLE_RATE_LP);

    /*
        if (!bme.begin(configuration->i2cAddress)) {
          return false;
        } else {
          bme.setTemperatureOversampling(BME680_OS_8X);
          bme.setHumidityOversampling(BME680_OS_2X);
          bme.setPressureOversampling(BME680_OS_4X);
          bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
          bme.setGasHeater(320, 150); // 320*C for 150 ms
          return true;
        }


      } else {
    #ifdef DEBUG
        Serial << endl << "Error: Address not set";
    #endif
        return false;
      }
    */
  }
  return true;
}

boolean AFESensorBME680::read() {
#ifdef DEBUG
  Serial << endl << "Sensor: BME680";
#endif

  if (Bme.run()) {
    data.temperature.value = Bme.temperature;
    data.pressure.value = Bme.pressure / 100;
    data.humidity.value = Bme.humidity;
    data.airQuality.gasResistance.value = Bme.gasResistance / 1000;
    data.airQuality.iaq.value = Bme.iaq;
    data.airQuality.iaq.accuracy = Bme.iaqAccuracy;
    data.airQuality.staticIaq.value = Bme.staticIaq;
    data.airQuality.staticIaq.accuracy = Bme.staticIaqAccuracy;
    data.airQuality.co2Equivalent.value = Bme.co2Equivalent;
    data.airQuality.co2Equivalent.accuracy = Bme.co2Accuracy;
    data.airQuality.breathVocEquivalent.value = Bme.breathVocEquivalent;
    data.airQuality.breathVocEquivalent.accuracy = Bme.breathVocAccuracy;
    return true;
  } else {
  #ifdef DEBUG
    checkBmeStatus();
  #endif
    return false;
  }

  /*

    if (bme.performReading()) {
      data.temperature = bme.temperature;
      data.pressure = bme.pressure / 100.0;
      data.humidity = bme.humidity;
      data.gasResistance = bme.gas_resistance / 1000;
      return true;
    } else {
      return false;
    }
    */
}

#ifdef DEBUG
void AFESensorBME680::checkBmeStatus() {

  if (Bme.status != BSEC_OK) {
    if (Bme.status < BSEC_OK) {
      Serial << endl << "BSEC error code : " << Bme.status;
    } else {
      Serial << endl << "BSEC warning code : " << Bme.status;
    }
  } else {
    Serial << endl << "BSEC stauts: OK";
  }

  if (Bme.bme680Status != BME680_OK) {
    if (Bme.bme680Status < BME680_OK) {
      Serial << endl << "BME680 error code : " << Bme.bme680Status;
    } else {
      Serial << endl << "BME680 warning code : " << Bme.bme680Status;
    }
  } else {
    Serial << endl << "BME680 stauts: OK";
  }
}
#endif
