<?php


$targetLanguage[0] = "pl";
$targetLanguage[1] = "en";

$targetAPI[0] = "domoticz.api";
$targetAPI[1] = "generic.api";
$targetAPI[2] = "generic.api";

$rootPath = "C:/Users/Adrian/Cloud/Pulpit/";
$targetFolder = $rootPath."afe.firmware.t".$type.".".$version;

/* 
### AFE_DEVICE_ID ###

0 ESP Generic
30 ESP32 30Pins  // From version 3.7.2 30Pins is obsolete. Replaced by version 38 pins
31 ESP32 38Pins

### HARDWARE ID ###

0  : ESP8266
1  : ESP8285
3  : ESP32-30p  // From version 3.7.2 30Pins is obsolete. Replaced by version 38 pins
4  : ESP32-38p


*/

$index=0;
$targetHardware[$index][0] = "esp8266";     // Folder name
$targetHardware[$index][1] = 0;             // AFE_DEVICE_ID, Look at /core/include/config/devices/AFE-Device.h
$targetHardware[$index][2] = "";            // Part of the file

$index++; // 1
$targetHardware[$index][0] = "esp8285";
$targetHardware[$index][1] = 0;
$targetHardware[$index][2] = "";

$index++; // 2
$targetHardware[$index][0] = "esp32";
$targetHardware[$index][1] = 30;
$targetHardware[$index][2] = "38pins";

$index++; // 3
$targetHardware[$index][0] = "esp32";
$targetHardware[$index][1] = 31;
$targetHardware[$index][2] = "38pins";


$folderStructure[0] = $targetFolder;
$folderStructure[1] = $targetFolder."/".$targetLanguage;
$folderStructure[2] = $folderStructure[1]."/".$targetLanguage;

// Standard API

/* ESP8266 1MB */
$index=0;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T7_8266_1m-ha/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[1];

/* ESP8266 4MB */
$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T7_8266_4m-ha/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[1];

/* ESP8266 4MB Debug */
$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T7_8266_4m-ha-development/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[1];


/* ESP8285 1MB */
$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T7_8285_1m-ha/firmware.bin";
$sourceFolder[$index]["chip"] = 8285;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 1;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[1];

/* ESP32 4MB 30p */
$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T7_esp32_4m_38P-ha/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 2;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[1];

/* ESP32 4MB 30p Debug */
$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T7_esp32_4m_38P-ha-development/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 2;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[1];

/* ESP32 4MB 38p */
$index++; // The bin file is not created (30p merged with 38p) stays for bin cont.
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T7_esp32_4m_38P-ha/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 3;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[1];

/* ESP32 4MB 38p  Debug*/
$index++; // The bin file is not created (30p merged with 38p) stays for bin cont.
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T7_esp32_4m_38P-ha-development/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 3;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[1];

/**
 * Domoticz API
 */

/* ESP8266 1MB */
$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T7_8266_1m-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[0];

/* ESP8266 4MB */
$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T7_8266_4m-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[0];

/* ESP8266 4MB Debug */
$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T7_8266_4m-domoticz-development/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[0];


/* ESP8285 1MB */
$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T7_8285_1m-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = 8285;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 1;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[0];

/* ESP32 4MB 30p */
$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T7_esp32_4m_38P-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 2;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[0];

/* ESP32 4MB 30p Debug */
$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T7_esp32_4m_38P-domoticz-development/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 2;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[0];

/* ESP32 4MB 38p */
$index++; // The bin file is not created (30p merged with 38p) stays for bin cont.
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T7_esp32_4m_38P-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 3;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[0];

/* ESP32 4MB 38p  Debug*/
$index++; // The bin file is not created (30p merged with 38p) stays for bin cont.
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T7_esp32_4m_38P-domoticz-development/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 3;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[0];

/**
 * Home Assistant API
 */

/* ESP8266 1MB */
$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T7_8266_1m-ha/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[2];

/* ESP8266 4MB */
$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T7_8266_4m-ha/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[2];

/* ESP8266 4MB Debug */
$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T7_8266_4m-ha-development/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[2];


/* ESP8285 1MB */
$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T7_8285_1m-ha/firmware.bin";
$sourceFolder[$index]["chip"] = 8285;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 1;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[2];

/* ESP32 4MB 30p */
$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T7_esp32_4m_38P-ha/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 2;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[2];

/* ESP32 4MB 30p Debug */
$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T7_esp32_4m_38P-ha-development/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 2;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[2];

/* ESP32 4MB 38p */
$index++; // The bin file is not created (30p merged with 38p) stays for bin cont.
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T7_esp32_4m_38P-ha/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 3;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[2];

/* ESP32 4MB 38p  Debug*/
$index++; // The bin file is not created (30p merged with 38p) stays for bin cont.
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T7_esp32_4m_38P-ha-development/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 3;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[2];

?>
