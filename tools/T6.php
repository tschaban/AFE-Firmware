<?php


/* Set this before run */

$type = "6";
$version = "2.2.0";
$language = "EN";

/******************/

$targetLanguage[0] = "PL";
$targetLanguage[1] = "EN";

$targetAPI[0] = "Domoticz API";
$targetAPI[1] = "Standard API";

$targetFolder = "C:/Users/Adrian/Cloud/Pulpit/AFE.Firmware.T".$type.".".$version;

$targetAPI[0] = "Domoticz API";
$targetAPI[1] = "Standard API";

$targetHardware[0] = "ESP Generic";
$targetHardware[1] = "ESP Generic with debugger";
$targetHardware[2] = "Weather Station v2 (iECS)";

$folderStructure[0] = $targetFolder;
$folderStructure[1] = $targetFolder."/".$targetLanguage;
$folderStructure[2] = $folderStructure[1]."/".$targetLanguage;

// Standard API

$sourceFolder[0]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T6_8266_4m-generic/firmware.bin";
$sourceFolder[0]["chip"] = "ESP8266";
$sourceFolder[0]["size"] = "4Mb";
$sourceFolder[0]["hardware"] = 0;
$sourceFolder[0]["api"] = $targetAPI[1];

$sourceFolder[1]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T6_8266_4m-development/firmware.bin";
$sourceFolder[1]["chip"] = "ESP8266.DEBUG";
$sourceFolder[1]["size"] = "4Mb";
$sourceFolder[1]["hardware"] = 1;
$sourceFolder[1]["api"] = $targetAPI[1];

$sourceFolder[2]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T6_8266_4m-iECSv20/firmware.bin";
$sourceFolder[2]["chip"] = "ESP8266.WeatherStationV2iECS";
$sourceFolder[2]["size"] = "4Mb";
$sourceFolder[2]["hardware"] = 2;
$sourceFolder[2]["api"] = $targetAPI[1];


// Domoticz API

$sourceFolder[3]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T6_8266_4m-generic-domoticz/firmware.bin";
$sourceFolder[3]["chip"] = "ESP8266";
$sourceFolder[3]["size"] = "4Mb";
$sourceFolder[3]["hardware"] = 0;
$sourceFolder[3]["api"] = $targetAPI[0];

$sourceFolder[4]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T6_8266_4m-development-domoticz/firmware.bin";
$sourceFolder[4]["chip"] = "ESP8266.DEBUG";
$sourceFolder[4]["size"] = "4Mb";
$sourceFolder[4]["hardware"] = 1;
$sourceFolder[4]["api"] = $targetAPI[0];

$sourceFolder[5]["file"] = "C:/Users/Adrian/Development/AFE-Firmware/.pio/build/AFE_T6_8266_4m-iECSv20-domoticz/firmware.bin";
$sourceFolder[5]["chip"] = "ESP8266.WeatherStationV2iECS";
$sourceFolder[5]["size"] = "4Mb";
$sourceFolder[5]["hardware"] = 2;
$sourceFolder[5]["api"] = $targetAPI[0];

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
