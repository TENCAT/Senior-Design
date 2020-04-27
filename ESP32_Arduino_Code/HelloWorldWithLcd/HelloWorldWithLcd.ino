#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>

// Set the LCD address to 0x27 in PCF8574 by NXP and Set to 0x3F in PCF8574A by Ti
LiquidCrystal_I2C lcd(0x27, 20, 4);

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

const char* host = "192.168.0.18";
const uint16_t port = 8090;
int potValue=0;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, customChar);
  lcd.createChar(1, customChar2);
  lcd.createChar(2, customChar3);
  lcd.createChar(3, moonChar);
  lcd.createChar(4, sunChar);
  
  lcd.home();
  lcd.write(0);
  lcd.setCursor(19, 0);
  lcd.write(2);
  lcd.setCursor(19, 3); 
  lcd.write(1);

  lcd.setCursor(0, 3);
  lcd.write(3);
  lcd.setCursor(1, 3);
  lcd.write(4);
  
  lcd.setCursor(4, 0); 
  lcd.print("Hello World!"); 
  lcd.setCursor(3, 2);
  lcd.print("This is a demo");

  delay(2000);
  lcd.clear();

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        lcd.clear();
        lcd.print("Trying to connect");
    }
  lcd.clear();
  lcd.print("Connected");

  delay(2500);
}

void loop() {
  WiFiClient client;

  if(!client.connect(host,port)){
    //do nothing
    delay(1000);
    return;
  }

  client.print("Hello from ESP32!");
  client.stop();
  potValue=analogRead(34);
  lcd.home();
  lcd.print("Pot reading");
  lcd.setCursor(0,1);
  lcd.write(0);
  lcd.setCursor(1,1);
  
  if(potValue >= 2730){
    lcd.print("L");
  }
  else if(potValue <= 1365){
    lcd.print("R");
  }
  else{
    lcd.print("C");
  }
  delay(500);
}
