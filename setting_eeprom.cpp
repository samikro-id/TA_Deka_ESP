#include "setting_eeprom.h"
#include <EEPROM.h>

void eeWdtFeed(){
  ESP.wdtFeed();                                  yield();
}

uint16_t arrayToInt(char* data){
  uint8_t length = strlen(data);
  uint16_t val = 0;

  for(uint8_t i=0; i<length; i++){
    if(data[i] >= 48 && data[i] <= 57){
      uint16_t dat = (data[i] - 48) * pow(10, length-i-1); 
      val += dat;
    } else {
      return 0;
    }

    eeWdtFeed();
  }
  return val;
}

uint8_t eeRead(uint16_t addr){
  uint8_t data;

  ESP.wdtFeed();                  yield();
  EEPROM.begin(EE_LENGTH);        delay(10);
  data = EEPROM.read(addr);
  EEPROM.commit();

  return data;
}

bool eeWrite(uint16_t addr, uint8_t data){
  
  ESP.wdtFeed();                  yield();
  EEPROM.begin(EE_LENGTH);        delay(10);
  EEPROM.write(addr, data);

  return EEPROM.commit();
}

bool eeWritePage(uint8_t addr, char* data, uint8_t len){

  for(uint8_t i=0; i<len; i++) {     
    EEPROM.write(addr + i, data[i]);  

    eeWdtFeed();
  }
  EEPROM.write(addr + len, '\n');
}

bool eeSetSSID(char* ssid, char* password){

  if(strlen(ssid) <= EE_WIFI_SSID_LEN && strlen(password) <= EE_WIFI_PSWD_LEN){

    ESP.wdtFeed();                  yield();
    EEPROM.begin(EE_LENGTH);        delay(10);
  
    eeWritePage(EE_ADDR_WIFI_SSID, ssid, strlen(ssid));
    eeWritePage(EE_ADDR_WIFI_PSWD, password, strlen(password));

    return EEPROM.commit();
  }else{

    return false;
  }
}

void eeGetSSID(char *ssid, char *password){
  char data[20];
  uint8_t c, i;

  ESP.wdtFeed();                  yield();
  EEPROM.begin(EE_LENGTH);        delay(10);

  i = 0;
  c = EEPROM.read(EE_ADDR_WIFI_SSID + i); 
  if(c != '\n'){
    do{
      data[i] = c;
      i++;
      c = EEPROM.read(EE_ADDR_WIFI_SSID + i);

      eeWdtFeed();
    }while(c != '\n' && i < EE_WIFI_SSID_LEN);
  }
  data[i] = NULL;
  memcpy(ssid, data, i);

  i = 0;
  c = EEPROM.read(EE_ADDR_WIFI_PSWD + i); 
  if(c != '\n'){
    do{
      data[i] = c;
      i++;
      c = EEPROM.read(EE_ADDR_WIFI_PSWD + i);

      eeWdtFeed();
    }while(c != '\n' && i < EE_WIFI_PSWD_LEN);
  }
  data[i] = NULL;
  memcpy(password, data, i);

  EEPROM.commit();
}

bool eeSetMqtt(char* url, char* port, char* username, char* password){

  if(strlen(url) <= EE_MQTT_URL_LEN && strlen(port) <= EE_MQTT_PORT_LEN && strlen(username) <= EE_MQTT_USER_LEN && strlen(password) <= EE_MQTT_PSWD_LEN){
    ESP.wdtFeed();                  yield();
    EEPROM.begin(EE_LENGTH);        delay(10);
  
    eeWritePage(EE_ADDR_MQTT_URL, url, strlen(url));
    eeWritePage(EE_ADDR_MQTT_PORT, port, strlen(port));
    eeWritePage(EE_ADDR_MQTT_USER, username, strlen(username));
    eeWritePage(EE_ADDR_MQTT_PSWD, password, strlen(password));

    return EEPROM.commit();
  }else{

    return false;
  }  
};

uint16_t eeGetMqtt(char* url, char* username, char* password){
  char data[64];
  uint8_t c, i;

  ESP.wdtFeed();                  yield();
  EEPROM.begin(EE_LENGTH);        delay(10);

  i = 0;
  c = EEPROM.read(EE_ADDR_MQTT_URL + i); 
  if(c != '\n'){
    do{
      data[i] = c;
      i++;
      c = EEPROM.read(EE_ADDR_MQTT_URL + i);

      eeWdtFeed();
    }while(c != '\n' && i < EE_MQTT_URL_LEN);
  }
  data[i] = NULL;
  memcpy(url, data, i);

  i = 0;
  c = EEPROM.read(EE_ADDR_MQTT_USER + i); 
  if(c != '\n'){
    do{
      data[i] = c;
      i++;
      c = EEPROM.read(EE_ADDR_MQTT_USER + i);

      eeWdtFeed();
    }while(c != '\n' && i < EE_MQTT_USER_LEN);
  }
  data[i] = NULL;
  memcpy(username, data, i);

  i = 0;
  c = EEPROM.read(EE_ADDR_MQTT_PSWD + i); 
  if(c != '\n'){
    do{
      data[i] = c;
      i++;
      c = EEPROM.read(EE_ADDR_MQTT_PSWD + i);

      eeWdtFeed();
    }while(c != '\n' && i < EE_MQTT_PSWD_LEN);
  }
  data[i] = NULL;
  memcpy(password, data, i);

  i = 0;
  c = EEPROM.read(EE_ADDR_MQTT_PORT + i); 
  if(c != '\n'){
    do{
      data[i] = c;
      i++;
      c = EEPROM.read(EE_ADDR_MQTT_PORT + i);

      eeWdtFeed();
    }while(c != '\n' && i < EE_MQTT_PORT_LEN);
  }
  data[i] = NULL;
  uint16_t intPort = arrayToInt(data);

  EEPROM.commit();

  return intPort;
};

bool eeSetId(char* user_id, char* group_id){
  if(strlen(user_id) <= EE_ID_USER_LEN && strlen(group_id) <= EE_ID_GROUP_LEN){
    ESP.wdtFeed();                  yield();
    EEPROM.begin(EE_LENGTH);        delay(10);
  
    eeWritePage(EE_ADDR_ID_USER, user_id, strlen(user_id));
    eeWritePage(EE_ADDR_ID_GROUP, group_id, strlen(group_id));

    return EEPROM.commit();
  }else{
    return false;
  }
};

void eeGetId(char* user_id, char* group_id){
  char data[35];
  uint8_t c, i;

  ESP.wdtFeed();                  yield();
  EEPROM.begin(EE_LENGTH);        delay(10);

  i = 0;
  c = EEPROM.read(EE_ADDR_ID_USER + i); 
  if(c != '\n'){
    do{
      data[i] = c;
      i++;
      c = EEPROM.read(EE_ADDR_ID_USER + i);
      if(c < 48 || c > 57){ c = '\n'; }           // pastikan character angka

      eeWdtFeed();
    }while(c != '\n' && i < EE_ID_USER_LEN);
  }
  data[i] = NULL;
  if(data[0] == NULL){
    data[0] = '0';
    data[1] = NULL;
    i++;
  }
  memcpy(user_id, data, i);

  i = 0;
  c = EEPROM.read(EE_ADDR_ID_GROUP + i); 
  if(c != '\n'){
    do{
      data[i] = c;
      i++;
      c = EEPROM.read(EE_ADDR_ID_GROUP + i);
      if(c < 48 || c > 57){ c = '\n'; }           // pastikan character angka
      
      eeWdtFeed();
    }while(c != '\n' && i < EE_ID_GROUP_LEN);
  }
  data[i] = NULL;
  if(data[0] == NULL){
    data[0] = '0';
    data[1] = NULL;
    i++;
  }
  memcpy(group_id, data, i);

  EEPROM.commit();
};

bool eeSetAuto(bool value){
  if(value == true) {    return eeWrite(EE_ADDR_AUTO, 100);  }
  else              {    return eeWrite(EE_ADDR_AUTO, 0);  }
}

bool eeGetAuto(){
  if(eeRead(EE_ADDR_AUTO) == 100) {    return true;  }
  else                            {    return false;  }
}

uint8_t eeGetFlag(){
  return eeRead(EE_ADDR_RESET);
}
 
bool eeRstFlag(){
  return eeWrite(EE_ADDR_RESET, 0);
};

bool eeIncFlag(){
  uint8_t  data;

  data = eeRead(EE_ADDR_RESET);
  if(data > 200) data = 0;
  data++;
  return eeWrite(EE_ADDR_RESET, data);
};

bool eeSetFlag(uint8_t value){
  return eeWrite(EE_ADDR_RESET, value);
};

bool eeSetDim(uint8_t dimming){
  return eeWrite(EE_ADDR_DIM, dimming);
};

uint8_t eeGetDim(){
  if(eeRead(EE_ADDR_DIM) > 100) {    return 100;  }
  else                          {    return eeRead(EE_ADDR_DIM);  }
};

bool eeSetTun(uint8_t tune){
  return eeWrite(EE_ADDR_TUN, tune);
};

uint8_t eeGetTun(){
  if(eeRead(EE_ADDR_TUN) > 100) {    return 100;  }
  else                          {    return eeRead(EE_ADDR_TUN);  }
};

bool eeSetFirmFlag(uint8_t value){
  return eeWrite(EE_ADDR_FIRM_FLAG, value);
};

uint8_t eeGetFirmFlag(){
  return eeRead(EE_ADDR_FIRM_FLAG);
};