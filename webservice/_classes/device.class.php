<?php
if (stripos($_SERVER['PHP_SELF'], "device.class.php") == true) die();

class device
{

    private $_id;
    private $_db;
    private $_data;
    private $_debug;
    private $_deviceInDB = false;

    public function __construct($id)
    {
        global $db, $debug;
        $this->_db = $db;
        $this->_id = $id;
        $this->_debug = $debug;

        $debug->push("- Looking for a device: " . $this->_id);
        $_res = $this->_db->executeQuery("SELECT * FROM " . C_TABLE_PREFIX . "devices WHERE id = '" . $this->_id . "'");
        if ($this->_data = $this->_db->fetchRecord($_res)) {
            $this->_debug->push("- Device found in the DB");
            $this->_deviceInDB = true;
        } else {
            $this->_debug->push("- Device NOT found in the DB");
            $this->_deviceInDB = false;
        }
    }

    public function found() {
      return $this->_deviceInDB;
    }

    public function add($type, $version, $deviceType)
    {
        $query = "INSERT INTO " . C_TABLE_PREFIX . "devices (id, firmware_type, firmware_version, firmware_installed_on,
        last_seen_on, device_type_id)  VALUES ('" . $this->_id . "','" . $type . "','" . $version . "',CURRENT_TIMESTAMP(),CURRENT_TIMESTAMP(),".$deviceType.")";
        $this->_deviceInDB = $this->_db->executeQuery($query);
        return $this->_deviceInDB;
    }

    public function updateLastSeenDate() {
      $this->_debug->push("Updating last seen date");
      $q = "UPDATE " . C_TABLE_PREFIX . "devices SET last_seen_on = CURRENT_TIMESTAMP() WHERE id = '" .  $this->_id . "'";
      return $this->_db->executeQuery($q);
    }

    public function updateVersion($type,$version, $deviceType) {
      $this->_debug->push("Updating device version");
      $q = "UPDATE " . C_TABLE_PREFIX . "devices SET firmware_type='".$type.
      "',device_type_id = ".$deviceType.
      ",firmware_version = '".$version.
      "', last_seen_on = CURRENT_TIMESTAMP(), firmware_upgraded_on = CURRENT_TIMESTAMP() WHERE id = '" .  $this->_id . "'";
      return $this->_db->executeQuery($q);
    }

    public function updateKeyNumber($keyNumber) {
      $this->_debug->push("Updating key#");
      $q = "UPDATE " . C_TABLE_PREFIX . "devices SET serial_id='".$keyNumber.
      "', last_seen_on = CURRENT_TIMESTAMP() WHERE id = '" .  $this->_id . "'";
      return $this->_db->executeQuery($q);
    }

    public function checkIfUpgraded($type,$version,$deviceType) {
        $this->_debug->push("Checking if firmware has been upgraded: ");
        if ($this->_data["firmware_version"] <> $version || $this->_data["firmware_type"] <> $type || $this->_data["device_type_id"] <> $deviceType) {
          $this->_debug->push("YES",false);
          return true;
        } else {
          $this->_debug->push("NO",false);
          return false;
        }
    }

    public function getKeyNumber() {
      return isset($this->_data["serial_id"])?$this->_data["serial_id"]:false;
    }
}
?>
