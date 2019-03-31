<?php
if (stripos($_SERVER['PHP_SELF'], "localization.class.php") == true) die();

class localization
{

    private $_id;
    private $_db;
    private $_location;
    private $_debug;

    public function __construct($ip)
    {
        global $db, $debug;
        $this->_db = $db;
        $this->_debug = $debug;
        $this->_debug->push("Reading location for: " . $ip);
        $this->_location = json_decode(file_get_contents('http://api.ipstack.com/' . $ip. '?access_key=10eb4dab17c25ed10586f7fb23eb2f7c'));
        $this->_debug->push("- ".$this->_location->country_code);
        $this->_debug->push("- ".$this->_location->country_name);
        $this->_debug->push("- ".$this->_location->region_code);
        $this->_debug->push("- ".$this->_location->region_name);
        $this->_debug->push("- ".$this->_location->zip);
        $this->_debug->push("- ".$this->_location->city);
    }


    public function add($deviceID)
    {

      $this->_debug->push("Updatin device localization");
      $query = "INSERT INTO " . C_TABLE_PREFIX . "device_locations	 (id,geo_country_code, geo_country_name, geo_region_code,
      geo_region_name,geo_zipcode,geo_city)  VALUES ('" . $deviceID . "','"
      . $this->_location->country_code . "','"
      . $this->_location->country_name . "','"
      . $this->_location->region_code . "','"
      . $this->_location->region_name . "','"
      . $this->_location->zip . "','"
      . $this->_location->city . "')";
      return $this->_db->executeQuery($query);
    }


    public function getCity() {
      return $this->_location->city;
    }

    public function getCountryCode() {
      return $this->_location->country_code;
    }

}
?>
