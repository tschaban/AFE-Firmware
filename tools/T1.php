<?php


/* Set this before run */

$type = "1";
$version = "2.3.0.B4";
$language = "PL";

/******************/

$targetLanguage[0] = "PL";
$targetLanguage[1] = "EN";

$targetAPI[0] = "Domoticz API";
$targetAPI[1] = "Standard API";

$targetFolder = "C:/Users/Adrian/Cloud/Pulpit/AFE.Firmware.T".$type.".".$version;

$targetAPI[0] = "Domoticz API";
$targetAPI[1] = "Standard API";

$index = 0;

$targetHardware[$index++] = "ESP Generic";
$targetHardware[$index++] = "ESP Generic with debugger";
$targetHardware[$index++] = "Sonoff 4CH and 4CH Pro";
$targetHardware[$index++] = "Sonoff Basic";
$targetHardware[$index++] = "Sonoff Touch";
//$targetHardware[2] = "Shelly-1";


$folderStructure[0] = $targetFolder;
$folderStructure[1] = $targetFolder."/".$targetLanguage;
$folderStructure[2] = $folderStructure[1]."/".$targetLanguage;

// Standard API

$index = 0;

$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T1_8266_1m-generic/firmware.bin";
$sourceFolder[$index]["chip"] = "ESP8266";
$sourceFolder[$index]["size"] = "1Mb";
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["api"] = $targetAPI[1];
$index++;

$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T1_8266_1m-generic-development/firmware.bin";
$sourceFolder[$index]["chip"] = "ESP8266.DEBUG";
$sourceFolder[$index]["size"] = "1Mb";
$sourceFolder[$index]["hardware"] = 1;
$sourceFolder[$index]["api"] = $targetAPI[1];
$index++;


$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T1_8285_1m-generic/firmware.bin";
$sourceFolder[$index]["chip"] = "ESP8285";
$sourceFolder[$index]["size"] = "1Mb";
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["api"] = $targetAPI[1];
$index++;

$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T1_8266_1m_Sonoff-Basic-V1/firmware.bin";
$sourceFolder[$index]["chip"] = "ESP8266.SonoffBasic";
$sourceFolder[$index]["size"] = "1Mb";
$sourceFolder[$index]["hardware"] = 4;
$sourceFolder[$index]["api"] = $targetAPI[1];
$index++;

$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T1_8285_1m_Sonoff-Basic-V1/firmware.bin";
$sourceFolder[$index]["chip"] = "ESP8285.SonoffBasic";
$sourceFolder[$index]["size"] = "1Mb";
$sourceFolder[$index]["hardware"] = 4;
$sourceFolder[$index]["api"] = $targetAPI[1];
$index++;

$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T1_8285_1m_Sonoff-4CH/firmware.bin";
$sourceFolder[$index]["chip"] = "ESP8285.Sonoff4CH";
$sourceFolder[$index]["size"] = "1Mb";
$sourceFolder[$index]["hardware"] = 3;
$sourceFolder[$index]["api"] = $targetAPI[1];
$index++;

$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T1_8285_1m_Sonoff-Touch-1G/firmware.bin";
$sourceFolder[$index]["chip"] = "ESP8285.SonoffTouchG1";
$sourceFolder[$index]["size"] = "1Mb";
$sourceFolder[$index]["hardware"] = 5;
$sourceFolder[$index]["api"] = $targetAPI[1];
$index++;

$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T1_8285_1m_Sonoff-Touch-2G/firmware.bin";
$sourceFolder[$index]["chip"] = "ESP8285.SonoffTouchG2";
$sourceFolder[$index]["size"] = "1Mb";
$sourceFolder[$index]["hardware"] = 5;
$sourceFolder[$index]["api"] = $targetAPI[1];
$index++;

$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T1_8285_1m_Sonoff-Touch-3G/firmware.bin";
$sourceFolder[$index]["chip"] = "ESP8285.SonoffTouchG3";
$sourceFolder[$index]["size"] = "1Mb";
$sourceFolder[$index]["hardware"] = 5;
$sourceFolder[$index]["api"] = $targetAPI[1];
$index++;

/*$sourceFolder[9]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T1_Shelly-1/firmware.bin";
$sourceFolder[9]["chip"] = "ESP8266.Shelly1";
$sourceFolder[9]["size"] = "2Mb";
$sourceFolder[9]["hardware"] = 2;
$sourceFolder[9]["api"] = $targetAPI[1];
*/
// Domoticz API

$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T1_8266_1m-generic-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = "ESP8266";
$sourceFolder[$index]["size"] = "1Mb";
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["api"] = $targetAPI[0];
$index++;

$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T1_8266_1m-generic-domoticz-development/firmware.bin";
$sourceFolder[$index]["chip"] = "ESP8266.DEBUG";
$sourceFolder[$index]["size"] = "1Mb";
$sourceFolder[$index]["hardware"] = 1;
$sourceFolder[$index]["api"] = $targetAPI[0];
$index++;

$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T1_8285_1m-generic-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = "ESP8285";
$sourceFolder[$index]["size"] = "1Mb";
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["api"] = $targetAPI[0];
$index++;

$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T1_8266_1m_Sonoff-Basic-V1-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = "ESP8266.SonoffBasic";
$sourceFolder[$index]["size"] = "1Mb";
$sourceFolder[$index]["hardware"] = 4;
$sourceFolder[$index]["api"] = $targetAPI[0];
$index++;

$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T1_8285_1m_Sonoff-Basic-V1-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = "ESP8285.SonoffBasic";
$sourceFolder[$index]["size"] = "1Mb";
$sourceFolder[$index]["hardware"] = 4;
$sourceFolder[$index]["api"] = $targetAPI[0];
$index++;

$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T1_8285_1m_Sonoff-4CH-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = "ESP8285.Sonoff4CH";
$sourceFolder[$index]["size"] = "1Mb";
$sourceFolder[$index]["hardware"] = 3;
$sourceFolder[$index]["api"] = $targetAPI[0];
$index++;

$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T1_8285_1m_Sonoff-Touch-1G-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = "ESP8285.SonoffTouchG1";
$sourceFolder[$index]["size"] = "1Mb";
$sourceFolder[$index]["hardware"] = 5;
$sourceFolder[$index]["api"] = $targetAPI[0];
$index++;

$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T1_8285_1m_Sonoff-Touch-2G-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = "ESP8285.SonoffTouchG2";
$sourceFolder[$index]["size"] = "1Mb";
$sourceFolder[$index]["hardware"] = 5;
$sourceFolder[$index]["api"] = $targetAPI[0];
$index++;

$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T1_8285_1m_Sonoff-Touch-3G-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = "ESP8285.SonoffTouchG3";
$sourceFolder[$index]["size"] = "1Mb";
$sourceFolder[$index]["hardware"] = 5;
$sourceFolder[$index]["api"] = $targetAPI[0];
$index++;
/*
$sourceFolder[19]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T1_Shelly-1-domoticz/firmware.bin";
$sourceFolder[19]["chip"] = "ESP8266.Shelly1";
$sourceFolder[19]["size"] = "2Mb";
$sourceFolder[19]["hardware"] = 2;
$sourceFolder[19]["api"] = $targetAPI[0];
*/

echo "\nCreating folders structure";
if (!file_exists($targetFolder)) {
    mkdir($targetFolder);
}

foreach ($targetLanguage as &$targetLanguageFolder) {
    if (!file_exists($targetFolder."/".$targetLanguageFolder)) {
        mkdir($targetFolder."/".$targetLanguageFolder);
    }
    foreach ($targetAPI as &$targetAPIFolder) {
        if (!file_exists($targetFolder."/".$targetLanguageFolder."/".$targetAPIFolder)) {
            mkdir($targetFolder."/".$targetLanguageFolder."/".$targetAPIFolder);
        }
        foreach ($targetHardware as &$targetHardwareFolder) {
            if (!file_exists($targetFolder."/".$targetLanguageFolder."/".$targetAPIFolder."/".$targetHardwareFolder)) {
                mkdir($targetFolder."/".$targetLanguageFolder."/".$targetAPIFolder."/".$targetHardwareFolder);                              
            }        
            $finalFolder[$targetLanguageFolder][$targetAPIFolder][$targetHardwareFolder] = $targetFolder."/".$targetLanguageFolder."/".$targetAPIFolder."/".$targetHardwareFolder;
        }    
    }
}


echo "\nCoping firmwares";

foreach ($sourceFolder as &$source) {
    $sourceToCopy = $source["file"];
    $fileName = "AFE.Firmware.T".$type.".".$version.".".$language.".".$source["chip"].".".$source["size"].".bin";
    $copyTo = $finalFolder[$language][$source["api"]][$targetHardware[$source["hardware"]]]."/".$fileName ;
     
    if (file_exists($sourceToCopy)) {
        rename($sourceToCopy, $copyTo);
        echo "\nSUCCESS: " . $fileName;
    } else {
        echo "\nERROR: File doesn't exist:  " . $sourceToCopy;
    }
  
    

}

?>
