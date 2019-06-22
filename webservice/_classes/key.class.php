<?php
if (stripos($_SERVER['PHP_SELF'], "key.class.php") == true) die();

class key
{

    private $_id;
    private $_db;
    private $_data;
    private $_debug;
    private $_keyInDB;

    public function __construct($key)
    {
        global $db, $debug;
        $this->_db = $db;
        $this->_id = $key;
        $this->_debug = $debug;

        $debug->push("Looking for a key#: " . $this->_id);
        $query = "SELECT * FROM " . C_TABLE_PREFIX . "serials WHERE id = '" . $this->_id . "'";
        $_res = $this->_db->executeQuery($query);
        if ($this->_data = $this->_db->fetchRecord($_res)) {
            $this->_keyInDB = true;
            $this->_debug->push("- Key: ".$this->_data["id"]);
            $this->_debug->push("- Valid: ". $this->_data["valid"]);
            $this->_debug->push("- Limit: ". $this->_data["limit"]);
        } else {
            $this->_keyInDB = false;
        }
    }

    public function found() {
      $this->_debug->push("Key# ".($this->_keyInDB?"":"NOT")." found in the DB");
      return $this->_keyInDB;
    }

    public function getLimits() {
        return $this->_data["limit"];
    }

    public function isValid() {
        return $this->_data["valid"]==1?true:false;
    }

    public function usedBy() {
        $count = $this->_db->executeCount("id","devices","serial_id = '".$this->_id."'");
        $this->_debug->push("- Key used by ".$count ." devices");
        return $count;
    }


}
?>
