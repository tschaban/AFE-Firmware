#include "AFE-Sites-Generator.h"

AFESitesGenerator::AFESitesGenerator() {}

const char* AFESitesGenerator::generateHeader() {
        return "<!doctype html>"
               "<html lang=\"en\">"
               "<head>"
               "<meta charset=\"utf-8\">"
               "<title>AFE Firmware</title>"
               "<style>body{margin:0}#c{display:table;width:100%}#l,#r{display:table-cell}#l{width:300px;background:#282828;padding:20px;color:#eee}#r{padding:20px}p,a,input,h1,h3,h4,span,label,button{font-family:sans-serif}a{text-decoration:none}.ltit,.ltag{margin:0}.ltag{font-weight:300;color:#b0cadb;margin-bottom:20px}.lst{list-style:none;margin:0;padding:0}.itm a{display:block;text-decoration:none;white-space:nowrap;padding:.2em 1em;color:#777;font-size:95%}.itm a:hover,.itm a:focus{background-color:#eee;text-decoration:none;padding:.2em 1.5em;color:#000}.ci{margin-bottom:2em}.ci h1{color:#aaa;border-bottom:1px solid #eee;font-size:110%;font-weight:500;letter-spacing:.1em}.ci .cd{color:#444;line-height:1.8em;font-size:80%;font-style:italic}.cm{color:#999;font-size:90%;margin:0 0 20px 0}.la{margin:0 .1em;padding:.3em 1em;color:#fff;background:#999;font-size:80%}.lr{background:#ca3c3c}.lg{background:#2fb548}fieldset{margin:0;padding:.35em 0 .75em;border:0}.cf{margin-bottom:.5em}.cc{margin:1em 0 .5em 9.4em}.cf label{text-align:right;display:inline-block;vertical-align:middle;width:10em;margin:0 1em 0 0;font-size:.875em}.cc label{font-size:.875em}input,select{padding:.5em .6em;display:inline-block;border:1px solid #ccc;vertical-align:middle;box-sizing:border-box}.hint{display:inline-block;padding-left:.3em;color:#aaa;vertical-align:middle;font-size:80%}.b{font-size:100%;padding:.5em 1em;border:1px solid #999;border:none rgba(0,0,0,0);text-decoration:none;color:white}.bs{background:#2fb548}.be{background:#ca3c3c}.bw{background:#df7514}.bc{background:#42b8dd}.b:hover,.b:focus{filter:alpha(opacity=90);background-image:-webkit-linear-gradient(transparent,rgba(0,0,0,0.05) 40%,rgba(0,0,0,0.10));background-image:linear-gradient(transparent,rgba(0,0,0,0.05) 40%,rgba(0,0,0,0.10))}</style>"
               "<script type=\"text/javascript\">"
               "hide=function(e){document.getElementById(e).style.display='none';};"
               "show=function(e){document.getElementById(e).style.display='inline';};"
               "</script>"
               "</head>"
               "<body>"
               "<div id=\"c\">"
               "<div id=\"l\">"
               "<h3 class=\"ltit\">AFE FIRMWARE</h3>"
               "<h4 class=\"ltag\">dla urządzeń zbudowanych o ESP8266</h4>"
               "<h4>MENU</h4>"
               "<ul class=\"lst\">"
               "<li class=\"itm\"><a href=\"\\\">Konfiguracja podstawowa</a></li>"
               "<li class=\"itm\"><a href=\"\\mqtt\">MQTT Broker</a></li>"
               "<li class=\"itm\"><a href=\"\\domoticz\">Domoticz</a></li>"
               "<li class=\"itm\"><a href=\"\\relay\">Przekaźnik</a></li>"
               "<li class=\"itm\"><a href=\"\\ds18b20\">Czujnik DS18B20</a></li>"
               "<li class=\"itm\"><a href=\"\\switch\">Przycisk / Włącznik</a></li>"
               "<li class=\"itm\"><a href=\"\\upgrade\">Aktulizacja</a></li>"
               "<li class=\"itm\"><a href=\"\\reset\">Przywracanie ustawień</a></li>"
               "<br><br>"
               "<li class=\"itm\"><a href=\"\\exit\">Zakończ konfigurację</a></li>"
               "</ul>"
               "<br><br>"
               "<h4>INFORMACJE</h4>"
               "<ul class=\"lst\">"
               "<li class=\"itm\"><a href=\"http://smart-house.adrian.czabanowski.com/firmware-sonoff/\" target=\"_blank\">Dokumentacja</a></li>"
               "<li class=\"itm\"><a href=\"http://smart-house.adrian.czabanowski.com/forum/firmware-do-przelacznika-sonoff/\" target=\"_blank\">Pomoc</a></li>"
               "<li class=\"itm\"><a href=\"https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE\" target=\"_blank\">Licencja</a></li>"
               "<li class=\"itm\"><a href=\"http://smart-house.adrian.czabanowski.com/firmware-sonoff/historia-zmian/\" target=\"_blank\">Wersja 1.0.0</a></li>"
               "</ul>"
               "</div>"
               "<div id=\"r\">";

}

const char* AFESitesGenerator::generateFooter() {
        return "</div></body></html>";
}

String AFESitesGenerator::addConfigurationBlock(const String title,const String description, const String body,  const String link) {
        String page ="<div class=\"ci\">";
        page+="<a name=\""+link+"\"></a>";
        page+="<h1>" + title + "</h1>";
        page+="<p class=\"cd\">" + description + "</p>";
        page+= body;
        page+= "</div>";

        return page;
}

const String AFESitesGenerator::generateConfigParameter_GPIO(const char* field, uint8_t selected) {
        String page = "<div class=\"cf\"><label>GPIO</label><select name=\"";
        page += field;
        page += "\">";
        for (uint8_t i=0;i<=16;i++) {
          page += "<option value=\""+String(i)+"\"" + (selected==i?"selected=\"selected\"":"") + ">"+String(i)+"</option>";
        }
        page += "</select></div>";
        return page;
}

String AFESitesGenerator::addDeviceNameConfiguration() {

  char name[32] = "SONOFF";

  String body="<fieldset>"
               "<div class=\"cf\">"
               "<label>Nazwa urządzenia</label>"
               "<input name=\"device_name\" type=\"text\" maxlength=\"32\" value=\""; body+=name; body+="\">"
               "<span class=\"hint\">Max 32 znaki. Informacja wymagana</span>"
               "</div>"
               "</fieldset>";

  return addConfigurationBlock(
          "Nazwa urządzenia"
          ,"Nazwa jest wykorzystywana jako nazwa hosta w Twojej lokalnej sieci WiFi oraz jako nazwa punktu dostępowego WiFi dla konfiguracji urządzenia"
          ,body
          ,"dev-name"
          );
}

String AFESitesGenerator::addWiFiConfiguration() {
  char name[32] = "ssid";
  char password[16] = "password";

  String body="<fieldset>";
  body+="<div class=\"cf\">";
  body+="<label>Nazwa sieci WiFI</label>";
  body+="<input name=\"wifi_name\" type=\"text\" maxlength=\"32\" value=\""; body+=name; body+="\">";
  body+="<span class=\"hint\">Max 32 znaków. Informacja wymagana</span>";
  body+="</div>";
  body+="<div class=\"cf\">";
  body+="<label>Hasło</label>";
  body+="<input type=\"password\" name=\"wifi_password\" maxlength=\"16\" value=\""; body+=password; body+="\">";
  body+="<span class=\"hint\">Max 16 znaków.</span>";
  body+="</div>";
  body+="</fieldset>";

  return addConfigurationBlock(
          "Konfiguracja dostępu do sieci WiFi"
          ,"Urządzenie bez skonfigurowanej sieci WiFi będzie działać tylko w trybie sterowania ręcznego"
          ,body
          ,"wifi"
          );
}



String AFESitesGenerator::addNetworkConfiguration() {

  boolean dhcp = true;

  uint8_t device_ip1 = 192;
  uint8_t device_ip2 = 168;
  uint8_t device_ip3 = 2;
  uint8_t device_ip4 = 255;

  uint8_t gate_ip1 = 255;
  uint8_t gate_ip2 = 255;
  uint8_t gate_ip3 = 255;
  uint8_t gate_ip4 = 0;

  uint8_t dns_ip1 = 192;
  uint8_t dns_ip2 = 168;
  uint8_t dns_ip3 = 2;
  uint8_t dns_ip4 = 100;

  String body="<fieldset>";

  body+="<div class=\"cc\">";
  body+="<label>";
  body+="<input name=\"dhcp_config\" type=\"checkbox\" value=\"1\""; body+=(dhcp?" checked=\"checked\"":""); body+="> Automatyczna konfiguracja przez DHCP?";
  body+="</label>";
  body+="</div>";

  body+="<div class=\"cf\">";
  body+="<label>Adres IP urządzenia</label>";
  body+="<input name=\"device_ip1\" type=\"number\" maxlength=\"3\" style=\"width:70px\" value=\""; body+=device_ip1; body+="\">.";
  body+="<input name=\"device_ip2\" type=\"number\" maxlength=\"3\" style=\"width:70px\" value=\""; body+=device_ip2; body+="\">.";
  body+="<input name=\"device_ip3\" type=\"number\" maxlength=\"3\" style=\"width:70px\" value=\""; body+=device_ip3; body+="\">.";
  body+="<input name=\"device_ip4\" type=\"number\" maxlength=\"3\" style=\"width:70px\" value=\""; body+=device_ip4; body+="\">";
  body+="<span class=\"hint\">Informacja wymagana</span>";
  body+="</div>";

  body+="<div class=\"cf\">";
  body+="<label>Bramka</label>";
  body+="<input name=\"gate_ip1\" type=\"number\" maxlength=\"3\" style=\"width:70px\" value=\""; body+=gate_ip1; body+="\">.";
  body+="<input name=\"gate_ip2\" type=\"number\" maxlength=\"3\" style=\"width:70px\" value=\""; body+=gate_ip2; body+="\">.";
  body+="<input name=\"gate_ip3\" type=\"number\" maxlength=\"3\" style=\"width:70px\" value=\""; body+=gate_ip3; body+="\">.";
  body+="<input name=\"gate_ip4\" type=\"number\" maxlength=\"3\" style=\"width:70px\" value=\""; body+=gate_ip4; body+="\">";
  body+="<span class=\"hint\">Informacja wymagana</span>";
  body+="</div>";

  body+="<div class=\"cf\">";
  body+="<label>DNS</label>";
  body+="<input name=\"dns_ip1\" type=\"number\" maxlength=\"3\" style=\"width:70px\" value=\""; body+=dns_ip1; body+="\">.";
  body+="<input name=\"dns_ip2\" type=\"number\" maxlength=\"3\" style=\"width:70px\" value=\""; body+=dns_ip2; body+="\">.";
  body+="<input name=\"dns_ip3\" type=\"number\" maxlength=\"3\" style=\"width:70px\" value=\""; body+=dns_ip3; body+="\">.";
  body+="<input name=\"dns_ip4\" type=\"number\" maxlength=\"3\" style=\"width:70px\" value=\""; body+=dns_ip4; body+="\">";
  body+="<span class=\"hint\">Informacja wymagana</span>";
  body+="</div>";

  body+="</fieldset>";

  return addConfigurationBlock(
          "Konfiguracja sieci"
          ,"Parametery sieci moga byc skonfigurowane automatycznie przez serwer DHCP rutera lub manualnie"
          ,body
          ,"network"
          );
}


String AFESitesGenerator::addRelayConfiguration() {
  char name[16] = "przekaznik";
  boolean present = true;
  uint8_t gpio = 12;
  uint8_t powerRestored = 1;
  uint8_t connectedToMqtt = 2;

  String body="<fieldset>";
  body+="<div class=\"cc\">";
  body+="<label>";
  body+="<input name=\"relay1_present\" type=\"checkbox\" value=\"1\""; body+=(present?" checked=\"checked\"":""); body+="> Włączony?";
  body+="</label>";
  body+="</div>";
  body+=generateConfigParameter_GPIO("relay1_gpio",gpio);
  body+="<div class=\"cf\">";
  body+="<label>Nazwa</label>";
  body+="<input name=\"relay1_name\" type=\"text\" maxlength=\"16\" value=\""; body+=name; body+="\">";
  body+="<span class=\"hint\">Informacja wymagana. Max 16 znaków</span>";
  body+="</div>";
  body+="<p class=\"cd\">Zachowanie się przekaźnia po przywróceniu zasilania lub ponownego podłączenia się do brokera MQTT (tyko dla sterowania przez MQTT)</p>";
  body+="<div class=\"cf\">";
  body+="<label>Przywrócenie zasilania</label>";
  body+="<select name=\"relay1_power_restored\">";
  body+="<option value=\"0\""; body+=(powerRestored==0?" selected=\"selected\"":""); body+=">Wyłączony</option>";
  body+="<option value=\"1\""; body+=(powerRestored==1?" selected=\"selected\"":""); body+=">Włączony</option>";
  body+="<option value=\"2\""; body+=(powerRestored==2?" selected=\"selected\"":""); body+=">Ostatnia zapamiętana wartość</option>";
  body+="<option value=\"3\""; body+=(powerRestored==3?" selected=\"selected\"":""); body+=">Przeciwna do ostatniej zapamiętanej wartości</option>";
  body+="</select>";
  body+="</div>";
  body+="<div class=\"cf\">";
  body+="<label>Włączony do MQTT</label>";
  body+="<select  name=\"relay1_mqtt_connected\">";
  body+="<option value=\"1\""; body+=(connectedToMqtt==0?" selected=\"selected\"":""); body+=">-- Wybierz --</option>";
  body+="<option value=\"1\""; body+=(connectedToMqtt==1?" selected=\"selected\"":""); body+=">Wyłączony</option>";
  body+="<option value=\"2\""; body+=(connectedToMqtt==2?" selected=\"selected\"":""); body+=">Włączony</option>";
  body+="<option value=\"3\""; body+=(connectedToMqtt==3?" selected=\"selected\"":""); body+=">Ostatnia zapamiętana wartość</option>";
  body+="<option value=\"4\""; body+=(connectedToMqtt==4?" selected=\"selected\"":""); body+=">Wartość z systemu sterowania</option>";
  body+="</select>";
  body+="</div>";
  body+="</fieldset>";

  return addConfigurationBlock(
          "Przekaźnik"
          ,"Przekaźnik musi miec nadaną nazwę np. lampa. Nazwa przekaźnika definiuje dedykowany temat MQTT dla przekaźnika z tematu zdefiniowanego w sekcji MQTT broker oraz poniżej nazwy. np /sonoff/lampa/"
          ,body
          ,"relay-1"
          );
}

String AFESitesGenerator::addMQTTBrokerConfiguration() {
  char host[32] = "192.168.2.231";
  uint16_t port = 1883;
  char user[16] = "user";
  char password[16] = "password";
  char topic[32] = "/AFEDevice/";

  String body="<fieldset>";
  body+="<div class=\"cf\">";
  body+="<label>Adres lub IP serwera</label>";
  body+="<input name=\"mqtt_host\" type=\"text\" maxlength=\"32\" value=\""; body+=host; body+="\">";
  body+="<span class=\"hint\">Informacja jest wymagana. Max 32 znaki</span>";
  body+="</div>";
  body+="<div class=\"cf\">";
  body+="<label>Port</label>";
  body+="<input name=\"mqtt_port\" type=\"number\" value=\"1883\" maxlength=\"5\" value=\""; body+=port; body+="\">";
  body+="<span class=\"hint\">Informacja jest wymagana</span>";
  body+="</div>";
  body+="<div class=\"cf\">";
  body+="<label>Użytkownik</label>";
  body+="<input name=\"mqtt_user\" type=\"text\"  maxlength=\"16\" value=\""; body+=user; body+="\">";
  body+="<span class=\"hint\">Max 16 znaki</span>";
  body+="</div>";
  body+="<div class=\"cf\">";
  body+="<label>Hasło</label>";
  body+="<input name=\"mqtt_password\" type=\"password\"  maxlength=\"16\" value=\""; body+=password; body+="\">";
  body+="<span class=\"hint\">Max 16 znaki</span>";
  body+="</div>";
  body+="<div class=\"cf\">";
  body+="<label>Temat wiadomości</label>";
  body+="<input name=\"mqtt_topic\" type=\"text\" value=\"/sonoff/\" maxlength=\"32\" value=\""; body+=topic; body+="\">";
  body+="<span class=\"hint\">Informacja jest wymagana. Format: /abc/def/. Max 32 znaki.</span>";
  body+="</div>";
  body+="</fieldset>";


  return addConfigurationBlock(
          "MQTT Broker"
          ,"Urządzenie bez skonfigurowanego MQTT Brokera może być sterowany tylko ręcznie"
          ,body
          ,"mqtt"
          );

}

String AFESitesGenerator::addDomoticzConfiguration() {
  uint8_t ip1 = 255;
  uint8_t ip2 = 10;
  uint8_t ip3 = 50;
  uint8_t ip4 = 0;
  uint16_t port = 8080;
  uint16_t idx = 12345;

  char user[16] = "user";
  char password[16] = "password";


  String body="<fieldset>";
  body+="<div class=\"cf\">";
  body+="<label>IP serwera</label>";
  body+="<input name=\"domoticz_ip1\" type=\"number\" maxlength=\"3\" style=\"width:70px\" value=\""; body+=ip1; body+="\">.";
  body+="<input name=\"domoticz_ip2\" type=\"number\" maxlength=\"3\" style=\"width:70px\" value=\""; body+=ip2; body+="\">.";
  body+="<input name=\"domoticz_ip3\" type=\"number\" maxlength=\"3\" style=\"width:70px\" value=\""; body+=ip3; body+="\">.";
  body+="<input name=\"domoticz_ip4\" type=\"number\" maxlength=\"3\" style=\"width:70px\" value=\""; body+=ip4; body+="\">";
  body+="<span class=\"hint\">Informacja wymagana</span>";
  body+="</div>";
  body+="<div class=\"cf\">";
  body+="<label>Port</label>";
  body+="<input name=\"domoticz_port\" type=\"number\" value=\"8080\" maxlength=\"5\" value=\""; body+=port; body+="\">";
  body+="<span class=\"hint\">Informacja wymagana</span>";
  body+="</div>";
  body+="<div class=\"cf\">";
  body+="<label>Użytkownik</label>";
  body+="<input name=\"domoticz_user\" type=\"text\"  maxlength=\"16\" value=\""; body+=user; body+="\">";
  body+="<span class=\"hint\">Max 16 znaki</span>";
  body+="</div>";
  body+="<div class=\"cf\">";
  body+="<label>Hasło</label>";
  body+="<input name=\"domoticz_password\" type=\"password\"  maxlength=\"16\" value=\""; body+=password; body+="\">";
  body+="<span class=\"hint\">Max 16 znaki</span>";
  body+="</div>";
  body+="<div class=\"cf\">";
  body+="<label>IDX urządzenia</label>";
  body+="<input name=\"domoticz_device_idx\" type=\"number\" value=\""; body+=idx; body+="\">";
  body+="<span class=\"hint\">Informacja wymagana. Do odczytania z konfiguracji Domoticz</span>";
  body+="</div>";
  body+="</fieldset>";

  return addConfigurationBlock(
          "Serwer Domoticz"
          ,"Konfiguracja wymagana jest jedynie kiedy jest potrzeba wysyłania stanu przekaźnika do serwera Domoticz"
          ,body
          ,"domoticz"
          );
}

String AFESitesGenerator::addDS18B20Configuration() {
  boolean present = true;
  uint16_t interval = 600;
  float correction = 0;
  uint8_t gpio = 14;

  String body="<fieldset>";
  body+="<div class=\"cc\">";
  body+="<label>";
  body+="<input name=\"ds18b20_present\" type=\"checkbox\" value=\"1\""; body+=(present?" checked=\"checked\"":""); body+="> Włączony?";
  body+="</label>";
  body+="</div>";
  body+=generateConfigParameter_GPIO("ds18b20_gpio",gpio);
  body+="<div class=\"cf\">";
  body+="<label>Odczyty co</label>";
  body+="<input name=\"ds18b20_interval\" type=\"number\" maxlength=\"5\" value=\""; body+=interval; body+="\">";
  body+="<span class=\"hint\">sekund. Zakres 10sek do 99999sek</span>";
  body+="</div>";
  body+="<div class=\"cf\">";
  body+="<label>Korekta wartości o</label>";
  body+="<input name=\"ds18b20_correction\" type=\"number\" maxlength=\"6\" value=\""; body+=correction; body+="\">";
  body+="<span class=\"hint\">stopni. Zakres -9.99 do 9.99, </span>";
  body+="</div>";
  body+="</fieldset>";

  return addConfigurationBlock(
          "Czujnik temperatury DS18B20"
          ,"DS18B20 jest obsługiwanym czujnikiem temperatury. Możliwe jest podłaczenie tylko jednego czujnika temperatury"
          ,body
          ,"ds18b20"
          );
}

String AFESitesGenerator::addSwitchConfiguration(const char* id) {
  boolean present = false;
  uint8_t gpio = 3;
  uint8_t type = 1;
  uint8_t sensitivity = 1;
  char functionality[] = "0";

  String body="<fieldset>";
  body+="<div class=\"cc\">";
  body+="<label>";
  body+="<input name=\"switch";
  body+=id;
  body+="_present\" type=\"checkbox\" value=\"1\""; body+=(present?" checked=\"checked\"":""); body+="> Włączony?";
  body+="</label>";
  body+="</div>";

  body+="<div class=\"cf\">";
  body+="<label>Funkcja</label>";
  body+="<select name=\"switch";
  body+=id;
  body+="_functionality\">";
  body+="<option value=\"0\""; body+=(functionality=="0"?" selected=\"selected\"":""); body+=">-- Wybierz -- </option>";
  body+="<option value=\"1\""; body+=(functionality=="1"?" selected=\"selected\"":""); body+=">Sterowanie przekaźnikiem</option>";
  body+="<option value=\"1\""; body+=(functionality=="2"?" selected=\"selected\"":""); body+=">Multifunkcyjny</option>";
  body+="<option value=\"R\""; body+=(functionality=="R"?" selected=\"selected\"":""); body+=">Reboot</option>";
  body+="<option value=\"C\""; body+=(functionality=="C"?" selected=\"selected\"":""); body+=">Tryb konfiguracji</option>";
  body+="<option value=\"U\""; body+=(functionality=="U"?" selected=\"selected\"":""); body+=">Aktualizacja oprogramowania</option>";
  body+="</select>";
  body+="<span class=\"hint\">Informacja wymagana.</span>";
  body+="</div>";

  char filed[13];
  sprintf(filed, "switch%s_gpio", id);

  body+=generateConfigParameter_GPIO(filed,gpio);
  body+="<div class=\"cf\">";
  body+="<label>Typ</label>";
  body+="<select name=\"switch";
  body+=id;
  body+="_type\">";
  body+="<option value=\"0\""; body+=(type==0?" selected=\"selected\"":""); body+=">Mono-stabilny</option>";
  body+="<option value=\"1\""; body+=(type==1?" selected=\"selected\"":""); body+=">Bi-stabilny</option>";
  body+="</select>";
  body+="</div>";
  body+="<div class=\"cf\">";
  body+="<label>Czułość</label>";
  body+="<select name=\"switch";
  body+=id;
  body+="_sensitivity\">";
  body+="<option value=\"0\""; body+=(sensitivity==0?" selected=\"selected\"":""); body+=">Niska</option>";
  body+="<option value=\"1\""; body+=(sensitivity==1?" selected=\"selected\"":""); body+=">Średnia</option>";
  body+="<option value=\"2\""; body+=(sensitivity==2?" selected=\"selected\"":""); body+=">Wysoka</option>";
  body+="</select>";
  body+="</div>";

  body+="</fieldset>";

  char title[22];
  sprintf(title, "Przycisk / włącznik #%s", id);


  return addConfigurationBlock(
          title
          ,"Ustawienie czułości włącznika należy ustawić pod indywidulane potrzeby. Wysoka czułości może powodować, że przekaźnik może zmieniać swój stan nadmiernie dla niektórych włączników"
          ,body
          ,"switch"
          );

}


String AFESitesGenerator::addUpgradeSection() {
  String body="<fieldset>";
  body+="<div class=\"cf\">";
  body+="<label>Wybierz firmware</label>";
  body+="<input class=\"bs\" name=\"firmware_file\" type=\"file\" accept=\".bin\">";
  body+="</div>";
  body+="<p class=\"cd\">Po zakończeniu aktualizacji urządzenie zostanie automatycznie zresetowane</p>";
  body+="<button class=\"b be\">Aktualizuj</button>";
  body+="</fieldset>";
  return addConfigurationBlock(
          "Aktualizacja oprogramowania"
          ,"<strong>UWAGA</strong>: funkcjonalnośc tylko dla zaawansowanych użytkowników. Nie odłączaj urządzenia od zasilania podczas aktualizacji."
          ,body
          ,"upgrade"
          );
}

String AFESitesGenerator::addResetSection() {
  String body="<fieldset>";
  body+="<input class=\"b be\">Przywróc ustawienia początkowe</button>";
  body+="</fieldset>";
  return addConfigurationBlock(
          "Przywracanie ustawień początkowych"
          ,"<strong>Uwaga</strong>: przywracanie ustawień początkowych kasuje wszystkie ustawienia urządzenia, włącznie z konfiguracją sieci WiFi"
          ,body
          ,"reset"
          );
}

String AFESitesGenerator::addExitSection() {
  String body="<fieldset>";
  body+="<p class=\"cd\">Proszę czekac.... trwa restart urzadzenia.</p>";
  body+="</fieldset>";
  return addConfigurationBlock(
          "Panek konfiguracyjny"
          ,""
          ,body
          ,"exit"
          );
}
