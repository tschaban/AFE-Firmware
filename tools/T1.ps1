# T1 Firmware Build Script - PowerShell Version
# how to run it example: .\main.ps1 -type "1" -version "3.5.0" -language "pl" -development $true

$targetLanguage = @("pl", "en")

$targetAPI = @("domoticz.api", "standard.api", "homeassistant.api")

$rootPath = "D:\Adrian\Desktop\"
$targetFolder = Join-Path $rootPath "afe.firmware.t$type.$version"

<#
### AFE_DEVICE_ID ###

0 ESP Generic
10 T1 E2 Custom
11 T1 E1 Custom
30 ESP32 30Pins
31 ESP32 38Pins

### HARDWARE ID ###

0  : ESP8266
1  : ESP8285
2  : ESP32-30p
3  : ESP32-38p
4  : E1
5  : E2
6  : ESP32-38p-E2
#>

# targetHardware: [Folder name, AFE_DEVICE_ID, Part of the file]
$targetHardware = @(
    @("esp8266", 0, ""),
    @("esp8285", 0, ""),
    @("esp32", 30, "30pins"),
    @("esp32", 31, "38pins"),
    @("esp8266", 11, "e1"),
    @("esp8266", 10, "e2"),
    @("esp32", 31, "38pins-e2")
)

$folderStructure = @(
    $targetFolder,
    "$targetFolder/$($targetLanguage[0])",
    "$targetFolder/$($targetLanguage[1])"
)

# Initialize sourceFolder array
$sourceFolder = @()

# Standard API ESP8266 4MB
$sourceFolder += @{
    file = "H:/projekty/AFE-Firmware/.pio/build/AFE_T1_8266_4m-generic/firmware.bin"
    chip = 8266
    size = 4
    hardware = 0
    debug = $false
    api = $targetAPI[1]
}

$sourceFolder += @{
    file = "H:/projekty/AFE-Firmware/.pio/build/AFE_T1_8266_4m-generic-development/firmware.bin"
    chip = 8266
    size = 4
    hardware = 0
    debug = $true
    api = $targetAPI[1]
}

# Standard API ESP8266 1MB
$sourceFolder += @{
    file = "H:/projekty/AFE-Firmware/.pio/build/AFE_T1_8266_1m-generic/firmware.bin"
    chip = 8266
    size = 1
    hardware = 0
    debug = $false
    api = $targetAPI[1]
}

# Standard API ESP8285
$sourceFolder += @{
    file = "H:/projekty/AFE-Firmware/.pio/build/AFE_T1_8285-generic/firmware.bin"
    chip = 8285
    size = 1
    hardware = 1
    debug = $false
    api = $targetAPI[1]
}

# Standard API Custom versions
$sourceFolder += @{
    file = "H:/projekty/AFE-Firmware/.pio/build/AFE_T1_8266_1m-generic-e1/firmware.bin"
    chip = 8266
    size = 1
    hardware = 4
    debug = $false
    api = $targetAPI[1]
}

$sourceFolder += @{
    file = "H:/projekty/AFE-Firmware/.pio/build/AFE_T1_8266_4m-generic-e2/firmware.bin"
    chip = 8266
    size = 4
    hardware = 5
    debug = $false
    api = $targetAPI[1]
}

# Standard API ESP32
$sourceFolder += @{
    file = "H:/projekty/AFE-Firmware/.pio/build/AFE_T1_esp32_4m_30P-generic/firmware.bin"
    chip = 32
    size = 4
    hardware = 2
    debug = $false
    api = $targetAPI[1]
}

$sourceFolder += @{
    file = "H:/projekty/AFE-Firmware/.pio/build/AFE_T1_esp32_4m_38P-generic/firmware.bin"
    chip = 32
    size = 4
    hardware = 3
    debug = $false
    api = $targetAPI[1]
}

$sourceFolder += @{
    file = "H:/projekty/AFE-Firmware/.pio/build/AFE_T1_esp32_4m_30P-generic-development/firmware.bin"
    chip = 32
    size = 4
    hardware = 2
    debug = $true
    api = $targetAPI[1]
}

$sourceFolder += @{
    file = "H:/projekty/AFE-Firmware/.pio/build/AFE_T1_esp32_4m_38P-generic-development/firmware.bin"
    chip = 32
    size = 4
    hardware = 3
    debug = $true
    api = $targetAPI[1]
}

# Standard API ESP32 38Pins E2
$sourceFolder += @{
    file = "H:/projekty/AFE-Firmware/.pio/build/AFE_T1_esp32_4m_38P-generic-e2/firmware.bin"
    chip = 32
    size = 4
    hardware = 6
    debug = $false
    api = $targetAPI[1]
}

$sourceFolder += @{
    file = "H:/projekty/AFE-Firmware/.pio/build/AFE_T1_esp32_4m_38P-generic-development-e2/firmware.bin"
    chip = 32
    size = 4
    hardware = 6
    debug = $true
    api = $targetAPI[1]
}

# Domoticz API ESP8266 4MB
$sourceFolder += @{
    file = "H:/projekty/AFE-Firmware/.pio/build/AFE_T1_8266_4m-domoticz/firmware.bin"
    chip = 8266
    size = 4
    hardware = 0
    debug = $false
    api = $targetAPI[0]
}

$sourceFolder += @{
    file = "H:/projekty/AFE-Firmware/.pio/build/AFE_T1_8266_4m-domoticz-development/firmware.bin"
    chip = 8266
    size = 4
    hardware = 0
    debug = $true
    api = $targetAPI[0]
}

# Domoticz API ESP8266 1MB
$sourceFolder += @{
    file = "H:/projekty/AFE-Firmware/.pio/build/AFE_T1_8266_1m-domoticz/firmware.bin"
    chip = 8266
    size = 1
    hardware = 0
    debug = $false
    api = $targetAPI[0]
}

# Domoticz API ESP8285
$sourceFolder += @{
    file = "H:/projekty/AFE-Firmware/.pio/build/AFE_T1_8285-domoticz/firmware.bin"
    chip = 8285
    size = 1
    hardware = 1
    debug = $false
    api = $targetAPI[0]
}

# Domoticz API Custom versions
$sourceFolder += @{
    file = "H:/projekty/AFE-Firmware/.pio/build/AFE_T1_8266_1m-domoticz-e1/firmware.bin"
    chip = 8266
    size = 1
    hardware = 4
    debug = $false
    api = $targetAPI[0]
}

$sourceFolder += @{
    file = "H:/projekty/AFE-Firmware/.pio/build/AFE_T1_8266_4m-domoticz-e2/firmware.bin"
    chip = 8266
    size = 4
    hardware = 5
    debug = $false
    api = $targetAPI[0]
}

# Domoticz API ESP32
$sourceFolder += @{
    file = "H:/projekty/AFE-Firmware/.pio/build/AFE_T1_esp32_4m_30P-domoticz/firmware.bin"
    chip = 32
    size = 4
    hardware = 2
    debug = $false
    api = $targetAPI[0]
}

$sourceFolder += @{
    file = "H:/projekty/AFE-Firmware/.pio/build/AFE_T1_esp32_4m_38P-domoticz/firmware.bin"
    chip = 32
    size = 4
    hardware = 3
    debug = $false
    api = $targetAPI[0]
}

$sourceFolder += @{
    file = "H:/projekty/AFE-Firmware/.pio/build/AFE_T1_esp32_4m_30P-domoticz-development/firmware.bin"
    chip = 32
    size = 4
    hardware = 2
    debug = $true
    api = $targetAPI[0]
}

$sourceFolder += @{
    file = "H:/projekty/AFE-Firmware/.pio/build/AFE_T1_esp32_4m_38P-domoticz-development/firmware.bin"
    chip = 32
    size = 4
    hardware = 3
    debug = $true
    api = $targetAPI[0]
}

# Domoticz API ESP32 38Pins E2
$sourceFolder += @{
    file = "H:/projekty/AFE-Firmware/.pio/build/AFE_T1_esp32_4m_38P-domoticz-e2/firmware.bin"
    chip = 32
    size = 4
    hardware = 6
    debug = $false
    api = $targetAPI[0]
}

$sourceFolder += @{
    file = "H:/projekty/AFE-Firmware/.pio/build/AFE_T1_esp32_4m_38P-domoticz-development-e2/firmware.bin"
    chip = 32
    size = 4
    hardware = 6
    debug = $true
    api = $targetAPI[0]
}

# HOME ASSISTANT

# HA API ESP8266 4MB
$sourceFolder += @{
    file = "H:/projekty/AFE-Firmware/.pio/build/AFE_T1_8266_4m-ha/firmware.bin"
    chip = 8266
    size = 4
    hardware = 0
    debug = $false
    api = $targetAPI[2]
}

$sourceFolder += @{
    file = "H:/projekty/AFE-Firmware/.pio/build/AFE_T1_8266_4m-ha-development/firmware.bin"
    chip = 8266
    size = 4
    hardware = 0
    debug = $true
    api = $targetAPI[2]
}

# HA API ESP8266 1MB
$sourceFolder += @{
    file = "H:/projekty/AFE-Firmware/.pio/build/AFE_T1_8266_1m-ha/firmware.bin"
    chip = 8266
    size = 1
    hardware = 0
    debug = $false
    api = $targetAPI[2]
}

# HA API ESP8285
$sourceFolder += @{
    file = "H:/projekty/AFE-Firmware/.pio/build/AFE_T1_8285-ha/firmware.bin"
    chip = 8285
    size = 1
    hardware = 1
    debug = $false
    api = $targetAPI[2]
}

# HA API Custom E1 and E2
$sourceFolder += @{
    file = "H:/projekty/AFE-Firmware/.pio/build/AFE_T1_8266_1m-ha-e1/firmware.bin"
    chip = 8266
    size = 1
    hardware = 4
    debug = $false
    api = $targetAPI[2]
}

$sourceFolder += @{
    file = "H:/projekty/AFE-Firmware/.pio/build/AFE_T1_8266_4m-ha-e2/firmware.bin"
    chip = 8266
    size = 4
    hardware = 5
    debug = $false
    api = $targetAPI[2]
}

# HA API ESP32
$sourceFolder += @{
    file = "H:/projekty/AFE-Firmware/.pio/build/AFE_T1_esp32_4m_30P-ha/firmware.bin"
    chip = 32
    size = 4
    hardware = 2
    debug = $false
    api = $targetAPI[2]
}

$sourceFolder += @{
    file = "H:/projekty/AFE-Firmware/.pio/build/AFE_T1_esp32_4m_38P-ha/firmware.bin"
    chip = 32
    size = 4
    hardware = 3
    debug = $false
    api = $targetAPI[2]
}

$sourceFolder += @{
    file = "H:/projekty/AFE-Firmware/.pio/build/AFE_T1_esp32_4m_30P-ha-development/firmware.bin"
    chip = 32
    size = 4
    hardware = 2
    debug = $true
    api = $targetAPI[2]
}

$sourceFolder += @{
    file = "H:/projekty/AFE-Firmware/.pio/build/AFE_T1_esp32_4m_38P-ha-development/firmware.bin"
    chip = 32
    size = 4
    hardware = 3
    debug = $true
    api = $targetAPI[2]
}

# HA API ESP32 38Pins E2
$sourceFolder += @{
    file = "H:/projekty/AFE-Firmware/.pio/build/AFE_T1_esp32_4m_38P-ha-e2/firmware.bin"
    chip = 32
    size = 4
    hardware = 6
    debug = $false
    api = $targetAPI[2]
}

$sourceFolder += @{
    file = "H:/projekty/AFE-Firmware/.pio/build/AFE_T1_esp32_4m_38P-ha-development-e2/firmware.bin"
    chip = 32
    size = 4
    hardware = 6
    debug = $true
    api = $targetAPI[2]
}
