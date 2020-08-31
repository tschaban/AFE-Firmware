/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _LANG_EN_h
#define _LANG_EN_h

#define L_LANGUAGE "en-EN"
#define L_LANGUAGE_SHORT "en"

/* Upgrades */
#define L_UPGRADED_TO_NEW_VERSION                                              \
  "Firmware has been upgraded to a newer version"
#define L_UPGRADED_TO_NEW_VERSION_TYPE                                         \
  "Firmware has been upgraded to a new type. Device reconfiguration is "       \
  "required."

/* Firmware names */
#if defined(AFE_DEVICE_SHELLY_1)
#define L_FIRMWARE_NAME "for Shelly-1"
#elif defined(AFE_DEVICE_SONOFF_BASIC_V1)
#define L_FIRMWARE_NAME "for Sonoff Basic"
#elif defined(T0_CONFIG)
#define L_FIRMWARE_NAME "WIFI SWITCH"
#elif defined(T1_CONFIG)
#define L_FIRMWARE_NAME "Switch with temperature sensor"
#elif defined(T2_CONFIG)
#define L_FIRMWARE_NAME "Switch with temperature and humidity sensor"
#elif defined(T3_CONFIG)
#define L_FIRMWARE_NAME "for 4 WiFi switches and PIR sensors"
#elif defined(T4_CONFIG)
#define L_FIRMWARE_NAME "for 4 WiFi switches"
#elif defined(T5_CONFIG)
#define L_FIRMWARE_NAME "to control gate controller"
#elif defined(T6_CONFIG)
#define L_FIRMWARE_NAME "Weather Station"
#endif

/* HTTP API */
#define L_DEVICE_NOT_EXIST "\"Device does not exist\""
#define L_COMMAND_NOT_IMPLEMENTED "\"Command is not implemented\""
#define L_PASWORD_PROTECTED "\"Not possible. Configuration is protected by password\""

#define L_DONATE_LINK "https://en.donate.afe-firmware.smartnydom.pl/"
#define L_DONATE "&#128077; Donate if you found this useful"

/* Menu */
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
#define L_FINISH_CONFIGURATION "Finish configuration"
#define L_INFORMATION "INFORMATION"
#define L_DOCUMENTATION "Documentation"
#define L_SETTINGS "Settings"
#define L_SETTINGS_HOT_SPOT "Settings (HotSpot mode)"
#define L_PRO_VERSION "PRO Version"
#define L_CONNECTED "Connected"
#define L_DISCONNECTED "Disconnected"

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

/* Form: index */
#define L_WRONG_PASSWORD "Incorrect password"
#define L_NORMAL_MODE "Normal mode"
#define L_HOTSPOT_MODE "HotSpot Mode"
#define L_LAUNCH_CONFIGURATION_PANEL "Launch: Configuration Panel"
#define L_OPENING_CONFIG_PANEL "Opening AFE Firmware configuration panel"

/* Form: device coniguration */
#define L_DEVICE_NAME "Device name"
#define L_DEVICE_SECTION_INFO                                                  \
  "Name is used for device identification within your LAN and a device's "     \
  "hotspot name used for configuration mode"
#define L_HARDWARE_CONFIGURATION "Hardware configuration"
#define L_HARDWARE_CONFIGURATION_INFO "Select connected items to your device"
#ifdef AFE_CONFIG_HARDWARE_LED
#define L_NUMBER_OF_LEDS "Number of LEDs"
#endif
#define L_NUMBER_OF_RELAYS "Number of relays"
#define L_NUMBER_OF_SWITCHES "Number of switches"
#define L_DO_MEASURE_ADC "Measurements from Analog Input"
#define L_DEVICE_CONTROLLING "Device controlling mechanism"
#define L_DEVICE_CONTROLLING_INFO "Enable / Disable APIs"
#ifdef AFE_CONFIG_HARDWARE_GATE
#define L_CONTROLLED_GATES "Gates/Doors controlled"
#endif
#define L_AUTOLOGOUT_TITLE "Automatic logout form the configuration panel"
#define L_AUTOLOGOUT_DESCRIPTION "Device configuration will be automatically closed after 10min. of idle time in the configuration panel"



/* Form: network configuration */
#define L_NETWORK_CONFIGURATION "WiFi Configuration"
#define L_NETWORK_CONFIGURATION_INFO                                           \
  "Device without connection to network will only work in manual mode"
#define L_SSID "WiFi name"
#define L_REFRESH "Refresh"
#define L_DHCP_ENABLED "Configuration over DHCP"
#define L_GATEWAY "Gateway"
#define L_SUBNET "Subnet"
#define L_DEVICE_IP "Device IP address"
#define L_DEVICE_IP_INFO                                                       \
  "You can set up device's IP address manually or automatically by DHCP"
#define L_NUMBER_OF_CONNECTIONS "Number of connection attempts"
#define L_TIME_BETWEEN_CONNECTIONS "Time between connections' attempts"
#define L_SLEEP_TIME "Sleep time before next connection attempts"
#define L_ADVANCED "Advanced"
#define L_DEVICE_CONNECTS "Device is connecting to"
#define L_CONNECT_TO "Connect to"
#define L_CONNECT "Connect"
#define L_SEARCH_FOR_IP_ADDRESS                                                \
  "In your WiFi router look for an IP address of a device with follwing MAC "  \
  "address"
#define L_FINISH_NETWORK_CONFIGURATION                                         \
  "Finish configuration by entering device IP address in the browser"

/* Form: MQTT Broker */
#define L_MQTT_TOPIC "Topic"
#define L_MQTT_IDX_LWT "LWT's IDX"
#define L_MQTT_TOPIC_LWT "LWT's Topic"
#define L_MQTT_CONFIGURATION_INFO "Enter MQTT Broker hostname or its IP address"
#define L_MQTT_TOPIC_EMPTY "If left empty, message won't be sent to MQTT Broker"
#define L_TIMEOUT "Timeout"

/* Form: Domoticz server configuration */
#define L_PROTOCOL "Protocol"
#define L_DOMOTICZ_CONFIGURATION "Domoticz Server"
#define L_DOMOTICZ_CONFIGURATION_INFO                                          \
  "Enter Domoticz hostname or its IP address"
#define L_NO_IF_IDX_0                                                          \
  "If IDX is set to 0 then a value won't be sent to Domoticz"

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
#define L_DOMOTICZ_VERSION_410 "4.10x or older"
#define L_DOMOTICZ_VERSION_2020 "2020.x or newer"
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
/* Form: Led configuration */
#define L_CHANGE_LED_INDICATION "Change LED ligtning to opposite"

/* Form: System LED configuration */
#define L_SYSTEM_LED "System LED"
#define L_SYSTEM_LED_INFO                                                      \
  "Select LED which will be informing about device status and its events"
#endif

/* Form: relay */
#define L_DEFAULT_VALUES "Default values"
#define L_DEFAULT_POWER_RESTORED "When power is restored set it to"
#define L_NO_ACTION "No action"
#define L_ON "On"
#define L_OFF "Off"
#define L_LAST_KNOWN_STATE "Last known state"
#define L_OPPOSITE_TO_LAST_KNOWN_STATE "Opposite to the last known state"
#define L_DEFAULT_MQTT_CONNECTED "After establishing connection to MQTT Broker"
#define L_DEFAULT_GET_FROM_MQTT                                                \
  "Get state from a relay's controlling system (over MQTT)"
#define L_AUTOMATIC_SWITCHING_OFF "Automatic switching off of the relay"
#define L_IMPULSE_DURATION "Impulse duration"
#define L_SWITCH_OFF_AFTER "Switch off after"
#define L_NO_ACTION_IF_0 "No action if it's set to 0"
#define L_THERMAL_PROTECTION "Thermal protection"
#define L_SWITCH_OFF_ABOVE "Switch off above"
#define L_RANGE "Range"
#define L_SELECT_LED_4_RELAY "Select LED informing about relay state"
#define L_RELAY_MQTT_TOPIC "MQTT Topic to control the relay"

/* Form: Switch */
#define L_FUNCTIONALITY "Functionality"
#define L_SYSTEM_BUTTON "System button"
#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY
#define L_CONTROL_RELAY "Controlling only the relay"
#endif
#ifdef AFE_CONFIG_FUNCTIONALITY_GATE
#define L_CONTROL_GATE "Controlling only the gate"
#endif
#define L_RELAY_CONTROLLED_BY_SWITCH "Controlling"
#define L_TYPE "Type"
#define L_MONOSTABLE "Monostable"
#define L_BISTABLE "Bistable"
#define L_SENSITIVENESS "Sensitiveness"
#define L_SENSITIVENESS_HINT                                                  \
  "Sensitiveness should be adjusted if switch didn't behave as expected "      \
  "while pressing it"
#define L_SWITCH_BUTTON "Switch / Button"
#define L_SWITCH_MQTT_TOPIC "MQTT Topic to monitor the state of the switch"

/* Form: Analog Input */
#define L_CANT_CHANGE "can't be set"
#define L_MEASURMENTS_INTERVAL "Measurement's interval"
#define L_MEASURED_VOLTAGE "Max input voltage on ADC"
#define L_NUMBER_OF_SAMPLES "Number of measurement samples"
#define L_RAW_DATA "IDX Raw data"
#define L_PERCENT "IDX Percent"
#define L_VOLTAGE "IDX voltage at input"
#define L_VOLTAGE_CALCULATED "IDX voltage calculated"
#define L_RESISTOR "Resistor"
#define L_VOLTAGE_DIVIDER "Voltage Divider"
#define L_ADC_MQTT_TOPIC "MQTT Topic: ADC"

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
#define L_BATTERY_METER "Battery level"
#define L_BATTERY_MQTT_TOPIC "MQTT Topic: battery level"
#define L_BATTERY_MIN_LEVEL "Battery level: min"
#define L_BATTERY_MAX_LEVEL "Battery level: max"
#endif

/* Form: upgrade and post upgrade */
#define L_SELECT_FIRMWARE "Select Firmware"
#define L_UPGRADE_INFO                                                         \
  "Device will be automatically rebooted after "                               \
  "upgrade<br><br><strong>Warning</strong>: after upgrade do not plug off "    \
  "the device from power source for around a minute.<br>Device's memory will " \
  "be formatted and default settings will be uploaded.<br><strong>This "       \
  "process cannot be interrupted</strong>"
#define L_UPGRADE "Upgrade"
#define L_DONT_PLUG_OFF                                                        \
  "<strong>Warning</strong>: don't plug off the device from a power source "   \
  "while upgrading"
#define L_UPGRADE_FAILED "Upgrade failed"
#define L_UPGRADE_SUCCESSFUL "Upgrade finished succesfully"
#define L_DEVICE_WILL_BE_REBOOTED                                              \
  "After 15s the device will be rebooted. Please wait"
#define L_RESTORE_DEFAULT_SETTINGS "Restore default settings"
#define L_WARNING "Warning"
#define L_CONFIGURATION_WILL_BE_REMOVED                                        \
  "restoring to default settings will remove all configuration's "             \
  "information, incl. WiFi configuration"
#define L_UPGRADE_IN_PROGRESS "Restoring configuration is in progress"
#define L_CONNECT_TO_HOTSPOT_AFTER_UPGRADE                                     \
  "After 20 seconds connect to WiFi network called "                           \
  "<strong>AFE-Device</strong> and open configuration panel"
#define L_RESTORING_DEFAULT_SETTING "Restoring to default settings"
#define L_REBOOT_IN_PROGRESS "Reboot in progress"
#define L_SITE_WILL_BE_RELOADED "Site will be automatically reloaded... wait"
#define L_REBOOT "Device reboot"

/* Form: password */
#define L_SET_PASSWORD "Set password"
#define L_SET_PASSWORD_TO_PANEL "Set password to the Configuration Panel"
#define L_SET_PASSWORD_INFO                                                    \
  "If you don't remember the password, reset the device to its default state"
#define L_PROTECT_BY_PASSWORD "Protect by password"

/* Form: Pro version */
#define L_PRO_VERSION "PRO Version"
#define L_KEY "Key"
#define L_VALID "Valid"
#define L_PRO_CANNOT_BE_COFIGURED                                              \
  "Version PRO can be configured once the device is connected to the Internet"

/* Form: Contactron */
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
#define L_LED_ASSIGNED_TO_SENSOR "LED assigned to the sensor"
#define L_SET_CONTACTRON_SENSITIVENESS                                         \
  "Sensitiveness should be adjusted experimentally until sensor behaves as "   \
  "expected"
#define L_MAGNETIC_SENSOR "Magnetic's sensor"
#define L_MAGNETIC_SENSORS "Magnetic's sensors"
#define L_NUMBER_OF_MAGNETIC_SENSORS "Number of magnetic's sensors"
#define L_CONTACTRON_MQTT_TOPIC "MQTT Topic to monitor the contactron"

#endif

/* Form: Gate */
#ifdef AFE_CONFIG_HARDWARE_GATE
#define L_GATE "Gate/Door"
#define L_IF_MAGNETIC_SENSOR "If magnetic sensor"
#define L_AND_SENSOR "and sensor"
#define L_IS_OPEN "is open"
#define L_ARE_OPEN "are open"
#define L_THEN "then"
#define L_IS_CLOSED "is closed"
#define L_ARE_CLOSED "są zamknięte"
#define L_GATES_STATES_CONFIGURATION "Gate states configuration"
#define L_SET_GATE_STATE "Set gate's state to"
#define L_OPENED "Opened"
#define L_CLOSED "Closed"
#define L_UNKNOWN_STATE "Unknown state"
#define L_PARTIALLY_OPENED "Partially opened"
#define L_GATE_CONFIGURATION "Gate/Door configuration"
#define L_NUMBER_OF_CONTROLLED_GATES "Number of controlled gates/doors"
#define L_RELAY_ID_CONTROLLING_GATE "Gate/Door controlled by Relay"
#define L_GATE_MQTT_TOPIC "MQTT Topic to control the gate/door"
#define L_IDX_GATE_STATE "IDX Gate/Door state"
#endif

/* HPMA11BS0 */
#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
#define L_PARTICLE_SENSOR "PM2.5/PM10 Particle Sensor"
#define L_NUMBER_OF_HPMA115S0_SENSORS "HPMA115S0 Sensor"
#define L_SENSOR_POST_SLEEP_INTERVAL                                           \
  "If the parameter below is different than 0, the sensor will go to sleep "   \
  "mode between measurements. The setting below defined how many seconds "     \
  "before a measurement the sensor should wake up. It should be lower than "   \
  "measurement's interval"
#define L_MEASURE_AFTER "Measure after"
#define L_MQTT_TOPIC_HPMA115S0 "HPMA115S0 Sensor's MQTT Topic"
#endif

/* BMX080 */
#ifdef AFE_CONFIG_HARDWARE_BMEX80
#define L_BMEX80_SENSOR "BMEx80 Sensor"
#define L_BMEX80_SENSORS "BMEx80 Sensors"
#define L_NUMBER_OF_BMEX80_SENSORS "Number of BMEx80 Sensors"
#define L_IDX_GAS_SENSOR "IDX Gas sensor"
#define L_IDX_IQA "IDX IQA"
#define L_IDX_STATIC_IAQ "IDX Static IQA"
#define L_IDX_CO2_EQUVALENT "IDX CO2 Equivalent"
#define L_IDX_BVOC_EQUIVALENT "IDX BVOC Equivalent"
#define L_MQTT_TOPIC_BMEX80 "BMEx80 Sensor's MQTT Topic"
#define L_BMEX80_SENSOR_TYPE "BMEx80 Sensor type"
#define L_REFRESH_SETTINGS_FOR_BMEX80_SENSOR "Show/Refresh configuration settings after sensor type change"
#define L_ALTITIDE "Above sea level"
#define L_METERS "Meters"
#endif

/* BH1750 */
#ifdef AFE_CONFIG_HARDWARE_BH1750
#define L_BH1750_SENSOR "BH1750 Sensor"
#define L_BH1750_SENSORS "BH1750 Sensors"
#define L_NUMBER_OF_BH1750_SENSORS "Number of BH1750 Sensors"
#define L_MQTT_TOPIC_BH1750 "BH1750 Sensor's MQTT Topic"
#endif

/* AS3935 */
#ifdef AFE_CONFIG_HARDWARE_AS3935
#define L_NUMBER_OF_AS3935_SENSORS "Number of AS3935 Sensors"
#define L_AS3935_SENSOR "AS3935 Sensor"
#define L_NOISE_FLOOR "Noise floor level"
#define L_NOISE_FLOOR_HINT "(0-low, 7-high)"
#define L_WATCHDOG_THRESHOLD "Sensitiveness"
#define L_WATCHDOG_THRESHOLD_HINT "(1-low, 10-high)"
#define L_SPIKES_REJECTION "Spikes rejections level"
#define L_SPIKES_REJECTION_HINT "(1-low, 11-high)"
#define L_MIN_SPIKES "Min. number of spikes to report the lightning"
#define L_AUTOMATIC_NOISE_FLOOR_CONTROL "Inteligent noise control"
#define L_SET_LEVEL_OF_NOISE_FLOOR "Set the level of noise if Intelignet noise control option is turned off"
#define L_SENSOR_INDOOR_OUTDOOR "Sensor localisation"
#define L_INDOOR "Indoor"
#define L_OUTDOOR "Outdoor"
#define L_MQTT_TOPIC_AS3935 "AS3935 Sensor's MQTT Topic"
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
#define L_DS18B20_SENSOR "DS18B20 temperature sensor"
#define L_DS18B20_SENSORS "DS18B20 Sensors"
#define L_NUMBER_OF_DS18B20_SENSORS "Number of DS18B20 sensors"
#define L_DS18B20_SENT_ONLY_CHANGES "Send data only if value of temperature has changed"
#define L_DS18B20_TEMPERATURE_CORRECTION "Temperature correction"
#endif

/* I2C or DS18B20 */
#if defined(AFE_CONFIG_HARDWARE_I2C) || defined(AFE_CONFIG_HARDWARE_DS18B20)
#define L_ADDRESS "Address"
#endif

/* Anemometer Sensor */
#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER_SENSOR
#define L_ANEMOMETER_SENSOR "Anemometer speed sensor"
#define L_ANEMOMETER_SENSITIVENESS_HINT "Sensitiveness should be adjusted experimentally until sensor behaves as "   \
  "expected. Lower value more sensitive sensor"
#define L_ANEMOMETER_CALIBRATION "Anemometer calibration"
#define L_ANEMOMETER_IMPULSE_DISTANCE_HINT "Wind distans per one anemometer impulse"
#define L_ANEMOMETER_IMPULSE_DISTANCE "Distance"
#endif

/* Rainmeter Sensor */
#ifdef AFE_CONFIG_HARDWARE_RAINMETER_SENSOR
#define L_RAINMETER "Rainmeter"
#define L_RAINMETER_CALIBRATION "Rainmeter resolution"
#define L_RAINMETER_RESOLUTION "Resolution"
#endif

/* Generic Sensors */
#ifdef AFE_CONFIG_TEMPERATURE
#define L_TEMPERATURE "Temperature"
#define L_IDX_TEMPERATURE "IDX Temperature"
#ifndef L_CORRECTIONS
#define L_CORRECTIONS "Corrections"
#endif
#ifndef L_UNITS
#define L_UNITS "Units"
#endif
#endif

#ifdef AFE_CONFIG_HUMIDITY
#define L_HUMIDITY "Humidity"
#define L_IDX_HUMIDITY "IDX Humidity"
#ifdef AFE_CONFIG_TEMPERATURE
#define L_IDX_DEW_POINT "IDX Dew Point"
#define L_IDX_HEAT_INDEX "IDX Heat Index"
#define L_IDX_TEMP_HUM "IDX Temp/Humidity"
#ifdef AFE_CONFIG_PRESSURE
#define L_IDX_TEMP_HUM_BAR "IDX Temp/Humi/Bar"
#endif
#endif
#ifndef L_CORRECTIONS
#define L_CORRECTIONS "Corrections"
#endif
#ifndef L_UNITS
#define L_UNITS "Units"
#endif
#endif

#ifdef AFE_CONFIG_PRESSURE
#define L_PRESSURE "Pressure"
#define L_IDX_PRESSURE "IDX Pressure"
#define L_IDX_ALT "Sensor: altitude"
#ifdef AFE_CONFIG_TEMPERATURE
#define L_IDX_RELATIVE_PRESSURE "IDX Relative pressure"
#endif
#ifndef L_CORRECTIONS
#define L_CORRECTIONS "Corrections"
#endif
#ifndef L_UNITS
#define L_UNITS "Units"
#endif
#endif

#endif
