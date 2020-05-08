#include <sensorInfo.h>
#include "LiquidCrystal_I2C.h"

LiquidCrystal_I2C lcd(0x27, 16, 2); 

void setup() {
  ScreenSetupFunction(lcd); 
  delay(10);
  lcd.backlight();  
 
}

void loop() {
  
  SteeringFunction(lcd);
  
  TimeOfDayFunction(lcd);
  
  SpeedFunction(lcd);
  
  DistanceFunction(lcd);


}
