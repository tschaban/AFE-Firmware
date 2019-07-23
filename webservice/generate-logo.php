<?php

function __autoload($class_name)
{
    require_once "_classes/" . $class_name . ".class.php";
}


require_once "_config.php";


$paypal = new logo();
$paypal->generate();




$_empty = "Null";

$_type = isset($_GET["type"]) ? strlen($_GET["type"]) == 0 ? $_empty : $_GET["type"] : $_empty;
$_version = (isset($_GET["version"]) && !empty($_GET["version"])) ? $_GET["version"] : $_empty;
$_device_id = (isset($_GET["device"]) && !empty($_GET["device"])) ? $_GET["device"] : $_empty;

$db = new database(C_DB_HOST, C_DB_USER, C_DB_PASSWORD, C_DB_NAME);

/* It gets location via IP address */
function getLocation()
{
    return json_decode(file_get_contents('http://api.ipstack.com/' . C_USER_IP. '?access_key=10eb4dab17c25ed10586f7fb23eb2f7c'));
}

/* Logs new AFE installed */
function firmwareInstalled($t, $v, $d)
{
    global $db;
    $location = getLocation();
    $q = "INSERT INTO " . C_TABLE_PREFIX . "logs (device_id, user_ip,AFE_FIRMWARE_TYPE, AFE_FIRMWARE_VERSION,firmware_installed_on,
    last_seen_on, geo_country_code,geo_country_name,geo_region_code,geo_region_name,geo_zipcode,geo_latitude,geo_longitude,geo_city )
     VALUES ('" . $d . "','" . C_USER_IP . "','" . $t . "','" . $v . "',CURRENT_TIMESTAMP(),CURRENT_TIMESTAMP(),'" .
      $location->country_code . "','" . $location->country_name . "','" . $location->region_code . "','" . $location->region_name . "','"
      . $location->zip . "','" . $location->latitude . "','" . $location->longitude . "','" . $location->city . "')";
    $db->executeQuery($q);
    $notification = new pushover();
    $notification->send("AFE Firmware", "AFE Firmware installed\nVersion: T" . $t . "/" . $v . "\nUser location: " . $location->country_name . "/" . $location->city);

}

/* Logs AFE upgraded */
function firmwareUpgraded($t, $v, $d, $oldt, $oldv)
{
    global $db;
    $location = getLocation();
    $q = "UPDATE " . C_TABLE_PREFIX . "logs SET AFE_FIRMWARE_TYPE = '" . $t . "', AFE_FIRMWARE_VERSION = '" . $v . "', firmware_upgraded_on = CURRENT_TIMESTAMP(), user_ip = '" . C_USER_IP . "', last_seen_on = CURRENT_TIMESTAMP(), geo_country_code='".$location->country_code."',geo_country_name='".$location->country_name."',geo_region_code='".$location->region_code."',geo_region_name='".$location->region_name."',geo_zipcode='".$location->zipcode."',geo_latitude='".$location->latitude."',geo_longitude='".$location->longitude."',geo_city='".$location->city."' WHERE device_id = '" . $d . "'";
    $db->executeQuery($q);

    $notification = new pushover();
    $notification->send("AFE Firmware", "AFE Firmware upgraded\nPrevious version: T" . $oldt . "/" . $oldv .
        "\nNew version: T" . $t . "/" . $v .
        "\nUser location: " . $location->country_name . "/" . $location->city);
}

function lastSeenUpdate($d)
{
    global $db;
    $q = "UPDATE " . C_TABLE_PREFIX . "logs SET user_ip = '" . C_USER_IP . "', last_seen_on = CURRENT_TIMESTAMP() WHERE device_id = '" . $d . "'";
    $db->executeQuery($q);
}

function lastSeenUpdateUnknownDevice($l, $f, $t)
{
    global $db;
    $location = getLocation();
    $q = "UPDATE " . C_TABLE_PREFIX . "logs SET user_ip = '" . C_USER_IP . "', last_seen_on = CURRENT_TIMESTAMP(),geo_country_code='".$location->country_code."',geo_country_name='".$location->country_name."',geo_region_code='".$location->region_code."',geo_region_name='".$location->region_name."',geo_zipcode='".$location->zipcode."',geo_latitude='".$location->latitude."',geo_longitude='".$location->longitude."',geo_city='".$location->city."' WHERE user_ip = '" . C_USER_IP . "' AND AFE_FIRMWARE_VERSION = '" . $f . "' AND AFE_FIRMWARE_TYPE='" . $t . "' AND device_id = 'Null'";
    $db->executeQuery($q);
}

if ($_device_id != $_empty) {
    $_res = $db->executeQuery("SELECT * FROM " . C_TABLE_PREFIX . "logs WHERE device_id = '" . $_device_id . "'");
    if ($data = $db->fetchRecord($_res)) {
        if ($data["AFE_FIRMWARE_VERSION"] <> $_version || $data["AFE_FIRMWARE_TYPE"] <> $_type) {

            firmwareUpgraded($_type, $_version, $_device_id,$data["AFE_FIRMWARE_TYPE"],$data["AFE_FIRMWARE_VERSION"]);

        } else {
            lastSeenUpdate($_device_id);
        }
    } else {
        firmwareInstalled($_type, $_version, $_device_id);
    }
} else {
    $_res = $db->executeQuery("SELECT log_id FROM " . C_TABLE_PREFIX . "logs WHERE user_ip = '" . C_USER_IP . "' AND AFE_FIRMWARE_VERSION = '" . $_version . "' AND AFE_FIRMWARE_TYPE='" . $_type . "' AND device_id = '" . $_empty . "'");
    if ($data = $db->fetchRecord($_res)) {
        lastSeenUpdateUnknownDevice($_version,$_type);
    } else {
        firmwareInstalled($_type, $_version, $_device_id);
    }
}

die();

?>
