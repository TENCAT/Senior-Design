#include "WiFi.h"

WiFiServer server(7021);

const char* ssid = "node_2";
const char* password = "Senior Design";
char c;

//Function prototypes

void setup() {
  IPAddress local_IP(10,0,0,1);
  IPAddress local_sub(255,255,255,0);
 Serial.begin(115200);
  Serial.println();
  
  // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  WiFi.disconnect(true);
  WiFi.mode(WIFI_AP_STA);

  WiFi.softAPConfig(local_IP,local_IP,local_sub);

   if(WiFi.softAP(ssid, password)){
    Serial.println("Finalized the access point");
  }

  WiFi.begin("node_1",password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
  }

 

  IPAddress IP = WiFi.softAPIP();
  Serial.println("AP IP address: ");
  Serial.println(IP);
  Serial.println(WiFi.gatewayIP());
  Serial.println(WiFi.subnetMask());
  server.begin();
  
}

void loop() {
  WiFiClient sendData;
  if(!sendData.connect("192.168.4.1",7021)){
    return;
  }
  WiFiClient client = server.available();
  if(client){
    while(client.connected()){
      delay(100);
      if(client.available()){
        c = client.read();
        Serial.print(c);
         client.print(c);
         sendData.print(c);
      }
     
    }
  }
  client.stop();
}
