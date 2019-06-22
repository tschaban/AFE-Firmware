<?php
if (stripos($_SERVER['PHP_SELF'], "logo.class.php") == true) die();

class logo
{

    public function __construct() {}

    public function generate()
    {
        $im = imagecreatefromjpeg("images/afe-firmware-header.jpg");
        header("Cache-Control: no-cache");
        header("Cache-Control: no-store");
        header("Expires: " . date('D, d M Y H:i:s', 0) . ' GMT');
        header("Pragma: no-cache");
        Header("Content-type: image/jpeg");
        imagejpeg($im);
        imagedestroy($im);

    }
}

?>
