#include <ESP8266WiFi.h>
/* Read eeprom_management.xlsx file for eeprom mapping format */

#define EE_LENGTH       265

/* Storage Length (byte) */
#define EE_WIFI_SSID_LEN    32      
#define EE_WIFI_PSWD_LEN    20      
#define EE_MQTT_PORT_LEN    5       
#define EE_MQTT_URL_LEN     64      
#define EE_MQTT_USER_LEN    64      
#define EE_MQTT_PSWD_LEN    32      
#define EE_ID_USER_LEN      16      
#define EE_ID_GROUP_LEN     16

/* Start Address EEPROM */
#define EE_ADDR_FIRM_FLAG   1       // Address 1
#define EE_ADDR_AUTO        2       // Address 2
#define EE_ADDR_RESET       3       // Address 3
#define EE_ADDR_WIFI_SSID   4       // Address 4 - 36 = 32 Byte + End
#define EE_ADDR_WIFI_PSWD   37      // Address 37 - 57 = 20 Byte + End
#define EE_ADDR_MQTT_PORT   58      // Address 58 - 63 = 5 Byte + End
#define EE_ADDR_MQTT_URL    64      // Address 64 - 128 = 64 Byte + End 
#define EE_ADDR_MQTT_USER   129     // Address 129 - 193 = 64 Byte + End
#define EE_ADDR_MQTT_PSWD   194     // Address 194 - 226 = 32 Byte + End
#define EE_ADDR_ID_USER     227     // Address 227 - 243 = 16 Byte + End
#define EE_ADDR_ID_GROUP    244     // Address 244 - 260 = 16 Byte + End
#define EE_ADDR_DIM         261     // Address 261
#define EE_ADDR_TUN         262     // Address 262

bool eeSetSSID(char* ssid, char* password);
void eeGetSSID(char *ssid, char *password);

bool eeSetMqtt(char* url, char* port, char* username, char* password);
uint16_t eeGetMqtt(char* url, char* username, char* password);

bool eeSetId(char* user_id, char* group_id);
void eeGetId(char* user_id, char* group_id);

bool eeSetAuto(bool value);
bool eeGetAuto();

uint8_t eeGetFlag();
bool eeRstFlag();
bool eeIncFlag();
bool eeSetFlag(uint8_t value);

bool eeSetDim(uint8_t dimming);
uint8_t eeGetDim();

bool eeSetTun(uint8_t tune);
uint8_t eeGetTun();

bool eeSetFirmFlag(uint8_t value);
uint8_t eeGetFirmFlag();
