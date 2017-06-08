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
  body+="<input name=\"device_name\" type=\"text\" maxlength=\"16\" value=\"SONOFF_12345678\">";
  body+="<span class=\"pure-form-message-inline\">Max 16 znaków. Informacja wymagana</span>";
  body+="</div>";
  body+="</fieldset>";

  page+=Site.addConfigurationBlock(
  "Nazwa urządzenia"
  ,"Nazwa jest wykorzystywana jako nazwa hosta w Twojej lokalnej sieci WiFi oraz jako nazwa punktu dostępowego WiFi dla konfiguracji urządzenia"
  ,body
  ,labels);
/*
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
  body+="<div class=\"pure-controls\">";
  body+="<label class=\"pure-checkbox\">";
  body+="<input name=\"Internal_relay_present\" type=\"checkbox\"> Podączony?";
  body+="</label>";
  body+="</div>";
  body+=Site.generateConfigParameter_GPIO("GPIO","internal_relay_gpio",1);
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


  //  MQTT Broker
  labels[0] = {0};

  body="<fieldset>";
  body+="<div class=\"pure-control-group\">";
  body+="<label>Adres lub IP serwera</label>";
  body+="<input name=\"mqtt_host\" type=\"text\" maxlength=\"32\">";
  body+="<span class=\"pure-form-message-inline\">Informacja jest wymagana. Max 32 znaki</span>";
  body+="</div>";
  body+="<div class=\"pure-control-group\">";
  body+="<label>Port</label>";
  body+="<input name=\"mqtt_port\" type=\"number\" value=\"1883\" maxlength=\"5\">";
  body+="<span class=\"pure-form-message-inline\">Informacja jest wymagana</span>";
  body+="</div>";
  body+="<div class=\"pure-control-group\">";
  body+="<label>Użytkownik</label>";
  body+="<input name=\"mqtt_user\" type=\"number\"  maxlength=\"16\">";
  body+="<span class=\"pure-form-message-inline\">Max 16 znaki</span>";
  body+="</div>";
  body+="<div class=\"pure-control-group\">";
  body+="<label>Hasło</label>";
  body+="<input name=\"mqtt_password\" type=\"password\"  maxlength=\"16\">";
  body+="<span class=\"pure-form-message-inline\">Max 16 znaki/span>";
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

  //  Domoticz
  labels[0] = {0};

  body="<fieldset>";
  body+="<div class=\"pure-control-group\">";
  body+="<label>IP serwera</label>";
  body+="<input name=\"domoticz_ip1\" type=\"number\" maxlength=\"3\" style=\"width:70px\">.";
  body+="<input name=\"domoticz_ip2\" type=\"number\" maxlength=\"3\" style=\"width:70px\">.";
  body+="<input name=\"domoticz_ip3\" type=\"number\" maxlength=\"3\" style=\"width:70px\">.";
  body+="<input name=\"domoticz_ip4\" type=\"number\" maxlength=\"3\" style=\"width:70px\">.";
  body+="<span class=\"pure-form-message-inline\">Informacja wymagana</span>";
  body+="</div>";
  body+="<div class=\"pure-control-group\">";
  body+="<label>Port</label>";
  body+="<input name=\"domoticz_port\" type=\"number\" value=\"8080\" maxlength=\"5\">";
  body+="<span class=\"pure-form-message-inline\">Informacja wymagana</span>";
  body+="</div>";
  body+="<div class=\"pure-control-group\">";
  body+="<label>IDX urządzenia</label>";
  body+="<input name=\"domoticz_device_idx\" type=\"number\">";
  body+="<span class=\"pure-form-message-inline\">Informacja wymagana. Do odczytania z konfiguracji Domoticz</span>";
  body+="</div>";
  body+="</fieldset>";

  page+=Site.addConfigurationBlock(
  "Serwer Domoticz"
  ,"Konfiguracja wymagana jest jedynie kiedy jest potrzeba wysyłania stanu przekaźnika do serwera Domoticz"
  ,body
  ,labels);


  //  DS18B20
  labels[0] = {1};

  body="<fieldset>";
  body+="<div class=\"pure-controls\">";
  body+="<label class=\"pure-checkbox\">";
  body+="<input name=\"ds18b20_present\" type=\"checkbox\"> Podączony?";
  body+="</label>";
  body+="</div>";
  body+=Site.generateConfigParameter_GPIO("GPIO","ds18b20_gpio",1);
  body+="<div class=\"pure-control-group\">";
  body+="<label>Odczyty co</label>";
  body+="<input name=\"ds18b20_interval\" type=\"number\" value=\"600\" maxlength=\"5\">";
  body+="<span class=\"pure-form-message-inline\">sekund. Zakres 10sek do 99999sek</span>";
  body+="</div>";
  body+="<div class=\"pure-control-group\">";
  body+="<label>Korekta wartości o</label>";
  body+="<input name=\"ds18b20_correction\" type=\"number\" value=\"0\" maxlength=\"6\">";
  body+="<span class=\"pure-form-message-inline\">stopni. Zakres -9.99 do 9.99, </span>";
  body+="</div>";
  body+="</fieldset>";

  page+=Site.addConfigurationBlock(
  "Czujnik temperatury DS18B20"
  ,"DS18B20 jest obsługiwanym czujnikiem temperatury. Możliwe jest podłaczenie tylko jednego czujnika temperatury"
  ,body
  ,labels);
*/

  //  External Relay
  labels[0] = {1};

  body="<fieldset>";

  body+="<div class=\"pure-controls\">";
  body+="<label class=\"pure-checkbox\">";
  body+="<input name=\"external_relay_present\" type=\"checkbox\"> Podączony?";
  body+="</label>";
  body+="</div>";
  body+=Site.generateConfigParameter_GPIO("GPIO","external_relay_gpio",1);
  body+="<div class=\"pure-control-group\">";
  body+="<label>Nazwa</label>";
  body+="<input name=\"external_relay_name\" type=\"text\" value=\"przekaznik-1\" maxlength=\"16\">";
  body+="<span class=\"pure-form-message-inline\">Informacja jest wymagana. Tylko litery i cyfry. Max 16 znaki</span>";
  body+="</div>";
  body+="<p>Zachowanieprzekaźnia po przywróceniu zasilania lub ponownego podłączenia się do brokera MQTT</p>";
  body+="<div class=\"pure-control-group\">";
  body+="<label>Przywrócenie zasilania</label>";
  body+="<select name=\"external_relay_power_restored\">";
  body+="<option value=\"0\">Wyłączony</option>";
  body+="<option value=\"1\">Włączony</option>";
  body+="<option value=\"2\">Ostatnia zapamiętana wartość</option>";
  body+="</select>";
  body+="</div>";
  body+="<div class=\"pure-control-group\">";
  body+="<label>Podłączony do MQTT</label>";
  body+="<select name=\"external_relay_mqtt_connected\">";
  body+="<option value=\"0\">Wyłączony</option>";
  body+="<option value=\"1\">Włączony</option>";
  body+="<option value=\"2\">Ostatnia zapamiętana wartość</option>";
  body+="<option value=\"3\">Wartość z systemu sterowania</option>";
  body+="</select>";
  body+="</div>";
  body+="</fieldset>";

  page+=Site.addConfigurationBlock(
  "Konfiguracja dodatkowego przekaźnika"
  ,"Każdy dodatkowy przekaźnik powinnien mieć unikalną nazwę. Nazwa przekaźnika jest częścio tematu MQTT. np /esp/nazwa_przekaznika/"
  ,body
  ,labels);


  //  External Switch
  labels[0] = {1};

  body="<fieldset>";
  body+="<div class=\"pure-controls\">";
  body+="<label class=\"pure-checkbox\">";
  body+="<input name=\"switch_present\" type=\"checkbox\"> Podączony?";
  body+="</label>";
  body+="</div>";
  body+=Site.generateConfigParameter_GPIO("GPIO","switch_gpio",1);
  body+="<div class=\"pure-control-group\">";
  body+="<label>Typ</label>";
  body+="<select name=\"switch_type\">";
  body+="<option value=\"0\">Mono-stabilny</option>";
  body+="<option value=\"1\">Bi-stabilny</option>";
  body+="</select>";
  body+="</div>";
  body+="<div class=\"pure-control-group\">";
  body+="<label>Czułość</label>";
  body+="<select name=\"switch_sensitivity\">";
  body+="<option value=\"0\">Niska</option>";
  body+="<option value=\"1\">Średnia</option>";
  body+="<option value=\"2\">Wysoka</option>";
  body+="</select>";
  body+="</div>";
  body+="</fieldset>";

  page+=Site.addConfigurationBlock(
  "Zewnętrzny włącznik"
  ,"Zewnętrzny przełącznik steruje tylko wbudowanym przekaźnikiem. Ustawienie czułości włącznika należy ustawić pod indywidulane potrzeby. Wysoka czułości może powodować, że przekaźnik może zmieniać swój stan nadmiernie dla niektórych włączników"
  ,body
  ,labels);

  page+=Site.generateRightPanel();
  page+=Site.generateFooter();
  return page;
}
