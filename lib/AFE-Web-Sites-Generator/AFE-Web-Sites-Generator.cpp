#include "AFE-Web-Sites-Generator.h"


AFEWebSites::AFEWebSites() {
}

String AFEWebSites::generateHTMLPage(String &page) {
        String htmlPage = generateHeader();
        htmlPage+=generateLeftPanel();
        htmlPage+=page;
        htmlPage+=generateRightPanel();
        htmlPage+=generateFooter();
        return htmlPage;
}


String AFEWebSites::generateHeader() {
        String page = "<!doctype html>";
        page+="<html lang=\"en\">";
        page+="<head>";
        page+="<meta charset=\"utf-8\">";
        page+="<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
        page+="<title>AFE Firmware</title>";
        page+="<style>html{font-family:sans-serif;-ms-text-size-adjust:100%;-webkit-text-size-adjust:100%}body{margin:0}footer,header,menu,nav,section{display:block}a{background-color:transparent}a:active,a:hover{outline:0}b,strong{font-weight:bold}h1{font-size:2em;margin:.67em 0}small{font-size:80%}sub,sup{font-size:75%;line-height:0;position:relative;vertical-align:baseline}sup{top:-0.5em}sub{bottom:-0.25em}button,input,select{color:inherit;font:inherit;margin:0}button{overflow:visible}button,select{text-transform:none}button,html input[type=\"button\"],input[type=\"submit\"]{-webkit-appearance:button;cursor:pointer}button::-moz-focus-inner,input::-moz-focus-inner{border:0;padding:0}input{line-height:normal}input[type=\"checkbox\"],input[type=\"radio\"]{box-sizing:border-box;padding:0}input[type=\"number\"]::-webkit-inner-spin-button,input[type=\"number\"]::-webkit-outer-spin-button{height:auto}table{border-collapse:collapse;border-spacing:0}td,th{padding:0}.pure-form input[type=\"text\"],.pure-form input[type=\"password\"],.pure-form input[type=\"email\"],.pure-form input[type=\"url\"],.pure-form input[type=\"number\"],.pure-form select{padding:.5em .6em;display:inline-block;border:1px solid #ccc;box-shadow:inset 0 1px 3px #ddd;border-radius:4px;vertical-align:middle;box-sizing:border-box}.pure-form input:not([type]){padding:.5em .6em;display:inline-block;border:1px solid #ccc;box-shadow:inset 0 1px 3px #ddd;border-radius:4px;box-sizing:border-box}.pure-form input[type=\"text\"]:focus,.pure-form input[type=\"password\"]:focus,.pure-form input[type=\"email\"]:focus,.pure-form input[type=\"url\"]:focus,.pure-form input[type=\"number\"]:focus,.pure-form select:focus{outline:0;border-color:#129fea}.pure-form input:not([type]):focus{outline:0;border-color:#129fea}.pure-form input[type=\"file\"]:focus,.pure-form input[type=\"radio\"]:focus,.pure-form input[type=\"checkbox\"]:focus{outline:thin solid #129fea;outline:1px auto #129fea}.pure-form .pure-checkbox,.pure-form .pure-radio{margin:.5em 0;display:block}.pure-form select{height:2.25em;border:1px solid #ccc;background-color:white}.pure-form select[multiple]{height:auto}.pure-form label{margin:.5em 0 .2em}.pure-form fieldset{margin:0;padding:.35em 0 .75em;border:0}.pure-form-stacked input[type=\"text\"],.pure-form-stacked input[type=\"password\"],.pure-form-stacked input[type=\"email\"],.pure-form-stacked input[type=\"url\"],.pure-form-stacked input[type=\"number\"],.pure-form-stacked input[type=\"file\"],.pure-form-stacked select,.pure-form-stacked label{display:block;margin:.25em 0}.pure-form-aligned input,.pure-form-aligned select,.pure-form-aligned .pure-help-inline,.pure-form-message-inline{display:inline-block;*display:inline;*zoom:1;vertical-align:middle}.pure-form-aligned .pure-control-group{margin-bottom:.5em}.pure-form-aligned .pure-control-group label{text-align:right;display:inline-block;vertical-align:middle;width:10em;margin:0 1em 0 0}.pure-form-aligned .pure-controls{margin:1.5em 0 0 11em}.pure-form input.pure-input-rounded,.pure-form .pure-input-rounded{border-radius:2em;padding:.5em 1em}.pure-form .pure-help-inline,.pure-form-message-inline{display:inline-block;padding-left:.3em;color:#666;vertical-align:middle;font-size:.875em}@media only screen and (max-width :480px){.pure-form button[type=\"submit\"]{margin:.7em 0 0}.pure-form input:not([type]),.pure-form input[type=\"text\"],.pure-form input[type=\"password\"],.pure-form input[type=\"email\"],.pure-form input[type=\"url\"],.pure-form input[type=\"number\"],.pure-form label{margin-bottom:.3em;display:block}.pure-group input:not([type]),.pure-group input[type=\"text\"],.pure-group input[type=\"password\"],.pure-group input[type=\"email\"],.pure-group input[type=\"url\"],.pure-group input[type=\"number\"]{margin-bottom:0}.pure-form-aligned .pure-control-group label{margin-bottom:.3em;text-align:left;display:block;width:100%}.pure-form-aligned .pure-controls{margin:1.5em 0 0 0}.pure-form .pure-help-inline,.pure-form-message-inline,.pure-form-message{display:block;font-size:.75em;padding:.2em 0 .8em}}.pure-g{*letter-spacing:normal;*word-spacing:-0.43em;text-rendering:optimizespeed;font-family:FreeSans,Arimo,\"Droid Sans\",Helvetica,Arial,sans-serif;-webkit-align-content:flex-start;-ms-flex-line-pack:start;align-content:flex-start}@media all and (-ms-high-contrast:none),(-ms-high-contrast:active){table .pure-g{display:block}}.opera-only :-o-prefocus,.pure-g{word-spacing:-0.43em}.pure-u{display:inline-block;*display:inline;zoom:1;letter-spacing:normal;word-spacing:normal;vertical-align:top;text-rendering:auto}.pure-g [class *= \"pure-u\"]{font-family:sans-serif}@media screen and (min-width:48em){.pure-u-md-1,.pure-u-md-1-4,.pure-u-md-3-4{display:inline-block;*display:inline;zoom:1;letter-spacing:normal;word-spacing:normal;vertical-align:top;text-rendering:auto}.pure-u-md-1-4{width:25%;*width:24.9690%}.pure-u-md-3-4{width:75%;*width:74.9690%}.pure-u-md-1{width:100%}}*{-webkit-box-sizing:border-box;-moz-box-sizing:border-box;box-sizing:border-box}a{text-decoration:none;color:#3d92c9}a:hover,a:focus{text-decoration:underline}h3{font-weight:100}.pure-img-responsive{max-width:100%;height:auto}#layout{padding:0}.header{text-align:center;top:auto;margin:3em auto}.sidebar{background:#282828;color:#fff}.brand-title,.brand-tagline{margin:0}.brand-title{text-transform:uppercase}.brand-tagline{font-weight:300;color:#b0cadb}.nav-list{margin:0;padding:0;list-style:none}.nav-item{display:inline-block;*display:inline;zoom:1}.nav-item a{background:transparent;border:2px solid #b0cadb;color:#fff;margin-top:1em;letter-spacing:.05em;text-transform:uppercase;font-size:85%}.nav-item a:hover,.nav-item a:focus{border:2px solid #3d92c9;text-decoration:none}.content-subhead{text-transform:uppercase;color:#aaa;border-bottom:1px solid #eee;padding:.4em 0;font-size:110%;font-weight:500;letter-spacing:.1em}.content{padding:2em 1em 0}.post{padding-bottom:2em}.post-title{font-size:2em;color:#222;margin-bottom:.2em}.post-description{font-family:Georgia,\"Cambria\",serif;color:#444;line-height:1.8em}.post-meta{color:#999;font-size:90%;margin:0 0 20px 0}.post-category{margin:0 .1em;padding:.3em 1em;color:#fff;background:#999;font-size:80%}.post-category-design{background:#5aba59}.post-category-pure{background:#4d85d1}.post-category-yui{background:#8156a7}.post-category-js{background:#df2d4f}.footer{padding:1em 0}.footer a{color:#ccc;font-size:80%}.footer .pure-menu a:hover,.footer .pure-menu a:focus{background:0}@media(min-width:48em){.content{padding:2em 3em 0;margin-left:25%}.header{margin:6% 2em 0;text-align:right}.sidebar{position:fixed;top:0;bottom:0}.footer{text-align:center}}.hidden{display:none}.visible{display:block}.pure-button{display:inline-block;zoom:1;line-height:normal;white-space:nowrap;vertical-align:middle;text-align:center;cursor:pointer;-webkit-user-drag:none;-webkit-user-select:none;-moz-user-select:none;-ms-user-select:none;user-select:none;box-sizing:border-box}.pure-button::-moz-focus-inner{padding:0;border:0}.pure-button{font-family:inherit;font-size:100%;padding:.5em 1em;color:#444;color:rgba(0,0,0,0.80);border:1px solid #999;border:none rgba(0,0,0,0);background-color:#e6e6e6;text-decoration:none;border-radius:2px}.pure-button-hover,.pure-button:hover,.pure-button:focus{filter:alpha(opacity=90);background-image:-webkit-linear-gradient(transparent,rgba(0,0,0,0.05) 40%,rgba(0,0,0,0.10));background-image:linear-gradient(transparent,rgba(0,0,0,0.05) 40%,rgba(0,0,0,0.10))}.pure-button:focus{outline:0}.pure-button-active,.pure-button:active{box-shadow:0 0 0 1px rgba(0,0,0,0.15) inset,0 0 6px rgba(0,0,0,0.20) inset;border-color:#000\9}.button-success,.button-error,.button-warning,.button-secondary{color:white;border-radius:4px;text-shadow:0 1px 1px rgba(0,0,0,0.2)}.button-success{background:#1cb841}.button-error{background:#ca3c3c}.button-warning{background:#df7514}.button-secondary{background:#42b8dd}.pure-menu{box-sizing:border-box}.pure-menu-list,.pure-menu-item{position:relative}.pure-menu-list{list-style:none;margin:0;padding:0}.pure-menu-item{padding:0;margin:0;height:100%}.pure-menu-link,.pure-menu-heading{display:block;text-decoration:none;white-space:nowrap}.pure-menu-heading{text-transform:uppercase;color:#b0cadb;font-weight:bold}.pure-menu-link{color:#777}.pure-menu-link,.pure-menu-heading{padding:.3em .1em}.pure-menu-active>.pure-menu-link,.pure-menu-link:hover,.pure-menu-link:focus{background-color:#eee;text-decoration:none}.pure-menu-horizontal{width:100%;white-space:nowrap}.pure-menu-horizontal .pure-menu-list{display:inline-block}.pure-menu-horizontal .pure-menu-item,.pure-menu-horizontal .pure-menu-heading,.pure-menu-horizontal .pure-menu-separator{display:inline-block;*display:inline;zoom:1;vertical-align:middle}</style>";
        page+="<script type=\"text/javascript\">";
        page+="hide=function(e){document.getElementById(e).style.display='none';};";
        page+="show=function(e){document.getElementById(e).style.display='inline';};";
        page+="</script>";
        page+="</head>";
        page+="<body>";
        page+="<div id=\"layout\" class=\"pure-g\">";
        page+="<div class=\"sidebar pure-u-1 pure-u-md-1-4\">";
        return page;
}

String AFEWebSites::generateLeftPanel() {
  String page = "<div class=\"header\">";
  page+="<h2 class=\"brand-title\">AFE Firmware</h2>";
  page+="<h3 class=\"brand-tagline\">dla urządzeń zbudowanych o ESP8266</h3>";

  page+="<nav class=\"nav\">";
  page+="<ul class=\"nav-list\">";
  page+="<li class=\"nav-item\"><a class=\"pure-button\" href=\"http://smart-house.adrian.czabanowski.com/firmware-sonoff/\" target=\"_blank\">Dokumentacja</a></li>";
  page+="<li class=\"nav-item\"><a class=\"pure-button\" href=\"https://smart-house.adrian.czabanowski.com/forum/\" target=\"_blank\">Forum</a></li>";
  page+="</ul>";
  page+="</nav>";

  page+="<br>";


  page+="<div class=\"pure-menu custom-restricted-width\">";
  page+="<span class=\"pure-menu-heading\">MENU</span>";

  page+="<ul class=\"pure-menu-list\">";
  page+=addMenuItem("#devicename","Nazwa");
  page+=addMenuItem("#devicemode","MQTT Broker");
  page+=addMenuItem("#","Domoticz");
  page+=addMenuItem("#","Czujnik");
  page+=addMenuItem("#","GPIO");
  page+=addMenuItem("#","Aktulizacja");
  page+=addMenuItem("#","Przywracanie ustawień");
  page+="<br><br>";
  page+=addMenuItem("#","Zakończ konfigurację");
  page+="</ul>";
  page+="</div>";


  page+="</div>";
  page+="</div>";
  page+="<div class=\"content pure-u-1 pure-u-md-3-4\">";


  return page;
}

String AFEWebSites::generateRightPanel() {
  String page = "<div class=\"footer\">";
  page+="<div class=\"pure-menu pure-menu-horizontal\">";
  page+="<ul>";
  page+=addMenuItem("","Wersja beta 1");
  page+=addMenuItem("https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE","Licencja");
  page+=addMenuItem("http://smart-house.adrian.czabanowski.com/forum/firmware-do-przelacznika-sonoff/","Pomoc");
  page+=addMenuItem("https://github.com/tschaban/AFE-Firmware","GitHub");
  page+="</ul>";
  page+="</div>";
  page+="</div>";
  return page;

}

String AFEWebSites::generateFooter() {
        String page = "</div>";
        page+="</div>";
        page+="</body>";
        page+="</html>";
        return page;
}

String AFEWebSites::addMenuItem(const String url, const String name) {
    return "<li class=\"pure-menu-item\"><a href=\">" + url + "\" class=\"pure-menu-link\">" + name + "</a></li>";
}

String AFEWebSites::addConfigurationBlock(const String title,const String description, const String body,  uint8_t labels[]) {
  String page ="<div class=\"posts\">";
  page+="<a name=\"devicename\"></a>";
  page+="<h1 class=\"content-subhead\">" + title + "</h1>";
  page+="<section class=\"post\">";
  page+="<header class=\"post-header\">";
  page+="<p class=\"post-meta\">";
  for (int i=0; i<sizeof(labels); i++) {
     page+= addLabel(labels[i]);
  }
  page+="</p>";
  page+="</header>";
  page+="<div class=\"post-description\">";
  page+="<p>" + description + "</p>";
  page+="<div class=\"pure-form pure-form-aligned\">";
  page+=body;
  page+="</div>";
  page+="</div>";
  page+="</section>";
  page+="</div>";

  return page;
}

String AFEWebSites::addLabel(uint8_t id) {
  if (id==0) {
    return "<span class=\"post-category post-category-js\">Wymagana konfigurcja</span>";
  } else {
    return "<span class=\"post-category post-category-js\">" + String(id) + "</span>";
  }
}
