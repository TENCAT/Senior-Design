#include "WiFi.h"

WiFiServer server(7021);

const char* ssid = "ESP32APoint";
const char* password = "abcdefghijk";

//Function prototypes

void setup() {
 Serial.begin(115200);
  Serial.println();
  
  // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  WiFi.disconnect(true);
  WiFi.mode(WIFI_AP);

   if(WiFi.softAP(ssid, password)){
    Serial.println("Finalized the access point");
  }

  WiFi.begin("node2","wegotthisnow");
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
 
 
  /*WiFi.begin("America's WIFI","I<3yOu3thOusand");
  while(WiFi.status() != WL_CONNECTED){
    Serial.println("Not connected");
    delay(500);
  }*/

  IPAddress IP = WiFi.softAPIP();
  Serial.println("AP IP address: ");
  Serial.println(IP);
  Serial.println(WiFi.gatewayIP());
  Serial.println(WiFi.subnetMask());
  server.begin();
  
}

void loop() {
  WiFiClient client = server.available();
  if(client){
    while(client.connected()){
      delay(100);
      if(client.available()){
        char c = client.read();
        Serial.print(c);
        
      }
      client.print(c);
    }
  }
  client.stop();
}
