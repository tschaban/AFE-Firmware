<?php

if (stripos($_SERVER['PHP_SELF'], "database.class.php") == true) die();


class database
{

    private $db_connect;
    private $_debug;

    public function __construct($db_host, $db_user, $db_pass, $db_name)
    {

        global $debug;
        $this->_debug = $debug;
        try {
            $this->db_connect = new PDO("mysql:host=" . $db_host . ";dbname=" . $db_name, $db_user, $db_pass, array(
                PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION,
                PDO::MYSQL_ATTR_INIT_COMMAND => "SET NAMES utf8"
            ));
        } catch (PDOException $e) {
            $this->_debug->push("Connection failed: " . $e->getMessage());
            die("Connection failed: " . $e->getMessage());
        }
    }

    public function __destruct()
    {
        unset($this->db_connect);
    }


    public function executeQuery($query)
    {

        $_s = $this->getmicrotime();
        $this->_debug->push("Executing query: ".$query);
        $result = $this->db_connect->prepare($query);
        $result->execute();
        $_e = number_format(round(($this->getmicrotime() - $_s) * 1000, 3), 3, ',', '');
        $this->_debug->push("- Execution time: ".$_e."ms");
        return $result;
    }

    public function executeCount($field, $table, $conditions = "")
    {
        $query = "SELECT Count(" . $field . ") FROM " . C_TABLE_PREFIX . $table . ($conditions ? " WHERE " . $conditions : "");
        return $this->fetchFirstColumn($query);
    }


    public function numberOfRows($result)
    {
        return $result->rowCount();
    }

    public function fetchRecord($result)
    {
        return $result->fetch();
    }


    public function fetchFirstColumn($query)
    {
        $result = $this->executeQuery($query);
        return $result->fetchColumn();
    }

    public function fetchAllRecords($query)
    {
        $result = $this->executeQuery($query);
        return $result->fetchAll();
    }

    private function getmicrotime()
{
    list($usec, $sec) = explode(" ", microtime());
    return ((float)$usec + (float)$sec);
}

}

?>
