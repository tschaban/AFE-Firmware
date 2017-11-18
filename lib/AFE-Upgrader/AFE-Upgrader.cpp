#include "AFE-Upgrader.h"

AFEUpgrader::AFEUpgrader() {
  FirmwareConfiguration = Data.getFirmwareConfiguration();
}

boolean AFEUpgrader::upgraded() {

  if (String(FirmwareConfiguration.version) == String(Defaults.getVersion())) {
    return false;
  } else {
    return true;
  }
}

void AFEUpgrader::upgrade() { Data.saveVersion(String(Defaults.getVersion())); }
