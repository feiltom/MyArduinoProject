#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <FS.h>
#include <ESP8266WebServer.h>
#include <TimeLib.h>
#include <time.h>
#include "password.h" //Declare const char *wifi_password = "XXXXXXXX"; const char *wifi_ssid = "XXXXXXXX";
String Ligne1;
String Ligne2;
extern "C" {
 #include "user_interface.h"              // to use deep sleep
uint16 readvdd33(void);
}

ESP8266WebServer server ( 80 );


void handleRoot() {
  char temp[1024];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;
  int voltage = readvdd33();
  char templigne1[17];
  char templigne2[17];
  Ligne1.toCharArray(templigne1, 17);
    Ligne2.toCharArray(templigne2, 17);
  snprintf ( temp, 1024,
  

"<html>\
  <head>\
    <meta http-equiv='refresh' content=\"5;URL='/'\"/>\
    <title>ESP8266 Demo</title>\
    <style>\
    @font-face {\
    font-family: 'LcdFonte';\
    src: url('lcd.ttf') format('TrueType');font-weight: normal;font-style: normal;\
}\
      body { background-color: #cccccc; font-family:  Helvetica, Arial, sans-serif; Color: #000088; }\
      p { font-family:'LcdFonte';font-size:x-large;}\
    </style>\
  </head>\
  <body>\
Set Time : <a href='#' OnClick='javascript:location.href = \"settime?time=\"+Math.round(Date.now()/ 1000+7200);'>UTC+2</a>\
  <a href='#' OnClick='javascript:location.href = \"settime?time=\"+Math.round(Date.now()/ 1000+3600);'>UTC+1</a>\
    <p>voltage: %02d</p>\
    <p  style='background-color: #3BCEFF'>%s</br>%s</p>\
    <p><a href='?menu=moins'>&lt;</a> <a href='?menu=ok'>ok</a> <a href='?menu=plus'>&gt; </a></p>\
  </body>\
</html>",

    voltage,templigne1,templigne2
  );
  server.send ( 200, "text/html", temp );
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }

  server.send ( 404, "text/plain", message );
}

void setup ( void ) {
  Serial.begin ( 115200 );
//  WiFi.softAP(wifi_ssid, wifi_password);
  Serial.println ( "" );

  // Wait for connection
    WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println ( "" );
  Serial.print ( "Connected to " );
  Serial.println ( ssid );
  Serial.print ( "IP address: " );
  Serial.println ( WiFi.localIP() );

  server.on("/lcd.ttf", loadFromSpiffs);
  server.on ( "/", handleRoot );
  server.on ( "/inline", []() {
    server.send ( 200, "text/plain", "this works as well" );
  } );
  server.on ( "/settime", []() {
    server.send ( 200, "text/plain", "time is set"  );
    String timestamp = server.arg (0);
    setTime(timestamp.toInt());
    } );
  server.onNotFound ( handleNotFound );
  server.begin();
  Serial.println ( "HTTP server started" );
  setTime(1357041600);
}
void handleDownload(){
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS failed to mount !\r\n");                    
  }
  else {
    String str = "";
    File f = SPIFFS.open("/lcd.ttf", "r");
    if (!f) {
      Serial.println("Can't open SPIFFS file !\r\n");          
    }
    else {
      char buf[1024];
      int siz = f.size();
      while(siz > 0) {
        size_t len = std::min((int)(sizeof(buf) - 1), siz);
        f.read((uint8_t *)buf, len);
        buf[len] = 0;
        str += buf;
        siz -= sizeof(buf) - 1;
      }
      f.close();
      server.send(200, "font/opentype", str);
    }
  }
} 
bool loadFromSpiffs(){
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS failed to mount !\r\n");                    
  }
  String dataType = "font/opentype";

  File dataFile = SPIFFS.open("/lcd.ttf", "r");
    if (!dataFile) {
      Serial.println("Can't open SPIFFS file !\r\n");          
    }
  if (server.streamFile(dataFile, dataType) != dataFile.size()) {
      Serial.println("Error !\r\n");          

  }

  dataFile.close();
  return true;
}

void loop ( void ) {
  server.handleClient();
delay(100);
   char tampon[17];

     snprintf ( tampon, 17, "%02d:%02d:%02d", hour(),minute(),second());
Ligne1=tampon;
snprintf ( tampon, 17, "[ ]Set [%s]Start","X");
Ligne2=tampon;

//        Serial.println(Ligne1);          

}


