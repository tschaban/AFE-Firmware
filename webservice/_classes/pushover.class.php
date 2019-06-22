<?php

/**
 * Created by PhpStorm.
 * User: Adrian
 * Date: 2015-02-22
 * Time: 15:41
 */
class pushover
{
    private $api;

    private $config = array(
        "user-key" => "ufMurNr7s4GpTf4am5RW3QkWejrtqV",
        "application-key" => "au8u9e2km9xz1inpgu7agu799xsaz3"
    );


    public function __construct()
    {
        $this->api = new pushoverAPI();
        $this->api->setToken($this->config["application-key"]);
        $this->api->setUser($this->config["user-key"]);
        $this->api->setDevice("iPhone-Adriana");
        $this->api->setUrl("http://adrian.czabanowski.com/afe/");
        $this->api->setUrlTitle("AFE Firmware Statistics");
        $this->api->setTimestamp(time());

    }

    public function send ($title,$message,$priority=-1) {
        $this->api->setTitle($title);
        $this->api->setMessage($message);
        $this->api->setPriority($priority);
        global $debug;
        $debug->push("Sending PushOver notification");
        $go = $this->api->send();
    }



}
