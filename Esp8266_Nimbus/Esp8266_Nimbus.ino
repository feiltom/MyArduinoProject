
#define DEBUG 1

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <PubSubClient.h>
#include <brzo_i2c.h>
#include "password.h" //Declare const char *wifi_password = "XXXXXXXX"; const char *wifi_ssid = "XXXXXXXX";

ESP8266WiFiMulti WiFiMulti;
const char* mqtt_server = "192.168.1.5";
WiFiClient wclient;
PubSubClient client(wclient);
long lastMsg = 0;
char msg[50];

const uint8_t fonttable[580] = {
        0x01, 0x00, 0x00, 0x00, 0x00, 0x00,      // Code for char Space 
        0x01, 0x7D, 0x00, 0x00, 0x00, 0x00,      // Code for char !
        0x05, 0x70, 0x60, 0x00, 0x70, 0x60,      // Code for char "
        0x05, 0x24, 0x7E, 0x24, 0x7E, 0x24,      // Code for char //
        0x04, 0x12, 0x6A, 0x2B, 0x24, 0x00,      // Code for char $
        0x05, 0x63, 0x64, 0x08, 0x13, 0x63,      // Code for char %
        0x05, 0x36, 0x49, 0x35, 0x02, 0x05,      // Code for char &
        0x02, 0x70, 0x60, 0x00, 0x00, 0x00,      // Code for char '
        0x02, 0x3E, 0x41, 0x00, 0x00, 0x00,      // Code for char (
        0x02, 0x41, 0x3E, 0x00, 0x00, 0x00,      // Code for char )
        0x05, 0x08, 0x3E, 0x1C, 0x3E, 0x08,      // Code for char *
        0x05, 0x08, 0x08, 0x3E, 0x08, 0x08,      // Code for char +
        0x02, 0x01, 0x06, 0x00, 0x00, 0x00,      // Code for char ,
        0x03, 0x08, 0x08, 0x08, 0x00, 0x00,      // Code for char -
        0x01, 0x01, 0x00, 0x00, 0x00, 0x00,      // Code for char .
        0x04, 0x04, 0x08, 0x10, 0x20, 0x00,      // Code for char /
        0x03, 0x3E, 0x41, 0x3E, 0x00, 0x00,      // Code for char 0
        0x03, 0x21, 0x7F, 0x01, 0x00, 0x00,      // Code for char 1
        0x03, 0x47, 0x49, 0x31, 0x00, 0x00,      // Code for char 2
        0x03, 0x22, 0x49, 0x36, 0x00, 0x00,      // Code for char 3
        0x03, 0x1C, 0x24, 0x7F, 0x00, 0x00,      // Code for char 4
        0x03, 0x79, 0x49, 0x46, 0x00, 0x00,      // Code for char 5
        0x03, 0x3E, 0x49, 0x26, 0x00, 0x00,      // Code for char 6
        0x03, 0x40, 0x4F, 0x70, 0x00, 0x00,      // Code for char 7
        0x03, 0x36, 0x49, 0x36, 0x00, 0x00,      // Code for char 8
        0x03, 0x32, 0x49, 0x3E, 0x00, 0x00,      // Code for char 9
        0x01, 0x14, 0x00, 0x00, 0x00, 0x00,      // Code for char :
        0x02, 0x01, 0x16, 0x00, 0x00, 0x00,      // Code for char ;
        0x03, 0x08, 0x14, 0x22, 0x00, 0x00,      // Code for char <
        0x03, 0x14, 0x14, 0x14, 0x00, 0x00,      // Code for char =
        0x03, 0x22, 0x14, 0x08, 0x00, 0x00,      // Code for char >
        0x03, 0x20, 0x4D, 0x30, 0x00, 0x00,      // Code for char ?
        0x04, 0x3E, 0x49, 0x55, 0x3D, 0x00,      // Code for char @
        0x03, 0x3F, 0x44, 0x3F, 0x00, 0x00,      // Code for char A
        0x03, 0x7F, 0x49, 0x36, 0x00, 0x00,      // Code for char B
        0x03, 0x3E, 0x41, 0x41, 0x00, 0x00,      // Code for char C
        0x03, 0x7F, 0x41, 0x3E, 0x00, 0x00,      // Code for char D
        0x03, 0x7F, 0x49, 0x49, 0x00, 0x00,      // Code for char E
        0x03, 0x7F, 0x48, 0x48, 0x00, 0x00,      // Code for char F
        0x04, 0x3E, 0x41, 0x49, 0x4E, 0x00,      // Code for char G
        0x03, 0x7F, 0x08, 0x7F, 0x00, 0x00,      // Code for char H
        0x03, 0x41, 0x7F, 0x41, 0x00, 0x00,      // Code for char I
        0x04, 0x46, 0x41, 0x7F, 0x40, 0x00,      // Code for char J
        0x03, 0x7F, 0x08, 0x77, 0x00, 0x00,      // Code for char K
        0x03, 0x7F, 0x01, 0x01, 0x00, 0x00,      // Code for char L
        0x05, 0x7F, 0x20, 0x10, 0x20, 0x7F,      // Code for char M
        0x04, 0x7F, 0x10, 0x08, 0x7F, 0x00,      // Code for char N
        0x04, 0x3E, 0x41, 0x41, 0x3E, 0x00,      // Code for char O
        0x03, 0x7F, 0x48, 0x30, 0x00, 0x00,      // Code for char P
        0x04, 0x3E, 0x41, 0x45, 0x3E, 0x00,      // Code for char Q
        0x03, 0x7F, 0x48, 0x37, 0x00, 0x00,      // Code for char R
        0x03, 0x31, 0x49, 0x46, 0x00, 0x00,      // Code for char S
        0x03, 0x40, 0x7F, 0x40, 0x00, 0x00,      // Code for char T
        0x04, 0x7E, 0x01, 0x01, 0x7E, 0x00,      // Code for char U
        0x03, 0x7E, 0x01, 0x7E, 0x00, 0x00,      // Code for char V
        0x05, 0x7E, 0x01, 0x06, 0x01, 0x7E,      // Code for char W
        0x03, 0x77, 0x08, 0x77, 0x00, 0x00,      // Code for char X
        0x03, 0x78, 0x07, 0x78, 0x00, 0x00,      // Code for char Y
        0x04, 0x47, 0x49, 0x51, 0x61, 0x00,      // Code for char Z
        0x02, 0x7F, 0x41, 0x00, 0x00, 0x00,      // Code for char [
        0x03, 0x30, 0x08, 0x06, 0x00, 0x00,      // Code for char BackSlash
        0x02, 0x41, 0x7F, 0x00, 0x00, 0x00,      // Code for char ]
        0x03, 0x20, 0x40, 0x20, 0x00, 0x00,      // Code for char ^
        0x03, 0x01, 0x01, 0x01, 0x00, 0x00,      // Code for char _
        0x02, 0x60, 0x10, 0x00, 0x00, 0x00,      // Code for char `
        0x03, 0x17, 0x15, 0x0F, 0x00, 0x00,      // Code for char a
        0x03, 0x3F, 0x05, 0x02, 0x00, 0x00,      // Code for char b
        0x03, 0x06, 0x09, 0x09, 0x00, 0x00,      // Code for char c
        0x03, 0x02, 0x05, 0x1F, 0x00, 0x00,      // Code for char d
        0x03, 0x06, 0x0D, 0x05, 0x00, 0x00,      // Code for char e
        0x03, 0x08, 0x3F, 0x48, 0x00, 0x00,      // Code for char f
        0x03, 0x3B, 0x2B, 0x3F, 0x00, 0x00,      // Code for char g
        0x03, 0x1F, 0x04, 0x03, 0x00, 0x00,      // Code for char h
        0x01, 0x17, 0x00, 0x00, 0x00, 0x00,      // Code for char i
        0x03, 0x02, 0x11, 0x5E, 0x00, 0x00,      // Code for char j
        0x03, 0x3F, 0x04, 0x1B, 0x00, 0x00,      // Code for char k
        0x02, 0x3E, 0x01, 0x00, 0x00, 0x00,      // Code for char l
        0x05, 0x0F, 0x08, 0x06, 0x08, 0x07,      // Code for char m
        0x03, 0x0F, 0x08, 0x07, 0x00, 0x00,      // Code for char n
        0x03, 0x06, 0x09, 0x06, 0x00, 0x00,      // Code for char o
        0x03, 0x1F, 0x14, 0x08, 0x00, 0x00,      // Code for char p
        0x03, 0x08, 0x14, 0x1F, 0x00, 0x00,      // Code for char q
        0x03, 0x07, 0x08, 0x08, 0x00, 0x00,      // Code for char r
        0x03, 0x09, 0x15, 0x12, 0x00, 0x00,      // Code for char s
        0x03, 0x1E, 0x09, 0x09, 0x00, 0x00,      // Code for char t
        0x03, 0x0E, 0x01, 0x0F, 0x00, 0x00,      // Code for char u
        0x03, 0x0E, 0x01, 0x0E, 0x00, 0x00,      // Code for char v
        0x05, 0x0E, 0x01, 0x06, 0x01, 0x0E,      // Code for char w
        0x03, 0x1B, 0x04, 0x1B, 0x00, 0x00,      // Code for char x
        0x03, 0x19, 0x05, 0x1E, 0x00, 0x00,      // Code for char y
        0x03, 0x13, 0x15, 0x19, 0x00, 0x00,      // Code for char z
        0x03, 0x08, 0x3E, 0x41, 0x00, 0x00,      // Code for char {
        0x01, 0x7F, 0x00, 0x00, 0x00, 0x00,      // Code for char |
        0x03, 0x41, 0x3E, 0x08, 0x00, 0x00,      // Code for char }
        0x04, 0x20, 0x40, 0x20, 0x40, 0x00,      // Code for char ~
        0x04, 0x1E, 0x32, 0x62, 0x32, 0x00       // Code for char 127
};

int NIMBUS_TEXT_MAX_WIDTH  =  41;
const uint8_t addrGaugeCtrl = 0x25;
const uint8_t addrGaugeDataCW[5] = {0xC0,0xE0,0xA0,0x80,0x0F};
const uint8_t addrGaugeDataCCW[5] = {0xD0,0xF0,0xB0,0x90,0x0F};
const uint8_t addrDispCtrl[4] =   {0x3A,0x3E,0x3C,0x38};
const uint8_t addrDispData[4] =  {0x3B,0x3F,0x3D,0x39};
uint8_t initDisp[20] = {0xE2,0x20,0xC0,0x8D,0xEB,0x81,0x30,0xB5,0xA1,0x31,0x46,0x2D,0x85,0xF2,0x00,0xF3,0x07,0x90,0xAF,0x40};
uint8_t buffer[24];

byte setGauge(int angle,int cadran,boolean way){
  if (way) {buffer[0] = addrGaugeDataCW[cadran];} else {buffer[0] = addrGaugeDataCCW[cadran];}
  buffer[1] = angle;buffer[2] = 0x00;
  brzo_i2c_start_transaction(0x25,400);
  brzo_i2c_write(buffer,3,false);
  return brzo_i2c_end_transaction();
}

void init_display(){
  for (int indexAddr=0; indexAddr <= 3; indexAddr++){
    //Serial.println("index");
    //Serial.println(indexAddr);
    brzo_i2c_start_transaction(addrDispCtrl[indexAddr],400);
    brzo_i2c_write(initDisp,20,false);
     brzo_i2c_end_transaction();
 //    Serial.println(brzo_i2c_end_transaction());
    delay(50);
  }
  
  buffer[0] = 0xB0;
  buffer[1] = 0x10;
  buffer[2] = 0x00;
  brzo_i2c_start_transaction(addrDispCtrl[0],400);
  brzo_i2c_write(buffer,3,false);
  brzo_i2c_end_transaction();
  delay(50);

  buffer[0] =0x7F;
  buffer[1] =0x00;
  buffer[2] =0x7F;
  buffer[3] =0x00;
  buffer[4] =0x7F;
  brzo_i2c_start_transaction(addrDispData[0],400);
  brzo_i2c_write(buffer,5,false);
  brzo_i2c_end_transaction();
  delay(50);
}

int PrintChar(int addrCadran,char asciichar,int remainingLen )
{
    int ordchar = asciichar ;   //# Get the ascii position of the char.
    int written = 0;
    int allowedLen;
    
    if (remainingLen<1){
      #ifdef DEBUG
        Serial.print("not enough space remaining on display");
      #endif
      return 0;
      }
    if (ordchar < 0x20){
      #ifdef DEBUG
        Serial.print("char out of range, too low");
      #endif
      return 0;
      }
    if (ordchar >= 0x7F){
      #ifdef DEBUG
        Serial.print("char out of range, too low");
      #endif
      return 0;
      }

    int firstcol = (ordchar - 0x20) * 6;

     if (remainingLen<fonttable[firstcol]){allowedLen = remainingLen;} else {allowedLen = fonttable[firstcol];}
     for (int col=1; col <= allowedLen; col++){
       buffer[0] =fonttable[firstcol + col];
       brzo_i2c_write(buffer,1,false);
       written += 1;  
       }

    buffer[0] =0x00;
       
    if( written<remainingLen ){brzo_i2c_write(buffer,1,false);written += 1;}
   return written;
}

void CheckOTA()
{
if((WiFiMulti.run() == WL_CONNECTED)) {
  #ifdef DEBUG
    Serial.print("Flash Size In Byte:");Serial.println(ESP.getFlashChipRealSize());
  #endif
 
        t_httpUpdate_return ret = ESPhttpUpdate.update("http://esp-ota.feillant.com/ota.php");
        
        switch(ret) {
            case HTTP_UPDATE_FAILED:
                #ifdef DEBUG
                  Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
                  Serial.println(ESP.getFlashChipRealSize());
                  #endif
                break;

            case HTTP_UPDATE_NO_UPDATES:
                #ifdef DEBUG
                  Serial.println("HTTP_UPDATE_NO_UPDATES");
                  #endif
                break;

            case HTTP_UPDATE_OK:
                #ifdef DEBUG
                  Serial.println("HTTP_UPDATE_OK");
                  #endif
                break;
        }
    }
}

char* explode(byte* stringbyte){
char str[32] = "";
char dlm[] = "|";
 
int cnt = 0;
byte* tab[10] = { 0 };
 
char *pch = strtok(str, dlm);
 
while ( pch != NULL ) {
    if (cnt < 10) {
        tab[cnt++] = pch;
    } else {
        break;
    }
    pch = strtok (NULL, dlm);
  return tab[2];
}
  }
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(explode(topic));
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
   // digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
   // digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void setup()
{
  #ifdef DEBUG
    Serial.begin(115200);
    Serial.println();Serial.println();Serial.println();
  #endif
  
  for(uint8_t t = 4; t > 0; t--) {
    #ifdef DEBUG
      Serial.printf("[SETUP] WAIT %d...\n", t);
      Serial.flush();
    #endif
    delay(1000);
    }

  WiFiMulti.addAP(wifi_ssid, wifi_password);

  Serial.println("Connecting Wifi...");
    if(wifiMulti.run() == WL_CONNECTED) {
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
    }
  brzo_i2c_setup(0,2,200);
  delay(50);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  CheckOTA();
//  delay(10000);

}



void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Nimbus",wifi_ssid,mqtt_password)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
    //  client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("cmnd/nimbus/*");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop()
{


/*
setGauge(40,0,1);
delay(50);
setGauge(60,1,1);
delay(50);
setGauge(80,2,1);
delay(50);
setGauge(100,3,1);
delay(50);
init_display();
  buffer[0] = 0xB0;
  buffer[1] = 0x10;
  buffer[2] = 0x00;
  brzo_i2c_start_transaction(addrDispCtrl[0],400);
      brzo_i2c_write(buffer,3,false);

     Serial.println(brzo_i2c_end_transaction());
  delay(50);
    brzo_i2c_start_transaction(addrDispData[0],400);
    
    PrintChar(0x39,'Y',41);
    PrintChar(0x39,'o',41);
    PrintChar(0x39,' ',41);
    PrintChar(0x39,'A',41);
    PrintChar(0x39,'l',41);
    PrintChar(0x39,'e',41);
    PrintChar(0x39,'x',41);
    //delay(50);
 PrintChar(0x39,' ',41);
    PrintChar(0x39,' ',41);
    PrintChar(0x39,' ',41);
     PrintChar(0x39,' ',41);
   
     Serial.println(brzo_i2c_end_transaction());
  delay(50);
*/
if (!client.connected()) {
    reconnect();
  }
  client.loop();


}
