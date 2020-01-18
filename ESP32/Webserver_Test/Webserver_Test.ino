#include <WiFi.h>
WiFiServer server(80);

String addr_mac = String(WiFi.macAddress());

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  
  WiFi.softAP(String("ProNet-"+addr_mac).c_str());

  server.begin();
}

void loop() {}
