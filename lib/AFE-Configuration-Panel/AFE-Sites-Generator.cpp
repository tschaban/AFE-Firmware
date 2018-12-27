#include "AFE-Sites-Generator.h"

AFESitesGenerator::AFESitesGenerator() { language = Data.getLanguage(); }

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
    page += "; url=/\">";
  }

  page += "<title>AFE Firmware ";
  page += configuration.version;
  page += " [T";
  page += configuration.type;
  page +=
      "]</title>"
      "<style>#l,#r{padding:20px}.ltag,.ltit,body{margin:0}.b,.itm "
      "a,a{text-decoration:none}body{background:#ddd;padding:15px}#c{display:"
      "table;width:100%}#l,#r{display:table-cell}#l{width:300px;background:#"
      "282828;color:#eee}#r{background:#eee}a,button,h1,h3,h4,input,label,li,p,"
      "span{font-family:sans-serif}.ltag{font-weight:300;color:#b0cadb;margin-"
      "bottom:20px}.lst{list-style:none;margin:0;padding:0}.itm "
      "a{display:block;white-space:nowrap;padding:.2em "
      "1em;color:#777;font-size:95%}.cf "
      "label,.hint,input,select{display:inline-block;vertical-align:middle}."
      "itm a:focus,.itm "
      "a:hover{background-color:#eee;text-decoration:none;padding:.2em "
      "1.5em;color:#000}.ci{margin-bottom:2em}.ci "
      "h1{color:#aaa;border-bottom:1px solid "
      "#ddd;font-size:110%;font-weight:500;letter-spacing:.1em}.ci "
      ".cd{color:#444;line-height:1.8em;font-size:80%;font-style:italic}.cm{"
      "color:#999;font-size:90%;margin:0 0 20px}.la{margin:0 .1em;padding:.3em "
      "1em;color:#fff;background:#999;font-size:80%}.cc label,.cf "
      "label{font-size:.875em}.lr{background:#ca3c3c}.bs,.lg{background:#"
      "2fb548}fieldset{margin:0;padding:.35em 0 "
      ".75em;border:0}.cf{margin-bottom:.5em}.cc{margin:1em 0 .5em 9.4em}.cf "
      "label{text-align:right;width:10em;margin:0 1em 0 "
      "0}input,select{padding:.5em .6em;border:1px solid "
      "#ccc;box-sizing:border-box}.hint{padding-left:.3em;color:#aaa;font-size:"
      "80%}.b{font-size:100%;padding:.5em 1em;border:1px solid "
      "#999;border:transparent;color:#fff}.be{background:#ca3c3c}.bw{"
      "background:#df7514}.bc{background:#42b8dd}.b:focus,.b:hover{filter:"
      "alpha(opacity=90);background-image:-webkit-linear-gradient(transparent,"
      "rgba(0,0,0,.05) "
      "40%,rgba(0,0,0,.1));background-image:linear-gradient(transparent,rgba(0,"
      "0,0,.05) 40%,rgba(0,0,0,.1))}</style>"
      "</head>"
      "<body>"
      "<div id=\"c\">"
      "<div id=\"l\">"
      "<h3 class=\"ltit\">AFE FIRMWARE</h3>"
      "<h4 class=\"ltag\">";
#ifdef T0_CONFIG
  page += language == 0 ? "Włącznik WiFi" : "WiFi Switch";
#elif T0_SHELLY_1_CONFIG
  page += language == 0 ? "dla Shelly-1" : "for Shelly-1";
#elif T1_CONFIG
  page += language == 0 ? "Włącznik z czujnikiem temperatury"
                        : "Switch with temperature sensor";
#elif T2_CONFIG
  page += language == 0 ? "Włącznik z czujnikiem temperatury i wilgotności"
                        : "Switch with temperature and humidity sensor";
#elif T3_CONFIG
  page += language == 0 ? "dla 4 włączników i czujników ruchu"
                        : "for 4 WiFi switches and PIR sensors";
#elif T4_CONFIG
  page += language == 0 ? "dla 4 włączników WiFi" : "for 4 WiFi switches";
#elif T5_CONFIG
  page += language == 0 ? "do kontrolowania sterownika bramy"
                        : "to control gate controller";
#elif T6_CONFIG
  page += language == 0 ? "Stacja Pogody" : "Wheater Station";
#endif

  page += "</h4><h4>MENU</h4>"
          "<ul class=\"lst\">";
  if (Device.getMode() != MODE_NORMAL) {
    Device.begin(); // Reading configuration data
    page += "<li class=\"itm\"><a href=\"\\?option=device\">";
    page += language == 0 ? "Urządzenie" : "Device";
    page += "</a></li> "
            "<li class=\"itm\"><a href=\"\\?option=network\">";
    page += language == 0 ? "Sieć WiFi" : "Network";
    page += "</a></li>";
    if (Device.configuration.mqttAPI) {
      page += "<li class=\"itm\"><a "
              "href=\"\\?option=mqtt\">MQTT "
              "Broker</a></li>";
    }
    if (Device.configuration.domoticzAPI) {
      page += "<li class=\"itm\"><a "
              "href=\"\\?option=domoticz\">";
      page += language == 0 ? "Serwer Domoticz" : "Domoticz Server";
      page += "</a></li>";
    }

    uint8_t itemPresent = 0;

    for (uint8_t i = 0; i < sizeof(Device.configuration.isLED); i++) {
      if (Device.configuration.isLED[i]) {
        itemPresent++;
      } else {
        break;
      }
    }

    if (itemPresent > 0) {
      page += "<li class=\"itm\"><a href=\"\\?option=led\">LED";
      page += language == 0 ? "y" : "s";
      page += "</a></li>";
    }

    /* Relay */
    itemPresent = 0;
    for (uint8_t i = 0; i < sizeof(Device.configuration.isRelay); i++) {
      if (Device.configuration.isRelay[i]) {
        itemPresent++;
      } else {
        break;
      }
    }

    if (itemPresent > 0) {
      page += "<li  class=\"itm\"><a><i>";
      page +=
          language == 0 ? "Konfiguracja przekaźników" : "Relays configuration";
      page += "</i></a></li>";

      for (uint8_t i = 0; i < itemPresent; i++) {
        page += "<li class=\"itm\"><a href=\"\\?option=relay";
        page += i;
        page += "\">&#8227; ";
        page += language == 0 ? "Przekaźnik" : "Relay";
        page += ": ";
        page += i + 1;
        page += "</a></li>";

/* Thermostat */
#if defined(T1_CONFIG)
        if (Device.configuration.isDS18B20) {
          page += "<li class=\"itm\"><a href=\"\\?option=thermostat\">&#8227; ";
          page += language == 0 ? "Termostat" : "Thermostat";
          page += "</a></li>";
        }
#endif

/* Humidistat */
#if defined(T2_CONFIG)
        if (Device.configuration.isDHT) {
          page += "<li class=\"itm\"><a href=\"\\?option=thermostat\">&#8227; ";
          page += language == 0 ? "Termostat" : "Thermostat";
          page += "</a></li>";
          page += "<li class=\"itm\"><a href=\"\\?option=humidistat\">&#8227; ";
          page += language == 0 ? "Regulator wilgotności" : "Humidistat";
          page += "</a></li>";
        }
#endif
      }
    }

    /* Switch */
    itemPresent = 0;
    for (uint8_t i = 0; i < sizeof(Device.configuration.isSwitch); i++) {
      if (Device.configuration.isSwitch[i]) {
        itemPresent++;
      } else {
        break;
      }
    }

    if (itemPresent > 0) {
      page += "<li  class=\"itm\"><a><i>";
      page += language == 0 ? "Przyciski / wyłączniki" : "Buttons / Switches";
      page += "</i></a></li>";

      for (uint8_t i = 0; i < itemPresent; i++) {
        page += "<li class=\"itm\"><a href=\"\\?option=switch";
        page += i;
        page += "\">&#8227; ";
        page += language == 0 ? "Przycisk: " : "Switch: ";
        page += i + 1;
        page += "</a></li>";
      }
    }

    /* Pir */
#if defined(T3_CONFIG)
    itemPresent = 0;
    for (uint8_t i = 0; i < sizeof(Device.configuration.isPIR); i++) {
      if (Device.configuration.isPIR[i]) {
        itemPresent++;
      } else {
        break;
      }
    }

    if (itemPresent > 0) {
      page += "<li class=\"itm\"><a><i>Konfiguracja "
              "czujników ruchu (PIR)</i></a></li>";
      for (uint8_t i = 0; i < 4; i++) {
        if (Device.configuration.isPIR[i]) {
          page += "<li class=\"itm\"><a href=\"\\?option=pir";
          page += i;
          page += "\">&#8227; Czujnik: ";
          page += i + 1;
          page += "</a></li>";
        }
      }
    }

#endif

/* Contactrons and Gate */
#if defined(T5_CONFIG)
    itemPresent = 0;
    for (uint8_t i = 0; i < sizeof(Device.configuration.isContactron); i++) {
      if (Device.configuration.isContactron[i]) {
        itemPresent++;
      } else {
        break;
      }
    }

    if (itemPresent > 0) {
      page += "<li class=\"itm\"><a><i>";
      page += language == 0 ? "Czujniki magnetyczne" : "Magnetic sensors";
      page += "</i></a></li>";
      for (uint8_t i = 0; i < itemPresent; i++) {
        page += "<li class=\"itm\"><a href=\"\\?option=contactron";
        page += i;
        page += "\"> - ";
        page += language == 0 ? "Czujnik: " : "Sensor: ";
        page += i + 1;
        page += "</a></li>";
      }

      page += "<li class=\"itm\"><a href=\"\\?option=gate\">";
      page +=
          language == 0 ? "Konfiguracja bram/drzwi" : "Gate/Door configuration";
      page += "</a></li>";
    }

#endif

/* Sensor DS18B20 */
#ifdef T1_CONFIG
    if (Device.configuration.isDS18B20) {
      page += "<li class=\"itm\"><a href=\"\\?option=ds18b20\">";
      page += language == 0 ? "Czujnik temperatury" : "Temperature sensor";
      page += "</a></li>";
    }
#endif

/* Sensor DHxx */
#if defined(T2_CONFIG) || defined(T5_CONFIG)
    if (Device.configuration.isDHT) {
      page += "<li class=\"itm\"><a href=\"\\?option=DHT\">";
      page += language == 0 ? "Czujnik DHT" : "DHT sensor";
      page += "</a></li>";
    }
#endif

/* UART */
#if defined(T6_CONFIG)
    if (Device.configuration.isHPMA115S0) {
      page += "<li class=\"itm\"><a href=\"\\?option=UART\">UART</a></li>";
      page += "<li class=\"itm\"><a href=\"\\?option=HPMA115S0\">";
      page += language == 0 ? "Czujnik cząstek PM2.5/PM10"
                            : "PM2.5/PM10 Particle Sensor";
      page += "</a></li>";
    }

    if (Device.configuration.isBME680) {
      page += "<li class=\"itm\"><a href=\"\\?option=BME680\">";
      page += language == 0 ? "Czujnik BME680" : "BME680 Sensor";
      page += "</a></li>";
    }

    if (Device.configuration.isBH1750) {
      page += "<li class=\"itm\"><a href=\"\\?option=BH1750\">";
      page += language == 0 ? "Czujnik BH1750" : "BH1750 Sensor";
      page += "</a></li>";
    }
#endif

    /* Language, Upgrade, Exit */
    page += "<br><br><li class=\"itm\"><a "
            "href=\"\\?option=language\">[PL] Język / "
            "[EN] Language</a></li><br><br><li class=\"itm\"><a "
            "href=\"\\update\">";
    page += language == 0 ? "Aktulizacja firmware" : "Firmware upgrade";
    page += "</a></li><li class=\"itm\"><a href=\"\\?option=reset\">";
    page += language == 0 ? "Przywracanie ustawień początkowych"
                          : "Reset to orginal state";
    page += "</a></li><br><br><li class=\"itm\"><a href=\"\\?option=exit\">";
    page += language == 0 ? "Zakończ konfigurację" : "Finish configuration";
  } else {
    /* Settings, Access Point */
    page += "<li class=\"itm\"><a href=\"\\?option=help&cmd=1\">";
    page += language == 0 ? "Ustawienia" : "Settings";
    page += "</a></li><li class=\"itm\"><a href=\"\\?option=help&cmd=2\">";
    page += language == 0 ? "Ustawienia (tryb:" : "Settings (mode:";
    page += " Access Point)";
  }
  /* Information section */
  page += "</a></li></ul><br><br><h4>INFORMA";
  page += language == 0 ? "CJE" : "TION";
  page += "</h4><ul class=\"lst\"><li class=\"itm\"><a "
          "href=\"https://www.smartnydom.pl/afe-firmware-";
  page += language == 0 ? "pl" : "en";
  page += "/\" target=\"_blank\">Do";
  page += language == 0 ? "kumentacja" : "cumentation";
  page += "</a></li><li class=\"itm\"><a "
          "href=\"https://www.smartnydom.pl/forum/afe-firmware/\" "
          "target=\"_blank\">";
  page += language == 0 ? "Pomoc" : "Help";
  page += "</a></li><li class=\"itm\"><a "
          "href=\"https://github.com/tschaban/AFE-Firmware/blob/master/"
          "LICENSE\" "
          "target=\"_blank\">Licenc";
  page += language == 0 ? "ja" : "e";
  page += "</a></li><li class=\"itm\"><a "
          "href=\"https://www.smartnydom.pl/afe-firmware-";
  page += language == 0 ? "pl" : "en";
  page += "/log\" target=\"_blank\">";
  page += language == 0 ? "Wersja" : "Version";
  page += " ";
  page += configuration.version;
  page += " [T";
  page += configuration.type;
  page += "]</a></li>"
          "</ul>";

  page += "</div>"
          "<div id=\"r\">";

  return page;
}

String AFESitesGenerator::addDeviceConfiguration() {
  DEVICE configuration = Data.getDeviceConfiguration();
  uint8_t itemsNumber = 0;

  String body = "<fieldset><div class=\"cf\"><label>";
  body += language == 0 ? "Nazwa urządzenia" : "Device name";
  body += "</label><input name=\"dn\" type=\"text\" maxlength=\"16\" "
          "value=\"";
  body += configuration.name;
  body += "\"><span class=\"hint\">Max 16 ";
  body += language == 0 ? "znaków" : "chars";
  body += "</span></div></fieldset>";

  String page = addConfigurationBlock(
      language == 0 ? "Urządzenie" : "Device",
      language == 0
          ? "Nazwa jest wykorzystywana jako nazwa urządzenia w Twojej "
            "lokalnej sieci WiFi oraz jako nazwa hotspot'a urządzenia"
          : "Name is used for device identification within your LAN and "
            "a device's hotspot name used for configuration mode",
      body);

  body = "<fieldset>";

/* LED */
#ifndef T0_SHELLY_1_CONFIG
  for (uint8_t i = 0; i < sizeof(Device.configuration.isLED); i++) {
    if (Device.configuration.isLED[i]) {
      itemsNumber++;
    } else {
      break;
    }
  }

  body += generateHardwareItemsList(
      sizeof(Device.configuration.isLED), itemsNumber, "hl",
      language == 0 ? "Ilość Led'ów" : "Number of LEDs");

#endif

/* Contactrons */
#if defined(T5_CONFIG)
  itemsNumber = 0;
  for (uint8_t i = 0; i < sizeof(Device.configuration.isContactron); i++) {
    if (Device.configuration.isContactron[i]) {
      itemsNumber++;
    } else {
      break;
    }
  }
  body += generateHardwareItemsList(
      sizeof(Device.configuration.isContactron), itemsNumber, "hc",
      language == 0 ? "Ilość czujników magnetycznych "
                    : "Number of magnetic sensors");

  itemsNumber = 0;
  for (uint8_t i = 0; i < sizeof(Device.configuration.isSwitch); i++) {
    if (Device.configuration.isSwitch[i]) {
      itemsNumber++;
    } else {
      break;
    }
  }
#else
  /* Relay */
  itemsNumber = 0;
  for (uint8_t i = 0; i < sizeof(Device.configuration.isRelay); i++) {
    if (Device.configuration.isRelay[i]) {
      itemsNumber++;
    } else {
      break;
    }
  }

  body += generateHardwareItemsList(
      sizeof(Device.configuration.isRelay), itemsNumber, "hr",
      language == 0 ? "Ilość przekaźników" : "Number of relay");
#endif

  /* Switch */
  itemsNumber = 0;
  for (uint8_t i = 0; i < sizeof(Device.configuration.isSwitch); i++) {
    if (Device.configuration.isSwitch[i]) {
      itemsNumber++;
    } else {
      break;
    }
  }

  body += generateHardwareItemsList(
      sizeof(Device.configuration.isSwitch), itemsNumber, "hs",
      language == 0 ? "Ilość przycisków" : "Number of switches");

#if defined(T1_CONFIG) || defined(T2_CONFIG) || defined(T5_CONFIG)
  body += "<div class=\"cc\"><label><input name =\"ds\" type=\"checkbox\" "
          "value=\"1\"";
#if defined(T1_CONFIG)
  body += configuration.isDS18B20 ? " checked=\"checked\">" : ">";
#elif defined(T2_CONFIG) || defined(T5_CONFIG)
  body += configuration.isDHT ? " checked=\"checked\">" : ">";
  body += language == 0 ? "Czujnik" : " Sensor";
#endif

#if defined(T1_CONFIG)
  body += " DS18B20";
#elif defined(T2_CONFIG) || defined(T5_CONFIG)
  body += " DHT";
#endif
  body += "</label></div>";
#endif

#if defined(T6_CONFIG)
  body += "<div class=\"cc\"><label><input name =\"ds\" type=\"checkbox\" "
          "value=\"1\"";
  body += configuration.isHPMA115S0 ? " checked=\"checked\">" : ">";
  body += language == 0 ? "Czujnik" : " Sensor";
  body += " HPMA115S0";
  body += "</label></div>";

  body += "<div class=\"cc\"><label><input name =\"b6\" type=\"checkbox\" "
          "value=\"1\"";
  body += configuration.isBME680 ? " checked=\"checked\">" : ">";
  body += language == 0 ? "Czujnik" : " Sensor";
  body += " BME680";
  body += "</label></div>";

  body += "<div class=\"cc\"><label><input name =\"bh\" type=\"checkbox\" "
          "value=\"1\"";
  body += configuration.isBH1750 ? " checked=\"checked\">" : ">";
  body += language == 0 ? "Czujnik" : " Sensor";
  body += " BH1750";
  body += "</label></div>";

#endif

#if defined(T3_CONFIG)
  itemsNumber = 0;
  for (uint8_t i = 0; i < sizeof(Device.configuration.isPIR); i++) {
    if (Device.configuration.isPIR[i]) {
      itemsNumber++;
    }
  }

  body += generateHardwareItemsList(
      sizeof(Device.configuration.isPIR), itemsNumber, "hp",
      language == 0 ? "Ilość czujników PIR" : "Number of PIRs");
#endif

  body += "</fieldset>";

  page += addConfigurationBlock(
      language == 0 ? "Konfiguracja urządzenia" : "Hardware configuration",
      language == 0 ? "Wybierz podłączone elementy do urządzenia"
                    : "Select connected items to your device",
      body);

  body = "<fieldset><div class=\"cc\"><label><input name=\"m\" "
         "type=\"checkbox\" value=\"1\"";
  body += configuration.mqttAPI ? " checked=\"checked\"" : "";
  body += ">MQTT API ";
  body += language == 0 ? "włączone" : "enabled";
  body += "?</label></div><div class=\"cc\"><label><input name=\"h\" "
          "type=\"checkbox\" value=\"1\"";
  body += configuration.httpAPI ? " checked=\"checked\"" : "";
  body += ">HTTP API ";
  body += language == 0 ? "włączone" : "enabled";
  body += "?</label></div>";

  body += "<div class=\"cc\"><label><input name=\"d\" type=\"checkbox\" "
          "value=\"1\"";
  body += configuration.domoticzAPI ? " checked=\"checked\"" : "";
  body += ">Domoticz API ";
  body += language == 0 ? "włączone" : "enabled";
  body += "?</label></div>";

  body += "</fieldset>";

  page += addConfigurationBlock(
      language == 0 ? "Sterowanie urządzeniem" : "Device controlling mechanism",
      language == 0
          ? "Włączanie / Wyłączanie mechanizmów sterowania urządzeniem"
          : "Enable / Disable APIs",
      body);

  return page;
}

String AFESitesGenerator::addNetworkConfiguration() {

  NETWORK configuration;
  configuration = Data.getNetworkConfiguration();

  String body = "<fieldset>";
  body += "<div class=\"cf\"><label>";
  body += language == 0 ? "Nazwa sieci WiFi" : "WiFi name";
  body += "</label><input name=\"s\" type=\"text\" maxlength=\"32\" value=\"";
  body += configuration.ssid;
  body += "\"><span class=\"hint\">Max 32 ";
  body += language == 0 ? "znaków" : "chars";
  body += "</span></div><div class=\"cf\"><label>";
  body += language == 0 ? "Hasło" : "Password";
  body += "</label><input type=\"password\" name=\"p\" maxlength=\"32\" "
          "value=\"";
  body += configuration.password;
  body += "\"><span class=\"hint\">Max 32 ";
  body += language == 0 ? "znaków" : "chars";
  body += "</span></div><div class=\"cf\"><label>MAC</label><input "
          "type=\"text\" readonly=\"readonly\" value=\"";
  body += WiFi.macAddress();
  body += "\"></div></fieldset>";

  String page = addConfigurationBlock(
      "WiFi",
      language == 0 ? "Urządzenie bez dostępu do sieci będzie "
                      "działać tylko w trybie sterowania ręcznego"
                    : "Device without access to network will only work in "
                      "manual mode",
      body);

  body = "<fieldset><div class=\"cc\"><label><input name=\"d\" "
         "type=\"checkbox\" value=\"1\"";
  body += (configuration.isDHCP ? " checked=\"checked\"" : "");
  body += "> ";
  body +=
      language == 0 ? "Konfiguracja przez DHCP?" : "Configuration over DHCP";
  body += "</label></div>";

  body += "<div class=\"cf\"><label>";
  body += language == 0 ? "Adres IP" : "IP Addess";
  body += "</label><input name=\"d1\" type=\"number\" max=\"255\" min=\"0\" "
          "step=\"1\" "
          "style=\"width:70px\" value=\"";
  body += configuration.ip[0];
  body +=
      "\">.<input name=\"d2\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
      "style=\"width:70px\" value=\"";
  body += configuration.ip[1];
  body +=
      "\">.<input name=\"d3\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
      "style=\"width:70px\" value=\"";
  body += configuration.ip[2];
  body += "\">.";
  body += "<input name=\"d4\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
          "style=\"width:70px\" value=\"";
  body += configuration.ip[3];
  body += "\"></div><div class=\"cf\"><label>";
  body += language == 0 ? "Bramka" : "Gateway";
  body += "</label><input name=\"g1\" type=\"number\" max=\"255\" min=\"0\" "
          "step=\"1\" "
          "style=\"width:70px\" value=\"";
  body += configuration.gateway[0];
  body +=
      "\">.<input name=\"g2\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
      "style=\"width:70px\" value=\"";
  body += configuration.gateway[1];
  body +=
      "\">.<input name=\"g3\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
      "style=\"width:70px\" value=\"";
  body += configuration.gateway[2];
  body +=
      "\">.<input name=\"g4\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
      "style=\"width:70px\" value=\"";
  body += configuration.gateway[3];
  body += "\"></div>";

  body += "<div class=\"cf\"><label>";
  body += language == 0 ? "Maska sieci" : "Subnet";
  body += "</label><input name=\"s1\" type=\"number\" max=\"255\" min=\"0\" "
          "step=\"1\" "
          "style=\"width:70px\" value=\"";
  body += configuration.subnet[0];
  body +=
      "\">.<input name=\"s2\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
      "style=\"width:70px\" value=\"";
  body += configuration.subnet[1];
  body +=
      "\">.<input name=\"s3\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
      "style=\"width:70px\" value=\"";
  body += configuration.subnet[2];
  body +=
      "\">.<input name=\"s4\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
      "style=\"width:70px\" value=\"";
  body += configuration.subnet[3];
  body += "\"></div></fieldset>";

  page += addConfigurationBlock(
      language == 0 ? "IP urządzenia" : "IP address",
      language == 0 ? "Możesz skonfigurować adres IP urządzenia ręcznie lub "
                      "automatycznie za pośrednictwem DHCP"
                    : "You can set up device's IP address manually or "
                      "automatically by DHCP",
      body);

  body = "<fieldset><div class=\"cf\"><label>";
  body += language == 0 ? "Ilość prób łączenia się"
                        : "Number of connection attempts";
  body += "</label><input name=\"na\" type=\"number\" min=\"1\" max=\"255\" "
          "step=\"1\" "
          "value=\"";
  body += configuration.noConnectionAttempts;
  body += "\"><span class=\"hint\">1-255</span></div><div class=\"cf\"><label>";
  body += language == 0 ? "Czas między próbami łączenia się"
                        : "Time between connections' attempts";
  body += "</label><input type=\"number\" name=\"wc\" "
          "min=\"1\" max=\"255\" step=\"1\" value=\"";
  body += configuration.waitTimeConnections;
  body += "\"><span class=\"hint\">1-255 (";
  body += language == 0 ? "sekundy" : "seconds";
  body += ")</span></div><div class=\"cf\"><label>";
  body += language == 0 ? "Czas po jakim powtórzyć próby łączenia się"
                        : "Sleep time before next connection attempts";
  body += "</label><input type=\"number\" name=\"ws\" min=\"1\" max=\"255\" "
          "step=\"1\" "
          "value=\"";
  body += configuration.waitTimeSeries;
  body += "\"><span class=\"hint\">1-255 (";
  body += language == 0 ? "sekundy" : "seconds";
  body += ")</span></div></fieldset>";

  page += addConfigurationBlock(language == 0 ? "Zaawansowane" : "Advanced", "",
                                body);

  return page;
}

String AFESitesGenerator::addMQTTBrokerConfiguration() {

  MQTT configuration;
  configuration = Data.getMQTTConfiguration();

  String body = "<fieldset><div class=\"cf\"><label>Hostname</label><input "
                "name=\"h\" type=\"text\" maxlength=\"32\" value=\"";
  body += configuration.host;
  body += "\"><span class=\"hint\">Max 32 ";
  body += language == 0 ? "znaków" : "chars";
  body += "</span></div><div class=\"cf\"><label>";
  body += language == 0 ? "Adres IP" : "IP address";
  body += "</label><input name=\"m1\" type=\"number\" max=\"255\" min=\"0\" "
          "step=\"1\" "
          "style=\"width:70px\" value=\"";
  body += configuration.ip[0];
  body +=
      "\">.<input name=\"m2\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
      "style=\"width:70px\" value=\"";
  body += configuration.ip[1];
  body +=
      "\">.<input name=\"m3\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
      "style=\"width:70px\" value=\"";
  body += configuration.ip[2];
  body +=
      "\">.<input name=\"m4\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
      "style=\"width:70px\" value=\"";
  body += configuration.ip[3];
  body += "\"></div><div class=\"cf\"><label>Port</label><input name=\"p\" "
          "type=\"number\""
          " min=\"0\" max=\"65535\" step=\"1\" value=\"";
  body += configuration.port;
  body += "\"></div><div class=\"cf\"><label>";
  body += language == 0 ? "Użytkownik" : "User";
  body += "</label><input name=\"u\" type=\"text\"  maxlength=\"32\" value=\"";
  body += configuration.user;
  body += "\"><span class=\"hint\">Max 32 ";
  body += language == 0 ? "znaków" : "chars";
  body += "</span></div><div class=\"cf\"><label>";
  body += language == 0 ? "Hasło" : "Password";
  body += "</label><input name=\"s\" type=\"password\"  maxlength=\"32\" "
          "value=\"";
  body += configuration.password;
  body += "\"><span class=\"hint\">Max 32 ";
  body += language == 0 ? "znaków" : "chars";
  body += "</span></div><div class=\"cf\"><label>";
  body += language == 0 ? "Temat wiadomości" : "MQTT Topic";
  body += "</label><input name=\"t\" type=\"text\""
          "maxlength=\"32\" value=\"";
  body += configuration.topic;
  body += "\"><span class=\"hint\">Format <strong>/abc/def/</strong>. Max 32 ";
  body += language == 0 ? "znaków" : "chars";
  body += "</span></div></fieldset>";

  return addConfigurationBlock(
      "MQTT Broker",
      language == 0 ? "Wprowadź adres hosta np. localhost lub adres IP"
                    : "Enter MQTT Broker hostname or its IP address",
      body);
}

String AFESitesGenerator::addDomoticzServerConfiguration() {
  DOMOTICZ configuration;
  configuration = Data.getDomoticzConfiguration();

  String body = "<fieldset>";

  body += "<div class=\"cf\"><label>";
  body += language == 0 ? "Protokół" : "Protocol";
  body += "</label><select name=\"t\"><option value=\"0\"";
  body += configuration.protocol == 0 ? " selected=\"selected\"" : "";
  body += ">http://</option><option value=\"1\"";
  body += configuration.protocol == 1 ? " selected=\"selected\"" : "";
  body += ">https://</option></select></div><div class=\"cf\">";
  body += "<label>Hostname/IP</label><input name=\"h\" type=\"text\" "
          "maxlength=\"40\" value=\"";
  body += configuration.host;
  body += "\">";
  body += "<span class=\"hint\">Max 40 ";
  body += language == 0 ? "znaków" : "chars";
  body += "</span></div><div class=\"cf\"><label>Port</label><input "
          "name=\"p\" type=\"number\""
          " min=\"0\" max=\"65535\" step=\"1\" value=\"";
  body += configuration.port;
  body += "\"></div><div class=\"cf\"><label>";
  body += language == 0 ? "Użytkownik" : "User";
  body += "</label><input name=\"u\" type=\"text\"  maxlength=\"32\" value=\"";
  body += configuration.user;
  body += "\"><span class=\"hint\">Max 32 ";
  body += language == 0 ? "znaków" : "chars";
  body += "</span></div><div class=\"cf\"><label>";
  body += language == 0 ? "Hasło" : "Password";
  body += "</label><input name=\"s\" type=\"password\"  maxlength=\"32\" "
          "value=\"";
  body += configuration.password;
  body += "\"><span class=\"hint\">Max 32 ";
  body += language == 0 ? "znaków" : "chars";
  body += "</span></div></fieldset>";

  return addConfigurationBlock(
      language == 0 ? "Serwer Domoticz" : "Domoticz Server",
      language == 0 ? "Wprowadź adres hosta np. localhost lub adres IP"
                    : "Enter Domoticz hostname or its IP address",
      body);
}

#ifndef T0_SHELLY_1_CONFIG
String AFESitesGenerator::addLEDConfiguration(uint8_t id) {
  LED configuration;
  configuration = Data.getLEDConfiguration(id);

  String body = "<fieldset>";
  char filed[13];
  sprintf(filed, "g%d", id);
  body += "<div class=\"cf\">";
  body += generateConfigParameter_GPIO(filed, configuration.gpio);
  body += "<label style=\"width: 300px;\"><input name=\"o";
  body += id;
  body += "\" type=\"checkbox\" value=\"1\"";
  body += configuration.changeToOppositeValue ? " checked=\"checked\"" : "";
  body += ">";
  body += language == 0 ? "Zmień świecenie diody LED na odwrotne"
                        : "Change LED ligtning to opposite";
  body += "</label></div></fieldset>";

  return addConfigurationBlock("LED #" + String(id + 1), "", body);
}

String AFESitesGenerator::addSystemLEDConfiguration() {
  uint8_t configuration = Data.getSystemLedID();
  AFEDevice Device;

  String body =
      "<fieldset><div class=\"cf\"><label>LED</label><select name=\"i\">";

  body += "<option value=\"0\"";
  body += configuration == 0 ? " selected=\"selected\"" : "";
  body += language == 0 ? ">Brak" : ">None";
  body += "</option>";

  for (uint8_t i = 1; i <= sizeof(Device.configuration.isLED); i++) {
    if (Device.configuration.isLED[i - 1]) {
      body += "<option value=\"";
      body += i;
      body += "\"";
      body += configuration == i ? " selected=\"selected\"" : "";
      body += ">";
      body += i;
      body += "</option>";
    } else {
      break;
    }
  }

  body += "</select></div></fieldset>";

  return addConfigurationBlock(
      language == 0 ? "LED systemowy" : "System LED",
      language == 0 ? "Wybierz diodę LED, która będzie sygnalizowała stan "
                      "urządzenia oraz występujące zdarzenia"
                    : "Select LED which will be informing about device status "
                      "and its events ",
      body);
}
#endif

String AFESitesGenerator::addRelayConfiguration(uint8_t id) {
  RELAY configuration = Data.getRelayConfiguration(id);
  DEVICE device = Data.getDeviceConfiguration();
  String body = "<fieldset>";
  char filed[13];
  sprintf(filed, "g%d", id);

  body += generateConfigParameter_GPIO(filed, configuration.gpio);

#if !defined(T5_CONFIG) // Not required for T5
  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Nazwa" : "Name";
  body += "</label>";
  body += "<input name=\"n" + String(id) +
          "\" type=\"text\" maxlength=\"16\" value=\"";
  body += configuration.name;
  body += "\">";
  body += "<span class=\"hint\">Max 16 ";
  body += language == 0 ? "znaków" : "chars";
  body += "</span>";
  body += "</div>";

  body += "<p class=\"cm\">";
  body += language == 0 ? "Wartości domyślne" : "Default values";
  body += "</p>";
  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Po przywróceniu zasilania"
                        : "When power is restored set it to";
  body += "</label>";
  body += "<select name=\"pr" + String(id) + "\">";
  body += "<option value=\"0\"";
  body += (configuration.statePowerOn == 0 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "Brak akcji" : "No action";
  body += "</option>";
  body += "<option value=\"1\"";
  body += (configuration.statePowerOn == 1 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "Wyłączony" : "Off";
  body += "</option>";
  body += "<option value=\"2\"";
  body += (configuration.statePowerOn == 2 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "Włączony" : "On";
  body += "</option>";
  body += "<option value=\"3\"";
  body += (configuration.statePowerOn == 3 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "Ostatnia zapamiętana wartość" : "Last known state";
  body += "</option>";
  body += "<option value=\"4\"";
  body += (configuration.statePowerOn == 4 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "Przeciwna do ostatniej zapamiętanej"
                        : "Opposite to the last known state";
  body += "</option>";
  body += "</select>";
  body += "</div>";

  if (device.mqttAPI) {
    body += "<div class=\"cf\">";
    body += "<label>";
    body += language == 0 ? "Po podłączeniu do brokera MQTT"
                          : "After establishing connection to MQTT Broker";
    body += "</label>";
    body += "<select  name=\"mc" + String(id) + "\">";
    body += "<option value=\"0\"";
    body +=
        (configuration.stateMQTTConnected == 0 ? " selected=\"selected\"" : "");
    body += ">";
    body += language == 0 ? "Brak akcji" : "No action";
    body += "</option>";
    body += "<option value=\"1\"";
    body +=
        (configuration.stateMQTTConnected == 1 ? " selected=\"selected\"" : "");
    body += ">";
    body += language == 0 ? "Wyłączony" : "Off";
    body += "</option>";
    body += "<option value=\"2\"";
    body +=
        (configuration.stateMQTTConnected == 2 ? " selected=\"selected\"" : "");
    body += ">";
    body += language == 0 ? "Włączony" : "On";
    body += "</option>";
    body += "<option value=\"3\"";
    body +=
        (configuration.stateMQTTConnected == 3 ? " selected=\"selected\"" : "");
    body += ">";
    body += language == 0 ? "Ostatnia zapamiętana wartość" : "Last known state";
    body += "</option>";
    body += "<option value=\"4\"";
    body +=
        (configuration.stateMQTTConnected == 4 ? " selected=\"selected\"" : "");
    body += ">";
    body += language == 0 ? "Przeciwna do ostatniej zapamiętanej"
                          : "Opposite to the last known state";
    body += "</option>";
    body += "<option value=\"5\"";
    body +=
        (configuration.stateMQTTConnected == 5 ? " selected=\"selected\"" : "");
    body += ">";
    body += language == 0
                ? "Wartość z systemu sterowania przekaźnikiem (przez MQTT)"
                : "Get state from a relay's controlling system (over MQTT)";
    body += "</option>";
    body += "</select>";
    body += "</div>";
  }
#endif

/* Relay Time off / Impuls is not applicable for T3 & T6 */
#if !(defined(T3_CONFIG) || defined(T6_CONFIG))
  body += "<br><p class=\"cm\">";
  body += language == 0 ? "Automatyczne wyłączenie przekaźnika"
                        : "Automatic switching off of the relay";
  body += "</p>";
  body += "<div class=\"cf\">";
  body += "<label>";
#if defined(T5_CONFIG)
  body += language == 0 ? "Długośc impulsu" : "Impulse duration";
#else
  body += language == 0 ? "Wyłącz po" : "Switch off after";
#endif
  body += "</label>";

#if defined(T5_CONFIG)
  body += "<input name=\"ot" + String(id) +
          "\" type=\"number\" step=\"1\" max=\"9999\" min=\"1\" value=\"";
#else
  body += "<input name=\"ot" + String(id) +
          "\" type=\"number\" step=\"0.01\" min=\"0\" max=\"86400\"  value=\"";
#endif
  body += configuration.timeToOff;
  body += "\">";
#if defined(T5_CONFIG)
  body += "<span class=\"hint\">1 - 9999 milise";
  body += language == 0 ? "kund" : "cunds";
#else
  body += "<span class=\"hint\">0.01 - 86400";
  body += language == 0 ? "sek (24h). Brak akcji jeśli jest 0"
                        : "sec (24h). No action if it's set to 0";
#endif
  body += "</span>";
  body += "</div>";

#endif /* T3/T6 exclusion end */

#ifdef T1_CONFIG
  if (device.isDS18B20)
#endif

#ifdef T2_CONFIG
    if (device.isDHT)
#endif

#if defined(T1_CONFIG) || defined(T2_CONFIG)
    {

      body += "<br><p class=\"cm\">";
      body += language == 0 ? "Zabezpieczenie termiczne" : "Thermal protection";
      body += "</p>";

      body += "<div class=\"cf\">";
      body += "<label>";
      body += language == 0 ? "Wyłącz powyżej" : "Switch off above";
      body += "</label>";
      body += "<input name=\"tp" + String(id) +
              "\" type=\"number\" step=\"1\" min=\"-67\" max=\"259\"  value=\"";
      body += configuration.thermalProtection;
      body += "\">";
      body += "<span class=\"hint\">";
      body += language == 0 ? "Zakres" : "Range";
      body += ": -55C : +125C (-67F : +259F). ";
      body += language == 0 ? "Brak akcji jeśli jest 0"
                            : "No action if it's set to 0";
      body += "</span></div>";
    }
#endif

#if !(defined(T0_SHELLY_1_CONFIG) || defined(T5_CONFIG))
  body += "<br><p class=\"cm\">";
  body += language == 0 ? "Wybierz LED sygnalizujący stan przekaźnika"
                        : "Select LED informing about relay state";
  body += "</p>";

  body += "<div class=\"cf\">";
  body += "<label>LED</label>";
  body += "<select  name=\"l" + String(id) + "\">";
  body += "<option value=\"0\"";
  body += configuration.ledID == 0 ? " selected=\"selected\"" : "";
  body += language == 0 ? ">Brak" : ">None";
  body += "</option>";

  for (uint8_t i = 1; i <= sizeof(Device.configuration.isLED); i++) {
    if (Device.configuration.isLED[i - 1]) {
      body += "<option value=\"";
      body += i;
      body += "\"";
      body += configuration.ledID == i ? " selected=\"selected\"" : "";
      body += ">";
      body += i;
      body += "</option>";
    } else {
      break;
    }
  }

  body += "</select>";
  body += "</div>";
#endif

  body += "</fieldset>";

  char title[23];
  language == 0 ? sprintf(title, "Przekaźnik #%d", id + 1)
                : sprintf(title, "Relay #%d", id + 1);

  String page = addConfigurationBlock(title, "", body);

#if !defined(T5_CONFIG)
  if (device.domoticzAPI) {
    body = "<fieldset>";
    body += "<div class=\"cf\">";
    body += "<label>IDX</label>";
    body += "<input name=\"x" + String(id) +
            "\" type=\"number\" step=\"1\" min=\"0\" max=\"999999\"  value=\"";
    body += configuration.idx;
    body += "\">";
    body += "<span class=\"hint\">";
    body += language == 0 ? "Zakres: " : "Range: ";
    body += "0 - 999999</span>";
    body += "</div>";
    body += "</fieldset>";

    page += addConfigurationBlock(
        "Domoticz",
        language == 0
            ? "Jeśli IDX jest 0 to wartośc nie będzie wysyłana do Domoticz"
            : "If IDX is set to 0 then a value won't be sent to Domoticz",
        body);
  }
#endif

  return page;
}

#if defined(T1_CONFIG) || defined(T2_CONFIG)

String AFESitesGenerator::addRegulatorConfiguration(uint8_t type) {
  RELAY configuration = Data.getRelayConfiguration(0);

#if defined(T2_CONFIG)
  if (type == THERMOSTAT_REGULATOR)
#endif
  {
    String body = generateTwoValueController(configuration.thermostat,
                                             THERMOSTAT_REGULATOR);

    return addConfigurationBlock(
        language == 0 ? "Termostat" : "Thermostat",
        language == 0
            ? "Termostat kontroluje przekaźnik w "
              "zależności od wartości temperatury"
            : "Thermostat controlls the relay depending on temperature value",
        body);

  }
#if defined(T2_CONFIG)
  else {
    String body = generateTwoValueController(configuration.humidistat,
                                             HUMIDISTAT_REGULATOR);

    return addConfigurationBlock(
        language == 0 ? "Regulator wilgotności" : "Humidistat",
        language == 0
            ? "Regulator wilgotności kontroluje przekaźnik w "
              "zależności od wartości wilgotności"
            : "Humidistat controlls the relay depending on humidity value",
        body);
  }
#endif
}
#endif

String AFESitesGenerator::addSwitchConfiguration(uint8_t id) {

  SWITCH configuration;
  configuration = Data.getSwitchConfiguration(id);

  String body = "<fieldset>";
  char filed[13];
  sprintf(filed, "g%d", id);
  body += "<div class=\"cf\">";
  body += generateConfigParameter_GPIO(filed, configuration.gpio);
  body += "</div>";
  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Funkcja" : "Functionality";
  body += "</label>";
  body += "<select name=\"f" + String(id) + "\">";
  body += "<option value=\"0\"";
  body += (configuration.functionality == 0 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "Przycisk systemowy" : "System button";
  body += "</option>";
  body += "<option value=\"1\"";
  body += (configuration.functionality == 1 ? " selected=\"selected\"" : "");
  body += ">";
#if defined(T5_CONFIG)
  body +=
      language == 0 ? "Tylko sterowanie bramą" : "Controlling only the gate";
#else
  body += language == 0 ? "Tylko sterowanie przekaźnikiem"
                        : "Controlling only the relay";
#endif
  body += "</option>";

  body += "</select>";
  body += "</div>";

  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Przekaźnik sterowany tym przyciskiem"
                        : "Select relay controlled by this switch";
  body += "</label>";

  body += "<select  name=\"r" + String(id) + "\">";

  body += "<option value=\"0\"";
  body += configuration.relayID == 0 ? " selected=\"selected\"" : "";
  body += language == 0 ? ">Brak" : ">None";
  body += "</option>";

  for (uint8_t i = 1; i <= sizeof(Device.configuration.isRelay); i++) {
    if (Device.configuration.isRelay[i - 1]) {
      body += "<option value=\"";
      body += i;
      body += "\"";
      body += configuration.relayID == i ? " selected=\"selected\"" : "";
      body += ">";
      body += i;
      body += "</option>";
    } else {
      break;
    }
  }

  body += "</select>";
  body += "</div>";
  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Typ" : "Type";
  body += "</label>";
  body += "<select name=\"t" + String(id) + "\">";
  body += "<option value=\"0\"";
  body += (configuration.type == 0 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "Monostabilny" : "Monostable";
  body += "</option>";
  body += "<option value=\"1\"";
  body += (configuration.type == 1 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "Bistabilny" : "Bistable";
  body += "</option>";
  body += "</select>";
  body += "</div>";
  body += "<br><p class=\"cm\">";
  body += language == 0
              ? "Czułość należy ustawić metodą prób, aż uzyska się "
                "pożądane działanie przycisku podczas jego wciskania"
              : "Sensitiveness should be adjusted if switch didn't behave "
                "as expected while pressing it";

  body += "</p><div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Czułość" : "Sensitiveness";
  body += "</label>";
  body += "<input name=\"s" + String(id) +
          "\" type=\"number\" max=\"999\" min=\"0\" step=\"1\" "
          "value=\"";
  body += configuration.sensitiveness;
  body += "\">";
  body += "<span class=\"hint\">0 - 999 (milise";
  body += language == 0 ? "kund" : "conds";
  body += ")</span>";
  body += "</div>";
  body += "</fieldset>";

  char title[23];
  language == 0 ? sprintf(title, "Przycisk / Włącznik #%d", id + 1)
                : sprintf(title, "Switch / Button #%d", id + 1);

  return addConfigurationBlock(title, "", body);
}

#ifdef T1_CONFIG
String AFESitesGenerator::addDS18B20Configuration() {

  DS18B20 configuration = Data.getSensorConfiguration();
  DEVICE device = Data.getDeviceConfiguration();

  String body = "<fieldset>";
  body += generateConfigParameter_GPIO("g", configuration.gpio);

  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Odczyty co" : "Read every";
  body += "</label>";
  body +=
      "<input name=\"i\" min=\"5\" max=\"86400\" step=\"1\" type=\"number\" "
      "value=\"";
  body += configuration.interval;
  body += "\">";
  body += "<span class=\"hint\">";
  body += language == 0 ? "sekund. Zakres: 5 do 86400sek"
                        : "seconds. Range: 5 to 86400sec";
  body += " (24h)</span>";
  body += "</div>";

  body += "<div class=\"cc\">";
  body += "<label>";
  body += "<input name=\"o\" type=\"checkbox\" value=\"1\"";
  body += configuration.sendOnlyChanges ? " checked=\"checked\"" : "";
  body += language == 0
              ? ">Wysyłać dane tylko, gdy wartość temperatury zmieni się"
              : ">Send data only if value of temperature has changed";
  body += "</label>";
  body += "</div>";

  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Korekta wartości o" : "Temperature correction";
  body += "</label>";
  body += "<input name=\"c\" type=\"number\" min=\"-9.99\" max=\"9.99\" "
          "step=\"0.01\" "
          "value=\"";
  body += configuration.correction;
  body += "\">";
  body += "<span class=\"hint\">";
  body += language == 0 ? "stopni. Zakres" : "degrees. Range";
  body += ": -9.99 - +9.99</span>";
  body += "</div>";
  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Jednostka" : "Unit";
  body += "</label>";
  body += "<select  name=\"u\">";
  body += "<option value=\"0\"";
  body += (configuration.unit == 0 ? " selected=\"selected\">" : ">");
  body += language == 0 ? "Celsjusz" : "Celsius";
  body += "</option>";
  body += "<option value=\"1\"";
  body += (configuration.unit == 1 ? " selected=\"selected\"" : "");
  body += ">Fahrenheit</option>";
  body += "</select>";
  body += "</div>";
  body += "</fieldset>";

  String page =
      addConfigurationBlock(language == 0 ? "Czujnik temperatury DS18B20"
                                          : "DS18B20 temperature sensor",
                            "", body);

  if (device.domoticzAPI) {
    body = "<fieldset>";
    body += "<div class=\"cf\">";
    body += "<label>IDX</label>";
    body += "<input name=\"x\" type=\"number\" step=\"1\" min=\"0\" "
            "max=\"999999\"  value=\"";
    body += configuration.idx;
    body += "\">";
    body += "<span class=\"hint\">";
    body += language == 0 ? "Zakres: " : "Range: ";
    body += "0 - 999999</span>";
    body += "</div>";
    body += "</fieldset>";
    page += addConfigurationBlock(
        "Domoticz",
        language == 0
            ? "Jeśli IDX jest 0 to wartośc nie będzie wysyłana do Domoticz"
            : "If IDX is set to 0 then a value won't be sent to Domoticz",
        body);
  }
  return page;
}
#endif

#if defined(T2_CONFIG) || defined(T5_CONFIG)
String AFESitesGenerator::addDHTConfiguration() {

  DH configuration = Data.getSensorConfiguration();
  DEVICE device = Data.getDeviceConfiguration();

  String body = "<fieldset>";
  body += generateConfigParameter_GPIO("g", configuration.gpio);

  body += "<div class=\"cf\"><label>Typ";
  body += language == 1 ? "e" : "";
  body += "</label><select name=\"t\"><option value=\"1\"";
  body += (configuration.type == 1 ? " selected=\"selected\"" : "");
  body += ">DH11</option><option value=\"2\"";
  body += (configuration.type == 2 ? " selected=\"selected\"" : "");
  body += ">DH21</option><option value=\"3\"";
  body += (configuration.type == 3 ? " selected=\"selected\"" : "");
  body += ">DH22</option></select></div>";

  body += "<div class=\"cf\"><label>";
  body += language == 0 ? "Odczyty co" : "Read every";
  body += "</label><input name=\"i\" min=\"10\" max=\"86400\" step=\"1\" "
          "type=\"number\" "
          "value=\"";
  body += configuration.interval;
  body += "\"><span class=\"hint\">";
  body += language == 0 ? "sekund. Zakres: 10 do 86400sek"
                        : "seconds. Range: 10 to 86400sec";
  body += " (24h)</span></div><div class=\"cc\"><label><input name=\"o\" "
          "type=\"checkbox\" value=\"1\"";
  body += configuration.sendOnlyChanges ? " checked=\"checked\"" : "";
  body +=
      language == 0
          ? ">Wysyłać dane tylko, gdy wartość temperatury lub wilgotności "
            "zmieni się"
          : ">Send data only if value of temperature or humidity has changed";
  body += "</label></div>";

  if (device.mqttAPI) {
    body += "<div class=\"cc\"><label><input name=\"p\" type=\"checkbox\" "
            "value=\"1\"";
    body += configuration.publishHeatIndex ? " checked=\"checked\"" : "";
    body += language == 0 ? ">Wysyłać temperaturę odczuwalną"
                          : ">Publish Heat Index";
    body += "?</label></div><div class=\"cc\"><label><input name=\"j\" "
            "type=\"checkbox\" value=\"1\"";
    body += configuration.publishDewPoint ? " checked=\"checked\"" : "";
    body += language == 0 ? ">Wysyłać punkt rosy" : ">Publish Dew Point";
    body += "?</label></div>";
  }

  body += "<br><p class=\"cm\">Temperatur";
  body += language == 0 ? "y" : "e";
  body += "</p>";

  body += "<div class=\"cf\"><label>";
  body += language == 0 ? "Korekta wartości o" : "Value to correct";
  body +=
      "</label><input name=\"c\" type=\"number\" min=\"-9.99\" max=\"9.99\" "
      "step=\"0.01\" value=\"";
  body += configuration.temperature.correction;
  body += "\"><span class=\"hint\">";
  body += language == 0 ? "stopni. Zakres" : "degrees. Range";
  body += ": -9.99 - +9.99</span></div><div class=\"cf\"><label>";
  body += language == 0 ? "Jednostka" : "Unit";
  body += "</label><select  name=\"u\"><option value=\"0\"";
  body +=
      (configuration.temperature.unit == 0 ? " selected=\"selected\">" : ">");
  body += language == 0 ? "Celsjusz" : "Celsius";
  body += "</option><option value=\"1\"";
  body += (configuration.temperature.unit == 1 ? " selected=\"selected\"" : "");
  body += ">Fahrenheit</option></select></div><br><p class=\"cm\">";
  body += language == 0 ? "Wilgotnośc" : "Humidity";
  body += "</p><div class=\"cf\"><label>";
  body += language == 0 ? "Korekta wartości o" : "Value to correct";
  body +=
      "</label><input name=\"d\" type=\"number\" min=\"-99.9\" max=\"99.9\" "
      "step=\"0.1\" value=\"";
  body += configuration.humidity.correction;
  body += "\"><span class=\"hint\">";
  body += language == 0 ? "Zakres" : "Range";
  body += ": -99.9 - +99.9</span></div>";

  body += "</fieldset>";

  String page = addConfigurationBlock(
      language == 0 ? "Czujnik temperatury i wilgotności DHT"
                    : "DHT temperature and humidity sensor",
      "", body);

  if (device.domoticzAPI) {
    body = "<fieldset>";
    body += "<div class=\"cf\"><label> ";
    body +=
        language == 0 ? "IDX czujnika temperatury" : "Temperature sensor IDX";
    body += " </label>";
    body += "<input name=\"xt\" type=\"number\" step=\"1\" min=\"0\" "
            "max=\"999999\"  value=\"";
    body += configuration.temperatureIdx;
    body += "\">";
    body += "<span class=\"hint\">";
    body += language == 0 ? "Zakres: " : "Range: ";
    body += "0 - 999999</span>";
    body += "</div>";

    body += "<div class=\"cf\"><label>";
    body += language == 0 ? "IDX czujnika wilgotności" : "Humidity sensor IDX";
    body += "</label><input name=\"xh\" type=\"number\" step=\"1\" min=\"0\" "
            "max=\"999999\"  value=\"";
    body += configuration.humidityIdx;
    body += "\">";
    body += "<span class=\"hint\">";
    body += language == 0 ? "Zakres: " : "Range: ";
    body += "0 - 999999</span>";
    body += "</div>";

    body += "<div class=\"cf\"><label>";
    body += language == 0 ? "IDX czujnika temperatury i wilgotności"
                          : "Temperature and humidity sensor IDX";
    body += "</label><input name=\"xth\" type=\"number\" step=\"1\" min=\"0\" "
            "max=\"999999\"  value=\"";
    body += configuration.temperatureAndHumidityIdx;
    body += "\">";
    body += "<span class=\"hint\">";
    body += language == 0 ? "Zakres: " : "Range: ";
    body += "0 - 999999</span>";
    body += "</div>";
    body += "</fieldset>";
    page += addConfigurationBlock(
        "Domoticz",
        language == 0
            ? "Jeśli IDX jest 0 to wartośc nie będzie wysyłana do Domoticz"
            : "If IDX is set to 0 then a value won't be sent to Domoticz",
        body);
  }

  return page;
}
#endif

#if defined(T3_CONFIG)
String AFESitesGenerator::addPIRConfiguration(uint8_t id) {
  PIR configuration = Data.getPIRConfiguration(id);
  DEVICE device = Data.getDeviceConfiguration();

  String body = "<fieldset>";
  char filed[13];
  sprintf(filed, "g%d", id);
  body += "<div class=\"cf\">";
  body += generateConfigParameter_GPIO(filed, configuration.gpio);
  body += "</div>";

  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Nazwa" : "Name";
  body += "</label>";
  body += "<input name=\"n" + String(id) +
          "\" type=\"text\" maxlength=\"16\" value=\"";
  body += configuration.name;
  body += "\">";
  body += "<span class=\"hint\">Max 16 ";
  body += language == 0 ? "znaków" : "chars";
  body += "</span>";
  body += "</div>";

  body += "<div class=\"cf\"><label>";
  body += language == 0 ? "Typ" : "Type";
  body += "</label><select name=\"o" + String(id) + "\"><option value=\"0\"";
  body +=
      (configuration.outputDefaultState == 0 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "NO" : "NO";
  body += "</option><option value=\"1\"";
  body +=
      (configuration.outputDefaultState == 1 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "NC" : "NC";
  body += "</option></select></div>";

  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "LED" : "LED";
  body += "</label>";
  body += "<select name=\"l" + String(id) + "\">";
  body += "<option value=\"9\" ";
  body += (configuration.ledId == 9 ? "selected=\"selected\"" : "");
  body += ">Brak</option>";
  for (uint8_t i = 0; i < 5; i++) {
    if (Device.configuration.isLED[i]) {
      body += "<option value=\"";
      body += i;
      body += "\" ";
      body += (configuration.ledId == i ? "selected=\"selected\"" : "");
      body += ">";
      body += i + 1;
      body += "</option>";
    }
  }
  body += "</select>";
  body += "<span class=\"hint\">";
  body += language == 0 ? "Dioda LED sygnalizuje wykryty ruch przez czujnik"
                        : "LED indicates motion detected by the sensor";
  body += "</span>";
  body += "</div>";
  body += "<br><p class=\"cm\">";
  body += language == 0
              ? "Czujnik PIR może bezpośrednio sterować jednym przekaźnikiem. "
                "Poniżej możesz wybrać którym oraz ustawić dodatkowe "
                "parametry sterowania"
              : "Motion detection sensor can control a relay assigned to it. "
                "Below you can choose a one and set additional parameters";
  body += "</p>";

  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Wybierz przekaźnik" : "Select relay";
  body += "</label>";
  body += "<select name=\"r" + String(id) + "\">";
  body += "<option value=\"9\" ";
  body += (configuration.relayId == 9 ? "selected=\"selected\"" : "");
  body += ">Brak</option>";
  for (uint8_t i = 0; i < 4; i++) {
    if (Device.configuration.isRelay[i]) {
      body += "<option value=\"";
      body += i;
      body += "\" ";
      body += (configuration.relayId == i ? "selected=\"selected\"" : "");
      body += ">";
      body += i + 1;
      body += "</option>";
    }
  }
  body += "</select>";
  body += "</div>";

  body += "<div class=\"cf\"><label>";
  body += language == 0 ? "Czas uruchomienia" : "How long active";
  body += "</label>";
  body += "<input name=\"d" + String(id) +
          "\" type=\"number\" max=\"86400\" min=\"0.01\" step=\"0.01\" "
          "value=\"";
  body += configuration.howLongKeepRelayOn;
  body += "\">";
  body += "<span class=\"hint\">0.01 - 86400 (24h) se";
  body += language == 0 ? "kund" : "conds";
  body += "</span>";
  body += "</div>";

  body += "<div class=\"cc\">";
  body += "<label>";
  body += "<input name=\"i";
  body += id;
  body += "\" type=\"checkbox\" value=\"1\"";
  body += configuration.invertRelayState ? " checked=\"checked\"" : "";
  body += ">";
  body += language == 0 ? "Wyłącz przekaźnik, gdy PIR wykryje ruch"
                        : "Switch off relay if PIR detects move";

  body += "</label></div></fieldset>";
  char title[25];
  language == 0 ? sprintf(title, "Czujnik ruchu (PIR) #%d", id + 1)
                : sprintf(title, "Motion detection sesnor (PIR) #%d", id + 1);

  String page = addConfigurationBlock(title, "", body);

  if (device.domoticzAPI) {
    body = "<fieldset><div class=\"cf\"><label>IDX</label><input name=\"x";
    body += id;
    body += "\" type=\"number\" step=\"1\" min=\"0\" max=\"999999\"  value=\"";
    body += configuration.idx;
    body += "\"><span class=\"hint\">";
    body += language == 0 ? "Zakres: " : "Range: ";
    body += "0 - 999999</span></div></fieldset>";
    page += addConfigurationBlock(
        "Domoticz",
        language == 0
            ? "Jeśli IDX jest 0 to wartośc nie będzie wysyłana do Domoticz"
            : "If IDX is set to 0 then a value won't be sent to Domoticz",
        body);
  }

  return page;
}
#endif

#if defined(T5_CONFIG)
String AFESitesGenerator::addContactronConfiguration(uint8_t id) {
  CONTACTRON configuration = Data.getContactronConfiguration(id);
  DEVICE deviceConfiguration = Device.configuration;

  String body = "<fieldset>";
  char filed[13];
  sprintf(filed, "g%d", id);
  body += "<div class=\"cf\">";
  body += generateConfigParameter_GPIO(filed, configuration.gpio);
  body += "</div><div class=\"cf\"><label>";
  body += language == 0 ? "Nazwa" : "Name";
  body += "</label><input name=\"n" + String(id) +
          "\" type=\"text\" maxlength=\"16\" value=\"";
  body += configuration.name;
  body += "\"><span class=\"hint\">Max 16 ";
  body += language == 0 ? "znaków" : "chars";
  body += "</span></div><div class=\"cf\"><label>";
  body += language == 0 ? "Typ" : "Type";
  body += "</label><select name=\"o" + String(id) + "\"><option value=\"0\"";
  body +=
      (configuration.outputDefaultState == 0 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "NO" : "NO";
  body += "</option><option value=\"1\"";
  body +=
      (configuration.outputDefaultState == 1 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "NC" : "NC";
  body += "</option></select></div><div class=\"cf\"><label>LED ";
  body += language == 0 ? "przypisany do czujnika" : "assigned to the sensor";
  body += "</label><select  name=\"l" + String(id) + "\"><option value=\"0\"";
  body += configuration.ledID == 0 ? " selected=\"selected\"" : "";
  body += language == 0 ? ">Brak" : ">None";
  body += "</option>";

  for (uint8_t i = 1; i <= sizeof(Device.configuration.isLED); i++) {
    if (Device.configuration.isLED[i - 1]) {
      body += "<option value=\"";
      body += i;
      body += "\"";
      body += configuration.ledID == i ? " selected=\"selected\"" : "";
      body += ">";
      body += i;
      body += "</option>";
    } else {
      break;
    }
  }

  body += "</select></div><br><p class=\"cm\">";
  body += language == 0
              ? "Czułość należy ustawić eksperymentalnie, aż uzyska się "
                "pożądane działanie czujnika magnetycznego"
              : "Sensitiveness should be adjusted experimentally until "
                "sensor behaves as expected";

  body += "</p><div class=\"cf\"><label>";
  body += language == 0 ? "Czułość" : "Sensitiveness";
  body += "</label>";
  body += "<input name=\"b" + String(id) +
          "\" type=\"number\" max=\"2000\" min=\"0\" step=\"1\" "
          "value=\"";
  body += configuration.bouncing;
  body += "\"><span class=\"hint\">0 - 2000 (milise";
  body += language == 0 ? "kund" : "conds";
  body += ")</span></div>";

  char title[23];
  language == 0 ? sprintf(title, "Czujnik magnetyczny #%d", id + 1)
                : sprintf(title, "Magnetic sensor #%d", id + 1);

  String page = addConfigurationBlock(title, "", body);

  if (deviceConfiguration.domoticzAPI) {
    body = "<div class=\"cf\"><label>IDX</label>";
    body += "<input name=\"x" + String(id) +
            "\" type=\"number\" step=\"1\" min=\"0\" max=\"999999\"  value=\"";
    body += configuration.idx;
    body += "\">";
    body += "<span class=\"hint\">";
    body += language == 0 ? "Zakres: " : "Range: ";
    body += "0 - 999999</span></div>";
    page += addConfigurationBlock(
        "Domoticz",
        language == 0
            ? "Jeśli IDX jest 0 to wartośc nie będzie wysyłana do Domoticz"
            : "If IDX is set to 0 then a value won't be sent to Domoticz",
        body);
  }

  body += "</fieldset>";

  return page;
}

String AFESitesGenerator::addGateConfiguration() {
  GATE gateConfiguration = Data.getGateConfiguration();
  DEVICE deviceConfiguration = Device.configuration;
  uint8_t noOfContactrons = deviceConfiguration.isContactron[1] ? 2 : 1;
  CONTACTRON configuration[noOfContactrons];

  for (uint8_t i = 0; i < noOfContactrons; i++) {
    configuration[i] = Data.getContactronConfiguration(i);
  }

  String body = "<fieldset>";

  body += "<p class=\"cm\">";
  if (language == 0) {
    body += "Jeśli czujnik magnetyczny: <strong>";
    body += configuration[0].name;

    if (noOfContactrons == 2) {
      body += "</strong> oraz czujnik: <strong>";
      body += configuration[1].name;
      body += "</strong> są otwarte";
    } else {
      body += "</strong> jest otwarty";
    }
    body += " to:";
  } else {
    body += "If magnetic sensor: <strong>";
    body += configuration[0].name;

    if (noOfContactrons == 2) {
      body += "</strong> and <strong>";
      body += configuration[1].name;
      body += "</strong> are open";
    } else {
      body += "</strong> is open";
    }
    body += " then:";
  }
  body += "</p>";
  body += generateGateStatesList(0, gateConfiguration.state[0]);
  if (noOfContactrons == 2) {
    body += "<br><br><p class=\"cm\">";
    if (language == 0) {
      body += "Jeśli czujnik magnetyczny: <strong>";
      body += configuration[0].name;
      body += "</strong> jest otwarty, a czujnik: <strong>";
      body += configuration[1].name;
      body += "</strong> jest zamknięty to:";
    } else {
      body += "If magnetic sensor: <strong>";
      body += configuration[0].name;
      body += "</strong> is open and sensor: <strong>";
      body += configuration[1].name;
      body += "</strong> is closed then:";
    }
    body += "</p>";
    body += generateGateStatesList(1, gateConfiguration.state[1]);

    body += "<br><br><p class=\"cm\">";
    if (language == 0) {
      body += "Jeśli czujnik magnetyczny: <strong>";
      body += configuration[0].name;
      body += "</strong> jest zamknięty, a czujnik: <strong>";
      body += configuration[1].name;
      body += "</strong> jest otwarty to:";
    } else {
      body += "If magnetic sensor: <strong>";
      body += configuration[0].name;
      body += "</strong> is closed and sensor: <strong>";
      body += configuration[1].name;
      body += "</strong> is open then:";
    }
    body += "</p>";
    body += generateGateStatesList(2, gateConfiguration.state[2]);
  }

  body += "<br><br><p class=\"cm\">";
  if (language == 0) {
    body += "Jeśli czujnik magnetyczny: <strong>";
    body += configuration[0].name;

    if (noOfContactrons == 2) {
      body += "</strong> oraz czujnik: <strong>";
      body += configuration[1].name;
      body += "</strong> są zamknięte";
    } else {
      body += "</strong> jest zamknięty ";
    }
    body += " to:";
  } else {
    body += "If magnetic sensor: <strong>";
    body += configuration[0].name;

    if (noOfContactrons == 2) {
      body += "</strong> and sensor: <strong>";
      body += configuration[1].name;
      body += "</strong> are closed";
    } else {
      body += "</strong> is closed ";
    }
    body += " then:";
  }
  body += "</p>";
  body += generateGateStatesList(3, gateConfiguration.state[3]);

  String page = addConfigurationBlock(
      language == 0 ? "Konfiguracja stanów bramy" : "Gate states configuration",
      "", body);

  if (deviceConfiguration.domoticzAPI) {
    body = "<div class=\"cf\">";
    body += "<label>IDX</label>";
    body += "<input name=\"x\" type=\"number\" step=\"1\" min=\"0\" "
            "max=\"999999\"  value=\"";
    body += gateConfiguration.idx;
    body += "\">";
    body += "<span class=\"hint\">";
    body += language == 0 ? "Zakres: " : "Range: ";
    body += "0 - 999999</span>";
    body += "</div>";

    page += addConfigurationBlock(
        "Domoticz",
        language == 0
            ? "Jeśli IDX jest 0 to wartośc nie będzie wysyłana do Domoticz"
            : "If IDX is set to 0 then a value won't be sent to Domoticz",
        body);
  }

  page += "</fieldset>";

  return page;
}

const String AFESitesGenerator::generateGateStatesList(uint8_t id, byte state) {
  String body = "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Ustaw stan bramy na" : "Set gate's state to";
  body += "</label>";
  body += "<select name=\"s" + String(id) + "\">";
  body += "<option value=\"";
  body += GATE_OPEN;
  body += "\"";
  body += (state == GATE_OPEN ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "Otwarta" : "Open";
  body += "</option>";
  body += "<option value=\"";
  body += GATE_PARTIALLY_OPEN;
  body += "\"";
  body += (state == GATE_PARTIALLY_OPEN ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "Częściowo otwarta" : "Partially open";
  body += "</option>";
  body += "<option value=\"";
  body += GATE_CLOSED;
  body += "\"";
  body += (state == GATE_CLOSED ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "Zamknięta" : "Closed";
  body += "</option>";
  body += "</select>";
  body += "</div>";

  return body;
}
#endif

#if defined(T6_CONFIG)
String AFESitesGenerator::addHPMA115S0Configuration() {

  HPMA115S0 configuration = Data.getHPMA115S0SensorConfiguration();
  DEVICE device = Data.getDeviceConfiguration();

  String body = "<fieldset>";

  body += "<div class=\"cf\"><label>";
  body += language == 0 ? "Interwał odczytów" : "Measurement's interval";
  body += "</label><input name=\"i\" min=\"5\" max=\"86400\" step=\"1\" "
          "type=\"number\" "
          "value=\"";
  body += configuration.interval;
  body += "\"><span class=\"hint\">";
  body += language == 0 ? "sekund. Zakres: 5 do 86400sek"
                        : "seconds. Range: 5 to 86400sec";
  body += " (24h)</span></div><div class=\"cc\"><label><input name=\"o\" "
          "type=\"checkbox\" value=\"1\"";
  body += configuration.sendOnlyChanges ? " checked=\"checked\"" : "";
  body += language == 0
              ? ">Wysyłać dane tylko, gdy wartość PM2.5 lub PM10 "
                "zmieni się"
              : ">Send data only if value of PM2.5 or PM10 has changed";
  body += "</label></div><br><br>";
  body += "<p class=\"cm\">";
  body +=
      language == 0
          ? "Jeśli poniższa wartość jest większa od 0 to czujnik będzie "
            "usypiany między odczytami. Wartość poniżej definiuje na ile "
            "sekund przed odczytem czujnik ma zostać uruchomiony. Wartość musi "
            "być mniejsza niż interwał pomiarów"
          : "If the parameter below is different than 0, the sensor will go to "
            "sleep mode between measurements. The setting below defined how "
            "many seconds before a measurement the sensor should wake up. It "
            "should be lower than measurement's interval";
  body += "</p>";
  body += "<div class=\"cf\"><label>";
  body += language == 0 ? "Pomiar po czasie" : "Measure after";
  body += "</label><input name=\"t\" min=\"0\" max=\"999\" step=\"1\" "
          "type=\"number\" "
          "value=\"";
  body += configuration.timeToMeasure;
  body += "\"><span class=\"hint\">";
  body += language == 0 ? "sekund. Zakres: 1 do 999sek"
                        : "seconds. Range: 1 to 999sec";
  body += "</span></div>";

  body += "</fieldset>";

  String page =
      addConfigurationBlock(language == 0 ? "Czujnik cząstek PM2.5/PM10"
                                          : "PM2.5/PM10 Particle Sensor",
                            "", body);

  if (device.domoticzAPI) {
    body = "<fieldset>";
    body += "<div class=\"cf\"><label> ";
    body += "IDX PM2.5 </label>";
    body += "<input name=\"x2\" type=\"number\" step=\"1\" min=\"0\" "
            "max=\"999999\"  value=\"";
    body += configuration.idx.pm25;
    body += "\">";
    body += "<span class=\"hint\">";
    body += language == 0 ? "Zakres: " : "Range: ";
    body += "0 - 999999</span>";
    body += "</div>";

    body += "<div class=\"cf\"><label>IDX PM10";
    body += "</label><input name=\"x1\" type=\"number\" step=\"1\" min=\"0\" "
            "max=\"999999\"  value=\"";
    body += configuration.idx.pm10;
    body += "\">";
    body += "<span class=\"hint\">";
    body += language == 0 ? "Zakres: " : "Range: ";
    body += "0 - 999999</span>";
    body += "</div>";

    body += "</fieldset>";
    page += addConfigurationBlock(
        "Domoticz",
        language == 0
            ? "Jeśli IDX jest 0 to wartośc nie będzie wysyłana do Domoticz"
            : "If IDX is set to 0 then a value won't be sent to Domoticz",
        body);
  }

  return page;
}

String AFESitesGenerator::addBME680Configuration() {

  BME680 configuration = Data.getBME680SensorConfiguration();
  DEVICE device = Data.getDeviceConfiguration();

  String body = "<fieldset>";

  body += "<div class=\"cf\"><label>";
  body += language == 0 ? "Interwał odczytów" : "Measurement's interval";
  body += "</label><input name=\"i\" min=\"5\" max=\"86400\" step=\"1\" "
          "type=\"number\" "
          "value=\"";
  body += configuration.interval;
  body += "\"><span class=\"hint\">";
  body += language == 0 ? "sekund. Zakres: 5 do 86400sek"
                        : "seconds. Range: 5 to 86400sec";
  body += " (24h)</span></div><div class=\"cc\"><label><input name=\"o\" "
          "type=\"checkbox\" value=\"1\"";
  body += configuration.sendOnlyChanges ? " checked=\"checked\"" : "";
  body += language == 0 ? ">Wysyłać dane tylko, gdy jedna z wartości "
                          "zmieni się"
                        : ">Send data only if one the values has changed";
  body += "</label></div><br><br>";
  body += "</fieldset>";

  String page = addConfigurationBlock(
      language == 0 ? "Czujnik BME680" : "BME680 Sensor", "", body);

  if (device.domoticzAPI) {
    body = "<fieldset>";
    body += "<div class=\"cf\"><label>IDX Temp/";
    body += language == 0 ? "Wilg/Bar" : "Humi/Bar";
    body += "</label><input name=\"t\" type=\"number\" step=\"1\" min=\"0\" "
            "max=\"999999\"  value=\"";
    body += configuration.idx.temperatureHumidityPressure;
    body += "\">";
    body += "<span class=\"hint\">";
    body += language == 0 ? "Zakres: " : "Range: ";
    body += "0 - 999999</span>";
    body += "</div>";

    body += "<div class=\"cf\"><label>IDX Temperatur";
    body += language == 0 ? "a" : "e";
    body += "</label><input name=\"e\" type=\"number\" step=\"1\" min=\"0\" "
            "max=\"999999\"  value=\"";
    body += configuration.idx.temperature;
    body += "\">";
    body += "<span class=\"hint\">";
    body += language == 0 ? "Zakres: " : "Range: ";
    body += "0 - 999999</span>";
    body += "</div>";

    body += "<div class=\"cf\"><label>IDX ";
    body += language == 0 ? "Wilgotność" : "Humidity";
    body += "</label><input name=\"h\" type=\"number\" step=\"1\" min=\"0\" "
            "max=\"999999\"  value=\"";
    body += configuration.idx.humidity;
    body += "\">";
    body += "<span class=\"hint\">";
    body += language == 0 ? "Zakres: " : "Range: ";
    body += "0 - 999999</span>";
    body += "</div>";

    body += "<div class=\"cf\"><label>IDX ";
    body += language == 0 ? "Ciśnienie" : "Pressure";
    body += "</label><input name=\"p\" type=\"number\" step=\"1\" min=\"0\" "
            "max=\"999999\"  value=\"";
    body += configuration.idx.pressure;
    body += "\">";
    body += "<span class=\"hint\">";
    body += language == 0 ? "Zakres: " : "Range: ";
    body += "0 - 999999</span>";
    body += "</div>";

    body += "<div class=\"cf\"><label>IDX ";
    body += language == 0 ? "Czujnik gazu" : "Gas sensor";
    body += "</label><input name=\"g\" type=\"number\" step=\"1\" min=\"0\" "
            "max=\"999999\"  value=\"";
    body += configuration.idx.gasResistance;
    body += "\">";
    body += "<span class=\"hint\">";
    body += language == 0 ? "Zakres: " : "Range: ";
    body += "0 - 999999</span>";
    body += "</div>";

    body += "</fieldset>";
    page += addConfigurationBlock(
        "Domoticz",
        language == 0
            ? "Jeśli IDX jest 0 to wartość nie będzie wysyłana do Domoticz"
            : "If IDX is set to 0 then a value won't be sent to Domoticz",
        body);
  }

  return page;
}

String AFESitesGenerator::addBH1750Configuration() {

  BH1750 configuration = Data.getBH1750SensorConfiguration();
  DEVICE device = Data.getDeviceConfiguration();

  String body = "<fieldset>";

  body += "<div class=\"cf\"><label>";
  body += language == 0 ? "Interwał odczytów" : "Measurement's interval";
  body += "</label><input name=\"i\" min=\"1\" max=\"86400\" step=\"1\" "
          "type=\"number\" "
          "value=\"";
  body += configuration.interval;
  body += "\"><span class=\"hint\">";
  body += language == 0 ? "sekund. Zakres: 1 do 86400sek"
                        : "seconds. Range: 1 to 86400sec";
  body += " (24h)</span></div><div class=\"cc\"><label><input name=\"o\" "
          "type=\"checkbox\" value=\"1\"";
  body += configuration.sendOnlyChanges ? " checked=\"checked\"" : "";
  body += language == 0 ? ">Wysyłać dane tylko, gdy jedna z wartości "
                          "zmieni się"
                        : ">Send data only if one the values has changed";
  body += "</label></div>";

  body += "<div class=\"cf\"><label>";
  body += language == 0 ? "Czułość" : "Sensitiveness";
  body += "</label><input name=\"m\" type=\"number\" value=\"";
  body += configuration.mode;
  body += "\" disabled><span class=\"hint\">";
  body += language == 0 ? "(brak możliwości zmiany)" : "(can't be set)";
  body += " </span></div>";

  body += "</fieldset>";

  String page = addConfigurationBlock(
      language == 0 ? "Czujnik BH1750" : "BH1750 Sensor", "", body);

  if (device.domoticzAPI) {
    body = "<fieldset>";
    body += "<div class=\"cf\"><label>IDX</label><input name=\"d\" "
            "type=\"number\" step=\"1\" min=\"0\" "
            "max=\"999999\"  value=\"";
    body += configuration.idx;
    body += "\"><span class=\"hint\">";
    body += language == 0 ? "Zakres: " : "Range: ";
    body += "0 - 999999</span>";
    body += "</div>";

    body += "</fieldset>";
    page += addConfigurationBlock(
        "Domoticz",
        language == 0
            ? "Jeśli IDX jest 0 to wartośc nie będzie wysyłana do Domoticz"
            : "If IDX is set to 0 then a value won't be sent to Domoticz",
        body);
  }

  return page;
}

String AFESitesGenerator::addSerialPortConfiguration() {
  SERIALPORT configuration = Data.getSerialPortConfiguration();

  String body = "<fieldset>";

  body += generateConfigParameter_GPIO("r", configuration.RXD, "GPIO RXD");
  body += generateConfigParameter_GPIO("t", configuration.TXD, "GPIO TXD");

  body += "</fieldset>";
  return addConfigurationBlock("UART", "", body);
  ;
}
#endif

String AFESitesGenerator::addUpgradeSection() {
  String body = "<fieldset><div class=\"cf\"><label>";
  body += language == 0 ? "Wybierz" : "Select";
  body += " firmware</label><input class=\"bs\" name=\"update\" type=\"file\" "
          "accept=\".bin\"></div><p class=\"cm\">";
  body +=
      language == 0
          ? "Po zakończeniu aktualizacji urządzenie zostanie "
            "automatycznie zresetowane<br><br><strong>Uwaga:</strong> po "
            "aktualizacji nie odłączaj urządzenia przez conajmniej 1min.<br>"
            "Urządzenie formatuje pamięć i wgrywa domyślne ustawienia. "
            "<br><strong>Ten proces nie może zostać przerwany</strong>."
          : "Device will be automatically rebooted after "
            "upgrade<br><br><strong>Warning</strong>: after upgrade do not "
            "plug off the device from power source for around a "
            "minute.<br>Device's memory "
            "will be formatted and default settings will be "
            "uploaded.<br><strong>This process cannot be "
            "interrupted</strong>";
  body += "</p><button type=\"submit\" class=\"b be\">";
  body += language == 0 ? "Aktualizuj" : "Upgrade";
  body += "</button></fieldset>";
  return addConfigurationBlock(
      language == 0 ? "Aktualizacja firmware" : "Firmware upgrade",
      language == 0 ? "<strong>UWAGA</strong>: nie odłączaj "
                      "urządzenia od źródła zasilania podczas aktualizacji"
                    : "<strong>Warning</strong>: don't plug off the device "
                      "from a power source while upgrading",
      body);
}

String AFESitesGenerator::addPostUpgradeSection(boolean status) {

  String body = "<fieldset><ul>";
  if (status) {
    body += "<li style=\"color:red\">";
    body += language == 0 ? "Aktualizacja nie powiodła się" : "Upgrade failed";
  } else {
    body += "<li class=\"cm\">";
    body += language == 0 ? "Aktualizacja zakończona pomyślnie"
                          : "Upgrade finished succesfully";
  }
  body += "</li><li class=\"cm\">";
  body += language == 0
              ? "Po 15 "
                "sekundach przełącznik zostanie przeładowany. Proszę czekać"
              : "After 15s device will be rebooted. Please wait";
  body += "....</li>";
  body += "</fieldset>";
  return addConfigurationBlock(
      language == 0 ? "Aktualizacja firmware" : "Firmware upgrade", "", body);
}

String AFESitesGenerator::addResetSection(uint8_t command) {
  String body = "<fieldset>";
  String subtitle;
  if (command == 0) {
    body += "<a href=\"\\?option=reset&cmd=1\" class=\"b be\">";
    body += language == 0 ? "Przywróć ustawienia początkowe"
                          : "Restore default settings";
    body += "</a><strong>";
    subtitle += language == 0 ? "Uwaga" : "Warning";
    subtitle += "</strong>: ";
    subtitle +=
        language == 0
            ? "przywrócenie ustawień "
              "początkowych usuwa wszystkie ustawienia "
              "urządzenia, włącznie z konfiguracją sieci WiFi"
            : "restoring to default settings will remove all configuration's "
              "information, incl. WiFi configuration";
  } else {
    subtitle += "";
    body += "<p class=\"cm\">";
    body += language == 0 ? "Trwa przywracanie ustawień początkowych"
                          : "Restoring configuration is in progress";
    body += "</p><p class=\"cm\">";

    body += language == 0 ? "Po 20 sekundach połącz się z siecią WiFi o "
                            "nazwie: <strong>AFE-Device</strong>, a następnie "
                            "połącz się z "
                            "panelem konfiguracyjnym pod adresem"
                          : "After 20 seconds connect to WiFi network called "
                            "<strong>AFE-Device</strong> and open "
                            "configuration panel";
    body += ": </p><a href=\"http://192.168.5.1\">http://192.168.5.1</a>";
  }
  body += "</fieldset>";
  return addConfigurationBlock(language == 0
                                   ? "Przywracanie ustawień początkowych"
                                   : "Restoring to default settings",
                               subtitle, body);
}

String AFESitesGenerator::addExitSection() {
  String body = "<fieldset><div class=\"cf\"><ul><li class=\"cm\">";
  body += language == 0 ? "Trwa restart urządzenia" : "Reboot in progress";
  body += ".....</li><li class=\"cm\">";
  body += language == 0 ? "Strona zostanie przeładowana automatycznie... czekaj"
                        : "Site will be automatically reloaded... wait";
  body += "</li></ul></div></fieldset>";
  return addConfigurationBlock(
      language == 0 ? "Restart urządzenia" : "Device reboot", "", body);
}

String AFESitesGenerator::addHelpSection() {
  DEVICE configuration;
  configuration = Data.getDeviceConfiguration();

  FIRMWARE firmware;
  firmware = Data.getFirmwareConfiguration();

  String body = "<fieldset><div class=\"cf\"><label>";
  body += language == 0 ? "Stan: uruchomione " : "Status: running";
  body += "</label><span></div></fieldset>";

  char title[29];
  language == 0 ? sprintf(title, "Urządzenie: %s", configuration.name)
                : sprintf(title, "Device: %s", configuration.name);

  String page = addConfigurationBlock(title, "", body);

  if (Device.getMode() != MODE_ACCESS_POINT) {

    body = "<p class=\"cm\">";
    body += language == 0
                ? "Oprogramowanie dostępne jest za darmo w ramach licencji "
                : "The software is available for free within terms of ";
    body += " <a "
            "href=\"https://github.com/tschaban/AFE-Firmware/blob/master/"
            "LICENSE\" "
            "target=\"_blank\"  style=\"color:#00e\">MIT</a>";
    body += language == 0 ? ". Jeśli spełnia Twoje oczekiwania to postaw "
                          : " licence. If it meets your expectations, then "
                            "consider buying a beer to the ";
    body += " <a href=\"https://www.smartnydom.pl/o-stronie/\" "
            "target=\"_blank\" style=\"color:#00e\">aut";
    body += language == 0 ? "orowi" : "hor";
    body += "</a> ";
    body += language == 0 ? "browarka" : "";
    body += " ;)</p>";

    body += "<a "
            "href=\"https://www.paypal.com/cgi-bin/"
            "webscr?cmd=_donations&business=VBPLM42PYCTM8&lc=PL&item_name="
            "Wsparcie%20projektu%20AFE%20Firmware&item_number=Firmware%20%5bvT";
    body += firmware.type;
    body += "%5d&currency_code=PLN&bn=PP%2dDonationsBF%3abtn_donateCC_LG%2egif%"
            "3aNonHosted\" target=\"_blank\"><img "
            "src=\"http://adrian.czabanowski.com/afe/donation/T";
    body += firmware.type;
    body += "/";
    body += firmware.version;
    body += "/";
    body += Data.getDeviceID();
    body += "/\" border=\"0\" alt=\"PayPal\"></a>";
    body += "</fieldset>";
    page += addConfigurationBlock(language == 0 ? "Wsparcie" : "Donation", "",
                                  body);
  }

  return page;
}

String AFESitesGenerator::addLanguageConfiguration() {

  String body = "<fieldset><div class=\"cf\"><label>Language / Język: "
                "</label><select name=\"l\"><option value=\"0\"";
  body += language == 0 ? " selected=\"selected\"" : "";
  body += ">Polski</option><option value=\"1\"";
  body += language == 1 ? " selected=\"selected\"" : "";
  body += ">English</option></select></div></fieldset>";
  String page = addConfigurationBlock("Language / Język", "", body);
  return page;
}

const char *AFESitesGenerator::generateFooter() {
  return "<hr style=\"border:0;border-top:1px solid "
         "#eee;\"/></div></div></body></html>";
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

const String AFESitesGenerator::generateConfigParameter_GPIO(
    const char *field, uint8_t selected, const String title) {
  String page = "<div class=\"cf\"><label>";
  page += title;
  page += "</label><select name=\"";
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

const String AFESitesGenerator::generateHardwareItemsList(
    uint8_t noOfItems, uint8_t noOffConnected, const char *field,
    const char *label) {

  String body = "<div class=\"cf\"><label>";
  body += label;
  body += "</label>";
  body += "<select name=\"";
  body += field;
  body += "\">";
  body += "<option value=\"0\"";
  body += (noOffConnected == 0 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "Brak" : "None";
  body += "</option>";

  for (uint8_t i = 1; i <= noOfItems; i++) {
    body += "<option value=\"";
    body += i;
    body += "\"";
    body += noOffConnected == i ? " selected=\"selected\"" : "";
    body += ">";
    body += i;
    body += "</option>";
  }

  body += "</select>";
  body += "</div>";

  return body;
}

#if defined(T1_CONFIG) || defined(T2_CONFIG)
const String
AFESitesGenerator::generateTwoValueController(REGULATOR configuration,
                                              uint8_t type) {

  String body = "<fieldset>";

  body += "<div class=\"cc\">";
  body += "<label>";
  body += "<input name=\"te\" type=\"checkbox\" value=\"1\"";
  body += configuration.enabled ? " checked=\"checked\">" : ">";

  body += language == 0 ? " włączony" : "enabled";

  body += "?</label>";
  body += "</div>";

  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Włącz jeśli" : "On if";
  if (type == THERMOSTAT_REGULATOR) {
    body += " temp.";
  } else {
    body += language == 0 ? " wilgo." : " Humidity ";
  }
  body += language == 0 ? "jest" : "is";
  body += "</label>";

  body += "<select name=\"ta\"><option value=\"0\"";
  body += (configuration.turnOnAbove == 0 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "mniejsza" : "below";
  body += "</option>";
  body += "<option value=\"1\"";
  body += (configuration.turnOnAbove == 1 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "większa" : "above";
  body += "</option>";
  body += "</select>";
  body += "<input name=\"tn\" type=\"number\" value=\"";
  body += configuration.turnOn;
  if (type == THERMOSTAT_REGULATOR) {
    body += "\" min=\"-55\" max=\"125\"";
  } else {
    body += "\" min=\"0\" max=\"100\"";
  }
  body += "step=\"any\"><span class=\"hint\">";
  body += language == 0 ? "Zakres" : "Range";
  if (type == THERMOSTAT_REGULATOR) {
    body += ": -55&deg;C : +125&deg;C (-67&deg;F : +260&deg;F)";
  } else {
    body += ": 0% : 100%";
  }

  body += "</span></div>";

  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Wyłącz jeśli" : "Off if";
  if (type == THERMOSTAT_REGULATOR) {
    body += " temp.";
  } else {
    body += language == 0 ? " wilgo." : " Humidity ";
  }
  body += language == 0 ? "jest" : "is";
  body += "</label>";
  body += "<select name=\"tb\"><option value=\"0\"";
  body += (configuration.turnOffAbove == 0 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "mniejsza" : "below";
  body += "</option>";
  body += "<option value=\"1\"";
  body += (configuration.turnOffAbove == 1 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "większa" : "above";
  body += "</option>";
  body += "</select>";
  body += "<input name=\"tf\" type=\"number\" value=\"";
  body += configuration.turnOff;
  if (type == THERMOSTAT_REGULATOR) {
    body += "\" min=\"-55\" max=\"125\"";
  } else {
    body += "\" min=\"0\" max=\"100\"";
  }
  body += "step=\"any\"><span class=\"hint\">";
  body += language == 0 ? "Zakres" : "Range";
  if (type == THERMOSTAT_REGULATOR) {
    body += ": -55&deg;C : +125&deg;C (-67&deg;F : +260&deg;F)";
  } else {
    body += ": 0% : 100%";
  }

  body += "</div></fieldset>";

  return body;
}
#endif
