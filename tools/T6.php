<?php


/* Set this before run */

$type = "6";
$version = "2.5.1";
$language = "EN";

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
$targetHardware[$index++] = "Weather Station v2 (iECS)";
$targetHardware[$index++] = "Weather Station v2.1 (iECS)";

$folderStructure[0] = $targetFolder;
$folderStructure[1] = $targetFolder."/".$targetLanguage;
$folderStructure[2] = $folderStructure[1]."/".$targetLanguage;


$index = 0;

// Standard API

$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T6_8266_4m-generic/firmware.bin";
$sourceFolder[$index]["chip"] = "ESP8266";
$sourceFolder[$index]["size"] = "4Mb";
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["api"] = $targetAPI[1];
$index++;

$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T6_8266_4m-development/firmware.bin";
$sourceFolder[$index]["chip"] = "ESP8266.DEBUG";
$sourceFolder[$index]["size"] = "4Mb";
$sourceFolder[$index]["hardware"] = 1;
$sourceFolder[$index]["api"] = $targetAPI[1];
$index++;

$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T6_8266_4m-iECSv20/firmware.bin";
$sourceFolder[$index]["chip"] = "ESP8266.WeatherStationV2iECS";
$sourceFolder[$index]["size"] = "4Mb";
$sourceFolder[$index]["hardware"] = 2;
$sourceFolder[$index]["api"] = $targetAPI[1];
$index++;

$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T6_8266_4m-iECSv21/firmware.bin";
$sourceFolder[$index]["chip"] = "ESP8266.WeatherStationV2.1iECS";
$sourceFolder[$index]["size"] = "4Mb";
$sourceFolder[$index]["hardware"] = 3;
$sourceFolder[$index]["api"] = $targetAPI[1];
$index++;

$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T6_8266_1m-generic/firmware.bin";
$sourceFolder[$index]["chip"] = "ESP8266";
$sourceFolder[$index]["size"] = "1Mb";
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["api"] = $targetAPI[1];
$index++;

// Domoticz API

$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T6_8266_4m-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = "ESP8266";
$sourceFolder[$index]["size"] = "4Mb";
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["api"] = $targetAPI[0];
$index++;

$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T6_8266_4m-development-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = "ESP8266.DEBUG";
$sourceFolder[$index]["size"] = "4Mb";
$sourceFolder[$index]["hardware"] = 1;
$sourceFolder[$index]["api"] = $targetAPI[0];
$index++;

$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T6_8266_4m-iECSv20-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = "ESP8266.WeatherStationV2iECS";
$sourceFolder[$index]["size"] = "4Mb";
$sourceFolder[$index]["hardware"] = 2;
$sourceFolder[$index]["api"] = $targetAPI[0];
$index++;

$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T6_8266_4m-iECSv21-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = "ESP8266.WeatherStationV2.1iECS";
$sourceFolder[$index]["size"] = "4Mb";
$sourceFolder[$index]["hardware"] = 3;
$sourceFolder[$index]["api"] = $targetAPI[0];
$index++;

$sourceFolder[$index]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T6_8266_1m-domoticz/firmware.bin";
$sourceFolder[$index]["chip"] = "ESP8266";
$sourceFolder[$index]["size"] = "1Mb";
$sourceFolder[$index]["hardware"] = 0;
$sourceFolder[$index]["api"] = $targetAPI[0];

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
