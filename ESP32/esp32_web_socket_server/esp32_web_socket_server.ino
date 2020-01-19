#include <ArduinoWebsockets.h>
#include <WiFi.h>

const char* ssid = "solomonbstoner"; //Enter SSID
const char* password = "password2"; //Enter Password

using namespace websockets;

WebsocketsServer server;
WebsocketsClient client;

void setup() {
  Serial.begin(115200);
  // Connect to wifi
  WiFi.begin(ssid, password);

  // Wait some time to connect to wifi
  while(WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(1000);
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());   //You can get IP address assigned to ESP

  server.listen(80);
  Serial.print("Is server live? ");
  Serial.println(server.available());

}



    

void loop() {
  if (!client.available()){
    client = server.accept();
    client.onMessage([&](WebsocketsMessage message) {
      Serial.print("Got Message: ");
      Serial.println(message.data());
    });
  }
  client.poll();
}
