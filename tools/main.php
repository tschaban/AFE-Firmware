<?php


/* Set this before run */

$type = "6";
$version = "3.7.0";
$language = "en";
$development = false;


/**
 * Main
 */

include("T".$type.".php");

function copyFolder($src, $dst) { 
  
    // open the source directory
    $dir = opendir($src); 
  
    // Make the destination directory if not exist
    @mkdir($dst); 
  
    // Loop through the files in source directory
    while( $file = readdir($dir) ) { 
  
        if (( $file != '.' ) && ( $file != '..' )) { 
            if ( is_dir($src . '/' . $file) ) 
            { 
  
                // Recursively calling custom copy function
                // for sub directory 
                copy_folder($src . '/' . $file, $dst . '/' . $file); 
  
            } 
            else { 
                copy($src . '/' . $file, $dst . '/' . $file); 
            } 
        } 
    } 
  
    closedir($dir);
} 



function createdIndexFile($path) {
    $handle = fopen($path."/index.html", "w");
    fclose($handle);
}

echo "\nCreating folders structure";
if (!file_exists($targetFolder)) {
    mkdir($targetFolder);
    createdIndexFile($targetFolder);
    echo "\nCopying bootloaders";
    mkdir($targetFolder."/esp32.bootloaders");
    copyFolder("esp32.bootloaders", $targetFolder."/esp32.bootloaders");
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

/**
 * Does not generate update for development version
 */
if (!$development) {
  fwrite($handle, "UPDATE afe_firmwares set current_version = 0 WHERE type = ".$type." AND language = '".$language."';\n");
}
echo "\nCoping firmwares";

foreach ($sourceFolder as &$source) {
    $sourceToCopy = $source["file"];
    $fileName = "afe.firmware.t".$type.".".$version.".".$language.".esp".$source["chip"].".". ($targetHardware[$source["hardware"]][2]?$targetHardware[$source["hardware"]][2].".":"") . $source["size"]."mb".($source["debug"]?".debug":"").".bin";
    $copyTo = $finalFolder[$language][$source["api"]][$targetHardware[$source["hardware"]][0]]."/".$fileName ;
     
    if (file_exists($sourceToCopy)) {
        copy($sourceToCopy, $copyTo);
        echo "\nSUCCESS: " . $fileName;
        fwrite($handle, "INSERT INTO afe_firmwares (type,version,chip,language,api,hardware,flash_size,current_version,downloaded,debug,path) VALUES (".$type.", '".$version."', ".$source["chip"].", '".$language."', '".($source["api"]==$targetAPI[0]?"D":($source["api"]==$targetAPI[1]?"S":"H"))."', ".$targetHardware[$source["hardware"]][1].", ".$source["size"].", ".($development?2:1).", 0, ".($source["debug"]?1:0).", '".str_replace($rootPath,"",$copyTo)."');\n");
        
    } else {
        echo "\nERROR: File doesn't exist:  " . $sourceToCopy;
    }
}

fclose($handle); 
?>
