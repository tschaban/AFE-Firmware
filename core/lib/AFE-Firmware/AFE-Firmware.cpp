#include "AFE-Firmware.h"

AFEFirmware::AFEFirmware(){};

void AFEFirmware::begin() {

  timer->miliseconds = millis();
  timer->minutes = 0;
  timer->hours = 0;
  timer->days = 0;
  timer->months = 0;

  Device->begin();
  firstBooting();

  /**
   * @brief saving information how many times firmare has been rebooted. For
   * debug purpose
   *
   */
  unsigned long _counter = API->Flash->getRebootCounter();
#ifdef DEBUG
  Serial << endl << F("INFO: Firmware rebooted: ") << _counter << F(" times");
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

void AFEFirmware::initializeNetwork(void) {
#ifdef DEBUG
  Serial << endl << F("INFO: BOOT: Starting network");
#endif
#ifdef AFE_CONFIG_HARDWARE_LED
  API->Network->begin(Device, API->Flash, Hardware->SystemLed);
#else
  API->Network->begin(Device, API->Flash);
#endif // AFE_CONFIG_HARDWARE_LED

  API->Network->listener();
}

#ifdef AFE_CONFIG_HARDWARE_LED
void AFEFirmware::initializeSystemLED(void) {
  uint8_t id = API->Flash->getSystemLedID();
#ifdef DEBUG
  boolean initialized = false;
#endif
  if (id != AFE_HARDWARE_ITEM_NOT_EXIST) {
    if (Device->configuration.noOfLEDs - 1 >= id) {
#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
      Led->addMCP23017Reference(MCP23017Broker);
#endif // AFE_CONFIG_HARDWARE_MCP23XXX
      if (Hardware->SystemLed->begin(API->Flash, id)) {
        Hardware->SystemLed->on();
#ifdef DEBUG
        initialized = true;
#endif
      }
    }
#ifdef DEBUG
    if (initialized) {
      Serial << endl << F("INFO: BOOT: System LED initialized");
    } else {
      Serial << endl << F("WARN: BOOT: System LED NOT initialized");
    }
#endif // DEBUG
  }
}
#endif

void AFEFirmware::validateProVersion(void) {

#ifdef DEBUG
  Serial << endl << F("INFO: AFE PRO: Checking AFE Pro Key");
#endif

  if (strlen(Configuration->Pro->serial) == 0 && Configuration->Pro->valid) {
    Configuration->Pro->valid = false;
#ifdef DEBUG
    Serial << endl << F("INFO: AFE PRO: Valid with no key");
#endif
    API->Flash->saveConfiguration(Configuration->Pro);
  } else if (strlen(Configuration->Pro->serial) > 0) {
    String _HtmlResponse;
    boolean isValid;
    _HtmlResponse.reserve(6);
    if (API->REST->sent(_HtmlResponse, AFE_CONFIG_JSONRPC_REST_METHOD_IS_PRO)) {
      isValid = _HtmlResponse.length() > 0 && _HtmlResponse.equals("true")
                    ? true
                    : false;

      if (Configuration->Pro->valid != isValid) {
        Configuration->Pro->valid = isValid;
        API->Flash->saveConfiguration(Configuration->Pro);
#ifdef DEBUG
        Serial << endl << F("INFO: AFE PRO: Key state has been changed");
#endif
      }

#ifdef DEBUG
      Serial << endl
             << F("INFO: AFE PRO: Key checked: ")
             << (isValid ? F("valid") : F("invalid"));
#endif
    }
#ifdef DEBUG
    else {
      Serial << endl << F("ERROR: AFE PRO: while checing the key");
    }
#endif
  }
}

void AFEFirmware::checkFirmwareVersion(void) {
#ifdef DEBUG
  Serial << endl << F("INFO: FIRMWARE: Checking firmware version");
#endif

  String _HtmlResponse;
  _HtmlResponse.reserve(10);

  if (API->REST->sent(
          _HtmlResponse,
          AFE_CONFIG_JSONRPC_REST_METHOD_GET_LATEST_FIRMWARE_VERSION)) {
    if (_HtmlResponse.length() > 0 &&
        _HtmlResponse.length() <=
            sizeof(Configuration->Version->installed_version)) {
      char _tempVersion[sizeof(Configuration->Version->installed_version)];
      _HtmlResponse.toCharArray(
          _tempVersion, sizeof(Configuration->Version->installed_version));
      if (strcmp(_tempVersion, Configuration->Version->installed_version) !=
          0) {
        API->Flash->saveLatestFirmwareVersion(_tempVersion);
#ifdef DEBUG
        Serial << endl
               << "INFO: FIRMWARE: New firmware version available: "
               << _HtmlResponse;
      } else {
        Serial << endl << "INFO: FIRMWARE: Up2date: " << _HtmlResponse;
#endif
      }
    }
  }
}

void AFEFirmware::firstBooting(void) {

#ifdef DEBUG
  Serial << endl << F("INFO: FIRMWARE: Checking if first time launch ... ");
#endif

  if (Device->getMode() == AFE_MODE_FIRST_TIME_LAUNCH) {
#ifdef DEBUG
    Serial << F("YES");
#endif
    if (Device->setDevice()) {
      Device->begin();
    } else {
      Device->reboot();
    }
#ifdef DEBUG
  } else {
    Serial << F("NO");
#endif
  }
}