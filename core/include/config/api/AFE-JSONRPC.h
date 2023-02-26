#ifndef _AFE_Hardware_JSONRPC_h
#define _AFE_Hardware_JSONRPC_h

#define AFE_CONFIG_JSONRPC_REST_API_URL "https://api.smartnydom.pl/rest/"
#define AFE_CONFIG_JSONRPC_REST_API_SERVER "api.smartnydom.pl"
#define AFE_CONFIG_JSONRPC_REST_API_SERVER_PORT 80
#define AFE_CONFIG_JSONRPC_REST_API_SERVER_URI "/rest"

#define AFE_CONFIG_JSONRPC_DOWNLOAD_API_URL "https://api.smartnydom.pl/download/"
#define AFE_CONFIG_JSONRPC_REST_METHOD_WELCOME "get-welcome-message"
#define AFE_CONFIG_JSONRPC_REST_METHOD_LATEST_VERSION "check-version"
#define AFE_CONFIG_JSONRPC_REST_METHOD_CHECK_PRO "check-pro"
#define AFE_CONFIG_JSONRPC_REST_METHOD_PRO_OWNER "pro-key-owner"
#define AFE_CONFIG_JSONRPC_REST_METHOD_IS_PRO "is-pro"
#define AFE_CONFIG_JSONRPC_REST_METHOD_GET_LATEST_FIRMWARE_ID "get-latest-firmware-id"
#define AFE_CONFIG_JSONRPC_REST_METHOD_BOTTOM_TEXT "get-footer-message"
#define AFE_CONFIG_JSONRPC_REST_METHOD_FOOTER_SECTION "get-footer-section"
#define AFE_CONFIG_JSONRPC_REST_METHOD_UPGRADE_FIRMMWARE "get-firmware-upgrade-message"
#define AFE_CONFIG_JSONRPC_REST_METHOD_GET_LATEST_FIRMWARE_VERSION "get-latest-firmware-version"
#define AFE_CONFIG_JSONRPC_REST_METHOD_GET_JSON_LATEST_FIRMWARE_VERSION "get-json-latest-firmware-version"


/**
 * @brief JSON REST API calls
 * {"jsonrpc":"2.0","method":"12345678901234567890123456789012345678901234567890","params":"","id":1}
 * 
 */
#define AFE_CONFIG_JSONRPC_MESSAGE_MAX_SIZE 155 // @TODO its best estimatiion
#define AFE_CONFIG_JSONRPC_JSON_RESPONSE_SIZE 380
#define AFE_CONFIG_JSONRPC_JSON_TIMEOUT 3000 // Response timeout in msec

#endif // _AFE_Hardware_JSONRPC_h