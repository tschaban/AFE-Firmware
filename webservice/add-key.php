<?php

$DEBUG = false;

if (!$DEBUG) {
  header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");
  header("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT");
  header("Access-Control-Allow-Methods: GET, POST, PUT, DELETE");
  header("Access-Control-Allow-Headers: X-Requested-With");
  header("X-Powered-By: Service api.smartnydom.pl");
  header("Content-type:application/json");
  header("Cache-Control: no-store, no-cache, must-revalidate");
  header("Cache-Control: post-check=0, pre-check=0", false);
  header("Pragma: no-cache");
}

function __autoload($class_name)
{
    require_once "_classes/" . $class_name . ".class.php";
}

$debug = new debugger($DEBUG);
$debug->push("Starting script");

require_once "_config.php";

$_empty = "Null";

/* Reading entry params */
$_key = isset($_GET["keyNumber"]) ? strlen($_GET["keyNumber"]) == 0 ? $_empty : $_GET["keyNumber"] : $_empty;
$_device_id = (isset($_GET["deviceID"]) && !empty($_GET["deviceID"])) ? $_GET["deviceID"] : $_empty;

$debug->push("- Key number: " . $_key);
$debug->push("- Device ID: " . $_device_id);
$_result["status"] = false;

/* Ending code if input parameters are empty */
if ($_key=="Null" || $_device_id == "Null") {
  $_result["error"] = "Missing input parameters: key# or device ID";
} else {

  $db = new database(C_DB_HOST, C_DB_USER, C_DB_PASSWORD, C_DB_NAME);
  $debug->push("Connected to DB");

  $debug->push("All classes initialized");
  $debug->push("Processing request");


 $Key = new key($_key);
 if ($Key->found()) { // Key exists
   if ($Key->isValid()) { // Key is valid
      $Device = new device($_device_id);
      if ($Device->found()) { // Device exist
        if ($Device->getKeyNumber()==false || ($Device->getKeyNumber()!=false && $Device->getKeyNumber()!=$_key)) { // Adding new key
          $debug->push("Adding/Updating key");
          $_key_devices_limit = $Key->getLimits();
          $_keys_used = $Key->usedBy();
          if ($_key_devices_limit>$_keys_used) { // enought limits per key?
            $Device->updateKeyNumber($_key); // adding key
            $_result["status"] = true;
          } else {
            $_result["error"] = "Exceeded number of key usage";
          }
        } else { // Device has already this key
          $_result["status"] = true;
        }
      } else {
        $_result["error"] = "Device has not been found";
      }
   } else { // Key is not valid
     $_result["error"] = "Key# is not valid";
   }
 } else { // Key doesn't exist
   $_result["error"] = "Key# has not been found";
 }
}

$debug->push("JSON: ");
$debug->push(json_encode($_result),false);
if (!$DEBUG) {
  echo json_encode($_result);
}

$debug->push("Completed");

$debug->push("Sending Pushover");
$Notification = new pushover();
$Notification->send("New davice added\nKey", "D:".$_device_id."\nK:".$_key."\nStatus: ".$_result["error"]);
$debug->push(" - done");


?>
