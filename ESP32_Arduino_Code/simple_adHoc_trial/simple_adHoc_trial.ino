#include "WiFi.h"

WiFiServer server(7021);
IPAddress local(10,0,0,1);
IPAddress subnet(255,255,255,0);
const char* ssid = "Node_2";
const char* password = "Senior Design";
const char* ipconnect="192.168.4.1";
int port=7021;
  char c;
//Function prototypes

void setup() {
 Serial.begin(115200);
  Serial.println();
  
  // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  WiFi.disconnect(true);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local,local,subnet);

   if(WiFi.softAP(ssid, password)){
    Serial.println("Finalized the access point");
  }

  WiFi.begin("node_1","Senior Design");
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
  WiFiClient node1;
   if (!node1.connect(ipconnect, port)) {
        Serial.println("connection failed");
   }
  WiFiClient client = server.available();
  if(client){
    while(client.connected()){
      delay(100);
      if(client.available()){
        c = client.read();
        Serial.print(c);
        client.print(c);
        node1.print(c);
      }
      
      
    }
  }
  
  client.stop();
}
