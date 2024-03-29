<?php

$targetLanguage[0] = "pl";
$targetLanguage[1] = "en";

$targetAPI[0] = "domoticz.api";
$targetAPI[1] = "standard.api";
$targetAPI[2] = "homeassistant.api";

$rootPath = "C:/Users/Adrian/Cloud/Pulpit/";
$targetFolder = $rootPath."afe.firmware.t".$type.".".$version;
 

/* 
### AFE_DEVICE_ID ###

0 ESP Generic
15 T2 E1 Custom
30 ESP32 30Pins  // From version 3.7.2 30Pins is obsolete. Replaced by version 38 pins
31 ESP32 38Pins

### HARDWARE ID ###

0  : ESP8266
1  : ESP8285
2  : ESP32-30p  // From version 3.7.2 30Pins is obsolete. Replaced by version 38 pins
3  : ESP32-38p
4  : E1

*/


$targetHardware[0][0] = "esp8266";  // Folder name
$targetHardware[0][1] = 0;          // AFE_DEVICE_ID, Look at /core/include/config/devices/AFE-Device.h
$targetHardware[0][2] = "";         // Part of the file

$targetHardware[1][0] = "esp8285";
$targetHardware[1][1] = 0; 
$targetHardware[1][2] = "";

$targetHardware[2][0] = "esp32";
$targetHardware[2][1] = 30; 
$targetHardware[2][2] = "38pins";

$targetHardware[3][0] = "esp32";
$targetHardware[3][1] = 31; 
$targetHardware[3][2] = "38pins";


$targetHardware[4][0] = "esp8266";
$targetHardware[4][1] = 15; 
$targetHardware[4][2] = "e1";


$folderStructure[0] = $targetFolder;
$folderStructure[1] = $targetFolder."/".$targetLanguage;
$folderStructure[2] = $folderStructure[1]."/".$targetLanguage;

// Standard API ESP8266 4MB

$index=0;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T2_8266_4m-generic/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[1];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T2_8266_4m-generic-development/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[1];

// Standard API ESP8266 1MB

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T2_8266_1m-generic/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[1];

/*
$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T2_8266_1m-generic-development/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[1];
*/

// Standard API ESP8285 

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T2_8285-generic/firmware.bin";
$sourceFolder[$index]["chip"] = 8285;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 1;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[1];

// Standard API Custom versions

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T2_8266_1m-generic-e1/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 4;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[1];


// Standard API ESP32

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T2_esp32_4m_38P-generic/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 2;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[1];

$index++; // The bin file is not created (30p merged with 38p) stays for bin cont.
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T2_esp32_4m_38P-generic/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 3;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[1];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T2_esp32_4m_38P-generic-development/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 2;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[1];

$index++; // The bin file is not created (30p merged with 38p) stays for bin cont.
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T2_esp32_4m_38P-generic-development/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 3;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[1];



// Domoticz API ESP8266 4MB

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T2_8266_4m-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[0];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T2_8266_4m-domoticz-development/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[0];

// Domoticz API ESP8266 1MB

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T2_8266_1m-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[0];

/*
$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T2_8266_1m-domoticz-development/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[0];
*/

// Domoticz API ESP8285

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T2_8285-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = 8285;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 1;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[0];

// Domoticz API Custom versions

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T2_8266_1m-domoticz-e1/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 4;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[0];

// Domoticz API ESP32

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T2_esp32_4m_38P-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 2;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[0];

$index++; // The bin file is not created (30p merged with 38p) stays for bin cont.
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T2_esp32_4m_38P-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 3;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[0];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T2_esp32_4m_38P-domoticz-development/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 2;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[0];

$index++; // The bin file is not created (30p merged with 38p) stays for bin cont.
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T2_esp32_4m_38P-domoticz-development/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 3;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[0];

/* HOME ASSISTANT */

// HA API ESP8266 4MB

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T2_8266_4m-ha/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[2];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T2_8266_4m-ha-development/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[2];

// HA API ESP8266 1MB

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T2_8266_1m-ha/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[2];

/*
$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T2_8266_1m-ha-development/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[2];
*/

// HA API ESP8285

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T2_8285-ha/firmware.bin";
$sourceFolder[$index]["chip"] = 8285;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 1;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[2];

// HA API Custom E1

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T2_8266_1m-ha-e1/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 4;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[2];

// HA API ESP32

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T2_esp32_4m_38P-ha/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 2;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[2];

$index++; // The bin file is not created (30p merged with 38p) stays for bin cont.
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T2_esp32_4m_38P-ha/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 3;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[2];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T2_esp32_4m_38P-ha-development/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 2;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[2];

$index++; // The bin file is not created (30p merged with 38p) stays for bin cont.
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T2_esp32_4m_38P-ha-development/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 3;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[2];
 
?>
