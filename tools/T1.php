<?php


$targetLanguage[0] = "pl";
$targetLanguage[1] = "en";

$targetAPI[0] = "domoticz.api";
$targetAPI[1] = "standard.api";

$rootPath = "C:/Users/Adrian/Cloud/Pulpit/";
$targetFolder = $rootPath."afe.firmware.t".$type.".".$version;

$index=0;
$targetHardware[$index][0] = "esp-generic";
$targetHardware[$index][1] = 0;

$index++;
$targetHardware[$index][0] = "esp-generic-debugger";
$targetHardware[$index][1] = 0;

$index++;
$targetHardware[$index][0] = "mega-pack";
$targetHardware[$index][1] = 10;

$index++;
$targetHardware[$index][0] = "custom-e1";
$targetHardware[$index][1] = 11;



$folderStructure[0] = $targetFolder;
$folderStructure[1] = $targetFolder."/".$targetLanguage;
$folderStructure[2] = $folderStructure[1]."/".$targetLanguage;

// Standard API

$index=0;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T1_8266_1m-generic/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[1];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T1_8266_1m-generic-development/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 1;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[1];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T1_8266_1m-generic-e1/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 3;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[1];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T1_8266_1m-generic-mega/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 2;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[1];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T1_8266_4m-generic/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[1];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T1_8266_4m-generic-development/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 1;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[1];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T1_8266_4m-generic-mega/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 2;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[1];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T1_8285_1m-generic/firmware.bin";
$sourceFolder[$index]["chip"] = 8285;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[1];

// Domoticz API

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T1_8266_1m-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[0];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T1_8266_1m-domoticz-development/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 1;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[0];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T1_8266_1m-domoticz-e1/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 3;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[0];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T1_8266_1m-domoticz-mega/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 2;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[0];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T1_8266_4m-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[0];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T1_8266_4m-domoticz-development/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 1;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[0];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T1_8266_4m-domoticz-mega/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 2;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[0];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T1_8285_1m-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = 8285;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[0];


?>
