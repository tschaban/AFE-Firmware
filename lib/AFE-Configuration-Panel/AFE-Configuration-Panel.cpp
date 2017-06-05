#include "AFE-Configuration-Panel.h"

AFEConfigurationPanel::AFEConfigurationPanel() {
}

String AFEConfigurationPanel::generateMainSite() {
  String page = Site.generateHeader();
  page+=Site.generateLeftPanel();

  // Device name
  uint8_t labels[] = {0};

  String body="<fieldset>";
  body+="<div class=\"pure-control-group\">";
  body+="<label>Nazwa urządzenia</label>";
  body+="<input name=\"device_name\" type=\"text\" maxlength=\"32\" value=\"SONOFF_12345678\">";
  body+="<span class=\"pure-form-message-inline\">Max 32 znaków. Informacja wymagana</span>";
  body+="</div>";
  body+="</fieldset>";

  page+=Site.addConfigurationBlock(
  "Nazwa urządzenia"
  ,"Nazwa jest wykorzystywana jako nazwa hosta w Twojej lokalnej sieci WiFi oraz jako nazwa punktu dostępowego WiFi dla konfiguracji urządzenia"
  ,body
  ,labels);

  // WiFi
  labels[0] = {1};

  body="<fieldset>";
  body+="<div class=\"pure-control-group\">";
  body+="<label>Nazwa sieci WiFI</label>";
  body+="<input name=\"wifi_name\" type=\"text\" maxlength=\"32\">";
  body+="<span class=\"pure-form-message-inline\">Max 32 znaków. Informacja wymagana</span>";
  body+="</div>";
  body+="<div class=\"pure-control-group\">";
  body+="<label>Hasło</label>";
  body+="<input type=\"password\" name=\"wifi_password\" maxlength=\"16\">";
  body+="<span class=\"pure-form-message-inline\">Max 16 znaków.</span>";
  body+="</div>";
  body+="</fieldset>";

  page+=Site.addConfigurationBlock(
  "Konfiguracja dostępu do sieci WiFi"
  ,"Urządzenie bez skonfigurowanej sieci WiFi będzie działać tylko w trybie sterowania ręcznego"
  ,body
  ,labels);


  // Internal relay
  labels[0] = {0};

  body="<fieldset>";
  body+="<div class=\"pure-control-group\">";
  body+="<label>Nazwa</label>";
  body+="<input name=\"internal_relay_name\" type=\"text\" maxlength=\"16\" value=\"przekaznik\">";
  body+="<span class=\"pure-form-message-inline\">Informacja wymagana. Max 16 znaków</span>";
  body+="</div>";
  body+="<p>Zachowanie się przekaźnia po przywróceniu zasilania lub ponownego podłączenia się do brokera MQTT (tyko dla sterowania przez MQTT)</p>";
  body+="<div class=\"pure-control-group\">";
  body+="<label class=\"pure-checkbox\">Przywrócenie zasilania</label>";
  body+="<select name=\"internal_relay_power_restored\">";
  body+="<option value=\"0\">Wyłączony</option>";
  body+="<option value=\"1\">Włączony</option>";
  body+="<option value=\"2\">Ostatnia zapamiętana wartość</option>";
  body+="</select>";
  body+="</div>";
  body+="<div class=\"pure-control-group\">";
  body+="<label class=\"pure-checkbox\">Podłączony do MQTT</label>";
  body+="<select>";
  body+="<option value=\"0\">Wyłączony</option>";
  body+="<option value=\"1\">Włączony</option>";
  body+="<option value=\"2\">Ostatnia zapamiętana wartość</option>";
  body+="<option value=\"3\">Wartość z systemu sterowania</option>";
  body+="</select>";
  body+="</div>";
  body+="</fieldset>";

  page+=Site.addConfigurationBlock(
  "Wbudowany przekaźnik"
  ,"Przekaźnik musi miec nadaną nazwę np. lampa. Nazwa przekaźnika definiuje dedykowany temat MQTT dla przekaźnika z tematu zdefiniowanego w sekcji MQTT broker oraz poniżej nazwy. np /sonoff/lampa/"
  ,body
  ,labels);


  /*  MQTT Broker
  labels[0] = {0};

  body="<fieldset>";
  body+="<div class=\"pure-control-group\">";
  body+="<label>Adres lub IP serwera</label>";
  body+="<input name=\"mqtt_host\" type=\"text\" maxlength=\"32\">";
  body+="<span class=\"pure-form-message-inline\">Informacja jest wymagana. Max 32 znaki/span>";
  body+="</div>";
  body+="<div class=\"pure-control-group\">";
  body+="<label>Port</label>";
  body+="<input name=\"mqtt_port\" type=\"number\" value=\"1883\" maxlength=\"5\">";
  body+="<span class=\"pure-form-message-inline\">Informacja jest wymagana</span>";
  body+="</div>";
  body+="<div class=\"pure-control-group\">";
  body+="<label>Użytkownik</label>";
  body+="<input name=\"mqtt_user\" type=\"number\">";
  body+="</div>";
  body+="<div class=\"pure-control-group\">";
  body+="<label>Hasło</label>";
  body+="<input name=\"mqtt_password\" type=\"password\">";
  body+="</div>";
  body+="<div class=\"pure-control-group\">";
  body+="<label>Temat wiadomości</label>";
  body+="<input name=\"mqtt_topic\" type=\"text\" value=\"/sonoff/\" maxlength=\"32\">";
  body+="<span class=\"pure-form-message-inline\">Informacja jest wymagana. Format: /abc/def/. Max 32 znaki.</span>";
  body+="</div>";
  body+="</fieldset>";

  page+=Site.addConfigurationBlock(
  "MQTT Broker"
  ,"Urządzenie bez skonfigurowanego MQTT Brokera może być sterowany tylko ręcznie"
  ,body
  ,labels);

*/

  page+=Site.generateRightPanel();
  page+=Site.generateFooter();
  return page;
}
