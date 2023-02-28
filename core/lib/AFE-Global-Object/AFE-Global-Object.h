/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_Global_Objects_h
#define _AFE_Global_Objects_h

#include <AFE-Configuration.h>

#include <AFE-API-JSONRPC.h>
#include <AFE-Data-Access.h>
#include <AFE-Device.h>
#include <AFE-Firmware.h>
#include <AFE-WiFi.h>

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
#include <AFE-API-HTTP-Domoticz.h>
#include <AFE-API-MQTT-Domoticz.h>
#else
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_HOME_ASSISTANT
#include <AFE-API-HomeAssistant-Integration.h>
#endif
#include <AFE-API-MQTT-Standard.h>
#endif


class AFEGlobalObject {
private:
public:

  struct GLOBAL_CORE_OBJECTS {
    AFEDataAccess *Data;
    AFEDevice *Device;
    AFEWiFi *Network;
    AFEJSONRPC *RestAPI;
    AFEFirmware *Firmware;

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    AFEAPIMQTTDomoticz *MqttAPI;
    AFEAPIHTTPDomoticz *HttpDomoticzAPI;
#else
    AFEAPIMQTTStandard *MqttAPI;
#endif
  };

  struct GLOBAL_HARDWARE_OBJECTS {
    AFELED *SystemLed;
    AFERelay *Relay[AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS];
    AFERelay *Contactron[AFE_CONFIG_HARDWARE_NUMBER_OF_CONTACTRONS];
  };

  GLOBAL_CORE_OBJECTS *Core = new GLOBAL_CORE_OBJECTS;
  GLOBAL_HARDWARE_OBJECTS *Hardware = new GLOBAL_HARDWARE_OBJECTS;

  AFEGlobalObject();
  void begin();
};

#endif
