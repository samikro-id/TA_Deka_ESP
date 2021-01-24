#include <ESP8266WiFi.h>

/***** define WiFi Configuration *****/
#define AP_PASSWORD         "seipandaan"
#define AP_MAXCONNECT       3
#define AP_CHANNEL          2
#define WIFI_START_FLAG     0x01

#define WIFI_TIMEOUT        20000                   // connection timeout to network 20 second

bool wifiStart(char* ssid, char* password);
bool wifiAP(char* ssid);