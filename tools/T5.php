<?php


/* Set this before run */

$type = "5";
$version = "3.1.0";
$language = "en";

/******************/

$targetLanguage[0] = "pl";
$targetLanguage[1] = "en";

$targetAPI[0] = "domoticz.api";
$targetAPI[1] = "standard.api";

$rootPath = "C:/Users/Adrian/Cloud/Pulpit/";
$targetFolder = $rootPath."afe.firmware.t".$type.".".$version;

/* 
### AFE_DEVICE_ID ###

0 ESP Generic
1 Sonoff Basic V1
2 Sonoff 4CH
3 Sonoff Dual R2
4 Sonoff TH10
5 Sonoff TH16
6 Sonoff Touch 1G
7 Sonoff Touch 2G
8 Sonoff Touch 3G
20 Shelly 1
21 iECS Gate Controller v2
22 iECS Weather Station v2
23 iECS Weather Station v2.1
24 iECS Gate Driver v3
10 T1 Mega pack
11 T1 E1 Custom
12 T5 E1 Custom
30 ESP32 30Pins
31 ESP32 38Pins

### HARDWARE ID ###

0  : ESP8266
1  : ESP8285
2  : iECS v2
3  : iECS v3
4  : ESP32-30p
5  : ESP32-38p
6  : E1 ESP32-30p
7  : E1 ESP32-38p
8  : E1 ESP8266


*/

$index=0;
$targetHardware[$index][0] = "esp8266";
$targetHardware[$index][1] = 0;
$targetHardware[$index][2] = "";

$index++; // 1
$targetHardware[$index][0] = "esp8285";
$targetHardware[$index][1] = 0;
$targetHardware[$index][2] = "";

$index++; // 2
$targetHardware[$index][0] = "iecs";
$targetHardware[$index][1] = 21;
$targetHardware[$index][2] = "iecs-gate-driver-v2";

$index++; // 3
$targetHardware[$index][0] = "iecs";
$targetHardware[$index][1] = 24;
$targetHardware[$index][2] = "iecs-gate-driver-v3";

$index++; // 4
$targetHardware[$index][0] = "esp32";
$targetHardware[$index][1] = 30;
$targetHardware[$index][2] = "30pins";

$index++;
$targetHardware[$index][0] = "esp32";
$targetHardware[$index][1] = 31;
$targetHardware[$index][2] = "38pins";

$index++;
$targetHardware[$index][0] = "custom";
$targetHardware[$index][1] = 12; // ESP32-30p
$targetHardware[$index][2] = "e1.30pins";

$index++;
$targetHardware[$index][0] = "custom";
$targetHardware[$index][1] = 13; // ESP32-38p
$targetHardware[$index][2] = "e1.38pins";

$index++;
$targetHardware[$index][0] = "custom";
$targetHardware[$index][1] = 14; // ESP8266
$targetHardware[$index][2] = "e1";


$folderStructure[0] = $targetFolder;
$folderStructure[1] = $targetFolder."/".$targetLanguage;
$folderStructure[2] = $folderStructure[1]."/".$targetLanguage;

// Standard API

$index=0;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T5_8266_1m-generic/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[1];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T5_8266_4m-generic/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[1];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T5_8266_4m-generic-development/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[1];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T5_8285_1m-generic/firmware.bin";
$sourceFolder[$index]["chip"] = 8285;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 1;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[1];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T5_8266_4m-iECSv20-generic/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 2;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[1];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T5_8266_4m-iECS-Gate-Driver-generic/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 3;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[1];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T5_esp32_4m_30P-generic/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 4;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[1];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T5_esp32_4m_30P-generic-development/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 4;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[1];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T5_esp32_4m_38P-generic/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 5;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[1];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T5_esp32_4m_38P-generic-development/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 5;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[1];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T5_esp32_4m_30P-generic-e1/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 6;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[1];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T5_esp32_4m_30P-generic-e1-development/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 6;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[1];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T5_esp32_4m_38P-generic-e1/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 7;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[1];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T5_esp32_4m_38P-generic-e1-development/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 7;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[1];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T5_8266_4m-generic-e1/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 8;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[1];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T5_8266_4m-generic-e1-development/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 8;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[1];


// Domoticz API

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T5_8266_1m-generic/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[0];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T5_8266_4m-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[0];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T5_8266_4m-domoticz-development/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[0];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T5_8285_1m-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = 8285;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 1;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[0];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T5_8266_4m-iECSv20-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 2;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[0];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T5_8266_4m-iECS-Gate-Driver-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 3;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[0];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T5_esp32_4m_30P-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 4;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[0];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T5_esp32_4m_30P-domoticz-development/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 4;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[0];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T5_esp32_4m_38P-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 5;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[0];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T5_esp32_4m_38P-domoticz-development/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 5;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[0];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T5_esp32_4m_30P-domoticz-e1/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 6;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[0];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T5_esp32_4m_30P-domoticz-e1-development/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 6;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[0];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T5_esp32_4m_38P-domoticz-e1/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 7;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[0];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T5_esp32_4m_38P-domoticz-e1-development/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 7;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[0];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T5_8266_4m-domoticz-e1/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 8;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[0];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T5_8266_4m-domoticz-e1-development/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 8;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[0];




function createdIndexFile($path) {
    $handle = fopen($path."/index.html", "w");
    fclose($handle);
}

echo "\nCreating folders structure";
if (!file_exists($targetFolder)) {
    mkdir($targetFolder);
    createdIndexFile($targetFolder);
}

foreach ($targetLanguage as &$targetLanguageFolder) {
    if (!file_exists($targetFolder."/".$targetLanguageFolder)) {
        mkdir($targetFolder."/".$targetLanguageFolder);
        createdIndexFile($targetFolder."/".$targetLanguageFolder);
    }
    foreach ($targetAPI as &$targetAPIFolder) {
        if (!file_exists($targetFolder."/".$targetLanguageFolder."/".$targetAPIFolder)) {
            mkdir($targetFolder."/".$targetLanguageFolder."/".$targetAPIFolder);
            createdIndexFile($targetFolder."/".$targetLanguageFolder."/".$targetAPIFolder);
        }
        foreach ($targetHardware as &$targetHardwareFolder) {
            if (!file_exists($targetFolder."/".$targetLanguageFolder."/".$targetAPIFolder."/".$targetHardwareFolder[0])) {
                mkdir($targetFolder."/".$targetLanguageFolder."/".$targetAPIFolder."/".$targetHardwareFolder[0]);
                createdIndexFile($targetFolder."/".$targetLanguageFolder."/".$targetAPIFolder."/".$targetHardwareFolder[0]);                              
            }        
            $finalFolder[$targetLanguageFolder][$targetAPIFolder][$targetHardwareFolder[0]] = $targetFolder."/".$targetLanguageFolder."/".$targetAPIFolder."/".$targetHardwareFolder[0];
        }    
    }
}


$handle = fopen($targetFolder."/script-".$language.".sql", "a");
fwrite($handle, "UPDATE afe_firmwares set current_version = 0 WHERE type = ".$type." AND language = '".$language."';\n");

echo "\nCoping firmwares";

foreach ($sourceFolder as &$source) {
    $sourceToCopy = $source["file"];
    $fileName = "afe.firmware.t".$type.".".$version.".".$language.".esp".$source["chip"].".". ($targetHardware[$source["hardware"]][2]?$targetHardware[$source["hardware"]][2].".":"") . $source["size"]."mb".($source["debug"]?".debug":"").".bin";
    $copyTo = $finalFolder[$language][$source["api"]][$targetHardware[$source["hardware"]][0]]."/".$fileName ;
     
    if (file_exists($sourceToCopy)) {
        copy($sourceToCopy, $copyTo);
        echo "\nSUCCESS: " . $fileName;
        fwrite($handle, "INSERT INTO afe_firmwares (type,version,chip,language,api,hardware,flash_size,current_version,downloaded,debug,path) VALUES (".$type.", '".$version."', ".$source["chip"].", '".$language."', '".($source["api"]==$targetAPI[0]?"D":"S")."', ".$targetHardware[$source["hardware"]][1].", ".$source["size"].", 1, 0, ".($source["debug"]?1:0).", '".str_replace($rootPath,"",$copyTo)."');\n");
        
    } else {
        echo "\nERROR: File doesn't exist:  " . $sourceToCopy;
    }
}

fclose($handle); 
?>
