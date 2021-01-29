/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _LANG_EN_h
#define _LANG_EN_h

#define L_LANGUAGE "en-EN"
#define L_LANGUAGE_SHORT "en"

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

/* Upgrades */
#define L_UPGRADED_TO_NEW_VERSION  "Firmware has been upgraded to a newer version"
#define L_UPGRADED_TO_NEW_VERSION_TYPE "Firmware has been upgraded to a new type. Device reconfiguration is required."

#define L_DONATE "&#128077; Like it? Consider supporting hundreds of hours I've spent on developing it so far. thx"

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
#define L_INTERFACE "Interfaces"


/* Forms commons */
#define L_NUMBER_OF_CHARS "chars"
#define L_ENABLED "enabled"
#define L_USERNAME "User name"
#define L_PASSWORD "Password"
#define L_SECONDS "seconds"
#define L_MILISECONDS "milisekonds"
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

/* Form: index */
#define L_INDEX_WRONG_PASSWORD "Incorrect password"
#define L_INDEX_NORMAL_MODE "Normal mode"
#define L_INDEX_HOTSPOT_MODE "HotSpot Mode"
#define L_INDEX_LAUNCH_CONFIGURATION_PANEL "Device is in operations mode"
#define L_INDEX_LAUNCH_CONFIGURATION_PANEL_HINT "To open configuration panel press one of the buttons."
#define L_INDEX_OPENING_CONFIG_PANEL "Opening AFE Firmware configuration panel"

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

#define L_DEVICE_DO_MEASURE_ADC "Measurements from Analog Input"
#define L_DEVICE_CONTROLLING "Device controlling mechanism"
#define L_DEVICE_CONTROLLING_INFO "Enable / Disable APIs"
#define L_DEVICE_CONTROLLED_GATES "Gates/Doors controlled"
#define L_DEVICE_AUTOLOGOUT_TITLE "Automatic logout form the configuration panel"
#define L_DEVICE_AUTOLOGOUT_DESCRIPTION "Device configuration will be automatically closed after 10min. of idle time in the configuration panel"
#define L_DEVICE_DOMOTICZ_VERSION_410 "4.10x or older"
#define L_DEVICE_DOMOTICZ_VERSION_2020 "2020.x or newer"
#define L_DEVICE_ADDITIONAL_FUNCTIONALITIES "Additional functionalities"


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
#define L_NETWORK_NUMBER_OF_CONNECTIONS "Number of connection attempts"
#define L_NETWORK_TIME_BETWEEN_CONNECTIONS "Time between connections' attempts"
#define L_NETWORK_SLEEP_TIME "Sleep time before next connection attempts"
#define L_NETWORK_ADVANCED "Advanced"
#define L_NETWORK_DEVICE_CONNECTS "Device is connecting to"
#define L_NETWORK_CONNECT_TO "Connect to"
#define L_NETWORK_CONNECT "Connect"
#define L_NETWORK_SEARCH_FOR_IP_ADDRESS "In your WiFi router look for an IP address of a device with follwing MAC address"
#define L_NETWORK_FINISH_NETWORK_CONFIGURATION "Finish configuration by entering device IP address in the browser"

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

#define L_BATTERY_METER "Battery level"
#define L_BATTERY_MQTT_TOPIC "MQTT Topic: battery level"
#define L_BATTERY_MIN_LEVEL "Battery level: min"
#define L_BATTERY_MAX_LEVEL "Battery level: max"

/* Form: upgrade and post upgrade */
#define L_UPGRADE_SELECT_FIRMWARE "Select Firmware"
#define L_UPGRADE_INFO                                                         \
  "Device will be automatically rebooted after "                               \
  "upgrade<br><br><strong>Warning</strong>: after upgrade do not plug off "    \
  "the device from power source for around a minute.<br>Device configuration requires an update<br><strong>This "       \
  "process cannot be interrupted</strong>"
#define L_UPGRADE "Upgrade"
#define L_UPGRADE_DONT_PLUG_OFF                                                        \
  "<strong>Warning</strong>: don't plug off the device from a power source "   \
  "while upgrading"
#define L_UPGRADE_FAILED "Upgrade failed"
#define L_UPGRADE_SUCCESSFUL "Upgrade finished succesfully"
#define L_UPGRADE_DEVICE_WILL_BE_REBOOTED                                              \
  "After 15s the device will be rebooted. Please wait"
#define L_UPGRADE_RESTORE_DEFAULT_SETTINGS "Restore default settings"
#define L_UPGRADE_WARNING "Warning"
#define L_UPGRADE_CONFIGURATION_WILL_BE_REMOVED                                        \
  "restoring to default settings will remove all configuration's "             \
  "information, incl. WiFi configuration"
#define L_UPGRADE_IN_PROGRESS "Restoring configuration is in progress"
#define L_UPGRADE_NETWORK_CONNECT_TO_HOTSPOT_AFTER_UPGRADE                                     \
  "After 20 seconds connect to WiFi network called "                           \
  "<strong>AFE-Device</strong> and open configuration panel"
#define L_UPGRADE_RESTORING_DEFAULT_SETTING "Restoring to default settings"
#define L_UPGRADE_REBOOT_IN_PROGRESS "Reboot in progress"
#define L_UPGRADE_SITE_WILL_BE_RELOADED "Site will be automatically reloaded... wait"
#define L_UPGRADE_REBOOT "Rebooting"

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
#define L_HPMA115S0_SENSOR "PM2.5/PM10 Particle Sensor"
#define L_HPMA115S0_POST_SLEEP_INTERVAL                                           \
  "If the parameter below is different than 0, the sensor will go to sleep "   \
  "mode between measurements. The setting below defined how many seconds "     \
  "before a measurement the sensor should wake up. It should be lower than "   \
  "measurement's interval"
#define L_HPMA115S0_MEASURE_AFTER "Measure after"
#define L_HPMA115S0_MQTT_TOPIC "HPMA115S0 Sensor's MQTT Topic"


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

/* BH1750 */
#define L_BH1750_SENSOR "BH1750 Sensor"
#define L_BH1750_SENSORS "BH1750 Sensors"
#define L_BH1750_MQTT_TOPIC "BH1750 Sensor's MQTT Topic"

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
#endif // AFE_CONFIG_HARDWARE_DHT

/* Anemometer Sensor */
#define L_ANEMOMETER_SENSOR "Anemometer speed sensor"
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

#define L_REGULATOR_CP_TEMPERATURE "Temperature"
#define L_REGULATOR_CP_HEAT_INDEX "Heat Index"
#define L_REGULATOR_CP_HUMIDITY "Humidity"
#define L_REGULATOR_CP_ABSOLOUTE_HUMIDITY "Absolute humidity"
#define L_REGULATOR_CP_DEW_POINT "Dew Point"

/* Generic Sensors: temperature */
#define L_TEMPERATURE "Temperature"
#define L_TEMPERATURE_IDX "IDX Temperature"
#define L_CORRECTIONS "Corrections"
#define L_UNITS "Units"

/* Generic Sensors: humidity */
#define L_HUMIDITY "Humidity"
#define L_HUMIDITY_IDX "IDX Humidity"
#define L_HUMIDITY_IDX_TEMP_HUM "IDX Temp/Humidity"
#define L_HUMIDITY_IDX_TEMP_HUM_BAR "IDX Temp/Humi/Bar"
#define L_HUMIDITY_IDX_ABSOLUTE "IDX Absolute humidity"
#define L_HUMIDITY_IDX_DEW_POINT "IDX Dew Point"
#define L_HUMIDITY_IDX_HEAT_INDEX "IDX Heat Index"

#define L_COMFORT_IDX "IDX Comfort"
#define L_PERCEPTION_IDX "IDX Perception"


/* Generic Sensors: pressure */
#define L_PRESSURE "Pressure"
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

#endif // _LANG_EN_h

