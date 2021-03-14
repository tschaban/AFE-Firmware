/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_CSS_h
#define _AFE_CSS_h

#include <AFE-Configuration.h>

const char JSON_RESPONSE[] PROGMEM =
    "{\"device\":{\"type\":\"{{device.type}}\",\"name\":\"{{device.name}}\"},\"command\":\"{{"
    "request.command}}\",\"data\":{{response.data}},\"status\":\"{{response.status}}\"}";


const char HTTP_HEADER[] PROGMEM =
    "<!doctype html><html lang=\"{{s.lang}}\"><head><meta "
    "charset=\"utf-8\">{{s.redirect}}<title>AFE "
    "Firmware {{f.version}} "
    "[T{{f.type}}]</"
    "title><link href=\"https://fonts.googleapis.com/css2?family=Titillium+Web&display=swap\" rel=\"stylesheet\"><style>html{background:url(http://api.smartnydom.pl/images/background.jpg) no-repeat center center fixed;-webkit-background-size:cover;-moz-background-size:cover;-o-background-size:cover;background-size:cover}.l,.r{margin:0;padding:0 1.2em 1.2em 1.2em;display:table-cell}.ltit,body{margin:auto}.b,.itm a,a{text-decoration:none}body{padding:2em;max-width:980px}.c{display:table;width:100%;margin-bottom:3em}.l{width:280px;background:#000;color:#fff;border-top-left-radius:.8em;border-bottom-left-radius:.8em;opacity:.8}.r{background:#eee;border-top-right-radius:.8em;border-bottom-right-radius:.8em;opacity:.93}a,button,h1,h3,h4,input,label,li,p,span{font-family:'Titillium Web',sans-serif}h4{font-size:.8em;margin-bottom:1px}.lst{list-style:none;margin:0;padding:0}.itm a,.itm i{display:block;white-space:nowrap;padding:0 1em;color:#777;font-size:.8em}.itm i{font-weight:700;padding-top:.5em}.cf label,.hint,input,select{display:inline-block;vertical-align:middle}.itm a:focus,.itm a:hover{background-color:#aaa;text-decoration:none;padding:0 1.5em;color:#000}.ci{margin-bottom:1em}.ci h1{color:#999;border-bottom:1px solid #ddd;font-size:1.2em;font-weight:500;margin:0}.ci .cd{color:#aaa;font-size:80%;font-style:italic;margin-bottom:2em;margin-top:0}.cm{color:#999;font-size:90%;margin:0 0 20px}.la{margin:0 .1em;padding:.3em 1em;color:#fff;background:#999;font-size:80%}.cc label,.cf label{font-size:.875em}.lr{background:#ca3c3c}.bs,.lg{background:#2fb548}fieldset{margin:0;padding:.35em 0 .75em;border:0}.cf{margin-bottom:.3em}.cc{margin:0 0 .1em 9.4em}.cf label{text-align:right;width:10em;margin:0 1em 0 0}input,select{padding:.4em .6em;border:1px solid #ccc;box-sizing:border-box}.hint{padding-left:.5em;color:#aaa;font-size:80%}.b{font-size:100%;padding:.2em 1em;border:1px solid #999;border:transparent;color:#fff}.be{background:#ca3c3c}.bw{background:#df7514}.bc{background:#42b8dd}.b:focus,.b:hover{background-image:-webkit-linear-gradient(transparent,rgba(0,0,0,.05) 40%,rgba(0,0,0,.1));background-image:linear-gradient(transparent,rgba(0,0,0,.05) 40%,rgba(0,0,0,.1))}</style></head><body>";

const char HTTP_FIRMWARE_BANNER_IMAGE[] PROGMEM = "<img src=\"http://api.smartnydom.pl/logo/T{{f.type}}/{{f.version}}/{{f.deviceID}}/{{f.deviceType}}/{{L_LANGUAGE_SHORT}}\" style=\"width: 100%;display: block\" alt=\"AFE Firmware\">"; 
const char HTTP_FIRMWARE_BANNER_TEXT[] PROGMEM = "<h3 class=\"la\">AFE Firmware T{{firmware.type}}v{{f.version}} {{f.deviceName}}</h3>"; 


const char HTTP_FIRMWARE_INFO_ITEM[] PROGMEM = "<li style=\"text-decoration:none;padding-left:1.5em;color:#777;font-size:0.9em\">{{f.info}}</li>";


const char HTTP_FOOTER_CONNECTED[] PROGMEM = "<a style=\"color:#0475b6;\" "
                                             "href=\"https://"
                                             "{{s.lang}}.donate.afe-firmware."
                                             "smartnydom.pl\">{{L_DONATE}}</a>";

const char HTTP_FORM_BLOCK_HEADER[] PROGMEM =
    "<div class=\"ci\"><h1>{{title}}</h1><p "
    "class=\"cd\">{{description}}</p><fieldset>";

const char HTTP_FORM_BLOCK_CLOSURE[] PROGMEM = "</fieldset></div>";

/* Fixed menu items */
const char HTTP_MENU_ITEM[] PROGMEM =
    "<li class=\"itm\"><a href=\"/?o={{site.id}}\">{{item.title}}</a></li>";

const char HTTP_MENU_ITEM_EXTERNAL[] PROGMEM =
    "<li class=\"itm\"><a href=\"{{site.url}}\">{{item.title}}</a></li>";


const char HTTP_MENU_SUBITEMS_HEADER[] PROGMEM =
    "<li class=\"itm\"><i>{{item.title}}</i></li>";

const char HTTP_MENU_SUBITEM[] PROGMEM =
    "<li class=\"itm\"><a href=\"/?o={{site.id}}&i={{item.id}}\">&#8227; "
    "{{item.title}}: {{item.id.display}}</a></li>";

const uint8_t GPIOS[] PROGMEM = {0, 1, 2, 3, 4, 5, 9, 10, 12, 13, 14, 15, 16};

#ifdef AFE_CONFIG_HARDWARE_MCP23017
const uint8_t MCP23017_GPIOS_ID[] PROGMEM = {0, 1, 2,  3,  4,  5,  6,  7,
                                             8, 9, 10, 11, 12, 13, 14, 15};
#endif

#ifdef AFE_CONFIG_HARDWARE_I2C
const char HTTP_ITEM_SCANNED_I2C_ADDRESSES_HEADER[] PROGMEM =
    "<div class=\"cf\"><label>{{item.label}}: </label><select "
    "name=\"address\"><option value=\"255\" "
    "{{item.selected}}>{{L_NONE}}</option>";

const char HTTP_ITEM_SCANNED_I2C_ADDRESSES[] PROGMEM =
    "<option value=\"{{item.value}}\" {{item.selected}}>[0x{{device.address}}] "
    ": {{device.name}}</option>";
#endif // AFE_CONFIG_HARDWARE_I2C

const char HTTP_INFO_TEXT[] PROGMEM = "<br><p class=\"cm\">{{item.value}}</p>";

const char HTTP_ITEM_CHECKBOX[] PROGMEM =
    "<div class=\"cc\"><label><input name=\"{{item.name}}\" "
    "type=\"{{item.type}}\" "
    "value=\"{{item.value}}\"{{item.checked}}{{item.disabled}}>{{item.label}}</"
    "label>{{item.hint}}</div>";

const char HTTP_ITEM_SELECT_OPEN[] PROGMEM =
    "<div class=\"cf\"><label>{{item.label}}</label><select "
    "name=\"{{item.name}}\">";

const char HTTP_ITEM_SELECT_OPTION[] PROGMEM =
    "<option value=\"{{item.value}}\" "
    "{{item.selected}}>{{item.label}}</option>";

const char HTTP_ITEM_SELECT_CLOSE[] PROGMEM = "</select></div>";

const char HTTP_ITEM_HINT[] PROGMEM = "<br><p class=\"cm\">{{item.hint}}</p>";

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
const char HTTP_ITEM_REGULATOR[] PROGMEM =
    "<div class=\"cf\"><label>{{L_REGULATOR_TURN_IF}}</label><select "
    "name=\"{{item.name}}\"><option "
    "value=\"0\"{{item.selected-0}}>{{L_REGULATOR_LOWER}}</option><option "
    "value=\"1\"{{item.selected-1}}>{{L_REGULATOR_HIGHER}}</option></"
    "select><input "
    "name=\"{{item.input.name}}\" type=\"number\" value=\"{{item.value}}\" "
    "step=\"any\"></div>";
#endif

#endif // _AFE_CSS_h
