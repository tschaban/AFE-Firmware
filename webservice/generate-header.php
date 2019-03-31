<?php

$DEBUG = false;

function __autoload($class_name)
{
    require_once "_classes/" . $class_name . ".class.php";
}


$debug = new debugger($DEBUG);
$debug->push("Starting script");

require_once "_config.php";


/* Generating logo */
if (!$DEBUG) {
  $_headerPicture = new logo();
  $_headerPicture->generate();
}


$_empty = "Null";

/* Reading entry params */
$_type = isset($_GET["firmwareType"]) ? strlen($_GET["firmwareType"]) == 0 ? $_empty : $_GET["firmwareType"] : $_empty;
$_version = (isset($_GET["firmwareVersion"]) && !empty($_GET["firmwareVersion"])) ? $_GET["firmwareVersion"] : $_empty;
$_device_id = (isset($_GET["deviceID"]) && !empty($_GET["deviceID"])) ? $_GET["deviceID"] : $_empty;

$debug->push("- Firmware type: " . $_type);
$debug->push("- Firmware version: " . $_version);
$debug->push("- Firmware device ID: " . $_device_id);

/* Ending code if input parameters are empty */
if ($_type=="Null" || $_version == "Null" || $_device_id=="Null") {
  die();
}


$db = new database(C_DB_HOST, C_DB_USER, C_DB_PASSWORD, C_DB_NAME);
$debug->push("Connected to DB");


$debug->push("All classes initialized");
$debug->push("Processing request");

$Device = new device($_device_id);

if ($Device->found()) { /* Device has been found */
  if ($Device->checkIfUpgraded($_type,$_version)) { /* Checking if firmware version has been upgraded */
    $Device->updateVersion($_type,$_version);
    $Notification = new pushover();
    $Notification->send("AFE Firmware", "Upgraded: to version: T" . $_type . "/" . $_version);
  } else { /* Updating last Seen date */
    $Device->updateLastSeenDate();
  }
} else { /* Adding new device to DB */
  $Device->add($_type,$_version);
  $Location = new localization(C_USER_IP);
  $Location->add($_device_id);
  $Notification = new pushover();
  $Notification->send("AFE Firmware", "Installed: T" . $_type . "/" . $_version
  . "\nUser location: " . $Location->getCountryCode() . "/" . $Location->getCity());
}


$debug->push("Completed");
die();
?>
