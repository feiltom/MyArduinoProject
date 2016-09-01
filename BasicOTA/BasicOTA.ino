
#define DEBUG 1

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include "password.h" //Declare const char *wifi_password = "XXXXXXXX"; const char *wifi_ssid = "XXXXXXXX";

#ifdef DEBUG
  #include <WiFiUdp.h>
  unsigned int localPort = 514;
  IPAddress syslogServer(192, 168, 1, 7);
  WiFiUDP udp;

void sendUdpSyslog(String msgtosend)
{
  unsigned int msg_length = msgtosend.length();
  byte* p = (byte*)malloc(msg_length);
  memcpy(p, (char*) msgtosend.c_str(), msg_length);

  udp.beginPacket(syslogServer, 514);
  udp.write("esp8266-OnlyOTA ");
  udp.write(p, msg_length);
  udp.endPacket();
  free(p);
}
#endif

ESP8266WiFiMulti WiFiMulti;
WiFiClient wclient;
String UdpBuffer;
void CheckOTA()
{
if((WiFiMulti.run() == WL_CONNECTED)) {
  #ifdef DEBUG
    sendUdpSyslog("Flash Size In Byte:");
    UdpBuffer=ESP.getFlashChipRealSize();
    sendUdpSyslog(UdpBuffer);
  #endif
 
        t_httpUpdate_return ret = ESPhttpUpdate.update("http://192.168.1.5/ota/ota.php","0");
        
        switch(ret) {
            case HTTP_UPDATE_FAILED:
                #ifdef DEBUG
                    char chArray[200] ;
                    sprintf(chArray,"HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
                    sendUdpSyslog(String(chArray));
                  #endif
                break;

            case HTTP_UPDATE_NO_UPDATES:
                #ifdef DEBUG
                 sendUdpSyslog("HTTP_UPDATE_NO_UPDATES");
                  #endif
                break;

            case HTTP_UPDATE_OK:
                #ifdef DEBUG
                  sendUdpSyslog("HTTP_UPDATE_OK");
                  #endif
                break;
        }
    }
}



void setup()
{
  WiFiMulti.addAP(wifi_ssid, wifi_password);
  int Attempt = 0;
  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
    Attempt++;
    if (Attempt == 200)
    {
      ESP.restart();
    }
  }
#ifdef DEBUG
  udp.begin(localPort);
  sendUdpSyslog(WiFi.localIP().toString());
#endif

  CheckOTA();
//  delay(10000);

}


void loop()
{



delay(50);


}
