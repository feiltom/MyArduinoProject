#include <ESP8266WiFi.h>          //ESP8266 Core WiFi Library (you most likely already have this in your sketch)

#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>  

void setup() {
  // initialize digital pin 13 as an output.
  pinMode(2, OUTPUT);
   Serial.begin(9600);
   WiFiManager wifiManager;
wifiManager.autoConnect();


}

// the loop function runs over and over again forever
void loop() {
    Serial.print("Button is ");

  digitalWrite(2, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  digitalWrite(2, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second
}

