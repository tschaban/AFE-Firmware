#include "AFE-Firmware.h"

AFEFirmware::AFEFirmware(){};

void AFEFirmware::begin()
{

  timer->miliseconds = millis();
  timer->minutes = 0;
  timer->hours = 0;
  timer->days = 0;
  timer->months = 0;
#ifdef DEBUG
  API->Flash->addReference(Debugger);
  API->REST->addReference(Debugger);
#endif
  API->Flash->initializeFileSystem();

#ifdef DEBUG
  Device->begin(API->Flash, Debugger);
#else
  Device->begin(API->Flash);
#endif

  firstBooting();

  /**
   * @brief saving information how many times firmare has been rebooted. For
   * debug purpose
   *
   */
  unsigned long _counter = API->Flash->getRebootCounter();
#ifdef DEBUG

  Debugger->printInformation(F("Firmware rebooted: "), F("BOOT"));
  Debugger->printValue(_counter, F("x"));

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
     // @TODO T4 Led->addMCP23017Reference(MCP23017Broker);
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
      Device->refreshConfiguration();
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