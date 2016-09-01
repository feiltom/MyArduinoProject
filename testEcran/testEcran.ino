//#include "ESP8266WiFi.h" 

#if defined(ESP8266)
#include <pgmspace.h>
#else
#include <avr/pgmspace.h>
#endif
#include <Wire.h>  // This library is already built in to the Arduino IDE
#include <LiquidCrystal_I2C.h> //This library you can add via Include Library > Manage Library > 
#include <RtcDS3231.h>

 
LiquidCrystal_I2C lcd(0x27, 16, 2);
RtcDS3231 Rtc;
 
void setup()
{
  Serial.begin(115200);

  delay(100);
  Wire.begin(5, 4);
    Rtc.Begin();

  lcd.init();   // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight 

}
 
void loop()
{
//Serial.Writeln
  
    RtcDateTime now = Rtc.GetDateTime();

     lcd.setCursor(0, 0);
     lcd.print(now.Hour());lcd.print(":");lcd.print(now.Minute());
 // lcd.print("test "+now.Hour()); // Start Print text to Line 1
  lcd.setCursor(0, 1);      
  lcd.print("[ ]Set  [ ]Start"); // Start Print Test to Line 2
   lcd.setCursor(9, 1); lcd.print("X");  
  delay(5000);
  
  
}
