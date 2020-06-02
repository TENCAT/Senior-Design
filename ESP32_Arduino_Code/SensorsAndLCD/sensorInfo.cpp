#include "Arduino.h"
#include "LiquidCrystal_I2C.h"
#include "sensorInfo.h"

LiquidCrystal_I2C lcd
int trigPin = 18;
int echoPin = 19;
int button1 = 16;
int button2 = 17;
int Pressed = 0;
int NotPressed = 1;
int SteeringVarible;
int PhotoCell = 15;
int Speed = 0;


byte customChar[] = {
  B00000,
  B01110,
  B10001,
  B10001,
  B11111,
  B10101,
  B10101,
  B01110
};

byte customChar2[] = {
B01000,
  B00100,
  B01110,
  B01110,
  B10101,
  B11011,
  B10101,
  B01110
};

byte customChar3[] {
  B01110,
  B11011,
  B10001,
  B10001,
  B11011,
  B01110,
  B00100,
  B00100
};

byte sunChar[] = {
  B00000,
  B10101,
  B01110,
  B11111,
  B01110,
  B10101,
  B00000,
  B00000
};

byte moonChar[] = {
  B00000,
  B01000,
  B11000,
  B11000,
  B11100,
  B11111,
  B01110,
  B00000
};

void ScreenSetupFunction(LiquidCrystal_I2C lcd){
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);  
  
  Serial.begin(9600);
  
  // put your setup code here, to run once:
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, customChar);//Steering
  lcd.createChar(1, customChar2);//Fire
  lcd.createChar(2, customChar3); //Distance
  lcd.createChar(3, moonChar);
  lcd.createChar(4, sunChar);
  
  lcd.home();
  lcd.write(0);
  
  lcd.setCursor(3, 0); 
  lcd.write(1); 

  lcd.setCursor(9, 0); 
  lcd.write(2);
  lcd.print('hello');

  delay(10);

}

//Steering
void SteeringFunction(){
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);  
  
  Serial.begin(9600);
  
  // put your setup code here, to run once:
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, customChar);//Steering
  lcd.createChar(1, customChar2);//Fire
  lcd.createChar(2, customChar3); //Distance
  lcd.createChar(3, moonChar);
  lcd.createChar(4, sunChar);
  
  lcd.home();
  lcd.write(0);
  
  lcd.setCursor(3, 0); 
  lcd.write(1); 

  lcd.setCursor(9, 0); 
  lcd.write(2);

  delay(10);
  }

void TimeOfDayFunction(){
//Time of day
  lcd.setCursor(15, 0);
  int DayTime = analogRead(PhotoCell); 
  if (DayTime>1000) {
    lcd.write(4);
  } else {
    lcd.write(3);
  }
  }

void SpeedFunction(){
  //Gas and Break
  if (digitalRead(button1)==Pressed) {
    Speed = Speed+1;  
  }
  if (digitalRead(button2)==Pressed) {
    Speed = Speed-1;
  }
  //If speed becomes negative the LCD gets all funky when you go back to (+) integers 
  //to solve this you need to clear 
  lcd.setCursor(6, 0);
  lcd.print(" ");
  lcd.setCursor(7, 0);
  lcd.print(" ");

  //This sets cars speed limit
  if (Speed>99) {
    Speed = Speed-1;
  }
  if (Speed<-99) {
    Speed = Speed+1;
  }
  lcd.setCursor(4, 0);
  lcd.print(Speed);
  }

int DistanceFunction(){
  //Creates a pulse, which its duration will be used to calculate the distance
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);//This records how long the pulse was
  long distance = (duration*.0343)/2;//Here we convert the pulse time in cm

  //The screen needs to be reset so the values dont overlap
  lcd.setCursor(10, 0);
  lcd.print(" ");
  lcd.setCursor(11, 0);
  lcd.print(" ");
  lcd.setCursor(12, 0);
  lcd.print(" ");
  lcd.setCursor(13, 0);
  lcd.print(" ");
  
  lcd.setCursor(10, 0);
  lcd.print(distance);
  }