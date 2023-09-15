#include "AFE-Firmware.h"

AFEFirmware::AFEFirmware(){};

void AFEFirmware::begin()
{

  timer->miliseconds = millis();
  timer->minutes = 0;
  timer->hours = 0;
  timer->days = 0;
  timer->months = 0;

  API->Flash->initializeFileSystem();

  Device->begin();

  firstBooting();

  /**
   * @brief saving information how many times firmare has been rebooted. For
   * debug purpose
   *
   */
  unsigned long _counter = API->Flash->getRebootCounter();
#ifdef DEBUG

  Debugger->printInformation(F("Firmware rebooted"), F("BOOT"), 0, 0);
  Debugger->printValue(_counter, F("times"));

#endif

/**
 * @brief Initializating REST API
 *
 */
#ifdef AFE_CONFIG_HARDWARE_LED
  API->REST->begin(API->Flash, Device, Hardware->SystemLed);
#else
  API->REST->begin(API->Flash, Device);
#endif // AFE_CONFIG_HARDWARE_LED

  API->Flash->getConfiguration(Configuration->Pro);
  API->Flash->getConfiguration(Configuration->Version);
#ifdef AFE_CONFIG_HARDWARE_I2C
  initializeIIC();
#endif
}

void AFEFirmware::initializeNetwork(void)
{
#ifdef DEBUG
  Debugger->printInformation(F("Starting network"), F("BOOT"));
  API->Network->addReference(Debugger);
#endif
#ifdef AFE_CONFIG_HARDWARE_LED
  API->Network->begin(Device, API->Flash, Hardware->SystemLed);
#else
  API->Network->begin(Device, API->Flash);
#endif // AFE_CONFIG_HARDWARE_LED

  API->Network->listener();
}

#ifdef AFE_CONFIG_HARDWARE_LED
void AFEFirmware::initializeSystemLED(void)
{
  uint8_t id = API->Flash->getSystemLedID();
#ifdef DEBUG
  boolean initialized = false;
#endif
  if (id != AFE_HARDWARE_ITEM_NOT_EXIST)
  {
    if (Device->configuration.noOfLEDs - 1 >= id)
    {
#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
      Led->addMCP23017Reference(MCP23017Broker);
#endif // AFE_CONFIG_HARDWARE_MCP23XXX
      if (Hardware->SystemLed->begin(API->Flash, id))
      {
        Hardware->SystemLed->on();
#ifdef DEBUG
        initialized = true;
#endif
      }
    }
#ifdef DEBUG
    if (initialized)
    {
      Debugger->printInformation(F("System LED initialized"), F("BOOT"));
    }
    else
    {
      Debugger->printWarning(F("System LED NOT initialized"), F("BOOT"));
    }
#endif // DEBUG
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_I2C
void AFEFirmware::initializeIIC(void)
{
  I2CPORT I2CBUSConfiguration;
  AFEI2CScanner *I2CBus = new AFEI2CScanner();
  boolean success = false;

#ifdef AFE_ESP32
  if (Device->configuration.noOfI2Cs > 0)
  {
    API->Flash->getConfiguration(0, &I2CBUSConfiguration);

#ifdef DEBUG
    Serial << endl
           << F("INFO: I2C[0]: SDA: ") << I2CBUSConfiguration.SDA
           << F(", SCL: ") << I2CBUSConfiguration.SCL << F(", Frequency: ")
           << (I2CBUSConfiguration.frequency / 1000) << F("Hz");
#endif

    success = Hardware->WirePort0->begin(I2CBUSConfiguration.SDA,
                                         I2CBUSConfiguration.SCL,
                                         I2CBUSConfiguration.frequency);
#else /* ESP8266 */
  API->Flash->getConfiguration(&I2CBUSConfiguration);

#ifdef DEBUG

  Debugger->printInformation(F("Initialization"), F("I2C"));
  Debugger->printBulletPoint("SDA:");
  Debugger->printValue(I2CBUSConfiguration.SDA);
  Debugger->printBulletPoint("SCL:");
  Debugger->printValue(I2CBUSConfiguration.SCL);

#endif

  Hardware->WirePort0->begin(I2CBUSConfiguration.SDA, I2CBUSConfiguration.SCL);
  success = true;
#endif // AFE_ESP32

#ifdef DEBUG
    // @TODO there was delay(100) here, testing without it
    if (!success)
    {
      Debugger->printError(F("Doesn't work"), F("I2C[0]"));
    }
    else
    {
      I2CBus->begin(Hardware->WirePort0);
      I2CBus->scanAll();
    }
#endif // DEBUG

#ifdef AFE_ESP32
  }

  if (Device->configuration.noOfI2Cs > 1)
  {
    API->Flash->getConfiguration(1, &I2CBUSConfiguration);

#ifdef DEBUG
    Serial << endl
           << F("INFO: I2C[1]: SDA: ") << I2CBUSConfiguration.SDA
           << F(", SCL: ") << I2CBUSConfiguration.SCL << F(", Frequency: ")
           << I2CBUSConfiguration.frequency / 1000 << F("Hz");
#endif // DEBUG

    success = Hardware->WirePort1->begin(I2CBUSConfiguration.SDA,
                                         I2CBUSConfiguration.SCL,
                                         I2CBUSConfiguration.frequency);

#ifdef DEBUG

    if (!success)
    {
      Serial << endl
             << F("ERROR: I2C[1]: Bus doesn't work");
    }
    else
    {
      Serial << endl
             << F("INFO: I2C[1]: Scannings for devices");
      I2CBus->begin(Hardware->WirePort1);
      I2CBus->scanAll();
    }
#endif // DEBUG
  }
#endif // AFE_ESP32

  delete I2CBus;
  I2CBus = NULL;
}
#endif

void AFEFirmware::validateProVersion(void)
{

#ifdef DEBUG
  Debugger->printInformation(F("Checking AFE Pro Key"), F("AFE Pro"));
#endif

  if (strlen(Configuration->Pro->serial) == 0 && Configuration->Pro->valid)
  {
    Configuration->Pro->valid = false;
#ifdef DEBUG
    Debugger->printInformation(F("Valid with no key"), F("AFE Pro"));
#endif
    API->Flash->saveConfiguration(Configuration->Pro);
  }
  else if (strlen(Configuration->Pro->serial) > 0)
  {
    String _HtmlResponse;
    boolean isValid;
    _HtmlResponse.reserve(6);
    if (API->REST->sent(_HtmlResponse, AFE_CONFIG_JSONRPC_REST_METHOD_IS_PRO))
    {
      isValid = _HtmlResponse.length() > 0 && _HtmlResponse.equals("true")
                    ? true
                    : false;

      if (Configuration->Pro->valid != isValid)
      {
        Configuration->Pro->valid = isValid;
        API->Flash->saveConfiguration(Configuration->Pro);
#ifdef DEBUG
        Debugger->printInformation(F("Key state has been changed"), F("AFE Pro"));
#endif
      }

#ifdef DEBUG
      Debugger->printInformation(F("Key checked: "), F("AFE Pro"));
      Debugger->printValue(isValid ? F("valid") : F("invalid"), 0);
#endif
    }
#ifdef DEBUG
    else
    {
      Debugger->printError(F("while checing the key"), F("AFE Pro"));
    }
#endif
  }
}

void AFEFirmware::checkFirmwareVersion(void)
{
#ifdef DEBUG
  Debugger->printInformation(F("Checking firmware version"), F("FIRMWARE"));
#endif

  String _HtmlResponse;
  _HtmlResponse.reserve(10);

  if (API->REST->sent(
          _HtmlResponse,
          AFE_CONFIG_JSONRPC_REST_METHOD_GET_LATEST_FIRMWARE_VERSION))
  {
    if (_HtmlResponse.length() > 0 &&
        _HtmlResponse.length() <=
            sizeof(Configuration->Version->installed_version))
    {
      char _tempVersion[sizeof(Configuration->Version->installed_version)];
      _HtmlResponse.toCharArray(
          _tempVersion, sizeof(Configuration->Version->installed_version));
      if (strcmp(_tempVersion, Configuration->Version->installed_version) !=
          0)
      {
        API->Flash->saveLatestFirmwareVersion(_tempVersion);
#ifdef DEBUG
        Debugger->printInformation(F("New firmware version available"), F("FIRMWARE"));
      }
      else
      {
        Debugger->printInformation(F("Up2date"), F("FIRMWARE"));
#endif
      }
    }
  }
}

void AFEFirmware::firstBooting(void)
{

#ifdef DEBUG
  Debugger->printInformation(F("Checking if first time launch"), F("FIRMWARE"));
#endif

  if (Device->getMode() == AFE_MODE_FIRST_TIME_LAUNCH)
  {
#ifdef DEBUG
    Debugger->printBulletPoint(F("Yes"));
#endif
    if (API->Flash->setDefaultConfiguration())
    {
      Device->begin();
    }
    else
    {
      Device->reboot();
    }
#ifdef DEBUG
  }
  else
  {
    Debugger->printBulletPoint(F("No"));
#endif
  }
}