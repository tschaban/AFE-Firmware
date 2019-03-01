#ifndef _LANG_EN_h
#define _LANG_EN_h

#define L_LANGUAGE "en-EN"
#define L_LANGUAGE_SHORT "en"

/* Firmware names */
#ifdef T0_CONFIG
#define L_FIRMWARE_NAME "WiFi Switch"
#elif T0_SHELLY_1_CONFIG
#define L_FIRMWARE_NAME "for Shelly-1"
#elif T1_CONFIG
#define L_FIRMWARE_NAME "Switch with temperature sensor"
#elif T2_CONFIG
#define L_FIRMWARE_NAME "Switch with temperature and humidity sensor"
#elif T3_CONFIG
#define L_FIRMWARE_NAME "for 4 WiFi switches and PIR sensors"
#elif T4_CONFIG
#define L_FIRMWARE_NAME "for 4 WiFi switches"
#elif T5_CONFIG
#define L_FIRMWARE_NAME "to control gate controller"
#elif T6_CONFIG
#define L_FIRMWARE_NAME "Wheater Station"
#endif

/* Menu */
#define L_DEVICE "Device"
#define L_NETWORK "Network"
#define L_MQTT_BROKER "MQTT Broker"
#define L_DOMOTICZ_SERVER "Domoticz Server"
#ifdef CONFIG_HARDWARE_LED
#define L_LEDS "LEDs"
#endif
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
#define L_SAVE "Save"

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

/* Form: device coniguration */
#define L_DEVICE_NAME "Device name"
#define L_DEVICE_SECTION_INFO                                                  \
  "Name is used for device identification within your LAN and a device's "     \
  "hotspot name used for configuration mode"
#define L_HARDWARE_CONFIGURATION "Hardware configuration"
#define L_HARDWARE_CONFIGURATION_INFO "Select connected items to your device"
#ifdef CONFIG_HARDWARE_LED
#define L_NUMBER_OF_LEDS "Number of LEDs"
#endif
#define L_NUMBER_OF_RELAYS "Number of relays"
#define L_NUMBER_OF_SWITCHES "Number of switches"
#define L_DO_MEASURE_ADC "Measurements from Analog Input"
#define L_DEVICE_CONTROLLING "Device controlling mechanism"
#define L_DEVICE_CONTROLLING_INFO "Enable / Disable APIs"

/* Form: network configuration */
#define L_NETWORK_CONFIGURATION_INFO                                           \
  "Device without connection to network will only work in manual mode"
#define L_SSID "WiFi name"
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

/* Form: MQTT Broker */
#define L_MQTT_TOPIC "MQTT Topic"
#define L_EXAMPLE "Example"
#define L_MQTT_CONFIGURATION_INFO "Enter MQTT Broker hostname or its IP address"

/* Form: Domoticz server configuration */
#define L_PROTOCOL "Protocol"
#define L_DOMOTICZ_CONFIGURATION "Domoticz Server"
#define L_DOMOTICZ_CONFIGURATION_INFO                                          \
  "Enter Domoticz hostname or its IP address"
#define L_NO_IF_IDX_0                                                          \
  "If IDX is set to 0 then a value won't be sent to Domoticz"

#ifdef CONFIG_HARDWARE_LED
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

/* Form: Switch */
#define L_FUNCTIONALITY "Functionality"
#define L_SYSTEM_BUTTON "System button"
#ifdef CONFIG_FUNCTIONALITY_RELAY
#define L_CONTROL_RELAY "Controlling only the relay"
#endif
#ifdef CONFIG_FUNCTIONALITY_GATE
#define L_CONTROL_GATE "Controlling only the gate"
#endif
#define L_RELAY_CONTROLLED_BY_SWITCH "Select relay controlled by this switch"
#define L_TYPE "Type"
#define L_MONOSTABLE "Monostable"
#define L_BISTABLE "Bistable"
#define L_SENSITIVENESS "Sensitiveness"
#define L__SENSITIVENESS_HINT                                                  \
  "Sensitiveness should be adjusted if switch didn't behave as expected "      \
  "while pressing it"
#define L_SWITCH_BUTTON "Switch / Button"

/* Form: Analog Input */
#define L_CANT_CHANGE "can't be set"
#define L_MEASURMENTS_INTERVAL "Measurement's interval"
#define L_MEASURED_VOLTAGE "Measured voltage"
#define L_NUMBER_OF_SAMPLES "Number of sample measurements"
#define L_RAW_DATA "Raw data"
#define L_PERCENT "Percent"
#define L_VOLTAGE "Voltage"

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
  "After 15s device will be rebooted. Please wait"
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

#endif
