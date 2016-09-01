#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "password.h" //Declare const char *wifi_password = "XXXXXXXX"; const char *wifi_ssid = "XXXXXXXX";

#define DEBUG_PRINT 1

unsigned int localPort = 514;
IPAddress syslogServer(192, 168, 1, 7);

String str;
//long startMills;

WiFiClient wifiClient;
WiFiUDP udp;

void wifi_connect() {

  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid, wifi_password);

  int Attempt = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Attempt++;
    if (DEBUG_PRINT) {
      Serial.print(".");
    }
    if (Attempt == 200)
    {
      ESP.restart();
    }
  }
  if (DEBUG_PRINT) {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
}

void setup()
{
  Serial.begin(115200);
  delay(20);
  wifi_connect();
  udp.begin(localPort);
  delay(500);
  if (WiFi.status() == WL_CONNECTED) {
    sendUdpSyslog("esp8266-02-syslog started");
    sendUdpSyslog(WiFi.localIP().toString());

  }
}

void loop()
{
  if (WiFi.status() == WL_CONNECTED) {
    str = Serial.readStringUntil('\n');
    str.trim();
    unsigned int msg_length = str.length();
    if ( msg_length > 0 ) {
      sendUdpSyslog(str);
    }
  } else {
    wifi_connect();
  }
}

void sendUdpSyslog(String msgtosend)
{
  unsigned int msg_length = msgtosend.length();
  byte* p = (byte*)malloc(msg_length);
  memcpy(p, (char*) msgtosend.c_str(), msg_length);

  udp.beginPacket(syslogServer, 514);
  udp.write("esp8266-02-syslog ");
  udp.write(p, msg_length);
  udp.endPacket();
  free(p);
}
