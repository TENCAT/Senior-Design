#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>

// Set the LCD address to 0x27 in PCF8574 by NXP and Set to 0x3F in PCF8574A by Ti
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Defines Tirg and Echo pins of the Ultrasonic Sensor
int trigPin = 18;
int echoPin = 19;
int button1 = 16; 
int button2 = 17; 
int Pressed = 0; 
int NotPressed = 1; 
int SteeringVarible;
int PhotoCell = 15; 
int Speed = 0; 

// Variables for the duration and the distance
long duration;
long distance;

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

byte clockChar[] = {
  B00000,
  B01110,
  B10011,
  B10101,
  B10001,
  B01110,
  B00000,
  B00000
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


void setup() {
  
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
  lcd.createChar(5, clockChar); //Clock
  
  lcd.home();
  lcd.write(0);
  lcd.setCursor(1, 0); 
  lcd.print(":");
  lcd.setCursor(1, 1); 
  lcd.print(":");
  lcd.setCursor(0,1); 
  lcd.write(5);

  lcd.setCursor(4, 0); 
  lcd.write(1); 
  lcd.setCursor(5, 0); 
  lcd.print(":");

  lcd.setCursor(4, 1); 
  lcd.write(2);
  lcd.setCursor(5, 1); 
  lcd.print(":");

  delay(10); 
}

void loop() {
  //Steering
  lcd.setCursor(2, 0);
  int potValue = (analogRead(34)-2040); 
  if ((0<potValue)&&(250>potValue)) {
    SteeringVarible = 0; 
  }
  if(potValue >= 2000){
    lcd.print("L");
  }
  else if(potValue <= -2000){
    lcd.print("R");
  }
  else{
    lcd.print("C");
  }

  //Time of day
  lcd.setCursor(2, 1);
  int DayTime = analogRead(PhotoCell); 
  if (DayTime>1000) {
    lcd.write(4);
  } else {
    lcd.write(3);
  }

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
  lcd.setCursor(8, 0);
  lcd.print(" ");
  lcd.setCursor(9, 0);
  lcd.print(" ");
  
  //This sets cars speed limit
  if (Speed>120) {
    Speed = Speed-1;
  }
  if (Speed<-99) {
    Speed = Speed+1;
  }
  lcd.setCursor(6, 0);
  lcd.print(Speed);

  //delay time before the ESP32 read the distance so we can see the measurements that will be displayed
  //All reading are in cm
  delay(100);
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);//This records how long the pulse was
  distance = (duration*.0343)/2;//Here we convert the pulse time in cm

  //The screen needs to be reset so the values dont overlap
  lcd.setCursor(6, 1);
  lcd.print(" ");
  lcd.setCursor(7, 1);
  lcd.print(" ");
  lcd.setCursor(8, 1);
  lcd.print(" ");
  lcd.setCursor(9, 1);
  lcd.print(" ");
  
  
  lcd.setCursor(6, 1);
  lcd.print(distance); 
  
  delay(10);
}
