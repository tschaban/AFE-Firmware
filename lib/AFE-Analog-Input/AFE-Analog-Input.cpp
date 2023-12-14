/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-Analog-Input.h"

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT

#ifdef DEBUG
AFEAnalogInput::AFEAnalogInput(AFEDataAccess *_Data, AFEDebugger *_Debugger)
{
  Debugger = _Debugger;
  Data = _Data;
};
#else
AFEAnalogInput::AFEAnalogInput(AFEDataAccess *_Data) { Data = _Data; };
#endif

#ifdef AFE_ESP32
void AFEAnalogInput::begin(uint8_t id)
{
  Data->getConfiguration(id, configuration);
  begin();
}
#endif

void AFEAnalogInput::begin()
{

#ifndef AFE_ESP32
  Data->getConfiguration(configuration);
#endif

#ifdef DEBUG
  Debugger->printHeader(2, 1, 72, AFE_DEBUG_HEADER_TYPE_DASH);
  Debugger->printValue(F("ADC Configuration"));
  Debugger->printBulletPoint(F("GPIO: "));
  Debugger->printValue(configuration->gpio);
  Debugger->printBulletPoint(F("Interval: "));
  Serial << configuration->interval;
  Debugger->printBulletPoint(F("Max VCC: "));
  Serial << configuration->maxVCC;
  Debugger->printBulletPoint(F("# of samples: "));
  Serial << configuration->numberOfSamples;
  Debugger->printBulletPoint(F("R[A]: "));
  Serial << configuration->divider.Ra;
  Debugger->printBulletPoint(F("r[B]: "));
  Serial << configuration->divider.Rb;

  Debugger->printHeader(1, 1, 72, AFE_DEBUG_HEADER_TYPE_DASH);
#endif
  _initialized = true;
}

boolean AFEAnalogInput::isReady()
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

void AFEAnalogInput::listener()
{

  if (_initialized)
  {
    unsigned long time = millis();

    if (startTime == 0)
    {
      startTime = time;
    }

    if (time - startTime >= configuration->interval * 1000)
    {

#ifdef DEBUG
      Debugger->printHeader(2, 1, 72, AFE_DEBUG_HEADER_TYPE_DASH);
      Debugger->printValue(F("Reading ADC input"));
      Debugger->printBulletPoint(F("Interval: "));
      Serial << (time - startTime) << F("msec");
#endif
      if (counterOfSamplings < configuration->numberOfSamples)
      {
        temporaryAnalogData += analogRead(configuration->gpio);
        counterOfSamplings++;
      }
      else
      {
        data->raw =
            (uint16_t)(temporaryAnalogData / configuration->numberOfSamples);

        data->percent = (float)data->raw * 100 / 1024;
        data->voltage = (double)(configuration->maxVCC * data->raw / 1024);
        if (configuration->divider.Rb > 0)
        {
          data->voltageCalculated = (data->voltage * (configuration->divider.Ra +
                                                      configuration->divider.Rb)) /
                                    configuration->divider.Rb;
        }
        else
        {
          data->voltageCalculated = data->voltage;
        }

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
        if (data->voltageCalculated >= configuration->battery.maxVoltage)
        {
          batteryPercentage = 100;
        }
        else if (data->voltageCalculated <= configuration->battery.minVoltage)
        {
          batteryPercentage = 0;
        }
        else if (configuration->battery.maxVoltage -
                     configuration->battery.minVoltage >
                 0)
        {
          batteryPercentage =
              (data->voltageCalculated - configuration->battery.minVoltage) *
              100 / (configuration->battery.maxVoltage - configuration->battery.minVoltage);
        }
        else
        {
          batteryPercentage = 0;
        }

#endif

#ifdef DEBUG
        Debugger->printBulletPoint(F("Number of samples: "));
        Debugger->printValue(counterOfSamplings);
        Debugger->printBulletPoint(F("Analog value: "));
        Serial << data->raw;
        Debugger->printBulletPoint(F("Percent: "));
        Debugger->printValue(data->percent);
        Debugger->printBulletPoint(F("Voltage: "));
        Serial << data->voltage;
        Debugger->printBulletPoint(F("Voltage Calculated: "));
        Serial << data->voltageCalculated;
        Debugger->printBulletPoint(F("Sampling time: "));
        Serial << endl
               << millis() - startTime - configuration->interval * 1000
               << F("msec");
        Debugger->printHeader(1, 1, 72, AFE_DEBUG_HEADER_TYPE_DASH);
#endif

        counterOfSamplings = 0;
        temporaryAnalogData = 0;
        ready = true;
        startTime = 0;
      }
    }
  }
}

void AFEAnalogInput::getJSON(char *json)
{
  sprintf(json,
          (const char *)F("{\"raw\":%d,\"percent\":%f,\"voltage\":%f,\"voltageCalculated\":%f}"),
          data->raw, data->percent, data->voltage, data->voltageCalculated);
}

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
/* Returns the sensor data in JSON format */
void AFEAnalogInput::getBatteryMeterJSON(char *json)
{
  sprintf(json,
          (const char *)F("{\"batterymeter\":[{\"value\":%.3f,\"unit\":\"%%\"},{\"value\":"
                          "%.3f,\"unit\":\"volt\"}]}"),
          batteryPercentage, data->voltageCalculated);
}
#endif

#endif // AFE_CONFIG_HARDWARE_ANALOG_INPUT