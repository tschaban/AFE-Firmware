<?php
if (stripos($_SERVER['PHP_SELF'], "debugger.class.php") == true) die();

class debugger
{

    private $debugging;

    public function __construct($debug)
    {
        $this->debugging = $debug;
    }

    public function push($text,$newline=true)
    {
      if ($this->debugging) {
        echo ($newline?"<br>":"").$text;
      }
    }
}
?>
