/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _LANG_EN_h
#define _LANG_EN_h

#define L_LANGUAGE "en-EN"
#define L_LANGUAGE_SHORT "en"

#define L_AFE_FIRMWARE "AFE Firmware"

/* Firmware names */
#if defined(AFE_DEVICE_SHELLY_1)
#define L_FIRMWARE_NAME "for Shelly-1"
#elif defined(AFE_DEVICE_SONOFF_BASIC_V1)
#define L_FIRMWARE_NAME "for Sonoff Basic"
#elif defined(T0_CONFIG)
#define L_FIRMWARE_NAME "Basic version"
#elif defined(T1_CONFIG)
#define L_FIRMWARE_NAME "Switch with temperature sensor"
#elif defined(T2_CONFIG)
#define L_FIRMWARE_NAME "Switch with temperature and humidity sensor"
#elif defined(T3_CONFIG)
#define L_FIRMWARE_NAME "for 4 WiFi switches and PIR sensors"
#elif defined(T4_CONFIG)
#define L_FIRMWARE_NAME "with MCP23017 expander"
#elif defined(T5_CONFIG)
#define L_FIRMWARE_NAME "to control gate controller"
#elif defined(T6_CONFIG)
#define L_FIRMWARE_NAME "Weather Station"
#endif

/* HTTP API */
#define L_DEVICE_NOT_EXIST "\"Device does not exist\""
#define L_COMMAND_NOT_IMPLEMENTED "\"Command is not implemented\""
#define L_404 "AFE FIRMWARE: Error 404: Page doesn't exist"
#define L_COMMNAD_NO_COMMAND "\"No command\""

#define L_DONATE "<small>&#128077; Like it? Consider supporting hundreds of hours spent on it by me. thx. adrian</small>"

/* Menu */
#define L_HARDWARE "Hardware"
#define L_FUNCTIONS "Functionalities"
#define L_FIRMWARE "Firmware"
#define L_DEVICE "Device"
#define L_NETWORK "Network"
#define L_MQTT_BROKER "MQTT Broker"
#define L_DOMOTICZ_SERVER "Domoticz Server"
#define L_LEDS "LEDs"
#define L_RELAYS_CONFIGURATION "Relays configuration"
#define L_RELAY "Relay"
#define L_BUTTONS_SWITCHES "Button / Switches"
#define L_SWITCH "Switch"
#define L_ANALOG_INPUT "Analog Input"
#define L_FIRMWARE_UPGRADE "Firmware upgrade"
#define L_RESET_DEVICE "Reset to orginal state"
#define L_FINISH_CONFIGURATION "&#10004; Finish configuration"
#define L_INFORMATION "INFORMATION"
#define L_DOCUMENTATION "Documentation"
#define L_SETTINGS "Settings"
#define L_SETTINGS_HOT_SPOT "Settings (HotSpot mode)"
#define L_PRO_VERSION "PRO Version"
#define L_NETWORK_CONNECTED "Connected"
#define L_DISCONNECTED "Disconnected"
#define L_MEASURMENTS_INTERVAL "Measurement's interval"
#define L_CONNECTIONS "Connections"
#define L_CLEDS "RGB LEDs"
#define L_HOME_ASSISTANT_INTEGRATION "Home Assistant Integration"


/* Forms commons */
#define L_NUMBER_OF_CHARS "chars"
#define L_ENABLED "enabled"
#define L_USERNAME "User name"
#define L_PASSWORD "Password"
#define L_SECONDS "seconds"
#define L_MILISECONDS "msec"
#define L_IP_ADDRESS "Address IP"
#define L_NONE "None"
#define L_NAME "Name"
#define L_SAVE "Save"
#define L_HELP "Help"
#define L_VERSION "Version"
#define L_DOMOTICZ_VERSION "Domoticz Version"
#define L_YES "Yes"
#define L_NO "No"
#define L_SENSOR "Sensor"
#define L_DISTANCE_UNIT "Distance unit"
#define L_CM "Centimeter"
#define L_M "Meter"
#define L_KM "Kilometers"
#define L_MILES "Miles"
#define L_RANGE "Range"
#define L_SENSITIVENESS "Sensitiveness"
#define L_ADDRESS "Address"
#define L_CORRECTIONS "Corrections"
#define L_UNITS "Units"
#define L_FREQUENCY "Frequency"
#define L_I2C_PORT "I2C Port"
#define L_GAIN "Gain"
#define L_FILE_NOT_FOUND_1 "The configuration file has not been found or there is a problem with its format"
#define L_FILE_NOT_FOUND_2 "Refresh page"
#define L_FILE_NOT_FOUND_3 "If the problem persists, set the configuration again or restore the initial settings"
#define L_ATTENTION "Attention"

/* Form: index */
#define L_INDEX_WRONG_PASSWORD "Incorrect password"
#define L_INDEX_NORMAL_MODE "Over WiFi"
#define L_INDEX_HOTSPOT_MODE "Direct via HotSpot"
#define L_INDEX_LAUNCH_CONFIGURATION_PANEL "Configuration panel"
#define L_INDEX_LAUNCH_CONFIGURATION_PANEL_HINT "To open configuration panel press one of the buttons."
#define L_INDEX_OPENING_CONFIG_PANEL "Opening {{n}} configuration panel"
#define L_INDX_INFORMATION_ABOUT_YOUR_VERSION "<a style=\"color:#0475b6\" href=\"/?o=47\">More</a> information about the device"
#define L_INDEX_DONATION "Donation"

/* Form: device coniguration */
#define L_DEVICE_NAME "Device name"
#define L_DEVICE_SECTION_INFO "Name is used for device identification within your LAN and a device's hotspot name used for configuration mode"
#define L_DEVICE_HARDWARE_CONFIGURATION "Hardware configuration"
#define L_DEVICE_HARDWARE_CONFIGURATION_INFO "Select connected items to your device"
#define L_DEVICE_NUMBER_OF_LEDS "Number of LEDs"
#define L_DEVICE_NUMBER_OF_RELAYS "Number of relays"
#define L_DEVICE_NUMBER_OF_SWITCHES "Number of switches"
#define L_DEVICE_NUMBER_OF_CONTACTRONS "Number of magnetic's sensors"
#define L_DEVICE_NUMBER_OF_CONTROLLED_GATES "Number of controlled gates/doors"
#define L_DEVICE_NUMBER_OF_HPMA115S0_SENSORS "HPMA115S0 sensor"
#define L_DEVICE_NUMBER_OF_BMEX80_SENSORS "Number of BMEx80's sensors"
#define L_DEVICE_NUMBER_OF_BH1750_SENSORS "Number of BH1750's sensors"
#define L_DEVICE_NUMBER_OF_AS3935_SENSORS "Number of AS3935's sensors"
#define L_DEVICE_NUMBER_OF_DS18B20_SENSORS "Number of DS18B20's sensors"
#define L_DEVICE_NUMBER_OF_REGULATORS "Number of regulators"
#define L_DEVICE_NUMBER_OF_THERMAL_PROTECTORS "Number of thermal protectors"
#define L_DEVICE_NUMBER_OF_BINARY_SENSORS "Number of binary's sensors"
#define L_DEVICE_NUMBER_OF_PN532_SENSORS "Number of PN532's sensors"
#define L_DEVICE_NUMBER_OF_MIFARE_CARDS "Number of MiFare cards"
#define L_DEVICE_NUMBER_OF_CLEDS "Number of WS281x RGB Leds"
#define L_DEVICE_NUMBER_OF_I2CS "Number of I2C port"
#define L_DEVICE_NUMBER_OF_TSL2561_SENSORS "Number of TSL2561's sensors"  
#define L_DEVICE_NUMBER_OF_ADC "Number of ADC inputs"

#define L_DEVICE_ADDITIONAL_FUNCTIONALITIES "Additional functionalities"
#define L_DEVICE_DO_MEASURE_ADC "Measurements from Analog Input"
#define L_DEVICE_CONTROLLING "Device controlling mechanism"
#define L_DEVICE_CONTROLLING_INFO "Enable / Disable APIs"
#define L_DEVICE_CONTROLLED_GATES "Gates/Doors controlled"
#define L_DEVICE_DOMOTICZ_VERSION_410 "4.10x or older"
#define L_DEVICE_DOMOTICZ_VERSION_2020 "2020.x or newer"
#define L_DEVICE_AUTOLOGOUT_TITLE "Automatic logout form the configuration panel"
#define L_DEVICE_AUTOLOGOUT_DESCRIPTION "Device configuration will be automatically closed after 10min. of idle time in the configuration panel"


/* Form: network configuration */
#define L_NETWORK_CONFIGURATION "WiFi Configuration"
#define L_NETWORK_CONFIGURATION_INFO  "Device without connection to network will only work in manual mode"
#define L_NETWORK_SSID "WiFi name"
#define L_NETWORK_REFRESH "Refresh"
#define L_NETWORK_DHCP_ENABLED "Configuration over DHCP"
#define L_NETWORK_GATEWAY "Gateway"
#define L_NETWORK_SUBNET "Subnet"
#define L_NETWORK_DEVICE_IP "Device IP address"
#define L_NETWORK_DEVICE_IP_INFO "You can set up device's IP address manually or automatically by DHCP"
#define L_NETWORK_NUMBER_OF_CONNECTIONS "Connection timeout"
#define L_NETWORK_TIME_BETWEEN_CONNECTIONS "Connections' attempts timeout"
#define L_NETWORK_SLEEP_TIME "Sleep time before next connection attempts"
#define L_NETWORK_ADVANCED "Advanced"
#define L_NETWORK_ALMOST "We're almost there!"
#define L_NETWORK_DEVICE_CONNECTS "Device is connecting to your wireless network: {{S}}"
#define L_NETWORK_CONNECT_TO "Connect now to your wireless network and open you WiFi Router configuration panel"
//#define L_NETWORK_CONNECT "Connect"
#define L_NETWORK_SEARCH_FOR_IP_ADDRESS "Look for a device's IP address where its MAC address is: {{M}} or hostname: AFE-Device"
#define L_NETWORK_FINISH_NETWORK_CONFIGURATION "Finish configuration in your browser using device's IP address"
#define L_NETWOK_NONE_BACKUP_SSID "--- None ---"
#define L_NETWORK_BACKUP_CONFIGURATION "Backup WiFi Configuration"
#define L_NETWORK_SWITCH_TO_BACKUP "Number of failures before switching to backup configuration"
#define L_NETWORK_RADIO_MODE "Radio Mode: 802.11"
#define L_NETWORK_OUTPUT_POWER "Transmit power (dBm)"


/* Form: MQTT Broker */
#define L_MQTT_TOPIC "Topic"
#define L_MQTT_IDX_LWT "LWT's IDX"
#define L_MQTT_TOPIC_LWT "LWT's Topic"
#define L_MQTT_CONFIGURATION_INFO "Enter MQTT Broker hostname or its IP address"
#define L_MQTT_TOPIC_EMPTY "If left empty, message won't be sent to MQTT Broker"
#define L_MQTT_TIMEOUT "Timeout"
#define L_MQTT_RETAIN_SECTION "MQTT Topics retain settings"
#define L_MQTT_RETAIN_SECTION_INFO "Sent messages will be retian by the MQTT Broker and automatically published to every client subscribing to them"
#define L_MQTT_RETAIN_LWT "Retain LWT messsage"
#define L_MQTT_RETAIN_ALL "Retain all messages"
#define L_MQTT_CONNECTION "Connection configuration"
#define L_MQTT_CONNECTION_HINT "In case the device must work without MQTT server host availability in the LAN, enable host PING check before establishing a connection to MQTT Broker.  You can aslo set the duration of a timeout per connection attempt. "
#define L_MQTT_USE_PING "Check if MQTT Server host is reachable in the LAN?"
#define L_MQTT_USE_PING_HINT "The host cannot block PING command"



/* Form: Domoticz server configuration */
#define L_DOMOTICZ_PROTOCOL "Protocol"
#define L_DOMOTICZ_CONFIGURATION "Domoticz Server"
#define L_DOMOTICZ_CONFIGURATION_INFO "Enter Domoticz hostname or its IP address"
#define L_DOMOTICZ_NO_IF_IDX_0  "If IDX is set to 0 then a value won't be sent to Domoticz"

/* Form: Led configuration */
#define L_LED_CHANGE_INDICATION "Change LED ligtning to opposite"
#define L_LED_SYSTEM "System LED"
#define L_LED_SYSTEM_INFO "Select LED which will be informing about device status and its events"

/* Form: relay */
#define L_RELAY_DEFAULT_VALUES "Default values"
#define L_RELAY_DEFAULT_POWER_RESTORED "When power is restored set it to"
#define L_RELAY_NO_ACTION "No action"
#define L_RELAY_ON "On"
#define L_RELAY_OFF "Off"
#define L_RELAY_LAST_KNOWN_STATE "Last known state"
#define L_RELAY_OPPOSITE_TO_LAST_KNOWN_STATE "Opposite to the last known state"
#define L_RELAY_DEFAULT_MQTT_CONNECTED "After establishing connection to MQTT Broker"
#define L_RELAY_DEFAULT_GET_FROM_MQTT "Get state from a relay's controlling system (over MQTT)"
#define L_RELAY_AUTOMATIC_SWITCHING_OFF "Automatic switching off of the relay"
#define L_RELAY_IMPULSE_DURATION "Impulse duration"
#define L_RELAY_SWITCH_OFF_AFTER "Switch off after"
#define L_RELAY_NO_ACTION_IF_0 "No action if it's set to 0"
#define L_RELAY_SWITCH_OFF_ABOVE "Switch off above"
#define L_RELAY_SELECT_LED_4_RELAY "Select LED informing about relay state"
#define L_RELAY_MQTT_TOPIC "MQTT Topic to control the relay"
#define L_RELAY_TRIGGERED "Triggered by"
#define L_RELAY_TRIGGERED_HIGH_SIGNAL "HIGH Signal"
#define L_RELAY_TRIGGERED_LOW_SIGNAL "LOW Signal"

/* Form: Thermal protection */
#define L_THERMAL_PROTECTOR "Thermal protection"
#define L_THERMAL_PROTECTOR_ENABLED "Enabled at startup"
#define L_THERMAL_PROTECTORS "Thermal protections"
#define L_THERMAL_PROTECTOR_AUTOMATIC_SWITCHING_OFF "Relay will be automatically switched off if the temperature exceeds set value"
#define L_THERMAL_PROTECTOR_MQTT_TOPIC "MQTT topic controlling thermal protection"


/* Form: Switch */
#define L_SWITCH_FUNCTIONALITY "Functionality"
#define L_SWITCH_SYSTEM_BUTTON "System button"
#define L_SWITCH_CONTROL_RELAY "Controlling only the relay"
#define L_SWITCH_CONTROL_GATE "Controlling only the gate"
#define L_SWITCH_RELAY_CONTROLLED "Controls relay"
#define L_SWITCH_TYPE "Type"
#define L_SWITCH_MONOSTABLE "Monostable"
#define L_SWITCH_BISTABLE "Bistable"
#define L_SWITCH_SENSITIVENESS_HINT "Sensitiveness should be adjusted if switch didn't behave as expected while pressing it"
#define L_SWITCH_BUTTON "Switch / Button"
#define L_SWITCH_MQTT_TOPIC "MQTT Topic to monitor the state of the switch"

/* Form: Binary sensor */
#define L_BINARY_SENSOR "Binary sensor"
#define L_BINARY_SENSORS "Binary sensors"
#define L_BINARY_SENSOR_MCP23017_CONNECTION "Sensor can be connected directly to the ESP's GPIO or through MCP23017 expander"
#define L_BINARY_SENSOR_MQTT_TOPIC "MQTT topic to monitor binary's sensor status"
#define L_BINARY_SENSOR_SENT_REVERTED_STATE "Invert the input signal"
#define L_BINARY_PULLUP_RESISTOR "Turn on the internal resistor (PULLUP)"
#define L_BINARY_SEND_AS_SWITCH "Send sensor state using ON/OFF values"
#define L_BINARY_SEND_AS_SWITCH_HINT "Default: OPEN/CLOSED"

/* Form: Analog Input */
#define L_ADC_CANT_CHANGE "can't be set"
#define L_ADC_MEASURED_VOLTAGE "Max input voltage on ADC"
#define L_ADC_NUMBER_OF_SAMPLES "Number of measurement samples"
#define L_ADC_IDX_RAW_DATA "IDX Raw data"
#define L_ADC_IDX_PERCENT "IDX Percent"
#define L_ADC_IDX_VOLTAGE "IDX voltage at input"
#define L_ADC_IDX_VOLTAGE_CALCULATED "IDX voltage calculated"
#define L_ADC_RESISTOR "Resistor"
#define L_ADC_VOLTAGE_DIVIDER "Voltage Divider"
#define L_ADC_MQTT_TOPIC "MQTT Topic: ADC"
#define L_ADC_INPUT "Input"

#define L_ADC_HA_VOLTAGE "Voltage on input"
#define L_ADC_HA_VOLTAGE_CALCULATED "Voltage"
#define L_ADC_HA_VOLTAGE_RAW "Raw"
#define L_ADC_HA_VOLTAGE_PERCENT "Raw"

#define L_BATTERY "Battery"
#define L_BATTERY_METER "Battery level"
#define L_BATTERY_MQTT_TOPIC "MQTT Topic: battery level"
#define L_BATTERY_MIN_LEVEL "Battery level: min"
#define L_BATTERY_MAX_LEVEL "Battery level: max"

/* Form: upgrade and post upgrade */
#define L_UPGRADE_SELECT_FIRMWARE "Select Firmware"
#define L_UPGRADE_READ_BEFORE "Read before upgrading the firmware"

#define L_UPGRADE_INTERUPTED "Don't refresh the browser while upgrading the device"
#define L_UPGRADE_DONT_PLUG_OFF "Don't plug off the device from the power source"
#define L_UPGRADE_TIME "The upgrade process will take up to a minute"
#define L_UPGRADE_AUTO_REBOOT "Device will be rebooted after upgrade process is finished"

#define L_UPGRADE_VIA_WAN "Upgrade over the Internet"
#define L_UPGRADE_VIA_WAN_HINT "Firmware will be automatically downloaded and uploaded to the device"

#define L_UPGRADE_FROM_FILE "Upload *.bin file"
#define L_UPGRADE "Upgrade with binary file"



#define L_UPGRADE_IN_PROGRESS "Upgrade in progress"

#define L_UPGRADE_FAILED "<span style=\"color:red\">Upgrade failed</span>"
#define L_UPGRADE_SUCCESSFUL "Upgrade finished successfully"
#define L_UPGRADE_DEVICE_WILL_BE_REBOOTED                                              \
  "After 15s the device will be rebooted. Please wait ..."
#define L_UPGRADE_RESTORE_DEFAULT_SETTINGS "Restore default settings"
#define L_RESTORE_CONFIGURATION_WILL_BE_REMOVED                                        \
  "<strong>Warning:</strong> restoring to default settings will remove all configuration's "             \
  "information, incl. WiFi configuration"
#define L_RESTORE_IN_PROGRESS "Restoring configuration is in progress"
#define L_RESTORE_NETWORK_CONNECT_TO_HOTSPOT_AFTER_UPGRADE "After 20 seconds connect to WiFi network called as your device name" 
#define L_RESTORE_NETWORK_CONNECT_TO_HOTSPOT_AFTER_UPGRADE_AFE "After 20 seconds connect to WiFi network called <strong>AFE Device</strong>" 
#define L_RESTORE_NETWORK_OPEN_PANEL  "Afterwards open configuration panel : <a href=\"http://192.168.5.1\">http://192.168.5.1</a>"
#define L_UPGRADE_RESTORING_DEFAULT_SETTING "Restoring to default settings"
#define L_UPGRADE_REBOOT_IN_PROGRESS "Reboot in progress"
#define L_UPGRADE_SITE_WILL_BE_RELOADED "Site will be automatically reloaded... wait"
#define L_UPGRADE_REBOOT "Rebooting"
#define L_UPGRADE_UPGRADE_SERVER_NOT_AVAILABLE "<strong>No access to {{n}} upgrade server</strong>"
#define L_UPGRADE_UPGRADE_FIRMWARE_NOT_AVAILABLE "<strong>No {{n}} version available for an upgrade</strong>"

#define L_UPGRADE_TIMEOUT "Upgrade failed. Problem with connecting to the firmware's server (timeout)"
#define L_UPGRADE_CANNOT_CONNECT_TO_SERVER "Upgrade failed. Problem with connecting to the firmware's server"
#define L_UPGRADE_SERVER_NONE_200 "Upgrade failed. Problem with firmware server [Error: Not HTTP 200]"
#define L_UPGRADE_FIRMWARE_SIZE_0 "Upgrade failed. Problem with firmware server [Error: Firmware size: 0kB]"
#define L_UPGRADE_WRONG_CONTENT_TYPE "Upgrade failed. Problem with firmware server [Error: Content type]"
#define L_UPGRADE_SUCCESS_MESSAGE "Firmware upgrade: uploaded %s. Size: %dkB"
#define L_UPGRADE_NOT_FULL_LOADED "Upgrade failed. Written only %dkB out of %dkB"
#define L_UPGRADE_SOMETHING_WRONG "Upgrade failed. Something went wrong [Error code: %d]"
#define L_UPGRADE_NO_SPACE "Upgrade failed. Not enough space to begin upgrade"
#define L_UPGRADE_NO_CONTENT "Upgrade failed. Problem with firmware server [Error: No response]"




/* Form: password */
#define L_PASSWORD_SET_PASSWORD "Set password"
#define L_PASSWORD_SET_PASSWORD_TO_PANEL "Set password to the Configuration Panel"
#define L_PASSWORD_SET_PASSWORD_INFO "If you don't remember the password, reset the device to its default state"
#define L_PASSWORD_PROTECT_BY_PASSWORD "Protect by password"
#define L_PASWORD_PROTECTED "\"Not possible. Configuration is protected by password\""

/* Form: Pro version */
#define L_PRO_VERSION "PRO Version"
#define L_PRO_KEY "Key"
#define L_PRO_VALID "Valid"
#define L_PRO_CANNOT_BE_CONFIGURED  "Version PRO can be configured once the device is connected to the Internet"

/* Form: Contactron */
#define L_CONTACTRON_LED_ASSIGNED_TO_SENSOR "LED assigned to the sensor"
#define L_CONTACTRON_SET_SENSITIVENESS  "Sensitiveness should be adjusted experimentally until sensor behaves as expected"
#define L_CONTACTRON "Magnetic's sensor"
#define L_CONTACTRONS "Magnetic's sensors"
#define L_CONTACTRON_MQTT_TOPIC "MQTT Topic to monitor the contactron"

/* Form: Gate */
#define L_GATE "Gate/Door"
#define L_GATE_IF_MAGNETIC_SENSOR "If magnetic sensor"
#define L_GATE_AND_SENSOR "and sensor"
#define L_GATE_IS_OPEN "is open"
#define L_GATE_ARE_OPEN "are open"
#define L_GATE_THEN "then"
#define L_GATE_IS_CLOSED "is closed"
#define L_GATE_ARE_CLOSED "są zamknięte"
#define L_GATE_STATES_CONFIGURATION "Gate states configuration"
#define L_GATE_SET_STATE "Set gate's state to"
#define L_GATE_OPENED "Opened"
#define L_GATE_CLOSED "Closed"
#define L_GATE_UNKNOWN_STATE "Unknown state"
#define L_GATE_PARTIALLY_OPENED "Partially opened"
#define L_GATE_CONFIGURATION "Gate/Door configuration"
#define L_GATE_RELAY_ID_CONTROLLING_GATE "Gate/Door controlled by Relay"
#define L_GATE_MQTT_TOPIC "MQTT Topic to control the gate/door"
#define L_GATE_IDX_STATE "IDX Gate/Door state"


/* HPMA11BS0 */
#define L_PARTICLE_SENSORS "Particle Sensors"
#define L_HPMA115S0_SENSOR "HPMA115S0"
#define L_HPMA115S0_POST_SLEEP_INTERVAL                                           \
  "If the parameter below is different than 0, the sensor will go to sleep "   \
  "mode between measurements. The setting below defined how many seconds "     \
  "before a measurement the sensor should wake up. It should be lower than "   \
  "measurement's interval"
#define L_HPMA115S0_MEASURE_AFTER "Measure after"
#define L_HPMA115S0_MQTT_TOPIC "HPMA115S0 Sensor's MQTT Topic"
#define L_HPMA115S0_WHO_NORMS "WHO Norms for the average daily particles concentration"
#define L_HPMA115S0_WHO_NORMS_HINT "You can overwrite WHO Norms with your own or applicable in your area"
#define L_HPMA115S0_WHO_NORM_UNIT "average daily concentration in µg/m3 "


/* BMX080 */
#define L_BMEX80_SENSOR "BMEx80 Sensor"
#define L_BMEX80_SENSORS "BMEx80 Sensors"
#define L_BMEX80_IDX_GAS_SENSOR "IDX Gas sensor"
#define L_BMEX80_IDX_IQA "IDX IQA"
#define L_BMEX80_IDX_STATIC_IAQ "IDX Static IQA"
#define L_BMEX80_IDX_CO2_EQUVALENT "IDX CO2 Equivalent"
#define L_BMEX80_IDX_BVOC_EQUIVALENT "IDX BVOC Equivalent"
#define L_BMEX80_MQTT_TOPIC "BMEx80 Sensor's MQTT Topic"
#define L_BMEX80_SENSOR_TYPE "BMEx80 Sensor type"
#define L_BMEX80_REFRESH_SETTINGS "Show/Refresh configuration settings after sensor type change"
#define L_BMEX80_ALTITIDE "Above sea level"
#define L_BMEX80_METERS "Meters"
#define L_BMEX80_IAQ "IAQ"
#define L_BMEX80_STATIC_IAQ "Static IAQ"
#define L_BMEX80_BVOC "BVOC Equivalent"
#define L_BMEX80_CO2 "CO2 Equivalent"
#define L_BMEX80_GAS "Gas Resistance"

/* BH1750 */
#define L_BH1750_SENSOR "BH1750 Sensor"
#define L_BH1750_SENSORS "BH1750 Sensors"
#define L_BH1750_MQTT_TOPIC "BH1750 Sensor's MQTT Topic"
#define L_BH1750_CONTINUES_1_LUX "Continuous reading: ~1 lux 120ms"
#define L_BH1750_CONTINUES_05_LUX "Continuous reading: ~0.5 lux 120ms"
#define L_BH1750_CONTINUES_4_LUX "Continuous reading: 4 lux 16ms"
#define L_BH1750_ONE_READ_1_LUX "One read: 1 lux 120ms"
#define L_BH1750_ONE_READ_05_LUX "One read: 0.5 lux 120ms"
#define L_BH1750_ONE_READ_4_LUX "One read: 4 lux 16ms"


/* TSL2561 */
#define L_TSL2561_SENSOR "TSL2561 Sensor"
#define L_TSL2561_SENSORS "TSL2561 Sensors"
#define L_TSL2561_MQTT_TOPIC "TSL2561 Sensor's MQTT Topic"
#define L_TSL2561_SENSITIVENESS_LOW "Low: fast reading: 14ms"
#define L_TSL2561_SENSITIVENESS_MID "Mid: quite reading read: 100ms"
#define L_TSL2561_SENSITIVENESS_HIGH "High: slow reading: 400ms"
#define L_TSL2561_GAIN_AUTO "Auto"
#define L_TSL2561_GAIN_NONE "1x"
#define L_TSL2561_GAIN_16 "16x"
#define L_TSL2561_GAIN_ILUMINANCE "IDX Illumination"
#define L_TSL2561_GAIN_BROADBAND "IDX Broadband"
#define L_TSL2561_GAIN_IR "IDX Infrared"
#define L_ILUMINANCE "Illuminance"

/* AS3935 */
#define L_AS3935_SENSOR "AS3935 Sensor"
#define L_AS3935_NOISE_FLOOR "Noise floor level"
#define L_AS3935_NOISE_FLOOR_HINT "(0-low, 7-high)"
#define L_AS3935_WATCHDOG_THRESHOLD "Sensitiveness"
#define L_AS3935_WATCHDOG_THRESHOLD_HINT "(1-low, 10-high)"
#define L_AS3935_SPIKES_REJECTION "Spikes rejections level"
#define L_AS3935_SPIKES_REJECTION_HINT "(1-low, 11-high)"
#define L_AS3935_MIN_SPIKES "Min. number of spikes to report the lightning"
#define L_AS3935_AUTOMATIC_NOISE_FLOOR_CONTROL "Inteligent noise control"
#define L_AS3935_SET_LEVEL_OF_NOISE_FLOOR "Set the level of noise if Intelignet noise control option is turned off"
#define L_AS3935_SENSOR_INDOOR_OUTDOOR "Sensor localisation"
#define L_AS3935_INDOOR "Indoor"
#define L_AS3935_OUTDOOR "Outdoor"
#define L_AS3935_MQTT_TOPIC "AS3935 Sensor's MQTT Topic"

/* DS18B20 */
#define L_DS18B20_SENSOR "DS18B20 temperature sensor"
#define L_DS18B20_SENSORS "DS18B20 Sensors"
#define L_DS18B20_SENT_ONLY_CHANGES "Send data only if value of temperature has changed"
#define L_DS18B20_TEMPERATURE_CORRECTION "Temperature correction"
#define L_DS18B20_MQTT_TOPIC "DS18B20 Sensor's MQTT Topic"
#define L_DS18B20_SEARCH "Scan GPIO"
#define L_DS18B20_NO_SENSOR "No sensor found. Check if it is properly connected and then scan the GPIO agai"
#define L_DS18B20_RESOLUTION "Resolution"
#define L_DS18B20_RESOLUTION_9B "Low (9bit)"
#define L_DS18B20_RESOLUTION_10B "Sufficient (10bit)"
#define L_DS18B20_RESOLUTION_11B "Quite good (11bit)"
#define L_DS18B20_RESOLUTION_12B "High (12bit)"


/* DHT */
#ifdef AFE_CONFIG_HARDWARE_DHT
#define L_DHT_SENSORS "Sensor DHT"
#define L_DHT_CONTROLLING_PARAMETER "Controlled by"
#define L_DHT_SENT_ONLY_CHANGES "Send data only if value of temperature or humidity has changed"
#define L_DHT_MQTT_TOPIC "DHT Sensor's MQTT Topic"
#define L_DHT_SENSOR_TYPE "Type"
#define L_DHT_AUTO_DETECT "Detect automatically"
#endif // AFE_CONFIG_HARDWARE_DHT

#if defined(AFE_CONFIG_TEMPERATURE) && defined(AFE_CONFIG_HUMIDITY) 
const char dewPointPerception_Dry[] PROGMEM = "A bit dry for some";
const char dewPointPerception_VeryComfy[] PROGMEM = "Very comfortable";
const char dewPointPerception_Comfy[] PROGMEM = "Comfortable";
const char dewPointPerception_Ok[] PROGMEM = "OK for most, but all perceive the humidity at upper edge";
const char dewPointPerception_UnComfy[] PROGMEM = "Somewhat uncomfortable for most people at upper edge";
const char dewPointPerception_QuiteUnComfy[] PROGMEM = "Very humid, quite uncomfortable";
const char dewPointPerception_VeryUnComfy[] PROGMEM = "Extremely uncomfortable, oppressive";
const char dewPointPerception_SevereUnComfy[] PROGMEM = "Severely high, even deadly for asthma related illnesses";
const char* const dewPointPerception[] PROGMEM = { dewPointPerception_Dry, dewPointPerception_VeryComfy,dewPointPerception_Comfy,dewPointPerception_Ok,dewPointPerception_UnComfy,dewPointPerception_QuiteUnComfy,dewPointPerception_VeryUnComfy,dewPointPerception_SevereUnComfy};

const char comfort_OK[] PROGMEM           = "OK";
const char comfort_TooHot[] PROGMEM       = "Too hot";
const char comfort_TooCold[] PROGMEM      = "Too cold";
const char comfort_TooDry[] PROGMEM       = "Too dry";
const char comfort_HotAndDry[] PROGMEM    = "Hot and dry";
const char comfort_ColdAndDry[] PROGMEM   = "Cold and dry";
const char comfort_Unknown[] PROGMEM      = "?";
const char comfort_TooHumid[] PROGMEM     = "Too humid";
const char comfort_HotAndHumid[] PROGMEM  = "Hot and humid";
const char comfort_ColdAndHumid[] PROGMEM = "Cold and humid";
const char* const Comfort[] PROGMEM = {comfort_OK,comfort_TooHot,comfort_TooCold,comfort_TooDry,comfort_HotAndDry,comfort_ColdAndDry,comfort_Unknown,comfort_TooHumid,comfort_HotAndHumid,comfort_ColdAndHumid};
#endif


/* Anemometer Sensor */
#define L_ANEMOMETER_SENSOR "Anemometer"
#define L_ANEMOMETER_SENSITIVENESS_HINT "Sensitiveness should be adjusted experimentally until sensor behaves as expected. Lower value more sensitive sensor"
#define L_ANEMOMETER_CALIBRATION "Anemometer calibration"
#define L_ANEMOMETER_IMPULSE_DISTANCE_HINT "Wind distans per one anemometer impulse"
#define L_ANEMOMETER_IMPULSE_DISTANCE "Distance"

/* Rainmeter Sensor */
#define L_RAINMETER "Rainmeter"
#define L_RAINMETER_CALIBRATION "Rainmeter resolution"
#define L_RAINMETER_RESOLUTION "Resolution"

/* Regulator */
#define L_REGULATOR_REGULATION "Adjustments"
#define L_REGULATORS "Regulators"
#define L_REGULATOR "Regulator"
#define L_REGULATOR_TURN_ON_IF "On if value is"
#define L_REGULATOR_TURN_OFF_IF "Off is value is"
#define L_REGULATOR_LOWER "lower"
#define L_REGULATOR_HIGHER "higher"
#define L_REGULATOR_ENABLED "Enabled after startup"
#define L_REGULATOR_MQTT_TOPIC "MQTT topic controlling the regulator"

/*  Generic Sensors: Heat Index */
#define L_HEAT_INDEX "Heat Index"
#define L_HEAT_INDEX_IDX "IDX Heat Index"

/*  Generic Sensors: Dew point */
#define L_DEW_POINT "Dew Point"
#define L_DEW_POINT_IDX "IDX Dew Point"

/* Generic Sensors: temperature */
#define L_TEMPERATURE "Temperature"
#define L_TEMPERATURE_IDX "IDX Temperature"

/* Generic Sensors: humidity */
#define L_HUMIDITY "Humidity"
#define L_HUMIDITY_IDX "IDX Humidity"
#define L_HUMIDITY_IDX_TEMP_HUM "IDX Temp/Humidity"
#define L_HUMIDITY_IDX_TEMP_HUM_BAR "IDX Temp/Humi/Bar"

/*  Generic Sensors: Absolute humidity */
#define L_ABSOLOUTE_HUMIDITY "Absolute humidity"
#define L_ABSOLUTE_HUMIDITY_IDX "IDX Absolute humidity"

/*  Generic Sensors: Comfort level */
#define L_COMFORT_LEVEL "Comfort level"
#define L_COMFORT_IDX "IDX Comfort"

/*  Generic Sensors: Perception */
#define L_PERCEPTION "Perception"
#define L_PERCEPTION_IDX "IDX Perception"


/* Generic Sensors: pressure */
#define L_PRESSURE "Pressure"
#define L_PRESSURE_RELATIVE "Relative Pressure"
#define L_PRESSURE_IDX "IDX Pressure"
#define L_PRESSURE_IDX_ALT "Sensor: altitude"
#define L_PRESSURE_IDX_RELATIVE_PRESSURE "IDX Relative pressure"

/* MCP32017 */
#define L_MCP23017_CONNECTION "Connection"
#define L_MCP23017_LED_CONNECTION "LED can be connected directly to ESP's GPIO or through MCP23017 expander"
#define L_MCP23107_LED_ADDITIONAL_SETTINGS "Additional settings"
#define L_MCP23017_RELAY_CONNECTION "Relay can be connected directly to ESP's GPIO or through MCP23017 expander"
#define L_MCP23017_RELAY_TRIGGERED "Relay triggered by"
#define L_MCP23017_SWITCH_CONNECTION "Switch can be connected directly to ESP's GPIO or through MCP23017 expander"
#define L_MCP23017_CONNECTION_VIA_MCP "If connection is made via MCP23017 (GPIO above must be set to NONE)"


/* PN532 */

#define L_PN532_SENSOR "PN532 Sensor"
#define L_PN532_MIFARE_ADMIN "MiFare Classic tools"
#define L_PN532_CARD_FORMAT "Card formatting"
#define L_PN532_CARD_FORMAT_HINT "All information will be permanently removed"
#define L_PN532_FORMAT_NFC "<a href=\"/?o=38&i={{o}}\">Format to NFC</a>"
#define L_PN532_FORMAT_MINIFARE "<a href=\"/?o=38&i={{o}}\">Format to Classic MiFare</a>"
#define L_PN532_READ_TAG "<div class=\"cf\"><label> </label><a class=\"b bc\" href=\"/?o=38&i={{o}}\">Read TAG</a></div>"
#define L_PN532_SAVE_TAG "Saving information on the card"
#define L_PN532_INFO_1 "The card should be formatted to NFC"
#define L_PN532_INFO_2 "Only alpha-numeric characters and basic special characters can be saved on the card"
#define L_PN532_INFO_3 "Place the card next to the reader and DO NOT move it while formatting, reading or writing information"
#define L_PN532_CURRENT_TAG "Current information stored on the card"
#define L_PN532_CURRENT_PRIMARY_TAG "TAG"
#define L_PN532_CURRENT_BACKUP_TAG "TAG: backup copy"
#define L_PN532_TAG_ID "ID"
#define L_PN532_TAG_WHO "User"
#define L_PN532_LED "LED informing about the status of the PN582 sensor"
#define L_PN532_REQUEST_PROCESSING "Request processing timeout"
#define L_PN532_TIMEOUT "Card waiting time"
#define L_PN532_INTERFACE "Interface"
#define L_PN532_LISTENER_TIMEOUT "Card reading interval"
#define L_PN532_SHOW_INTERFACE_CONFIGURATION "Show interface configuration parameters"
#define L_PN532_SEARCH_I2C_CONNECTED_DEVICES "Search for devices on I2C"
#define L_PN532_TIMEOUTS "Readings configuration"
#define L_PN532_MQTT_TOPIC "MQTT topic to send MiFare card information"

#define L_MIFARE_CARD "MiFare Classic Card"
#define L_MIFARE_CARDS "MiFare Classic Cards"
#define L_MIFARE "MiFare"
#define L_MIFARE_CARD_ID "Card ID"
#define L_MIFARE_CARD_CONTROLS_RELAY_GATE "Relay / gate control"
#define L_MIFARE_CARD_CONTROLS "Controls"
#define L_MIFARE_CARD_ACTION "Action"
#define L_MIFARE_CARD_ACTION_ON "ON"
#define L_MIFARE_CARD_ACTION_OFF "OFF"
#define L_MIFARE_CARD_ACTION_TOGGLE "Toggle"
#define L_MIFARE_CARD_INTEGRATION "Integration"
#define L_MIFARE_CARD_INTEGRATION_HINT "Integration configuration with the automation system"
#define L_MIFARE_CARD_SEND_AS_SWITCH "Send card detection using: ON/OFF values"
#define L_MIFARE_CARD_SEND_AS_SWITCH_HINT "Default: OPEN/CLOSED"
#define L_MIFARE_CARD_MQTT_TOPIC "MQTT Topic to monitor authorizations of MiFare card"
#define L_MIFARE_CARD_HOW_LONG_KEEP_STATE "How long CLOSED (ON) state should be kept after detecting a card"
#define L_MIFARE_CARD_TIME "Time"
#define L_MIFARE_CARD_NONE_PRO "The standard version of {{n}} supports up to 4 MiFare Classic cards.<br>The AFE Pro version supports up to 10 cards.<br>More information about <a href=\"https://afe.smartnydom.pl/en/postawowe-informacje/wersja-pro\"> AFE Pro </a>"
#define L_MIFARE_CARD_BACKUP_TAG "Backup copy of the TAG stored on the card"


#define L_MIFARE_ADMIN_INFO "Information"


/* CLED */

#define L_CLED_STRIP "LED Strip"
#define L_CLED_BRIGHTNESS "Brightness"
#define L_CLED_COLOR_RGB "Color RGB"

#define L_CLED_CONFIGURATION "RGB LEDs configuration"
#define L_CLED_ONOFF_CONFIGURATION "LEDs: ON/OFF configuration"
#define L_CLED_ONOFF_CONFIGURATION_ON "LED's turned ON settings"
#define L_CLED_ONOFF_CONFIGURATION_OFF "LED's turned OFF settings"
#define L_CLED_EFFECTS_CONFIGURATION "Effects' configurations"
#define L_CLED_EFFECTS_URL "<a href=\"/?o={{u.u}}&i={{u.i}}\">{{u.l}}</a>"
#define L_CLED_TIMEOUT "How long keep this state"

#define L_CLED_EFFECT_BLINKING_CONFIGURATION "Blinking effect"
#define L_CLED_EFFECT_FADE_IN_OUT_CONFIGURATION "Fade In/Out effect"
#define L_CLED_EFFECT_WAVE_CONFIGURATION "Wave effect"

#define L_CLED_EFFECT_WAVE_ON_LED_COLOR "Active LED RGB color"
#define L_CLED_EFFECT_WAVE_OFF_LED_COLOR "Inactive LED RGB color"
#define L_CLED_EFFECT_WAVE_SPEED "Wave speed"

#define L_CLED_MAX_BRIGHTNESS "Max brightness"
#define L_CLED_MIN_BRIGHTNESS "Min brightness"
#define L_CLED_FADE_IN_OUT_TIMEOUT "Fade speed"

#define L_CLED_ON_OFF_IDX "On/Off IDX"
#define L_CLED_EFFECTS_IDX "Effect IDX"
#define L_CLED_MQTT_TOPIC "MQTT Topic controlling RGB LED"

#define L_CLED_MQTT_ON_OFF_TOPIC "On/Off Topic"
#define L_CLED_MQTT_EFFECTS_TOPIC "Effect's Topic"

#define L_CLEDS_HINT "This firmware version doesn't enable to change the GPIO parameter of the WS281x RGB LED strip. It must be connected as described below"
#define L_CLED_NUMBER_OF_LEDS "Number of leds in the strip"

#define L_CLED_API_BRIGHTNESS_CONVERSION "Brightness values"
#define L_CLED_API_BRIGHTNESS_CONVERSION_0_255 "0 .. 255"
#define L_CLED_API_BRIGHTNESS_CONVERSION_0_100 "0..100"
#define L_CLED_API_BRIGHTNESS_CONVERSION_0_1 "0..1"

#define L_CLED_EFFECT_OFF_COMMAND "Off"
#define L_CLED_EFFECT_LABEL "Light effects"

//#define L_CLED_EFFECT_DEVICE_LIGHT "LED Effect: Device backlight"
//#define L_CLED_EFFECT_PN532_SENSOR "LED Effect: PN532 sensor state"
//#define L_CLEDS "RGB Leds"
////
//#define L_CLED_COLOR_RGBS_ORDER "Colors order"
//#define L_CLED_EFFECT_FADE_IN_OUT "Fade In/Out effect"

//#define L_CLED_TIME_WAVE "Wave speed"

/*
#define L_CLED_LIGHT_SENSOR "Light sensor"
#define L_CLED_LIGHT_LEVEL "Light level"
#define L_CLED_BACKLIGHT_CONFIG "Backlight configuration"
#define L_CLED_RULE "Rule"
#define L_CLED_RULE_HINT "The backlight rule is triggered if the light level is below the sensor's reading. The first condition met stops processing following rules"

#define L_CLED_LIGHT_EFFECTS "Light effects"
#define L_CLED_DEVICE_BACKLIGHT "Device's backlight"
#define L_CLED_PN532_EFFECTS "PN532 sensor's light effects"
*/

/* Home Assistant */
#define L_HA_DISCOVERY_TOPIC "Home Assistant discovery topic"
#define L_HA_PUBLISHING "Publish/Update configuration?"
#define L_HA_UPDATE_REMOVE "Remove old configuration?"
#define L_HA_RETAIN_CONFIGURATION "Retain configuraion in MQTT Broker"

/* Firmware version and details */
#define L_FIRMWAR_YOUR_CURRENT_FIRMWARE "Your current firmware"
#define L_FIRMWARE_TYPE "AFE T{{f.t}}"
#define L_FIRMWARE_VERSION "Version {{f.v}} (Build:{{f.b}})"
#define L_FIRMWARE_VERSION_DATE "Compilation date: {{f.k}}"
#define L_ESP_CHIP "Chip: ESP{{f.e}}"
#define L_ESP_FLASH_SIZE "Flash: {{f.s}} formatted to: {{f.f}}"
#define L_FIRMWARE_API "API: {{f.a}}"
#define L_FIRMWARE_DEVICE_NAME "Device type: {{f.d}}"
#define L_FIRMWARE_DEVICE_ID "Device ID: {{f.n}}"
#define L_FIRMWARE_PRO_YES "Pro version: Yes"
#define L_FIRMWARE_PRO_NO "Pro version: No"
#define L_REBOOTS_NUMBER "Firmware restarted {{x}} times"
#define L_ADDITIONAL_INFORMATION "Additional information"
#define L_WIFI_RSSI "WiFi Signal Strength Indicator {{x}} dBm ({{t}})"
#define L_WIFI_SIGNAL "Signal"
#define L_WIFI_RSSI_30 "Amazing"
#define L_WIFI_RSSI_67 "Very Good"
#define L_WIFI_RSSI_70 "Okay"
#define L_WIFI_RSSI_80 "Not Good"
#define L_WIFI_RSSI_90 "Unusable"
#endif // _LANG_EN_h

