/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_HOME_ASSISTANT_Structure_h
#define _AFE_HOME_ASSISTANT_Structure_h

#include <AFE-Configuration.h>
#include <Arduino.h>
#include <AFE-MQTT-Structure.h>


struct HOME_ASSISTANT_CONFIG {
    boolean addingComponents;
    boolean removeingComponents;  
    MQTT_TOPIC discovery;
};

#endif
