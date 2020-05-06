#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <SensorsHeader.h> 

// Set the LCD address to 0x27 in PCF8574 by NXP and Set to 0x3F in PCF8574A by Ti
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  SensorsHeader.ScreenSetupFunction(); 
}

void loop() {
  SensorsHeader.SensorsHeader.SteeringFunction(); 
  SensorsHeader.TimeOfDayFunction(); 
  SensorsHeader.SpeedFunction(); 
  delay(100); 
  SensorsHeader.DistanceFunction(); 
  delay(10);
}
