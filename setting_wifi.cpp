#include "setting_wifi.h"

IPAddress apIp(192,168,4,1);
IPAddress apGateway(192,168,4,1);
IPAddress apSubnet(255,255,255,0);

uint32_t prev_time;

void wifiWdtFeed(){
  ESP.wdtFeed();                                  yield();
}

bool wifiStart(char* ssid, char* password) {
  WiFi.disconnect(true);                          
  WiFi.mode(WIFI_STA);                            delay(10);
  ESP.wdtFeed();                                  yield();

  WiFi.begin(ssid, password);                     Serial.println(ssid);
  
  prev_time = millis();
  while((millis() - prev_time) < WIFI_TIMEOUT){
    wifiWdtFeed();

    if(WiFi.status() == WL_CONNECTED){
      break;
    }
  }
  
  if(WiFi.status() == WL_CONNECTED){
    Serial.print("STA : ");                     Serial.println(WiFi.localIP());
    return true;
  }
  else {
    return false;
  }
}

bool wifiAP(char* ssid){
  WiFi.disconnect(true);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIp, apGateway, apSubnet);
  
  uint8_t n = 0;
  do{
    n++;
    wifiWdtFeed();
  }while(n < 10 && WiFi.softAP(ssid, AP_PASSWORD, AP_CHANNEL, false, AP_MAXCONNECT) == false );

  Serial.println(n);

  if(n < 4){
    Serial.print("AP : ");                       Serial.println(WiFi.softAPIP());
    return true;
  }

  return false;
}
