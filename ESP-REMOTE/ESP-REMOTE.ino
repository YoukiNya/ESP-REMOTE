/*
 * 基于ESP32-arduino的网页RF灯具开关控制系统
 * 可用芯片：ESP32/ESP8266
 * Ver_0.45β
 * 用于ESP8266时请勿选择GPIO2，超再生发射模块DATA针脚为下拉，会导致启动失败
 * TX=1 RX=3    启动时RX可下拉
 */

#define sendpin 3
#define ZHYDD   138813456,32//中会吊灯      1000 0100 0110 0010 0000 0001 0000
#define ZHYSD   138813488,32//中会射灯      1000 0100 0110 0010 0000 0011 0000
#define ZBGSD   138813504,32//主办射灯      1000 0100 0110 0010 0000 0100 0000
#define ZBGDD   138813537,32//主办吊灯      1000 0100 0110 0010 0000 0110 0000
#define DYD     138813440,32//打印区灯      1000 0100 0110 0010 0000 0000 0000
#define CBD     138813472,32//餐吧吊灯      1000 0100 0110 0010 0000 0010 0000
#define DHYDD   138813552,32//大会吊灯      1000 0100 0110 0010 0000 0111 0000
#define DHYSD   138813520,32//大会射灯      1000 0100 0110 0010 0000 0101 0000
//以上是我自己使用的遥控开关数据，你可以自己修改

//#include "WiFi.h"
//#include "SPIFFS.h"
#include "ESP8266WiFi.h"
#include "FS.h"
#include "ESPAsyncWebServer.h"
#include "RCSwitch.h"
const char* ssid = "你的WIFISSID";
const char* password = "WIFI密码";
RCSwitch mySwitch = RCSwitch();
AsyncWebServer server(80);


void setup()
 {
  Serial.begin(115200);
  
  IPAddress staticIP(192,168,1,180);
  IPAddress gateway(192,168,1,1);
  IPAddress subnet(255,255,255,0);
  WiFi.config(staticIP, gateway, subnet);
  //这里把IP地址设置成了192.168.1.180，你可以自己修改

  
  mySwitch.enableTransmit(sendpin);

  
  SPIFFS.begin();
  
/*  USE IN ESP32
 *   
 if(!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
*/
bool ok = SPIFFS.begin();
  if (ok) {
    Serial.println("ok");
  }
    else{Serial.println("An Error has occurred while mounting SPIFFS");
    }

  WiFi.setSleepMode(WIFI_LIGHT_SLEEP); 
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println(WiFi.localIP());



  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false);
  });
  
  server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false);
  });
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });


  //中会议吊灯
  server.on("/ZHYDD", HTTP_GET, [](AsyncWebServerRequest *request){
    mySwitch.setPulseLength(353);
    mySwitch.send(ZHYDD);
    request->redirect("/index.html");
  });

  //中会议射灯
  server.on("/ZHYSD", HTTP_GET, [](AsyncWebServerRequest *request){
    mySwitch.setPulseLength(353);
    mySwitch.send(ZHYSD);
    request->redirect("/index.html");
  });

  //主办公区射灯
  server.on("/ZBGSD", HTTP_GET, [](AsyncWebServerRequest *request){
    mySwitch.setPulseLength(353);
    mySwitch.send(ZBGSD);
    request->redirect("/index.html");
  });

  //打印区灯
  server.on("/DYD", HTTP_GET, [](AsyncWebServerRequest *request){
    mySwitch.setPulseLength(353);
    mySwitch.send(DYD);
    request->redirect("/index.html");
  });

  //餐吧吊灯
  server.on("/CBD", HTTP_GET, [](AsyncWebServerRequest *request){
    mySwitch.setPulseLength(353);
    mySwitch.send(CBD);
    request->redirect("/index.html");
  });

    //大会议室吊灯
  server.on("/DHYDD", HTTP_GET, [](AsyncWebServerRequest *request){
    mySwitch.setPulseLength(353);
    mySwitch.send(DHYDD);
    request->redirect("/index.html");
  });

    //大会议室射灯
  server.on("/DHYSD", HTTP_GET, [](AsyncWebServerRequest *request){
    mySwitch.setPulseLength(353);
    mySwitch.send(DHYSD);
    request->redirect("/index.html");
  });


  
  server.begin();
}

void loop()
{
  
}
