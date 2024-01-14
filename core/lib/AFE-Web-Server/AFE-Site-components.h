/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_CSS_h
#define _AFE_CSS_h

#include <AFE-Configuration.h>

const char JSON_RESPONSE[] PROGMEM =
    "{\"device\":{\"type\":\"{{device.type}}\",\"name\":\"{{device.name}}\"},\"command\":\"{{"
    "request.command}}\",\"data\":{{response.data}},\"status\":\"{{response.status}}\"}";


const char HTTP_HEADER[] PROGMEM =
    "<!doctype html><html lang=\"{{s.lang}}\"><head><meta "
    "charset=\"utf-8\">{{s.r}}<title>AFE : {{d}}</"
    "title><link href=\"https://fonts.googleapis.com/css2?family=Titillium+Web&display=swap\" rel=\"stylesheet\"><style>html{background:url({{u}}/images/background-3.8.0.jpg) no-repeat center center fixed;-webkit-background-size:cover;-moz-background-size:cover;-o-background-size:cover;background-size:cover}.l,.r{margin:0;padding:1.2em 1.2em 2.2em 1.2em;display:table-cell}.ltit,body{margin:auto}.b,.itm a,a{text-decoration:none}body{padding:2em;max-width:980px}.c{display:table;width:100%;margin-bottom:.5em}.m{text-decoration:none;padding-left:1.5em;color:#777;font-size:0.9em}.l{width:280px;background:#000;color:#fff;border-top-left-radius:.8em;border-bottom-left-radius:.8em;opacity:.93}.r{background:#eee;border-top-right-radius:.8em;border-bottom-right-radius:.8em;opacity:.93}a,button,h1,h3,h4,input,label,li,p,span,small{font-family:'Titillium Web',sans-serif}h4{font-size:.8em;margin-bottom:1px}.lst{list-style:none;margin:0;padding:0}.itm a,.itm i{display:block;white-space:nowrap;padding:0 1em;color:#aaa;font-size:.8em}.itm i{font-weight:700;padding-top:.5em}.cf label,.hint,input,select{display:inline-block;vertical-align:middle}.itm a:focus,.itm a:hover{background-color:#aaa;text-decoration:none;padding:0 1.5em;color:#000}.ci{margin-bottom:1em}.ci h1{color:#999;border-bottom:1px solid #ddd;font-size:1.2em;font-weight:500;margin:0}.ci .cd{color:#aaa;font-size:80%;font-style:italic;margin-bottom:2em;margin-top:0}.cm{color:#999;font-size:90%;margin:0 0 20px}.la{margin:0 .1em;padding:.3em 1em;color:#fff;background:#999;font-size:80%}.cc label,.cf label{font-size:.875em}.lr{background:#ca3c3c}.bs,.lg{background:#2fb548}fieldset{margin:0;padding:.35em 0 .75em;border:0}.cf{margin-bottom:.3em}.cc{margin:0 0 .1em 9.4em}.cf label{text-align:right;width:10em;margin:0 1em 0 0}input,select{padding:.4em .6em;border:1px solid #ccc;box-sizing:border-box}.hint{padding-left:.5em;color:#aaa;font-size:80%}.b{font-size:100%;padding:.2em 1em;border:1px solid #999;border:transparent;color:#fff}.be{background:#ca3c3c}.bw{background:#df7514}.bc{background:#42b8dd}.b:focus,.b:hover{background-image:-webkit-linear-gradient(transparent,rgba(0,0,0,.05) 40%,rgba(0,0,0,.1));background-image:linear-gradient(transparent,rgba(0,0,0,.05) 40%,rgba(0,0,0,.1))}</style></head><body>";

const char HTTP_FIRMWARE_BANNER_IMAGE[] PROGMEM = "<img src=\"{{u}}/logo/T{{f.t}}/{{f.v}}/{{f.n}}/{{f.h}}/{{f.l}}\" style=\"width: 100%;display: block\" alt=\"{{n}}\">"; 
const char HTTP_FIRMWARE_BANNER_TEXT[] PROGMEM = "<h3 class=\"la\">{{n}} T{{firmware.type}}v{{f.v}} {{f.d}}</h3>"; 


const char HTTP_MESSAGE_LINE_ITEM[] PROGMEM = "<li class=\"m\">{{I}}</li>";

const char HTTP_FORM_BLOCK_HEADER[] PROGMEM =
    "<div class=\"ci\"><h1>{{T}}</h1><p "
    "class=\"cd\">{{D}}</p><fieldset>";

const char HTTP_FORM_BLOCK_CLOSURE[] PROGMEM = "</fieldset></div>";

/* Fixed menu items */


const char HTTP_MENU_HEADER[] PROGMEM ="</ul><h4>&#10150; {{m.h}}</h4><ul class=\"lst\">";

const char HTTP_MENU_ITEM[] PROGMEM =
    "<li class=\"itm\"><a href=\"/?o={{s.i}}\">{{i.i}}</a></li>";

const char HTTP_MENU_ITEM_EXTERNAL[] PROGMEM =
    "<li class=\"itm\"><a href=\"{{s.u}}\">{{i.i}}</a></li>";


const char HTTP_MENU_SUBITEMS_HEADER[] PROGMEM =
    "<li class=\"itm\"><i>{{i.i}}</i></li>";

const char HTTP_MENU_SUBITEM[] PROGMEM =
    "<li class=\"itm\"><a href=\"/?o={{s.i}}&i={{i.D}}\">&#8227; "
    "{{i.i}}: {{i.y}}</a></li>";

#ifndef AFE_ESP32
const uint8_t GPIOS[] PROGMEM = {0, 1, 2, 3, 4, 5, 9, 10, 12, 13, 14, 15, 16};
const uint8_t GPIOS_ADC[] PROGMEM = {17};
#else
#ifdef AFE_ESP32_30_PINS
const uint8_t GPIOS[] PROGMEM = {0, 1, 2, 3, 4, 5, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33, 34, 35, 36, 39};
#else // AFE_ESP32_38_PINS
const uint8_t GPIOS[] PROGMEM = {0, 1, 2, 3, 4, 5, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39};
#endif // AFE_ESP32_30_PINS
const uint8_t GPIOS_ADC[] PROGMEM = {32, 33, 34, 35};
#endif // !AFE_ESP32

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
const uint8_t MCP23017_GPIOS_ID[] PROGMEM = {0, 1, 2,  3,  4,  5,  6,  7,
                                             8, 9, 10, 11, 12, 13, 14, 15};
#endif



#ifdef AFE_CONFIG_HARDWARE_I2C
const char HTTP_ITEM_SCANNED_I2C_ADDRESSES_HEADER[] PROGMEM =
    "<div class=\"cf\"><label>{{i.l}}: </label><select "
    "name=\"address\"><option value=\"255\" "
    "{{i.s}}>{{L_NONE}}</option>";

const char HTTP_ITEM_SCANNED_I2C_ADDRESSES[] PROGMEM =
    "<option value=\"{{i.v}}\" {{i.s}}>[0x{{device.address}}] "
    ": {{device.name}}</option>";
#endif // AFE_CONFIG_HARDWARE_I2C

const char HTTP_INFO_TEXT[] PROGMEM = "<br><p class=\"cm\">{{i.v}}</p>";

const char HTTP_ITEM_CHECKBOX[] PROGMEM =
    "<div class=\"cc\"><label><input name=\"{{i.n}}\" "
    "type=\"{{i.t}}\" "
    "value=\"{{i.v}}\"{{i.c}}{{i.d}}>{{i.l}}</"
    "label>{{i.h}}</div>";

const char HTTP_ITEM_SELECT_OPEN[] PROGMEM =
    "<div class=\"cf\"><label>{{i.l}}</label><select "
    "name=\"{{i.n}}\">";

const char HTTP_ITEM_SELECT_OPTION[] PROGMEM =
    "<option value=\"{{i.v}}\" "
    "{{i.s}}>{{i.l}}</option>";

const char HTTP_ITEM_SELECT_CLOSE[] PROGMEM = "</select></div>";

const char HTTP_ITEM_HINT[] PROGMEM = "<br><p class=\"cm\">{{i.h}}</p>";

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
const char HTTP_ITEM_REGULATOR[] PROGMEM =
    "<div class=\"cf\"><label>{{L.R}}</label><select "
    "name=\"{{i.n}}\"><option "
    "value=\"0\"{{i.s0}}>{{LRL}}</option><option "
    "value=\"1\"{{i.s1}}>{{LRH}}</option></"
    "select><input "
    "name=\"{{i.in}}\" type=\"number\" value=\"{{i.v}}\" "
    "step=\"any\"></div>";
#endif

#if defined(AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT) || defined(AFE_CONFIG_HARDWARE_CLED_LIGHT_CONTROLLED_EFFECT)
const char HTTP_FIXED_CLED_CONFIG_PARAMS[] PROGMEM = "<input name=\"m\" type=\"hidden\" value=\"0\"><input name=\"l\" type=\"hidden\" value=\"8\"><input name=\"o\" type=\"hidden\" value=\"39\">";
#endif

#ifdef AFE_CONFIG_HARDWARE_CLED
const char HTTP_ITEM_CLED_COLOR_RGB[] PROGMEM = "<div class=\"cf\"><label>{{i.l}}</label>{{i.r}}{{i.g}}{{i.b}}</div>";
const char HTTP_ITEM_CLED_COLOR_RGB_COLOR[] PROGMEM = "<input name=\"{{i.i}}\" type=\"number\" min=\"0\" max=\"255\" step=\"1\" value=\"{{i.v}}\">";
#endif



#endif // _AFE_CSS_h
