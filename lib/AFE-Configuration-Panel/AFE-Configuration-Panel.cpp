#include "AFE-Configuration-Panel.h"

AFEConfigurationPanel::AFEConfigurationPanel() {
}

String AFEConfigurationPanel::generateMainSite() {
  String page = Site.generateHeader();
  page+=Site.generateLeftPanel();
  uint8_t labels[] = {1,2,3};
  page+=Site.addConfigurationBlock("Nazwa urządzenia","Nazwa jest wykorzystywana jako nazwa hosta w Twojej lokalnej sieci WiFi oraz jako nazwa punktu dostępowego WiFi dla konfiguracji urządzenia"
  ,"Handle root"
  ,labels);
  page+=Site.generateRightPanel();
  page+=Site.generateFooter();
  return page;
}
