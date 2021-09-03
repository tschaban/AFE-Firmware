<?php

/* Set this before run */
$version = "3.0.3.B1";
$language = "en";
//$language = "pl";

/******************/
$type = "0";

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
30 ESP32 30Pins
31 ESP32 38Pins

### HARDWARE ID ###

0  : ESP8266
1  : ESP8285
2  : ESP32-30p
3  : ESP32-38p
4  : Shelly-1
5  : Sonoff Basic
6  : Sonoff 4CH
7  : Sonoff Touch 1
8  : Sonoff Touch 2
9  : Sonoff Touch 3

*/





$targetHardware[0][0] = "esp8266";  // Folder name
$targetHardware[0][1] = 0;          // AFE_DEVICE_ID
$targetHardware[0][2] = "generic";  // Part of the file

$targetHardware[1][0] = "esp8285";
$targetHardware[1][1] = 0; 
$targetHardware[1][2] = "generic    ";

$targetHardware[2][0] = "esp32";
$targetHardware[2][1] = 30; 
$targetHardware[2][2] = "30pins";

$targetHardware[3][0] = "esp32";
$targetHardware[3][1] = 31; 
$targetHardware[3][2] = "38pins";

$targetHardware[4][0] = "shelly";
$targetHardware[4][1] = 20;
$targetHardware[4][2] = "shelly-1";

$targetHardware[5][0] = "sonoff";
$targetHardware[5][1] = 1;
$targetHardware[5][2] = "sonoff-basic";

$targetHardware[6][0] = "sonoff";
$targetHardware[6][1] = 2;
$targetHardware[6][2] = "sonoff-4ch";

$targetHardware[7][0] = "sonoff";
$targetHardware[7][1] = 6;
$targetHardware[7][2] = "sonoff-touch-g1";

$targetHardware[8][0] = "sonoff";
$targetHardware[8][1] = 7;
$targetHardware[8][2] = "sonoff-touch-g2";

$targetHardware[9][0] = "sonoff";
$targetHardware[9][1] = 8;
$targetHardware[9][2] = "sonoff-touch-g3";

$folderStructure[0] = $targetFolder;
$folderStructure[1] = $targetFolder."/".$targetLanguage;
$folderStructure[2] = $folderStructure[1]."/".$targetLanguage;

// Standard API ESP8266

$index;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T0_8266_4m-generic/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[1];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T0_8266_4m-generic-development/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[1];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T0_8266_1m-generic/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[1];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T0_8266_1m-development/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[1];

// Standard API ESP8285 

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T0_8285_1m-generic/firmware.bin";
$sourceFolder[$index]["chip"] = 8285;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 1;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[1];

// Standard API Sonoffs

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T0_8266_1m_Sonoff-Basic-V1/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 5;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[1];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T0_8285_1m_Sonoff-Basic-V1/firmware.bin";
$sourceFolder[$index]["chip"] = 8285;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 5;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[1];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T0_8285_1m_Sonoff-4CH/firmware.bin";
$sourceFolder[$index]["chip"] = 8285;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 6;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[1];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T0_8285_1m_Sonoff-Touch-1G/firmware.bin";
$sourceFolder[$index]["chip"] = 8285;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 7;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[1];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T0_8285_1m_Sonoff-Touch-2G/firmware.bin";
$sourceFolder[$index]["chip"] = 8285;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 8;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[1];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T0_8285_1m_Sonoff-Touch-3G/firmware.bin";
$sourceFolder[$index]["chip"] = 8285;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 9;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[1];

// Standard API Shelly

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T0_Shelly-1/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 2;
$sourceFolder[$index]["hardware"] = 4;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[1];

// Standard API ESP32

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T0_esp32_4m_30P-generic/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 2;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[1];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T0_esp32_4m_38P-generic/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 3;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[1];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T0_esp32_4m_30P-generic-development/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 2;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[1];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T0_esp32_4m_38P-generic-development/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 3;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[1];



// Domoticz API ESP8266

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T0_8266_4m-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[0];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T0_8266_4m-domoticz-development/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[0];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T0_8266_1m-generic-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[0];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T0_8266_1m-development-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[0];


// Domoticz API ESP8285

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T0_8285_1m-generic-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = 8285;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 1;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[0];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T0_8266_1m_Sonoff-Basic-V1-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 5;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[0];

// Domoticz API Sonoffs

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T0_8285_1m_Sonoff-Basic-V1-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = 8285;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 5;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[0];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T0_8285_1m_Sonoff-4CH-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = 8285;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 6;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[0];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T0_8285_1m_Sonoff-Touch-1G-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = 8285;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 7;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[0];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T0_8285_1m_Sonoff-Touch-2G-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = 8285;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 8;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[0];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T0_8285_1m_Sonoff-Touch-3G-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = 8285;
$sourceFolder[$index]["size"] = 1;
$sourceFolder[$index]["hardware"] = 9;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[0];

// Domoticz API Shelly

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T0_Shelly-1-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = 8266;
$sourceFolder[$index]["size"] = 2;
$sourceFolder[$index]["hardware"] = 4;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[0];

// Domoticz API ESP32

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T0_esp32_4m_30P-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 2;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[0];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T0_esp32_4m_38P-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 3;
$sourceFolder[$index]["debug"] = false;
$sourceFolder[$index]["api"] = $targetAPI[0];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T0_esp32_4m_30P-domoticz-development/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 2;
$sourceFolder[$index]["debug"] = true;
$sourceFolder[$index]["api"] = $targetAPI[0];

$index++;
$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T0_esp32_4m_38P-domoticz-development/firmware.bin";
$sourceFolder[$index]["chip"] = 32;
$sourceFolder[$index]["size"] = 4;
$sourceFolder[$index]["hardware"] = 3;
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
    $fileName = "afe.firmware.t".$type.".".$version.".".$language.".esp".$source["chip"].".". $targetHardware[$source["hardware"]][2] . "." . $source["size"]."mb".($source["debug"]?".debug":"").".bin";
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
