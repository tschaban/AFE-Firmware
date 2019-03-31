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


  $Device = new device($_device_id);

  if (!$Device->found()) {
    $_result["error"] = "Device has not been found in DB";
  }  else {
    $_deviceKeyNumber = $Device->getKeyNumber();
    if ($_deviceKeyNumber==false) {
      $_result["error"] = "Key# has not been set for a device";
    } else {
      $Key = new key($_key);
      if ($Key->found()) {
         if ($Key->isValid()) {
             $_result["status"] = true;
         } else {
             $_result["error"] = "Key# is not valid";
         }
      } else {
         $_result["error"] = "Key# has not been found";
      }
    }
  }

}

$debug->push("JSON: ");
$debug->push(json_encode($_result),false);
if (!$DEBUG) {
  echo json_encode($_result);
}
$debug->push("Completed");

?>
