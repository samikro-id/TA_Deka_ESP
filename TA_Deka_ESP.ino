#include <ESP8266WiFi.h>                  // Install esp8266 by ESP8266 Community version 2.6.3
#include <ArduinoJson.h>                  // Install Library by Benoit Blanchon version 5.13.5
#include <PubSubClient.h>                 // Install Library by Nick O'Leary version 2.7.0

WiFiClient espClient;
PubSubClient client(espClient);

uint32_t current_time;
uint32_t chart_time;
uint32_t led_time;

bool led_state = false;

char payload_get[50];
bool mqtt = false;

char text[100];

char ssid[] = "samikro";
char pass[] = "samikroid";

#define MQTT_BROKER     "broker.emqx.io"      //
#define MQTT_PORT       1883                        //
#define MQTT_USERNAME   ""                          // Change to your Username from Broker
#define MQTT_PASSWORD   ""                          // Change to your password from Broker
#define MQTT_TIMEOUT    10
#define MQTT_QOS        0
#define MQTT_RETAIN    false

#define MQTT2_BROKER     "mqtt.thingspeak.com"      //
#define MQTT2_PORT       1883                        //
#define MQTT2_USERNAME   "dekaputra"                          // Change to your Username from Broker
#define MQTT2_PASSWORD   "U3N5KWAK07VCDH7S"                          // Change to your password from Broker
#define MQTT2_TIMEOUT    2
#define MQTT2_QOS        0
#define MQTT2_RETAIN     false

void callback(char* topic, byte* payload, unsigned int length) { //A new message has been received
  memcpy(payload_get, payload, length);
  mqtt = true;
}

float tegangan, arus, energy;
bool relay;

/*
 *  enum rst_info->reason {
 *    0 : REASON_DEFAULT_RST, normal startup by power on
 *    1 : REASON_WDT_RST, hardware watch dog reset 
 *    2 : REASON_EXCEPTION_RST, exception reset, GPIO status won't change 
 *    3 : REASON_SOFT_WDT_RST, software watch dog reset, GPIO status won't change 
 *    4 : REASON_SOFT_RESTART, software restart ,system_restart , GPIO status won't change 
 *    5 : REASON_DEEP_SLEEP_AWAKE, wake up from deep-sleep 
 *    6 : REASON_EXT_SYS_RST, external system reset 
 *  };
 */
rst_info *myResetInfo;

void setup(){
  delay(300);
  Serial.begin(2400);

  initLed();

  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);                            delay(10);
  ESP.wdtFeed();                                  yield();

  current_time = millis();
  led_time = current_time;

  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED){
    yield();

    current_time = millis();
    if((current_time-led_time) > 1000){          // toggle led setiap detik
      toggleLed();
    }

    if(WiFi.status() == WL_CONNECTED){
      break;
    }
  }
  
  if(WiFi.status() == WL_CONNECTED){
    connectMqtt();
  }

  chart_time = millis() + 60000;
}

void loop(){
  if(WiFi.status() == WL_CONNECTED){

    if((current_time-chart_time) >= 60000){
      publishChart();

      chart_time = millis();
    }

    if(mqtt){
      if(strstr(payload_get, "data")){
        client.publish("samikro/data/project/1","{\"volt\":100,\"current\":50,\"energy\":20,\"status\":\"ON\"}",false);
      }

      if(strstr(payload_get, "control")){
        if(strstr(payload_get, "ON")){

        }
        if(strstr(payload_get, "OFF")){
          
        }
      }

      mqtt = false;
    }
  }

  current_time = millis();        // update current time
  client.loop();
}

void publishChart(){
  Serial.println("{\"operation\":\"data\"}");

  client.disconnect();
  client.setServer(MQTT2_BROKER, MQTT2_PORT);

  uint8_t i;
  for(i=0; i<MQTT2_TIMEOUT; i++){

    if(client.connect("7912c119-3a3c-4127-ac2e-440aadd3599a", MQTT2_USERNAME, MQTT2_PASSWORD)){
      delay(500);

      client.publish("channels/1289348/publish/I8VQHYUWACAQX3IJ","field1=100&field2=50&field3=20&field4=10",false);
      break;
    }
  }

  connectMqtt();
}

bool connectMqtt(){
  client.disconnect();
  client.setServer(MQTT_BROKER, MQTT_PORT);
  client.setCallback(callback);
  mqtt = false;

  uint8_t i;
  for(i = 0; i < MQTT_TIMEOUT; i++) {
      
    if( client.connect("7912c119-3a3c-4127-ac2e-440aadd3599a", MQTT_USERNAME, MQTT_PASSWORD) ){
      delay(500);
      if(client.subscribe("samikro/cmd/project/1", MQTT_QOS)){
        return true;
        break;
      }
    }

    delay(500);                                 
  }
  return false;
}

void clearData(){
  uint8_t n;
  for(n=0; n<50; n++){
    payload_get[n] = 0;
  }
}

/***** Serial ******/

bool waitSerial(){

  previous_time = millis();
  do{
      if(Serial.available() > 0){
          break;
      }
  }while((millis() - previous_time) <= 10);

  n = 0;
  while(Serial.available() > 0){
      byte d = (char) pzemSerial.read();
      recv[n] = d;
      n++;
  }
}

/***** LED Setting *****/

void initLed(){
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);      // default mati
  led_state = false;
}

void toggleLed(){
  if(led_state == false){
    digitalWrite(LED_BUILTIN, LOW);     // nyalakan LED
    led_state = true;
  }else{
    digitalWrite(LED_BUILTIN, HIGH);    // matikan LED
    led_state = false;
  }
}

void setLed(bool state){
  if(state){
    digitalWrite(LED_BUILTIN, LOW);     // nyalakan LED
  }else{
    digitalWrite(LED_BUILTIN, HIGH);    // matikan LED
  }

  led_state = state;
}
