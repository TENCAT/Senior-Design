/* Darek Konopka
 *  Here I will be making a radar using 2 ultrasonic sensors
 *  A potentiameter as a steering wheel 
 *  photocell to tell time of day
 */

// Defines Tirg and Echo pins of the Ultrasonic Sensor
int trigPin = 10;
int echoPin = 11;
int button1 = 4; 
int button2 = 5; 
int Pressed = 0; 
int NotPressed = 1; 
int SteeringVarible; 

// Variables for the duration and the distance
long duration;
long distance;

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);  
  
  Serial.begin(9600);
 }
void ButtonFunction() {
  
  if (digitalRead(button1)==Pressed) {
  Serial.println("Button1 = Pressed");
 }else{
  Serial.println("Button1 = Not"); 
 }
 if (digitalRead(button2)==Pressed) {
  Serial.println("Button2 = Pressed");
 }else{
  Serial.println("Button2 = Not"); 
 }
}

void UltraSonicFunction() {
  //Displays results in cm
  //First we set up the UltraSonic Sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);//This records how long the pulse was
  distance = (duration*.0343)/2;//Here we convert the pulse time in cm
  Serial.print("Distance: ");
  Serial.println(distance);
  delay(50);
}

void PotFunction() {
  SteeringVarible = (analogRead(A0)-526); 
  if ((-10<SteeringVarible)&&(10>SteeringVarible)) {
    SteeringVarible = 0; 
  }
  Serial.println(SteeringVarible); 
}

void loop() {

 //ButtonFunction();
 //UltraSonicFunction(); 
 //PotFunction();  
 delay(10);
}
