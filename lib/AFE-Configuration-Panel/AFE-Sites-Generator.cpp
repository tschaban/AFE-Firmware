#include "AFE-Sites-Generator.h"

AFESitesGenerator::AFESitesGenerator() {}

const String AFESitesGenerator::generateHeader(uint8_t redirect) {

  FIRMWARE configuration;
  configuration = Data.getFirmwareConfiguration();

  String page = "<!doctype html>"
                "<html lang=\"en\">"
                "<head>"
                "<meta charset=\"utf-8\">";

  if (redirect > 0) {
    page += "<meta http-equiv=\"refresh\" content=\"";
    page += String(redirect);
    page += ";URL=/\">";
  }

  page += "<title>AFE Firmware ";
  page += configuration.version;
  page += " [T";
  page += configuration.type;
  page +=
      "]</title>"
      "<style>body{margin:0}#c{display:table;width:100%}#l,#r{display:table-"
      "cell}#l{width:300px;background:#282828;padding:20px;color:#eee}#r{"
      "padding:20px}p,a,input,h1,h3,h4,span,label,button,li{font-family:sans-"
      "serif}a{text-decoration:none}.ltit,.ltag{margin:0}.ltag{font-weight:300;"
      "color:#b0cadb;margin-bottom:20px}.lst{list-style:none;margin:0;padding:"
      "0}.itm "
      "a{display:block;text-decoration:none;white-space:nowrap;padding:.2em "
      "1em;color:#777;font-size:95%}.itm a:hover,.itm "
      "a:focus{background-color:#eee;text-decoration:none;padding:.2em "
      "1.5em;color:#000}.ci{margin-bottom:2em}.ci "
      "h1{color:#aaa;border-bottom:1px solid "
      "#eee;font-size:110%;font-weight:500;letter-spacing:.1em}.ci "
      ".cd{color:#444;line-height:1.8em;font-size:80%;font-style:italic}.cm{"
      "color:#999;font-size:90%;margin:0 0 20px 0}.la{margin:0 "
      ".1em;padding:.3em "
      "1em;color:#fff;background:#999;font-size:80%}.lr{background:#ca3c3c}.lg{"
      "background:#2fb548}fieldset{margin:0;padding:.35em 0 "
      ".75em;border:0}.cf{margin-bottom:.5em}.cc{margin:1em 0 .5em 9.4em}.cf "
      "label{text-align:right;display:inline-block;vertical-align:middle;width:"
      "10em;margin:0 1em 0 0;font-size:.875em}.cc "
      "label{font-size:.875em}input,select{padding:.5em "
      ".6em;display:inline-block;border:1px solid "
      "#ccc;vertical-align:middle;box-sizing:border-box}.hint{display:inline-"
      "block;padding-left:.3em;color:#aaa;vertical-align:middle;font-size:80%}."
      "b{font-size:100%;padding:.5em 1em;border:1px solid #999;border:none "
      "rgba(0,0,0,0);text-decoration:none;color:white}.bs{background:#2fb548}."
      "be{background:#ca3c3c}.bw{background:#df7514}.bc{background:#42b8dd}.b:"
      "hover,.b:focus{filter:alpha(opacity=90);background-image:-webkit-linear-"
      "gradient(transparent,rgba(0,0,0,0.05) "
      "40%,rgba(0,0,0,0.10));background-image:linear-gradient(transparent,rgba("
      "0,0,0,0.05) 40%,rgba(0,0,0,0.10))}</style>"
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
      "<ul class=\"lst\">";
  if (Device.getMode() != MODE_NORMAL) {
    page += "<li class=\"itm\"><a href=\"\\?option=basic\">Konfiguracja "
            "podstawowa</a></li>"
            "<li class=\"itm\"><a href=\"\\?option=mqtt\">MQTT Broker</a></li>";
    /* @TODO DOMOTICZ
page +=
"<li class=\"itm\"><a href=\"\\?option=domoticz\">Domoticz</a></li>";
*/
    page += "<li class=\"itm\"><a href=\"\\?option=relay\">Przekaźnik</a></li>";
    /* @TODO DS18B20
    page += "<li class=\"itm\"><a href=\"\\?option=ds18b20\">Czujnik "
            "DS18B20</a></li>";
*/
    page += "<li class=\"itm\"><a href=\"\\?option=switch\">Przycisk / "
            "Włącznik</a></li>"
            "<li class=\"itm\"><a href=\"\\update\">Aktulizacja</a></li>"
            "<li class=\"itm\"><a href=\"\\?option=reset\">Przywracanie "
            "ustawień</a></li>"
            "<br><br>"
            "<li class=\"itm\"><a href=\"\\?option=exit\">Zakończ "
            "konfigurację</a></li>";
  } else {
    page += "<li class=\"itm\"><a href=\"\\?option=help&command=1\">Uruchom "
            "konfigurację</a></li>"
            "<li class=\"itm\"><a href=\"\\?option=help&command=2\">Uruchom "
            "Access Point</a></li>";
  }

  page +=
      "</ul>"
      "<br><br>"
      "<h4>INFORMACJE</h4>"
      "<ul class=\"lst\">"
      "<li class=\"itm\"><a "
      "href=\"http://smart-house.adrian.czabanowski.com/afe-firmware-pl/\" "
      "target=\"_blank\">Dokumentacja</a></li>"
      "<li class=\"itm\"><a "
      "href=\"http://smart-house.adrian.czabanowski.com/forum/"
      "firmware-do-przelacznika-sonoff/\" target=\"_blank\">Pomoc</a></li>"
      "<li class=\"itm\"><a "
      "href=\"https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE\" "
      "target=\"_blank\">Licencja</a></li>"
      "<li class=\"itm\"><a "
      "href=\"http://smart-house.adrian.czabanowski.com/afe-firmware-pl/t0/"
      "change-log"
      "historia-zmian/\" target=\"_blank\">Wersja ";
  page += configuration.version;
  page += " [T";
  page += configuration.type;
  page += "]</a></li>"
          "</ul>";
  if (Device.getMode() != MODE_ACCESS_POINT) {
    page += "<br><br>"
            "<h4>WSPARCIE</h4>";
    page += "<p class=\"cm\">Oprogramowanie dostępne jest za darmo w ramach "
            "licencji openSource <a "
            "href=\"https://github.com/tschaban/AFE-Firmware/blob/master/"
            "LICENSE\" "
            "target=\"_blank\"  style=\"color:#fff\">MIT</a></p>";
    page += "<p class=\"cm\">Jeśli spełnia Twoje oczekiwania "
            "to rozważ wsparcie <a "
            "href=\"https://adrian.czabanowski.com\" "
            "target=\"_blank\"  style=\"color:#fff\">autora</a>. Z góry "
            "dzięki.</p>";
    page +=
        "<a "
        "href=\"https://www.paypal.com/cgi-bin/"
        "webscr?cmd=_donations&business=VBPLM42PYCTM8&lc=PL&item_name="
        "Wsparcie%20projektu%20AFE%20Firmware&item_number=Firmware%20%5bvT0%"
        "5d&currency_code=PLN&bn=PP%2dDonationsBF%3abtn_donateCC_LG%2egif%"
        "3aNonHosted\" target=\"_blank\"><img "
        "src=\"https://adrian.czabanowski.com/images/paypal.png\" "
        "border=\"0\" alt=\"PayPal\" style=\"width:290px\"></a>";
  }
  page += "</div>"
          "<div id=\"r\">";
  return page;
}

const char *AFESitesGenerator::generateFooter() {
  return "</div></body></html>";
}

String AFESitesGenerator::addConfigurationBlock(const String title,
                                                const String description,
                                                const String body) {
  String page = "<div class=\"ci\">";
  page += "<h1>" + title + "</h1>";
  page += "<p class=\"cd\">" + description + "</p>";
  page += body;
  page += "</div>";

  return page;
}

const String AFESitesGenerator::generateConfigParameter_GPIO(const char *field,
                                                             uint8_t selected) {
  String page = "<div class=\"cf\"><label>GPIO</label><select name=\"";
  page += field;
  page += "\">";
  for (uint8_t i = 0; i <= 16; i++) {
    page += "<option value=\"" + String(i) + "\"" +
            (selected == i ? "selected=\"selected\"" : "") + ">" + String(i) +
            "</option>";
  }
  page += "</select></div>";
  return page;
}

String AFESitesGenerator::addNetworkConfiguration() {

  NETWORK configuration;
  configuration = Data.getNetworkConfiguration();

  String body =
      "<fieldset>"
      "<div class=\"cf\">"
      "<label>Nazwa urządzenia</label>"
      "<input name=\"hostname\" type=\"text\" maxlength=\"32\" value=\"";
  body += configuration.host;
  body += "\">"
          "<span class=\"hint\">Max 32 znaki. Informacja wymagana</span>"
          "</div>"
          "</fieldset>";

  String page = addConfigurationBlock(
      "Nazwa urządzenia",
      "Nazwa jest wykorzystywana jako nazwa hosta w Twojej lokalnej sieci WiFi "
      "oraz jako nazwa punktu dostępowego WiFi dla konfiguracji urządzenia",
      body);

  body = "<fieldset>";
  body += "<div class=\"cf\">";
  body += "<label>Nazwa sieci WiFI</label>";
  body += "<input name=\"wifi_ssid\" type=\"text\" maxlength=\"32\" value=\"";
  body += configuration.ssid;
  body += "\">";
  body += "<span class=\"hint\">Max 32 znaków. Informacja wymagana</span>";
  body += "</div>";
  body += "<div class=\"cf\">";
  body += "<label>Hasło</label>";
  body += "<input type=\"password\" name=\"wifi_password\" maxlength=\"32\" "
          "value=\"";
  body += configuration.password;
  body += "\">";
  body += "<span class=\"hint\">Max 32 znaków.</span>";
  body += "</div>";
  body += "</fieldset>";

  page +=
      addConfigurationBlock("Konfiguracja dostępu do sieci WiFi",
                            "Urządzenie bez skonfigurowanej sieci WiFi będzie "
                            "działać tylko w trybie sterowania ręcznego",
                            body);

  body = "<fieldset>";
  body += "<div class=\"cc\">";
  body += "<label>";
  body += "<input name=\"dhcp_config\" type=\"checkbox\" value=\"1\"";
  body += (configuration.isDHCP ? " checked=\"checked\"" : "");
  body += "> Automatyczna konfiguracja przez DHCP?";
  body += "</label>";
  body += "</div>";

  body += "<div class=\"cf\">";
  body += "<label>Adres IP urządzenia</label>";
  body += "<input name=\"device_ip1\" type=\"number\" maxlength=\"3\" "
          "style=\"width:70px\" value=\"";
  body += configuration.ip[0];
  body += "\">.";
  body += "<input name=\"device_ip2\" type=\"number\" maxlength=\"3\" "
          "style=\"width:70px\" value=\"";
  body += configuration.ip[1];
  body += "\">.";
  body += "<input name=\"device_ip3\" type=\"number\" maxlength=\"3\" "
          "style=\"width:70px\" value=\"";
  body += configuration.ip[2];
  body += "\">.";
  body += "<input name=\"device_ip4\" type=\"number\" maxlength=\"3\" "
          "style=\"width:70px\" value=\"";
  body += configuration.ip[3];
  body += "\">";
  body += "<span class=\"hint\">Informacja wymagana</span>";
  body += "</div>";

  body += "<div class=\"cf\">";
  body += "<label>Bramka</label>";
  body += "<input name=\"gateway_ip1\" type=\"number\" maxlength=\"3\" "
          "style=\"width:70px\" value=\"";
  body += configuration.gateway[0];
  body += "\">.";
  body += "<input name=\"gateway_ip2\" type=\"number\" maxlength=\"3\" "
          "style=\"width:70px\" value=\"";
  body += configuration.gateway[1];
  body += "\">.";
  body += "<input name=\"gateway_ip3\" type=\"number\" maxlength=\"3\" "
          "style=\"width:70px\" value=\"";
  body += configuration.gateway[2];
  body += "\">.";
  body += "<input name=\"gateway_ip4\" type=\"number\" maxlength=\"3\" "
          "style=\"width:70px\" value=\"";
  body += configuration.gateway[3];
  body += "\">";
  body += "<span class=\"hint\">Informacja wymagana</span>";
  body += "</div>";

  body += "<div class=\"cf\">";
  body += "<label>Maska sieci</label>";
  body += "<input name=\"subnet_ip1\" type=\"number\" maxlength=\"3\" "
          "style=\"width:70px\" value=\"";
  body += configuration.subnet[0];
  body += "\">.";
  body += "<input name=\"subnet_ip2\" type=\"number\" maxlength=\"3\" "
          "style=\"width:70px\" value=\"";
  body += configuration.subnet[1];
  body += "\">.";
  body += "<input name=\"subnet_ip3\" type=\"number\" maxlength=\"3\" "
          "style=\"width:70px\" value=\"";
  body += configuration.subnet[2];
  body += "\">.";
  body += "<input name=\"subnet_ip4\" type=\"number\" maxlength=\"3\" "
          "style=\"width:70px\" value=\"";
  body += configuration.subnet[3];
  body += "\">";
  body += "<span class=\"hint\">Informacja wymagana</span>";
  body += "</div>";

  body += "</fieldset>";

  page += addConfigurationBlock("Konfiguracja sieci",
                                "Parametery sieci moga byc skonfigurowane "
                                "automatycznie przez serwer DHCP rutera lub "
                                "manualnie",
                                body);

  body = "<fieldset>";
  body += "<div class=\"cf\">";
  body += "<label>Ilośc prób łączania się</label>";
  body += "<input name=\"no_connection_attempts\" type=\"number\" "
          "maxlength=\"3\" value=\"";
  body += configuration.noConnectionAttempts;
  body += "\">";
  body += "<span class=\"hint\">Od 1-255. Informacja wymagana</span>";
  body += "</div>";
  body += "<div class=\"cf\">";
  body += "<label>Czas oczekiwania między próbami</label>";
  body += "<input type=\"number\" name=\"wait_time_connections\" "
          "maxlength=\"3\" value=\"";
  body += configuration.waitTimeConnections;
  body += "\">";
  body += "<span class=\"hint\">1-255 sekund. Informacja wymagana</span>";
  body += "</div>";
  body += "<div class=\"cf\">";
  body += "<label>Czas po jakim powtórzyc próby łaczenia się</label>";
  body += "<input type=\"number\" name=\"wait_time_series\" maxlength=\"3\" "
          "value=\"";
  body += configuration.waitTimeSeries;
  body += "\">";
  body += "<span class=\"hint\">Od 1-255 sekund. Informacja wymagana.</span>";
  body += "</div>";
  body += "</fieldset>";

  page += addConfigurationBlock("Ustawienia dotyczące połączeń",
                                "Zaawansowane ustawienia dotyczące łączeniem "
                                "się do sieci WiFi oraz MQTT Brokera",
                                body);

  return page;
}

String AFESitesGenerator::addMQTTBrokerConfiguration() {

  MQTT configuration;
  configuration = Data.getMQTTConfiguration();

  String body = "<fieldset>";
  body += "<div class=\"cf\">";
  body += "<label>Adres MQTT Brokera (hostname)</label>";
  body += "<input name=\"mqtt_host\" type=\"text\" maxlength=\"32\" value=\"";
  body += configuration.host;
  body += "\">";
  body += "<span class=\"hint\">Max 32 znaki</span>";
  body += "</div>";

  body += "<div class=\"cf\">";
  body += "<label>Adres IP MQTT Brokera</label>";
  body += "<input name=\"mqtt_ip1\" type=\"number\" maxlength=\"3\" "
          "style=\"width:70px\" value=\"";
  body += configuration.ip[0];
  body += "\">.";
  body += "<input name=\"mqtt_ip2\" type=\"number\" maxlength=\"3\" "
          "style=\"width:70px\" value=\"";
  body += configuration.ip[1];
  body += "\">.";
  body += "<input name=\"mqtt_ip3\" type=\"number\" maxlength=\"3\" "
          "style=\"width:70px\" value=\"";
  body += configuration.ip[2];
  body += "\">.";
  body += "<input name=\"mqtt_ip4\" type=\"number\" maxlength=\"3\" "
          "style=\"width:70px\" value=\"";
  body += configuration.ip[3];
  body += "\">";
  body += "<span class=\"hint\">Opcjonalne jeśli wprowadzony został adres MQTT "
          "powyżej</span>";
  body += "</div>";

  body += "<div class=\"cf\">";
  body += "<label>Port</label>";
  body += "<input name=\"mqtt_port\" type=\"number\""
          "maxlength=\"5\" value=\"";
  body += configuration.port;
  body += "\">";
  body += "<span class=\"hint\">Informacja jest wymagana</span>";
  body += "</div>";
  body += "<div class=\"cf\">";
  body += "<label>Użytkownik</label>";
  body += "<input name=\"mqtt_user\" type=\"text\"  maxlength=\"32\" value=\"";
  body += configuration.user;
  body += "\">";
  body += "<span class=\"hint\">Max 32 znaki</span>";
  body += "</div>";
  body += "<div class=\"cf\">";
  body += "<label>Hasło</label>";
  body += "<input name=\"mqtt_password\" type=\"password\"  maxlength=\"32\" "
          "value=\"";
  body += configuration.password;
  body += "\">";
  body += "<span class=\"hint\">Max 32 znaki</span>";
  body += "</div>";
  body += "<div class=\"cf\">";
  body += "<label>Temat wiadomości</label>";
  body += "<input name=\"mqtt_topic\" type=\"text\""
          "maxlength=\"32\" value=\"";
  body += configuration.topic;
  body += "\">";
  body += "<span class=\"hint\">Informacja jest wymagana. Format: /abc/def/. "
          "Max 32 znaki.</span>";
  body += "</div>";
  body += "</fieldset>";

  return addConfigurationBlock("MQTT Broker",
                               "Urządzenie bez skonfigurowanego MQTT Brokera "
                               "może być sterowany tylko ręcznie. Wprowadź "
                               "adres hosta np. localhost lub adres IP",
                               body);
}

/* @TODO DOMOTICZ
String AFESitesGenerator::addDomoticzConfiguration() {

  DOMOTICZ configuration;
  configuration = Data.getDomoticzConfiguration();

  String body = "<fieldset>";
  body += "<div class=\"cf\">";
  body += "<label>IP serwera</label>";
  body += "<input name=\"domoticz_ip1\" type=\"number\" maxlength=\"3\" "
          "style=\"width:70px\" value=\"";
  body += configuration.ip[0];
  body += "\">.";
  body += "<input name=\"domoticz_ip2\" type=\"number\" maxlength=\"3\" "
          "style=\"width:70px\" value=\"";
  body += configuration.ip[1];
  body += "\">.";
  body += "<input name=\"domoticz_ip3\" type=\"number\" maxlength=\"3\" "
          "style=\"width:70px\" value=\"";
  body += configuration.ip[2];
  body += "\">.";
  body += "<input name=\"domoticz_ip4\" type=\"number\" maxlength=\"3\" "
          "style=\"width:70px\" value=\"";
  body += configuration.ip[3];
  body += "\">";
  body += "<span class=\"hint\">Informacja wymagana</span>";
  body += "</div>";
  body += "<div class=\"cf\">";
  body += "<label>Port</label>";
  body += "<input name=\"domoticz_port\" type=\"number\" "
          "maxlength=\"5\" value=\"";
  body += configuration.port;
  body += "\">";
  body += "<span class=\"hint\">Informacja wymagana</span>";
  body += "</div>";
  body += "<div class=\"cf\">";
  body += "<label>Użytkownik</label>";
  body +=
      "<input name=\"domoticz_user\" type=\"text\"  maxlength=\"32\" value=\"";
  body += configuration.user;
  body += "\">";
  body += "<span class=\"hint\">Max 32 znaki</span>";
  body += "</div>";
  body += "<div class=\"cf\">";
  body += "<label>Hasło</label>";
  body += "<input name=\"domoticz_password\" type=\"password\"  "
          "maxlength=\"32\" value=\"";
  body += configuration.password;
  body += "\">";
  body += "<span class=\"hint\">Max 32 znaki</span>";
  body += "</div>";

  body += "</fieldset>";

  return addConfigurationBlock("Serwer Domoticz",
                               "Konfiguracja wymagana jest jedynie kiedy jest "
                               "potrzeba wysyłania stanu przekaźnika do "
                               "serwera Domoticz",
                               body);
}
*/
String AFESitesGenerator::addRelayConfiguration(uint8_t id) {

  RELAY configuration;
  configuration = Data.getRelayConfiguration(id);

  String body = "<fieldset>";
  body += "<div class=\"cc\">";
  body += "<label>";
  body += "<input name=\"relay" + String(id) +
          "_present\" type=\"checkbox\" value=\"1\"";
  body += (configuration.present ? " checked=\"checked\"" : "");
  body += "> Włączony?";
  body += "</label>";
  body += "</div>";

  char filed[13];
  sprintf(filed, "relay%d_gpio", id);

  body += generateConfigParameter_GPIO(filed, configuration.gpio);

  /* @TODO For MQTT only */
  body += "<div class=\"cf\">";
  body += "<label>Nazwa</label>";
  body += "<input name=\"relay" + String(id) +
          "_name\" type=\"text\" maxlength=\"16\" value=\"";
  body += configuration.name;
  body += "\">";
  body += "<span class=\"hint\">Informacja wymagana. Max 16 znaków</span>";
  body += "</div>";

  /* @TODO DOMOTICZ
  body += "<div class=\"cf\">";
  body += "<label>IDX urządzenia</label>";
  body += "<input name=\"relay" + String(id) +
          "_idx\" type=\"number\" maxlength=\"3\" value=\"";
  body += configuration.idx;
  body += "\">";
  body += "<span class=\"hint\">Informacja wymagana. Do odczytania z "
          "konfiguracji Domoticz</span>";
  body += "</div>";

  body += "<div class=\"cc\">";
  body += "<label>";
  body += "<input name=\"relay" + String(id) +
          "_publish_to_domoticz\" type=\"checkbox\" value=\"1\"";
  body += (configuration.publishToDomoticz ? " checked=\"checked\"" : "");
  body += "> Publikowac stan przekaźnika do Domoticz?";
  body += "</label>";
  body += "</div>";

*/
  body += "<p class=\"cd\">Zachowanie przekaźnia po przywróceniu zasilania "
          "lub nawiązanie połączeni do brokera MQTT</p>";
  body += "<div class=\"cf\">";
  body += "<label>Przywrócenie zasilania</label>";
  body += "<select name=\"relay" + String(id) + "_power_restored\">";
  body += "<option value=\"0\"";
  body += (configuration.statePowerOn == 0 ? " selected=\"selected\"" : "");
  body += ">Brak akcji</option>";
  body += "<option value=\"1\"";
  body += (configuration.statePowerOn == 1 ? " selected=\"selected\"" : "");
  body += ">Wyłączony</option>";
  body += "<option value=\"2\"";
  body += (configuration.statePowerOn == 2 ? " selected=\"selected\"" : "");
  body += ">Włączony</option>";
  body += "<option value=\"3\"";
  body += (configuration.statePowerOn == 3 ? " selected=\"selected\"" : "");
  body += ">Ostatnia zapamiętana wartość</option>";
  body += "<option value=\"4\"";
  body += (configuration.statePowerOn == 4 ? " selected=\"selected\"" : "");
  body += ">Przeciwna do ostatniej zapamiętanej wartości</option>";
  body += "</select>";
  body += "</div>";

  /* @TODO For MQTT only */
  body += "<div class=\"cf\">";
  body += "<label>Włączony do MQTT</label>";
  body += "<select  name=\"relay" + String(id) + "_mqtt_connected\">";
  body += "<option value=\"0\"";
  body +=
      (configuration.stateMQTTConnected == 0 ? " selected=\"selected\"" : "");
  body += ">Brak akcji</option>";
  body += "<option value=\"1\"";
  body +=
      (configuration.stateMQTTConnected == 1 ? " selected=\"selected\"" : "");
  body += ">Wyłączony</option>";
  body += "<option value=\"2\"";
  body +=
      (configuration.stateMQTTConnected == 2 ? " selected=\"selected\"" : "");
  body += ">Włączony</option>";
  body += "<option value=\"3\"";
  body +=
      (configuration.stateMQTTConnected == 3 ? " selected=\"selected\"" : "");
  body += ">Ostatnia zapamiętana wartość</option>";
  body += "<option value=\"4\"";
  body +=
      (configuration.stateMQTTConnected == 4 ? " selected=\"selected\"" : "");
  body += ">Przeciwna do ostatniej zapamiętanej wartości</option>";
  body += "<option value=\"5\"";
  body +=
      (configuration.stateMQTTConnected == 5 ? " selected=\"selected\"" : "");
  body += ">Wartość z systemu sterowania</option>";
  body += "</select>";
  body += "</div>";

  body += "<p class=\"cd\">Ustaw poniższy parametr jeśli przekaźnik ma się "
          "wyłączyc automatycznie po zadanym czasie.</p>"
          "<p class=\"cd\">Wartośc 0: brak automatycznego wyłączania "
          "przekaźnika.</p>";

  body += "<div class=\"cf\">";
  body += "<label>Wyłącz po</label>";
  body += "<input name=\"relay" + String(id) +
          "_off_time\" type=\"number\" maxlength=\"5\" value=\"";
  body += configuration.timeToOff;
  body += "\">";
  body += "<span class=\"hint\">sekundach. Zakres 0.01sek do 99999sek</span>";
  body += "</div>";

  body += "</fieldset>";

  char title[23];
  sprintf(title, "Przkeźnik #%d", id);

  return addConfigurationBlock(
      title
      /* @TODO Set the description for MQTT or Domoticz */
      ,
      "Przekaźnik musi miec nadaną nazwę np. lampa. Nazwa przekaźnika "
      "definiuje dedykowany temat MQTT dla przekaźnika z tematu zdefiniowanego "
      "w sekcji MQTT broker oraz poniżej nazwy. np. /afe/lampa/",
      body);
}

String AFESitesGenerator::addSwitchConfiguration(uint8_t id) {

  SWITCH configuration;
  configuration = Data.getSwitchConfiguration(0);

  String body = "<fieldset>";
  body += "<div class=\"cc\">";
  body += "<label>";
  body += "<input name=\"switch" + String(id) +
          "_present\" type=\"checkbox\" value=\"1\"";
  body += (configuration.present ? " checked=\"checked\"" : "");
  body += "> Włączony?";
  body += "</label>";
  body += "</div>";

  body += "<div class=\"cf\">";
  body += "<label>Funkcja</label>";
  body += "<select name=\"switch" + String(id) + "_functionality\">";
  body += "<option value=\"0\"";
  body += (configuration.functionality == 0 ? " selected=\"selected\"" : "");
  body += ">-- Wybierz -- </option>";
  body += "<option value=\"1\"";
  body += (configuration.functionality == 1 ? " selected=\"selected\"" : "");
  body += ">Sterowanie przekaźnikiem</option>";
  body += "<option value=\"2\"";
  body += (configuration.functionality == 2 ? " selected=\"selected\"" : "");
  body += ">Multifunkcyjny</option>";
  body += "<option value=\"3\"";
  body += (configuration.functionality == 3 ? " selected=\"selected\"" : "");
  body += ">Reboot</option>";
  body += "<option value=\"4\"";
  body += (configuration.functionality == 4 ? " selected=\"selected\"" : "");
  body += ">Tryb konfiguracji</option>";
  body += "<option value=\"5\"";
  body += (configuration.functionality == 5 ? " selected=\"selected\"" : "");
  body += ">Aktualizacja oprogramowania</option>";
  body += "</select>";
  body += "<span class=\"hint\">Informacja wymagana.</span>";
  body += "</div>";

  char filed[13];
  sprintf(filed, "switch%d_gpio", id);

  body += generateConfigParameter_GPIO(filed, configuration.gpio);
  body += "<div class=\"cf\">";
  body += "<label>Typ</label>";
  body += "<select name=\"switch" + String(id) + "_type\">";
  body += "<option value=\"0\"";
  body += (configuration.type == 0 ? " selected=\"selected\"" : "");
  body += ">Mono-stabilny</option>";
  body += "<option value=\"1\"";
  body += (configuration.type == 1 ? " selected=\"selected\"" : "");
  body += ">Bi-stabilny</option>";
  body += "</select>";
  body += "</div>";
  body += "<div class=\"cf\">";
  body += "<label>Czułośc</label>";
  body += "<input name=\"switch" + String(id) +
          "_sensitivity\" type=\"number\" maxlength=\"3\" "
          "value=\"";
  body += configuration.sensitiveness;
  body += "\">";
  body += "<span class=\"hint\">msek. Zakres 0 do 999 msek</span>";
  body += "</div>";
  body += "</fieldset>";

  char title[23];
  sprintf(title, "Przycisk / włącznik #%d", id);

  return addConfigurationBlock(title,
                               "Ustawienie czułości włącznika należy ustawić "
                               "pod indywidulane potrzeby. Wysoka czułości "
                               "może powodować, że przekaźnik może zmieniać "
                               "swój stan nadmiernie dla niektórych włączników",
                               body);
}
/* @TODO DS18B20
String AFESitesGenerator::addDS18B20Configuration() {

  DS18B20 configuration;
  configuration = Data.getDS18B20Configuration();

  String body = "<fieldset>";
  body += "<div class=\"cc\">";
  body += "<label>";
  body += "<input name=\"ds18b20_present\" type=\"checkbox\" value=\"1\"";
  body += (configuration.present ? " checked=\"checked\"" : "");
  body += "> Włączony?";
  body += "</label>";
  body += "</div>";
  body += generateConfigParameter_GPIO("ds18b20_gpio", configuration.gpio);
  body += "<div class=\"cf\">";
  body += "<label>Odczyty co</label>";
  body += "<input name=\"ds18b20_interval\" type=\"number\" maxlength=\"5\" "
          "value=\"";
  body += configuration.interval;
  body += "\">";
  body += "<span class=\"hint\">sekund. Zakres 10sek do 99999sek</span>";
  body += "</div>";
  body += "<div class=\"cf\">";
  body += "<label>Korekta wartości o</label>";
  body += "<input name=\"ds18b20_correction\" type=\"number\" maxlength=\"6\" "
          "value=\"";
  body += configuration.correction;
  body += "\">";
  body += "<span class=\"hint\">stopni. Zakres -9.99 do 9.99, </span>";
  body += "</div>";
  body += "<div class=\"cf\">";
  body += "<label>Jednostka</label>";
  body += "<select  name=\"ds18b20_unit\">";
  body += "<option value=\"0\"";
  body += (configuration.unit == 0 ? " selected=\"selected\"" : "");
  body += ">celsjusz</option>";
  body += "<option value=\"1\"";
  body += (configuration.unit == 1 ? " selected=\"selected\"" : "");
  body += ">fahrenheit</option>";
  body += "</select>";
  body += "</div>";

  body += "</fieldset>";

  return addConfigurationBlock("Czujnik temperatury DS18B20",
                               "DS18B20 jest obsługiwanym czujnikiem "
                               "temperatury. Możliwe jest podłaczenie tylko "
                               "jednego czujnika temperatury",
                               body);
}
*/
String AFESitesGenerator::addUpgradeSection() {
  String body = "<fieldset>";
  body += "<div class=\"cf\">";
  body += "<label>Wybierz firmware</label>";
  body += "<input class=\"bs\" name=\"update\" type=\"file\" "
          "accept=\".bin\" "
          "value=\"W:\\AFE-Firmware\\.pioenvs\\esp01_1m\\firmware.bin\">";
  body += "</div>";
  body += "<p class=\"cm\">Po zakończeniu aktualizacji urządzenie zostanie "
          "automatycznie zresetowane</p>";
  body += "<button type=\"submit\" class=\"b be\">Aktualizuj</button>";
  body += "</fieldset>";
  return addConfigurationBlock("Aktualizacja oprogramowania",
                               "<strong>UWAGA</strong>: funkcjonalnośc tylko "
                               "dla zaawansowanych użytkowników. Nie odłączaj "
                               "urządzenia od zasilania podczas aktualizacji.",
                               body);
}

String AFESitesGenerator::addPostUpgradeSection(boolean status) {

  String body = "<fieldset>";
  body += status ? "<p style=\"color:red\">Aktualizacja nie powiodła się</p>"
                 : "<p class=\"cm\">Aktualizacja zakończona pomyślnie!</p><p "
                   "class=\"cm\">Po 10 "
                   "sekundach przełącznik zostanie przeładowany z wgranym "
                   "oprogramowaniem. Proszę czekać.</p>";
  body += "</fieldset>";
  return addConfigurationBlock("Aktualizacja oprogramowania",
                               "Status aktualizacji", body);
}

String AFESitesGenerator::addResetSection(uint8_t command) {
  String body = "<fieldset>";
  String subtitle;
  if (command == 0) {
    body += "<a href=\"\\?option=reset&command=1\" class=\"b be\">Przywróc "
            "ustawienia początkowe</a>";
    subtitle = "<strong>Uwaga</strong>: przywracanie ustawień "
               "początkowych kasuje wszystkie ustawienia "
               "urządzenia, włącznie z konfiguracją sieci WiFi";
  } else {
    subtitle += "Trwa przywracanie ustawień początkowych</strong>";
    body += "<p class=\"cm\">Po 20 sekundach połącz się z siecią WiFi o "
            "nazwie: <strong>AFE-Device</strong>, a następnie połącz się się z "
            "panelem konfigurayjnym pod adresem: </p>";
    body += "<a href=\"http://192.168.5.1\">http://192.168.5.1</a>";
  }
  body += "</fieldset>";
  return addConfigurationBlock("Przywracanie ustawień początkowych", subtitle,
                               body);
}

String AFESitesGenerator::addExitSection() {
  String body = "<fieldset>";
  body += "<div class=\"cf\">";
  body += "<p class=\"cm\">Trwa restart urzadzenia.....</p>";
  body += "<p class=\"cm\">Strona zostanie przeładowana automatycznie... "
          "czekaj</p>";
  body += "</div>";
  body += "</fieldset>";
  return addConfigurationBlock("Restart urządzenia", "", body);
}

String AFESitesGenerator::addHelpSection() {
  NETWORK NetworkConfiguration;
  NetworkConfiguration = Data.getNetworkConfiguration();

  String body = "<fieldset>";
  body += "<div class=\"cf\">";
  body += "<label>Nazwa urządzenia</label>";
  body += "<span>";
  body += NetworkConfiguration.host;
  body += "</span>";
  body += "</div>";
  body += "</fieldset>";
  return addConfigurationBlock("Urządzenie działa poprawnie", "", body);
}

const String AFESitesGenerator::generateMQTTHelp(const char *label,
                                                 const char *topic,
                                                 const char *command,
                                                 const char *value) {
  String body;

  body += "<p></p><p class=\"cm\">";
  body += label;
  body += "</p><span><label>Temat : </label>";
  body += topic;
  body += command;
  body += "</span><br><span><label>Wiadomośc :</label> ";
  body += value;
  body += "</span>";

  return body;
}

String AFESitesGenerator::addHelpMQTTTopicSection() {
  MQTT MQTTConfiguration;
  MQTTConfiguration = Data.getMQTTConfiguration();

  RELAY RelayConfiguration;
  RelayConfiguration = Data.getRelayConfiguration(0);

  String body = "<fieldset>";
  body += "<div class=\"cf\">";

  body += "<p class=\"cm\" style=\"color:black\"><strong>Urządzenie jest "
          "zasubskrybowane do następujących tematów MQTT</strong></p>";

  body += generateMQTTHelp("Restart urządzenia", MQTTConfiguration.topic, "cmd",
                           "reboot");

  body += generateMQTTHelp("Uruchomienie trybu konfiguracji",
                           MQTTConfiguration.topic, "cmd", "configurationMode");

  body += generateMQTTHelp("Włączenie przekaźnika",
                           RelayConfiguration.mqttTopic, "cmd", "ON");
  body += generateMQTTHelp("Wyłączenie przekaźnika",
                           RelayConfiguration.mqttTopic, "cmd", "OFF");
  body += generateMQTTHelp("Pobranie stanu przekaźnika",
                           RelayConfiguration.mqttTopic, "cmd", "reportState");

  body += "<p></p><p></p><p class=\"cm\" "
          "style=\"color:black\"><strong>Urządzenie publikuje następujące "
          "tematy</strong></p>";

  body += generateMQTTHelp("Stan przekaźnika", RelayConfiguration.mqttTopic,
                           "state", "ON lub OFF");
  body += generateMQTTHelp("Pobierz wartośc przekaźnika z serwera",
                           RelayConfiguration.mqttTopic, "get", "defaultState");

  body += "</div>";
  body += "</fieldset>";
  return addConfigurationBlock("Pomoc: tematy MQTT", "", body);
}
