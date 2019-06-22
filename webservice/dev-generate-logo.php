<?php

/* THIS IS DEV VERSION, ONCE CODE READY IT SHOULD BE MOVED TO generate-logo.php */
function __autoload($class_name)
{
    require_once "_classes/" . $class_name . ".class.php";
}


require_once "_config.php";


$paypal = new logo();
$paypal->generate();


$location = getLocation();

$_empty = "Null";

$_type = isset($_GET["type"]) ? strlen($_GET["type"]) == 0 ? $_empty : $_GET["type"] : $_empty;
$_version = (isset($_GET["version"]) && !empty($_GET["version"])) ? $_GET["version"] : $_empty;
$_device_id = (isset($_GET["device"]) && !empty($_GET["device"])) ? $_GET["device"] : $_empty;

$db = new database(C_DB_HOST, C_DB_USER, C_DB_PASSWORD, C_DB_NAME);

/* It gets location via IP address */


/* Logs new AFE installed */
function firmwareInstalled($t, $v, $d, $l)
{
    global $db;
    $q = "INSERT INTO " . C_TABLE_PREFIX . "logs (device_id, user_ip,firmware_type, firmware_version,firmware_installed_on, last_seen_on, geo_country_code,geo_country_name,geo_region_code,geo_region_name,geo_zipcode,geo_latitude,geo_longitude,geo_city,geo_areacode,geo_metro_code,geo_time_zone )  VALUES ('" . $d . "','" . C_USER_IP . "','" . $t . "','" . $v . "',CURRENT_TIMESTAMP(),CURRENT_TIMESTAMP(),'" . $l->country_code . "','" . $l->country_name . "','" . $l->region_code . "','" . $l->region_name . "','" . $l->zipcode . "','" . $l->latitude . "','" . $l->longitude . "','" . $l->city . "','" . $l->areacode . "','" . $l->metro_code . "','" . $l->time_zone . "')";
    $db->executeQuery($q);
    $notification = new pushover();
    $notification->send("AFE Firmware", "AFE Firmware installed\nVersion: T" . $t . "/" . $v . "\nUser location: " . $l->country_name . "/" . $l->city);

}

/* Logs AFE upgraded */
function firmwareUpgraded($t, $v, $d, $l)
{
    global $db;
    $q = "UPDATE " . C_TABLE_PREFIX . "logs SET firmware_type = '" . $t . "', firmware_version = '" . $v . "', firmware_upgraded_on = CURRENT_TIMESTAMP(), user_ip = '" . C_USER_IP . "', last_seen_on = CURRENT_TIMESTAMP(), geo_country_code='".$l->country_code."',geo_country_name='".$l->country_name."',geo_region_code='".$l->region_code."',geo_region_name='".$l->region_name."',geo_zipcode='".$l->zipcode."',geo_latitude='".$l->latitude."',geo_longitude='".$l->longitude."',geo_city='".$l->city."',geo_areacode='".$l->areacode."',geo_metro_code='".$l->metro_code."',geo_time_zone='".$l->time_zone."' WHERE device_id = '" . $d . "'";
    $db->executeQuery($q);
}

function lastSeenUpdate($d, $l)
{
    global $db;
    $q = "UPDATE " . C_TABLE_PREFIX . "logs SET user_ip = '" . C_USER_IP . "', last_seen_on = CURRENT_TIMESTAMP(),geo_country_code='".$l->country_code."',geo_country_name='".$l->country_name."',geo_region_code='".$l->region_code."',geo_region_name='".$l->region_name."',geo_zipcode='".$l->zipcode."',geo_latitude='".$l->latitude."',geo_longitude='".$l->longitude."',geo_city='".$l->city."',geo_areacode='".$l->areacode."',geo_metro_code='".$l->metro_code."',geo_time_zone='".$l->time_zone."' WHERE device_id = '" . $d . "'";
    $db->executeQuery($q);
}

function lastSeenUpdateUnknownDevice($l, $f, $t)
{
    global $db;
    $q = "UPDATE " . C_TABLE_PREFIX . "logs SET user_ip = '" . C_USER_IP . "', last_seen_on = CURRENT_TIMESTAMP(),geo_country_code='".$l->country_code."',geo_country_name='".$l->country_name."',geo_region_code='".$l->region_code."',geo_region_name='".$l->region_name."',geo_zipcode='".$l->zipcode."',geo_latitude='".$l->latitude."',geo_longitude='".$l->longitude."',geo_city='".$l->city."',geo_areacode='".$l->areacode."',geo_metro_code='".$l->metro_code."',geo_time_zone='".$l->time_zone."' WHERE user_ip = '" . C_USER_IP . "' AND firmware_version = '" . $f . "' AND firmware_type='" . $t . "' AND device_id = 'Null'";
    $db->executeQuery($q);
}

if ($_device_id != $_empty) {
    $_res = $db->executeQuery("SELECT * FROM " . C_TABLE_PREFIX . "logs WHERE device_id = '" . $_device_id . "'");
    if ($data = $db->fetchRecord($_res)) {
        if ($data["firmware_version"] <> $_version || $data["firmware_type"] <> $_type) {

            firmwareUpgraded($_type, $_version, $_device_id, $location);

            $notification = new pushover();
            $notification->send("AFE Firmware", "AFE Firmware upgraded\nPrevious version: T" . $data["firmware_type"] . "/" . $data["firmware_version"] .
                "\nNew version: T" . $_type . "/" . $_version .
                "\nUser location: " . $location->country_name . "/" . $location->city);

        } else {
            lastSeenUpdate($_device_id, $location);
        }
    } else {
        firmwareInstalled($_type, $_version, $_device_id, $location);
    }
} else {
    $_res = $db->executeQuery("SELECT log_id FROM " . C_TABLE_PREFIX . "logs WHERE user_ip = '" . C_USER_IP . "' AND firmware_version = '" . $_version . "' AND firmware_type='" . $_type . "' AND device_id = '" . $_empty . "'");
    if ($data = $db->fetchRecord($_res)) {
        lastSeenUpdateUnknownDevice($location,$_version,$_type);
    } else {
        firmwareInstalled($_type, $_version, $_device_id, $location);
    }
}

die();

?>
